<?php

/**
 * @file
 * Helper functions for taxonomy administration.
 */

/**
 * This is the callback for taxonomy translations.
 *
 * Gets the urls:
 *  admin/content/taxonomy/%taxonomy_vocabulary/translation
 *  admin/content/taxonomy/i18n/term/xx
 *  admin/content/taxonomy/i18n/term/new/xx
 *  admin/content/taxonomy/vid/translation/op/trid
 */
function i18ntaxonomy_page_vocabulary($vocabulary, $op = NULL, $tid = NULL) {
  switch ($op) {
    case 'edit':
      drupal_set_title(t('Edit term translations'));
      $output = drupal_get_form('i18ntaxonomy_translation_term_form', $vocabulary->vid, $tid);
      break;

    default:
      $output = i18ntaxonomy_translation_overview($vocabulary->vid);
  }
  return $output;
}

/**
 * Produces a vocabulary translation form.
 */
function i18ntaxonomy_translation_term_form($form_state, $vid, $trid = NULL, $edit = array()) {
  $languages = i18n_supported_languages();

  if ($trid == 'new') {
    $translations = array();
    $form['trid'] = array(
      '#type' => 'hidden',
      '#value' => 0
    );
  }
  else {
    $form['trid'] = array(
      '#type' => 'hidden',
      '#value' => $trid
    );
    $translations = i18ntaxonomy_term_get_translations(array('trid' => $trid));
  }
  $vocabulary = taxonomy_vocabulary_load($vid);

  // List of terms for languages.
  foreach ($languages as $lang => $langname) {
    $current = isset($translations[$lang]) ? $translations[$lang]->tid : '';
    $tree = i18ntaxonomy_get_tree($vid, $lang);
    $form[$lang] = array('#type' => 'fieldset', '#tree' => TRUE);
    $form[$lang]['tid'] = _i18ntaxonomy_term_select($langname, $current, $tree);
    $form[$lang]['old'] = array(
      '#type' => 'hidden',
      '#value' => $current
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  if ($trid != 'new') {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete')
    );
  }
  $form['destination'] = array(
    '#type' => 'hidden',
    '#value' => 'admin/content/taxonomy/'. arg(3) .'/translation'
  );
  return $form;
}

/**
 * Form callback: Process vocabulary translation form.
 */
function i18ntaxonomy_translation_term_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Save'):
      i18ntaxonomy_translation_save($form_state['values'], $form_state['values']['trid']);
      drupal_set_message(t('Term translations have been updated.'));
      break;

    case t('Delete'):
      // Delete old translations for this trid.
      db_query("UPDATE {term_data} SET trid = 0 WHERE trid = %d", $form_state['values']['trid']);
      drupal_set_message(t('The term translation has been deleted.'));
      break;
  }
}

/**
 * Save taxonomy term translations.
 *
 * @param $terms
 *   Array of terms indexed by language.
 * @param $trid
 *   Optional translation set id.
 */
function i18ntaxonomy_translation_save($terms, $trid = 0) {
  // Delete old translations for this trid.
  if ($trid) {
    db_query("UPDATE {term_data} SET trid = 0 WHERE trid = %d", $trid);
  }
  // Now pick up all the tids in an array.
  $translations = array();
  foreach (i18n_supported_languages() as $lang => $name) {
    if (isset($terms[$lang]) && ($term = (array)$terms[$lang]) && $tid = $term['tid']) {
      $translations[$lang] = $tid;
    }
  }
  // Now set a translation set with all these terms. We need some table locking to avoid race conditions.
  // when other translations created simulaneously. @TODO Find a better way.
  if (count($translations)) {
    db_lock_table('term_data');
    $trid = (is_numeric($trid) && $trid) ? $trid : i18ntaxonomy_next_trid();
    $params = array_merge(array($trid), $translations);
    db_query('UPDATE {term_data} SET trid = %d WHERE tid IN('. db_placeholders($translations) .')', $params);
    db_unlock_tables();
  }
}

/**
 * Get next free trid.
 */
function i18ntaxonomy_next_trid() {
  $current = (int)db_result(db_query('SELECT max(trid) FROM {term_data}'));
  return $current + 1;
}

/**
 * Generate a tabular listing of translations for vocabularies.
 */
function i18ntaxonomy_translation_overview($vid) {
  $vocabulary = taxonomy_vocabulary_load($vid);
  drupal_set_title(check_plain($vocabulary->name));
  $output = '';

  $languages = i18n_supported_languages();
  $header = array_merge($languages, array(t('Operations')));
  $links = array();
  $types = array();
  // Get terms/translations for this vocab.
  $result = db_query('SELECT * FROM {term_data} t WHERE vid = %d', $vocabulary->vid);
  $terms = $messages = array();
  while ($term = db_fetch_object($result)) {
    if ($term->trid && $term->language) {
      $terms[$term->trid][$term->language] = $term;
    }
  }
  // Reorder data for rows and languages.
  $rows = array();
  foreach ($terms as $trid => $terms) {
    $thisrow = array();
    foreach ($languages as $lang => $name) {
      if (array_key_exists($lang, $terms)) {
        $thisrow[] = l($terms[$lang]->name, 'taxonomy/term/'. $terms[$lang]->tid);
      }
      else {
        $thisrow[] = '--';
      }
    }
    $thisrow[] = l(t('edit'), "admin/content/taxonomy/$vid/translation/edit/$trid");
    $rows[] = $thisrow;
  }
  if ($rows) {
    $output .= theme('table', $header, $rows);
  }
  else {
    $messages[] = t('No translations defined for this vocabulary.');
  }
  $messages[]= l(t('Create new translation'), "admin/content/taxonomy/$vid/translation/edit/new");
  $output .= theme('item_list', $messages);
  return $output;
}
