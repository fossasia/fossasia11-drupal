<?php
// $Id: noderelationships.admin.inc,v 1.1.2.20 2010/05/16 18:26:01 markuspetrux Exp $

/**
 * @file
 * Implementation of the administration pages of the module.
 */

/**
 * Menu callback; relationships administration.
 */
function noderelationships_admin_page($op, $type_name = NULL) {
  // Include common module functions.
  module_load_include('inc', 'noderelationships');

  if (isset($type_name) && ($type = noderelationships_get_localized_content_type($type_name))) {
    if (empty($op) || $op == 'erd') {
      return noderelationships_erd_diagram($type);
    }
    if ($op == 'noderef') {
      return drupal_get_form('noderelationships_admin_settings_noderef', $type);
    }
    if ($op == 'backref') {
      return drupal_get_form('noderelationships_admin_settings_backref', $type);
    }
  }

  // Fallback to page not found.
  drupal_not_found();
}

/**
 * Build the Entity Relationship Diagram (ERD).
 */
function noderelationships_erd_diagram($type) {
  // Load information about relationships.
  $relationships = noderelationships_get_relationships();

  // If this type does not have relations, then we're done here.
  if (!isset($relationships['referrer'][$type->type]) && !isset($relationships['referred'][$type->type])) {
    drupal_set_message(t('Could not find relations for this content type.'), 'warning');
    return '';
  }

  $referrer_types = isset($relationships['referred'][$type->type]) ? $relationships['referred'][$type->type]['referrer_types'] : array();
  $referred_types = isset($relationships['referrer'][$type->type]) ? $relationships['referrer'][$type->type]['referred_types'] : array();
  $nodereference_fields = isset($relationships['referrer'][$type->type]['fields']) ? $relationships['referrer'][$type->type]['fields'] : array();

  $destination = drupal_get_destination();
  $content_types = content_types();
  $type_url_str = str_replace('_', '-', $type->type);
  $current_self_referred = FALSE;
  $referrer_entities = array();
  $referred_entities = array();
  $current_entity_fields = array();

  foreach ($referrer_types as $referrer_type => $field_names) {
    if ($referrer_type == $type->type) {
      $current_self_referred = TRUE;
      continue;
    }
    $referrer_url_str = str_replace('_', '-', $referrer_type);
    $type_label = noderelationships_get_localized_content_type_name($referrer_type);
    $type_label = ($type->type == $referrer_type ? check_plain($type_label) : l($type_label, 'admin/content/node-type/'. $referrer_url_str .'/relationships'));
    $type_items = array();
    foreach ($field_names as $field_name) {
      $type_items[] = array(
        'data' => l($content_types[$referrer_type]['fields'][$field_name]['widget']['label'], 'admin/content/node-type/'. $referrer_url_str .'/fields/'. $field_name, array('query' => $destination)),
        'attributes' => array('class' => 'noderelationships-erd-noderef'),
      );
    }
    $referrer_entities[] = array('label' => $type_label, 'items' => $type_items);
  }

  if (!empty($referred_types[$type->type])) {
    foreach ($content_types[$type->type]['fields'] as $field_name => $field) {
      if (in_array($field_name, $referred_types[$type->type])) {
        unset($nodereference_fields[$field_name]);
        $current_entity_fields[] = array(
          'data' => l($field['widget']['label'], 'admin/content/node-type/'. $type_url_str .'/fields/'. $field_name, array('query' => $destination)),
          'attributes' => array('class' => 'noderelationships-erd-noderef'),
        );
      }
    }
  }

  foreach ($referred_types as $referred_type => $field_names) {
    if ($referred_type == $type->type) {
      continue;
    }
    $referred_url_str = str_replace('_', '-', $referred_type);
    $type_label = noderelationships_get_localized_content_type_name($referred_type);
    $type_label = ($type->type == $referred_type ? check_plain($type_label) : l($type_label, 'admin/content/node-type/'. $referred_url_str .'/relationships'));
    $type_items = array();
    foreach ($field_names as $field_name) {
      unset($nodereference_fields[$field_name]);
      $type_items[] = array(
        'data' => l($content_types[$type->type]['fields'][$field_name]['widget']['label'], 'admin/content/node-type/'. $type_url_str .'/fields/'. $field_name, array('query' => $destination)),
        'attributes' => array('class' => 'noderelationships-erd-noderef'),
      );
    }
    $referred_entities[] = array('label' => $type_label, 'items' => $type_items);
  }

  foreach ($nodereference_fields as $field_name => $types) {
    $field = $content_types[$type->type]['fields'][$field_name];
    $current_entity_fields[] = array(
      'data' => l($field['widget']['label'], 'admin/content/node-type/'. $type_url_str .'/fields/'. $field_name, array('query' => $destination)),
      'attributes' => array('class' => 'noderelationships-erd-noderef-any'),
    );
  }

  $diagram = '';
  if (!empty($referrer_entities)) {
    $diagram .= theme('noderelationships_erd_group', 'referrer', $referrer_entities);
    $diagram .= theme('noderelationships_erd_relation', t('Referred from'));
  }
  $current_label = array('data' => check_plain($type->name));
  if ($current_self_referred) {
    $current_label['attributes'] = array('class' => 'noderelationships-erd-self');
  }
  $diagram .= theme('noderelationships_erd_current', $current_label, $current_entity_fields);
  if (!empty($referred_entities)) {
    $diagram .= theme('noderelationships_erd_relation', t('Refers to'));
    $diagram .= theme('noderelationships_erd_group', 'referred', $referred_entities);
  }

  $admin_help_text = t('This panel displays an Entity Relationship Diagram (ERD) relative to the content type %type.', array(
    '%type' => $type->name,
  ));
  $output = theme('noderelationships_admin_help', $admin_help_text);
  $output .= theme('noderelationships_erd_diagram', $diagram);
  return $output;
}

