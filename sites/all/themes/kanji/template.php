<?php // $Id$
function kanji_preprocess_page(&$vars, $hook) { 
  $classes = split(' ', $vars['body_node_classes']);
  if($vars['sidebar_first'] && $vars['sidebar_last']) {
    $classes[] = t('two-sidebars');
  } else {
    if($vars['sidebar_first'] || $vars['sidebar_last']) {
      $classes[] = t('one-sidebar');
    } else {
      $classes[] = t('no-sidebars');
    }
  }
  
  $vars['body_node_classes_array'] = $classes;
  $vars['body_node_classes'] = implode(' ', $classes);
  $vars['primary_links_tree'] = '';
  
  if ($vars['primary_links']) {
    if (module_exists('i18nmenu')) {
      $vars['primary_links_tree'] = i18nmenu_translated_tree(variable_get('menu_primary_links_source', 'primary-links'));
    } else {
      $vars['primary_links_tree'] = menu_tree(variable_get('menu_primary_links_source', 'primary-links'));
    }
    $vars['primary_links_tree'] = preg_replace('/<ul class="menu/i', '<ul class="menu sf-menu', $vars['primary_links_tree'], 1);
  } else {
    $vars['primary_links_tree'] = theme('links', $vars['primary_links'], array('class' => 'menu'));
  }
}

function kanji_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' | ', $breadcrumb) .'</div>';
  }
}

function kanji_feed_icon($url, $title) {
  if ($image = theme('image', path_to_theme() . '/images/rss.png', t('Syndicate content'), $title)) {
    return '<a href="'. check_url($url) .'" class="feed-icon">'. $image .'</a>';
  }
}


