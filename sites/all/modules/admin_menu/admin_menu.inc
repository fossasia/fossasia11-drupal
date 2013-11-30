<?php

/**
 * The key function that builds the menu links whenever there is a menu rebuild.
 */
function _admin_menu_rebuild_links() {
  $menu = NULL;
  // Since it's possible this core function might change, check
  // that it exists. We do this instead of calling menu_router_build()
  // since that may trigger another menu rebuild that is not protected
  // by the lock API.
  if (function_exists('_menu_router_cache')) {
    // Get the newly rebuilt menu.
    $menu = _menu_router_cache();
  }
  else {
    $menu = menu_router_build();
  }
  if (!$menu) {
    // Something went wrong. Don't risk triggering another menu rebuild.
    return;
  }
  // Add normal and suggested items as links.
  $menu_links = array();
  foreach ($menu as $path => $item) {
    // Exclude menu callbacks, include items below admin/* and node/add/*.
    if ($item['type'] != MENU_CALLBACK && (($item['_parts'][0] == 'admin' && count($item['_parts']) > 1) || (strpos($path, 'node/add') === 0))) {
      // TODO: handle local tasks with wildcards
      if (!strpos($path, '%')) {
        $item = admin_menu_link_build($item);
        $menu_links[$path] = $item;
        $sort[$path] = $item['_number_parts'];
      }
    }
  }
  $deleted = admin_menu_adjust_items($menu_links, $sort);
  if ($menu_links) {
    // Make sure no child comes before its parent.
    array_multisort($sort, SORT_NUMERIC, $menu_links);

    foreach ($menu_links as $item) {
      admin_menu_link_save($item);
    }
  }
  // Allow modules to add more links. If you want to alter links saved by
  // admin_menu, use hook_menu_link_alter() and look for
  // $item['module'] == 'admin_menu'
  $links = array();
  foreach (module_implements('admin_menu') as $module) {
    $function = $module . '_admin_menu';
    $links = array_merge_recursive($links, $function($deleted));
  }
  foreach ($links as $item) {
    admin_menu_link_save($item);
  }
}

/**
 * Prepare a menu link from basic information formatted for a router item.
 */
function admin_menu_link_build($item) {
  $item['module'] = 'admin_menu';
  $item['menu_name'] = 'admin_menu';
  $item += array(
    'link_title' => $item['title'],
    'link_path' => $item['path'],
    'hidden' => 0,
    'options' => array(),
  );
  $item['options']['alter'] = TRUE;
  // DAM does not output item descriptions to prevent mouseover clashes and
  // increase page loading performance.  However, the following code shows how
  // link attributes can be added (for ajaxified DAM functionality later).
  /*
  if (!empty($item['description'])) {
    $item['options']['attributes']['title'] = $item['description'];
  }
  */
  if (!empty($item['query'])) {
    $item['options']['query'] = $item['query'];
  }
  return $item;
}

/**
 * Convenience function that looks up the plid if $item['parent_path'] is set.
 */
function admin_menu_link_save($item) {
  $item = admin_menu_link_build($item);

  // Check whether we are able to update an existing item.
  $existing_item = db_fetch_array(db_query("SELECT mlid, plid, has_children FROM {menu_links} WHERE link_path = '%s' AND menu_name = '%s'", $item['link_path'], 'admin_menu'));
  if ($existing_item) {
    $item['mlid'] = $existing_item['mlid'];
    $item['plid'] = $existing_item['plid'];
    $item['has_children'] = $existing_item['has_children'];
  }

  // Look up the parent path for both new and existing links, since the parent
  // may change.
  if (isset($item['parent_path'])) {
    if ($item['parent_path'] == '<root>') {
      // <root> means that we want the link at the top level.
      $item['plid'] = 0;
    }
    else {
      $plid = db_result(db_query("SELECT mlid from {menu_links} WHERE link_path = '%s' AND menu_name = '%s'", $item['parent_path'], 'admin_menu'));
      if ($plid) {
        $item['plid'] = $plid;
      }
    }
  }

  menu_link_save($item);
}

