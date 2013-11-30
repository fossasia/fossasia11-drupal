<?php
// $Id: menu_block.pages.inc,v 1.21 2010/03/22 19:19:36 johnalbin Exp $

/**
 * @file
 * Provides infrequently used pages for menu_block.
 */

/**
 * Implements hook_help().
 */
function _menu_block_help($path, $arg) {
  $output = '';
  switch ($path) {
    case 'admin/build/block':
      $output = '<p>' . t('Use the <a href="!url">add menu block page</a> to create a customizable menu block. You will then be able to configure your menu block before adding it.', array('!url' => url('admin/build/block/add-menu-block'))) . '</p>';
      if (module_exists('help')) {
        $output .= theme('more_help_link', url('admin/help/menu_block'));
      }
      break;

    case 'admin/build/block/configure':
    case 'admin/build/block/add-menu-block':
      if (module_exists('help')) {
        $output = '<p>' . t('To learn more about configuring menu blocks, see <a href="!url">menu block’s detailed help</a>.', array('!url' => url('admin/help/menu_block'))) . '</p>';
      }
      break;

    case 'admin/help#menu_block':
      $output =
        '<h3>' . t('Adding menu blocks') . '</h3>'
      . '<p>' . t('To add new menu blocks, use the "<a href="!add-url">Add menu block</a>" tab (or button) on the <a href="!block-url">administer blocks page</a>. You will then be able to configure your menu block before adding it.', array('!add-url' => url('admin/build/block/add-menu-block'), '!block-url' => url('admin/build/block'))) . '</p>'
      . '<h3>' . t('Configuring menu blocks') . '</h3>'
      . '<p>' . t('When adding or configuring a menu block, several configuration options are available:') . '</p>'
      . '<dl>'
      . '<dt><strong>' . t('Block title') . '</strong></dt>'
      . '<dd>' . t('For menu trees that start with the 1st level, the default block title will be the menu name. For menu trees that start with the 2nd level or deeper, the default block title will be the title for the parent menu item of the specified level.')
      . '<p>' . t('For example, if the active menu trail for the Navigation menu is: Administer › Site building › Menus › Primary links, then a menu block configured to start with the 1st level of the Navigation will display a block title of “Navigation”. And a menu block configured to start with the 3rd level of the Navigation menu will display a block title of “Site building”.') . '</p></dd>'
      . '<dt><strong>' . t('Block title as link') . '</strong></dt>'
      . '<dd>' . t('For menu trees that start with the 2nd level or deeper, the default block title will be the title for the parent menu item of the specified level. If this option is checked, the block title will be a link to that menu item.') . '</dd>'
      . '<dt><strong>' . t('Administrative title') . '</strong></dt>'
      . '<dd>' . t('To help identify the block on the administer blocks page, you can specify a unique title to be used on that page. If blank, the regular title will be used.') . '</dd>'
      . '<dt><strong>' . t('Parent item') . '</strong></dt>'
      . '<dd>' . t('First select the menu. Then select the parent item from that menu. The tree of links will only contain children of the selected parent item.') . '</dd>'
      . '<dt><strong>' . t('Starting level') . '</strong></dt>'
      . '<dd>' . t('Blocks that start with the 1st level will always be visible. Blocks that start with the 2nd level or deeper will only be visible when the trail to the active menu item is in the block’s tree.') . '</dd>'
      . '<dt><strong>' . t('Make the starting level follow the active menu item') . '</strong></dt>'
      . '<dd>' . t('If the active menu item is deeper than the level specified above, the starting level will follow the active menu item. Otherwise, the starting level of the tree will remain fixed.') . '</dd>'
      . '<dt><strong>' . t('Maximum depth') . '</strong></dt>'
      . '<dd>' . t('From the starting level, specify the maximum depth of the tree. Blocks with a maximum depth of 1 will just be a single un-nested list of links with none of those links’ children displayed.') . '</dd>'
      . '<dt><strong>' . t('Expand all children') . '</strong></dt>'
      . '<dd>' . t('All children of this menu will be expanded.') . '</dd>'
      . '<dt><strong>' . t('Sort') . '</strong></dt>'
      . '<dd>' . t('Sort each item in the active trail to the top of its level. When used on a deep or wide menu tree, the active menu item’s children will be easier to see when the page is reloaded.') . '</dd>'
      . '</dl>'
      . '<h3>' . t('Styling menu blocks') . '</h3>'
      . '<p>' . t('Themers should be aware of the myriad of classes, templates and theme functions available to them. See the <a href="!url">online documentation</a> or the README.txt file for detailed information about each of them.', array('!url' => url('http://drupal.org/node/748022'))) . '</p>'
      . '<h3>' . t('Menu block API') . '</h3>'
      . '<p>' . t('Developers can use the API of this module to create their own menu trees outside the confines of blocks. All of the publicly available API functions are documented in the menu_block.module file.') . '</p>'
      . '<p>' . t('In addition, Menu block implements HOOK_get_menus() and HOOK_menu_block_tree_alter(). See menu_block.api.php for documentation.') . '</p>';
      break;
  }
  return $output;
}
