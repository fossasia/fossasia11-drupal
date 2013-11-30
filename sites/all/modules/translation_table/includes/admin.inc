<?php
// $Id: admin.inc,v 1.1.2.12 2010/10/20 12:19:26 pasqualle Exp $

/**
 * @file
 * The administration interface.
 */

/**
 * Header of the translation table.
 *
 * @param $languages
 *   languages to translate to
 *
 * @return
 *   header row for theme_table
 */
function _translation_table_get_header($languages) {
  $header = array(array('data' => t('Original'), 'sort' => 'asc', 'field' => 'source'));

  foreach ($languages as $lang_code => $lang_name) {
    $header[] = array('data' => $lang_name);
  }

  $header[] = array('data' => t('Operations'), 'colspan' => 3);
  return $header;
}

/**
 * One row of the translation table.
 *
 * @param $source
 *   source string record
 * @param $languages
 *   languages to translate to
 * @return
 *   table row for theme_table
 */
function _translation_table_row($source, $languages) {
  $form['source'] = array(
    '#type' => 'markup',
    '#value' => check_plain($source->source),
  );
  $form['location'] = array(
    '#type' => 'value',
    '#value' => check_plain($source->location),
  );

  foreach ($languages as $lang_code => $lang_name) {
    $translation = db_result(db_query("SELECT lt.translation FROM {locales_target} lt WHERE lt.lid = %d AND lt.language = '%s'", $source->lid, $lang_code));
    $form[$lang_code] = array(
      '#type' => 'textfield',
      '#default_value' => $translation,
      '#size' => 35,
      '#maxlength' => NULL,
    );
  }
  return $form;
}

/**
 * Submit handler for the translation table.
 */
function translation_table_submit_translations($form, &$form_state) {
  switch ($form_state['clicked_button']['#id']) {
    case 'edit-submit':
      $language_list = locale_language_list('language', TRUE);
      if (!empty($form_state['values']['strings'])) {
        foreach ($form_state['values']['strings'] as $lid => $values) {
          foreach ($values as $lang_code => $translation) {
            if (in_array($lang_code, $language_list)) {
              _translation_table_update_translation($lid, $lang_code, $translation);
            }
          }
        }
      }
      break;
  }

  // Redirect to current page.
  $query = isset($_GET['page']) ? array('page' => $_GET['page']) : array();
  $form_state['redirect'] = array($_GET['q'], $query);
}

/**
 * Update, create or delete translation as needed.
 */
function _translation_table_update_translation($lid, $lang_code, $translation) {
  if ($translation == '') {
    db_query("DELETE FROM {locales_target} WHERE lid = %d AND language = '%s'", $lid, $lang_code);
    return;
  }
  db_query("UPDATE {locales_target} SET translation = '%s' WHERE lid = %d AND language = '%s'", $translation, $lid, $lang_code);
  if (!db_affected_rows()) {
    db_query("INSERT INTO {locales_target} (lid, language, translation) VALUES (%d, '%s', '%s')", $lid, $lang_code, $translation);
  }
}