/**
 * Implementation of hook_admin_menu().
 *
 * @param &$deleted
 *   Array of links under admin/* that were removed by admin_menu_adjust_items().
 *   If one of these links is added back, it should be removed from the array.
 */
function admin_menu_admin_menu(&$deleted) {
  $links = array();
  $icon_path = '<front>';

  // Add link to manually run cron.
  $links[] = array(
    'title' => 'Run cron',
    'path' => 'admin/reports/status/run-cron',
    'weight' => 50,
    'parent_path' => $icon_path,
  );
  // Add link to run update.php.
  $links[] = array(
    'title' => 'Run updates',
    'path' => 'update.php',
    'weight' => 50,
    'parent_path' => $icon_path,
    'options' => array('external' => TRUE),
  );

  // Move 'By module' item into Site configuration.
  if (isset($deleted['admin/by-module'])) {
    $deleted['admin/by-module']['parent_path'] = 'admin/settings';
    $deleted['admin/by-module']['weight'] = -10;
    $links[] = $deleted['admin/by-module'];
    unset($deleted['admin/by-module']);
  }

  // Add link to drupal.org.
  $links[] = array(
    'title' => 'Drupal.org',
    'path' => 'http://drupal.org',
    'weight' => 100,
    'parent_path' => $icon_path,
  );
  // Add links to project issue queues.
  $links[] = array(
    'title' => 'Drupal issue queue',
    'path' => 'http://drupal.org/project/issues/drupal',
    'weight' => -10,
    'parent_path' => 'http://drupal.org',
  );
  $projects = array();
  foreach (module_list(FALSE, FALSE, TRUE) as $module) {
    $info = drupal_parse_info_file(drupal_get_path('module', $module) .'/'. $module .'.info');
    if (!isset($info['project']) || (isset($info['project']) && ($info['project'] == 'drupal' || isset($projects[$info['project']])))) {
      continue;
    }
    $projects[$info['project']] = 1;
    $url = 'http://drupal.org/project/issues/'. $info['project'];
    // Filtering project versions via query string is not yet supported.
    // @see http://drupal.org/node/97569
    // $url .= !empty($info['version']) ? '/'. $info['version'] : '';
    $links[] = array(
      'title' => check_plain($info['name']) . ' issue queue',
      'path' => $url,
      'parent_path' => 'http://drupal.org',
    );
  }

  // Add 'Create <content-type>' items to Content management menu.
  if (isset($deleted['node/add'])) {
    $deleted['node/add']['parent_path'] = 'admin/content';
    $deleted['node/add']['weight'] = 0;
    $links[] = $deleted['node/add'];
    unset($deleted['node/add']);
  }
  foreach($deleted as $path => $item) {
    if (strpos($path, 'node/add') !== FALSE) {
      $links[] = $deleted[$path];
      unset($deleted[$path]);
    }
  }
  // Make sure longer paths are after shorter ones
  ksort($deleted);
  foreach (node_get_types('types', NULL, TRUE) as $type) {
    $type_url_str = str_replace('_', '-', $type->type);
    $type_path = 'admin/content/node-type/' . $type_url_str;
    $links[$type_path] = array(
      'title' => 'Edit !content-type',
      'path' => $type_path,
      'parent_path' => 'admin/content/types',
      'options' => array('t' => array('!content-type' => $type->name)),
    );
    unset($deleted[$type_path . '/edit']);
    // CCK and other modules adding to node types handled here.
    foreach($deleted as $path => $item) {
      // Precise test needed to account for multiple content-types having the
      // same prefix in their name.
      if ($path === $type_path || strpos($path, $type_path . '/') === 0) {
        // Logically, parent path can never go shorter than $type_path.
        $i = $item['_number_parts'] - 1;
        do {
          $parent_path = implode('/', array_slice($item['_parts'], 0, $i));
          --$i;
        } while (!isset($links[$parent_path]) && $i);
        $item['parent_path'] = $parent_path;
        $links[$path] = $item;
        unset($deleted[$path]);
      }
    }
  }

  // Add clear-cache.
  $links[] = array(
    'title' => 'Flush all caches',
    'path' => 'admin_menu/flush-cache',
    'query' => 'destination',
    'weight' => 20,
    'parent_path' => $icon_path,
  );
  $caches = array(
    'admin_menu' => 'Administration menu',
    'cache' => 'Cache tables',
    'menu' => 'Menu',
    'requisites' => 'Page requisites',
    'theme' => 'Theme registry',
  );
  foreach ($caches as $name => $title) {
    $links[] = array(
      'title' => $title,
      'path' => 'admin_menu/flush-cache/' . $name,
      'query' => 'destination',
      'parent_path' => 'admin_menu/flush-cache',
    );
  }

  // Add devel module links
  if (module_exists('devel')) {
    // Add variable editor.
    $links[] = array(
      'title' => 'Variable editor',
      'path' => 'devel/variable',
      'weight' => 20,
      'parent_path' => $icon_path,
    );
    // Add switch_user items.
    if ($devel_user_links = module_invoke('devel', 'switch_user_list')) {
      foreach ($devel_user_links as $link) {
        if (is_array($link)) {
          $links[] = array(
            'title' => $link['title'],
            'description' => $link['attributes']['title'],
            'path' => $link['href'],
            'options' => array(
              'query' => $link['query'],
              'html' => !empty($link['html']),
            ),
            'parent_path' => 'logout',
          );
        }
        // @todo Remove when Devel 6.x-1.16 has been released.
        elseif (preg_match('!href="' . base_path() . '([^\?]+)\?([^"]+)" title="([^"]+)">((<em>)?[^<]+(</em>)?)!', $link, $match)) {
          $links[] = array(
            'title' => $match[4],
            'description' => $match[3],
            'path' => urldecode($match[1]),
            'weight' => 20,
            'query' => 'destination',
            'parent_path' => 'logout',
            'options' => array('html' => TRUE),
          );
        }
      }
    }
  }
  // Add developer modules toggle link.
  $saved_state = variable_get('admin_menu_devel_modules_enabled', NULL);
  $links[] = array(
    'title' => isset($saved_state) ? t('Enable developer modules') : t('Disable developer modules'),
    'path' => 'admin_menu/toggle-modules',
    'weight' => 88,
    'parent_path' => $icon_path,
    'options' => array('query' => 'destination'),
  );

  return $links;
}

