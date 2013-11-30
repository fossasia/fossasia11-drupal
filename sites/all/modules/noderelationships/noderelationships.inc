<?php
// $Id: noderelationships.inc,v 1.1.2.30 2010/05/16 18:26:01 markuspetrux Exp $

/**
 * @file
 * Common functions for the noderelationships module.
 */

/**
 * Get the list of back reference regions.
 */
function noderelationships_get_back_reference_regions() {
  return array(
    NODERELATIONSHIPS_BACKREF_REGION_FIELD => t('Fields: Render back reference views as content fields'),
    NODERELATIONSHIPS_BACKREF_REGION_PAGE => t('Page: Render back reference views grouped on a single fieldset on main node view'),
    NODERELATIONSHIPS_BACKREF_REGION_TAB => t('Tab: Render back reference views in separate relationships tab'),
  );
}

/**
 * Get a list of relationship settings matching the given conditions.
 *
 * @param $where
 *   Expression for the WHERE clause of the query.
 * @param $args
 *   A variable list of arguments for the query.
 */
function noderelationships_settings_list($where) {
  $args = func_get_args();
  array_shift($args);
  // Check for 'All arguments in one array' syntax.
  if (isset($args[0]) && is_array($args[0])) {
    $args = $args[0];
  }
  $result = db_query('SELECT type_name, relation_type, related_type, field_name, settings FROM {noderelationships_settings} WHERE '. $where, $args);
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $row->settings = (!empty($row->settings) ? (array)unserialize($row->settings) : array());
    $rows[] = $row;
  }
  return $rows;
}

/**
 * Get relationship settings for the given content type.
 *
 * @param $nodetype
 *   The node type.
 * @param $settings_group
 *   The group of settings the caller is interested in.
 *   Optional, options: 'noderef', 'backref', 'all' (default).
 *
 * @return array
 *   The requested settings array.
 */
function noderelationships_settings_load($nodetype, $settings_group = 'all') {
  // Build default settings structure.
  $settings = array(
    'noderef' => array(
      'search_and_reference_view' => array(),
      'view_in_new_window' => array(),
      'edit_reference' => array(),
      'create_and_reference' => array(),
      'translate_and_reference' => array(),
    ),
    'backref' => array(
      'regions' => array(),
    ),
  );

  // Read settings from database.
  $regions = noderelationships_get_back_reference_regions();
  foreach (noderelationships_settings_list("type_name = '%s'", $nodetype) as $row) {
    $relation_key = $row->related_type .':'. $row->field_name;

    if ($row->relation_type == 'noderef') {
      if (!empty($row->settings['search_and_reference_view'])) {
        $settings['noderef']['search_and_reference_view'][$row->field_name] = $row->settings['search_and_reference_view'];
      }
      if (!empty($row->settings['view_in_new_window'])) {
        $settings['noderef']['view_in_new_window'][$row->field_name] = $row->field_name;
      }
      if (!empty($row->settings['edit_reference'])) {
        $settings['noderef']['edit_reference'][$row->field_name] = $row->field_name;
      }
      if (!empty($row->settings['create_and_reference'])) {
        $settings['noderef']['create_and_reference'][$row->field_name] = $row->field_name;
      }
      if (!empty($row->settings['translate_and_reference'])) {
        $settings['noderef']['translate_and_reference'][$row->field_name] = $row->field_name;
      }
    }
    elseif ($row->relation_type == 'backref') {
      $region = $row->settings['region'];
      if (isset($regions[$region])) {
        if (!isset($settings['backref']['regions'][$region])) {
          $settings['backref']['regions'][$region] = array();
        }
        $settings['backref']['regions'][$region][$relation_key] = array(
          'weight' => $row->settings['weight'],
          'back_reference_view' => $row->settings['back_reference_view'],
        );
      }
    }
  }

  // Sort back reference regions by weight.
  foreach (array_keys($settings['backref']['regions']) as $region) {
    noderelationships_settings_region_sort($settings['backref']['regions'][$region]);
  }

  if ($settings_group == 'noderef') {
    return $settings['noderef'];
  }
  elseif ($settings_group == 'backref') {
    return $settings['backref'];
  }

  return $settings;
}

/**
 * Set relationship settings for the given content type.
 */
function noderelationships_settings_save($nodetype, $settings) {
  $old_settings_hash = md5(serialize(noderelationships_settings_load($nodetype)));
  $new_settings_hash = md5(serialize($settings));
  // Quit if settings have not been changed.
  if ($old_settings_hash == $new_settings_hash) {
    return FALSE;
  }

  // Prepare the settings for database storage.
  $settings_rows = array();
  if (!empty($settings['noderef'])) {
    $noderef_settings = $settings['noderef'];
    if (!empty($noderef_settings) && is_array($noderef_settings)) {
      foreach ($noderef_settings as $option_name => $option_values) {
        if (!empty($option_values) && is_array($option_values)) {
          foreach ($option_values as $field_name => $value) {
            $settings_key = 'noderef::'. $field_name;
            if (!isset($settings_rows[$settings_key])) {
              $settings_rows[$settings_key] = array();
            }
            $settings_rows[$settings_key][$option_name] = $value;
          }
        }
      }
    }
  }
  if (!empty($settings['backref']) && !empty($settings['backref']['regions'])) {
    $backref_settings = $settings['backref']['regions'];
    if (!empty($backref_settings) && is_array($backref_settings)) {
      foreach ($backref_settings as $region => $region_relations) {
        if (!empty($region_relations) && is_array($region_relations)) {
          foreach ($region_relations as $relation_key => $relation_info) {
            $settings_key = 'backref:'. $relation_key;
            $relation_info['region'] = $region;
            $settings_rows[$settings_key] = $relation_info;
          }
        }
      }
    }
  }

  // Update the settings in the database.
  db_query("DELETE FROM {noderelationships_settings} WHERE type_name = '%s'", $nodetype);
  foreach ($settings_rows as $settings_key => $settings_row) {
    list($relation_type, $related_type, $field_name) = explode(':', $settings_key);
    $args = array($nodetype, $relation_type, $related_type, $field_name, serialize($settings_row));
    db_query("INSERT INTO {noderelationships_settings} (type_name, relation_type, related_type, field_name, settings) VALUES ('%s', '%s', '%s', '%s', '%s')", $args);
  }

  // Synchronize back reference settings with back reference fields.
  noderelationships_cck_backref_sync_fields($nodetype);

  return TRUE;
}

