<?php
/**
 * @file
 * System module integration.
 */

/**
 * Implements hook_init().
 */
function sexybookmarks_init() {
  module_invoke_all('sexybookmarks_init');
}

/**
 * Implements hook_theme().
 */
function sexybookmarks_theme() {
  $items = array();
  drupal_alter('sexybookmarks_theme', $items);
  return $items;
}

/**
 * Implements hook_theme().
 */
function sexybookmarks_form_alter(&$form, &$form_state, $form_id) {
  drupal_alter('sexybookmarks_form_alter', $form, $form_state, $form_id);
}
