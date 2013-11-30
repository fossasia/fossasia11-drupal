<?php

/**
 * @file
 * Documentation for pathauto API.
 *
 * @see hook_token_info
 * @see hook_tokens
 */

function hook_path_alias_types() {
}

function hook_pathauto($op) {
}

/**
 * Alter Pathauto-generated aliases before saving.
 *
 * @param string $alias
 *   The automatic alias after token replacement and strings cleaned.
 * @param array $context
 *   An associative array of additional options, with the following elements:
 *   - 'module': The module or type of object being aliased.
 *   - 'op': A string with the operation being performed on the object being
 *     aliased. Can be either 'insert', 'update', 'return', or 'bulkupdate'.
 *   - 'source': A string of the source path for the alias (e.g. 'node/1').
 *     This can be altered by reference.
 *   - 'data': An array of keyed objects to pass to token_replace(). Note this
 *     variable may not be consistent in older versions of Pathauto.
 *   - 'type': The sub-type or bundle of the object being aliased.
 *   - 'language': A string of the language code for the alias (e.g. 'en').
 *     This can be altered by reference.
 *   - 'pattern': A string of the pattern used for aliasing the object.
 *
 * @see pathauto_create_alias()
 */
function hook_pathauto_alias_alter(&$alias, &$context) {
  // Add a suffix so that all aliases get saved as 'content/my-title.html'
  $alias .= '.html';

  // Force all aliases to be saved as language neutral.
  $context['language'] = '';
}

/**
 * Alter the list of punctuation characters for Pathauto control.
 *
 * @param $punctuation
 *   An array of punctuation to be controlled by Pathauto during replacement
 *   keyed by punctuation name. Each punctuation record should be an array
 *   with the following key/value pairs:
 *   - value: The raw value of the punctuation mark.
 *   - name: The human-readable name of the punctuation mark. This must be
 *     translated using t() already.
 */
function hook_pathauto_punctuation_chars_alter(array &$punctuation) {
  // Add the trademark symbol.
  $punctuation['trademark'] = array('value' => '™', 'name' => t('Trademark symbol'));

  // Remove the dollar sign.
  unset($punctuation['dollar']);
}
