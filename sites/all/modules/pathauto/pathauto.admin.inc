<?php

/**
 * @file
 * Admin page callbacks for the Pathauto module.
 *
 * @ingroup pathauto
 */

/**
 * Form builder; Configure the URL alias patterns.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function pathauto_patterns_form($form_state) {
  module_load_include('inc', 'pathauto', 'pathauto.pathauto');

  // Call the hook on all modules - an array of 'settings' objects is returned
  $all_settings = module_invoke_all('pathauto', 'settings');
  foreach ($all_settings as $settings) {
    $module = $settings->module;
    $patterndescr = $settings->patterndescr;
    $patterndefault = $settings->patterndefault;
    $groupheader = $settings->groupheader;

    $form[$module] = array(
      '#type' => 'fieldset',
      '#title' => $groupheader,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    // Prompt for the default pattern for this module
    $variable = 'pathauto_' . $module . '_pattern';
    $form[$module][$variable] = array(
      '#type' => 'textfield',
      '#title' => $patterndescr,
      '#default_value' => variable_get($variable, $patterndefault),
      '#size' => 65,
      '#maxlength' => 1280,
      '#element_validate' => array('_pathauto_validate_pattern_element'),
      '#after_build' => array('_pathauto_validate_pattern_element'),
      '#token_types' => array($settings->token_type),
      '#min_tokens' => 1,
      '#parents' => array($variable),
    );

    // If the module supports a set of specialized patterns, set
    // them up here
    if (isset($settings->patternitems)) {
      foreach ($settings->patternitems as $itemname => $itemlabel) {
        $variable = 'pathauto_' . $module . '_' . $itemname . '_pattern';
        $form[$module][$variable] = array(
          '#type' => 'textfield',
          '#title' => $itemlabel,
          '#default_value' => variable_get($variable, ''),
          '#size' => 65,
          '#maxlength' => 1280,
          '#element_validate' => array('_pathauto_validate_pattern_element'),
          '#after_build' => array('_pathauto_validate_pattern_element'),
          '#token_types' => array($settings->token_type),
          '#min_tokens' => 1,
          '#parents' => array($variable),
        );
      }
    }

    // Display the user documentation of placeholders supported by
    // this module, as a description on the last pattern
    $form[$module]['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Use -raw replacements for text to avoid problems with HTML entities.'),
    );
    $form[$module]['token_help']['help'] = array(
      '#type' => 'markup',
      '#value' => theme('token_tree', array($settings->token_type)),
    );
  }

  return system_settings_form($form);
}

/**
 * Form builder; Configure the Pathauto settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function pathauto_settings_form() {
  module_load_include('inc', 'pathauto');

  $form['pathauto_verbose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose'),
    '#default_value' => variable_get('pathauto_verbose', FALSE),
    '#description' => t('Display alias changes (except during bulk updates).'),
  );

  $form['pathauto_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator'),
    '#size' => 1,
    '#maxlength' => 1,
    '#default_value' => variable_get('pathauto_separator', '-'),
    '#description' => t('Character used to separate words in titles. This will replace any spaces and punctuation characters. Using a space or + character can cause unexpected results.'),
  );

  $form['pathauto_case'] = array(
    '#type' => 'radios',
    '#title' => t('Character case'),
    '#default_value' => variable_get('pathauto_case', PATHAUTO_CASE_LOWER),
    '#options' => array(
      PATHAUTO_CASE_LEAVE_ASIS => t('Leave case the same as source token values.'),
      PATHAUTO_CASE_LOWER => t('Change to lower case'),
    ),
  );

  $max_length = _pathauto_get_schema_alias_maxlength();

  $form['pathauto_max_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum alias length'),
    '#size' => 3,
    '#maxlength' => 3,
    '#default_value' => variable_get('pathauto_max_length', 100),
    '#min_value' => 1,
    '#max_value' => $max_length,
    '#description' => t('Maximum length of aliases to generate. 100 is the recommended length. @max is the maximum possible length. See <a href="@pathauto-help">Pathauto help</a> for details.', array('@pathauto-help' => url('admin/help/pathauto'), '@max' => $max_length)),
    '#element_validate' => array('_pathauto_validate_numeric_element'),
  );
  $form['pathauto_max_component_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum component length'),
    '#size' => 3,
    '#maxlength' => 3,
    '#default_value' => variable_get('pathauto_max_component_length', 100),
    '#min_value' => 1,
    '#max_value' => $max_length,
    '#description' => t('Maximum text length of any component in the alias (e.g., [title]). 100 is the recommended length. @max is the maximum possible length. See <a href="@pathauto-help">Pathauto help</a> for details.', array('@pathauto-help' => url('admin/help/pathauto'), '@max' => $max_length)),
    '#element_validate' => array('_pathauto_validate_numeric_element'),
  );

  $form['pathauto_update_action'] = array(
    '#type' => 'radios',
    '#title' => t('Update action'),
    '#default_value' => variable_get('pathauto_update_action', PATHAUTO_UPDATE_ACTION_DELETE),
    '#options' => array(
      PATHAUTO_UPDATE_ACTION_NO_NEW => t('Do nothing. Leave the old alias intact.'),
      PATHAUTO_UPDATE_ACTION_LEAVE => t('Create a new alias. Leave the existing alias functioning.'),
      PATHAUTO_UPDATE_ACTION_DELETE => t('Create a new alias. Delete the old alias.'),
      PATHAUTO_UPDATE_ACTION_REDIRECT => t('Create a new alias. Redirect from old alias.'),
    ),
    '#description' => t('What should Pathauto do when updating an existing content item which already has an alias?'),
  );
  if (!module_exists('path_redirect')) {
    // Remove the redirection option if Path redirect is not enabled.
    unset($form['pathauto_update_action']['#options'][PATHAUTO_UPDATE_ACTION_REDIRECT]);
    if (variable_get('pathauto_update_action', NULL) == PATHAUTO_UPDATE_ACTION_REDIRECT) {
      // Fix the current update action variable as well.
      variable_set('pathauto_update_action', PATHAUTO_UPDATE_ACTION_DELETE);
    }
  }

  $form['pathauto_transliterate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Transliterate prior to creating alias'),
    '#default_value' => variable_get('pathauto_transliterate', FALSE),
    '#description' => t('When a pattern includes certain characters (such as those with accents) should Pathauto attempt to transliterate them into the ASCII-96 alphabet? Transliteration is handled by the Transliteration module.'),
  );
  if (!module_exists('transliteration')) {
    // Remove the redirection option if Transliteration is not enabled.
    $form['pathauto_transliterate']['#access'] = FALSE;
    variable_set('pathauto_transliterate', FALSE);
  }

  $form['pathauto_reduce_ascii'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reduce strings to letters and numbers'),
    '#default_value' => variable_get('pathauto_reduce_ascii', FALSE),
    '#description' => t('Filters the new alias to only letters and numbers found in the ASCII-96 set.'),
  );

  $form['pathauto_ignore_words'] = array(
    '#type' => 'textarea',
    '#title' => t('Strings to Remove'),
    '#default_value' => variable_get('pathauto_ignore_words', PATHAUTO_IGNORE_WORDS),
    '#description' => t('Words to strip out of the URL alias, separated by commas. Do not use this to remove punctuation.'),
    '#wysiwyg' => FALSE,
  );

  $form['punctuation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Punctuation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $punctuation = pathauto_punctuation_chars();
  foreach ($punctuation as $name => $details) {
    $details['default'] = PATHAUTO_PUNCTUATION_REMOVE;
    if ($details['value'] == variable_get('pathauto_separator', '-')) {
      $details['default'] = PATHAUTO_PUNCTUATION_REPLACE;
    }
    $form['punctuation']['pathauto_punctuation_' . $name] = array(
      '#type' => 'select',
      '#title' => $details['name'] . ' (<code>' . check_plain($details['value']) . '</code>)',
      '#default_value' => variable_get('pathauto_punctuation_' . $name, $details['default']),
      '#options' => array(
        PATHAUTO_PUNCTUATION_REMOVE => t('Remove'),
        PATHAUTO_PUNCTUATION_REPLACE => t('Replace by separator'),
        PATHAUTO_PUNCTUATION_DO_NOTHING => t('No action (do not replace)'),
      ),
    );
  }

  return system_settings_form($form);
}

/**
 * Element validation callback for URL alias patterns.
 *
 * This function performs the following validations:
 * - Checks if the pattern has at least one token.
 * - Checks if any tokens with raw companions are being used and recommends
 *   use of the raw tokens.
 */
