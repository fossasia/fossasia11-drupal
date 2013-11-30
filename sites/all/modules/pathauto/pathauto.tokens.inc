<?php

/**
 * @file
 * Builds placeholder replacement tokens for pathauto.
 *
 * @ingroup pathauto
 */

/**
 * Implements hook_token_list().
 */
function _pathauto_token_list($type = 'all') {
  $tokens = array();

  if (module_exists('taxonomy')) {
    if ($type == 'taxonomy' || $type == 'all') {
      $tokens['taxonomy']['catpath'] = t('As [cat], but including its supercategories separated by /.');
      $tokens['taxonomy']['catpath-raw'] = t('As [cat-raw], but including its supercategories separated by /.');
      $tokens['taxonomy']['catalias'] = t('The URL alias of the taxonomy term.');
      $tokens['taxonomy']['catalias-raw'] = t('The URL alias of the taxonomy term.');
    }
    if ($type == 'node' || $type == 'all') {
      $tokens['node']['termpath'] = t('As [term], but including its supercategories separated by /.');
      $tokens['node']['termpath-raw'] = t('As [term-raw], but including its supercategories separated by /.');
      $tokens['node']['termalias'] = t('The URL alias of the taxonomy term.');
      $tokens['node']['termalias-raw'] = t('The URL alias of the taxonomy term.');
    }
  }

  if (module_exists('book')) {
    if ($type == 'node' || $type == 'all') {
      $tokens['node']['bookpathalias'] = t('The URL alias of the parent book of the node.');
      $tokens['node']['bookpathalias-raw'] = t('The URL alias of the parent book of the node.');
    }
  }

  return $tokens;
}

/**
 * Implements hook_token_values().
 */
function _pathauto_token_values($type, $object = NULL, $options = array(), $label = NULL) {
  $values = array();

  if ($type == 'node' && !empty($object)) {
    // Token [bookpathalias].
    if (module_exists('book')) {
      $values['bookpathalias'] = '';
      $values['bookpathalias-raw'] = '';
      if (!empty($object->book['plid']) && $parent = book_link_load($object->book['plid'])) {
        $values['bookpathalias-raw'] = drupal_get_path_alias($parent['href']);
        $values['bookpathalias']     = check_plain($values['bookpathalias-raw']);
      }
    }

    // Tokens [termpath], [termpath-raw], and [termalias].
    if (module_exists('taxonomy')) {
      // Get the lowest-weighted term from the lowest-weighted vocabulary.
      // This query is copied from @taxonomy_node_get_terms()
      $term = db_fetch_object(db_query_range('SELECT t.* FROM {term_node} r INNER JOIN {term_data} t ON r.tid = t.tid INNER JOIN {vocabulary} v ON t.vid = v.vid WHERE r.vid = %d ORDER BY v.weight, t.weight, t.name', $object->vid, 0, 1));
      if ($term) {
        $values = array_merge($values, _pathauto_token_values('taxonomy', $term, $options, 'term'));
      }
      else {
        $values['termpath'] = '';
        $values['termpath-raw'] = '';
        $values['termalias'] = '';
      }
    }
  }

  if ($type == 'taxonomy' && !empty($object)) {
    // In the realm of nodes these are 'terms', in the realm of taxonomy, 'cats'.
    if (!isset($label)) {
      $label = 'cat';
    }

    $values[$label . 'path'] = '';
    $values[$label . 'path-raw'] = '';
    $values[$label . 'alias'] = '';
    $values[$label . 'alias-raw'] = '';

    // Tokens [catpath] and [catpath-raw].
    if (isset($object->tid)) {
      $parents = taxonomy_get_parents_all($object->tid);
      $catpath = $catpath_raw = array();
      foreach ($parents as $parent) {
        array_unshift($catpath, check_plain($parent->name));
        array_unshift($catpath_raw, $parent->name);
      }

      $values[$label . 'path'] = !empty($options['pathauto']) ? $catpath : implode('/', $catpath);
      $values[$label . 'path-raw'] = !empty($options['pathauto']) ? $catpath_raw : implode('/', $catpath_raw);

      // Token [catalias-raw] and [catalias].
      $values[$label . 'alias-raw'] = drupal_get_path_alias(taxonomy_term_path($object));
      $values[$label . 'alias']     = check_plain($values[$label . 'alias-raw']);
    }
  }

  return $values;
}
