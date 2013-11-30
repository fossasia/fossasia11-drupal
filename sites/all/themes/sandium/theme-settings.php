<?php
// $Id$

/**
 * Implementation of hook_settings() for themes.
 */
function sandium_settings($settings) {
  drupal_add_js(drupal_get_path('theme', 'sandium') .'/js/superfish.js');
  drupal_add_js(drupal_get_path('theme', 'sandium') .'/js/supersub.js');
  drupal_add_css(drupal_get_path('theme', 'sandium') .'style.css');
  
  $block = module_invoke($menu, 'block', 'view', $delta);
  print $block['content'];
}