/**
 * Render the Entity Relationship Diagram (ERD).
 */
function theme_noderelationships_erd_diagram($diagram) {
  $module_path = drupal_get_path('module', 'noderelationships');
  drupal_add_css($module_path .'/css/noderelationships.admin_erd.css');
  drupal_add_js($module_path .'/js/jquery.corner.js');
  drupal_add_js($module_path .'/js/admin.erd.js');

  $output = '<div class="noderelationships-erd">'."\n";
  $output .= $diagram;
  $output .= '</div>'."\n";
  return $output;
}

/**
 * Render a group of entities in the ERD.
 */
function theme_noderelationships_erd_group($group_type, $entities) {
  $output = '<div class="noderelationships-erd-group noderelationships-erd-'. $group_type .'">';
  foreach ($entities as $entity) {
    $output .= theme('noderelationships_erd_entity', $entity['label'], $entity['items']);
  }
  $output .= '</div>'."\n";
  return $output;
}

/**
 * Render the current entity in the ERD.
 */
function theme_noderelationships_erd_current($label, $items = NULL) {
  $output = '<div class="noderelationships-erd-current">';
  $output .= theme('noderelationships_erd_entity', $label, $items);
  $output .= '</div>'."\n";
  return $output;
}

/**
 * Render an entity in the ERD.
 */
function theme_noderelationships_erd_entity($label, $items = NULL) {
  $output = '<div class="noderelationships-erd-entity">';
  if (!is_array($label)) {
    $label = array('data' => $label);
  }
  $output .= '<div class="noderelationships-erd-entity-caption">';
  $output .= '<h2'. (isset($label['attributes']) ? drupal_attributes($label['attributes']) : '') .'>'. $label['data'] .'</h2>';
  $output .= '</div>';
  $output .= '<div class="noderelationships-erd-fields">';
  if (empty($items)) {
    $output .= '&nbsp;';
  }
  else {
    foreach ($items as $key => $item) {
      if (!is_array($item)) {
        $item = array('data' => $item);
      }
      $items[$key] = '<li'. (isset($item['attributes']) ? drupal_attributes($item['attributes']) : '') .'>'. $item['data'] .'</li>';
    }
    $output .= '<ul>'. implode('', $items) .'</ul>';
  }
  $output .= '</div>';
  $output .= '</div>'."\n";
  return $output;
}

/**
 * Render a relation in the ERD.
 */
function theme_noderelationships_erd_relation($label) {
  $output = '<div class="noderelationships-erd-relation"><div class="noderelationships-erd-relation-box">';
  $output .= '<div class="noderelationships-erd-relation-caption"><h2>'. $label .'</h2></div>';
  $output .= '</div></div>'."\n";
  return $output;
}

/**
 * Node reference extras form.
 */
