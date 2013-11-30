<?php
// $Id: content.translation_table.inc,v 1.1.2.1 2009/09/29 02:12:36 pasqualle Exp $

/**
 * @file
 * Translation table for the cck module.
 */

/**
 * Implementation of hook_translation_table_data().
 */
function content_translation_table_data() {
  $items['cck'] = array(
    'title' => 'CCK',
    'description' => 'Edit CCK translations',
    'form' => 'content_translation_table_cck_form',
    'file' => 'modules/content.translation_table.inc',
  );

  return $items;
}

/**
 * Menu callback; Admin form for CCK translation.
 */
function content_translation_table_cck_form(&$form_state) {
  $languages_selected = isset($_SESSION['translation_table']['languages_selected']) ? $_SESSION['translation_table']['languages_selected'] : locale_language_list('name', FALSE);
  $cck_scope = isset($_SESSION['translation_table']['cck_scope']) ? $_SESSION['translation_table']['cck_scope'] : 0;

  $form['filter'] = content_translation_table_cck_filter($languages_selected, $cck_scope);
  $form['filtered_form'] = content_translation_table_cck_filtered_form($languages_selected, $cck_scope);
  $form['#submit'][] = 'content_translation_table_cck_form_submit';
  $form['#submit'][] = 'translation_table_submit_translations';
  return $form;
}

/**
 * CCK filter.
 */
function content_translation_table_cck_filter($languages_selected, $cck_scope) {
  $form['languages_selected'] = array(
    '#type' => 'select',
    '#title' => t('Languages'),
    '#description' => t('Select the languages to display.'),
    '#options' => locale_language_list('name', TRUE),
    '#default_value' => array_keys($languages_selected),
    '#multiple' => TRUE,
  );

  $scope_options = array(
    0 => t('- All -'),
    t('Content types') => array(),
    t('Allowed values per field') => array(),
  );
  foreach (node_get_types('names') as $type => $name) {
    $scope_options[t('Content types')]['type|'. $type] = $name;
  }
  foreach (content_fields() as $field_name => $field) {
    if (empty($field['allowed_values_php']) && !empty($field['allowed_values'])) {
      $function = $field['module'] .'_allowed_values';
      $allowed_values = function_exists($function) ? $function($field) : (array) content_allowed_values($field);
      if (!empty($allowed_values)) {
        $scope_options[t('Allowed values per field')]['allowed_values|'. $field_name] = t($field['widget']['label']);
      }
    }
  }
  $form['cck_scope'] = array(
    '#type' => 'select',
    '#title' => t('Scope'),
    '#description' => t('Select the kind of strings to display.'),
    '#options' => $scope_options,
    '#default_value' => $cck_scope,
  );

  $form['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['#theme'] = 'translation_table_filter';
  return $form;
}

/**
 * Form for CCK translation.
 *
 * @param $languages
 *   languages to translate to
 * @param $cck_scope
 */
function content_translation_table_cck_filtered_form($languages, $cck_scope) {
  $header = _translation_table_get_header($languages);

  switch ($cck_scope) {
    case '0':
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls WHERE ls.textgroup = 'cck'";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0);
      break;

    default:
      list($scope_prefix, $scope_suffix) = explode('|', $cck_scope);
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls WHERE ls.textgroup = 'cck'";
      if ($scope_prefix == 'type') {
        // Retrieve field and fieldgroup strings.
        $sql .= " AND (ls.location LIKE 'field:%s-%%' OR ls.location LIKE 'fieldgroup:%s-%%')";
        $args = array($scope_suffix, $scope_suffix);
      }
      else {
        // Retrieve allowed values related strings.
        $sql .= " AND ls.location LIKE 'field:%s:option\_%%'";
        $args = array($scope_suffix);
      }
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0, NULL, $args);
      break;
  }

  $form['strings']['#tree'] = TRUE;
  $form['#cache'] = TRUE;

  $form['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );

  while ($source = db_fetch_object($result)) {
    if (strlen(trim($source->source)) > 0) {
      $form['strings'][$source->lid] = _translation_table_row($source, $languages);
    }
  }

  $form['languages'] = array(
    '#type' => 'value',
    '#value' => $languages,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  $form['#theme'] = 'translation_table';
  return $form;
}

/**
 * Submit handler for the CCK translation form.
 */
function content_translation_table_cck_form_submit($form, &$form_state) {
  switch ($form_state['clicked_button']['#id']) {
    case 'edit-filter':
    case 'edit-submit':
      $_SESSION['translation_table']['cck_scope'] = $form_state['values']['cck_scope'];
      $_SESSION['translation_table']['languages_selected'] = array_intersect_key(locale_language_list('name', TRUE), $form_state['values']['languages_selected']);
      break;
  }
}
