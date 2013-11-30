<?php

// Load locale library
include_once './includes/locale.inc';

/**
 * @file
 * Admin page callbacks for the i18nstrings module.
 */
function i18nstrings_admin_refresh_page() {
  $output = '';
  $output .= drupal_get_form('i18nstrings_admin_refresh');
  return $output;
}

/**
 * Form callback. Refresh textgroups.
 */
function i18nstrings_admin_refresh() {
  // Select textgroup/s. Just the ones that have a 'refresh callback'
  $groups = module_invoke_all('locale', 'groups');
  unset($groups['default']);
  foreach (array_keys($groups) as $key) {
    if (!i18nstrings_group_info($key, 'refresh callback')) {
      unset($groups[$key]);
    }
  }
  $form['groups'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select text groups'),
    '#options' => $groups,
    '#description' => t('If a text group is no showing up here it means this feature is not implemented for it.'),
  );
  $form['refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh strings'),
    '#suffix' => '<p>'. t('This will create all the missing strings for the selected text groups.') .'</p>',
  );
  // Get all languages, except default language.
  $languages = locale_language_list('name', TRUE);
  unset($languages[language_default('language')]);
  $form['languages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select languages'),
    '#options' => $languages,
  );
  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update translations'),
    '#suffix' => '<p>'. t('This will fetch all existing translations from the localization tables for the selected text groups and languages.') .'</p>',
  );
  return $form;
}

/**
 * Form submission.
 */
function i18nstrings_admin_refresh_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $groups = array_filter($form_state['values']['groups']);
  $languages = array_filter($form_state['values']['languages']);
  $group_names = module_invoke_all('locale', 'groups');
  if ($op == t('Refresh strings') && $groups) {
    foreach ($groups as $group) {
      if (i18nstrings_refresh_group($group, TRUE)) {
        drupal_set_message(t("Successfully refreshed strings for %group", array('%group' => $group_names[$group])));
      }
      else {
        drupal_set_message(t("Cannot refresh strings for %group.", array('%group' => $group_names[$group])), 'warning');
      }
    }
  }
  elseif ($op == t('Update translations') && $groups && $languages) {
    $count = 0;
    foreach ($languages as $language) {
      $count += i18nstrings_admin_update($language, $groups);
    }
    drupal_set_message(format_plural($count, '1 string has been updated.', '@count strings have been updated.'));
  }
}

/**
 * Update strings for language.
 */
function i18nstrings_admin_update($language, $groups) {
  $params = $groups;
  $params[] = $language;
  $sql = 'SELECT g.*, t.translation, t.lid as tlid, i.format FROM {locales_source} g INNER JOIN {locales_source} s ON g.source = s.source AND s.lid <> g.lid ';
  $sql .= 'INNER JOIN {locales_target} t ON s.lid = t.lid LEFT JOIN {locales_target} t2 ON g.lid = t2.lid ';
  $sql .= 'INNER JOIN {i18n_strings} i ON i.lid = g.lid ';
  $sql .= 'WHERE t2.lid IS NULL AND g.textgroup IN ('. db_placeholders($groups, 'varchar') .") AND t.language = '%s'";
  $result = db_query($sql, $params);
  $count = 0;
  while ($string = db_fetch_object($result)) {
    // Just update strings when no input format, otherwise it could be dangerous under some circumstances.
    if (empty($string->format) && !empty($string->translation)) {
      $count++;
      db_query("INSERT INTO {locales_target} (translation, lid, language) VALUES('%s', %d, '%s')", $string->translation, $string->lid, $language);
    }
  }
  return $count;
}

/**
 * Configure filters for string translation.
 * 
 * This has serious security implications so this form needs the 'administer filters' permission
 */
function i18nstrings_admin_settings() {
  include_once './includes/locale.inc';
  // As the user has administer filters permissions we get a full list here
  foreach (filter_formats() as $fid => $format) {
    $format_list[$fid] = $format->name;
  }
  $form['i18nstrings_allowed_formats'] = array(
    '#title' => t('Translatable input formats'),
    '#options' => $format_list,
    '#type' => 'checkboxes',
    '#default_value' => variable_get('i18nstrings_allowed_formats', array(variable_get('filter_default_format', 1))),
    '#description' => t('Only the strings that have the input formats selected will be allowed by the translation system. All the others will be deleted next time the strings are refreshed.'),
  );
  // Whitelist text groups without formatted strings for backwards compatibility
  $textgroups = module_invoke_all('locale', 'groups');
  unset($textgroups['default']);
  foreach (array_keys($textgroups) as $group) {
    $format = i18nstrings_group_info($group, 'format');
    if (isset($format)) {
      // This one already has format information, so remove from list
      unset($textgroups[$group]);
    }
  }
  // If there are 'old' textgroups, display the bypass option
  if ($textgroups) {
    $form['i18nstrings_allowed_textgroups'] = array(
      '#title' => t('Safe text groups'),
      '#options' => $textgroups,
      '#type' => 'checkboxes',
      '#default_value' => variable_get('i18nstrings_allowed_textgroups', array()),
      '#description' => t('Select text groups to bypass filter format checking. . <strong>It is unsafe to check this option unless you are sure all the strings from that text groups are safe for translators</strong>. This option is just for backwards compatibility until all the contributed modules implement the new strings API.'),
    );
  }
  elseif (variable_get('i18nstrings_allowed_textgroups', 0)) {
    // Just in case there's a leftover variable before we updated some of the modules
    variable_del('i18nstrings_allowed_textgroups');
  }
  $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);
  return system_settings_form($form);
}