/**
 * Add some hard-coded features for better user experience.
 *
 * @param array $menu_links
 *   An array containing the complete administration menu structure, passed by
 *   reference.
 * @param array $sort
 *   An array containing the # parts of each link - must be updated if a link
 *   is added.
 * @return
 *   An array of links that were removed from $menu_links.
 */
function admin_menu_adjust_items(&$menu_links, &$sort) {
  global $user;
  $links = array();
  $deleted = array();

  // Change or remove items, or add new top-level items.
  $deleted['admin/by-module'] = $menu_links['admin/by-module'];
  unset($menu_links['admin/by-module'], $sort['admin/by-module']);
  $deleted['admin/by-task'] = $menu_links['admin/by-task'];
  unset($menu_links['admin/by-task'], $sort['admin/by-task']);

  // Remove certain links to re-position them in admin_menu_admin_menu().
  foreach ($menu_links as $path => $link) {
    // Remove links below
    // - admin/content/node-type/*
    // - node/add*
    if (strpos($path, 'admin/content/node-type/') !== FALSE || strpos($path, 'node/add') !== FALSE) {
      $deleted[$path] = $link;
      unset($menu_links[$path], $sort[$path]);
    }
  }
  // Add the icon containing special links.
  $links[] = array(
    'title' => theme('admin_menu_icon'),
    'path' => '<front>',
    'weight' => -100,
    'parent_path' => '<root>',
    'options' => array('extra class' => 'admin-menu-icon', 'html' => TRUE),
  );
  // Add link to show current authenticated/anonymous users - we will add the
  // data dynamically in the _alter hook.
  $links[] = array(
    'title' => 'icon_users',
    'path' => 'user',
    'weight' => -90,
    'parent_path' => '<root>',
    'options' => array('extra class' => 'admin-menu-action admin-menu-icon admin-menu-users', 'html' => TRUE),
  );
  $links[] = array(
    'title' => 'Log out @username',
    'path' => 'logout',
    'weight' => -100,
    'parent_path' => '<root>',
    // Note: @username is dynamically replaced by default, we just invoke
    // replacement by setting the 't' key here.
    'options' => array('extra class' => 'admin-menu-action admin-menu-logout', 't' => array()),
  );
  foreach ($links as $item) {
    $path = $item['path'];
    $item = admin_menu_link_build($item);
    $menu_links[$path] = $item;
    $sort[$path] = 1;
  }

  return $deleted;
}

