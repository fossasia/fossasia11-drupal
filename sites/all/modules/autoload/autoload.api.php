<?php

/**
 * @file
 * Hooks provided by the Autoload module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Perform necessary alterations to the list of files parsed by the registry.
 *
 * Modules can manually modify the list of files before the registry parses
 * them. The $modules array provides the .info file information, which includes
 * the list of files registered to each module. Any files in the list can then
 * be added to the list of files that the registry will parse, or modify
 * attributes of a file.
 *
 * @param $files
 *   List of files to be parsed by the registry. The list will contain
 *   files found in each enabled module's info file and the core includes
 *   directory. The array is keyed by the file path and contains an array of
 *   the related module's name and weight as used internally by
 *   _autoload_registry_update() and related functions.
 *
 *   For example:
 *   @code
 *     $files["modules/system/system.module"] = array(
 *       'module' => 'system',
 *       'weight' => 0,
 *     );
 *   @endcode
 * @param $modules
 *   An array containing all module information stored in the {system} table.
 *   Each element of the array also contains the module's .info file
 *   information in the property 'info'. An additional 'dir' property has been
 *   added to the module information which provides the path to the directory
 *   in which the module resides. The example shows how to take advantage of
 *   both properties.
 *
 * @see _autoload_registry_update()
 */
function hook_autoload_registry_files_alter(&$files, $modules) {
  foreach ($modules as $module) {
    // Only add test files for disabled modules, as enabled modules should
    // already include any test files they provide.
    if (!$module->status) {
      $dir = $module->dir;
      foreach ($module->info['files'] as $file) {
        if (substr($file, -5) == '.test') {
          $files["$dir/$file"] = array('module' => $module->name, 'weight' => $module->weight);
        }
      }
    }
  }
}

/**
 * @} End of "addtogroup hooks".
 */
