<?php

/**
 * @file
 * Sitemap generation and rebuilding functions for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * Given an internal Drupal path, return the alias for the path.
 *
 * This is similar to drupal_get_path_alias(), but designed to fetch all alises
 * at once so that only one database query is executed instead of several or
 * possibly thousands during sitemap generation.
 *
 * @param $path
 *   An internal Drupal path.
 * @param $language
 *   A language code to use when looking up the paths.
 */
function xmlsitemap_get_path_alias($path, $language) {
  static $aliases;
  static $last_language;

  if (!isset($aliases)) {
    $aliases['all'] = array();
    $query = db_query("SELECT src, dst FROM {url_alias} WHERE language = '' ORDER BY pid");
    while ($alias = db_fetch_array($query)) {
      $aliases['all'][$alias['src']] = $alias['dst'];
    }
  }
  if ($language && $last_language != $language) {
    unset($aliases[$last_language]);
    $aliases[$language] = array();
    $query = db_query("SELECT src, dst FROM {url_alias} WHERE language = '%s' ORDER BY pid", $language);
    while ($alias = db_fetch_array($query)) {
      $aliases[$language][$alias['src']] = $alias['dst'];
    }
    $last_language = $language;
  }

  if ($language && isset($aliases[$language][$path])) {
    return $aliases[$language][$path];
  }
  elseif (isset($aliases['all'][$path])) {
    return $aliases['all'][$path];
  }
  else {
    return $path;
  }
}

/**
 * Perform operations before rebuilding the sitemap.
 */
function _xmlsitemap_regenerate_before() {
  // Attempt to increase the memory limit.
  _xmlsitemap_set_memory_limit();

  if (variable_get('xmlsitemap_developer_mode', 0)) {
    watchdog('xmlsitemap', 'Starting XML sitemap generation. Memory usage: @memory-peak.', array(
        '@memory-peak' => format_size(_xmlsitemap_memory_get_peak_usage()),
      ),
      WATCHDOG_DEBUG
    );
  }
}

/**
 * Wrapper function for memory_get_peak_usage() for PHP versions below 5.2.
 */
function _xmlsitemap_memory_get_peak_usage() {
  if (function_exists('memory_get_peak_usage')) {
    return memory_get_peak_usage(TRUE);
  }
  else {
    return 'N/A';
  }
}

function _xmlsitemap_get_memory_usage($start = FALSE) {
  static $memory_start;
  $current = 0;
  if (function_exists('memory_get_peak_usage')) {
    $current = memory_get_peak_usage(TRUE);
  }
  if (function_exists('memory_get_usage')) {
    $current = version_compare(PHP_VERSION, '5.2') ? memory_get_usage(TRUE) : memory_get_usage();
  }
  if (!isset($memory_start) || $start) {
    $memory_start = $current;
  }
  return $current - $memory_start;
}

/**
 * Calculate the optimal PHP memory limit for sitemap generation.
 *
 * This function just makes a guess. It does not take into account
 * the currently loaded modules.
 */
function _xmlsitemap_get_optimal_memory_limit() {
  $optimal_limit = &xmlsitemap_static(__FUNCTION__);
  if (!isset($optimal_limit)) {
    // Set the base memory amount from the provided core constant.
    $optimal_limit = parse_size(DRUPAL_MINIMUM_PHP_MEMORY_LIMIT);

    // Add memory based on the chunk size.
    $optimal_limit += xmlsitemap_get_chunk_size() * 500;

    // Add memory for storing the url aliases.
    if (variable_get('xmlsitemap_prefetch_aliases', 1)) {
      $aliases = db_result(db_query("SELECT COUNT(pid) FROM {url_alias}"));
      $optimal_limit += $aliases * 250;
    }
  }
  return $optimal_limit;
}

/**
 * Calculate the optimal memory level for sitemap generation.
 *
 * @param $new_limit
 *   An optional PHP memory limit in bytes. If not provided, the value of
 *   _xmlsitemap_get_optimal_memory_limit() will be used.
 */
