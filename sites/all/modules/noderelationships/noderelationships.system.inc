<?php
// $Id: noderelationships.system.inc,v 1.1.2.3 2009/12/28 05:16:04 markuspetrux Exp $

/**
 * @file
 * Implementation of system related functions that are not executed
 * during normal site operation.
 */

/**
 * Implementation of hook_theme().
 */
function _noderelationships_theme() {
  return array(
    // Entity relation diagram.
    'noderelationships_erd_diagram' => array(
      'arguments' => array('diagram' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),
    'noderelationships_erd_group' => array(
      'arguments' => array('group_type' => NULL, 'entities' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),
    'noderelationships_erd_current' => array(
      'arguments' => array('label' => NULL, 'items' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),
    'noderelationships_erd_entity' => array(
      'arguments' => array('label' => NULL, 'items' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),
    'noderelationships_erd_relation' => array(
      'arguments' => array('label' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),

    // Node relationship settings.
    'noderelationships_admin_settings_noderef' => array(
      'arguments' => array('form' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),
    'noderelationships_admin_settings_backref' => array(
      'arguments' => array('form' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),
    'noderelationships_admin_help' => array(
      'arguments' => array('message' => NULL),
      'file' => 'noderelationships.admin.inc',
    ),

    // Back reference views.
    'noderelationships_backref_view' => array(
      'arguments' => array('title' => NULL, 'content' => NULL),
      'file' => 'noderelationships.pages.inc',
    ),
    'noderelationships_formatter_default' => array(
      'arguments' => array('element' => NULL),
      'file' => 'noderelationships.pages.inc',
    ),
    'noderelationships_formatter_count' => array(
      'arguments' => array('element' => NULL),
      'file' => 'noderelationships.pages.inc',
    ),

    // Node reference extras.
    'noderelationships_noderef_page_tabs' => array(
      'arguments' => array('mode' => NULL, 'referrer_type' => NULL, 'field_name' => NULL),
      'file' => 'noderelationships.pages.inc',
    ),
    'noderelationships_noderef_page_content' => array(
      'arguments' => array('content' => NULL),
      'file' => 'noderelationships.pages.inc',
    ),
    'noderelationships_noderef_page_prefix' => array(
      'arguments' => array(),
      'file' => 'noderelationships.pages.inc',
    ),
    'noderelationships_noderef_page_suffix' => array(
      'arguments' => array(),
      'file' => 'noderelationships.pages.inc',
    ),
    'noderelationships_noderef_singleselect' => array(
      'arguments' => array('referrer_field' => NULL),
      'file' => 'noderelationships.pages.inc',
    ),
    'noderelationships_noderef_multiselect' => array(
      'arguments' => array('referrer_field' => NULL),
      'file' => 'noderelationships.pages.inc',
    ),
  );
}

/**
 * Implementation of hook_menu().
 */
function _noderelationships_menu() {
  $items = array();

  // Include common module functions.
  module_load_include('inc', 'noderelationships');

  // Any event which causes a menu_rebuild could potentially mean that the
  // relationships data is updated -- content type changes, etc.
  noderelationships_cache_clear();

  // Build new menu items for each content type (relationships management).
  foreach (array_keys(node_get_types()) as $type_name) {
    $type_url_str = str_replace('_', '-', $type_name);
    $items['admin/content/node-type/'. $type_url_str .'/relationships'] = array(
      'title' => 'Relationships',
      'page callback' => 'noderelationships_admin_page',
      'page arguments' => array(5, $type_name),
      'access arguments' => array('administer content types'),
      'type' => MENU_LOCAL_TASK,
      'weight' => 10,
      'file' => 'noderelationships.admin.inc',
    );
    $admin_tabs = array(
      'erd' => 'Entity relations diagram',
      'noderef' => 'Node reference extras',
      'backref' => 'Back reference settings',
    );
    $item_weight = 0;
    foreach ($admin_tabs as $op => $title) {
      $items['admin/content/node-type/'. $type_url_str .'/relationships/'. $op] = array(
        // Coder note: the content of $title is a literal string, which is fine.
        'title' => $title,
        'page callback' => 'noderelationships_admin_page',
        'page arguments' => array(5, $type_name),
        'access arguments' => array('administer content types'),
        'type' => ($item_weight == 0 ? MENU_DEFAULT_LOCAL_TASK : MENU_LOCAL_TASK),
        'weight' => $item_weight,
        'file' => 'noderelationships.admin.inc',
      );
      $item_weight++;
    }
  }

  // Add back reference tab to nodes. Note this tab is added for all nodes,
  // but visibility is controlled by the access callback function.
  $items['node/%node/relationships'] = array(
    'title' => 'Relationships',
    'page callback' => 'noderelationships_backref_page',
    'page arguments' => array(1),
    'access callback' => 'noderelationships_backref_access',
    'access arguments' => array(1),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'file' => 'noderelationships.pages.inc',
  );

  // Search or Create and reference.
  $items['noderelationships/search'] = array(
    'title' => 'Search and reference',
    'page callback' => 'noderelationships_noderef_page',
    'page arguments' => array(1),
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
    'file' => 'noderelationships.pages.inc',
  );
  $items['noderelationships/create'] = array(
    'title' => 'Create and reference',
    'page callback' => 'noderelationships_noderef_page',
    'page arguments' => array(1),
    'access callback' => '_node_add_access',
    'type' => MENU_CALLBACK,
    'file' => 'noderelationships.pages.inc',
  );
  $items['noderelationships/ahah/search'] = array(
    'title' => 'Search and reference',
    'page callback' => 'noderelationships_noderef_ahah_search',
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
    'file' => 'noderelationships.pages.inc',
  );

  return $items;
}
