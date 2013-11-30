<?php
// $Id: template.php,v 1.3 2010/04/24 13:19:56 antsin Exp $

/*
+----------------------------------------------------------------+
|   BlogBuzz for Dupal 6.x - Version 2.0                         |
|   Copyright (C) 2010 Antsin.com All Rights Reserved.           |
|   @license - GNU GENERAL PUBLIC LICENSE                        |
|----------------------------------------------------------------|
|   Theme Name: BlogBuzz                                         |
|   Description: BlogBuzz by Antsin                              |
|   Author: Antsin.com                                           |
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
    'blogbuzz_style' => 'chocolate',
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

function get_blogbuzz_style() {
  $style = theme_get_setting('blogbuzz_style');
  return $style;
}

drupal_add_css(drupal_get_path('theme', 'blogbuzz') . '/css/' . get_blogbuzz_style() . '.css', 'theme');

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
  if (!$vars['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    $classes[] = blogbuzz_id_safe('page-' . $path);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $classes[] = blogbuzz_id_safe('section-' . $section);
  }

  if (isset($vars['node'])) {
    $classes[] = ($vars['node']) ? 'full-node' : 'none-node';
  }
 
  $vars['body_classes_array'] = $classes;
  $vars['body_classes'] = implode(' ', $classes); // Concatenate with spaces.
  
  // Add content top & postscript classes with number of active sub-regions
  $region_list = array(
    'main_bottom' => array('main_bottom_one', 'main_bottom_two', 'main_bottom_three', 'main_bottom_four'), 
    'footer' => array('footer_one', 'footer_two', 'footer_three', 'footer_four')
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
function blogbuzz_preprocess_node(&$vars, $hook) {
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
  if ($vars['id'] == 1) {
    $classes[] = 'node-first';
  }
  if ($vars['uid'] && $vars['uid'] == $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  if ($vars['teaser']) {
    $classes[] = 'node-teaser'; // Node is displayed as teaser.
  }
  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $classes[] = blogbuzz_id_safe('node-type-' . $vars['type']);
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
function blogbuzz_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Special classes for blocks.
  $classes = array('block');
  $classes[] = 'block-' . $block->module;
  $classes[] = 'region-' . $vars['block_zebra'];
  $classes[] = $vars['zebra'];
  $classes[] = 'region-count-' . $vars['block_id'];
  $classes[] = 'count-' . $vars['id']; 

  // Render block classes.
  $vars['classes'] = implode(' ', $classes);
}

function blogbuzz_preprocess_comment(&$vars, $hook) {
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

function blogbuzz_preprocess_ddblock_cycle_block_content(&$vars) {
  if ($vars['output_type'] == 'view_fields') {
    $content = array();
    // Add slider_items for the template 
    // If you use the devel module uncomment the following line to see the theme variables
    // dsm($vars['settings']['view_name']);  
    // dsm($vars['content'][0]);
    // If you don't use the devel module uncomment the following line to see the theme variables
    // drupal_set_message('<pre>' . var_export($vars['settings']['view_name'], true) . '</pre>');
    // drupal_set_message('<pre>' . var_export($vars['content'][0], true) . '</pre>');
    if ($vars['settings']['view_name'] == 'showcase_with_image') {
      foreach ($vars['content'] as $key1 => $result) {
        // add slide_image variable 
        if (isset($result->node_data_field_pager_item_text_field_image_fid)) {
          // get image id
          $fid = $result->node_data_field_pager_item_text_field_image_fid;
          // get path to image
          $filepath = db_result(db_query("SELECT filepath FROM {files} WHERE fid = %d", $fid));
          //  use imagecache (imagecache, preset_name, file_path, alt, title, array of attributes)
          if (module_exists('imagecache') && is_array(imagecache_presets()) && $vars['imgcache_slide'] <> '<none>'){
            $slider_items[$key1]['slide_image'] = 
            theme('imagecache', 
                  $vars['imgcache_slide'], 
                  $filepath,
                  $result->node_title);
          }
          else {          
            $slider_items[$key1]['slide_image'] = 
              '<img src="' . base_path() . $filepath . 
              '" alt="' . $result->node_title . 
              '"/>';     
          }          
        }
        // add slide_text variable
        if (isset($result->node_data_field_pager_item_text_field_slide_text_value)) {
          $slider_items[$key1]['slide_text'] =  $result->node_data_field_pager_item_text_field_slide_text_value;
        }
        // add slide_title variable
        if (isset($result->node_title)) {
          $slider_items[$key1]['slide_title'] =  $result->node_title;
        }
        // add slide_read_more variable and slide_node variable
        if (isset($result->nid)) {
          $slider_items[$key1]['slide_read_more'] =  l('Continue', 'node/' . $result->nid);
          $slider_items[$key1]['slide_node'] =  'node/' . $result->nid;
        }
      }
      $vars['slider_items'] = $slider_items;  
    }    
  }
}  

/**
 * Override or insert variables into the ddblock_cycle_pager_content templates.
 *   Used to convert variables from view_fields  to pager_items template variables
 *  Only used for custom pager items
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 */
function blogbuzz_preprocess_ddblock_cycle_pager_content(&$vars) {
  if (($vars['output_type'] == 'view_fields') && ($vars['pager_settings']['pager'] == 'custom-pager')){
    $content = array();
    // Add pager_items for the template 
    // If you use the devel module uncomment the following lines to see the theme variables
    // dsm($vars['pager_settings']['view_name']);     
    // dsm($vars['content'][0]);     
    // If you don't use the devel module uncomment the following lines to see the theme variables
    // drupal_set_message('<pre>' . var_export($vars['pager_settings'], true) . '</pre>');
    // drupal_set_message('<pre>' . var_export($vars['content'][0], true) . '</pre>');
    if ($vars['pager_settings']['view_name'] == 'showcase_with_image') {
      foreach ($vars['content'] as $key1 => $result) {
        // add pager_item_image variable
        if (isset($result->node_data_field_pager_item_text_field_image_fid)) {
          $fid = $result->node_data_field_pager_item_text_field_image_fid;
          $filepath = db_result(db_query("SELECT filepath FROM {files} WHERE fid = %d", $fid));
          //  use imagecache (imagecache, preset_name, file_path, alt, title, array of attributes)
          if (module_exists('imagecache') && 
              is_array(imagecache_presets()) && 
              $vars['imgcache_pager_item'] <> '<none>'){
            $pager_items[$key1]['image'] = 
              theme('imagecache', 
                    $vars['pager_settings']['imgcache_pager_item'],              
                    $filepath,
                    $result->node_data_field_pager_item_text_field_pager_item_text_value);
          }
          else {          
            $pager_items[$key1]['image'] = 
              '<img src="' . base_path() . $filepath . 
              '" alt="' . $result->node_data_field_pager_item_text_field_pager_item_text_value . 
              '"/>';     
          }          
        }
        // add pager_item _text variable
        if (isset($result->node_data_field_pager_item_text_field_pager_item_text_value)) {
          $pager_items[$key1]['text'] =  $result->node_data_field_pager_item_text_field_pager_item_text_value;
        }
      }
    }
    $vars['pager_items'] = $pager_items;
  }    
}

