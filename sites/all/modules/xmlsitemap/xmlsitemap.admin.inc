<?php

/**
 * @file
 * Administrative page callbacks for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

function xmlsitemap_sitemap_list_form_local_actions() {
  $local_actions['add'] = xmlsitemap_get_operation_link('admin/settings/xmlsitemap/add', array('modal' => TRUE));
  return $local_actions;
}

/**
 * Render a tableselect list of XML sitemaps for the main admin page.
 */
function xmlsitemap_sitemap_list_form() {
  xmlsitemap_load_all_includes();
  $destination = drupal_get_destination();

  // Add the local actions.
  $form['actions'] = array(
    '#type' => 'markup',
    '#value' => theme('links', xmlsitemap_sitemap_list_form_local_actions(), array('class' => 'item-list action-links')),
  );

  // Build the 'Update options' form.
  $form['#operations'] = module_invoke_all('xmlsitemap_sitemap_operations');
  $operations = array();
  foreach ($form['#operations'] as $operation => $operation_info) {
    $operations[$operation] = $operation_info['label'];
  }
  asort($operations);

  $form['operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#access' => module_exists('elements'),
  );
  $form['operations']['operation'] = array(
    '#type' => 'select',
    '#options' => $operations,
    '#default_value' => 'update',
  );
  $form['operations']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    //'#validate' => array('xmlsitemap_sitemap_list_form_validate'),
    //'#submit' => array('xmlsitemap_sitemap_list_form_submit'),
  );

  $contexts = xmlsitemap_get_context_info();

  $header = array();
  $header['url'] = array('data' => t('URL'));
  foreach ($contexts as $context_key => $context_info) {
    if (!empty($context_info['summary callback'])) {
      $header['context_' . $context_key] = $context_info['label'];
    }
  }
  $header['updated'] = array('data' => t('Last updated'), 'field' => 'updated', 'sort' => 'asc');
  $header['links'] = array('data' => t('Links'), 'field' => 'links');
  $header['chunks'] = array('data' => t('Pages'), 'field' => 'chunks');
  $header['operations'] = array('data' => t('Operations'));

  $query = db_query("SELECT smid FROM {xmlsitemap_sitemap}" . tablesort_sql($header));
  $smids = xmlsitemap_db_fetch_col($query);
  $sitemaps = !empty($smids) ? xmlsitemap_sitemap_load_multiple($smids) : array();

  $options = array();
  foreach ($sitemaps as $smid => $sitemap) {
    $sitemap->url = url($sitemap->uri['path'], $sitemap->uri['options']);

    $options[$smid]['url'] = array(
      'data' => l($sitemap->url, $sitemap->url),
    );

    foreach ($contexts as $context_key => $context_info) {
      if (!empty($context_info['summary callback'])) {
        $options[$smid]['context_' . $context_key] = _xmlsitemap_sitemap_context_summary($sitemap, $context_key, $context_info);
      }
    }

    $options[$smid]['updated'] = $sitemap->updated ? format_date($sitemap->updated, 'short') : t('Never');
    $options[$smid]['links'] = $sitemap->updated ? $sitemap->links : '-';
    $options[$smid]['chunks'] = $sitemap->updated ? $sitemap->chunks : '-';

    // @todo Highlight sitemaps that need updating.
    //$options[$smid]['#attributes']['class'] = 'warning';

    $operations = array();
    $operations['edit'] = xmlsitemap_get_operation_link('admin/settings/xmlsitemap/edit/' . $smid, array('title' => t('Edit'), 'modal' => TRUE));
    $operations['delete'] = xmlsitemap_get_operation_link('admin/settings/xmlsitemap/delete/' . $smid, array('title' => t('Delete'), 'modal' => TRUE));
    if ($operations) {
      $options[$smid]['operations'] = array(
        'data' => theme('links', $operations, array('class' => 'links inline')),
      );
    }
    else {
      $options[$smid]['operations'] = t('None (sitemap locked)');
    }
  }

  if (module_exists('elements')) {
    $form['sitemaps'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No XML sitemaps available.') . ' ' . l('Add a new XML sitemap', 'admin/settings/xmlsitemap/add'),
    );
  }
  else {
    if (empty($options)) {
      $options[] = array(array(
        'data' => t('No XML sitemaps available.') . ' ' . l('Add a new XML sitemap', 'admin/settings/xmlsitemap/add'),
        'colspan' => count($header),
        'class' => 'empty',
      ));
    }
    $form['sitemaps'] = array(
      '#type' => 'markup',
      '#value' => theme('table', $header, $options),
    );
  }

  return $form;
}

