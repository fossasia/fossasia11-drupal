<?php

/**
 * Implementation of hook_drush_command().
 */
function imagecache_drush_command() {
  $items = array();

  $items['imagecache-flush'] = array(
    'callback' => 'imagecache_drush_preset_flush',
    'description' => dt('Flush an imagecache preset.'),
    'examples' => array(
      'drush imagecache-flush foobar' => dt('Flush the ImageCache preset "foobar".'),
    ),
    'aliases' => array('icf'),
  );

  $items['imagecache-build'] = array(
    'callback' => 'imagecache_drush_preset_build',
    'description' => dt('Build imagecache derivates for all images for a given preset.'),
    'examples' => array(
      'drush imagecache-build foobar' => dt('Build all images for preset "foobar".'),
    ),
    'aliases' => array('icb'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function imagecache_drush_help($section) {
  switch ($section) {
    case 'drush:imagecache-flush':
      return dt('Flush a given preset.');
    case 'drush:imagecache-build':
      return dt('Build derivative images for a given preset.');
  }
}

/**
 * Drush callback to perform actual imagecache preset flush.
 */
function imagecache_drush_preset_flush() {
  $args = func_get_args();

  // Rebuild imagecache presets.
  foreach (imagecache_presets(TRUE) as $preset) {
    $preset_names[] = $preset['presetname'];
  }

  if (empty($args)) {
    $choice = drush_choice($preset_names, 'Enter a number to choose which preset to flush.');
    if ($choice !== FALSE) {
      $args[] = $preset_names[$choice];
    }
  }
  else {
    // Implement 'all'
    if (count($args) == 1 && $args[0] == 'all') {
      $args = $preset_names;
    }
  }

  // Remove any invalid preset names and report them as errors.
  $not_found = array_diff($args, $preset_names);
  $args = array_intersect($args, $preset_names);
  if ($not_found) {
    drush_log(dt('Preset(s) not found: @presets', array('@presets' => implode($not_found, ' '))), 'error');
  }

  if (empty($args)) {
    return FALSE;
  }

  $path = drush_get_context('DRUSH_DRUPAL_ROOT') .'/'. file_directory_path() .'/imagecache/';
  foreach ($args as $arg) {
    // Load preset.
    if ($preset = imagecache_preset_by_name($arg)) {
      // This mimics the logic inside of the function
      // imagecache_preset_flush(), but without the access check.
      $presetdir = $path . $preset['presetname'];
      if (is_dir($presetdir)) {
        _imagecache_recursive_delete($presetdir);
        drush_log(dt('Flushed "@preset" preset.', array('@preset' => $arg)), 'ok');
      }
      else {
        drush_log(dt('Cache for preset "@preset" was already empty.', array('@preset' => $arg)), 'ok');
      }
    }
  }
  return TRUE;
}

/**
 * Drush callback to perform actual imagecache preset build.
 */
function imagecache_drush_preset_build() {
  $args = func_get_args();

  // Rebuild imagecache presets.
  foreach (imagecache_presets(TRUE) as $preset) {
    $preset_names[] = $preset['presetname'];
  }

  if (empty($args)) {
    $choice = drush_choice($preset_names, 'Enter a number to choose which preset to flush.');
    if ($choice !== FALSE) {
      $args[] = $preset_names[$choice];
    }
  }
  elseif ($args[0] == 'all') {
    // Implement 'all'
    $args = $preset_names;
  }

  // Remove any invalid preset names and report them as errors.
  $not_found = array_diff($args, $preset_names);
  $args = array_intersect($args, $preset_names);
  if ($not_found) {
    drush_log(dt('Preset(s) not found: @presets', array('@presets' => implode($not_found, ' '))), 'error');
  }

  if (empty($args)) {
    return FALSE;
  }

  // Get a list of files to processes.
  $file_query = db_query("SELECT filepath FROM {files} where filemime LIKE 'image%' ORDER BY fid DESC");
  $files = array();
  drush_log(dt('Generating file list...', array()), 'ok');
  while ($filepath = db_result($file_query)) {
    if (file_exists($filepath)) {
      $files[] = $filepath;
    }
  }
  if (empty($files)) {
    drush_log(dt('No images found in the files table.', array()), 'error');
    return FALSE;
  }
  $count = count($files);
  drush_log(dt('Done. @count files to process using these presets: @presets', array('@count' => $count, '@presets' => implode(' ', $args))), 'ok');

  // Generate the images.
  $counter = 0;
  $mod = round($count / 200);
  foreach ($files as $filepath) {
    foreach ($args as $arg) {
      $path = imagecache_create_path($arg, $filepath);
      if (!file_exists($path)) {
        imagecache_generate_image($arg, $filepath);
        if (file_exists($path)) {
          drush_log(dt('File "@file" created.', array('@file' => $path)), 'ok');
        }
        else {
          drush_log(dt('File "@file" not created.', array('@file' => $path)), 'error');
        }
      }
    }
    // Output progress.
    $counter++;
    if ($counter % $mod == 0) {
      drush_log(dt('@percent% done.', array('@percent' => round($counter / $count * 100, 2))), 'ok');
    }
  }

  return TRUE;
}
