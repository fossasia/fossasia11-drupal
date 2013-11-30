<?php
// $Id: menu_block.admin.inc,v 1.40.2.4 2010/03/24 19:44:00 johnalbin Exp $

/**
 * @file
 * Provides infrequently used functions for menu_block.
 */

/**
 * Menu callback: display the menu block addition form.
 */
function menu_block_add_block_form(&$form_state) {
  module_load_include('inc', 'block', 'block.admin');
  return block_admin_configure($form_state, 'menu_block', NULL);
}

/**
 * Save the new menu block.
 */
function menu_block_add_block_form_submit($form, &$form_state) {
  // Determine the delta of the new block.
  $block_ids = variable_get('menu_block_ids', array());
  $delta = empty($block_ids) ? 1 : max($block_ids) + 1;

  // Save the new array of blocks IDs.
  $block_ids[] = $delta;
  variable_set('menu_block_ids', $block_ids);

  // Save the block configuration.
  _menu_block_block_save($delta, $form_state['values']);

  // Run the normal new block submission (borrowed from block_add_block_form_submit).
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, %d, %d)", $form_state['values']['visibility'], trim($form_state['values']['pages']), $form_state['values']['custom'], $form_state['values']['title'], $form_state['values']['module'], $theme->name, 0, 0, $delta, BLOCK_NO_CACHE);
    }
  }

  foreach (array_filter($form_state['values']['roles']) as $rid) {
    db_query("INSERT INTO {blocks_roles} (rid, module, delta) VALUES (%d, '%s', '%s')", $rid, $form_state['values']['module'], $delta);
  }

  drupal_set_message(t('The block has been created.'));
  cache_clear_all();

  $form_state['redirect'] = 'admin/build/block';
  return;
}

/**
 * Alters the block admin form to add delete links next to menu blocks.
 */
function _menu_block_form_block_admin_display_form_alter(&$form, $form_state) {
  foreach (variable_get('menu_block_ids', array()) AS $delta) {
    $form['menu_block_' . $delta]['delete'] = array('#value' => l(t('delete'), 'admin/build/block/delete-menu-block/'. $delta));
  }
}

/**
 * Menu callback: confirm deletion of menu blocks.
 */
function menu_block_delete(&$form_state, $delta = 0) {
  $title = _menu_block_format_title(menu_block_get_config($delta));
  $form['block_title'] = array('#type' => 'hidden', '#value' => $title);
  $form['delta'] = array('#type' => 'hidden', '#value' => $delta);

  return confirm_form($form, t('Are you sure you want to delete the "%name" block?', array('%name' => $title)), 'admin/build/block', NULL, t('Delete'), t('Cancel'));
}

/**
 * Deletion of menu blocks.
 */