/**
 * Validate xmlsitemap_sitemap_list_form submissions.
 */
function xmlsitemap_sitemap_list_form_validate($form, &$form_state) {
  $form_state['values']['sitemaps'] = array_filter($form_state['values']['sitemaps']);

  // Error if there are no items to select.
  if (!count($form_state['values']['sitemaps'])) {
    form_set_error('', t('No sitemaps selected.'));
  }
}

/**
 * Process xmlsitemap_sitemap_list_form submissions.
 *
 * Execute the chosen 'Update option' on the selected sitemaps.
 */
function xmlsitemap_sitemap_list_form_submit($form, &$form_state) {
  $operation = $form['#operations'][$form_state['values']['operation']];

  // Filter out unchecked sitemaps.
  $sitemaps = array_filter($form_state['values']['sitemaps']);

  if (!empty($operation['confirm']) && empty($form_state['values']['confirm'])) {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of redirects.
    $form_state['rebuild'] = TRUE;
  }
  else {
    $function = $operation['callback'];

    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($sitemaps), $operation['callback arguments']);
    }
    else {
      $args = array($sitemaps);
    }
    call_user_func_array($function, $args);

    $count = count($form_state['values']['sitemaps']);
    //watchdog('xmlsitemap', '@action @count XML sitemaps.', array('@action' => $operation['action past'], '@count' => $count));
    drupal_set_message(format_plural(count($sitemaps), '@action @count XML sitemap.', '@action @count XML sitemaps.', array('@action' => $operation['action past'], '@count' => $count)));
    //$form_state['redirect'] = 'admin/settings/xmlsitemap';
  }
}

function xmlsitemap_sitemap_edit_form(array &$form_state, stdClass $sitemap = NULL) {
  _xmlsitemap_set_breadcrumb();

  if (!isset($sitemap)) {
    $sitemap = new stdClass();
    $sitemap->smid = NULL;
    $sitemap->context = array();
  }

  $form['#sitemap'] = $sitemap;

  $form['smid'] = array(
    '#type' => 'value',
    '#value' => $sitemap->smid,
  );
  // The context settings should be form_alter'ed by the context modules.
  $form['context'] = array(
    '#tree' => TRUE,
  );

  $form['actions'] = array(
    '#weight' => 100,
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'markup',
    '#value' => l(t('Cancel'), isset($_GET['destination']) ? $_GET['destination'] : 'admin/settings/xmlsitemap'),
  );

  // Let other modules alter this form with their context settings.
  $form['#pre_render'][] = 'xmlsitemap_sitemap_edit_form_pre_render';

  return $form;
}

function xmlsitemap_sitemap_edit_form_pre_render($form) {
  $visible_children = xmlsitemap_element_get_visible_children($form['context']);
  if (empty($visible_children)) {
    $form['context']['empty'] = array(
      '#type' => 'markup',
      '#value' => '<p>' . t('There are currently no XML sitemap contexts available.') . '</p>',
    );
  }
  return $form;
}

function xmlsitemap_sitemap_edit_form_validate($form, &$form_state) {
  // If there are no context options, the $form_state['values']['context']
  // disappears.
  $form_state['values'] += array('context' => array());
  $existing = xmlsitemap_sitemap_load_by_context($form_state['values']['context']);
  if ($existing && $existing->smid != $form_state['values']['smid']) {
    form_set_error('context', t('A sitemap with the same context already exists.'));
  }
}

function xmlsitemap_sitemap_edit_form_submit($form, &$form_state) {
  $sitemap = (object) $form_state['values'];
  xmlsitemap_sitemap_save($sitemap);
  drupal_set_message(t('The sitemap has been saved.'));
  $form_state['redirect'] = 'admin/settings/xmlsitemap';
}

function xmlsitemap_sitemap_delete_form(array &$form_state, stdClass $sitemap) {
  _xmlsitemap_set_breadcrumb();

  $count = (int) db_result(db_query("SELECT COUNT(smid) FROM {xmlsitemap_sitemap}"));
  if ($count === 1 && empty($_POST)) {
    drupal_set_message(t('It is not recommended to delete the only XML sitemap.'), 'error');
  }

  $form['#sitemap'] = $sitemap;
  $form['smid'] = array(
    '#type' => 'value',
    '#value' => $sitemap->smid,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete the XML sitemap?'),
    'admin/settings/xmlsitemap',
    '',
    t('Delete'),
    t('Cancel')
  );
}

