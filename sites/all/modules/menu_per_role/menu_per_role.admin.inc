<?php
/**
 * @file
 * Administration form for menu items.
 */

/**
 * Implementation of hook_menu().
 *
 * This function creates the actual menu array and returns it.
 */
function _menu_per_role_menu() {
  $menu = array();

  $menu['admin/settings/menu_per_role'] = array(
    'title' => 'Menu per Role',
    'description' => 'Settings for the menu per role module',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('menu_per_role_settings_form'),
    'access arguments' => array('administer menu_per_role'),
    'file' => 'menu_per_role.admin.inc',
  );

  return $menu;
}

/**
 *
 */
function menu_per_role_settings_form($form_state) {
  $form = array();

  $form['menu_per_role_uid1_see_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('The administrator (UID=1) sees everything'),
    '#description' => t('When selected, the administrator will not be affected by Menu per Role. (i.e. All the menus are visible to him.)'),
    '#default_value' => variable_get('menu_per_role_uid1_see_all', 1),
  );

  $admin_roles = user_roles(FALSE, 'administer menu_per_role');
  $form['menu_per_role_admin_see_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('The menu per role administrators see everything'),
    '#description' => t('When selected, all the menu per role administrators see all the menus, whether they were marked as hidden or not.')
      . ' ' . t('!check assigned the "administer menu_per_role" permission.',
          array('!check' => l('Check the roles',
                            'admin/user/permissions',
                            array('fragment' => 'module-menu_per_role'))))
      . (empty($admin_roles) ? '<br /><span style="color: red;">' . t('IMPORTANT NOTE: No roles were marked with the "administer menu_per_role" permission.') . '</span>' : ''),
    '#default_value' => variable_get('menu_per_role_admin_see_all', 0),
  );

  $form['menu_per_role_hide_show'] = array(
    '#type' => 'radios',
    '#title' => t('Select what is shown when editing menu items'),
    '#options' => array(0 => t('Hide and Show check boxes'), 1 => t('Only Hide check boxes'), 2 => t('Only Show check boxes')),
    '#description' => t('By default, both list of check boxes are shown when editing a menu item (in the menu administration area or while editing a node.) This option let you choose to only show the "Show menu item only to selected roles" or "Hide menu item from selected roles". WARNING: changing this option does not change the existing selection. This means some selection will become invisible when you hide one of the set of check boxes...'),
    '#default_value' => variable_get('menu_per_role_hide_show', 0),
  );

  return system_settings_form($form);
}

/*
 * Implementation of hook_form_alter().
 */
function _menu_per_role_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'menu_edit_item') {
    $f = &$form;
    $form['submit']['#weight'] = 9;
    // this callback does not work in nodes because the mlid is not available, see menu_per_role_nodeapi()
    $form['#submit'][] = '_menu_per_role_form_submit';
  }
  elseif (isset($form['#node']) && $form['#node']->type .'_node_form' == $form_id && isset($form['menu'])) {
    $f = &$form['menu'];
  }
  if (isset($f)) {
    $default_value_roles = $form['menu']['mlid']['#value']
      ? _menu_per_role_get_roles($form['menu']['mlid']['#value'], 0)
      : array();
    $default_value_hide_from_roles = $form['menu']['mlid']['#value']
      ? _menu_per_role_get_roles($form['menu']['mlid']['#value'], 1)
      : array();
    $f['menu_per_role'] = array(
      '#type' => 'fieldset',
      '#title' => t('Restrict item visibility'),
      '#collapsible' => TRUE,
      '#collapsed' => (count($default_value_roles) + count($default_value_hide_from_roles)) == 0,
      '#weight' => 5,
      '#description' => t('Check to know whether the user has proper visibility permissions to see this menu item. Note that both checks are always performed.'),
    );
    $hide_show = variable_get('menu_per_role_hide_show', 0);
    if ($hide_show == 0 || $hide_show == 2) {
      $f['menu_per_role']['menu_per_role_roles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Show menu item only to selected roles'),
        '#options' => user_roles(),
        '#default_value' => $default_value_roles,
        '#description' => t('Check no role to leave the access permission to the default. A user who is not part of at least one of the selected roles will not see this menu item.'),
      );
    }
    if ($hide_show == 0 || $hide_show == 1) {
      $f['menu_per_role']['menu_per_role_hide_from_roles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Hide menu item from selected roles'),
        '#options' => user_roles(),
        '#default_value' => $default_value_hide_from_roles,
        '#description' => t('Check no role to leave the access permission to the default. A user who is part of any one of these roles will not see this menu item.'),
      );
    }
  }
}


/*
 * Internal function to save the data in our table.
 *
 * This function is also called from the menu_per_role_nodeapi() function.
 */
function _menu_per_role_form_submit($form, &$form_state) {
  global $db_type;

  if ($form_state['submitted'] && user_access('administer menu_per_role')) {
    $mlid = $form_state['values']['menu']['mlid'];
    if ($mlid) {
      // hide but to those roles
      $rids = array();
      $hide_show = variable_get('menu_per_role_hide_show', 0);
      if ($hide_show == 0 || $hide_show == 2) {
        $roles = isset($form_state['values']['menu_per_role_roles']) ?
                       $form_state['values']['menu_per_role_roles']
                       : $form_state['values']['menu']['menu_per_role']['menu_per_role_roles'];
        foreach ($roles as $rid => $checked) {
          if ($checked) {
            $rids[] = $rid;
          }
        }
      }
      $rids_str = implode(',', $rids);
      // show but to those roles
      $hrids = array();
      if ($hide_show == 0 || $hide_show == 1) {
        $roles = isset($form_state['values']['menu_per_role_hide_from_roles']) ?
                       $form_state['values']['menu_per_role_hide_from_roles']
                       : $form_state['values']['menu']['menu_per_role']['menu_per_role_hide_from_roles'];
        foreach ($roles as $rid => $checked) {
          if ($checked) {
            $hrids[] = $rid;
          }
        }
      }
      $hrids_str = implode(',', $hrids);
      // save in our table
      //db_lock_table('menu_per_role');
      if ($rids_str || $hrids_str) {
        db_query("UPDATE {menu_per_role} SET rids = '%s', hrids = '%s' WHERE mlid = %d", $rids_str, $hrids_str, $mlid);
        if (db_affected_rows() == 0) {
          // if nothing was affected, the row did not exist yet
          // (although with MySQL this may fail because db_affected_rows() may only return
          // rows that have been changed instead of the # of rows that match the WHERE clause.)
          if ($db_type != 'pgsql') {
            // MySQL hack
            $insert = !db_result(db_query("SELECT 1 FROM {menu_per_role} WHERE mlid = %d", $mlid));
          }
          else {
            $insert = TRUE;
          }
          if ($insert) {
            db_query("INSERT INTO {menu_per_role} (mlid, rids, hrids) VALUES (%d, '%s', '%s')", $mlid, $rids_str, $hrids_str);
          }
        }
      }
      else {
        // we don't need to save it when empty, instead, remove that completely
        db_query("DELETE FROM {menu_per_role} WHERE mlid = %d", $mlid);
      }
      //db_unlock_tables();
      // reset the menus
      menu_cache_clear_all();
    }
    elseif (isset($form_state['values']['menu_per_role_roles'])) {
      drupal_set_message(t('The menu link identifier was not defined on Submit in <b>Menu per Role</b>. You are most certainly adding a new menu item. For this feature to work when adding a menu item, you must apply the patch defined in <a href="http://drupal.org/node/326210" target="_blank">node #326210</a>. That patch is included in this module for that purpose.'), 'error');
    }
  }
}

// vim: ts=2 sw=2 et syntax=php