/**
 * Form builder function for module settings.
 */
function admin_menu_theme_settings() {
  $form['admin_menu_margin_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Adjust top margin'),
    '#default_value' => variable_get('admin_menu_margin_top', 1),
    '#description' => t('If enabled, the site output is shifted down approximately 20 pixels from the top of the viewport to display the administration menu. If disabled, some absolute- or fixed-positioned page elements may be covered by the administration menu.'),
  );
  $form['admin_menu_position_fixed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep menu at top of page'),
    '#default_value' => variable_get('admin_menu_position_fixed', 0),
    '#description' => t('If enabled, the administration menu is always displayed at the top of the browser viewport (even after the page is scrolled). <strong>Note: In some browsers, this setting results in a malformed page, an invisible cursor, non-selectable elements in forms, or other issues. Disable this option if these issues occur.</strong>'),
  );
  $form['tweaks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
  );
  $form['tweaks']['admin_menu_tweak_modules'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapse fieldsets on modules page'),
    '#default_value' => variable_get('admin_menu_tweak_modules', 0),
    '#description' => t('If enabled, fieldsets on the <a href="!modules-url">modules</a> page are automatically collapsed when loading the page.', array('!modules-url' => url('admin/build/modules'))),
  );
  if (module_exists('util')) {
    $form['tweaks']['admin_menu_tweak_modules']['#description'] .= '<br /><strong>'. t('If the Utility module was installed for this purpose, it can be safely disabled and uninstalled.') .'</strong>';
  }
  $form['tweaks']['admin_menu_tweak_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Move local tasks into menu'),
    '#default_value' => variable_get('admin_menu_tweak_tabs', 0),
    '#description' => t('If enabled, the tabs on the current page are moved into the administration menu. This feature is only available in themes that use the CSS classes <code>tabs primary</code> and <code>tabs secondary</code> for tabs.'),
  );
  $form = system_settings_form($form);
  $form['wipe description'] = array(
    '#type' => 'item',
    '#value' => t('If the administration menu displays duplicate menu items, you may need to rebuild your menu items using the <em>Wipe and rebuild</em> button.'),
  );
  $form['wipe'] = array(
    '#type' => 'submit',
    '#value' => t('Wipe and rebuild'),
    '#submit' => array('admin_menu_wipe'),
  );

  return $form;
}

/**
 * Wipe the menu so it can be rebuilt from scratch.
 */
function admin_menu_wipe() {
  db_query("DELETE FROM {menu_links} WHERE menu_name = 'admin_menu'");
  menu_cache_clear('admin_menu');
  menu_rebuild();
}

/**
 * Helper function for admin_menu_form_devel_admin_settings_alter().
 *
 * Extends Devel module with Administration Menu developer settings.
 */
function _admin_menu_devel_settings_form_alter(&$form, $form_state) {
  // Shift system_settings_form buttons.
  $weight = isset($form['buttons']['#weight']) ? $form['buttons']['#weight'] : 0;
  $form['buttons']['#weight'] = $weight + 1;

  $form['admin_menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Administration menu settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $display_options = array('mid', 'weight', 'pid');
  $display_options = array(0 => t('None'), 'mlid' => t('Menu link ID'), 'weight' => t('Weight'), 'plid' => t('Parent link ID'));
  $form['admin_menu']['admin_menu_display'] = array(
    '#type' => 'radios',
    '#title' => t('Display additional data for each menu item'),
    '#default_value' => variable_get('admin_menu_display', 0),
    '#options' => $display_options,
    '#description' => t('Display the selected items next to each menu item link.'),
  );
  $form['admin_menu']['admin_menu_show_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display all menu items'),
    '#default_value' => variable_get('admin_menu_show_all', 0),
    '#description' => t('If enabled, all menu items are displayed regardless of your site permissions. <em>Note: Do not enable on a production site.</em>'),
  );
}

