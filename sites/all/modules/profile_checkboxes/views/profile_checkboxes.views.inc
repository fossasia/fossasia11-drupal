<?php

/**
 * @file
 * Provide views support for profile_checkboxes.module.
 */

/**
* Implementation of hook_views_handlers().
*/
function profile_checkboxes_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'profile_checkboxes') .'/views',
    ),
    'handlers' => array(
      'profile_checkboxes_handler_field_unserialize_selection' => array(
        'parent' => 'views_handler_field',
      ),
    'profile_checkboxes_handler_filter_selection' => array(
      'parent' => 'views_handler_filter_profile_selection',
      ),
    ),
  );
}

/**
* Implementation of hook_views_data_alter().
*/
function profile_checkboxes_views_data_alter(&$data) {
  $fields = profile_views_get_fields();
  foreach($fields as $field) {
    if ($field->type == 'selection') {
      if ($delimiter = profile_checkboxes_field_delimiter($field->fid)) {
        $handle_name = 'profile_values_' . $field->name;
        $data[$handle_name]['value']['field']['delimiter'] = $delimiter;
        $data[$handle_name]['value']['field']['handler'] = 'profile_checkboxes_handler_field_unserialize_selection';
        $data[$handle_name]['value']['filter']['handler'] = 'profile_checkboxes_handler_filter_selection';
      }
    }
  }
}