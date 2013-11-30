<?php

require_once('theme-settings.php');


/**
 * Initialize theme settings
 */
global $theme_key;
if (db_is_active()) {
  fusion_core_initialize_theme_settings($theme_key);
}


/**
 * Maintenance page preprocessing
 */
function fusion_core_preprocess_maintenance_page(&$vars) {
  if (db_is_active()) {
    fusion_core_preprocess_page($vars);
  }
}


/**
 * Page preprocessing
 */
function fusion_core_preprocess_page(&$vars) {
  global $language, $theme_key, $theme_info, $user;

  // Remove sidebars if disabled e.g., for Panels
  if (!$vars['show_blocks']) {
    $vars['sidebar_first'] = '';
    $vars['sidebar_last'] = '';
  }
  // Set grid info & row widths
  $grid_name = substr(theme_get_setting('theme_grid'), 0, 7);
  $grid_type = substr(theme_get_setting('theme_grid'), 7);
  $grid_width = (int)substr($grid_name, 4, 2);
  $vars['grid_width'] = $grid_name . $grid_width;
  $sidebar_first_width = ($vars['sidebar_first']) ? theme_get_setting('sidebar_first_width') : 0;
  $sidebar_last_width = ($vars['sidebar_last']) ? theme_get_setting('sidebar_last_width') : 0;
  $vars['sidebar_first_width'] = $grid_name . $sidebar_first_width;
  $vars['main_group_width'] = $grid_name . ($grid_width - $sidebar_first_width);
  // For nested elements in a fluid grid calculate % widths & add inline
  if ($grid_type == 'fluid') {
    $sidebar_last_width = round(($sidebar_last_width/($grid_width - $sidebar_first_width)) * 100, 2);
    $vars['content_group_width'] = '" style="width:' . (100 - $sidebar_last_width) . '%';
    $vars['sidebar_last_width'] = '" style="width:' . $sidebar_last_width . '%';
  }
  else {
    $vars['content_group_width'] = $grid_name . ($grid_width - ($sidebar_first_width + $sidebar_last_width));
    $vars['sidebar_last_width'] = $grid_name . $sidebar_last_width;
  }

  // Add to array of helpful body classes
  $body_classes = explode(' ', $vars['body_classes']);                                               // Default classes
  if (isset($vars['node'])) {
    $body_classes[] = ($vars['node']) ? 'full-node' : '';                                            // Full node
    $body_classes[] = (($vars['node']->type == 'forum') || (arg(0) == 'forum')) ? 'forum' : '';      // Forum page
  }
  else {
    $body_classes[] = (arg(0) == 'forum') ? 'forum' : '';                                            // Forum page
  }
  if (module_exists('panels') && function_exists('panels_get_current_page_display')) {               // Panels page
    $body_classes[] = (panels_get_current_page_display()) ? 'panels' : '';
  }
  $body_classes[] = 'layout-'. (($vars['sidebar_first']) ? 'first-main' : 'main') . (($vars['sidebar_last']) ? '-last' : '');  // Sidebars active
  $body_classes[] = theme_get_setting('sidebar_layout');                                             // Sidebar layout
  $body_classes[] = (theme_get_setting('theme_font') != 'none') ? theme_get_setting('theme_font') : '';                        // Font family
  $body_classes[] = theme_get_setting('theme_font_size');                                            // Font size
  $body_classes[] = (user_access('administer blocks', $user) && theme_get_setting('grid_mask')) ? 'grid-mask-enabled' : '';    // Grid mask overlay
  $body_classes[] = 'grid-type-' . $grid_type;                                                       // Fixed width or fluid
  $body_classes[] = 'grid-width-' . sprintf("%02d", $grid_width);                                    // Grid width in units
  $body_classes[] = ($grid_type == 'fluid') ? theme_get_setting('fluid_grid_width') : '';            // Fluid grid width in %
  $body_classes = array_filter($body_classes);                                                       // Remove empty elements
  $vars['body_classes'] = implode(' ', $body_classes);                                               // Create class list separated by spaces
  
  // Add a unique css id for the body tag by converting / or + or _ in the current page alias into a dash (-).
  $vars['body_id'] = 'pid-' . strtolower(fusion_core_clean_css_identifier(drupal_get_path_alias($_GET['q'])));

  // Generate links tree & add Superfish class if dropdown enabled, else make standard primary links
  $vars['primary_links_tree'] = '';
  if ($vars['primary_links']) {
    if (theme_get_setting('primary_menu_dropdown') == 1) {
      // Check for menu internationalization
      if (module_exists('i18nmenu')) {
        $vars['primary_links_tree'] = i18nmenu_translated_tree(variable_get('menu_primary_links_source', 'primary-links'));
      }
      else {
        $vars['primary_links_tree'] = menu_tree(variable_get('menu_primary_links_source', 'primary-links'));
      }
      $vars['primary_links_tree'] = preg_replace('/<ul class="menu/i', '<ul class="menu sf-menu', $vars['primary_links_tree'], 1);
    }
    else {
      $vars['primary_links_tree'] = theme('links', $vars['primary_links'], array('class' => 'menu'));
    }
  }

  // Remove breadcrumbs if disabled
  if (theme_get_setting('breadcrumb_display') == 0) {
    $vars['breadcrumb'] = '';
  }

  // Add grid, color, ie6, ie7, ie8 & local stylesheets, including inherited & rtl versions
  $grid_style = '/css/' . theme_get_setting('theme_grid');
  $themes = fusion_core_theme_paths($theme_key);
  $vars['setting_styles'] = $vars['ie6_styles'] = $vars['ie7_styles'] = $vars['ie8_styles'] = $vars['local_styles'] = '';
  $query_string = '?'. substr(variable_get('css_js_query_string', '0'), 0, 1);
  foreach ($themes as $name => $path) {
    $link = '<link type="text/css" rel="stylesheet" media="all" href="' . base_path() . $path;
    $vars['setting_styles'] .= (file_exists($path . $grid_style . '.css')) ? $link . $grid_style . '.css' . $query_string . '"/>' . "\n" : '';
    $vars['ie6_styles'] .= (file_exists($path . '/css/ie6-fixes.css')) ? $link . '/css/ie6-fixes.css' . $query_string . '"/>' . "\n" : '';
    $vars['ie7_styles'] .= (file_exists($path . '/css/ie7-fixes.css')) ? $link . '/css/ie7-fixes.css' . $query_string . '" />' . "\n" : '';
    $vars['ie8_styles'] .= (file_exists($path . '/css/ie8-fixes.css')) ? $link . '/css/ie8-fixes.css' . $query_string . '" />' . "\n" : '';
    $vars['local_styles'] .= (file_exists($path . '/css/local.css')) ? $link . '/css/local.css' . $query_string . '" />' . "\n" : '';
    if (defined('LANGUAGE_RTL') && $language->direction == LANGUAGE_RTL) {
      $vars['setting_styles'] .= (file_exists($path . $grid_style . '-rtl.css')) ? $link . $grid_style . '-rtl.css" />' . "\n" : '';
      $vars['ie6_styles'] .= (file_exists($path . '/css/ie6-fixes-rtl.css')) ? $link . '/css/ie6-fixes-rtl.css' . $query_string . '" />' . "\n" : '';
      $vars['ie7_styles'] .= (file_exists($path . '/css/ie7-fixes-rtl.css')) ? $link . '/css/ie7-fixes-rtl.css' . $query_string . '" />' . "\n" : '';
      $vars['ie8_styles'] .= (file_exists($path . '/css/ie8-fixes-rtl.css')) ? $link . '/css/ie8-fixes-rtl.css' . $query_string . '" />' . "\n" : '';
      $vars['local_styles'] .= (file_exists($path . '/css/local-rtl.css')) ? $link . '/css/local-rtl.css' . $query_string . '" />' . "\n" : '';
    }
  }

  // Use grouped import setting to avoid 30 stylesheet limit in IE
  if (theme_get_setting('fix_css_limit') && !variable_get('preprocess_css', FALSE)) {
    $css = drupal_add_css();
    $style_count = substr_count($vars['setting_styles'] . $vars['ie6_styles'] . $vars['ie7_styles'] . $vars['ie8_styles'] . $vars['local_styles'], '<link');
    if (fusion_core_css_count($css) > (30 - $style_count)) {
      $styles = '';
      $suffix = "\n".'</style>'."\n";
      foreach ($css as $media => $types) {
        $prefix = '<style type="text/css" media="'. $media .'">'."\n";
        $imports = array();
        foreach ($types as $files) {
          foreach ($files as $file => $preprocess) {
            $imports[] = '@import "'. base_path() . $file .'";';
            if (count($imports) == 30) {
              $styles .= $prefix . implode("\n", $imports) . $suffix;
              $imports = array();
            }
          }
        }
        $styles .= (count($imports) > 0) ? ($prefix . implode("\n", $imports) . $suffix) : '';
      }
      $vars['styles'] = $styles;
    }
  }
  
  // Replace page title as Drupal core does, but strip tags from site slogan.
  // Site name and slogan do not need to be sanitized because the permission
  // 'administer site configuration' is required to be set and should be given to
  // trusted users only.
  // No sanitization will be applied when using the page title module.
  if (!module_exists('page_title')) {
    if (drupal_get_title()) {
      $head_title = array(strip_tags(drupal_get_title()), variable_get('site_name', 'Drupal'));
    }
    else {
      $head_title = array(variable_get('site_name', 'Drupal'));
      if (variable_get('site_slogan', '')) {
        $head_title[] = strip_tags(variable_get('site_slogan', ''));
      }
    }
    if (is_array($head_title)) $head_title = implode(' | ', $head_title);  
    $vars['head_title'] = $head_title;
  } 
}