function menu_block_delete_submit($form, &$form_state) {
  // Remove the menu block configuration variables.
  $delta = $form_state['values']['delta'];
  $block_ids = variable_get('menu_block_ids', array());
  unset($block_ids[array_search($delta, $block_ids)]);
  sort($block_ids);
  variable_set('menu_block_ids', $block_ids);
  variable_del("menu_block_{$delta}_title_link");
  variable_del("menu_block_{$delta}_admin_title");
  variable_del("menu_block_{$delta}_parent");
  variable_del("menu_block_{$delta}_level");
  variable_del("menu_block_{$delta}_follow");
  variable_del("menu_block_{$delta}_depth");
  variable_del("menu_block_{$delta}_expanded");
  variable_del("menu_block_{$delta}_sort");

  db_query("DELETE FROM {blocks} WHERE module = 'menu_block' AND delta = %d", $delta);
  db_query("DELETE FROM {blocks_roles} WHERE module = 'menu_block' AND delta = %d", $delta);
  drupal_set_message(t('The "%name" block has been removed.', array('%name' => $form_state['values']['block_title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/build/block';
  return;
}

/**
 * Returns the 'list' $op info for hook_block().
 */
function _menu_block_block_list() {
  $blocks = array();
  foreach (variable_get('menu_block_ids', array()) AS $delta) {
    $blocks[$delta]['info'] = _menu_block_format_title(menu_block_get_config($delta));
    // Menu blocks can't be cached because each menu item can have
    // a custom access callback. menu.inc manages its own caching.
    $blocks[$delta]['cache'] = BLOCK_NO_CACHE;
  }
  return $blocks;
}

/**
 * Return the title of the block.
 *
 * @param $config
 *   array The configuration of the menu block.
 * @return
 *   string The title of the block.
 */
function _menu_block_format_title($config) {
  // If an administrative title is specified, use it.
  if (!empty($config['admin_title'])) {
    return check_plain($config['admin_title']);
  }
  $menus = menu_block_get_all_menus();
  $menus[MENU_TREE__CURRENT_PAGE_MENU] = t('Current menu');
  if (empty($config['menu_name']) || empty($menus[$config['menu_name']])) {
    $title = t('Unconfigured menu block');
  }
  else {
    // Show the configured levels in the block info
    $replacements = array(
      '@menu_name' => $menus[$config['menu_name']],
      '@level1' => $config['level'],
      '@level2' => $config['level'] + $config['depth'] - 1,
    );
    if ($config['parent_mlid']) {
      $parent_item = menu_link_load($config['parent_mlid']);
      $replacements['@menu_name'] = $parent_item['title'];
    }
    if ($config['follow']) {
      $title = t('@menu_name (active menu item)', $replacements);
    }
    elseif ($config['depth'] == 1) {
      $title = t('@menu_name (level @level1)', $replacements);
    }
    elseif ($config['depth']) {
      if ($config['expanded']) {
        $title = t('@menu_name (expanded levels @level1-@level2)', $replacements);
      }
      else {
        $title = t('@menu_name (levels @level1-@level2)', $replacements);
      }
    }
    else {
      if ($config['expanded']) {
        $title = t('@menu_name (expanded levels @level1+)', $replacements);
      }
      else {
        $title = t('@menu_name (levels @level1+)', $replacements);
      }
    }
  }
  return $title;
}

/**
 * Returns the 'configure' $op info for hook_block().
 */
function _menu_block_block_configure($delta) {
  // Create a pseudo form state.
  $form_state = array('values' => menu_block_get_config($delta));
  return menu_block_configure_form($form_state);
}

/**
 * Returns the configuration form for a menu tree.
 *
 * @param $form_state
 *   array An associated array of configuration options should be present in the
 *   'values' key. If none are given, default configuration is assumed.
 * @return
 *   array The form in Form API format.
 */
function menu_block_configure_form(&$form_state) {
  $config = array();
  // Get the config from the form state.
  if (!empty($form_state['values'])) {
    $config = $form_state['values'];
    if (!empty($config['parent'])) {
      list($config['menu_name'], $config['parent_mlid']) = explode(':', $config['parent']);
    }
  }
  // Merge in the default configuration.
  $config += menu_block_get_config();

  // Get the list of menus.
  $menus = menu_block_get_all_menus();

  // Build the standard and jquery versions of the parent item options.
  $parent_options = $parent_options_js = array();
  foreach ($menus AS $menu_name => $title) {
    if ($menu_name == MENU_TREE__CURRENT_PAGE_MENU) {
      $options = array($menu_name . ':0' => $title);
      $parent_options += $options;
      // For the js options, replace the root item option with better help text.
      $options[$menu_name . ':0'] = '<' . t('root of menu') . '>';
    }
    else {
      // Retrieve the entire tree of each menu.
      $options = menu_parent_options(array($menu_name => $title), array('mlid' => 0));
      $parent_options += $options;
      $options[$menu_name . ':0'] = '<' . t('root of @menu_name', array('@menu_name' => $title)) . '>';
    }
    // Render the options as <option> elements.
    $parent_options_js[$menu_name] = form_select_options(array('#value' => NULL), $options);
  }
  // Build a select element that is only needed for the jquery version.
  $menus_select = theme('select', array(
    '#title' => t('Menu'),
    '#value' => $config['menu_name'],
    '#options' => $menus,
    '#name' => 'parent_menu',
    '#id' => 'edit-parent-menu',
    '#attributes' => array('class' => 'menu-block-parent-menu'),
    '#size' => 1,
    '#required' => FALSE,
    '#multiple' => FALSE,
    '#parents' => array('parent_menu'),
  ));
  // Load the javascript data.
  $settings = array(
    'menu_block' => array(
      'menus' => $menus_select . '<label id="item-label">' . t('Item') . ':</label>',
      'menus_default' => $config['menu_name'],
      'parent_options' => $parent_options_js,
      'parent_default' => $config['menu_name'] . ':' . $config['parent_mlid'],
    ),
  );
  drupal_add_js($settings, 'setting');
  drupal_add_js(drupal_get_path('module', 'menu_block') . '/menu-block.js');

  // Build the standard form.
  $form['menu-block-wrapper-start'] = array('#value' => '<div id="menu-block-settings" class="menu-block-configure-form">');
  drupal_add_css(drupal_get_path('module', 'menu_block') . '/menu-block-admin.css');
  $form['title_link'] = array(
    '#prefix' => '<div class="menu-block-title-link">',
    '#type' => 'checkbox',
    '#title' => t('Block title as link'),
    '#default_value' => $config['title_link'],
    '#description' => t('Make the default block title a link to that menu item. An overridden block title will not be a link.'),
    '#suffix' => '</div>',
  );
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => $config['admin_title'],
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this block. If blank, the regular title will be used.'),
  );
  $form['parent'] = array(
    '#type' => 'select',
    '#title' => t('Parent item'),
    '#default_value' => $config['menu_name'] . ':' . $config['parent_mlid'],
    '#options' => $parent_options,
    '#description' => t('The tree of links will only contain children of the selected parent item. Using <em>&lt;the menu selected by the page&gt;</em> can be customized on the <a href="!url">Menu block settings page</a>.', array('!url' => url('admin/settings/menu_block'))),
    '#attributes' => array('class' => 'menu-block-parent'),
    '#element_validate' => array('menu_block_configure_form_parent_validate'),
  );
  $form['level'] = array(
    '#type' => 'select',
    '#title' => t('Starting level'),
    '#default_value' => $config['level'],
    '#options' => array(
      '1'  => t('1st level (primary)'),
      '2'  => t('2nd level (secondary)'),
      '3'  => t('3rd level (tertiary)'),
      '4'  => t('4th level'),
      '5'  => t('5th level'),
      '6'  => t('6th level'),
      '7'  => t('7th level'),
      '8'  => t('8th level'),
      '9'  => t('9th level'),
    ),
    '#description' => t('Blocks that start with the 1st level will always be visible. Blocks that start with the 2nd level or deeper will only be visible when the trail to the active menu item is in the block’s tree.'),
  );
  // The value of "follow" in the database/config array is either FALSE or the
  // value of the "follow_parent" form element.
  if ($follow = $config['follow']) {
    $follow_parent = $follow;
    $follow = 1;
  }
  else {
    $follow_parent = 'active';
  }
  $form['follow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make the starting level follow the active menu item.'),
    '#default_value' => $follow,
    '#description' => t('If the active menu item is deeper than the level specified above, the starting level will follow the active menu item. Otherwise, the starting level of the tree will remain fixed.'),
    '#element_validate' => array('menu_block_configure_form_follow_validate'),
    '#attributes' => array('class' => 'menu-block-follow'),
  );
  $form['follow_parent'] = array(
    '#prefix' => '<div class="menu-block-follow-parent">',
    '#type' => 'select',
    '#title' => t('Starting level will be'),
    '#default_value' => $follow_parent,
    '#options' => array(
      'active' => t('Active menu item'),
      'child' => t('Children of active menu item'),
    ),
    '#description' => t('When following the active menu item, specify if the starting level should be the active menu item or its children.'),
    '#suffix' => '</div>',
  );
  $form['depth'] = array(
    '#type' => 'select',
    '#title' => t('Maximum depth'),
    '#default_value' => $config['depth'],
    '#options' => array(
      '1'  => '1',
      '2'  => '2',
      '3'  => '3',
      '4'  => '4',
      '5'  => '5',
      '6'  => '6',
      '7'  => '7',
      '8'  => '8',
      '9'  => '9',
      '0'  => t('Unlimited'),
    ),
    '#description' => t('From the starting level, specify the maximum depth of the menu tree.'),
  );
  $form['expanded'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Expand all children</strong> of this tree.'),
    '#default_value' => $config['expanded'],
  );
  $form['sort'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Sort</strong> menu tree by the active menu item’s trail.'),
    '#default_value' => $config['sort'],
    '#description' => t('Sort each item in the active trail to the top of its level. When used on a deep or wide menu tree, the active menu item’s children will be easier to see when the page is reloaded.'),
  );
  $form['menu-block-wrapper-close'] = array('#value' => '</div>');

  return $form;
}

/**
 * Validates the parent element of the block configuration form.
 */
function menu_block_configure_form_parent_validate($element, &$form_state) {
  // The "Menu block" config has the "parent" value split into "menu_name" and
  // "parent_mlid" values. The value of "parent" stored in the database/config array is either FALSE
  list($form_state['values']['menu_name'], $form_state['values']['parent_mlid']) = explode(':', $form_state['values']['parent']);
}

/**
 * Validates the follow element of the block configuration form.
 */
function menu_block_configure_form_follow_validate($element, &$form_state) {
  // The value of "follow" stored in the database/config array is either FALSE
  // or the value of the "follow_parent" form element.
  if ($form_state['values']['follow'] && !empty($form_state['values']['follow_parent'])) {
    $form_state['values']['follow'] = $form_state['values']['follow_parent'];
  }
}

/**
 * Returns the 'save' $op info for hook_block().
 */
function _menu_block_block_save($delta, $edit) {
  variable_set("menu_block_{$delta}_title_link", $edit['title_link']);
  variable_set("menu_block_{$delta}_admin_title", $edit['admin_title']);
  variable_set("menu_block_{$delta}_parent", $edit['parent']);
  variable_set("menu_block_{$delta}_level", $edit['level']);
  variable_set("menu_block_{$delta}_follow", $edit['follow']);
  variable_set("menu_block_{$delta}_depth", $edit['depth']);
  variable_set("menu_block_{$delta}_expanded", $edit['expanded']);
  variable_set("menu_block_{$delta}_sort", $edit['sort']);
}

/**
 * Menu block admin settings form.
 *
 * @return
 *   The settings form used by Menu block.
 */
function menu_block_admin_settings_form($form_state) {
  // Grab all the menus.
  $menus = menu_block_get_all_menus();
  unset($menus[MENU_TREE__CURRENT_PAGE_MENU]);
  $total_menus = count($menus);

  // Load stored configuration.
  $menu_order = variable_get('menu_block_menu_order', array('primary-links' => '', 'secondary-links' => ''));

  // Merge the saved configuration with any un-configured menus.
  $all_menus = $menu_order + $menus;

  $form['heading'] = array(
    '#value' => '<p>' . t('If a block is configured to use <em>"the menu selected by the page"</em>, the block will be generated from the first "available" menu that contains a link to the page.') . '</p>',
  );

  // Orderable list of menu selections.
  $form['menu_order'] = array(
    '#tree' => TRUE,
    '#theme' => 'menu_block_menu_order',
  );

  $order = 0;
  foreach (array_keys($all_menus) as $menu_name) {
    $form['menu_order'][$menu_name] = array(
      'title' => array(
        '#value' => check_plain($menus[$menu_name]),
      ),
      'available' => array(
        '#type' => 'checkbox',
        '#attributes' => array('title' => t('Select from the @menu_name menu', array('@menu_name' => $menus[$menu_name]))),
        '#default_value' => isset($menu_order[$menu_name]),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#default_value' => $order - $total_menus,
        '#delta' => $total_menus,
        '#id' => 'edit-menu-block-menus-' . $menu_name,
      ),
    );
    $order++;
  }

  $form['footer_note'] = array(
    '#value' => '<p>' . t('The above list will <em>not</em> affect menu blocks that are configured to use a specific menu.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission handler.
 */
function menu_block_admin_settings_form_submit($form, &$form_state) {
  $menu_order = array();
  foreach ($form_state['values']['menu_order'] AS $menu_name => $row) {
    if ($row['available']) {
      // Add available menu and its weight to list.
      $menu_order[$menu_name] = (int) $row['weight'];
    }
  }
  // Sort the keys by the weight stored in the value.
  asort($menu_order);
  foreach ($menu_order AS $menu_name => $weight) {
    // Now that the array is sorted, the weight is redundant data.
    $menu_order[$menu_name] = '';
  }
  variable_set('menu_block_menu_order', $menu_order);
}

/**
 * Theme a drag-to-reorder table of menu selection checkboxes.
 */
function theme_menu_block_menu_order($element) {
  drupal_add_tabledrag('menu-block-menus', 'order', 'sibling', 'menu-weight');

  $header = array(
    t('Menu'),
    t('Available'),
    t('Weight'),
  );

  // Generate table of draggable menu names.
  $rows = array();
  foreach (element_children($element) as $menu_name) {
    $element[$menu_name]['weight']['#attributes']['class'] = 'menu-weight';
    $rows[] = array(
      'data' => array(
        drupal_render($element[$menu_name]['title']),
        drupal_render($element[$menu_name]['available']),
        drupal_render($element[$menu_name]['weight']),
      ),
      'class' => 'draggable',
    );
  }
  return theme('table', $header, $rows, array('id' => 'menu-block-menus'));
}
