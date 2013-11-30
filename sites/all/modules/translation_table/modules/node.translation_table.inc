<?php
// $Id: node.translation_table.inc,v 1.1.2.1 2009/09/29 02:12:36 pasqualle Exp $

/**
 * @file
 * Provide translation table functionality for the node module.
 */

/**
 * Implementation of hook_translation_table_data().
 */
function node_translation_table_data() {
  $items['nodetype'] = array(
    'title' => 'Content type',
    'description' => 'Edit content type translations',
    'form' => 'node_translation_table_nodetype_form',
    'file' => 'modules/node.translation_table.inc',
  );

  return $items;
}

/**
 * Menu callback; Admin form for node type translation.
 */
function node_translation_table_nodetype_form(&$form_state) {
  $languages_selected = isset($_SESSION['translation_table']['languages_selected']) ? $_SESSION['translation_table']['languages_selected'] : locale_language_list('name', FALSE);
  $nodetype = isset($_SESSION['translation_table']['nodetype']) ? $_SESSION['translation_table']['nodetype'] : 0;

  $form['filter'] = node_translation_table_nodetype_filter($languages_selected, $nodetype);
  $form['filtered_form'] = node_translation_table_nodetype_filtered_form($languages_selected, $nodetype);
  $form['#submit'][] = 'node_translation_table_nodetype_form_submit';
  $form['#submit'][] = 'translation_table_submit_translations';
  return $form;
}

/**
 * Node type filter.
 */
function node_translation_table_nodetype_filter($languages_selected, $nodetype) {
  $form['languages_selected'] = array(
    '#type' => 'select',
    '#title' => t('Languages'),
    '#description' => t('Select the languages to display.'),
    '#options' => locale_language_list('name', TRUE),
    '#default_value' => array_keys($languages_selected),
    '#multiple' => TRUE,
  );

  $form['nodetype'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#description' => t('Select the content types to display.'),
    '#options' => array_merge(array( 0 => t('- All -')), node_get_types('names')),
    '#default_value' => $nodetype,
  );

  $form['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['#theme'] = 'translation_table_filter';
  return $form;
}

/**
 * Form for node type translation.
 * Note: If the node type is not in the locales_source table, then it won't be
 * displayed.
 *
 * @param $languages
 *   languages to translate to
 * @param $nodetype
 *    0: show all
 *    else: filter by node type
 */
function node_translation_table_nodetype_filtered_form($languages, $nodetype) {
  $header = _translation_table_get_header($languages);

  switch ($nodetype) {
    case '0':
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls WHERE ls.textgroup = 'nodetype'";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0);
      break;

    default:
      $sql = "SELECT ls.lid, ls.source, ls.location FROM {locales_source} ls WHERE ls.textgroup = 'nodetype' AND ls.location LIKE 'type:%s%'";
      $sql .= tablesort_sql($header);
      $result = pager_query($sql, 50, 0, NULL, $nodetype);
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
 * Submit handler for the node type translation form.
 */
function node_translation_table_nodetype_form_submit($form, &$form_state) {
  switch ($form_state['clicked_button']['#id']) {
    case 'edit-filter':
    case 'edit-submit':
      $_SESSION['translation_table']['nodetype'] = $form_state['values']['nodetype'];
      $_SESSION['translation_table']['languages_selected'] = array_intersect_key(locale_language_list('name', TRUE), $form_state['values']['languages_selected']);
      break;
  }
}
