<?php
/**
 * @file
 * Node Reference module integration.
 */

/**
 * Implements hook_sexybookmarks_field_formatter_info() on behalf of link.module.
 */
function nodereference_sexybookmarks_field_formatter_info() {
  return array(
    'nodereference' => array(
      'callback' => 'nodereference_sexybookmarks_field_formatter_data'
    ),
  );
}

/**
 * Callback for Node Reference module SexyBookmarks formatter.
 */
function nodereference_sexybookmarks_field_formatter_data($item) {
  $node = node_load($item['nid']);

  return array(
    'url' => url("node/{$node->nid}", array('absolute' => TRUE)),
    'title' => $node->title,
  );
}
