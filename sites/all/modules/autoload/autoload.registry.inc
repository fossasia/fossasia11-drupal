<?php

/**
 * @file
 * This file contains the code registry parser engine. This file is backported
 * from Drupal 7 core's registry.inc file.
 */

/**
 * @defgroup registry Code registry
 * @{
 * The code registry engine.
 *
 * Autoload maintains an internal registry of all functions or classes in the
 * system, allowing it to lazy-load code files as needed (reducing the amount
 * of code that must be parsed on each request).
 */

/**
 * Does the work for autoload_registry_update().
 */
function _autoload_registry_update() {
  // Get current list of modules and their files.
  $modules = array();
  $query = db_query("SELECT * FROM {system} WHERE type = 'module'");
  // Get the list of files we are going to parse.
  $files = array();
  while ($module = db_fetch_object($query)) {
    $module->info = unserialize($module->info);
    $dir = dirname($module->filename);

    // Store the module directory for use in hook_registry_files_alter().
    $module->dir = $dir;

    if ($module->status && !empty($module->info['files'])) {
      // Add files for enabled modules to the registry.
      foreach ($module->info['files'] as $file) {
        $files["$dir/$file"] = array('module' => $module->name, 'weight' => $module->weight);
      }
    }

    $modules[$module->name] = $module;
  }
  foreach (file_scan_directory('includes', '/\.inc$/') as $filename => $file) {
    $files["$filename"] = array('module' => '', 'weight' => 0);
  }

  // Add support for the old-style hook_autoload_info() data.
  $classes = array();
  foreach (module_implements('autoload_info') as $module) {
    $module_classes = module_invoke($module, 'autoload_info');
    if (isset($module_classes) && is_array($module_classes)) {
      foreach (array_keys($module_classes) as $item) {
        $module_classes[$item] += array(
          'module' => $module,
          'file path' => drupal_get_path('module', $module),
          'weight' => isset($modules[$module]) ? $modules[$module]->weight : 0,
        );
      }
      $classes = array_merge($classes, $module_classes);
    }
  }
  // Alter the lookup table as defined by modules. The keys are class and interface names.
  drupal_alter('autoload_info', $classes);
  foreach ($classes as $item) {
    $filename = $item['file path'] . '/' . $item['file'];
    if (!isset($files[$filename])) {
      $files["$filename"] = array('module' => $item['module'], 'weight' => $item['weight']);
    }
  }

  try {
    // Allow modules to manually modify the list of files before the registry
    // parses them. The $modules array provides the .info file information, which
    // includes the list of files registered to each module. Any files in the
    // list can then be added to the list of files that the registry will parse,
    // or modify attributes of a file.
    drupal_alter('autoload_registry_files', $files, $modules);
    foreach (autoload_registry_get_parsed_files() as $filename => $file) {
      // Add the hash for those files we have already parsed.
      if (isset($files[$filename])) {
        $files[$filename]['hash'] = $file['hash'];
      }
      else {
        // Flush the registry of resources in files that are no longer on disc
        // or are in files that no installed modules require to be parsed.
        db_query("DELETE FROM {autoload_registry} WHERE filename = '%s'", $filename);
        db_query("DELETE FROM {autoload_registry_file} WHERE filename = '%s'", $filename);
      }
    }
    $parsed_files = _autoload_registry_parse_files($files);

    $unchanged_resources = array();
    $lookup_cache = array();
    if ($cache = cache_get('autoload')) {
      $lookup_cache = $cache->data;
    }
    foreach ($lookup_cache as $key => $file) {
      // If the file for this cached resource is carried over unchanged from
      // the last registry build, then we can safely re-cache it.
      if ($file && in_array($file, array_keys($files)) && !in_array($file, $parsed_files)) {
        $unchanged_resources[$key] = $file;
      }
    }
    module_implements('', '', TRUE);
    _autoload_registry_check_code(AUTOLOAD_REGISTRY_RESET_LOOKUP_CACHE);
  }
  catch (Exception $e) {
    throw $e;
  }

  // We have some unchanged resources, warm up the cache - no need to pay
  // for looking them up again.
  if (count($unchanged_resources) > 0) {
    cache_set('autoload', $unchanged_resources);
  }
}

/**
 * Return the list of files in registry_file
 */
function autoload_registry_get_parsed_files() {
  $files = array();
  // We want the result as a keyed array.
  $query = db_query("SELECT * FROM {autoload_registry_file}");
  while ($file = db_fetch_array($query)) {
    $files[$file['filename']] = $file;
  }
  return $files;
}

/**
 * Parse all files that have changed since the registry was last built, and
 * save their function and class listings.
 *
 * @param $files
 *  The list of files to check and parse.
 */
function _autoload_registry_parse_file($filename, $contents, $module = '', $weight = 0) {
  if (preg_match_all('/^\s*(?:abstract)?\s*(class|interface)\s+([a-zA-Z0-9_]+)/m', $contents, $matches)) {
    foreach ($matches[2] as $key => $name) {
      db_query("INSERT INTO {autoload_registry} (name, type, filename, module, weight) VALUES ('%s', '%s', '%s', '%s', %d)", array(
        $name,
        $matches[1][$key],
        $filename,
        $module,
        $weight,
      ));
    }
  }
}

/**
 * Parse a file and save its function and class listings.
 *
 * @param $filename
 *  Name of the file we are going to parse.
 * @param $contents
 *  Contents of the file we are going to parse as a string.
 * @param $module
 *   (optional) Name of the module this file belongs to.
 * @param $weight
 *   (optional) Weight of the module.
 */
function _autoload_registry_parse_files($files) {
  $parsed_files = array();
  foreach ($files as $filename => $file) {
    if (file_exists($filename)) {
      $hash = hash_file('sha256', $filename);
      if (empty($file['hash']) || $file['hash'] != $hash) {
        // Delete registry entries for this file, so we can insert the new resources.
        db_query("DELETE FROM {autoload_registry} WHERE filename = '%s'", $filename);
        $file['hash'] = $hash;
        $parsed_files[$filename] = $file;
      }
    }
  }
  foreach ($parsed_files as $filename => $file) {
    _autoload_registry_parse_file($filename, file_get_contents($filename), $file['module'], $file['weight']);
    db_query("UPDATE {autoload_registry_file} SET hash = '%s' WHERE filename = '%s'", $file['hash'], $filename);
    if (!db_affected_rows()) {
      db_query("INSERT INTO {autoload_registry_file} (filename, hash) VALUES ('%s', '%s')", $filename, $file['hash']);
    }
  }
  return array_keys($parsed_files);
}