function xmlsitemap_sitemap_delete_form_submit($form, $form_state) {
  xmlsitemap_sitemap_delete($form_state['values']['smid']);
  drupal_set_message(t('The sitemap has been deleted.'));
  $form_state['redirect'] = 'admin/settings/xmlsitemap';
}

/**
 * Form builder; Administration settings form.
 *
 * @see system_settings_form()
 * @see xmlsitemap_settings_form_validate()
 */
function xmlsitemap_settings_form() {
  $form['xmlsitemap_minimum_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Minimum sitemap lifetime'),
    '#options' => array(0 => t('No minimum')) + drupal_map_assoc(array(300, 900, 1800, 3600, 10800, 21600, 43200, 86400, 172800, 259200, 604800), 'format_interval'),
    '#description' => t('The minimum amount of time that will elapse before the sitemaps are regenerated. The sitemaps will also only be regenerated on cron if any links have been added, updated, or deleted.') . '<br />' . t('Recommended value: %value.', array('%value' => t('1 day'))),
    '#default_value' => variable_get('xmlsitemap_minimum_lifetime', 0),
  );
  $form['xmlsitemap_xsl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include a stylesheet in the sitemaps for humans.'),
    '#description' => t('When enabled, this will add formatting and tables with sorting to make it easier to view the XML sitemap data instead of viewing raw XML output. Search engines will ignore this.'),
    '#default_value' => variable_get('xmlsitemap_xsl', 1),
  );
  $form['xmlsitemap_prefetch_aliases'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefetch URL aliases during sitemap generation.'),
    '#description' => t('When enabled, this will fetch all URL aliases at once instead of one at a time during sitemap generation. For medium or large sites, it is recommended to disable this feature as it uses a lot of memory.'),
    '#default_value' => variable_get('xmlsitemap_prefetch_aliases', 1),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('xmlsitemap_developer_mode', 0),
    '#weight' => 10,
    '#group' => FALSE,
  );
  //$form['advanced']['xmlsitemap_gz'] = array(
  //  '#type' => 'checkbox',
  //  '#title' => t('Generate additional compressed sitemaps using gzip.'),
  //  '#default_value' => xmlsitemap_var('gz'),
  //  '#disabled' => !function_exists('gzencode'),
  //);
  $form['advanced']['xmlsitemap_chunk_size'] = array(
    '#type' => 'select',
    '#title' => t('Number of links in each sitemap page'),
    '#options' => array('auto' => t('Automatic (recommended)')) + drupal_map_assoc(array(100, 500, 1000, 2500, 5000, 10000, 25000, XMLSITEMAP_MAX_SITEMAP_LINKS)),
    '#default_value' => xmlsitemap_var('chunk_size'),
    // @todo This description is not clear.
    '#description' => t('If there are problems with rebuilding the sitemap, you may want to manually set this value. If you have more than @max links, an index with multiple sitemap pages will be generated. There is a maximum of @max sitemap pages.', array('@max' => XMLSITEMAP_MAX_SITEMAP_LINKS)),
  );
  $form['advanced']['xmlsitemap_batch_limit'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of sitemap links to process at once'),
    '#options' => drupal_map_assoc(array(5, 10, 25, 50, 100, 250, 500, 1000, 2500, 5000)),
    '#default_value' => xmlsitemap_var('batch_limit'),
    '#description' => t('If you have problems running cron or rebuilding the sitemap, you may want to lower this value.'),
  );
  $directory = xmlsitemap_get_directory();
  file_check_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS, 'xmlsitemap_path');
  $form['advanced']['xmlsitemap_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Sitemap cache directory'),
    '#default_value' => variable_get('xmlsitemap_path', 'xmlsitemap'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('Subdirectory where the sitemap data will be stored. This folder <strong>must not be shared</strong> with any other Drupal site or install using XML sitemap.'),
    '#field_prefix' =>  file_directory_path() . '/',
    '#required' => TRUE,
  );
  $form['advanced']['xmlsitemap_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Default base URL'),
    '#default_value' => variable_get('xmlsitemap_base_url', $GLOBALS['base_url']),
    '#size' => 30,
    '#description' => t('This is the default base URL used for sitemaps and sitemap links.'),
    '#required' => TRUE,
  );
  $form['advanced']['xmlsitemap_lastmod_format'] = array(
    '#type' => 'select',
    '#title' => t('Last modification date format'),
    '#options' => array(
      XMLSITEMAP_LASTMOD_SHORT => t('Short'),
      XMLSITEMAP_LASTMOD_MEDIUM => t('Medium'),
      XMLSITEMAP_LASTMOD_LONG => t('Long'),
    ),
    '#default_value' => variable_get('xmlsitemap_lastmod_format', XMLSITEMAP_LASTMOD_MEDIUM),
  );
  foreach ($form['advanced']['xmlsitemap_lastmod_format']['#options'] as $key => &$label) {
    $label .= ' (' . gmdate($key, REQUEST_TIME) . ')';
  }
  $form['advanced']['xmlsitemap_developer_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable developer mode to expose additional settings.'),
    '#default_value' => variable_get('xmlsitemap_developer_mode', 0),
  );

  $entities = xmlsitemap_get_link_info(NULL, TRUE);
  module_load_all_includes('inc', 'xmlsitemap');
  foreach ($entities as $entity => $entity_info) {
    $form[$entity] = array(
      '#type' => 'fieldset',
      '#title' => $entity_info['label'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'xmlsitemap_settings',
      '#weight' => 20,
    );

    if (!empty($entity_info['bundles'])) {
      // If this entity has bundles, show a bundle setting summary.
      xmlsitemap_add_form_entity_summary($form[$entity], $entity, $entity_info);
    }

    if (!empty($entity_info['xmlsitemap']['settings callback'])) {
      // Add any entity-specific settings.
      $entity_info['xmlsitemap']['settings callback']($form[$entity]);
    }

    // Ensure that the entity fieldset is not shown if there are no accessible
    // sub-elements.
    $form[$entity]['#access'] = (bool) xmlsitemap_element_get_visible_children($form[$entity]);
  }

  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';
  $form['#validate'][] = 'xmlsitemap_settings_form_validate';
  $form['#submit'][] = 'xmlsitemap_settings_form_submit';
  $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);

  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 100;
  return $form;
}