function _xmlsitemap_set_memory_limit($new_limit = NULL) {
  $current_limit = @ini_get('memory_limit');
  if ($current_limit && $current_limit != -1) {
    if (!is_null($new_limit)) {
      $new_limit = _xmlsitemap_get_optimal_memory_limit();
    }
    if (parse_size($current_limit) < $new_limit) {
      return @ini_set('memory_limit', $new_limit);
    }
  }
}

/**
 * Generate one page (chunk) of the sitemap.
 *
 * @param $sitemap
 *   An unserialized data array for an XML sitemap.
 * @param $page
 *   An integer of the specific page of the sitemap to generate.
 */
function xmlsitemap_generate_page(stdClass $sitemap, $page) {
  xmlsitemap_load_all_includes();

  try {
    $writer = new XMLSitemapWriter($sitemap, $page);
    $writer->startDocument();
    $writer->generateXML();
    $writer->endDocument();
  }
  catch (Exception $e) {
    trigger_error($e->getMessage(), E_USER_ERROR);
    throw $e;
    return FALSE;
  }

  return $writer->getSitemapElementCount();
}

function xmlsitemap_generate_chunk(stdClass $sitemap, XMLSitemapWriter $writer, $chunk) {
  $lastmod_format = variable_get('xmlsitemap_lastmod_format', XMLSITEMAP_LASTMOD_MEDIUM);

  $url_options = $sitemap->uri['options'];
  $url_options += array(
    'absolute' => TRUE,
    'base_url' => variable_get('xmlsitemap_base_url', $GLOBALS['base_url']),
    'language' => language_default(),
    'alias' => variable_get('xmlsitemap_prefetch_aliases', TRUE),
  );

  $last_url = '';
  $link_count = 0;

  $query = array(
    'SELECT'   => 'SELECT x.loc, x.lastmod, x.changefreq, x.changecount, x.priority, x.language',
    'FROM'     => 'FROM {xmlsitemap} x',
    'WHERE'    => 'WHERE x.access = 1 AND x.status = 1',
    'ORDER BY' => 'ORDER BY x.language, x.loc',
  );
  $args = array();

  // Allow other modules to alter the sitemap query SQL and arguments.
  static $alter;
  if (!isset($alter)) {
    // Skip altering if there are no modules to invoke.
    xmlsitemap_load_all_includes();
    $alter = (bool) module_implements('query_xmlsitemap_generate_alter');
  }
  if ($alter) {
    $data = &$query;
    $data['__drupal_alter_by_ref'] = array(&$args);
    drupal_alter('query_xmlsitemap_generate', $data, $sitemap);
  }

  $sql = implode($query, ' ');
  $offset = max($chunk - 1, 0) * xmlsitemap_get_chunk_size();
  $limit = xmlsitemap_get_chunk_size();
  $query = db_query_range($sql, $args, $offset, $limit);

  while ($link = db_fetch_array($query)) {
    $link['language'] = $link['language'] ? xmlsitemap_language_load($link['language']) : $url_options['language'];
    if ($url_options['alias']) {
      $link['loc'] = xmlsitemap_get_path_alias($link['loc'], $link['language']->language);
    }
    $link_options = array(
      'language' => $link['language'],
      'xmlsitemap_link' => $link,
      'xmlsitemap_sitemap' => $sitemap,
    );
    // @todo Add a separate hook_xmlsitemap_link_url_alter() here?
    $link_url = url($link['loc'], $link_options + $url_options);

    // Skip this link if it was a duplicate of the last one.
    // @todo Figure out a way to do this before generation so we can report
    // back to the user about this.
    if ($link_url == $last_url) {
      continue;
    }
    else {
      $last_url = $link_url;
      // Keep track of the total number of links written.
      $link_count++;
    }

    $element = array();
    $element['loc'] = $link_url;
    if ($link['lastmod']) {
      $element['lastmod'] = gmdate($lastmod_format, $link['lastmod']);
      // If the link has a lastmod value, update the changefreq so that links
      // with a short changefreq but updated two years ago show decay.
      // We use abs() here just incase items were created on this same cron run
      // because lastmod would be greater than REQUEST_TIME.
      $link['changefreq'] = (abs(REQUEST_TIME - $link['lastmod']) + $link['changefreq']) / 2;
    }
    if ($link['changefreq']) {
      $element['changefreq'] = xmlsitemap_get_changefreq($link['changefreq']);
    }
    if (isset($link['priority']) && $link['priority'] != 0.5) {
      // Don't output the priority value for links that have 0.5 priority. This
      // is the default 'assumed' value if priority is not included as per the
      // sitemaps.org specification.
      $element['priority'] = number_format($link['priority'], 1);
    }
    $writer->writeSitemapElement('url', $element);
  }

  return $link_count;
}

