<?php
/**
 * @file
 * Block module integration.
 */

/**
 * Implements hook_block().
 */
function sexybookmarks_block($op = 'list', $delta = 0, $edit = array()) {
  switch ($op) {
    case 'list':
      return sexybookmarks_block_info();

    case 'configure':
      return sexybookmarks_block_configure($delta);

    case 'save':
      sexybookmarks_block_save($delta, $edit);

    case 'view':
      return sexybookmarks_block_view($delta);
  }
}

function sexybookmarks_block_info() {
  $blocks = array();

  $blocks['current'] = array(
    'info' => t('SexyBookmarks: Current URL'),
    'cache' => BLOCK_NO_CACHE,
  );

  $blocks['site'] = array(
    'info' => t('SexyBookmarks: Site URL'),
    'cache' => BLOCK_NO_CACHE,
  );

  return $blocks;
}

function sexybookmarks_block_configure($delta) {
  if (!module_exists('ctools')) {
    return array();
  }

  $form = array();

  $options = array('' => '- None -');
  ctools_include('export');
  foreach (ctools_export_crud_load_all('sexybookmarks_profiles') as $profile) {
    if (empty($profile->disabled)) {
      $options[$profile->name] = $profile->name;
    }
  }
  $form['profile'] = array(
    '#type' => 'select',
    '#title' => t('SexyBookmarks profile'),
    '#options' => $options,
    '#default_value' => variable_get("sexybookmarks_block_{$delta}_profile", 'default'),
  );

  return $form;
}

function sexybookmarks_block_save($delta = 0, $edit = array()) {
  variable_set("sexybookmarks_block_{$delta}_profile", $edit['profile']);
}

function sexybookmarks_block_view($delta) {
  if (function_exists($function = "sexybookmarks_block_{$delta}_view")) {
    return $function();
  }
}

function sexybookmarks_block_current_view() {
  $profile = variable_get('sexybookmarks_block_current_profile', 'default');
  if (!empty($profile)) {
    return array(
      'subject' => t('SexyBookmarks'),
      'content' => theme('sexybookmarks', array(
        'profile' => $profile,
        'id' => 'block-sexybookmarks-current',
      ))
    );
  }
}

function sexybookmarks_block_site_view() {
  $profile = variable_get('sexybookmarks_block_site_profile', 'default');
  if (!empty($profile)) {
    return array(
      'subject' => t('SexyBookmarks'),
      'content' => theme('sexybookmarks', array(
        'profile' => $profile,
        'url' => url(NULL, array('absolute' => TRUE)),
        'title' => variable_get('site_name', 'Drupal'),
        'id' => 'block-sexybookmarks-site',
      ))
    );
  }
}
