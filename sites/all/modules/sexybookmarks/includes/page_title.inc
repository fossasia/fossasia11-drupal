<?php
/**
 * @file
 * Page Title module integration.
 */

/**
 * Implements hook_sexybookmarks_config_alter() on behalf of page_title.module.
 */
function page_title_sexybookmarks_config_alter($config, $variables) {
  if (!isset($variables[0]['title']) || $variables[0]['title'] == drupal_get_title() ? strip_tags(drupal_get_title()) : check_plain(variable_get('site_name', 'Drupal'))) {
    $variables[0]['title'] = page_title_page_get_title();
  }
}
