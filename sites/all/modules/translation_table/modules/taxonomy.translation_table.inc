<?php
// $Id: taxonomy.translation_table.inc,v 1.1.2.1 2009/09/29 02:12:36 pasqualle Exp $

/**
 * @file
 * Translation table for the taxonomy module.
 */

/**
 * Implementation of hook_translation_table_data().
 */
function taxonomy_translation_table_data() {
  $items['taxonomy'] = array(
    'title' => 'Taxonomy',
    'description' => 'Edit taxonomy translations',
    'form' => 'taxonomy_translation_table_taxonomy_form',
    'file' => 'modules/taxonomy.translation_table.inc',
  );

  return $items;
}

/**
 * Menu callback; Admin form for taxonomy translation.
 */
function taxonomy_translation_table_taxonomy_form(&$form_state) {
  $languages_selected = isset($_SESSION['translation_table']['languages_selected']) ? $_SESSION['translation_table']['languages_selected'] : locale_language_list('name', FALSE);
  $vid = isset($_SESSION['translation_table']['vid']) ? $_SESSION['translation_table']['vid'] : -1;

  $form['filter'] = taxonomy_translation_table_taxonomy_filter($languages_selected, $vid);
  $form['filtered_form'] = taxonomy_translation_table_taxonomy_filtered_form($languages_selected, $vid);
  $form['#submit'][] = 'taxonomy_translation_table_taxonomy_form_submit';
  $form['#submit'][] = 'translation_table_submit_translations';
  return $form;
}

/**
 * Taxonomy filter.
 */
function taxonomy_translation_table_taxonomy_filter($languages_selected, $vid) {
  $form['languages_selected'] = array(
    '#type' => 'select',
    '#title' => t('Languages'),
    '#description' => t('Select the languages to display.'),
    '#options' => locale_language_list('name', TRUE),
    '#default_value' => array_keys($languages_selected),
    '#multiple' => TRUE,
  );

  $vocabulary_options[0] = t('- Display all taxonomy strings -');
  $vocabulary_options[-1] = t('- Display vocabulary strings -');

  if (module_exists('taxonomy')) {
    if ($vocabularies = taxonomy_get_vocabularies()) {
      foreach ($vocabularies as $key => $vocabulary) {
        $vocabulary_options[$key] = $vocabulary->name;
      }
    }
  }

  $form['vid'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#description' => t('Select the vocabulary.'),
    '#options' => $vocabulary_options,
    '#default_value' => $vid,
  );

  $form['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['#theme'] = 'translation_table_filter';
  return $form;
}

/**
 * Form for taxonomy translation.
 * Note: If term and vocabulary strings are not in the locales_source table,
 * then they won't be displayed.
 *
 * @param $languages
 *   languages to translate to
 * @param $vid
 *   -1: show vocabulary names only
 *    0: show all
 *    else: filter by vocabulary ID
 */
function taxonomy_translation_table_taxonomy_filtered_form($languages, $vid) {
  $header = _translation_table_get_header($languages);

  switch ($vid) {
    case 0:
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls WHERE ls.textgroup = 'taxonomy'";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0);
      break;

    case -1:
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls LEFT JOIN {i18n_strings} s ON ls.lid = s.lid WHERE s.type = 'vocabulary'";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0);
      break;

    default:
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls LEFT JOIN {i18n_strings} s ON ls.lid = s.lid WHERE s.type = 'term' AND s.objectid IN (SELECT td.tid FROM {term_data} td WHERE td.vid = %d)";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0, NULL, $vid);
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
 * Submit handler for the taxonomy translation form.
 */
function taxonomy_translation_table_taxonomy_form_submit($form, &$form_state) {
  switch ($form_state['clicked_button']['#id']) {
    case 'edit-filter':
    case 'edit-submit':
      $_SESSION['translation_table']['vid'] = $form_state['values']['vid'];
      $_SESSION['translation_table']['languages_selected'] = array_intersect_key(locale_language_list('name', TRUE), $form_state['values']['languages_selected']);
      break;
  }
}