/**
 * Node preprocessing
 */
function fusion_core_preprocess_node(&$vars) {
  // Build array of handy node classes
  $node_classes = array();
  $node_classes[] = $vars['zebra'];                                      // Node is odd or even
  $node_classes[] = (!$vars['node']->status) ? 'node-unpublished' : '';  // Node is unpublished
  $node_classes[] = ($vars['sticky']) ? 'sticky' : '';                   // Node is sticky
  $node_classes[] = $vars['teaser'] ? 'teaser' : 'full-node';            // Node is teaser or full-node
  $node_classes[] = 'node-type-'. $vars['node']->type;                   // Node is type-x, e.g., node-type-page
  $node_classes[] = (isset($vars['skinr'])) ? $vars['skinr'] : '';       // Add Skinr classes if present
  $node_classes = array_filter($node_classes);                           // Remove empty elements
  $vars['node_classes'] = implode(' ', $node_classes);                   // Implode class list with spaces

  // Add node_top and node_bottom region content
  $vars['node_top'] = theme('blocks', 'node_top');
  $vars['node_bottom'] = theme('blocks', 'node_bottom');

  // Render Ubercart fields into separate variables for node-product.tpl.php
  if (module_exists('uc_product') && uc_product_is_product($vars) && $vars['template_files'][0] == 'node-product') {
    $node = node_build_content(node_load($vars['nid']), $vars['teaser'], $vars['page']);
    $vars['fusion_uc_image'] = drupal_render($node->content['image']);
    $vars['fusion_uc_body'] = drupal_render($node->content['body']);
    $vars['fusion_uc_display_price'] = drupal_render($node->content['display_price']);
    $vars['fusion_uc_add_to_cart'] = drupal_render($node->content['add_to_cart']);
    $vars['fusion_uc_sell_price'] = drupal_render($node->content['sell_price']);
    $vars['fusion_uc_cost'] = drupal_render($node->content['cost']);
    $vars['fusion_uc_weight'] = (!empty($node->weight)) ? drupal_render($node->content['weight']) : '';   // Hide weight if empty
    if ($vars['fusion_uc_weight'] == '') {
      unset($node->content['weight']);
    }
    $dimensions = !empty($node->height) && !empty($node->width) && !empty($node->length);                 // Hide dimensions if empty
    $vars['fusion_uc_dimensions'] = ($dimensions) ? drupal_render($node->content['dimensions']) : '';
    if ($vars['fusion_uc_dimensions'] == '') {
      unset($node->content['dimensions']);
    }
    $list_price = !empty($node->list_price) && $node->list_price > 0;                                     // Hide list price if empty or zero
    $vars['fusion_uc_list_price'] = ($list_price) ? drupal_render($node->content['list_price']) : '';
    if ($vars['fusion_uc_list_price'] == '') {
      unset($node->content['list_price']);
    }
    $vars['fusion_uc_additional'] = drupal_render($node->content);                                        // Render remaining fields
  }
}


