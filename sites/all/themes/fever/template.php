<?php
// $Id: template.php,v 1.1 2010/08/05 07:51:20 antsin Exp $

/*
+----------------------------------------------------------------+
|   Fever for Dupal 6.x - Version 1.0                            |
|   Copyright (C) 2010 Antsin.com All Rights Reserved.           |
|   @license - GNU GENERAL PUBLIC LICENSE                        |
|----------------------------------------------------------------|
|   Theme Name: Fever                                            |
|   Description: Fever by Antsin                                 |
|   Author: Antsin.com                                           |
|   Date: 5th August 2010                                        |
|   Website: http://www.antsin.com/                              |
|----------------------------------------------------------------+
*/  

/**
 * Initialize theme settings
 */
if (is_null(theme_get_setting('user_notverified_display'))) {
  global $theme_key;
  // Get node types
  $node_types = node_get_types('names');
  
  /**
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the theme-settings.php file.
   */
  $defaults = array(
    'fever_style' => 'orange',
  );
  
  // Make the default content-type settings the same as the default theme settings,
  // so we can tell if content-type-specific settings have been altered.
  $defaults = array_merge($defaults, theme_get_settings());
    
  // Get default theme settings.
  $settings = theme_get_settings($theme_key);  
  
  // Don't save the toggle_node_info_ variables
  if (module_exists('node')) {
    foreach (node_get_types() as $type => $name) {
      unset($settings['toggle_node_info_'. $type]);
    }
  }

  // Save default theme settings
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  // Force refresh of Drupal internals
  theme_get_setting('', TRUE);
}

