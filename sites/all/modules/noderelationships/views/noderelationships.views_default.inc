<?php
// $Id: noderelationships.views_default.inc,v 1.1.2.6 2009/06/13 18:36:26 markuspetrux Exp $

/**
 * @file
 * Provide default views for the Node Relationships module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function noderelationships_views_default_views() {
  $backref_view = &_noderelationships_views_default_views_backref();
  $noderef_view = &_noderelationships_views_default_views_noderef();
  return array(
    $backref_view->name => $backref_view,
    $noderef_view->name => $noderef_view,
  );
}

/**
 * Default view to provide back references.
 */
function &_noderelationships_views_default_views_backref() {
  $view = new view;
  $view->name = NODERELATIONSHIPS_BACKREF_VIEW_NAME;
  $view->description = 'Back references to nodes of any type. The Node Relationships module will dynamically add the proper relationship as well as the arguments to filter by the node type and nodereference field depending on the context the view is executed.';
  $view->tag = NODERELATIONSHIPS_BACKREF_VIEW_TAG;
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;

  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'link_to_node' => TRUE,
      'relationship' => 'none',
    ),
    'name' => array(
      'label' => 'Author',
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'link_to_user' => 1,
      'exclude' => 0,
      'relationship' => 'none',
    ),
    'changed' => array(
      'label' => 'Last update',
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
      'operator' => '=',
      'value' => '',
      'group' => 0,
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 10);
  $handler->override_option('use_pager', '1');
  $handler->override_option('use_more', 1);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'title' => 'title',
      'name' => 'name',
      'changed' => 'changed',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'changed' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'changed',
  ));

  $handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('empty', 'No items were found.');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('path', NODERELATIONSHIPS_VIEW_PATH_PREFIX .'/'. str_replace('_', '-', $view->name));

  return $view;
}

/**
 * Default view to enhance node reference fields with search and reference capabilities.
 */
function &_noderelationships_views_default_views_noderef() {
  $view = new view;
  $view->name = NODERELATIONSHIPS_NODEREF_VIEW_NAME;
  $view->description = 'Default view to enhance node reference fields with search and reference capabilities. The Node Relationships module will dynamically add the proper filters by referenceable types depending on the context the view is executed.';
  $view->tag = NODERELATIONSHIPS_NODEREF_VIEW_TAG;
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;

  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'link_to_node' => 1,
      'relationship' => 'none',
    ),
    'name' => array(
      'label' => 'Author',
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'link_to_user' => 1,
      'exclude' => 0,
      'relationship' => 'none',
    ),
    'changed' => array(
      'label' => 'Last update',
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
      'operator' => '=',
      'value' => '',
      'group' => 0,
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'relationship' => 'none',
    ),
    'title' => array(
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'case' => 0,
      'operator' => 'contains',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'title_op',
        'identifier' => 'title',
        'label' => 'Title',
        'optional' => 1,
        'remember' => 1,
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'operator' => 'in',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'type_op',
        'identifier' => 'type',
        'label' => 'Type',
        'optional' => 0,
        'single' => 0,
        'remember' => 1,
        'reduce' => 1,
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('empty', 'No items were found.');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'title' => 'title',
      'name' => 'name',
      'changed' => 'changed',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'changed' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'changed',
  ));

  $handler = $view->new_display('page', 'Table style', 'page_table');
  $handler->override_option('path', NODERELATIONSHIPS_VIEW_PATH_PREFIX .'/'. str_replace('_', '-', $view->name) .'/page-table');

  $handler = $view->new_display('page', 'Grid style', 'page_grid');
  $handler->override_option('style_plugin', 'grid');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'columns' => '4',
    'alignment' => 'horizontal',
  ));
  $handler->override_option('sorts', array(
    'changed' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
  ));
  $handler->override_option('path', NODERELATIONSHIPS_VIEW_PATH_PREFIX .'/'. str_replace('_', '-', $view->name) .'/page-grid');

  return $view;
}