/**
 * Comment preprocessing
 */
function fusion_core_preprocess_comment(&$vars) {
  global $user;
  static $comment_odd = TRUE;                                                                             // Comment is odd or even

  // Build array of handy comment classes
  $comment_classes = array();
  $comment_classes[] = $comment_odd ? 'odd' : 'even';
  $comment_odd = !$comment_odd;
  $comment_classes[] = ($vars['comment']->status == COMMENT_NOT_PUBLISHED) ? 'comment-unpublished' : '';  // Comment is unpublished
  $comment_classes[] = ($vars['comment']->new) ? 'comment-new' : '';                                      // Comment is new
  $comment_classes[] = ($vars['comment']->uid == 0) ? 'comment-by-anon' : '';                             // Comment is by anonymous user
  $comment_classes[] = ($user->uid && $vars['comment']->uid == $user->uid) ? 'comment-mine' : '';         // Comment is by current user
  $vars['author_comment'] = ($vars['comment']->uid == $vars['node']->uid) ? TRUE : FALSE;                 // Comment is by node author
  $comment_classes[] = ($vars['author_comment']) ? 'comment-by-author' : '';
  $comment_classes = array_filter($comment_classes);                                                      // Remove empty elements
  $vars['comment_classes'] = implode(' ', $comment_classes);                                              // Create class list separated by spaces
}