/**
 * Form validator; Check the sitemap files directory.
 *
 * @see xmlsitemap_settings_form()
 */
function xmlsitemap_settings_form_validate($form, &$form_state) {
  // Check that the chunk size will not create more than 1000 chunks.
  $chunk_size = $form_state['values']['xmlsitemap_chunk_size'];
  if ($chunk_size != 'auto' && $chunk_size != 50000 && (xmlsitemap_get_link_count() / $chunk_size) > 1000) {
    form_set_error('xmlsitemap_chunk_size', t('The sitemap page link count of @size will create more than 1,000 sitemap pages. Please increase the link count.', array('@size' => $chunk_size)));
  }

  $base_url = &$form_state['values']['xmlsitemap_base_url'];
  $base_url = rtrim($base_url, '/');
  if ($base_url != '' && !valid_url($base_url, TRUE)) {
    form_set_error('xmlsitemap_base_url', t('Invalid base URL.'));
  }
}

/**
 * Submit handler;
 *
 * @see xmlsitemap_settings_form()
 */
function xmlsitemap_settings_form_submit($form, $form_state) {
  // Save any changes to the frontpage link.
  xmlsitemap_link_save(array('type' => 'frontpage', 'id' => 0, 'loc' => ''));
}

/**
 * Menu callback; Confirm rebuilding of the sitemap.
 *
 * @see xmlsitemap_rebuild_form_submit()
 */
