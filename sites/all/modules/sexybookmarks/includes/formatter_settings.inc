<?php
/**
 * @file
 * Formatter Settings module integration.
 */

/**
 * Implements hook_formatter_settings_form().
 */
function sexybookmarks_formatter_settings_form($content_type, $field_name, $formatter_name, $settings) {
  if (!module_exists('ctools')) {
    return array();
  }

  $element = array();

  $options = array('' => '- None -');
  ctools_include('export');
  foreach (ctools_export_crud_load_all('sexybookmarks_profiles') as $profile) {
    if (empty($profile->disabled)) {
      $options[$profile->name] = $profile->name;
    }
  }
  $element['profile'] = array(
    '#type' => 'select',
    '#title' => t('Profile'),
    '#options' => $options,
    '#default_value' => isset($settings['profile']) ? $settings['profile'] : 'default',
  );

  return $element;
}

/**
 * Implements hook_formatter_settings_summary().
 */
function sexybookmarks_formatter_settings_summary($content_type, $field_name, $formatter_name, $settings) {
  $profile = isset($settings['profile']) ? $settings['profile'] : 'default';
  return !empty($profile) ? t('Profile: !profile', array('!profile' => $profile)) : t('Disabled');
}
