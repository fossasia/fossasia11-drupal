<?php
// $Id: lightbox2.insert.inc,v 1.1.2.6 2010/09/22 10:47:15 snpower Exp $

/**
 * @file
 * Lightbox2 support for Insert module.
 */

/**
 * Implementation of hook_insert_styles().
 */
function lightbox2_insert_styles() {
  $cck_formatters = lightbox2_field_formatter_info();
  $insert_styles = array();
  // Reformat CCK formatter ids as Insert style ids.
  foreach ($cck_formatters as $formatter_id => $formatter_info) {
    // Currently only the "imagefield--lightbox2" formatters are implemented as styles.
    if (preg_match('/^imagefield__lightbox2__((?:_(?!_)|[^_])+)__((?:_(?!_)|[^_])+)$/', $formatter_id, $matches)) {
      $style_id = 'lightbox2--' . $matches[1] . '--' . $matches[2];
      $insert_styles[$style_id] = $formatter_info;
    }
  }
  return $insert_styles;
}

/**
 * Implementation of hook_insert_content().
 */
function lightbox2_insert_content($item, $style, $widget) {
  if (preg_match('/^lightbox2--((?:-(?!-)|[^-])+)--((?:-(?!-)|[^-])+)$/', $style['name'], $matches)) {
    $image_preset_name = $matches[1];
    $link_preset_name = $matches[2];
    return theme('lightbox2_insert_image', $item, $widget, 'view', $image_preset_name, $link_preset_name);
  }
  else {
    return '';
  }
}

/**
 * Theme the content that will be inserted for Lightbox2 presets.
 */
function template_preprocess_lightbox2_insert_image(&$vars) {
  if ($vars['image_preset_name'] != 'original') {
    $filepath = imagecache_create_path($vars['image_preset_name'], $vars['item']['filepath']);
  }
  else {
    $filepath = $vars['item']['filepath'];
  }
  $vars['url'] = insert_create_url($filepath);

  if ($vars['link_preset_name'] != 'original') {
    $linkpath = imagecache_create_path($vars['link_preset_name'], $vars['item']['filepath']);
  }
  else {
    $linkpath = $vars['item']['filepath'];
  }
  $vars['linkurl'] = insert_create_url($linkpath);

  $vars['download_link'] = '';
  $download_link_text = check_plain(variable_get('lightbox2_download_link_text', 'Download Original'));
  if (!empty($download_link_text) && user_access('download original image')) {
    $vars['download_link'] = '<br /><br />' . l($download_link_text, $vars['linkurl'], array('attributes' => array('target' => '_blank', 'id' => 'lightbox2-download-link-text')));
  }

  $vars['class'] = !empty($vars['widget']['insert_class']) ? $vars['widget']['insert_class'] : '';
}