function noderelationships_admin_settings_noderef(&$form_state, $type) {
  $form = array();
  $reference_fields = noderelationships_get_reference_fields($type->type);

  // If this type does not have relations, then we're done here.
  if (empty($reference_fields)) {
    drupal_set_message(t('Could not find node reference fields referring to other content types from %type.', array('%type' => $type->name)), 'warning');
    return $form;
  }

  $admin_help_text = t('This panel allows you to configure extra settings for <em>Node Reference</em> fields defined in the content type %type-name.
You can modify and/or clone (recommended) the default view for the <em>Search and reference</em> feature should you need to add more fields, filters or change any other option to suit your needs.
These extra options are available only to node reference fields configured to use the autocomplete widget.', array(
    '%type-name' => $type->name,
  ));
  $form['help'] = array(
    '#type' => 'markup',
    '#value' => theme('noderelationships_admin_help', $admin_help_text),
  );

  // Obtain extra nodereference settings for this type.
  $noderef_settings = noderelationships_settings_load($type->type, 'noderef');

  $options_search_and_reference_view = array('' => '<'. t('none') .'>') + noderelationships_get_views('noderef');

  $form['search_and_reference_view'] = array(
    '#tree' => TRUE,
  );
  $form['view_in_new_window'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $noderef_settings['view_in_new_window'],
    '#options' => array(),
  );
  $form['edit_reference'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $noderef_settings['edit_reference'],
    '#options' => array(),
  );
  $form['create_and_reference'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $noderef_settings['create_and_reference'],
    '#options' => array(),
  );
  if (module_exists('translation')) {
    $form['translate_and_reference'] = array(
      '#type' => 'checkboxes',
      '#default_value' => $noderef_settings['translate_and_reference'],
      '#options' => array(),
    );
  }
  $form['#noderelationships-type'] = $type->type;

  $form['#noderelationships-relations'] = array();
  foreach ($reference_fields as $field_name => $referenceable_types) {
    $field = content_fields($field_name, $type->type);

    $form['#noderelationships-relations'][$field_name] = array(
      'referenceable_types' => $referenceable_types,
      'field_name' => $field_name,
      'field' => $field,
    );

    if ($field['widget']['type'] == 'nodereference_autocomplete') {
      $form['view_in_new_window']['#options'][$field_name] = t('View reference in new window');
      $form['edit_reference']['#options'][$field_name] = t('Edit reference');
      $form['create_and_reference']['#options'][$field_name] = t('Create and reference');
      if (isset($form['translate_and_reference'])) {
        $form['translate_and_reference']['#options'][$field_name] = t('Translate and reference');
      }
      $form['search_and_reference_view'][$field_name] = array(
        '#type' => 'select',
        '#title' => t('Search and reference view'),
        '#default_value' => (!empty($noderef_settings['search_and_reference_view'][$field_name]) ? $noderef_settings['search_and_reference_view'][$field_name] : ''),
        '#options' => $options_search_and_reference_view,
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Submit handler for the node reference extras form.
 */
function noderelationships_admin_settings_noderef_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $settings = noderelationships_settings_load($form['#noderelationships-type']);

  // Move form values to settings array.
  $settings['noderef'] = array(
    'search_and_reference_view' => array_filter($form_values['search_and_reference_view']),
    'view_in_new_window' => array_filter($form_values['view_in_new_window']),
    'edit_reference' => array_filter($form_values['edit_reference']),
    'create_and_reference' => array_filter($form_values['create_and_reference']),
    'translate_and_reference' => (isset($form_values['translate_and_reference']) ? array_filter($form_values['translate_and_reference']) : array()),
  );

  // Update settings and clear caches if changes exist.
  if (noderelationships_settings_save($form['#noderelationships-type'], $settings)) {
    noderelationships_cache_clear_all();
  }
  drupal_set_message(t('The settings have been saved.'));
}

/**
 * Render the node reference extras form.
 */
function theme_noderelationships_admin_settings_noderef($form) {
  if (!isset($form['help'])) {
    return drupal_render($form);
  }
  $output = drupal_render($form['help']);

  $module_path = drupal_get_path('module', 'noderelationships');
  drupal_add_css($module_path .'/css/noderelationships.admin_settings.css');
  drupal_add_js($module_path .'/js/admin.view_settings.js');
  $js_settings = array(
    'mode' => 'noderef',
    'viewSettingsUrl' => url('admin/build/views/edit'),
    'defaultViewName' => NODERELATIONSHIPS_BACKREF_VIEW_NAME,
  );
  drupal_add_js(array('nodeRelationships' => $js_settings), 'setting');

  $destination = drupal_get_destination();
  $headers = array(
    array('data' => t('Reference field')),
    array('data' => t('Referred types')),
    array('data' => t('Required'), 'class' => 'noderelationships-cell-center'),
    array('data' => t('Cardinality'), 'class' => 'noderelationships-cell-center'),
    array('data' => t('Extra node reference widget features')),
  );
  if (isset($form['translate_and_reference'])) {
    // Display a warning if i18n module is not enabled, or not configured
    // for the "Translate and reference" feature to work properly.
    if (!module_exists('i18n')) {
      drupal_set_message(t('It is recommended to install the <a href="@i18n-url">Internationalization</a> module in order to take full advantage of the &quot;Translate and reference&quot; feature.', array('@i18n-url' => 'http://drupal.org/project/i18n')), 'warning');
    }
    elseif (!variable_get('i18n_translation_switch', 0)) {
      drupal_set_message(t('It is recommended to enable the &quot;Switch interface for translating&quot; option in the <a href="@i18n-admin">Multilingual system settings</a> page for the &quot;Translate and reference&quot; feature to work properly.', array('@i18n-admin' => url('admin/settings/language/i18n'))), 'warning');
    }
  }
  $rows = array();

  foreach ($form['#noderelationships-relations'] as $field_name => $relation_info) {
    $referenceable_types = $relation_info['referenceable_types'];
    $field = $relation_info['field'];
    $cells = array();

    // Node reference field.
    $type_url_str = str_replace('_', '-', $field['type_name']);
    $field_cell = l($field['widget']['label'], 'admin/content/node-type/'. $type_url_str .'/fields/'. $field_name, array('query' => $destination));
    $field_class = (in_array($form['#noderelationships-type'], $referenceable_types) ? 'noderelationships-cell-noderef-self' : (empty($referenceable_types) ? 'noderelationships-cell-noderef-any' : 'noderelationships-cell-noderef'));
    $cells[] = array(
      'data' => '<div class="'. $field_class .'">'. $field_cell .'</div>',
    );

    // Related types.
    $items = array();
    if (empty($referenceable_types)) {
      $type_cell = t('Any');
      $type_class = 'noderelationships-cell-referred';
      $items[] = '<div class="'. $type_class .'">'. $type_cell .'</div>';
    }
    else {
      foreach ($referenceable_types as $referred_type) {
        $type_url_str = str_replace('_', '-', $referred_type);
        $type_cell = noderelationships_get_localized_content_type_name($referred_type);
        if ($form['#noderelationships-type'] == $referred_type) {
          $type_cell = check_plain($type_cell);
          $type_class = 'noderelationships-cell-current-self';
        }
        else {
          $type_cell = l($type_cell, 'admin/content/node-type/'. $type_url_str .'/relationships/backref');
          $type_class = 'noderelationships-cell-referred';
        }
        $items[] = '<div class="'. $type_class .'">'. $type_cell .'</div>';
      }
    }
    $cells[] = array(
      'data' => implode('', $items),
    );

    // Required.
    $cells[] = array(
      'data' => ($field['required'] ? t('Yes') : t('No')),
      'class' => 'noderelationships-cell-center',
    );

    // Cardinality.
    $cells[] = array(
      'data' => (empty($field['multiple']) ? 1 : ($field['multiple'] == 1 ? t('Unlimited') : (int)$field['multiple'])),
      'class' => 'noderelationships-cell-center',
    );

    // Extra settings for node reference fields are available only for the
    // nodereference_autocomplete widget.
    $extra_features = array();
    if (isset($form['search_and_reference_view'][$field_name])) {

      // Search and reference view.
      $extra_features[] = drupal_render($form['search_and_reference_view'][$field_name]);

      // View reference in new window.
      $extra_features[] = drupal_render($form['view_in_new_window'][$field_name]);

      // Edit reference.
      $extra_features[] = drupal_render($form['edit_reference'][$field_name]);

      // Create and reference.
      $extra_features[] = drupal_render($form['create_and_reference'][$field_name]);

      // Translate and reference.
      if (isset($form['translate_and_reference'])) {
        $extra_features[] = drupal_render($form['translate_and_reference'][$field_name]);
      }
    }
    if (empty($extra_features)) {
      $extra_features[] = t('These options are available only for node reference autocomplete widgets.');
    }
    $cells[] = array(
      'data' => implode('', $extra_features),
      'class' => 'noderelationships-cell-options',
    );

    $rows[] = array('data' => $cells);
  }

  $output .= theme('table', $headers, $rows, array('id' => 'noderelationships-settings-table'));
  $output .= drupal_render($form);

  return $output;
}

/**
 * Back reference settings form.
 */
function noderelationships_admin_settings_backref(&$form_state, $type) {
  $form = array();
  $referrer_types = noderelationships_get_referrer_types($type->type);

  // If this type does not have relations, then we're done here.
  if (empty($referrer_types)) {
    drupal_set_message(t('Could not find content types referring to %type.', array('%type' => $type->name)), 'warning');
    return $form;
  }

  $admin_help_text = t('This panel displays <em>Node Reference</em> fields that may refer to nodes of type %type.
These fields generate <em>back references</em> that you can easily display using a view on several regions (<em>Page</em>, <em>Tab</em> or <em>Fields</em>).
You can modify and/or clone (recommended) the default view should you need to add more fields, filters or change any other option to suit your needs.
To enable these <em>back references</em> just drag them to the region of your choice.
Back references assigned to the <em>Page</em> and <em>Tab</em> regions can be reordered from this panel.
Back references assigned to the <em>Fields</em> region can be reordered from the "Manage fields" panel.', array(
    '%type' => $type->name,
  ));
  $form['help'] = array(
    '#type' => 'markup',
    '#value' => theme('noderelationships_admin_help', $admin_help_text),
  );

  // Obtain back reference settings for this type.
  $backref_settings = noderelationships_settings_load($type->type, 'backref');

  // Prepare a list of all available relations.
  $available_relations = array();
  foreach ($referrer_types as $referrer_type => $field_names) {
    foreach ($field_names as $field_name) {
      $available_relations[$referrer_type .':'. $field_name] = array(
        'referrer_type' => $referrer_type,
        'field_name' => $field_name,
      );
    }
  }

  // Build the list of all relations assigned to active regions.
  $form['#noderelationships-regions'] = array();
  foreach (array_keys(noderelationships_get_back_reference_regions()) as $region) {
    $form['#noderelationships-regions'][$region] = array();
    if (isset($backref_settings['regions'][$region])) {
      foreach ($backref_settings['regions'][$region] as $relation_key => $relation_info) {
        if (isset($available_relations[$relation_key])) {
          $referrer_type = $available_relations[$relation_key]['referrer_type'];
          $field_name = $available_relations[$relation_key]['field_name'];
          $form['#noderelationships-regions'][$region][$relation_key] = array(
            'referrer_type' => $referrer_type,
            'field_name' => $field_name,
            'field' => content_fields($field_name, $referrer_type),
            'weight' => $relation_info['weight'],
            'back_reference_view' => $relation_info['back_reference_view'],
          );
          unset($available_relations[$relation_key]);
        }
      }
    }
  }

  // Assign remaining relations to the disabled region.
  $form['#noderelationships-regions']['none'] = array();
  foreach ($available_relations as $relation_key => $available_relation) {
    $referrer_type = $available_relation['referrer_type'];
    $field_name = $available_relation['field_name'];
    $form['#noderelationships-regions']['none'][$relation_key] = array(
      'referrer_type' => $referrer_type,
      'field_name' => $field_name,
      'field' => content_fields($field_name, $referrer_type),
      'weight' => 0,
      'back_reference_view' => '',
    );
  }
  unset($available_relations);

  $options_back_reference_region = array('none' => '<'. t('none') .'>') + noderelationships_get_back_reference_regions();
  $options_back_reference_view = noderelationships_get_views('backref');

  $form['weight'] = array(
    '#tree' => TRUE,
  );
  $form['back_reference_region'] = array(
    '#tree' => TRUE,
  );
  $form['back_reference_view'] = array(
    '#tree' => TRUE,
  );
  $form['#noderelationships-type'] = $type->type;

  foreach ($form['#noderelationships-regions'] as $region => $region_relations) {
    foreach ($region_relations as $relation_key => $relation_info) {
      $referrer_type = $relation_info['referrer_type'];
      $field_name = $relation_info['field_name'];
      $field = $relation_info['field'];

      $form['weight'][$relation_key] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($relation_info['weight']) ? $relation_info['weight'] : 0),
        '#size' => 3,
        '#attributes' => array('class' => 'noderelationships-weight noderelationships-weight-'. $region),
      );
      $form['back_reference_region'][$relation_key] = array(
        '#type' => 'select',
        '#default_value' => $region,
        '#options' => $options_back_reference_region,
        '#attributes' => array('class' => 'noderelationships-region-select noderelationships-region-'. $region),
      );
      $form['back_reference_view'][$relation_key] = array(
        '#type' => 'select',
        '#default_value' => (isset($relation_info['back_reference_view']) ? $relation_info['back_reference_view'] : ''),
        '#options' => $options_back_reference_view,
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Submit handler for the back reference settings form.
 */
function noderelationships_admin_settings_backref_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $settings = noderelationships_settings_load($form['#noderelationships-type']);

  // Move form values to settings array.
  $settings['backref'] = array('regions' => array());
  foreach (array_keys(noderelationships_get_back_reference_regions()) as $region) {
    foreach ($form_values['back_reference_region'] as $relation_key => $relation_region) {
      if ($region == $relation_region) {
        if (!isset($settings['backref']['regions'][$region])) {
          $settings['backref']['regions'][$region] = array();
        }
        $settings['backref']['regions'][$region][$relation_key] = array(
          'weight' => $form_values['weight'][$relation_key],
          'back_reference_view' => $form_values['back_reference_view'][$relation_key],
        );
      }
    }
  }

  // Update settings and clear caches if changes exist.
  if (noderelationships_settings_save($form['#noderelationships-type'], $settings)) {
    noderelationships_cache_clear_all();
  }
  drupal_set_message(t('The settings have been saved.'));
}

/**
 * Render the back reference settings form.
 */
function theme_noderelationships_admin_settings_backref($form) {
  if (!isset($form['help'])) {
    return drupal_render($form);
  }
  $output = drupal_render($form['help']);

  $module_path = drupal_get_path('module', 'noderelationships');
  drupal_add_css($module_path .'/css/noderelationships.admin_settings.css');
  drupal_add_js($module_path .'/js/admin.drag.js');
  drupal_add_js($module_path .'/js/admin.view_settings.js');
  $js_settings = array(
    'mode' => 'backref',
    'viewSettingsUrl' => url('admin/build/views/edit'),
    'defaultViewName' => NODERELATIONSHIPS_BACKREF_VIEW_NAME,
  );
  drupal_add_js(array('nodeRelationships' => $js_settings), 'setting');
  jquery_ui_add(array('effects.transfer'));

  $destination = drupal_get_destination();
  $headers = array(
    array('data' => ''),
    array('data' => t('Weight'), 'class' => 'noderelationships-cell-weight'),
    array('data' => t('Referrer type/field')),
    array('data' => t('Required'), 'class' => 'noderelationships-cell-center'),
    array('data' => t('Cardinality'), 'class' => 'noderelationships-cell-center'),
    array('data' => t('Back reference view')),
    array('data' => t('Back reference field')),
    array('data' => t('Region'), 'class' => 'noderelationships-cell-region'),
  );
  $headers_count = count($headers);

  $backref_fields = noderelationships_get_backreferences($form['#noderelationships-type']);
  $regions = noderelationships_get_back_reference_regions() + array('none' => t('Disabled'));
  $rows = array();

  foreach ($form['#noderelationships-regions'] as $region => $region_relations) {
    $rows[] = array(
      'data' => array(array(
        'data' => $regions[$region],
        'class' => 'region',
        'colspan' => $headers_count,
      )),
      'class' => 'region region-'. $region,
    );
    $rows[] = array(
      'data' => array(array(
        'data' => '<em>'. t('No items in this region') .'</em>',
        'colspan' => $headers_count,
      )),
      'class' => 'region-message region-'. $region .'-message region-'. (empty($region_relations) ? 'empty' : 'populated'),
    );

    drupal_add_tabledrag('noderelationships-settings-table', 'match', 'sibling', 'noderelationships-region-select', 'noderelationships-region-'. $region);
    drupal_add_tabledrag('noderelationships-settings-table', 'order', 'sibling', 'noderelationships-weight', 'noderelationships-weight-'. $region);

    foreach ($region_relations as $relation_key => $relation_info) {
      $field_name = $relation_info['field_name'];
      $field = $relation_info['field'];

      $cells = array();

      // Drag'n'drop handle, Weight and Back reference region.
      $cells[] = array('data' => '');
      $cells[] = array(
        'data' => drupal_render($form['weight'][$relation_key]),
        'class' => 'noderelationships-cell-weight',
      );

      // Referrer type.
      $type_url_str = str_replace('_', '-', $field['type_name']);
      $type_cell = noderelationships_get_localized_content_type_name($field['type_name']);
      if ($form['#noderelationships-type'] == $field['type_name']) {
        $type_cell = check_plain($type_cell);
        $type_class = 'noderelationships-cell-current-self';
      }
      else {
        $type_cell = l($type_cell, 'admin/content/node-type/'. $type_url_str .'/relationships/noderef');
        $type_class = 'noderelationships-cell-referrer';
      }

      // Referrer field.
      $type_url_str = str_replace('_', '-', $field['type_name']);
      $field_cell = l($field['widget']['label'], 'admin/content/node-type/'. $type_url_str .'/fields/'. $field_name, array('query' => $destination));
      $field_class = ($form['#noderelationships-type'] == $field['type_name'] ? 'noderelationships-cell-noderef-self' : 'noderelationships-cell-noderef');

      // Referrer type/field.
      $cells[] = array(
        'data' => '<div class="'. $type_class .'">'. $type_cell .' / '. $field_cell .'</div>',
      );

      // Required.
      $cells[] = array(
        'data' => ($field['required'] ? t('Yes') : t('No')),
        'class' => 'noderelationships-cell-center',
      );

      // Cardinality.
      $cells[] = array(
        'data' => (empty($field['multiple']) ? 1 : ($field['multiple'] == 1 ? t('Unlimited') : (int)$field['multiple'])),
        'class' => 'noderelationships-cell-center',
      );

      // Back reference view.
      $cells[] = array(
        'data' => drupal_render($form['back_reference_view'][$relation_key]),
        'class' => 'noderelationships-cell-options',
      );

      // Back reference region.
      $cells[] = array(
        'data' => drupal_render($form['back_reference_region'][$relation_key]),
        'class' => 'noderelationships-cell-region',
      );

      // Back reference field.
      $backref_field_name = noderelationships_cck_backref_build_name($form['#noderelationships-type'], $field['type_name'], $field_name);
      if (isset($backref_fields[$backref_field_name])) {
        $type_url_str = str_replace('_', '-', $form['#noderelationships-type']);
        $backref_label = $backref_fields[$backref_field_name]['widget_label'];
        $backref_cell = l($backref_label, 'admin/content/node-type/'. $type_url_str .'/fields/'. $backref_field_name, array('query' => $destination));
      }
      else {
        $backref_label = t('Back references from @referrer-label in @referrer-type', array(
          '@referrer-label' => $field['widget']['label'],
          '@referrer-type' => noderelationships_get_localized_content_type_name($field['type_name']),
        ));
        $backref_cell = '<span title="'. t('This field will be automatically created when the form is submitted.') .'">'. $backref_label .'</span>';
      }
      $cells[] = array(
        'data' => $backref_cell,
        'class' => 'noderelationships-cell-field',
      );

      $rows[] = array('data' => $cells, 'class' => 'draggable noderelationships-region-'. $region);
    }
  }

  $output .= theme('table', $headers, $rows, array('id' => 'noderelationships-settings-table'));
  $output .= drupal_render($form);

  return $output;
}

/**
 * Render a help block.
 */
function theme_noderelationships_admin_help($message) {
  return '<div class="help">'. $message .'</div>';
}

/**
 * Alter variables used for content-admin-field-overview-form.tpl.php.
 */
function _noderelationships_preprocess_content_field_overview_form(&$vars) {
  // Alter row information for back reference fields.
  foreach ($vars['rows'] as $index => $row) {
    if ($row->row_type == 'field' && isset($vars['form'][$row->field_name]['field']['#value'])) {
      $current_field = $vars['form'][$row->field_name]['field']['#value'];
      if (isset($current_field['type']) && $current_field['type'] == 'noderelationships_backref') {
        $type_url_str = str_replace('_', '-', $vars['form']['#type_name']);

        // Use the field type label to display information about the back
        // reference itself.
        if (isset($current_field['referrer_field']) && isset($current_field['referrer_type'])) {
          $referrer_field = content_fields($current_field['referrer_field'], $current_field['referrer_type']);
          $vars['rows'][$index]->type = t('Back references from @referrer-label in @referrer-type', array(
            '@referrer-label' => $referrer_field['widget']['label'],
            '@referrer-type' => noderelationships_get_localized_content_type_name($current_field['referrer_type']),
          ));
        }

        // Use the remove link to point to back reference settings.
        $vars['rows'][$index]->remove = '<br />'. l(t('Back reference settings'), 'admin/content/node-type/'. $type_url_str .'/relationships/backref');
      }
    }
  }
}

/**
 * Alter CCK administration forms.
 *
 * @see noderelationships_form_alter()
 */
function _noderelationships_cck_admin_forms_alter(&$form, $form_state, $form_id) {
  // Alter CCK fields overview form (Manage fields screen).
  if ($form_id == 'content_field_overview_form') {
    if (isset($form['_add_new_field']) && isset($form['_add_new_field']['type']) && isset($form['_add_new_field']['type']['#options'])) {
      // Hide back reference fields from the "Add new field" selector to
      // prevent users from creating Back reference fields manually.
      // We will create these kind of fields from the back reference settings
      // page provided by node relationships module itself.
      if (isset($form['_add_new_field']['type']['#options']['noderelationships_backref'])) {
        unset($form['_add_new_field']['type']['#options']['noderelationships_backref']);
      }
    }
    if (isset($form['_add_existing_field']) && isset($form['_add_existing_field']['field_name']) && isset($form['_add_existing_field']['field_name']['#options'])) {
      // Hide back reference fields from the "Add existing field" selector to
      // prevent the field from being shared. We don't want per field tables.
      foreach (content_fields() as $field) {
        if ($field['type'] == 'noderelationships_backref' && isset($form['_add_existing_field']['field_name']['#options'][$field['field_name']])) {
          unset($form['_add_existing_field']['field_name']['#options'][$field['field_name']]);
        }
      }
    }
    // The above changes to the form are mostly related to usability, where we
    // simply hide options users are not allowed to use, but we need to check
    // this properly. So we now add a validate callback so that we can prevent
    // users from creating back reference fields manually.
    $form['#validate'][] = '_noderelationships_content_field_overview_form_validate';
  }

  // Alter CCK field settings form.
  elseif ($form_id == 'content_field_edit_form' && isset($form['#field']) && isset($form['#field']['type'])) {
    // Node reference field settings.
    if ($form['#field']['type'] == 'nodereference') {
      // Add a submit callback so that we can get rid of information about back
      // references that are no longer enabled.
      $form['#submit'][] = '_noderelationships_nodereference_settings_form_submit';
      return;
    }
    // Back reference field settings.
    elseif ($form['#field']['type'] == 'noderelationships_backref') {
      unset($form['basic']);
      $form['field']['required']['#access'] = FALSE;
      $form['field']['multiple']['#access'] = FALSE;
      $form['widget']['description']['#access'] = FALSE;
      unset($form['widget']['default_value_fieldset']);

      $field = $form['#field'];
      $type = noderelationships_get_localized_content_type($form['type_name']['#value']);
      $type_url_str = str_replace('_', '-', $form['type_name']['#value']);

      $form['widget']['#title'] = t('Back reference field settings');
      $form['widget']['#description'] = t('Back reference fields for the %type content type are created automatically from the settings specified in the <a href="@backref-settings">back reference settings panel</a> for the %type content type.', array(
        '@backref-settings' => url('admin/content/node-type/'. $type_url_str .'/relationships/backref'),
        '%type' => $type->name,
      ));
      $form['widget']['label'] = array(
        '#type' => 'textfield',
        '#title' => t('Label'),
        '#default_value' => $field['widget']['label'],
        '#required' => TRUE,
        '#description' => t('A human-readable name to be used as the label for the back references view displayed for this field in the %type content type.', array('%type' => $type->name)),
      );

      $form['field']['#title'] = t('Referrer information');
      $form['field']['#description'] = t('This section provides information about the node reference field that generates back references to the %type content type.', array(
        '%type' => $type->name,
      ));
      $form['field']['referrer_type'] = array(
        '#type' => 'textfield',
        '#title' => t('Referrer type'),
        '#value' => $field['referrer_type'],
        '#disabled' => TRUE,
      );
      $form['field']['referrer_field'] = array(
        '#type' => 'textfield',
        '#title' => t('Referrer field'),
        '#value' => $field['referrer_field'],
        '#disabled' => TRUE,
      );

      $form['#validate'][] = '_noderelationships_backreference_settings_form_validate';
    }
  }
}

/**
 * Validate handler for CCK fields overview form (Manage fields screen).
 */
function _noderelationships_content_field_overview_form_validate($form, &$form_state) {
  // Make sure back reference fields are not added manually.
  if (isset($form_state['values']['_add_new_field'])) {
    $field = $form_state['values']['_add_new_field'];
    if (!empty($field['type']) && $field['type'] == 'noderelationships_backref') {
      form_set_error('_add_new_field][type', t('Add existing field: Back reference fields are managed by the Node Reference module automatically.'));
    }
  }
  // Make sure back reference fields are not shared.
  if (isset($form_state['values']['_add_existing_field'])) {
    $field = $form_state['values']['_add_existing_field'];
    if (!empty($field['field_name'])) {
      $existing_field = content_fields($field['field_name']);
      if (isset($existing_field['type']) && $existing_field['type'] == 'noderelationships_backref') {
        form_set_error('_add_existing_field][field_name', t('Add existing field: Back reference fields are managed by the Node Reference module automatically.'));
      }
    }
  }
}

/**
 * Validate handler for back reference field settings form.
 */
function _noderelationships_backreference_settings_form_validate($form, &$form_state) {
  // Make sure back reference fields are single valued and not required.
  form_set_value($form['field']['required'], 0, $form_state);
  form_set_value($form['field']['multiple'], 0, $form_state);
  // Get rid of widget related data (not used for these fields).
  form_set_value($form['widget']['description'], '', $form_state);
  form_set_value($form['widget']['default_value_php'], '', $form_state);
}

/**
 * Submit handler for nodereference field settings form.
 */
function _noderelationships_nodereference_settings_form_submit($form, &$form_state) {
  // Get rid of information about back references that are no longer enabled.
  $form_values = $form_state['values'];
  $nodetype = $form_values['type_name'];
  $field_name = $form_values['field_name'];
  $referenceable_types = (!empty($form_values['referenceable_types']) ? array_filter($form_values['referenceable_types']) : array());
  $back_references = noderelationships_settings_list("relation_type = 'backref' AND related_type = '%s' AND field_name = '%s'", $nodetype, $field_name);
  $types_to_sync = array();
  foreach ($back_references as $back_reference) {
    if (!isset($referenceable_types[$back_reference->type_name])) {
      noderelationships_settings_delete("relation_type = 'backref' AND type_name = '%s' AND field_name = '%s'", $back_reference->type_name, $field_name);
      if (!isset($types_to_sync[$back_reference->type_name])) {
        $types_to_sync[$back_reference->type_name] = $back_reference->type_name;
      }
    }
  }
  // Synchronize back reference settings with back reference fields.
  noderelationships_cck_backref_sync_fields($types_to_sync);
}

/**
 * Alter views ui related forms.
 *
 * @see noderelationships_form_alter()
 */
function _noderelationships_views_ui_form_alter(&$form, $form_state, $form_id, $view_tag) {
  // Make sure the path in page displays is not changed.
  // It should be unique and private for this module.
  if ($form_id == 'views_ui_edit_display_form') {
    if (isset($form_state['section']) && $form_state['section'] == 'path' && isset($form['path'])) {
      _noderelationships_views_ui_form_element_disable($form['path'], array(
        'value' => NODERELATIONSHIPS_VIEW_PATH_PREFIX .'/'. str_replace('_', '-', $form_state['view']->name) .'/'. str_replace('_', '-', $form_state['display_id']),
        'description' => t('The path assigned to this view is dynamically customized by the Node Relationships module.'),
      ));
    }
    return;
  }

  // Make sure the view tag is not changed on view settings and clone view forms.
  if ($form_id == 'views_ui_edit_details_form' || $form_id == 'views_ui_add_form') {
    if (isset($form['tag'])) {
      _noderelationships_views_ui_form_element_disable($form['tag'], array(
        'value' => ($form_id == 'views_ui_edit_details_form' ? $form_state['view']->tag : $form['tag']['#default_value']),
        'description' => t('The tag assigned to this view is marked readonly because it is used to trigger the dynamic customizations of the view performed by the Node Relationships module.'),
      ));
    }
    return;
  }
}

/**
 * Assign an arbitrary value and disable the given form element.
 */
function _noderelationships_views_ui_form_element_disable(&$element, $options) {
  $element['#disabled'] = TRUE;
  $element['#default_value'] = $element['#value'] = $options['value'];
  $element['#description'] = $options['description'];
  $element['#size'] = 80;
  if (!isset($element['#element_validate'])) {
    $element['#element_validate'] = array();
  }
  array_unshift($element['#element_validate'], '_noderelationships_views_ui_form_element_validate');
}

/**
 * Assign default value for disabled elements in view ui settings form.
 */
function _noderelationships_views_ui_form_element_validate($element, &$form_state) {
  form_set_value($element, $element['#default_value'], $form_state);
}