function _pathauto_validate_pattern_element(&$element, &$form_state) {
  // Get the current value of the element (since this can be used during both
  // form display and validation).
  $value = isset($element['#value']) ? $element['#value'] : $element['#default_value'];

  // Empty patterns need no further validation.
  if (!drupal_strlen($value)) {
    return $element;
  }

  // Check to see if the required token functions are available.
  if (!function_exists('token_scan') || !function_exists('token_element_validate')) {
    drupal_set_message(t('Please make sure you are using the latest version of the Token module.'), 'error', FALSE);
    return $element;
  }

  // Run token validation.
  $element = token_element_validate($element, $form_state);

  // Find any non-raw tokens that do have a raw companion token and warn.
  if (!form_get_errors($element)) {
    // Skip if there are already errors on this field.
    module_load_include('inc', 'pathauto');
    $not_raw_tokens = array();
    $raw_tokens = _pathauto_get_raw_tokens();
    foreach (token_scan($value) as $token) {
      if (substr($token, -4) === '-raw') {
        // Skip raw tokens.
        continue;
      }
      elseif (in_array($token . '-raw', $raw_tokens)) {
        drupal_set_message(t('You are using the token [%token] which has a raw companion token [%raw_token]. For Pathauto patterns you should use the -raw version of tokens unless you really know what you are doing. See the <a href="@pathauto-help">Pathauto help</a> for more details.', array('%token' => $token, '%raw_token' => $token . '-raw', '@pathauto-help' => url('admin/help/pathauto'))), 'error', FALSE);
      }
    }
  }

  return $element;
}

