<?php
// $Id: dhtml_menu.admin.inc,v 1.5.2.3 2008/11/08 17:35:12 arancaytar Exp $

/**
 * @file dhtml_menu.admin.inc
 * Functions that are only called on the admin pages.
 */

/**
 * Module settings form.
 */
function dhtml_menu_settings(&$form_state) {
  $form['dhtml_menu_effects'] = array(
    '#type' => 'checkboxes',
    '#title' => t('DHTML Menu effects'),
    '#options' => array(
      'slide' => t('Animated sliding effect'),
      'siblings' => t('Keep only one menu open at a time.'),
      'relativity' => t('For the above option, consider each menu tree separately.'),
      'children' => t('Close menus when their parent menu is closed.'),
      'clone' => t('Add an extra page link to each expanding menu item.'),
      'doubleclick' => t('Go to the page on double-click.'),
    ),
    '#default_value' => variable_get('dhtml_menu_effects', unserialize(DHTML_MENU_DEFAULT)),
  );

  $form['dhtml_menu_disabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Menus without DHTML'),
    '#options' => dhtml_menu_menus(),
    '#default_value' => variable_get('dhtml_menu_disabled', array()),
    '#description' => t('DHTML will be used for all menus by default, but can be switched off for specific menus.'),
  );
  
  return system_settings_form($form);
}

/**
 * Build human-readable menu names for book menus.
 * This fetches the title of the book outline's root node. Titles are cached for performance.
 */
function dhtml_menu_menus() {
  $titles = cache_get('dhtml_book_titles');
  $titles = isset($titles->data) ? $titles->data : array();
  
  $menu_internal = menu_get_names();
  
  foreach ($menu_internal as $key) {
    if (preg_match('/book-toc-([0-9]+)/', $key, $match)) {
      if (!isset($titles[$match[1]])) {
        $node = node_load($match[1]);
        $titles[$match[1]] = $node->title;
      }
      $menus[$key] = t('Book: %title', array('%title' => $titles[$match[1]]));
    }
    else $menus[$key] = $key;
  }
  
  cache_set('dhtml_book_titles', $titles);
  
  return $menus;
}