function phptemplate_preprocess_page(&$vars) {

  // Add conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  $classes = split(' ', $vars['body_classes']);
  // Remove the mostly useless page-ARG0 class.
  if ($index = array_search(preg_replace('![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s', '', 'page-'. drupal_strtolower(arg(0))), $classes)) {
    unset($classes[$index]);
  }

  $vars['body_classes_array'] = $classes;
  $vars['body_classes'] = implode(' ', $classes); // Concatenate with spaces.
  
  // Add content top & postscript classes with number of active sub-regions
  $region_list = array(
    'content_bottom' => array('content_bottom_one', 'content_bottom_two', 'content_bottom_three', 'content_bottom_four'), 
  );

  foreach ($region_list as $sub_region_key => $sub_region_list) {
    $active_regions = array();
    foreach ($sub_region_list as $region_item) {
      if ($vars[$region_item]) {
        $active_regions[] = $region_item;
      }
    }
    $vars[$sub_region_key] = $sub_region_key .'-'. strval(count($active_regions));
  }
  
  // Generate menu tree from source of primary links
  $vars['primary_links_tree'] = menu_tree(variable_get('menu_primary_links_source', 'primary-links'));
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function fever_preprocess_node(&$vars, $hook) {
  // Special classes for nodes
  $classes = array('node');
  if ($vars['sticky']) {
    $classes[] = 'sticky';
  }
  if (!$vars['status']) {
    $classes[] = 'node-unpublished';
    $vars['unpublished'] = TRUE;
  }
  else {
    $vars['unpublished'] = FALSE;
  }
  if ($vars['uid'] && $vars['uid'] == $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  if ($vars['id'] == 1) {
    $classes[] = 'node-first';
  }
  if ($vars['teaser']) {
    $classes[] = 'node-teaser'; // Node is displayed as teaser.
  }
  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $classes[] = fever_id_safe('node-type-' . $vars['type']);
  $vars['classes'] = implode(' ', $classes); // Concatenate with spaces
  if (module_exists('taxonomy')) {
    $term_links = array();
    foreach ($vars['node']->taxonomy as $term) {
      $term_links[] = l($term->name, 'taxonomy/term/' . $term->tid,
        array(
          'attributes' => array(
            'title' => $term->description
        )));
    }
    $vars['node_terms'] = implode(', ', $term_links);
  }
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function fever_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Special classes for blocks.
  $classes = array('block');
  $classes[] = 'block-' . $block->module;
  $classes[] = 'region-count-' . $vars['block_id'];

  // Render block classes.
  $vars['classes'] = implode(' ', $classes);
}

function fever_preprocess_comment(&$vars, $hook) {
  // Add an "unpublished" flag.
  $vars['unpublished'] = ($vars['comment']->status == COMMENT_NOT_PUBLISHED);

  // If comment subjects are disabled, don't display them.
  if (variable_get('comment_subject_field_' . $vars['node']->type, 1) == 0) {
    $vars['title'] = '';
  }

  // Special classes for comments.
  $classes = array('comment');
  if ($vars['comment']->new) {
    $classes[] = 'comment-new';
  }
  $classes[] = $vars['status'];
  $classes[] = $vars['zebra'];
  if ($vars['id'] == 1) {
    $classes[] = 'first';
  }
  if ($vars['id'] == $vars['node']->comment_count) {
    $classes[] = 'last';
  }
  if ($vars['comment']->uid == 0) {
    // Comment is by an anonymous user.
    $classes[] = 'comment-by-anon';
  }
  else {
    if ($vars['comment']->uid == $vars['node']->uid) {
      // Comment is by the node author.
      $classes[] = 'comment-by-author';
    }
    if ($vars['comment']->uid == $GLOBALS['user']->uid) {
      // Comment was posted by current user.
      $classes[] = 'comment-mine';
    }
  }
  $vars['classes'] = implode(' ', $classes);
}

/**
 * Override or insert variables into the views_slideshow_ddblock_cycle_block_content templates.
 *   Used to convert variables from view_fields to slider_items template variables
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * 
 */
function fever_preprocess_views_slideshow_ddblock(&$vars) {
  drupal_rebuild_theme_registry();
  $settings = $vars['views_slideshow_ddblock_slider_settings'];
  // for showing debug info
  views_slideshow_ddblock_show_content_debug_info($vars);
  if ($settings['output_type'] == 'view_fields') {
    if ($settings['view_name'] == 'showcase_with_image' && $settings['view_display_id'] == 'block_1') {
      if (!empty($vars['views_slideshow_ddblock_content'])) {
        foreach ($vars['views_slideshow_ddblock_content'] as $key1 => $result) {
          // add slide image variable
          $slider_items[$key1]['slide_image'] = views_slideshow_ddblock_add_image(
            $vars,
            // determines which imagcache preset to use, leave to 'slider_item_image'
            'slider_item_image',
            // name of CCK generated image field, change if needed.
            $result->node_data_field_pager_item_text_field_image_fid,
            // alt attribute of image or NULL
            $result->node_title,
            // cck content type for default image or NULL, change if needed
            NULL, //'ddblock_news_item',
            // cck fieldname for default image or NULL, change if needed
            NULL, //'field_image',
            // to link the image to or NULL, change if needed
            NULL // base_path() . 'node/' . $result->nid
          );
          // add slide_text variable
          if (isset($result->node_data_field_pager_item_text_field_slide_text_value)) {
            $slider_items[$key1]['slide_text'] =  check_markup($result->node_data_field_pager_item_text_field_slide_text_value);
          }
          // add slide_title variable
          if (isset($result->node_title)) {
            $slider_items[$key1]['slide_title'] =  check_plain($result->node_title);
          }
          // add slide_read_more variable and slide_node variable
          if (isset($result->nid)) {
            $slider_items[$key1]['slide_read_more'] = '<a href="' . base_path() . 'node/' .  $result->nid . '">' . t('more &raquo;') . '</a>';
            $slider_items[$key1]['slide_node'] = base_path() . 'node/' . $result->nid;
          }
        }
      }
    }    
    $vars['views_slideshow_ddblock_slider_items'] = $slider_items;
  }
}

/**
 * Override or insert variables into the views_slideshow_ddblock_cycle_pager_content templates.
 *   Used to convert variables from view_fields  to pager_items template variables
 *  Only used for custom pager items
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 */
function fever_preprocess_views_slideshow_ddblock_pager_content(&$vars) {
  $settings = $vars['views_slideshow_ddblock_pager_settings'];
  // for showing debug info
  views_slideshow_ddblock_show_pager_debug_info($vars);
  if (($settings['output_type'] == 'view_fields') && 
      ($settings['pager'] == 'number-pager' || 
      $settings['pager'] == 'custom-pager' ||
      $settings['pager'] == 'scrollable-pager' )) {
    if ($settings['view_name'] == 'showcase_with_image' && $settings['view_display_id'] == 'block_1') {
      if (!empty($vars['views_slideshow_ddblock_content'])) {
        foreach ($vars['views_slideshow_ddblock_content'] as $key1 => $result) {
          // add pager_item_image variable
          $pager_items[$key1]['image'] = views_slideshow_ddblock_add_image(
            $vars,
            // determines which imagcache preset to use, leave to 'pager_item_image'
            'pager_item_image',
            // name of CCK generated image field, change if needed.
            $result->node_data_field_pager_item_text_field_image_fid,
            // alt attribute of image or NULL
            $result->node_data_field_pager_item_text_field_pager_item_text_value,
            // cck content type for default image or NULL, change if needed
            NULL, //'ddblock_news_item',
            // cck fieldname for default image or NULL, change if needed
            NULL, //'field_image',
            // to link the image to or NULL, change if needed
            NULL // base_path() . 'node/' . $result->nid
          );
          // add pager_item _text variable
          if (isset($result->node_data_field_pager_item_text_field_pager_item_text_value)) {
            $pager_items[$key1]['text'] =  check_plain($result->node_data_field_pager_item_text_field_pager_item_text_value);
          }
		  // add slide_title variable
          if (isset($result->node_title)) {
            $pager_items[$key1]['slide_title'] =  check_plain($result->node_title);
          }
        }
      }
    }
    $vars['views_slideshow_ddblock_pager_items'] = $pager_items;
  }    
}

/**
* Override the search theme form so we can change the label
* @return 
* @param $form Object
*/
function phptemplate_search_theme_form($form) {
  $output = '';
  
  $form['search_theme_form']['#title'] = t('');
  $form['submit']['#value'] = '';

  $output .= drupal_render($form);
  return $output;
}

function phptemplate_search_block_form($form) {
  $output = '';
  
  // the search_block_form element is the search's text field, it also happens to be the form id, so can be confusing
  $form['search_block_form']['#title'] = t('');

  $output .= drupal_render($form);
  return $output;
}

function fever_menu_item_link($link) {
  if (empty($link['options'])) {
    $link['options'] = array();
  }

  if (empty($link['type'])) {
    $true = TRUE;
  }
  
  // Do special stuff for PRIMARY LINKS here
  if ($link['menu_name'] == 'primary-links') {
    $link['title'] = '<span>' . check_plain($link['title']) . '</span>';
    $link['options']['html'] = TRUE;
  }

  return l($link['title'], $link['href'], $link['options']);
}

/**
 * Converts a string to a suitable html ID attribute.
 *
 * http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 * valid ID attribute in HTML. This function:
 *
 * - Ensure an ID starts with an alpha character by optionally adding an 'id'.
 * - Replaces any character except alphanumeric characters with dashes.
 * - Converts entire string to lowercase.
 *
 * @param $string
 *   The string
 * @return
 *   The converted string
 */
function fever_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
  // If the first character is not a-z, add 'id' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id' . $string;
  }
  return $string;
}

/*
* Override filter.module's theme_filter_tips() function to disable tips display.
*/
function fever_filter_tips($tips, $long = FALSE, $extra = '') {
  return '';
}

function fever_filter_tips_more_info () {
  return '';
}

/**
 * Function spanify firstword 
 */
function wordlimit($string, $length = 50, $ellipsis = "...") {
  $words = explode(' ', strip_tags($string));
  if (count($words) > $length)
    return implode(' ', array_slice($words, 0, $length)) . $ellipsis;
  else
    return $string;
}

/**
 * Override theme_button 
 */
function phptemplate_button($element) { 
  // Make sure not to overwrite classes. 
  if (isset($element['#attributes']['class'])) { 
    $element['#attributes']['class'] = 'form-'. $element['#button_type'] .' '. $element['#attributes']['class']; 
  }
  else { 
    $element['#attributes']['class'] = 'form-'. $element['#button_type']; 
  } 
  // We here wrap the output with a couple span tags 
  return '<span class="button"><input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ') .'id="'. $element['#id'].'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']) ." /></span>\n"; 
}