/**
 * Comment wrapper preprocessing
 * Defaults for comments display
 */
function fusion_core_preprocess_comment_wrapper(&$vars) {
  $vars['display_mode'] = COMMENT_MODE_FLAT_EXPANDED;
  $vars['display_order'] = COMMENT_ORDER_OLDEST_FIRST;
  $vars['comment_controls_state'] = COMMENT_CONTROLS_HIDDEN;
}

/**
 * Returns a list of blocks.  
 * Uses Drupal block interface and appends any blocks assigned by the Context module.
 */
function fusion_core_block_list($region) {
  $drupal_list = block_list($region);
  if (module_exists('context') && $context = context_get_plugin('reaction', 'block')) {
    $context_list = $context->block_list($region);
    $drupal_list = array_merge($context_list, $drupal_list);
  }
  return $drupal_list;
}

/**
 * Block preprocessing
 */
function fusion_core_preprocess_block(&$vars) {
  global $theme_info, $user;
  static $regions, $sidebar_first_width, $sidebar_last_width, $grid_name, $grid_width, $grid_fixed;
  
  // Initialize position to avoid notice if function returns.
  $vars['position'] = '';  

  // Do not process blocks outside defined regions
  if (!in_array($vars['block']->region, array_keys($theme_info->info['regions']))) {
    return;
  }

  // Initialize block region grid info once per page
  if (!isset($regions)) {
    $grid_name = substr(theme_get_setting('theme_grid'), 0, 7);
    $grid_width = (int)substr($grid_name, 4, 2);
    $grid_fixed = (substr(theme_get_setting('theme_grid'), 7) != 'fluid') ? 1 : 0;
    $sidebar_first_width = (fusion_core_block_list('sidebar_first')) ? theme_get_setting('sidebar_first_width') : 0;
    $sidebar_last_width = (fusion_core_block_list('sidebar_last')) ? theme_get_setting('sidebar_last_width') : 0;
    $regions = fusion_core_set_regions($grid_width, $sidebar_first_width, $sidebar_last_width);
  }

  // Increment block count for current block's region, add first/last position class
  $regions[$vars['block']->region]['count']++;
  $region_count = $regions[$vars['block']->region]['count'];
  $total_blocks = $regions[$vars['block']->region]['total'];
  $vars['position'] = ($region_count == 1) ? 'first' : '';
  $vars['position'] .= ($region_count == $total_blocks) ? ' last' : '';

  // Set a default block width if not already set by Skinr
  if (!isset($vars['skinr']) || (strpos($vars['skinr'], $grid_name) === false)) {
    // Stack blocks vertically in sidebars by setting to full sidebar width
    if ($vars['block']->region == 'sidebar_first') {
      $width = ($grid_fixed) ? $sidebar_first_width : $grid_width;  // Sidebar width or 100% (if fluid)
    }
    elseif ($vars['block']->region == 'sidebar_last') {
      $width = ($grid_fixed) ? $sidebar_last_width : $grid_width;  // Sidebar width or 100% (if fluid)
    }
    else {
      // Default block width = region width divided by total blocks, adding any extra width to last block
      $region_width = ($grid_fixed) ? $regions[$vars['block']->region]['width'] : $grid_width;  // fluid grid regions = 100%
      $width_adjust = (($region_count == $total_blocks) && ($region_width % $total_blocks)) ? $region_width % $total_blocks : 0;
      $width = ($total_blocks) ? floor($region_width / $total_blocks) + $width_adjust : 0;
    }
    $vars['skinr'] = (isset($vars['skinr'])) ? $vars['skinr'] . ' ' . $grid_name . $width : $grid_name . $width;
  }

  if (isset($vars['skinr']) && (strpos($vars['skinr'], 'superfish') !== false) &&
     ($vars['block']->module == 'menu' || ($vars['block']->module == 'user' && $vars['block']->delta == 1))) {
    $superfish = ' sf-menu';
    $superfish .= (strpos($vars['skinr'], 'superfish-vertical')) ? ' sf-vertical' : '';
    $vars['block']->content = preg_replace('/<ul class="menu/i', '<ul class="menu' . $superfish, $vars['block']->content, 1);
  }

  // Add block edit links for admins
  if (user_access('administer blocks', $user) && theme_get_setting('block_config_link')) {
    $vars['edit_links'] = '<div class="fusion-edit">'. implode(' ', fusion_core_edit_links($vars['block'])) .'</div>';
  }
}