function xmlsitemap_rebuild_form() {
  if (!$_POST && !variable_get('xmlsitemap_rebuild_needed', FALSE)) {
    if (!variable_get('xmlsitemap_regenerate_needed', FALSE)) {
      drupal_set_message(t('Your sitemap is up to date and does not need to be rebuilt.'), 'error');
    }
    else {
      $_REQUEST += array('destination' => 'admin/settings/xmlsitemap');
      drupal_set_message(t('A rebuild is not necessary. If you are just wanting to regenerate the XML sitemap files, you can <a href="@link-cron">run cron manually</a>.', array('@link-cron' => url('admin/reports/status/run-cron', array('query' => drupal_get_destination())))), 'warning');
    }
  }

  // Build a list of rebuildable link types.
  module_load_include('generate.inc', 'xmlsitemap');
  $rebuild_types = xmlsitemap_get_rebuildable_link_types();

  $form['entities'] = array(
    '#type' => 'select',
    '#title' => t("Select which link types you would like to rebuild"),
    '#description' => t('If no link types are selected, the sitemap files will just be regenerated.'),
    '#multiple' => TRUE,
    '#options' => drupal_map_assoc($rebuild_types),
    '#default_value' => variable_get('xmlsitemap_rebuild_needed', FALSE) || !variable_get('xmlsitemap_developer_mode', 0) ? $rebuild_types : array(),
    '#access' => variable_get('xmlsitemap_developer_mode', 0),
  );
  $form['save_custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save and restore any custom inclusion and priority links.'),
    '#default_value' => TRUE,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to rebuild the XML sitemap?'),
    'admin/settings/xmlsitemap',
    '',
    t('Rebuild sitemap'),
    t('Cancel')
  );
}

/**
 * Submit handler; Starts the sitemap rebuild batch.
 *
 * @see xmlsitemap_rebuild_form()
 * @see xmlsitemap_rebuild_batch()
 */
function xmlsitemap_rebuild_form_submit($form, &$form_state) {
  module_load_include('generate.inc', 'xmlsitemap');
  $batch = xmlsitemap_rebuild_batch($form_state['values']['entities'], $form_state['values']['save_custom']);
  batch_set($batch);
  $form_state['redirect'] = 'admin/settings/xmlsitemap';
}

/**
 * Add a table summary for an entity and its bundles.
 */
function xmlsitemap_add_form_entity_summary(&$form, $entity, array $entity_info) {
  $priorities = xmlsitemap_get_priority_options(NULL, FALSE);
  $statuses = xmlsitemap_get_status_options(NULL);
  $destination = drupal_get_destination();

  $rows = array();
  $totals = array('total' => 0, 'indexed' => 0, 'visible' => 0);
  foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
    // Fetch current per-bundle link total and indexed counts.
    $status = xmlsitemap_get_link_type_indexed_status($entity, $bundle);
    $totals['total'] += $status['total'];
    $totals['indexed'] += $status['indexed'];
    $totals['visible'] += $status['visible'];

    $row = array();
    $edit_link = xmlsitemap_get_operation_link("admin/settings/xmlsitemap/settings/$entity/$bundle", array('modal' => TRUE));
    if (!empty($edit_link['router info']['access'])) {
      $row[] = l($bundle_info['label'], $edit_link['href'], $edit_link);
    }
    else {
      // Bundle labels are assumed to be un-escaped input.
      $row[] = check_plain($bundle_info['label']);
    }
    $row[] = $statuses[$bundle_info['xmlsitemap']['status'] ? 1 : 0];
    $row[] = $priorities[number_format($bundle_info['xmlsitemap']['priority'], 1)];
    $row[] = $status['total'];
    $row[] = $status['indexed'];
    $row[] = $status['visible'];
    $rows[] = $row;
  }

  if ($rows) {
    $header = array(
      isset($entity_info['bundle label']) ? $entity_info['bundle label'] : '',
      t('Inclusion'),
      t('Priority'),
      t('Available'),
      t('Indexed'),
      t('Visible'),
    );
    $rows[] = array(
      array(
        'data' => t('Totals'),
        'colspan' => 3,
        'header' => TRUE,
      ),
      array(
        'data' => $totals['total'],
        'header' => TRUE,
      ),
      array(
        'data' => $totals['indexed'],
        'header' => TRUE,
      ),
      array(
        'data' => $totals['visible'],
        'header' => TRUE,
      ),
    );
    $form['summary'] = array(
      '#value' => theme('table', $header, $rows),
    );
  }
}

/**
 * Add the link type XML sitemap options to the link type's form.
 *
 * Caller is responsible for ensuring xmlsitemap_link_bundle_settings_save()
 * is called during submission.
 */
