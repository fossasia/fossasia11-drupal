<?php

/**
 * @file
 * User page callbacks for the translation module.
 */

/**
 * Overview page for a node's translations.
 *
 * Replacement for Drupal core translation module's pages
 *
 * @param $node
 *   Node object.
 */
function i18n_translation_node_overview($node) {
  if ($node->tnid) {
    // Already part of a set, grab that set.
    $tnid = $node->tnid;
    $translations = translation_node_get_translations($node->tnid);
  }
  else {
    // We have no translation source nid, this could be a new set, emulate that.
    $tnid = $node->nid;
    $translations = array($node->language => $node);
  }

  $header = array(t('Language'), t('Title'), t('Status'), t('Operations'));

  foreach (language_list() as $language) {
    $options = array();
    $language_name = $language->name;
    // We may need to switch interface language for translations
    $params = variable_get('i18n_translation_switch', 0) ? array('language' => $language) : array();
    if (isset($translations[$language->language])) {
      // Existing translation in the translation set: display status.
      // We load the full node to check whether the user can edit it.
      $translation_node = node_load($translations[$language->language]->nid);
      $title = l($translation_node->title, 'node/'. $translation_node->nid, $params);
      if (node_access('update', $translation_node)) {
        $options[] = l(t('edit'), "node/$translation_node->nid/edit", $params);
      }
      $status = $translation_node->status ? t('Published') : t('Not published');
      $status .= $translation_node->translate ? ' - <span class="marker">'. t('outdated') .'</span>' : '';
      if ($translation_node->nid == $tnid) {
        $language_name = t('<strong>@language_name</strong> (source)', array('@language_name' => $language_name));
      }
    }
    else {
      // No such translation in the set yet: help user to create it.
      $title = t('n/a');
      if (node_access('create', $node)) {
        $options[] = l(t('add translation'), 'node/add/'. str_replace('_', '-', $node->type), array('query' => "translation=$node->nid&language=$language->language") + $params);
      }
      $status = t('Not translated');
    }
    $rows[] = array($language_name, $title, $status, implode(" | ", $options));
  }

  drupal_set_title(t('Translations of %title', array('%title' => $node->title)));
  $output = theme('table', $header, $rows);
  if (user_access('administer translations')) {
    $output .= drupal_get_form('i18n_node_select_translation', $node, $translations);
  }
  return $output;
}

/**
 * Form to select existing nodes as translation
 *
 * This one uses autocomplete fields for all languages
 */
function i18n_node_select_translation($form_state, $node, $translations) {
  $form['node'] = array('#type' => 'value', '#value' => $node);
  $form['translations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select translations for %title', array('%title' => $node->title)),
    '#tree' => TRUE,
    '#theme' => 'i18n_node_select_translation',
    '#description' => t("Alternatively, you can select existing nodes as translations of this one or remove nodes from this translation set. Only nodes that have the right language and don't belong to other translation set will be available here.")
  );
  foreach (language_list() as $language) {
    if ($language->language != $node->language) {
      $trans_nid = isset($translations[$language->language]) ? $translations[$language->language]->nid : 0;
      $form['translations']['nid'][$language->language] = array('#type' => 'value', '#value' => $trans_nid);
      $form['translations']['language'][$language->language] = array('#value' => $language->name);
      $form['translations']['node'][$language->language] = array(
        '#type' => 'textfield',
        '#maxlength' => 255,
        '#autocomplete_path' => 'i18n/node/autocomplete/' . $node->type . '/' . $language->language,
        '#default_value' => $trans_nid ? i18n_node_nid2autocomplete($trans_nid) : '',
      );
    }
  }
  $form['buttons']['update'] = array('#type' => 'submit', '#value' => t('Update translations'));
  //$form['buttons']['clean'] = array('#type' => 'submit', '#value' => t('Delete translation set'));
  return $form;
}

/**
 * Form validation
 */
function i18n_node_select_translation_validate($form, &$form_state) {
  foreach ($form_state['values']['translations']['node'] as $lang => $title) {
    if (!$title) {
      $nid = 0;
    }
    else {
      $nid = i18n_node_autocomplete2nid($title, "translations][node][$lang", array($node->type), array($lang));
    }
    $form_state['values']['translations']['nid'][$lang] = $nid;
  }
}

/**
 * Form submission: update / delete the translation set
 */