/**
 * Views preprocessing
 * Add view type class (e.g., node, teaser, list, table)
 */
function fusion_core_preprocess_views_view(&$vars) {
  $vars['css_name'] = $vars['css_name'] .' view-style-'. views_css_safe(strtolower($vars['view']->type));
}


/**
 * Search result preprocessing
 */
function fusion_core_preprocess_search_result(&$vars) {
  static $search_zebra = 'even';

  $search_zebra = ($search_zebra == 'even') ? 'odd' : 'even';
  $vars['search_zebra'] = $search_zebra;
  $result = $vars['result'];
  $vars['url'] = check_url($result['link']);
  $vars['title'] = check_plain($result['title']);

  // Check for snippet existence. User search does not include snippets.
  $vars['snippet'] = '';
  if (isset($result['snippet']) && theme_get_setting('search_snippet')) {
    $vars['snippet'] = $result['snippet'];
  }

  $info = array();
  if (!empty($result['type']) && theme_get_setting('search_info_type')) {
    $info['type'] = check_plain($result['type']);
  }
  if (!empty($result['user']) && theme_get_setting('search_info_user')) {
    $info['user'] = $result['user'];
  }
  if (!empty($result['date']) && theme_get_setting('search_info_date')) {
    $info['date'] = format_date($result['date'], 'small');
  }
  if (isset($result['extra']) && is_array($result['extra'])) {
    // $info = array_merge($info, $result['extra']);  Drupal bug?  [extra] array not keyed with 'comment' & 'upload'
    if (!empty($result['extra'][0]) && theme_get_setting('search_info_comment')) {
      $info['comment'] = $result['extra'][0];
    }
    if (!empty($result['extra'][1]) && theme_get_setting('search_info_upload')) {
      $info['upload'] = $result['extra'][1];
    }
  }

  // Provide separated and grouped meta information.
  $vars['info_split'] = $info;
  $vars['info'] = implode(' - ', $info);

  // Provide alternate search result template.
  $vars['template_files'][] = 'search-result-'. $vars['type'];
}


/**
 * Username override
 * Hides or shows username '(not verified)' text
 */
function fusion_core_username($object) {
  if ((!$object->uid) && $object->name) {
    $output = (!empty($object->homepage)) ? l($object->name, $object->homepage, array('attributes' => array('rel' => 'nofollow'))) : check_plain($object->name);
    $output .= (theme_get_setting('user_notverified_display') == 1) ? t(' (not verified)') : '';
  }
  else {
    $output = theme_username($object);
  }
  return $output;
}


/**
 * File element override
 * Sets form file input max width
 */