/**
 * Delete relationship settings matching the given conditions.
 *
 * @param $where
 *   Expression for the WHERE clause of the delete query.
 * @param $args
 *   A variable list of arguments for the delete query.
 */
function noderelationships_settings_delete($where) {
  $args = func_get_args();
  array_shift($args);
  // Check for 'All arguments in one array' syntax.
  if (isset($args[0]) && is_array($args[0])) {
    $args = $args[0];
  }
  db_query('DELETE FROM {noderelationships_settings} WHERE '. $where, $args);
}

/**
 * Delete relationship settings when a nodereference field has been deleted.
 */
function noderelationships_settings_delete_nodereference($nodetype, $field_name) {
  // Prepare data to delete relations related to a CCK field instance.
  $args = array($nodetype, $field_name);
  $noderef_conditions = "relation_type = 'noderef' AND type_name = '%s' AND field_name = '%s'";
  $backref_conditions = "relation_type = 'backref' AND related_type = '%s' AND field_name = '%s'";

  // Delete node reference extras settings.
  noderelationships_settings_delete($noderef_conditions, $args);

  // Scan all back reference settings affected by this instance.
  $types_to_sync = array();
  foreach (noderelationships_settings_list($backref_conditions, $args) as $row) {
    if (!isset($types_to_sync[$row->type_name])) {
      $types_to_sync[$row->type_name] = $row->type_name;
    }
  }

  // Delete back reference related settings.
  noderelationships_settings_delete($backref_conditions, $args);

  // Synchronize back reference settings with back reference fields.
  noderelationships_cck_backref_sync_fields($types_to_sync);
}

/**
 * Update relationship settings when a content type has been renamed.
 */
function noderelationships_settings_rename_type($oldname, $newname) {
  db_query("UPDATE {noderelationships_settings} SET type_name = '%s' WHERE type_name = '%s'", array($newname, $oldname));
  db_query("UPDATE {noderelationships_settings} SET related_type = '%s' WHERE related_type = '%s'", array($newname, $oldname));

  // Load the list of potentially affected back reference related types.
  $types_to_sync = array($newname => $newname);
  $result = db_query("SELECT DISTINCT type_name FROM {noderelationships_settings} WHERE related_type = '%s'", $newname);
  while ($row = db_fetch_object($result)) {
    if (!isset($types_to_sync[$row->type_name])) {
      $types_to_sync[$row->type_name] = $row->type_name;
    }
  }
  // Synchronize back reference settings with back reference fields.
  noderelationships_cck_backref_sync_fields($types_to_sync);
}

/**
 * Sort back reference regions by weight.
 */
function noderelationships_settings_region_sort(&$region) {
  uasort($region, 'noderelationships_settings_element_sort');
}

/**
 * Helper function to sort relations by 'weight' attribute.
 *
 * Based on element_sort(), which acts on '#weight' keys.
 */
function noderelationships_settings_element_sort($a, $b) {
  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    return 0;
  }
  return ($a_weight < $b_weight) ? -1 : 1;
}

/**
 * Get the list of all content types that have some kind of relation.
 */
function noderelationships_get_relationed_types($reset = FALSE) {
  $relationships = noderelationships_get_relationships($reset);
  $relationed_types = array_keys($relationships['referrer']);
  foreach (array_keys($relationships['referred']) as $type) {
    if (!isset($relationships['referrer'][$type])) {
      $relationed_types[] = $type;
    }
  }
  sort($relationed_types);
  return $relationed_types;
}

/**
 * Get the list of content types related to the given type that can be created
 * by the current user.
 */
function noderelationships_get_creatable_types($nodetype, $reset = FALSE) {
  static $creatable_types;
  if (!isset($creatable_types) || $reset) {
    $creatable_types = array();
  }
  if (!isset($creatable_types[$nodetype])) {
    $creatable_types[$nodetype] = array();
    foreach (array_keys(noderelationships_get_referred_types($nodetype, $reset)) as $referred_type) {
      if (node_access('create', $referred_type)) {
        $creatable_types[$nodetype][$referred_type] = $referred_type;
      }
    }
  }
  return $creatable_types[$nodetype];
}

/**
 * Get the list of node reference fields for the given type.
 */
function noderelationships_get_reference_fields($nodetype, $reset = FALSE) {
  static $reference_fields;
  if (!isset($reference_fields)) {
    $reference_fields = array();
  }
  if (!isset($reference_fields[$nodetype]) || $reset) {
    $reference_fields[$nodetype] = array();
    foreach (noderelationships_get_nodereferences($reset) as $field_name => $info) {
      if (isset($info[$nodetype])) {
        $reference_fields[$nodetype][$field_name] = $info[$nodetype];
      }
    }
    ksort($reference_fields[$nodetype]);
  }
  return $reference_fields[$nodetype];
}

