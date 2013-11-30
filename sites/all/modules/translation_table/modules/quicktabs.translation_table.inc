<?php
// $Id: quicktabs.translation_table.inc,v 1.1.2.1 2009/09/29 02:12:36 pasqualle Exp $

/**
 * @file
 * Translation table for the quicktabs module.
 */

/**
 * Implementation of hook_translation_table_data().
 */
function quicktabs_translation_table_data() {
  $items['quicktabs'] = array(
    'title' => 'Quick Tabs',
    'description' => 'Edit quick tabs translations',
    'form' => 'quicktabs_translation_table_quicktabs_form',
    'file' => 'modules/quicktabs.translation_table.inc',
  );

  return $items;
}

/**
 * Menu callback; Admin form for quicktabs translation.
 */
function quicktabs_translation_table_quicktabs_form(&$form_state) {
  $languages_selected = isset($_SESSION['translation_table']['languages_selected']) ? $_SESSION['translation_table']['languages_selected'] : locale_language_list('name', FALSE);
  $qtid = isset($_SESSION['translation_table']['qtid']) ? $_SESSION['translation_table']['qtid'] : 0;

  $form['filter'] = quicktabs_translation_table_quicktabs_filter($languages_selected, $qtid);
  $form['filtered_form'] = quicktabs_translation_table_quicktabs_filtered_form($languages_selected, $qtid);
  $form['#submit'][] = 'quicktabs_translation_table_quicktabs_form_submit';
  $form['#submit'][] = 'translation_table_submit_translations';
  return $form;
}

/**
 * Quicktabs filter.
 */
function quicktabs_translation_table_quicktabs_filter($languages_selected, $qtid) {
  $form['languages_selected'] = array(
    '#type' => 'select',
    '#title' => t('Languages'),
    '#description' => t('Select the languages to display.'),
    '#options' => locale_language_list('name', TRUE),
    '#default_value' => array_keys($languages_selected),
    '#multiple' => TRUE,
  );

  $options[0] = t('- Display all quicktabs -');
  $result = db_query('SELECT qtid, title FROM {quicktabs} ORDER BY title');
  while ($row = db_fetch_object($result)) {
    $options[$row->qtid] = check_plain($row->title);
  }

  $form['qtid'] = array(
    '#type' => 'select',
    '#title' => t('Quicktab'),
    '#description' => t('Select a quicktab.'),
    '#options' => $options,
    '#default_value' => $qtid,
  );

  $form['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['#theme'] = 'translation_table_filter';
  return $form;
}

/**
 * Form for quicktabs translation.
 *
 * @param $languages
 *   languages to translate to
 */
function quicktabs_translation_table_quicktabs_filtered_form($languages, $qtid) {
  $header = _translation_table_get_header($languages);

  switch ($qtid) {
    case 0:
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls WHERE ls.textgroup = 'quicktabs'";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0);
      break;

    default:
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls WHERE ls.textgroup = 'quicktabs' AND ls.location LIKE 'tab:%s--%'";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0, NULL, $qtid);
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
 * Submit handler for the quicktabs translation form.
 */
function quicktabs_translation_table_quicktabs_form_submit($form, &$form_state) {
  switch ($form_state['clicked_button']['#id']) {
    case 'edit-filter':
    case 'edit-submit':
      $_SESSION['translation_table']['qtid'] = $form_state['values']['qtid'];
      $_SESSION['translation_table']['languages_selected'] = array_intersect_key(locale_language_list('name', TRUE), $form_state['values']['languages_selected']);
      break;
  }
}