function fusion_core_file($variables) {
  $variables['element']['#size'] = (!isset($variables['element']['#size']) || $variables['element']['#size'] > 40) ? 40 : $variables['element']['#size'];
  return theme_file($variables);
}


/**
 * Custom theme functions
 */
function fusion_core_theme() {
  return array(
    'grid_row' => array(
      'arguments' => array('element' => NULL, 'name' => NULL, 'class' => NULL, 'width' => NULL),
    ),
    'grid_block' => array(
      'arguments' => array('element' => NULL, 'name' => NULL),
    ),
  );
}


/**
 * Row & block theme functions
 * Adds divs to elements in page.tpl.php
 */
function fusion_core_grid_row($element, $name, $class='', $width='', $extra='') {
  $output = '';
  $extra = ($extra) ? ' ' . $extra : '';
  if ($element) {
    if ($class == 'full-width') {
      $output .= '<div id="' . $name . '-wrapper" class="' . $name . '-wrapper full-width">' . "\n";
      $output .= '<div id="' . $name . '" class="' . $name . ' row ' . $width . $extra . '">' . "\n";
    }
    else {
      $output .= '<div id="' . $name . '" class="' . $name . ' row ' . $class . ' ' . $width . $extra . '">' . "\n";
    }
    $output .= '<div id="' . $name . '-inner" class="' . $name . '-inner inner clearfix">' . "\n";
    $output .= $element;
    $output .= '</div><!-- /' . $name . '-inner -->' . "\n";
    $output .= '</div><!-- /' . $name . ' -->' . "\n";
    $output .= ($class == 'full-width') ? '</div><!-- /' . $name . '-wrapper -->' . "\n" : '';
  }
  return $output;
}


function fusion_core_grid_block($element, $name) {
  $output = '';
  if ($element) {
    $output .= '<div id="' . $name . '" class="' . $name . ' block">' . "\n";
    $output .= '<div id="' . $name . '-inner" class="' . $name . '-inner inner clearfix">' . "\n";
    $output .= $element;
    $output .= '</div><!-- /' . $name . '-inner -->' . "\n";
    $output .= '</div><!-- /' . $name . ' -->' . "\n";
  }
  return $output;
}


/**
 * Block region grid info function
 * Defaults match grid_row widths set in preprocess_page()
 */
function fusion_core_set_regions($grid_width, $sidebar_first_width, $sidebar_last_width) {
  $sidebar_total = $sidebar_first_width + $sidebar_last_width;
  $regions = array(
    'header_top' => array('width' => $grid_width, 'total' => count(fusion_core_block_list('header_top')), 'count' => 0),
    'header' => array('width' => $grid_width, 'total' => count(fusion_core_block_list('header')), 'count' => 0),
    'preface_top' => array('width' => $grid_width, 'total' => count(fusion_core_block_list('preface_top')), 'count' => 0),
    'preface_bottom' => array('width' => $grid_width - $sidebar_first_width, 'total' => count(fusion_core_block_list('preface_bottom')), 'count' => 0),
    'sidebar_first' => array('width' => $sidebar_first_width, 'total' => count(fusion_core_block_list('sidebar_first')), 'count' => 0),
    'content_top' => array('width' => $grid_width - $sidebar_total, 'total' => count(fusion_core_block_list('content_top')), 'count' => 0),
    'content' => array('width' => $grid_width - $sidebar_total, 'total' => count(fusion_core_block_list('content')), 'count' => 0),
    'node_top' => array('width' => $grid_width - $sidebar_total, 'total' => count(fusion_core_block_list('node_top')), 'count' => 0),
    'node_bottom' => array('width' => $grid_width - $sidebar_total, 'total' => count(fusion_core_block_list('node_bottom')), 'count' => 0),
    'content_bottom' => array('width' => $grid_width - $sidebar_total, 'total' => count(fusion_core_block_list('content_bottom')), 'count' => 0),
    'sidebar_last' => array('width' => $sidebar_last_width, 'total' => count(fusion_core_block_list('sidebar_last')), 'count' => 0),
    'postscript_top' => array('width' => $grid_width - $sidebar_first_width, 'total' => count(fusion_core_block_list('postscript_top')), 'count' => 0),
    'postscript_bottom' => array('width' => $grid_width, 'total' => count(fusion_core_block_list('postscript_bottom')), 'count' => 0),
    'footer' => array('width' => $grid_width, 'total' => count(fusion_core_block_list('footer')), 'count' => 0)
  );
  return $regions;
}