/**
 * Generate the index sitemap.
 *
 * @param $sitemap
 *   An unserialized data array for an XML sitemap.
 */
function xmlsitemap_generate_index(stdClass $sitemap) {
  xmlsitemap_load_all_includes();

  try {
    $writer = new XMLSitemapIndexWriter($sitemap);
    $writer->startDocument();
    $writer->generateXML();
    $writer->endDocument();
  }
  catch (Exception $e) {
    watchdog_exception('xmlsitemap', $e);
    throw $e;
    return FALSE;
  }

  return $writer->getSitemapElementCount();
}

// BATCH OPERATIONS ------------------------------------------------------------

/**
 * Batch information callback for regenerating the sitemap files.
 *
 * @param $smids
 *   An optional array of XML sitemap IDs. If not provided, it will load all
 *   existing XML sitemaps.
 */
function xmlsitemap_regenerate_batch(array $smids = array()) {
  if (empty($smids)) {
    $smids = xmlsitemap_db_fetch_col(db_query("SELECT smid FROM {xmlsitemap_sitemap}"));
  }

  //$t = get_t();
  $batch = array(
    'operations' => array(),
    //'error_message' => $t('An error has occurred.'),
    'finished' => 'xmlsitemap_regenerate_batch_finished',
    'title' => t('Regenerating Sitemap'),
    'file' => drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.generate.inc',
  );

  // Set the regenerate flag in case something fails during file generation.
  $batch['operations'][] = array('xmlsitemap_batch_variable_set', array(array('xmlsitemap_regenerate_needed' => TRUE)));
  $batch['operations'][] = array('xmlsitemap_batch_timer_start', array());

  // @todo Get rid of this batch operation.
  $batch['operations'][] = array('_xmlsitemap_regenerate_before', array());

  // Generate all the sitemap pages for each context.
  foreach ($smids as $smid) {
    $batch['operations'][] = array('xmlsitemap_regenerate_batch_generate', array($smid));
    $batch['operations'][] = array('xmlsitemap_regenerate_batch_generate_index', array($smid));
  }

  // Clear the regeneration flag.
  $batch['operations'][] = array('xmlsitemap_batch_variable_set', array(array('xmlsitemap_regenerate_needed' => FALSE)));

  return $batch;
}

/**
 * Batch callback; generate all pages of a sitemap.
 */
function xmlsitemap_regenerate_batch_generate($smid, array &$context) {
  if (!isset($context['sandbox']['sitemap'])) {
    $context['sandbox']['sitemap'] = xmlsitemap_sitemap_load($smid);
    $context['sandbox']['sitemap']->chunks = 1;
    $context['sandbox']['sitemap']->links = 0;
    $context['sandbox']['max'] = XMLSITEMAP_MAX_SITEMAP_LINKS;

    // Clear the cache directory for this sitemap before generating any files.
    xmlsitemap_check_directory($context['sandbox']['sitemap']);
    xmlsitemap_clear_directory($context['sandbox']['sitemap']);
  }

  $sitemap = &$context['sandbox']['sitemap'];
  $links = xmlsitemap_generate_page($sitemap, $sitemap->chunks);
  $context['message'] = t('Now generating %sitemap-url.', array('%sitemap-url' => url('sitemap.xml', $sitemap->uri['options'] + array('query' => array('page' => $sitemap->chunks)))));

  if ($links) {
    $sitemap->links += $links;
    $sitemap->chunks++;
  }
  else {
    // Cleanup the 'extra' empty file.
    $file = xmlsitemap_sitemap_get_file($sitemap, $sitemap->chunks);
    if (file_exists($file) && $sitemap->chunks > 1) {
      file_delete($file);
    }
    $sitemap->chunks--;

    // Save the updated chunks and links values.
    $context['sandbox']['max'] = $sitemap->chunks;
    $sitemap->updated = REQUEST_TIME;
    xmlsitemap_sitemap_get_max_filesize($sitemap);
    xmlsitemap_sitemap_save($sitemap);
  }

  if ($sitemap->chunks != $context['sandbox']['max']) {
    $context['finished'] = $sitemap->chunks / $context['sandbox']['max'];
  }
}