function xmlsitemap_add_link_bundle_settings(array &$form, array &$form_state, $entity, $bundle) {
  $entity_info = xmlsitemap_get_link_info($entity);
  $bundle_info = xmlsitemap_link_bundle_load($entity, $bundle);

  $form['xmlsitemap'] = array(
    '#type' => 'fieldset',
    '#title' => t('XML sitemap'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('administer xmlsitemap'),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.js',
      ),
    ),
    '#tree' => TRUE,
    '#entity' => $entity,
    '#bundle' => $bundle,
    '#entity_info' => $entity_info,
    '#bundle_info' => $bundle_info,
  );
  $form['xmlsitemap']['description'] = array(
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#value' => t('Changing these type settings will affect any items of this type that have either inclusion or priority set to default.'),
  );
  $form['xmlsitemap']['status'] = array(
    '#type' => 'select',
    '#title' => t('Inclusion'),
    '#options' => xmlsitemap_get_status_options(),
    '#default_value' => $bundle_info['status'],
  );
  $form['xmlsitemap']['priority'] = array(
    '#type' => 'select',
    '#title' => t('Default priority'),
    '#options' => xmlsitemap_get_priority_options(),
    '#default_value' => $bundle_info['priority'],
    '#process' => array('form_expand_ahah', 'ctools_dependent_process'),
    '#dependency' => array('edit-xmlsitemap-status' => array(1)),
  );

  // Hide the priority field if the link is excluded.
  if (module_exists('ctools')) {
    ctools_include('dependent');
  }

  $form += array('#submit' => array());
  array_unshift($form['#submit'], 'xmlsitemap_link_bundle_settings_form_submit');

  if (isset($form['submit'])) {
    $form['submit'] += array('#weight' => 40);
  }
  if (isset($form['delete'])) {
    $form['delete'] += array('#weight' => 50);
  }
}

function xmlsitemap_link_bundle_settings_form(array &$form_state, array $bundle) {
  if (empty($form_state['ajax']) && $admin_path = xmlsitemap_get_bundle_path($bundle['entity'], $bundle['bundle'])) {
    // If this is a non-ajax form, redirect to the bundle administration page.
    $destination = drupal_get_destination();
    unset($_REQUEST['destination']);
    drupal_goto($admin_path, $destination);
  }
  else {
    drupal_set_title( t('@bundle XML sitemap settings', array('@bundle' => $bundle['info']['label'])));
  }

  $form = array();
  xmlsitemap_add_link_bundle_settings($form, $form_state, $bundle['entity'], $bundle['bundle']);
  $form['xmlsitemap']['#type'] = 'markup';
  $form['xmlsitemap']['#value'] = '';
  $form['xmlsitemap']['#access'] = TRUE;
  $form['xmlsitemap']['#show_message'] = TRUE;

  $form['actions'] = array(
    '#weight' => 100,
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#value' => l(t('Cancel'), isset($_GET['destination']) ? $_GET['destination'] : 'admin/settings/xmlsitemap/settings'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Add a link's XML sitemap options to the link's form.
 *
 * @todo Add changefreq overridability.
 */
function xmlsitemap_add_form_link_options(array &$form, $entity, $bundle, $id) {
  $info = xmlsitemap_get_link_info($entity);

  if (!$info || empty($info['bundles'][$bundle])) {
    return;
  }

  if (!$link = xmlsitemap_link_load($entity, $id)) {
    $link = array();
  }

  $bundle_info = xmlsitemap_link_bundle_load($entity, $bundle);
  $link += array(
    'access' => 1,
    'status' => $bundle_info['status'],
    'status_default' => $bundle_info['status'],
    'status_override' => 0,
    'priority' => $bundle_info['priority'],
    'priority_default' => $bundle_info['priority'],
    'priority_override' => 0,
  );

  $form['xmlsitemap'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('XML sitemap'),
    '#collapsible' => TRUE,
    '#collapsed' => !$link['status_override'] && !$link['priority_override'],
    '#access' => user_access('administer xmlsitemap') || xmlsitemap_link_bundle_access($bundle_info),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.js',
      ),
    ),
  );

  if (xmlsitemap_link_bundle_access($bundle_info) && $path = xmlsitemap_get_bundle_path($entity, $bundle)) {
    $form['xmlsitemap']['description'] = array(
      '#prefix' => '<div class="description">',
      '#suffix' => '</div>',
      '#value' => t('The default XML sitemap settings for this @bundle can be changed <a href="@link-type">here</a>.', array('@bundle' => drupal_strtolower($info['bundle label']), '@link-type' => url($path, array('query' => drupal_get_destination())))),
    );
  }

  // Show a warning if the link is not accessible and will not be included in
  // the sitemap.
  if ($id && !$link['access']) {
    $form['xmlsitemap']['warning'] = array(
      '#type' => 'markup',
      '#prefix' => '<p><strong>',
      '#suffix' => '</strong></p>',
      '#value' => ('This item is not currently visible to anonymous users, so it will not be included in the sitemap.'),
    );
  }

  // Status field (inclusion/exclusion)
  $form['xmlsitemap']['status'] = array(
    '#type' => 'select',
    '#title' => t('Inclusion'),
    '#options' => xmlsitemap_get_status_options($link['status_default']),
    '#default_value' => $link['status_override'] ? $link['status'] : 'default',
  );
  $form['xmlsitemap']['status_default'] = array(
    '#type' => 'value',
    '#value' => $link['status_default'],
  );
  $form['xmlsitemap']['status_override'] = array(
    '#type' => 'value',
    '#value' => $link['status_override'],
  );

  // Priority field
  $form['xmlsitemap']['priority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#options' => xmlsitemap_get_priority_options($link['priority_default']),
    '#default_value' => $link['priority_override'] ? number_format($link['priority'], 1) : 'default',
    '#description' => t('The priority of this URL relative to other URLs on your site.'),
    '#process' => array('form_expand_ahah', 'ctools_dependent_process'),
    '#dependency' => array('edit-xmlsitemap-status' => array(1)),
  );
  if ($link['status_default']) {
    $form['xmlsitemap']['priority']['#dependency']['edit-xmlsitemap-status'][] = 'default';
  }
  $form['xmlsitemap']['priority_default'] = array(
    '#type' => 'value',
    '#value' => $link['priority_default'],
  );
  $form['xmlsitemap']['priority_override'] = array(
    '#type' => 'value',
    '#value' => $link['priority_override'],
  );

  // Other persistent fields.
  //$form['xmlsitemap']['lastmod'] = array(
  //  '#type' => 'value',
  //  '#value' => $node->xmlsitemap['lastmod'],
  //);
  //$form['xmlsitemap']['changefreq'] = array(
  //  '#type' => 'value',
  //  '#value' => $node->xmlsitemap['changefreq'],
  //);
  //$form['xmlsitemap']['changecount'] = array(
  //  '#type' => 'value',
  //  '#value' => $node->xmlsitemap['changecount'],
  //);

  // Hide the priority field if the link is excluded.
  if (module_exists('ctools')) {
    ctools_include('dependent');
  }

  // Add the submit handler to adjust the default values if selected.
  $form += array('#submit' => array());
  if (!in_array('xmlsitemap_process_form_link_options', $form['#submit'])) {
    array_unshift($form['#submit'], 'xmlsitemap_process_form_link_options');
  }
}

