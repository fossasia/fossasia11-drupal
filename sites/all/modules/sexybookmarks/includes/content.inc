<?php
/**
 * @file
 * Content module integration.
 */

/**
 * Implements hook_sexybookmarks_theme_alter() on behalf of content.module.
 */
function content_sexybookmarks_theme_alter($items) {
  $items['sexybookmarks_formatter_sexybookmarks'] = array(
    'arguments' => array('element' => NULL),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function sexybookmarks_field_formatter_info() {
  $formatters = array();

  $info = module_invoke_all('sexybookmarks_field_formatter_info');
  $formatters['sexybookmarks'] = array(
    'label' => t('SexyBookmarks'),
    'field types' => array_keys($info),
  );

  return $formatters;
}

function theme_sexybookmarks_formatter_sexybookmarks($element) {
  $info = module_invoke_all('sexybookmarks_field_formatter_info');
  $field = content_fields($element['#field_name']);

  $profile = variable_get("node_sexybookmarks_profile_{$element['#node']->type}", 'default');
  if (function_exists('formatter_settings_get')) {
    $settings = formatter_settings_get($element);
    $profile = isset($settings['profile']) ? $settings['profile'] : $profile;
  }

  if (!empty($profile) && isset($info[$field['type']]) && isset($info[$field['type']]['callback']) && function_exists($info[$field['type']]['callback'])) {
    $data = $info[$field['type']]['callback']($element['#item']);
    return theme('sexybookmarks', array(
      'profile' => $profile,
      'url' => $data['url'],
      'title' => $data['title'],
    ));
  }

  return '';
}

/**
 * Implements hook_content_extra_fields().
 */
function sexybookmarks_content_extra_fields($type_name) {
  if (!module_exists('ctools')) {
    return;
  }

  ctools_include('export');
  $profile = ctools_export_crud_load('sexybookmarks_profiles', variable_get("node_sexybookmarks_profile_{$type_name}", 'default'));
  if (!empty($profile)) {
    return array(
      'sexybookmarks' => array(
        'label' => 'SexyBookmarks',
        'weight' => 100,
      )
    );
  }
}
