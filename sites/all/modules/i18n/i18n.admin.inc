<?php

/**
 * @file
 * Extended multilanguage administration and module settings UI.
 */

/**
 * Form builder function.
 *
 * TO DO: Add exclude paths for content selection
 * Some options have been removed from previous versions:
 * - Languages are now taken from locale module unless defined in settings file.
 * - Language dependent tables are authomatically used if defined in settings file.
 */
function i18n_admin_settings() {
  // Content selection options.
  $form['selection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content selection'),
    //'#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );
  $form['selection']['i18n_selection_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Content selection mode'),
    '#default_value' => variable_get('i18n_selection_mode', 'simple'),
    '#options' => _i18n_selection_mode(),
    '#description' => t('Determines which content to show depending on the current page language and the default language of the site.'),
  );

  // Node translation links setting.
  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content translation links'),
  );
  $form['links']['i18n_hide_translation_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide content translation links'),
    '#description' => t('Hide the links to translations in content body and teasers. If you choose this option, switching language will only be available from the language switcher block.'),
    '#default_value' => variable_get('i18n_hide_translation_links', 0),
  );
  $form['links']['i18n_translation_switch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Switch interface for translating'),
    '#default_value' => variable_get('i18n_translation_switch', 0),
    '#description' => t('Switch interface language to fit node language when creating or editing a translation. If not checked the interface language will be independent from node language.'),
  );
  return system_settings_form($form);
}

// List of selection modes
function _i18n_selection_mode() {
  return array(
    'simple' => t('Current language and language neutral.'),
    'mixed' => t('Mixed current language (if available) or default language (if not) and language neutral.'),
    'default' => t('Only default language and language neutral.'),
    'strict' => t('Only current language.'),
    'off' => t('All content. No language conditions apply.'),
  );
}

/**
 * Variables overview form
 */
function i18n_admin_variables_form() {
  $i18n_variables = i18n_variable();
  $i18n_current = array();
  $result = db_query("SELECT DISTINCT(name) FROM {i18n_variable}");
  while ($variable = db_fetch_object($result)) {
    $i18n_current[] = $variable->name;
  }
  $i18n_list = array_unique(array_merge($i18n_variables, $i18n_current));
  foreach ($i18n_list as $name) {
    $is_multilingual = in_array($name, $i18n_variables);
    $has_value = in_array($name, $i18n_current);
    if ($is_multilingual) {
      $class = $has_value ? 'ok' : 'info';
    }
    elseif ($has_value) {
      $class = 'error';
    }
    $rows[] = array(
      'class' => $class, 
      'data' => array(
        array('data' => $name, 'colspan' => 2, 'header' => TRUE),
        $is_multilingual ? t('Yes') : t('No'),
        $has_value ? t('Yes') : t('No'),
      ),
    );
  }
  if ($i18n_list) {
    $header = array('', t('Variable name'), t('Is multilingual'), t('Has translations'));
    $form['variables']['#value'] = theme('table', $header, $rows, array('class' => 'system-status-report'));
  }
  else {
    $form['variables']['#value'] = t('There are no multilingual variables.');
  }
  if (count($i18n_list) > count($i18n_variables)) {
    $form['clean'] = array(
      '#type' => 'fieldset',
      '#description' => t('Delete all existing translations for variables that are not marked as multilingual.'),
    );
    $form['clean']['cleanup'] = array(
      '#type' => 'submit',
      '#value' => t('Clean up variables'),
    );    
  }
  if ($i18n_current) {
    $form['delete'] = array(
      '#type' => 'fieldset',
      '#description' => t('Delete all existing translations for variables.'),
    );
    $form['delete']['deleteall'] = array(
      '#type' => 'submit',
      '#value' => t('Delete all translations'),
    );      
  }
  return $form;
}

/**
 * Admin variables form submission
 */
function i18n_admin_variables_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  switch ($op) {
    case t('Clean up variables'):
      if ($variables = i18n_variable()) {
        db_query("DELETE FROM {i18n_variable} WHERE name NOT IN (". db_placeholders($variables, 'varchar') . ')', $variables);
        break;
      }
      // Intenational no break, if no variables defined delete all
    case t('Delete all translations'):
      db_query("DELETE FROM {i18n_variable}");
      break;
  }
  // Rebuild cache
  cache_clear_all('variables:', 'cache', TRUE);
}