function i18n_node_select_translation_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : NULL;
  $node = $form_state['values']['node'];
  $translations = $node->tnid ? translation_node_get_translations($node->tnid) : array($node->language => $node);
  foreach ($translations as $trans) {
    $current[$trans->language] = $trans->nid;
  }
  $update = array($node->language => $node->nid) + array_filter($form_state['values']['translations']['nid']);
  // Compute the difference to see which are the new translations and which ones to remove
  $new = array_diff_assoc($update, $current);
  $remove = array_diff_assoc($current, $update);

  // The tricky part: If the existing source is not in the new set, we need to create a new tnid
  if ($node->tnid && in_array($node->tnid, $update)) {
    $tnid = $node->tnid;
    $add = $new;
  }
  else {
    // Create new tnid, which is the source node
    $tnid = $node->nid;
    $add = $update;
  }
  // Now update values for all nodes
  if ($add) {
    $args = array('' => $tnid) + $add;
    db_query('UPDATE {node} SET tnid = %d WHERE nid IN (' . db_placeholders($add) . ')', $args);
    if (count($new)) {
      drupal_set_message(format_plural(count($new), 'Added a node to the translation set.', 'Added @count nodes to the translation set.'));
    }
  }
  if ($remove) {
    db_query('UPDATE {node} SET tnid = 0 WHERE nid IN (' . db_placeholders($remove) . ')', $remove);
    drupal_set_message(format_plural(count($remove), 'Removed a node from the translation set.', 'Removed @count nodes from the translation set.'));
  }
}

/**
 * Node title autocomplete callback
 */
function i18n_node_autocomplete($type, $language, $string = '') {
  $params = array('type' => $type, 'language' => $language, 'tnid' => 0);
  $matches = array();
  foreach (_i18n_node_references($string, 'contains', $params) as $id => $row) {
    // Add a class wrapper for a few required CSS overrides.
    $matches[$row['title'] ." [nid:$id]"] = '<div class="reference-autocomplete">'. $row['rendered'] . '</div>';
  }
  drupal_json($matches);
}

/**
 * Generates 'title [nid:$nid]' for the autocomplete field
 */
function i18n_node_nid2autocomplete($nid) {
  if ($node = node_load($nid)) {
    return check_plain($node->title) . ' [nid:' . $nid .']';
  }
  else {
    return t('Not found');
  }
}

/**
 * Reverse mapping from node title to nid
 *
 * We also handle autocomplete values (title [nid:x]) and validate the form
 */
function i18n_node_autocomplete2nid($name, $field = NULL, $type, $language) {
  if (!empty($name)) {
    preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $name, $matches);
    if (!empty($matches)) {
      // Explicit [nid:n].
      list(, $title, $nid) = $matches;
      if (!empty($title) && ($node = node_load($nid)) && $title != $node->title) {
        if ($field) {
          form_set_error($field, t('Node title mismatch. Please check your selection.'));
        }
        $nid = NULL;
      }
    }
    else {
      // No explicit nid.
      $reference = _i18n_node_references($name, 'equals', array('type' => $type, 'language' => $language), 1);
      if (!empty($reference)) {
        $nid = key($reference);
      }
      elseif ($field) {
        form_set_error($field, t('Found no valid post with that title: %title', array('%title' => $name)));
      }
    }
  }
  return !empty($nid) ? $nid : NULL;
}

/**
 * Find node title matches.
 *
 * @param $string
 *   String to match against node title
 * @param $match
 *   Match mode: 'contains', 'equals', 'starts_with'
 * @param $params
 *   Other query arguments: type, language or numeric ones
 *
 * Some code from CCK's nodereference.module
 */
function _i18n_node_references($string, $match = 'contains', $params = array(), $limit = 10) {
  $where = $args = array();
  $match_operators = array(
    'contains' => "LIKE '%%%s%%'",
    'equals' => "= '%s'",
    'starts_with' => "LIKE '%s%%'",
  );
  foreach ($params as $key => $value) {
    $type = in_array($key, array('type', 'language')) ? 'char' : 'int';
    if (is_array($value)) {
      $where[] = "n.$key IN (" . db_placeholders($value, $type) . ') ';
      $args = array_merge($args, $value);
    }
    else {
      $where[] = "n.$key = " . db_type_placeholder($type);
      $args[] = $value;
    }
  }
  $where[] = 'n.title '. (isset($match_operators[$match]) ? $match_operators[$match] : $match_operators['contains']);
  $args[] = $string;
  // Disable and reenable i18n selection mode so no language conditions are inserted
  i18n_selection_mode('off');
  $sql = db_rewrite_sql('SELECT n.nid, n.title, n.type FROM {node} n WHERE ' . implode(' AND ', $where) . ' ORDER BY n.title, n.type');
  $result = db_query_range($sql, $args, 0, $limit) ;
  i18n_selection_mode('reset');
  $references = array();
  while ($node = db_fetch_object($result)) {
    $references[$node->nid] = array(
      'title' => $node->title,
      'rendered' => check_plain($node->title),
    );
  }
  return $references;
}

/**
 * Theme select translation form
 * @ingroup themeable
 */
function theme_i18n_node_select_translation($elements) {
  $output = '';
  if (isset($elements['nid'])) {
    $rows = array();
    foreach (element_children($elements['nid']) as $lang) {
      $rows[] = array(
        drupal_render($elements['language'][$lang]),
        drupal_render($elements['node'][$lang]),
      );
    }
    $output .= theme('table', array(), $rows);
    $output .= drupal_render($elements);
  }
  return $output;
}