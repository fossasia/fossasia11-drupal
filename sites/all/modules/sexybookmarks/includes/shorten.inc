<?php
/**
 * @file
 * Shorten module integration.
 */

/**
 * Implements hook_sexybookmarks_form_alter_alter() on behalf of shorten.module.
 */
function shorten_sexybookmarks_form_alter_alter($form, $form_state, $form_id) {
  if ($form_id == 'ctools_export_ui_edit_item_form') {
    $form['config']['settings']['shortener']['#options'] = array_merge(
      array('drupal_shorten' => t('Shorten module')),
      $form['config']['settings']['shortener']['#options']
    );
  }
}

/**
 * Implements hook_sexybookmarks_config_alter() on behalf of shorten.module.
 */
function shorten_sexybookmarks_config_alter($config, $variables) {
  if ($config['shortener'] == 'drupal_shorten') {
    $config['shortener'] = 'none';
    $variables[0]['url'] = shorten_url($variables[0]['url']);
  }
}
