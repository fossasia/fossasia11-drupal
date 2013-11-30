<?php

/**
 * @file
 * Miscellaneous functions for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * Condition builder for queries.
 */
function _xmlsitemap_build_conditions(array &$conditions, array $args = array(), array $options = array()) {
  $options += array(
    'table' => 'xmlsitemap',
    'operator' => '=',
    'update' => FALSE,
  );

  $operators = array(
    'null' => array('=' => 'IS', '<>' => 'IS NOT'),
    'in' => array('=' => 'IN', '<>' => 'NOT IN'),
  );

  foreach ($conditions as $field => $value) {
    if (is_int($field)) {
      continue;
    }
    elseif ($value === NULL) {
      if ($options['update']) {
        $conditions[$field] = "$field = NULL";
      }
      else {
        $operator = $operators['null'][$options['operator']];
        $conditions[$field] = "$field $operator NULL";
      }
    }
    elseif (is_array($value)) {
      if ($options['update']) {
        trigger_error(strtr('Update not supported for field %field in @function.', array('%field' => theme('placeholder', $field), '@function' => __FUNCTION__)));
        unset($conditions[$field]);
      }
      else {
        $operator = $operators['in'][$options['operator']];
        $type = _xmlsitemap_get_field_type($options['table'], $field);
        $conditions[$field] = "$field $operator (" . db_placeholders($value, $type) . ")";
        $args = array_merge($args, $value);
      }
    }
    else {
      $operator = $options['operator'];
      $placeholder = db_type_placeholder(_xmlsitemap_get_field_type($options['table'], $field));
      $conditions[$field] = "$field $operator $placeholder";
      $args[] = $value;
    }
  }

  return $args;
}

/**
 * Special implementation of drupal_write_record() to allow NULL values.
 *
 * @todo Remove when http://drupal.org/node/227677 is fixed.
 */
function xmlsitemap_write_record($table, &$object, $update = array()) {
  // Standardize $update to an array.
  if (is_string($update)) {
    $update = array($update);
  }

  $schema = drupal_get_schema($table);
  if (empty($schema)) {
    return FALSE;
  }

  // Convert to an object if needed.
  if (is_array($object)) {
    $object = (object) $object;
    $array = TRUE;
  }
  else {
    $array = FALSE;
  }

  $fields = $defs = $values = $serials = $placeholders = array();
  $object_fields = get_object_vars($object);

  // Go through our schema, build SQL, and when inserting, fill in defaults for
  // fields that are not set.
  foreach ($schema['fields'] as $field => $info) {
    // Special case -- skip serial types if we are updating.
    if ($info['type'] == 'serial') {
      if (empty($update)) {
        // Track serial fields so we can helpfully populate them after the query.
        $serials[] = $field;
      }
      continue;
    }

    // For inserts, populate defaults from Schema if not already provided
    if (!isset($object->$field) && !count($update) && isset($info['default']) && !array_key_exists($field, $object_fields)) {
      $object->$field = $info['default'];
    }

    // Build arrays for the fields, placeholders, and values in our query.
    if (isset($object->$field) || (array_key_exists($field, $object_fields) && empty($info['not null']))) {
      $fields[] = $field;
      if (isset($object->$field)) {
        $placeholders[] = db_type_placeholder($info['type']);
        $values[] = empty($info['serialize']) ? $object->$field : serialize($object->$field);
      }
      else {
        $placeholders[] = '%s';
        $values[] = 'NULL';
      }
    }
  }

  // Build the SQL.
  $query = '';
  if (!count($update)) {
    $query = "INSERT INTO {". $table ."} (". implode(', ', $fields) .') VALUES ('. implode(', ', $placeholders) .')';
    $return = SAVED_NEW;
  }
  else {
    $query = '';
    foreach ($fields as $id => $field) {
      if ($query) {
        $query .= ', ';
      }
      $query .= $field .' = '. $placeholders[$id];
    }

    foreach ($update as $key) {
      $conditions[] = "$key = ". db_type_placeholder($schema['fields'][$key]['type']);
      $values[] = $object->$key;
    }

    $query = "UPDATE {". $table ."} SET $query WHERE ". implode(' AND ', $conditions);
    $return = SAVED_UPDATED;
  }

  // Execute the SQL.
  if (db_query($query, $values)) {
    if ($serials) {
      // Get last insert ids and fill them in.
      foreach ($serials as $field) {
        $object->$field = db_last_insert_id($table, $field);
      }
    }
  }
  else {
    $return = FALSE;
  }

  // If we began with an array, convert back so we don't surprise the caller.
  if ($array) {
    $object = (array) $object;
  }

  return $return;
}

/**
 * Fetch a short blurb string about module maintainership and sponsors.
 *
 * This message will be FALSE in 'official' releases.
 */
function _xmlsitemap_get_blurb($check_version = TRUE) {
  static $blurb;

  if (!isset($blurb)) {
    $blurb = FALSE;
    if (!$check_version || (($version = _xmlsitemap_get_version()) && preg_match('/dev|unstable|alpha|beta|HEAD/i', $version))) {
      $sponsors = array(
        l('Symantec', 'http://www.symantec.com/'),
        l('WebWise Solutions', 'http://www.webwiseone.com/'),
        l('Volacci', 'http://www.volacci.com/'),
        l('lanetro', 'http://www.lanetro.com/'),
        l('Coupons Dealuxe', 'http://couponsdealuxe.com/'),
      );
      // Don't extract the following string for translation.
      $blurb = '<div class="description"><p>Thank you for helping test the XML sitemap module rewrite. Please consider helping offset developer free time by <a href="http://davereid.chipin.com/">donating</a> or if your company is interested in sponsoring the rewrite or a specific feature, please <a href="http://davereid.net/contact">contact the developer</a>. Thank you to the following current sponsors: ' . implode(', ', $sponsors) . ', and all the individuals that have donated. This message will not be seen in the stable versions.</p></div>';
      //http://drupalmodules.com/module/xml-sitemap
    }
  }

  return $blurb;
}

function _xmlsitemap_get_version() {
  static $version;
  if (!isset($version)) {
    $modules = module_rebuild_cache();
    $version = $modules['xmlsitemap']->info['version'];
  }
  return $version;
}

/**
 * Check the status of all hook_requirements() from any xmlsitemap modules.
 */
function xmlsitemap_check_status() {
  $messages = &xmlsitemap_static(__FUNCTION__);

  if (!isset($messages)) {
    // Cache the list of modules that are checked.
    if ($cache = cache_get('xmlsitemap:registry:requirements')) {
      $modules = $cache->data;
    }
    else {
      $modules = array();
      module_load_all_includes('install');
      foreach (module_implements('requirements') as $module) {
        if (strpos($module, 'xmlsitemap') !== FALSE) {
          $modules[] = $module;
        }
      }
      cache_set('xmlsitemap:registry:requirements', $modules);
    }

    $messages = array();
    foreach ($modules as $module) {
      module_load_install($module);
      $requirements = module_invoke($module, 'requirements', 'runtime');
      foreach ($requirements as $requirement) {
        if (isset($requirement['severity']) && max(REQUIREMENT_OK, $requirement['severity'])) {
          $messages[] = $requirement['description'];
        }
      }
    }

    if ($messages) {
      $message = t('One or more problems were detected with your XML sitemap configuration: !messages', array('!messages' => theme('item_list', $messages)));
      if (user_access('access site reports')) {
        $message .= t('Check the <a href="@status-report">status report</a> for more information.', array('@status-report' => url('admin/reports/status')));
      }
      drupal_set_message($message, 'warning', FALSE);
    }
  }

  return !empty($messages);
}
