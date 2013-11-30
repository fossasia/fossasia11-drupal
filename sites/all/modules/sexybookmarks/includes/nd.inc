<?php
/**
 * @file
 * Node Displays module integration.
 */

/**
 * Implements hook_sexybookmarks_init() on behalf of nd.module.
 */
function nd_sexybookmarks_init() {
  $nd_fields = isset($GLOBALS['conf']['nd_fields']) ? $GLOBALS['conf']['nd_fields'] : array();
  if (!isset($nd_fields['sexybookmarks'])) {
    $nd_fields['sexybookmarks'] = array(
      'title' => 'SexyBookmarks',
      'exclude' => array(),
      'type' => DS_FIELD_TYPE_CODE,
      'status' => DS_FIELD_STATUS_CUSTOM,
      'properties' => array(
        'formatters' => array(
          'ds_eval_code' => 'Default',
        ),
        'code' => '<?php
return theme(\'sexybookmarks\', array(
  \'profile\' => variable_get("node_sexybookmarks_profile_{$object->type}", \'default\'),
  \'url\' => url("node/{$node->nid}", array(\'absolute\' => TRUE)),
  \'title\' => $node->title,
));
?>',
        'token' => 0
      ),
    );
    variable_set('nd_fields', $nd_fields);
  }
}