/**
 * Batch callback; generate the index page of a sitemap.
 */
function xmlsitemap_regenerate_batch_generate_index($smid, array &$context) {
  $sitemap = xmlsitemap_sitemap_load($smid);
  if ($sitemap->chunks > 1) {
    xmlsitemap_generate_index($sitemap);
    $context['message'] = t('Now generating sitemap index %sitemap-url.', array('%sitemap-url' => url('sitemap.xml', $sitemap->uri['options'])));
  }
}

/**
 * Batch callback; sitemap regeneration finished.
 */
function xmlsitemap_regenerate_batch_finished($success, $results, $operations) {
  $results['elapsed'] = round((microtime(TRUE) - $results['start']) * 1000, 2);
  $elapsed = format_interval($results['elapsed'] / 1000);

  if ($success && !variable_get('xmlsitemap_regenerate_needed', FALSE)) {
    variable_set('xmlsitemap_generated_last', REQUEST_TIME);
    //drupal_set_message(t('The sitemaps were regenerated.'));
    // Show a watchdog message that the sitemap was regenerated.
    watchdog('xmlsitemap',
      'Finished XML sitemap generation in @elapsed. Memory usage: @memory-peak.',
      array(
        '@elapsed' => $elapsed,
        '@memory-peak' => format_size(_xmlsitemap_memory_get_peak_usage()),
      ),
      WATCHDOG_NOTICE
    );
  }
  else {
    drupal_set_message(t('The sitemaps were not successfully regenerated.'), 'error');
  }
}

/**
 * Batch information callback for rebuilding the sitemap data.
 */
function xmlsitemap_rebuild_batch(array $entities, $save_custom = FALSE) {
  $batch = array(
    'operations' => array(),
    'finished' => 'xmlsitemap_rebuild_batch_finished',
    'title' => t('Rebuilding Sitemap'),
    'file' => drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.generate.inc',
  );

  // Set the rebuild flag in case something fails during the rebuild.
  $batch['operations'][] = array('xmlsitemap_batch_variable_set', array(array('xmlsitemap_rebuild_needed' => TRUE)));
  $batch['operations'][] = array('xmlsitemap_batch_timer_start', array());

  // Purge any links first.
  $batch['operations'][] = array('xmlsitemap_rebuild_batch_clear', array($entities, (bool) $save_custom));

  // Fetch all the sitemap links and save them to the {xmlsitemap} table.
  foreach ($entities as $entity) {
    $info = xmlsitemap_get_link_info($entity);
    $batch['operations'][] = array($info['xmlsitemap']['rebuild callback'], array($entity));
  }

  // Clear the rebuild flag.
  $batch['operations'][] = array('xmlsitemap_batch_variable_set', array(array('xmlsitemap_rebuild_needed' => FALSE)));

  // Add the regeneration batch.
  $regenerate_batch = xmlsitemap_regenerate_batch();
  $batch['operations'] = array_merge($batch['operations'], $regenerate_batch['operations']);

  return $batch;
}

/**
 * Batch callback; start the timer.
 */
function xmlsitemap_batch_timer_start(array &$context) {
  if (!isset($context['results']['start'])) {
    $context['results']['start'] = microtime(TRUE);
  }
}

/**
 * Batch callback; set an array of variables and their values.
 */
function xmlsitemap_batch_variable_set(array $variables) {
  foreach ($variables as $variable => $value) {
    variable_set($variable, $value);
  }
}

/**
 * Batch callback; clear sitemap links for entites.
 */