/**
 * Menu callback to enable/disable developer modules.
 *
 * This saves up to 150ms on each uncached page request. Not much, but
 * on larger Drupal sites this is actually a 10% performance increase.
 */
function admin_menu_toggle_modules() {
  // URL token prtects this against CSRF attacks.
  if(!isset($_GET['token']) || ($_GET['token'] !== drupal_get_token($_GET['q']))) {
    return MENU_ACCESS_DENIED;
  }
  $rebuild = FALSE;
  $saved_state = variable_get('admin_menu_devel_modules_enabled', NULL);
  if (isset($saved_state)) {
    // Re-enable modules that were enabled before.
    module_enable($saved_state);
    variable_del('admin_menu_devel_modules_enabled');
    drupal_set_message(t('Enabled these modules: !module-list.', array('!module-list' => implode(', ', $saved_state))));
    $rebuild = TRUE;
  }
  else {
    // Allow site admins to override this variable via settings.php.
    $devel_modules = variable_get('admin_menu_devel_modules', array('cache_disable', 'coder', 'content_copy', 'debug', 'delete_all', 'demo', 'devel', 'devel_node_access', 'devel_themer', 'macro', 'form_controller', 'imagecache_ui', 'journal', 'trace', 'upgrade_status', 'user_display_ui', 'util', 'views_ui', 'views_theme_wizard'));
    // Store currently enabled modules in a variable.
    $devel_modules = array_intersect(module_list(FALSE, FALSE), $devel_modules);
    if (!empty($devel_modules)) {
      variable_set('admin_menu_devel_modules_enabled', $devel_modules);
      // Disable developer modules.
      module_disable($devel_modules);
      drupal_set_message(t('Disabled these modules: !module-list.', array('!module-list' => implode(', ', $devel_modules))));
      $rebuild = TRUE;
    }
    else {
      drupal_set_message(t('No developer modules are enabled.'));
    }
  }
  if ($rebuild) {
    // Make sure everything is rebuilt, basically a combination of the calls
    // from system_modules() and system_modules_submit().
    drupal_rebuild_theme_registry();
    menu_rebuild();
    cache_clear_all('schema', 'cache');
    cache_clear_all();
    drupal_clear_css_cache();
    drupal_clear_js_cache();
    // Synchronize to catch any actions that were added or removed.
    actions_synchronize();
  }
  drupal_goto('');
}

/**
 * Flush all caches or a specific one.
 *
 * @param $name
 *   (optional) Name of cache to flush.
 */
function admin_menu_flush_cache($name = NULL) {
  // URL token protects this against CSRF attacks.
  if(!isset($_GET['token']) || ($_GET['token'] !== drupal_get_token($_GET['q']))) {
    return MENU_ACCESS_DENIED;
  }
  switch ($name) {
    case 'admin_menu':
      admin_menu_wipe();
      break;

    case 'cache':
      // Don't clear cache_form - in-progress form submissions may break.
      // Ordered so clearing the page cache will always be the last action.
      $core = array('cache', 'cache_block', 'cache_filter', 'cache_page');
      $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
      foreach ($cache_tables as $table) {
        cache_clear_all('*', $table, TRUE);
      }
      break;

    case 'menu':
      menu_rebuild();
      break;

    case 'requisites':
      // Change query-strings on css/js files to enforce reload for all users.
      _drupal_flush_css_js();

      drupal_clear_css_cache();
      drupal_clear_js_cache();
      break;

    case 'theme':
      module_invoke('system', 'theme_data');
      drupal_rebuild_theme_registry();
      break;

    default:
      // Flush all caches; no need to re-implement this.
      module_load_include('inc', 'system', 'system.admin');
      $form = $form_state = array();
      system_clear_cache_submit($form, $form_state);
      break;
  }
  drupal_goto();
}