/**
 * Validate a form element that should have an numeric value.
 */
function _pathauto_validate_numeric_element($element, &$form_state) {
  $value = $element['#value'];

  if (!is_numeric($value)) {
    form_error($element, t('The field %name is not a valid number.', array('%name' => $element['#title'])));
  }
  elseif (isset($element['#max_value']) && $value > $element['#max_value']) {
    form_error($element, t('The field %name cannot be greater than @max.', array('%name' => $element['#title'], '@max' => $element['#max_value'])));
  }
  elseif (isset($element['#min_value']) && $value < $element['#min_value']) {
    form_error($element, t('The field %name cannot be less than @min.', array('%name' => $element['#title'], '@min' => $element['#min_value'])));
  }
}

/**
 * Validate pathauto_settings_form form submissions.
 */
function pathauto_settings_form_validate($form, &$form_state) {
  module_load_include('inc', 'pathauto');

  // Perform a basic check for HTML characters in the strings to remove field.
  if (strip_tags($form_state['values']['pathauto_ignore_words']) != $form_state['values']['pathauto_ignore_words']) {
    form_set_error('pathauto_ignore_words', t('The <em>Strings to remove</em> field must not contain HTML. Make sure to disable any WYSIWYG editors for this field.'));
  }

  // Validate that the separator is not set to be removed per http://drupal.org/node/184119
  // This isn't really all that bad so warn, but still allow them to save the value.
  $separator = $form_state['values']['pathauto_separator'];
  $punctuation = pathauto_punctuation_chars();
  foreach ($punctuation as $name => $details) {
    if ($details['value'] == $separator) {
      $action = $form_state['values']['pathauto_punctuation_' . $name];
      if ($action == PATHAUTO_PUNCTUATION_REMOVE) {
        drupal_set_message(t('You have configured the @name to be the separator and to be removed when encountered in strings. This can cause problems with your patterns and especially with the catpath and termpath patterns. You should probably set the action for @name to be "replace by separator".', array('@name' => $details['name'])), 'error');
      }
    }
  }
}

/**
 * Form contructor for path alias bulk update form.
 *
 * @see pathauto_bulk_update_form_submit()
 * @ingroup forms
 */
function pathauto_bulk_update_form() {
  $form['#update_callbacks'] = array();

  $form['update'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select the types of un-aliased paths for which to generate URL aliases'),
    '#options' => array(),
    '#default_value' => array(),
  );

  $pathauto_settings = module_invoke_all('pathauto', 'settings');
  foreach ($pathauto_settings as $settings) {
    if (!empty($settings->batch_update_callback)) {
      $form['#update_callbacks'][$settings->batch_update_callback] = $settings;
      $form['update']['#options'][$settings->batch_update_callback] = $settings->groupheader;
    }
  }

  $form['actions']['#weight'] = 100;
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Form submit handler for path alias bulk update form.
 *
 * @see pathauto_batch_update_form()
 * @see pathauto_bulk_update_batch_finished()
 */
function pathauto_bulk_update_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('Bulk updating URL aliases'),
    'operations' => array(
      array('pathauto_bulk_update_batch_start', array()),
    ),
    'finished' => 'pathauto_bulk_update_batch_finished',
    'file' => drupal_get_path('module', 'pathauto') . '/pathauto.admin.inc',
  );

  foreach ($form_state['values']['update'] as $callback) {
    if (!empty($callback)) {
      $settings = $form['#update_callbacks'][$callback];
      if (!empty($settings->batch_file)) {
        $batch['operations'][] = array('pathauto_bulk_update_batch_process', array($callback, $settings));
      }
      else {
        $batch['operations'][] = array($callback, array());
      }
    }
  }

  batch_set($batch);
}