/**
 * Get the list of referrer types for the given referred type.
 */
function noderelationships_get_referrer_types($referred_type, $reset = FALSE) {
  $relationships = noderelationships_get_relationships($reset);
  if (isset($relationships['referred'][$referred_type])) {
    return $relationships['referred'][$referred_type]['referrer_types'];
  }
  return array();
}

/**
 * Get the list of referred types for the given referrer type.
 */
function noderelationships_get_referred_types($referrer_type, $reset = FALSE) {
  $relationships = noderelationships_get_relationships($reset);
  if (isset($relationships['referrer'][$referrer_type])) {
    return $relationships['referrer'][$referrer_type]['referred_types'];
  }
  return array();
}

/**
 * Build the list of node relationships on the system.
 *
 * This function is basically the heart of this module. We build the
 * list of all relationed content types by parsing all nodereference
 * fields.
 *
 * Non-standard requirement:
 * The 'referenceable_types' attribute of nodereference fields must
 * be populated explicitly by the user, regardless of the use of views
 * for nodereference widgets.
 *
 * We cannot use content_types() because we could be invoked while
 * the data content_types() generates is being computed.
 * Note that noderelationships_content_extra_fields() is invoked
 * by content_types() to process hook_content_extra_fields() and
 * noderelationships_content_extra_fields() needs the information
 * we generate here.
 * So we read field settings directly from database using the same
 * method as content_field_instance_read(), just focussing on data
 * we're interested in.
 */
function noderelationships_get_relationships($reset = FALSE) {
  static $relationships;

  // Provide the data from static or cached storage when possible.
  if (!$reset) {
    // Do we have static data?
    if (isset($relationships)) {
      return $relationships;
    }
    // Do we have cached data?
    if ($cached = cache_get('noderelationships_relationships', content_cache_tablename())) {
      $relationships = $cached->data;
      return $relationships;
    }
  }

  // Build the relationships structure.
  $relationships = array(
    'referrer' => array(),
    'referred' => array(),
    'nodereferences' => noderelationships_get_nodereferences(TRUE),
  );

  foreach ($relationships['nodereferences'] as $field_name => $field_instances) {
    foreach ($field_instances as $referrer_type => $referenceable_types) {
      // Build the list of referrers by child type and field name.
      if (!isset($relationships['referrer'][$referrer_type])) {
        $relationships['referrer'][$referrer_type] = array('referred_types' => array(), 'fields' => array($field_name => array()));
      }
      foreach ($referenceable_types as $referred_type) {
        if (!isset($relationships['referrer'][$referrer_type]['referred_types'][$referred_type])) {
          $relationships['referrer'][$referrer_type]['referred_types'][$referred_type] = array();
        }
        $relationships['referrer'][$referrer_type]['referred_types'][$referred_type][] = $field_name;

        if (!isset($relationships['referrer'][$referrer_type]['fields'][$field_name])) {
          $relationships['referrer'][$referrer_type]['fields'][$field_name] = array();
        }
        $relationships['referrer'][$referrer_type]['fields'][$field_name][] = $referred_type;

        // Build the list of referred types by parent type and field name.
        if (!isset($relationships['referred'][$referred_type])) {
          $relationships['referred'][$referred_type] = array('referrer_types' => array(), 'fields' => array());
        }

        if (!isset($relationships['referred'][$referred_type]['referrer_types'][$referrer_type])) {
          $relationships['referred'][$referred_type]['referrer_types'][$referrer_type] = array();
        }
        $relationships['referred'][$referred_type]['referrer_types'][$referrer_type][] = $field_name;

        if (!isset($relationships['referred'][$referred_type]['fields'][$field_name])) {
          $relationships['referred'][$referred_type]['fields'][$field_name] = array();
        }
        $relationships['referred'][$referred_type]['fields'][$field_name][] = $referrer_type;
      }
    }
  }

  // Sort phase.
  foreach (array_keys($relationships['referrer']) as $referrer_type) {
    ksort($relationships['referrer'][$referrer_type]['fields']);
    ksort($relationships['referrer'][$referrer_type]['referred_types']);
  }
  ksort($relationships['referrer']);
  foreach (array_keys($relationships['referred']) as $referred_type) {
    ksort($relationships['referred'][$referred_type]['fields']);
    ksort($relationships['referred'][$referred_type]['referrer_types']);
  }
  ksort($relationships['referred']);

  // Cache a copy of the relationships structure.
  cache_set('noderelationships_relationships', $relationships, content_cache_tablename());

  return $relationships;
}

/**
 * Get a list of all 'nodereference' fields in the system.
 */
