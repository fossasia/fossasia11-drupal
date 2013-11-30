<?php
/**
 * @file
 * Link module integration.
 */

/**
 * Implements hook_sexybookmarks_field_formatter_info() on behalf of link.module.
 */
function link_sexybookmarks_field_formatter_info() {
  return array(
    'link' => array(
      'callback' => 'link_sexybookmarks_field_formatter_data'
    ),
  );
}

/**
 * Callback for Link module SexyBookmarks formatter.
 */
function link_sexybookmarks_field_formatter_data($item) {
  return array(
    'url' => $item['url'],
    'title' => $item['title'],
  );
}