function xmlsitemap_rebuild_batch_clear(array $entities, $save_custom, &$context) {
  if (!empty($entities)) {
    $sql = "DELETE FROM {xmlsitemap} WHERE type IN (" . db_placeholders($entities, 'varchar') . ')';

    // If we want to save the custom data, make sure to exclude any links
    // that are not using default inclusion or priority.
    if ($save_custom) {
      $sql .= ' AND status_override = 0 AND priority_override = 0';
    }

    db_query($sql, $entities);
  }

  $context['message'] = t('Purging links.');
}

/**
 * Batch callback; fetch and add the sitemap links for a specific entity.
 */
function xmlsitemap_rebuild_batch_fetch($entity, &$context) {
  if (!isset($context['sandbox']['info'])) {
    $context['sandbox']['info'] = xmlsitemap_get_link_info($entity);
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['last_id'] = 0;
  }
  $info = $context['sandbox']['info'];

  // Build the generic query.
  $base_table = db_escape_table($info['base table']);
  $id_key = db_escape_string($info['entity keys']['id']);
  $query = $args = $ids = array();
  $query['SELECT'] = "SELECT $id_key";
  $query['FROM']   = "FROM {{$base_table}}";
  $query['WHERE']  = "WHERE $id_key > %d";
  $args[] = $context['sandbox']['last_id'];

  if (!empty($info['entity keys']['bundle'])) {
    $bundle_key = db_escape_string($info['entity keys']['bundle']);
    $bundle_type = _xmlsitemap_get_field_type($info['base table'], $info['entity keys']['bundle']);
    $bundles = xmlsitemap_get_link_type_enabled_bundles($entity);
    $query['WHERE'] .= " AND $bundle_key IN (" . db_placeholders($bundles, $bundle_type) . ")";
    $args = array_merge($args, $bundles);
  }

  if (!isset($context['sandbox']['max'])) {
    $sql = implode(' ', $query);
    $sql = str_replace("SELECT $id_key", "SELECT COUNT($id_key)", $sql);
    $context['sandbox']['max'] = (int) db_result(db_query($sql, $args));
    if (!$context['sandbox']['max']) {
      // If there are no items to process, skip everything else.
      return;
    }
  }

  // PostgreSQL cannot have the ORDERED BY in the count query.
  $query['ORDER BY'] = "ORDER BY $id_key";

  $limit = 20; //variable_get('xmlsitemap_batch_limit', 100)
  $sql = implode(' ', $query);
  $query = db_query_range($sql, $args, 0, $limit);

  $ids = xmlsitemap_db_fetch_col($query);
  $info['xmlsitemap']['process callback']($ids);
  $context['sandbox']['last_id'] = end($ids);
  $context['sandbox']['progress'] += count($ids);
  $context['message'] = t('Now processing %entity @last_id (@progress of @count).', array('%entity' => $entity, '@last_id' => $context['sandbox']['last_id'], '@progress' => $context['sandbox']['progress'], '@count' => $context['sandbox']['max']));

  if ($context['sandbox']['progress'] >= $context['sandbox']['max']) {
    $context['finished'] = 1;
  }
  else {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch callback; sitemap rebuild finished.
 */
function xmlsitemap_rebuild_batch_finished($success, $results, $operations) {
  if ($success && !variable_get('xmlsitemap_rebuild_needed', FALSE)) {
    // Reset the rebuild flag since it was successful.
    variable_set('xmlsitemap_rebuild_needed', FALSE);
    drupal_set_message(t('The sitemap links were rebuilt.'));
  }
  else {
    drupal_set_message(t('The sitemap links were not successfully rebuilt.'), 'error');
  }
}

function xmlsitemap_get_rebuildable_link_types() {
  $rebuild_types = array();
  $entities = xmlsitemap_get_link_info();

  foreach ($entities as $entity => $info) {
    if (empty($info['xmlsitemap']['rebuild callback'])) {
      // If the entity is missing a rebuild callback, skip.
      continue;
    }
    if (!empty($info['entity keys']['bundle']) && !xmlsitemap_get_link_type_enabled_bundles($entity)) {
      // If the entity has bundles, but no enabled bundles, skip since
      // rebuilding wouldn't get any links.
      continue;
    }
    else {
      $rebuild_types[] = $entity;
    }
  }

  return $rebuild_types;
}