function noderelationships_get_nodereferences($reset = FALSE) {
  static $nodereferences;
  if (!isset($nodereferences) || $reset) {
    $nodereferences = array();

    $db_result = db_query("SELECT nfi.field_name, nfi.type_name, nf.global_settings
      FROM {". content_instance_tablename() ."} nfi
        INNER JOIN {". content_field_tablename() ."} nf ON nfi.field_name = nf.field_name
      WHERE nf.type = 'nodereference' AND nf.active = 1
      ORDER BY nfi.weight ASC, nfi.label ASC");

    while ($instance = db_fetch_object($db_result)) {
      $global_settings = (!empty($instance->global_settings) ? (array)unserialize($instance->global_settings) : array());
      $referenceable_types = (!empty($global_settings['referenceable_types']) ? array_keys(array_filter($global_settings['referenceable_types'])) : array());
      $field_name = $instance->field_name;
      $referrer_type = $instance->type_name;

      if (!isset($nodereferences[$field_name])) {
        $nodereferences[$field_name] = array();
      }
      $nodereferences[$field_name][$referrer_type] = $referenceable_types;
    }
  }
  return $nodereferences;
}

/**
 * Get a list of all 'noderelationships_backref' fields in the system.
 *
 * @param $nodetype
 *   If set, return information for this content type.
 * @param $reset
 *   If TRUE, discard static information and load information from database.
 */
function noderelationships_get_backreferences($nodetype = NULL, $reset = FALSE) {
  static $backreferences;
  if (!isset($backreferences) || $reset) {
    $backreferences = array();

    $db_result = db_query("SELECT nfi.field_name, nfi.type_name, nfi.label, nf.global_settings
      FROM {". content_instance_tablename() ."} nfi
        INNER JOIN {". content_field_tablename() ."} nf ON nfi.field_name = nf.field_name
      WHERE nf.type = 'noderelationships_backref' AND nf.active = 1
      ORDER BY nfi.weight ASC, nfi.label ASC");

    while ($instance = db_fetch_object($db_result)) {
      $global_settings = (!empty($instance->global_settings) ? (array)unserialize($instance->global_settings) : array());
      $field_name = $instance->field_name;
      $type_name = $instance->type_name;

      if (!isset($backreferences[$type_name])) {
        $backreferences[$type_name] = array();
      }
      $backreferences[$type_name][$field_name] = array(
        'widget_label' => $instance->label,
        'referrer_type' => $global_settings['referrer_type'],
      );
    }
  }
  if (isset($nodetype)) {
    return (isset($backreferences[$nodetype]) ? $backreferences[$nodetype] : array());
  }
  return $backreferences;
}

/**
 * Clear cached information about relationships.
 *
 * Note that we use the cache table provided by CCK itself, and CCK will
 * clear the whole cache (including our objects) when it needs to.
 *
 * We may still need to force a rebuild of the relationships structure,
 * therefore we can use this function, or load the data with the $reset
 * option enabled.
 *
 * @see noderelationships_get_relationships()
 */
function noderelationships_cache_clear() {
  cache_clear_all('noderelationships_relationships', content_cache_tablename());
}

/**
 * Clear content cache and (optionally) rebuild menus.
 */
function noderelationships_cache_clear_all($rebuild_menu = FALSE) {
  cache_clear_all('*', content_cache_tablename(), TRUE);
  if ($rebuild_menu) {
    menu_rebuild();
  }
}

/**
 * Synchronize back reference settings with back reference fields.
 */
function noderelationships_cck_backref_sync_fields($types_to_sync) {
  if (!is_array($types_to_sync)) {
    $types_to_sync = array($types_to_sync);
  }

  $rebuild = FALSE;

  foreach ($types_to_sync as $nodetype) {
    // Load back reference settings for fields region.
    $backref_settings = noderelationships_settings_load($nodetype, 'backref');
    $region_backrefs = (!empty($backref_settings['regions'][NODERELATIONSHIPS_BACKREF_REGION_FIELD]) ? $backref_settings['regions'][NODERELATIONSHIPS_BACKREF_REGION_FIELD] : array());

    // Get the list of back reference fields already defined for this content type.
    $backref_fields = noderelationships_get_backreferences($nodetype, TRUE);

    // Walk through all back reference settings to create new back reference fields.
    foreach ($region_backrefs as $relation_key => $relation_info) {
      list($referrer_type, $field_name) = explode(':', $relation_key);

      // Find backref to $nodetype from $field_name in $referrer_type.
      $backref_name = noderelationships_cck_backref_build_name($nodetype, $referrer_type, $field_name);
      if (isset($backref_fields[$backref_name])) {
        // So far so good, this back reference field already exists.
        unset($backref_fields[$backref_name]);
      }
      else {
        // This back reference field does not exist, so we should create one.

        $referrer_field = content_fields($field_name, $referrer_type);
        $field_label = t('Back references from @referrer-label in @referrer-type', array(
          '@referrer-label' => $referrer_field['widget']['label'],
          '@referrer-type' => noderelationships_get_localized_content_type_name($referrer_type),
        ));

        noderelationships_cck_backref_create(array(
          'type_name' => $nodetype,
          'field_name' => $backref_name,
          'label' => $field_label,
          'referrer_type' => $referrer_type,
          'referrer_field' => $field_name,
        ));
        $rebuild = TRUE;

        drupal_set_message(t('Back reference field created: %field', array('%field' => $field_label)));
      }
    }

    // Remove any back reference field that is not enabled in back reference settings.
    foreach ($backref_fields as $field_name => $field_info) {
      noderelationships_cck_backref_delete($nodetype, $field_name);
      $rebuild = TRUE;
      drupal_set_message(t('Back reference field removed: %field', array('%field' => $field_info['widget_label'])));
    }
  }

  // Clear caches and rebuild menu only if any field has been created or removed.
  if ($rebuild) {
    content_clear_type_cache(TRUE);
    menu_rebuild();
  }
}

/**
 * Build a unique back reference field name.
 */
function noderelationships_cck_backref_build_name($referred_type, $referrer_type, $field_name) {
  return substr('field_backref_'. md5($referred_type .':'. $referrer_type .':'. $field_name), 0, 32); // Coder note: This is just fine.
}

/**
 * Create a back reference field.
 *
 * @param $field_options
 *   An array with the following elements:
 *   - type_name       The content type where the is about to be created.
 *   - field_name      The name of the new field.
 *   - label           The label of the new field.
 *   - referrer_type   The referrer content type.
 *   - referrer_field  The name of the nodereference field in the referrer type.
 */
function noderelationships_cck_backref_create($field_options) {
  // Load the Create/Read/Update/Delete library for CCK objects.
  module_load_include('inc', 'content', 'includes/content.crud');

  // Build the field structure.
  $field = array(
    'label' => $field_options['label'],
    'field_name' => $field_options['field_name'],
    'type_name' => $field_options['type_name'],
    'type' => 'noderelationships_backref',
    'widget_type' => 'noderelationships_backref',
    'module' => 'noderelationships',
    'widget_module' => 'noderelationships',
    'required' => 0,
    'multiple' => 0,
    'description' => '',
    'default_value' => NULL,
    'default_value_php' => NULL,
    'default_value_widget' => NULL,
    'columns' => array(),
    'weight' => noderelationships_get_element_weight($field_options['type_name']),
    // Global field settings.
    'referrer_type' => $field_options['referrer_type'],
    'referrer_field' => $field_options['referrer_field'],
  );

  // Create the field.
  content_field_instance_create($field, FALSE);
}

/**
 * Delete a back reference field.
 */
function noderelationships_cck_backref_delete($type_name, $field_name) {
  // Load the Create/Read/Update/Delete library for CCK objects.
  module_load_include('inc', 'content', 'includes/content.crud');

  // Make sure the field exists and it belongs to us.
  $read_params = array(
    'field_name' => $field_name,
    'type_name' => $type_name,
    'widget_module' => 'noderelationships',
  );
  $instances = content_field_instance_read($read_params, TRUE);
  if (!empty($instances)) {
    content_field_instance_delete($field_name, $type_name, FALSE);
  }
}

/**
 * Obtain the default weight for a new CCK "Manage fields" element.
 *
 * See content_field_overview_form() in cck/includes/content.admin.inc.
 *
 * @param $nodetype
 *   The node type.
 *
 * @return
 *   An integer with the element weight.
 */
function noderelationships_get_element_weight($nodetype) {
  $weights = array();
  if (module_exists('fieldgroup')) {
    foreach (fieldgroup_groups($nodetype) as $group) {
      $weights[] = $group['weight'];
    }
  }
  $content_type = content_types($nodetype);
  foreach ($content_type['fields'] as $field) {
    $weights[] = $field['widget']['weight'];
  }
  return !empty($weights) ? (max($weights) + 1) : 1;
}

/**
 * Allow external modules alter noderelationships labels.
 *
 * Hook prototype:
 *   @code
 *     hook_noderelationships_label_alter(&$label, $label_id, $context, $arguments)
 *   @endcode
 *
 * @param $label
 *   The default value for the label.
 * @param $label_id
 *   An string that identifies whice label it is.
 * @param $context
 *   An array with information about the context.
 * @param $arguments
 *   An array with arguments that the label depends on.
 */
function noderelationships_alter_label(&$label, $label_id, $context, $arguments) {
  drupal_alter('noderelationships_label', $label, $label_id, $context, $arguments);
}

/**
 * Get the list of views in the system.
 */
function noderelationships_get_views($relation_type) {
  $views = array();

  if ($relation_type == 'backref') {
    $views[''] = '['. t('Default view') .']';
  }

  foreach (views_get_all_views() as $view) {
    // Ignore disabled views.
    if ($view->disabled) {
      continue;
    }
    // Ignore views that are not based on node table.
    if ($view->base_table != 'node') {
      continue;
    }
    // Filter views depending on relation type.
    if ($relation_type == 'backref' && ($view->tag != NODERELATIONSHIPS_BACKREF_VIEW_TAG || $view->name == NODERELATIONSHIPS_BACKREF_VIEW_NAME)) {
      continue;
    }
    elseif ($relation_type == 'noderef' && $view->tag != NODERELATIONSHIPS_NODEREF_VIEW_TAG) {
      continue;
    }
    // Let's look at all displays defined for the view.
    $view_displays = array();
    foreach ($view->display as $display_id => $display_info) {
      if ($relation_type == 'backref') {
        // For backref views we are interested only in default display.
        if ($display_info->display_plugin != 'default') {
          continue;
        }
      }
      elseif ($relation_type == 'noderef') {
        // For noderef views we are interested only in page displays.
        if ($display_info->display_plugin != 'page') {
          continue;
        }
        // For the moment, we can only guarantee support for the following
        // style pulgins:
        $supported_style_plugins = array('table', 'grid', 'fluid_grid');
        $view->set_display($display_id);
        if (!in_array($view->display_handler->get_option('style_plugin'), $supported_style_plugins)) {
          continue;
        }
      }
      $view_displays[$display_id] = $display_info->display_title;
    }
    if (!empty($view_displays)) {
      $view_displays_count = count($view_displays);
      foreach ($view_displays as $display_id => $display_title) {
        $view_label = $view_name = ($view->name == NODERELATIONSHIPS_NODEREF_VIEW_NAME ? t('Default view') : $view->name);
        if ($view_displays_count > 1) {
          $view_label .= ' ('. $display_title .')';
        }
        if ($view->name == NODERELATIONSHIPS_NODEREF_VIEW_NAME) {
          $view_label = '['. $view_label .']';
        }
        if ($view_displays_count > 1) {
          if (!isset($views[$view_name])) {
            $views[$view_name] = array();
          }
          $views[$view_name][$view->name .':'. $display_id] = $display_title;
        }
        else {
          $views[$view->name .':'. $display_id] = $view_label;
        }
      }
    }
  }

  // Sort views alphabetically.
  uksort($views, '_noderelationships_sort_strncmp');

  return $views;
}

/**
 * Case-insensitive string compare function.
 */
function _noderelationships_sort_strncmp($a, $b) {
  $a = drupal_strtolower($a);
  $b = drupal_strtolower($b);
  return ($a == $b ? 0 : ($a < $b ? -1 : 1));
}

/**
 * Abort a custom view with page not found.
 */
function noderelationships_abort_view(&$view, &$view_args) {
  $view_args = array(NULL);
  $view->display_handler->override_option('arguments', array(
    'nid' => array(
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'default_action' => 'not found',
    )
  ));
}

/**
 * Customize node title and body labels for fields in the main table.
 */
function noderelationships_customize_views_node_fields(&$view, $display_id, $type_info) {
  if (($item = $view->get_item($display_id, 'field', 'title'))) {
    if ($item['table'] == 'node' && $item['relationship'] == 'none') {
      $item['label'] = $type_info->title_label;
      $view->set_item($display_id, 'field', 'title', $item);
    }
  }
  if ($referrer_type->has_body && ($item = $view->get_item($display_id, 'field', 'body'))) {
    if ($item['table'] == 'node' && $item['relationship'] == 'none') {
      $item['label'] = $type_info->body_label;
      $view->set_item($display_id, 'field', 'body', $item);
    }
  }
}

/**
 * Get overrides for back reference views.
 *
 * @param $view
 * @param $referrer_field
 *
 * @see noderelationships_customize_backref_view()
 * @see content_views_data()
 */
function noderelationships_get_backref_view_overrides($view, $referrer_field) {
  // Obtain database and views information about the field.
  if (!function_exists('content_views_field_views_data')) {
    module_load_include('inc', 'content', 'includes/views/content.views');
  }
  $module = $referrer_field['module'];
  $views_data = module_invoke($module, 'field_settings', 'views data', $referrer_field);
  $table_alias = content_views_tablename($referrer_field);
  $db_info = content_database_info($referrer_field);
  $nodereference_column = $db_info['columns']['nid']['column'];
  $view_overrides = array();

  // Build relationship overrides.
  $view_overrides['relationships'] = array(
    $nodereference_column => array(
      'label' => $views_data[$table_alias][$nodereference_column]['relationship']['label'],
      'id' => $nodereference_column,
      'table' => $table_alias,
      'field' => $nodereference_column,
      'required' => 1,
      'delta' => -1,
      'relationship' => 'none',
    ),
  );
  $existing_view_relationships = $view->display_handler->get_option('relationships');
  if (!empty($existing_view_relationships)) {
    $view_overrides['relationships'] = array_merge($view_overrides['relationships'], $existing_view_relationships);
  }

  // Build argument overrides.
  $common_argument_options = array(
    'default_action' => 'empty',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'relationship' => 'none',
  );
  $view_overrides['arguments'] = array(
    $nodereference_column => array_merge($common_argument_options, array(
      'id' => $nodereference_column,
      'table' => $table_alias,
      'field' => $nodereference_column,
    )),
    'type' => array_merge($common_argument_options, array(
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
    )),
  );

  return $view_overrides;
}

/**
 * Apply custom configuration to the given back reference view.
 */
function noderelationships_customize_backref_view(&$view, $display_id, &$view_args, $reset_current_display = TRUE) {
  // Nothing to do if the view has already been processed.
  if (!empty($view->noderelationships_processed)) {
    return;
  }

  // Mark the view as being already processed.
  $view->noderelationships_processed = TRUE;

  // If the view is not executed with the expected number of arguments, then
  // we should abort this view.
  if (count($view_args) < 3 || !is_numeric($view_args[0])) {
    noderelationships_abort_view($view, $view_args);
    return FALSE;
  }

  // We expect to see the following arguments:
  // 0 - nid of the referred node.
  // 1 - type of the referring node.
  // 2 - name of the nodereference field in the referring type.
  list($referred_nid, $type_name, $field_name) = $view_args;

  // Load the referred node.
  if (!($referred_node = node_load($referred_nid))) {
    noderelationships_abort_view($view, $view_args);
    return FALSE;
  }

  // Load settings of the referring field.
  $referrer_field = content_fields($field_name, $type_name);
  if (empty($referrer_field)) {
    drupal_set_message(t('Could not load field information to properly customize the view %view-name.', array('%view-name' => $view->name)), 'error');
    watchdog('noderelationships', 'Could not load field information to properly customize the view %view-name (field: @field-name, type: @type-name).', array('%view-name' => $view->name, '@field-name' => $field_name, '@type-name' => $type_name), WATCHDOG_ERROR);
    noderelationships_abort_view($view, $view_args);
    return FALSE;
  }

  drupal_add_css(drupal_get_path('module', 'noderelationships') .'/css/noderelationships.backref_views.css');

  // Load the related node types.
  $referrer_type = noderelationships_get_localized_content_type($type_name);
  $referred_type = noderelationships_get_localized_content_type($referred_node->type);

  // Get view overrides for the given nodereference field.
  $view_overrides = noderelationships_get_backref_view_overrides($view, $referrer_field);

  // Customization related to the page display.
  if ($view->display_handler->display->display_plugin == 'page') {
    $view_title = $view->get_title();

    $context = array(
      'referred_node' => $referred_node,
      'referred_type' => $referred_type,
      'referrer_type' => $referrer_type,
      'referrer_field' => $referrer_field,
      'field_name' => $field_name,
    );

    // Build custom title, if nothing exists.
    if (empty($view_title)) {
      // Prepare default view title.
      $arguments = array(
        '%node-title' => $referred_node->title,
        '%referrer-type-name' => $referrer_type->name,
        '%referred-type-name' => $referred_type->name,
        '%referrer-label' => $referrer_field['widget']['label'],
      );
      $view_title = t('Back references from %referrer-label in %referrer-type-name for %referred-type-name: %node-title', $arguments);

      // Allow external modules alter the view title.
      noderelationships_alter_label($view_title, 'backref_referred_page_title', $context, $arguments);

      $view_overrides['title'] = $view_title;
    }

    // Build custom breadcrumb.
    $view->noderelationships_breadcrumb = array(
      l(t('Home'), NULL),
      l($referred_node->title, 'node/'. $referred_node->nid),
    );

    // Allow external modules alter the breadcrumb.
    drupal_alter('noderelationships_breadcrumb', $view->noderelationships_breadcrumb, $view, $view_args, $context);
  }

  // Reset the current page display so that the changes take effect.
  // This is necessary for AJAX requests and page displays managed by
  // the views menu callback.
  if ($reset_current_display && isset($view->current_display)) {
    unset($view->current_display);
  }

  // Activate the specified display.
  $view->set_display($display_id);

  // Allow external modules alter the view.
  drupal_alter('noderelationships_view', $view_overrides, $view, $display_id, $view_args);

  // Apply dynamic customization to the view display.
  foreach ($view_overrides as $option => $definition) {
    $view->display_handler->override_option($option, $definition);
  }

  // If this is the default backref view AND it has not been overridden, then
  // we want to customize the node title and body labels for fields in the main
  // table to match those defined in the content type settings page.
  if ($view->name == NODERELATIONSHIPS_BACKREF_VIEW_NAME && $view->type == t('Default')) {
    noderelationships_customize_views_node_fields($view, $display_id, $referrer_type);
  }

  return TRUE;
}

/**
 * Get overrides for search and reference views.
 *
 * @param $view
 * @param $referrer_field
 *
 * @see noderelationships_customize_noderef_view()
 */
function noderelationships_get_noderef_view_overrides($view, $referrer_field) {
  $view_overrides = array();

  // Customize view fields.
  $new_view_fields = array();
  $new_view_fields['noderelationships_nid'] = array(
    'id' => 'noderelationships_nid',
    'table' => 'node',
    'field' => 'nid',
    'label' => 'X-NodeRelationships-Nid',
    'relationship' => 'none',
  );
  $new_view_fields['noderelationships_title'] = array(
    'id' => 'noderelationships_title',
    'table' => 'node',
    'field' => 'title',
    'label' => 'X-NodeRelationships-Title',
    'relationship' => 'none',
  );
  $existing_view_fields = $view->display_handler->get_option('fields');
  if (!empty($existing_view_fields)) {
    $new_view_fields = array_merge($new_view_fields, $existing_view_fields);
  }
  $view_overrides['fields'] = $new_view_fields;

  // Customize view filters.
  $referenceable_types = (isset($referrer_field['referenceable_types']) ? array_filter($referrer_field['referenceable_types']) : array());
  if (!empty($referenceable_types)) {
    $new_view_filters = $view->display_handler->get_option('filters');
    if (empty($new_view_filters)) {
      $new_view_filters = array();
    }
    // Make sure we have a filter by content type.
    if (isset($new_view_filters['type'])) {
      $new_view_filters['type']['value'] = $referenceable_types;
      // If this is the default noderef view AND it has not been overridden,
      // then we want to control if the filter is exposed or not depending
      // on number of referenceable types.
      if ($view->name == NODERELATIONSHIPS_NODEREF_VIEW_NAME && $view->type == t('Default')) {
        $new_view_filters['type']['exposed'] = (count($referenceable_types) > 1 ? TRUE : FALSE);
      }
    }
    else {
      $new_view_filters['type'] = array(
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'operator' => 'in',
        'value' => $referenceable_types,
        'group' => '0',
        'exposed' => FALSE,
        'relationship' => 'none',
      );
      if (count($referenceable_types) > 1) {
        $new_view_filters['type']['exposed'] = TRUE;
        $new_view_filters['type']['expose'] = array(
          'use_operator' => 0,
          'operator' => 'type_op',
          'identifier' => 'type',
          'label' => 'Type',
          'optional' => 0,
          'single' => 0,
          'remember' => 1,
          'reduce' => 1,
        );
      }
    }
    $view_overrides['filters'] = $new_view_filters;
  }

  // Dynamic customization dependent on active style plugin.
  $style_plugin = $view->display_handler->get_option('style_plugin');
  if ($style_plugin == 'table') {
    $style_options = $view->display_handler->get_option('style_options');
    $new_style_columns = array(
      'noderelationships_nid' => 'noderelationships_nid',
      'noderelationships_title' => 'noderelationships_title',
    );
    if (!empty($style_options['columns'])) {
      $new_style_columns = array_merge($new_style_columns, $style_options['columns']);
    }
    $style_options['columns'] = $new_style_columns;

    $new_style_info = array(
      'noderelationships_nid' => array('sortable' => 0, 'separator' => ':'),
      'noderelationships_title' => array('sortable' => 0, 'separator' => ''),
    );
    if (!empty($style_options['info'])) {
      $new_style_info = array_merge($new_style_info, $style_options['info']);
    }
    $style_options['info'] = $new_style_info;

    $view_overrides['style_options'] = $style_options;
  }

  // Make sure Views AJAX is enabled, also disable the Views "more" button.
  $view_overrides['use_ajax'] = TRUE;
  $view_overrides['use_more'] = 0;

  // Customize the view path.
  $view_overrides['path'] = 'noderelationships/search/%/%';

  return $view_overrides;
}

/**
 * Apply custom configuration to the given search and reference view.
 */
function noderelationships_customize_noderef_view(&$view, $display_id, &$view_args, $reset_current_display = TRUE) {
  // Nothing to do if the view has already been processed.
  if (!empty($view->noderelationships_processed)) {
    return;
  }

  // Mark the view as being already processed.
  $view->noderelationships_processed = TRUE;

  // If the view is not executed with the expected number of arguments, then
  // we should abort this view.
  if (count($view_args) < 2) {
    noderelationships_abort_view($view, $view_args);
    return FALSE;
  }

  // We expect to see the following arguments:
  // 0 - type of the referrer node.
  // 1 - name of the nodereference field in the referrer type.
  list($type_name, $field_name) = $view_args;

  // Load settings of the referrer field.
  $referrer_field = content_fields($field_name, $type_name);
  if (empty($referrer_field)) {
    drupal_set_message(t('Could not load field information to properly customize the view %view-name.', array('%view-name' => $view->name)), 'error');
    watchdog('noderelationships', 'Could not load field information to properly customize the view %view-name (field: @field-name, type: @type-name).', array('%view-name' => $view->name, '@field-name' => $field_name, '@type-name' => $type_name), WATCHDOG_ERROR);
    noderelationships_abort_view($view, $view_args);
    return FALSE;
  }

  // Load the related node type.
  $referrer_type = noderelationships_get_localized_content_type($type_name);

  // Reset the current page display so that the changes take effect.
  // This is necessary for AJAX requests and page displays.
  if ($reset_current_display && isset($view->current_display)) {
    unset($view->current_display);
  }

  // Activate the specified display.
  $view->set_display($display_id);

  // Get view overrides for the given nodereference field.
  $view_overrides = noderelationships_get_noderef_view_overrides($view, $referrer_field);

  // Build custom title, if nothing exists.
  $view_title = $view->get_title();

  if (empty($view_title)) {
    // Prepare default view title.
    $arguments = array(
      '%referrer-label' => $referrer_field['widget']['label'],
    );
    $view_title = t('Search and reference %referrer-label', $arguments);

    // Allow external modules alter the view title.
    $context = array(
      'referrer_type' => $referrer_type,
      'referrer_field' => $referrer_field,
      'field_name' => $field_name,
    );
    noderelationships_alter_label($view_title, 'noderef_search_page_title', $context, $arguments);

    $view_overrides['title'] = $view_title;
  }

  // Allow external modules alter the view.
  drupal_alter('noderelationships_view', $view_overrides, $view, $display_id, $view_args);

  // Apply dynamic customization to the view display.
  foreach ($view_overrides as $option => $definition) {
    $view->display_handler->override_option($option, $definition);
  }

  return TRUE;
}

/**
 * Get the localized version of the given content type.
 */
function noderelationships_get_localized_content_type($nodetype) {
  $type = node_get_types('type', $nodetype);
  if ($type && ($i18nstrings = noderelationships_get_i18nstrings())) {
    $type->name = $i18nstrings("nodetype:type:$nodetype:name", $type->name);
    if (!empty($type->description)) {
      $type->description = $i18nstrings("nodetype:type:$nodetype:description", $type->description);
    }
    if (!empty($type->title_label)) {
      $type->title_label = $i18nstrings("nodetype:type:$nodetype:title", $type->title_label);
    }
    if (!empty($type->body_label)) {
      $type->body_label = $i18nstrings("nodetype:type:$nodetype:body", $type->body_label);
    }
    if (!empty($type->help)) {
      $type->help = $i18nstrings("nodetype:type:$nodetype:help", $type->help);
    }
  }
  return $type;
}

/**
 * Get the localized name of the given content type.
 */
function noderelationships_get_localized_content_type_name($nodetype) {
  $name = node_get_types('name', $nodetype);
  if ($i18nstrings = noderelationships_get_i18nstrings()) {
    $name = $i18nstrings("nodetype:type:$nodetype:name", $name);
  }
  return $name;
}

/**
 * Get the name of the i18nstrings() function, it was tt().
 *
 * @todo Remove when this issue is stable in i18n module.
 * @link http://drupal.org/node/358839 Fatal error: Call to undefined function tt() @endlink
 */
function noderelationships_get_i18nstrings() {
  if (function_exists('i18nstrings')) {
    return 'i18nstrings';
  }
  if (function_exists('tt')) {
    return 'tt';
  }
  return FALSE;
}

/**
 * Get the localized language name.
 */
function noderelationships_get_localized_language_list() {
  if (module_exists('locale')) {
    return locale_language_list();
  }
  $language_list = array();
  foreach (language_list() as $langcode => $language) {
    $language_list[$langcode] = $language->name;
  }
  return $language_list;
}

/**
 * Check if translation is enabled for the given type and the current user
 * is allowed to create translations.
 */
function noderelationships_translation_supported_type($nodetype) {
  if (module_exists('translation') && user_access('translate content') && translation_supported_type($nodetype)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Compose a query string to append to node reference extra page requests.
 *
 * @return
 *   A query string that consists of all components of the current page request.
 */
function noderelationships_querystring() {
  return drupal_query_string_encode($_GET, array_merge(array('q', 'destination', 'pass', 'translation', 'language'), array_keys($_COOKIE)));
}