/**
* Display the simple view of rows one after another
*/
function blogbuzz_preprocess_views_view_unformatted(&$vars) {
$view     = $vars['view'];
$rows     = $vars['rows'];

$vars['classes'] = array();
// Set up striping values.
foreach ($rows as $id => $row) {
   $vars['classes'][$id] = 'views-row views-row-' . ($id + 1);
   $vars['classes'][$id] .= ' views-row-' . ($id % 2 ? 'even' : 'odd');
   if ($id == 0) {
     $vars['classes'][$id] .= ' views-row-first';
   }
}
$vars['classes'][$id] .= ' views-row-last';
}

/**
* Override the search block form so we can change the label
* @return 
* @param $form Object
*/
function phptemplate_search_block_form($form) {
  $output = '';
  
  // the search_block_form element is the search's text field, it also happens to be the form id, so can be confusing
  $form['search_block_form']['#title'] = t('');
  $form['submit']['#value'] = 'Search';

  $output .= drupal_render($form);
  return $output;
}

function blogbuzz_links($links, $attributes = array('class' => 'links')) {
  $output = '';

  if (count($links) > 0) {
    $output = '<ul'. drupal_attributes($attributes) .'>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = $key;

      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) {
        $class .= ' first';
      }
      if ($i == $num_links) {
        $class .= ' last';
      }
      if (isset($link['href']) && $link['href'] == $_GET['q']) {
        $class .= ' active';
      }

	  $output .= '<li class="'. $class .'">';

     // Is the title HTML?
      $html = isset($link['html']) && $link['html'];

      // Initialize fragment and query variables.
      $link['query'] = isset($link['query']) ? $link['query'] : NULL;
      $link['fragment'] = isset($link['fragment']) ? $link['fragment'] : NULL;

      if (isset($link['href'])) {
        $link_options = array('attributes'  => $link['attributes'],
                              'query'       => $link['query'],
                              'fragment'    => $link['fragment'],
                              'absolute'    => FALSE,
                              'html'        => $html);
        $output .= l($link['title'], $link['href'], $link_options);
      }

      else if (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span'. $span_attributes .'>'. $link['title'] .'</span>';
      }

      $i++;
      $output .= "</li>";
      $output .= " \n";
    }
    $output .= '</ul>';
  }
  return $output;
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
function blogbuzz_id_safe($string) {
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
function blogbuzz_filter_tips($tips, $long = FALSE, $extra = '') {
  return '';
}

function blogbuzz_filter_tips_more_info () {
  return '';
}

// Override theme_button 
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