/**
 * Batch callback; count the current number of URL aliases for comparison later.
 */
function pathauto_bulk_update_batch_start(&$context) {
  $context['results']['count_before'] = db_result(db_query("SELECT COUNT(*) FROM {url_alias}"));
}

/**
 * Common batch processing callback for all operations.
 *
 * Required to load our include the proper batch file.
 */
function pathauto_bulk_update_batch_process($callback, $settings, &$context) {
  if (!empty($settings->batch_file)) {
    require_once './' . $settings->batch_file;
  }
  return $callback($context);
}

/**
 * Batch finished callback.
 */
function pathauto_bulk_update_batch_finished($success, $results, $operations) {
  if ($success) {
    // Count the current number of URL aliases after the batch is completed
    // and compare to the count before the batch started.
    $results['count_after'] = db_result(db_query("SELECT COUNT(*) FROM {url_alias}"));
    $results['count_changed'] = max($results['count_after'] - $results['count_before'], 0);
    if ($results['count_changed']) {
      drupal_set_message(format_plural($results['count_changed'], 'Generated 1 URL alias.', 'Generated @count URL aliases.'));
    }
    else {
      drupal_set_message(t('No new URL aliases to generate.'));
    }
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }
}

/**
 * Menu callback; select certain alias types to delete.
 */
function pathauto_admin_delete() {
  /* TODO:
   1) all - DONE
   2) all node aliases - DONE
   4) all user aliases - DONE
   5) all taxonomy aliases - DONE
   6) by node type
   7) by taxonomy vocabulary
   8) no longer existing aliases (see http://drupal.org/node/128366 )
   9) where src like 'pattern' - DON'T DO
   10) where dst like 'pattern' - DON'T DO
  */

  $form['delete'] = array(
    '#type' => 'fieldset',
    '#title' => t('Choose aliases to delete'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // First we do the "all" case
  $total_count = db_result(db_query('SELECT count(1) FROM {url_alias}'));
  $form['delete']['all_aliases'] = array(
    '#type' => 'checkbox',
    '#title' => t('All aliases'),
    '#default_value' => FALSE,
    '#description' => t('Delete all aliases. Number of aliases which will be deleted: %count.', array('%count' => $total_count)),
  );

  // Next, iterate over an array of objects/alias types which can be deleted and provide checkboxes
  $objects = module_invoke_all('path_alias_types');
  foreach ($objects as $internal_name => $label) {
    $count = db_result(db_query("SELECT count(1) FROM {url_alias} WHERE src LIKE '%s%%'", $internal_name));
    $form['delete'][$internal_name] = array(
      '#type' => 'checkbox',
      '#title' => $label, // This label is sent through t() in the hard coded function where it is defined
      '#default_value' => FALSE,
      '#description' => t('Delete aliases for all @label. Number of aliases which will be deleted: %count.', array('@label' => $label, '%count' => $count)),
    );
  }

  // Warn them and give a button that shows we mean business
  $form['warning'] = array('#value' => '<p>' . t('<strong>Note:</strong> there is no confirmation. Be sure of your action before clicking the "Delete aliases now!" button.<br />You may want to make a backup of the database and/or the url_alias table prior to using this feature.') . '</p>');
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete aliases now!'),
  );

  return $form;
}

/**
 * Process pathauto_admin_delete form submissions.
 */
function pathauto_admin_delete_submit($form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    if ($value) {
      if ($key === 'all_aliases') {
        db_query('DELETE FROM {url_alias}');
        drupal_set_message(t('All of your path aliases have been deleted.'));
      }
      $objects = module_invoke_all('path_alias_types');
      if (array_key_exists($key, $objects)) {
        db_query("DELETE FROM {url_alias} WHERE src LIKE '%s%%'", $key);
        drupal_set_message(t('All of your %type path aliases have been deleted.', array('%type' => $objects[$key])));
      }
    }
  }
  $form_state['redirect'] = 'admin/build/path/delete_bulk';
}