/**
 * Get a list of priority options.
 *
 * @param $default
 *   Include a 'default' option.
 * @param $guides
 *   Add helpful indicators for the highest, middle and lowest values.
 * @return
 *   An array of options.
 */
function xmlsitemap_get_priority_options($default = NULL, $guides = TRUE) {
  $options = array();
  $priorities = array(
    '1.0' => t('1.0'),
    '0.9' => t('0.9'),
    '0.8' => t('0.8'),
    '0.7' => t('0.7'),
    '0.6' => t('0.6'),
    '0.5' => t('0.5'),
    '0.4' => t('0.4'),
    '0.3' => t('0.3'),
    '0.2' => t('0.2'),
    '0.1' => t('0.1'),
    '0.0' => t('0.0'),
  );

  if (isset($default)) {
    $default = number_format($default, 1);
    $options['default'] = t('Default (@value)', array('@value' => $priorities[$default]));
  }

  // Add the rest of the options.
  $options += $priorities;

  if ($guides) {
    $options['1.0'] .= ' ' . t('(highest)');
    $options['0.5'] .= ' ' . t('(normal)');
    $options['0.0'] .= ' ' . t('(lowest)');
  }

  return $options;
}

/**
 * Get a list of priority options.
 *
 * @param $default
 *   Include a 'default' option.
 * @return
 *   An array of options.
 *
 * @see _xmlsitemap_translation_strings()
 */
function xmlsitemap_get_status_options($default = NULL) {
  $options = array();
  $statuses = array(
    1 => t('Included'),
    0 => t('Excluded'),
  );

  if (isset($default)) {
    $default = $default ? 1 : 0;
    $options['default'] = t('Default (@value)', array('@value' => drupal_strtolower($statuses[$default])));
  }

  $options += $statuses;

  return $options;
}