/**
 * Block edit links function
 * Create block edit links for admins
 */
function fusion_core_edit_links($block) {
  $path = 'admin/build/block/configure/' . $block->module . '/' . $block->delta;
  $return = drupal_get_destination();
  // Use 'edit' for custom blocks, 'configure' for others
  if ($block->module == 'block') {
    $text = t('edit block');
    $block_info = array('@region' => str_replace('_', ' ', $block->region));
    $attributes = array('title' => t('edit the content of this Custom block (in @region)', $block_info), 'class' => 'fusion-block-edit');
  }
  else {
    $text = t('configure block');
    $block_info = array('@type' => ucwords($block->module), '@region' => str_replace('_', ' ', $block->region));
    $attributes = array('title' => t('configure this @type block (in @region)', $block_info), 'class' => 'fusion-block-config');
  }
  $edit_links[] = l($text, $path, array('attributes' => $attributes, 'query' => $return));
  // Add extra 'edit menu' for menu blocks
  if (user_access('administer menu') && ($block->module == 'menu' || ($block->module == 'user' && $block->delta == 1))) {
    $text = t('edit menu');
    $path = 'admin/build/menu-customize/' . (($block->module == 'user') ? 'navigation' : $block->delta);
    $attributes = array('title' => t('edit the menu of this @type block (in @region)', $block_info), 'class' => 'fusion-edit-menu');
    $edit_links[] = l($text, $path, array('attributes' => $attributes, 'query' => $return));
  }
  return $edit_links;
}


/**
 * CSS count function
 * Counts the total number of CSS files in $vars['css']
 */
function fusion_core_css_count($array) {
  $count = 0;
  foreach ($array as $item) {
    $count = (is_array($item)) ? $count + fusion_core_css_count($item) : $count + 1;
  }
  return $count;
}


/**
 * Theme paths function
 * Retrieves current theme path and its parent
 * theme paths, in parent-to-child order.
 */
function fusion_core_theme_paths($theme) {
  $all_parents = array();
  $themes = list_themes();
  $all_parents[$theme] = drupal_get_path('theme', $theme);
  $base_theme = $themes[$theme]->info['base theme'];
  while ($base_theme) {
    $all_parents[$base_theme] = drupal_get_path('theme', $base_theme);
    $base_theme = (isset($themes[$base_theme]->info['base theme'])) ? $themes[$base_theme]->info['base theme'] : '';
  }
  return array_reverse($all_parents);
}


/**
 * Theme settings link function
 * Creates link with prefix and suffix text
 * ($options info: http://api.drupal.org/api/function/l)
 */
function fusion_core_themesettings_link($prefix, $suffix, $text, $path, $options) {
  return $prefix . (($text) ? l($text, $path, $options) : '') . $suffix;
}

/**
 * @function fusion_core_clean_css_identifier()
 *   backport of drupal_clean_css_identifier() from Drupal 7.x
 *
 * @param $identifier
 *   the identifier to clean
 * @param $filter
 *   an array of string replacements to use on the identifier 
 *
 * @return
 *   A string safe for use as a CSS class or ID
 **/
 
 function fusion_core_clean_css_identifier($identifier, $filter = array(' ' => '-', '_' => '-', '/' => '-', '[' => '-', ']' => '')) {
 
   // By default, we filter using Drupal's coding standards.
   $identifier = strtr($identifier, $filter);
 
   // Valid characters in a CSS identifier are:
   // - the hyphen (U+002D)
   // - a-z (U+0030 - U+0039)
   // - A-Z (U+0041 - U+005A)
   // - the underscore (U+005F)
   // - 0-9 (U+0061 - U+007A)
   // - ISO 10646 characters U+00A1 and higher
   // We strip out any character not in the above list.
   $identifier = preg_replace('/[^\x{002D}\x{0030}-\x{0039}\x{0041}-\x{005A}\x{005F}\x{0061}-\x{007A}\x{00A1}-\x{FFFF}]/u', '', $identifier);
 
   return $identifier;
 
 }
