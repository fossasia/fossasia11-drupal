<?php
/**
 * @file
 * Custom Formatters module integration.
 */

/**
 * Implements hook_custom_formatters_defaults().
 */
function sexybookmarks_custom_formatters_defaults() {
  $formatters = array();

  // Example: SexyBookmarks.
  $formatters['example_sexybookmarks'] = array(
    'name' => 'example_sexybookmarks',
    'label' => 'Example: SexyBookmarks',
    'field_types' => 'a:1:{i:0;s:4:"text";}',
    'multiple' => '0',
    'description' => 'An example Mobile Codes formatter',
    'mode' => 'advanced',
    'code' => '/**
 * This is an example Advanced Custom Formatter to demonstrate how to output various items as a SexyBookmarks.
 */
$profile = \'default\';

/**
 * If Devel module is present (recommended), display the contents of the $element variable to help with construction of the formatter.
 */
if (module_exists(\'devel\')) {
  dpm($element);
}

/**
 * Render Node URL as a SexyBookmark.
 */
/* -- Delete this line if you want to use this code -- /
$node = $element[\'#node\'];
$url = url("node/{$node->nid}", array(\'absolute\' => TRUE));
$title = $node->title;
return theme(\'sexybookmarks\', array(
  \'profile\' => $profile,
  \'url\' => $url,
  \'title\' => $title,
));
// */',
    'status' => '1',
  );

  return $formatters;
}
