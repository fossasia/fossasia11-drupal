<?php
// $Id: template.php,v 1.2 2010/12/28 21:18:31 skounis Exp $

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class($sidebar_left, $sidebar_right) {
  if ($sidebar_left != '' && $sidebar_right != '') {
    $class = 'sidebars';
  }
  else {
    if ($sidebar_left != '') {
      $class = 'sidebar-left';
    }
    if ($sidebar_right != '') {
      $class = 'sidebar-right';
    }
  }

  if (isset($class)) {
    print ' class="'. $class .'"';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
  }
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = null) {
  static $node_type;
  if (isset($type)) $node_type = $type;

  if (!$content || $node_type == 'forum') {
    return '<div id="comments">'. $content . '</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
  }
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function _phptemplate_variables($hook, $vars) {
  if ($hook == 'page') {

    if ($secondary = menu_secondary_local_tasks()) {
      $output = '<span class="clear"></span>';
      $output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
      $vars['tabs2'] = $output;
    }

    // Hook into color.module
    if (module_exists('color')) {
      _color_page_alter($vars);
    }
    return $vars;
  }
  return array();
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= "<ul class=\"tabs primary\">\n". $primary ."</ul>\n";
  }

  return $output;
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function phptemplate_get_ie_styles() {
  global $language;

  $iecss = '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . path_to_theme() .'/fix-ie.css" />';
  if ($language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="all">@import "'. base_path() . path_to_theme() .'/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}

/**
 * GT
 * Returns the terms of a specific vocabulary.
 * Paremeters: Node id, Vocabulary id
 *
 */
function skodaxanthifc_print_only_terms_of($node, $vid) {
	$terms = taxonomy_node_get_terms_by_vocabulary($node, $vid);
	if ($terms) {
		$links = array();
		$output .= '';
		foreach ($terms as $term) {
			//$links[] = l($term->name, taxonomy_term_path($term), array('rel' => 'tag', 'title' => strip_tags($term->description)));
			$output .= $term->name;
		}
	//$output .= implode(', ', $links);
	$output .= ' ';
	}
	$output .= '';
	return $output;
}

/**
  * Theme override for search form.
  */
  function skodaxanthifc_search_theme_form($form) {
   
    unset($form['search_theme_form']['#title']);
    //$form['submit']['#value'] = '';
    //$form['search_theme_form']['#value'] = 'Search.';
   // $form['search_theme_form']['#attributes'] = array('onblur' => "if (this.value == '') {this.value = 'Search.';}", 'onfocus' => "if (this.value == 'Search.') {this.value = '';}" );

    $output .= drupal_render($form);
    return $output;
}