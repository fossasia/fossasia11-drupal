<?php
// $Id: noderelationships.pages.inc,v 1.1.2.28 2010/05/16 18:26:01 markuspetrux Exp $

/**
 * @file
 * Implementation of user land pages.
 */

/**
 * Menu callback; Display back reference views in the node relationships tab.
 */
function noderelationships_backref_page($referred_node) {
  // Include common module functions.
  module_load_include('inc', 'noderelationships');

  // Obtain back reference settings for this type.
  $backref_settings = noderelationships_settings_load($referred_node->type, 'backref');
  $backref_regions = $backref_settings['regions'];

  if (empty($backref_regions[NODERELATIONSHIPS_BACKREF_REGION_TAB])) {
    drupal_not_found();
    return;
  }

  $region_settings = $backref_regions[NODERELATIONSHIPS_BACKREF_REGION_TAB];
  $content = noderelationships_backref_build_content($referred_node, $region_settings);
  $output = (!empty($content) ? drupal_render($content) : '');

  if (empty($output)) {
    drupal_set_message(t('This %type-name does not have relations.', array('%type-name' => noderelationships_get_localized_content_type_name($referred_node->type))), 'warning');
  }
  return $output;
}

/**
 * Implementation of hook_nodeapi('view').
 *
 * @param $referred_node
 * @param $region_settings
 */
function _noderelationships_nodeapi_view($referred_node, &$region_settings) {
  $content = noderelationships_backref_build_content($referred_node, $region_settings);
  if (!empty($content)) {
    $referred_node->content['noderelationships'] = array(
      '#type' => 'fieldset',
      '#title' => t('Related content'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => noderelationships_get_element_weight($referred_node->type),
      'content' => $content,
    );
  }
}

/**
 * Build back reference content for the given node and region settings.
 *
 * @param $referred_node
 * @param $region_settings
 */
function noderelationships_backref_build_content($referred_node, &$region_settings) {
  $content = array();
  $referred_type = noderelationships_get_localized_content_type($referred_node->type);

  foreach ($region_settings as $relation_key => $relation_info) {
    list($type_name, $field_name) = explode(':', $relation_key);
    $referrer_type = noderelationships_get_localized_content_type($type_name);

    $context = array(
      'region_settings' => $region_settings,
      'referred_node' => $referred_node,
      'referred_type' => $referred_type,
      'referrer_type' => $referrer_type,
      'field_name' => $field_name,
    );

    $output = noderelationships_backref_render_view($relation_info['back_reference_view'], $context);

    if (!empty($output)) {
      $content['backref_'. $type_name .'_'. $field_name] = array(
        '#value' => $output,
        '#weight' => $relation_info['weight'],
      );
    }
  }

  // Allow external modules alter back references content.
  $context = array(
    'region_settings' => $region_settings,
    'referred_node' => $referred_node,
    'referred_type' => $referred_type,
  );
  drupal_alter('noderelationships_backref_content', $content, $context);

  return $content;
}

/**
 * Display a back reference view.
 *
 * @param $back_reference_view
 * @param $context
 * @param $render_view_title
 *
 * @see noderelationships_views_pre_view()
 * @see noderelationships_customize_backref_view()
 */
function noderelationships_backref_render_view($back_reference_view, $context, $render_view_title = TRUE) {
  static $pager_element = 0;

  $referred_node = &$context['referred_node'];
  $referred_type = &$context['referred_type'];
  $referrer_type = &$context['referrer_type'];
  $field_name = $context['field_name'];

  // Load settings of the referring field.
  $referrer_field = content_fields($field_name, $referrer_type->type);
  if (empty($referrer_field)) {
    drupal_set_message(t('Could not locate %field in content type %type.', array('%field' => $field_name, '%type' => $referrer_type->type)), 'error');
    watchdog('noderelationships', 'Could not locate %field in content type %type.', array('%field' => $field_name, '%type' => $referrer_type->type), WATCHDOG_ERROR);
    return '';
  }

  // Prepare the name of the view and display.
  if (empty($back_reference_view)) {
    $view_name = NODERELATIONSHIPS_BACKREF_VIEW_NAME;
    $display_id = 'default';
  }
  else {
    list($view_name, $display_id) = explode(':', $back_reference_view);
  }

  // Load the view and check access to the given display.
  if (!($view = views_get_view($view_name)) || !$view->access($display_id)) {
    drupal_set_message(t('Could not load the view %view:%display.', array('%view' => $view_name, '%display' => $display_id)), 'error');
    watchdog('noderelationships', 'Could not load the view %view:%display.', array('%view' => $view_name, '%display' => $display_id), WATCHDOG_ERROR);
    return '';
  }

  // Prepare values for the view arguments as follows:
  // 0 - nid of the referred node.
  // 1 - type of the referring node.
  // 2 - name of the nodereference field in the referring type.
  $view_args = array($referred_node->nid, $referrer_type->type, $field_name);

  // Assign a different pager element for each view in the same page request.
  $view->display_handler->override_option('pager_element', $pager_element);

  // Make sure the view is completely valid.
  $errors = $view->validate();
  if (is_array($errors)) {
    foreach ($errors as $error) {
      drupal_set_message($error, 'error');
    }
    return '';
  }

  // Execute and render the view in preview mode. Note that we only generate
  // output if the executed view query returns any result.
  $view->preview = TRUE;
  $view->pre_execute($view_args);
  $view->execute($display_id);
  if (!empty($view->result)) {
    $content = $view->display_handler->preview();
  }
  $view->post_execute();
  if (empty($view->result)) {
    return '';
  }

  // Now that we have output for this view, we need to increase the pager
  // element counter.
  $pager_element++;

  // Find out a title for this view.
  if ($render_view_title) {
    // If the view title is empty (default), then we'll generate one.
    $view_title = $view->get_title();
    if (empty($view_title)) {
      $arguments = array(
        '%node-title' => $referred_node->title,
        '%referrer-type-name' => $referrer_type->name,
        '%referred-type-name' => $referred_type->name,
        '%referrer-label' => $referrer_field['widget']['label'],
      );
      $view_title = t('Back references from %referrer-label in %referrer-type-name for %referred-type-name: %node-title', $arguments);

      // Allow external modules alter the view title.
      noderelationships_alter_label($view_title, 'backref_view_title', $context, $arguments);
    }
  }
  else {
    $view_title = '';
  }

  return theme('noderelationships_backref_view', $view_title, $content);
}

/**
 * Render a block for a back reference view.
 */
function theme_noderelationships_backref_view($title, $content) {
  $output = '<dl class="noderelationships-backref-view">';
  if (!empty($title)) {
    $output .= '<dt>'. $title .'</dt>';
  }
  $output .= '<dd>'. $content .'</dd>';
  $output .= '</dl>';
  return $output;
}

/**
 * Field formatter for back references view.
 */
function theme_noderelationships_formatter_default($element) {
  // Check if we have a node here. We should.
  if (empty($element['#node']) || empty($element['#node']->nid)) {
    return '';
  }

  // Load back reference field data.
  $field = content_fields($element['#field_name'], $element['#type_name']);
  if (empty($field['referrer_field']) || empty($field['referrer_type'])) {
    return '';
  }

  // Include common module functions.
  module_load_include('inc', 'noderelationships');

  // Obtain back reference settings for this type.
  $backref_settings = noderelationships_settings_load($element['#type_name'], 'backref');
  $backref_regions = $backref_settings['regions'];
  if (empty($backref_regions[NODERELATIONSHIPS_BACKREF_REGION_FIELD])) {
    return '';
  }

  // Check we really have relation information.
  $region_settings = $backref_regions[NODERELATIONSHIPS_BACKREF_REGION_FIELD];
  if (empty($region_settings[$field['referrer_type'] .':'. $field['referrer_field']])) {
    return '';
  }
  $relation_info = $region_settings[$field['referrer_type'] .':'. $field['referrer_field']];

  // Build the context structure and render the back reference view.
  $context = array(
    'region_settings' => $region_settings,
    'referred_node' => $element['#node'],
    'referred_type' => noderelationships_get_localized_content_type($element['#type_name']),
    'referrer_type' => noderelationships_get_localized_content_type($field['referrer_type']),
    'field_name' => $field['referrer_field'],
  );
  return noderelationships_backref_render_view($relation_info['back_reference_view'], $context, FALSE);
}

/**
 * Field formatter for back references count.
 */
function theme_noderelationships_formatter_count($element) {
  // Check if we have a node here. We should.
  if (empty($element['#node']) || empty($element['#node']->nid)) {
    return '';
  }

  // Load back reference field data.
  $field = content_fields($element['#field_name'], $element['#type_name']);
  if (empty($field['referrer_field']) || empty($field['referrer_type'])) {
    return '';
  }

  // Include common module functions.
  module_load_include('inc', 'noderelationships');

  // Obtain back reference settings for this type.
  $backref_settings = noderelationships_settings_load($element['#type_name'], 'backref');
  $backref_regions = $backref_settings['regions'];
  if (empty($backref_regions[NODERELATIONSHIPS_BACKREF_REGION_FIELD])) {
    return '';
  }

  // Check we really have relation information.
  $region_settings = $backref_regions[NODERELATIONSHIPS_BACKREF_REGION_FIELD];
  if (empty($region_settings[$field['referrer_type'] .':'. $field['referrer_field']])) {
    return '';
  }

  // Get information about the referrer field.
  $referrer_field = content_fields($field['referrer_field'], $field['referrer_type']);
  if (empty($referrer_field)) {
    return '';
  }
  $db_info = content_database_info($referrer_field);
  if (empty($db_info) || empty($db_info['columns'])) {
    return '';
  }
  $db_column = $field['referrer_field'] .'_'. current(array_keys($db_info['columns']));

  // Compute and return the count of back references for the given field.
  return db_result(db_query('SELECT COUNT(*) FROM {'. $db_info['table'] .'} WHERE '. $db_column .' = %d', $element['#node']->nid));
}

/**
 * Menu callback; Search or Create and reference.
 */
function noderelationships_noderef_page($mode, $referrer_type = NULL, $field_name = NULL, $arg = NULL) {
  // Enable the Modal Frame API for the child window.
  modalframe_child_js();

  // Try to load information about the requested nodereference field.
  $referrer_field = (isset($field_name) && isset($referrer_type) ? content_fields($field_name, $referrer_type) : NULL);
  if (empty($referrer_field)) {
    drupal_not_found();
    return;
  }

  $module_path = drupal_get_path('module', 'noderelationships');
  drupal_add_css($module_path .'/css/noderelationships.noderef_dialog.css');
  drupal_add_js($module_path .'/js/noderef_dialog.js');

  // Include common module functions.
  module_load_include('inc', 'noderelationships');

  // Obtain settings for nodereference extras for this type.
  $noderef_settings = noderelationships_settings_load($referrer_type, 'noderef');
  $create_or_translate_enabled = (isset($noderef_settings['create_and_reference'][$field_name]) || (isset($noderef_settings['translate_and_reference'][$field_name]) && noderelationships_translation_supported_type($referrer_type)));

  if ($mode == 'create' && $create_or_translate_enabled) {
    $content = noderelationships_noderef_page_create($referrer_type, $field_name, $arg);
    if (!empty($content)) {
      $output = '';
      if (isset($noderef_settings['search_and_reference_view'][$field_name]) && !noderelationships_noderef_get_translation_page_title()) {
        $output .= theme('noderelationships_noderef_page_tabs', $mode, $referrer_type, $field_name);
      }
      $output .= theme('noderelationships_noderef_page_content', $content);
    }
  }
  elseif ($mode == 'search' && isset($noderef_settings['search_and_reference_view'][$field_name])) {
    list($view_name, $display_id) = explode(':', $noderef_settings['search_and_reference_view'][$field_name]);
    $field_multiple = (isset($arg) && $arg == 'multiselect' ? (int)$referrer_field['multiple'] : 0);
    $content = noderelationships_noderef_page_search($referrer_type, $field_name, $referrer_field, $view_name, $display_id, $field_multiple);
    if (!empty($content)) {
      $output = '';
      if ($create_or_translate_enabled && !$field_multiple) {
        $output .= theme('noderelationships_noderef_page_tabs', $mode, $referrer_type, $field_name);
      }
      $output .= theme('noderelationships_noderef_page_content', $content);
    }
  }

  if (isset($output)) {
    return $output;
  }

  // Determine if an HTTP error has already been sent.
  if (preg_match('`^HTTP/1.1\s*[45][0-9][0-9]\s*`m', drupal_get_headers())) {
    return;
  }
  // Default to a 404 error page.
  drupal_not_found();
}

/**
 * Build the create and reference page.
 *
 * @see node_add_page()
 * @see node_add()
 * @see _noderelationships_child_node_form_alter()
 * @see _noderelationships_child_node_form_submit()
 */
function noderelationships_noderef_page_create($referrer_type, $field_name, $new_type) {
  $reference_fields = noderelationships_get_reference_fields($referrer_type);

  if (isset($reference_fields[$field_name])) {
    // Build the list of content types related to the given parent node type,
    // and nodereference field, that can be created by the current user.
    $creatable_types = noderelationships_get_creatable_types($referrer_type);

    // Do we know the content type the user wants to create?
    if (empty($new_type)) {
      // Display the list of content types that can be referred from the
      // given field in the current parent content type.
      $node_add_list = array();
      foreach ($reference_fields[$field_name] as $new_type) {
        if (isset($creatable_types[$new_type])) {
          $translated_menuitem = menu_get_item('node/add/'. str_replace('_', '-', $new_type));
          if (!empty($translated_menuitem)) {
            $translated_menuitem['href'] = 'noderelationships/create/'. $referrer_type .'/'. $field_name .'/'. $new_type;
            $translated_menuitem['localized_options']['attributes'] = array('class' => 'modalframe-exclude');
            $query = noderelationships_querystring();
            if (!empty($query)) {
              $translated_menuitem['localized_options']['query'] = $query;
            }
            $node_add_list[] = $translated_menuitem;
          }
        }
      }
      // Only display the content type selection screen if more than one
      // type is available.
      if (count($node_add_list) > 1) {
        return theme('node_add_list', $node_add_list);
      }
    }

    // And now? Do we know the content type the user wants to create?
    if (!empty($new_type)) {
      // Let's try to redirect to a regular node/add form for the specified type.
      if (isset($creatable_types[$new_type])) {
        $_GET['noderelationships_referrer_type'] = $referrer_type;
        $_GET['noderelationships_field_name'] = $field_name;
        $query = drupal_query_string_encode($_GET, array('q'));
        unset($_REQUEST['destination'], $_REQUEST['edit']['destination']);
        drupal_goto('node/add/'. str_replace('_', '-', $new_type), $query);
      }
    }
  }
}

/**
 * Get the translation page title.
 *
 * @see translation_nodeapi()
 */
function noderelationships_noderef_get_translation_page_title($reset = FALSE) {
  static $page_title;
  if (!isset($page_title) || $reset) {
    $page_title = FALSE;
    if (isset($_GET['translation']) && isset($_GET['language']) && is_numeric($_GET['translation']) && user_access('translate content')) {
      $translation_source = node_load($_GET['translation']);
      if (!empty($translation_source) && node_access('view', $translation_source)) {
        $language_list = noderelationships_get_localized_language_list();
        if (isset($language_list[$_GET['language']]) && ($translation_source->language != $_GET['language'])) {
          $page_title = t('Translating @title from @lang-from to @lang-to', array(
            '@title' => $translation_source->title,
            '@lang-from' => $language_list[$translation_source->language],
            '@lang-to' => $language_list[$_GET['language']],
          ));
        }
      }
    }
  }
  return $page_title;
}

/**
 * Build the search and reference page.
 */
function noderelationships_noderef_page_search($referrer_type, $field_name, $referrer_field, $view_name, $display_id, $field_multiple) {
  // Load the view and check access to the given display.
  if (!($view = views_get_view($view_name)) || !$view->access($display_id)) {
    drupal_set_message(t('Could not load the view %view:%display.', array('%view' => $view_name, '%display' => $display_id)), 'error');
    watchdog('noderelationships', 'Could not load the view %view:%display.', array('%view' => $view_name, '%display' => $display_id), WATCHDOG_ERROR);
    return;
  }

  // Add javascript to the search and reference page.
  $js_settings = array(
    'maxAllowedValues' => ($field_multiple == 1 ? 0 : ($field_multiple == 0 ? 1 : $field_multiple)),
  );
  drupal_add_js(array('nodeRelationships' => $js_settings), 'setting');
  jquery_ui_add(array('ui.sortable', 'effects.transfer'));

  // Prepare values for the view arguments as follows:
  // 0 - type of the referrer node.
  // 1 - name of the nodereference field in the referrer type.
  $view_args = array($referrer_type, $field_name);

  // Make sure the view is completely valid.
  $errors = $view->validate();
  if (is_array($errors)) {
    foreach ($errors as $error) {
      drupal_set_message($error, 'error');
    }
    return '';
  }

  // Execute the view display.
  $output = $view->execute_display($display_id, $view_args);

  if (!empty($output)) {
    if ($referrer_field['multiple'] && $js_settings['maxAllowedValues'] != 1) {
      $output = theme('noderelationships_noderef_multiselect', $referrer_field) ."\n". $output;
    }
    else {
      $output = theme('noderelationships_noderef_singleselect', $referrer_field) ."\n". $output;
    }
  }

  return $output;
}

/**
 * Render the single selection panel for search and reference views.
 */
function theme_noderelationships_noderef_singleselect($referrer_field) {
  $output = '<div class="noderelationships-noderef-singleselect">';
  $output .= '<label>'. t('Currently selected %field', array('%field' => $referrer_field['widget']['label'])) .':</label>';
  $output .= ' <span></span>';
  $output .= '</div>';

  return $output;
}

/**
 * Render the multi selection panel for search and reference views.
 */
function theme_noderelationships_noderef_multiselect($referrer_field) {
  $multiselect = '<div class="noderelationships-noderef-multiselect-items">';
  $multiselect .=   '<label>'. check_plain($referrer_field['widget']['label']) .':</label>';
  $multiselect .=   '<div class="clear-block">';
  $multiselect .= '<div class="noderelationships-noderef-multiselect-items-list"><ul></ul></div>';
  $multiselect .= '<div class="noderelationships-noderef-multiselect-actions">';
  $multiselect .=   '<a href="javascript:void(0)" class="modalframe-exclude noderelationships-noderef-multiselect-button noderelationships-noderef-multiselect-sort-desc" title="'. check_plain(t('Sort descending')) .'">'. check_plain(t('Desc.')) .'</a>';
  $multiselect .=   '<a href="javascript:void(0)" class="modalframe-exclude noderelationships-noderef-multiselect-button noderelationships-noderef-multiselect-sort-asc" title="'. check_plain(t('Sort ascending')) .'">'. check_plain(t('Asc.')) .'</a>';
  $multiselect .=   '<a href="javascript:void(0)" class="modalframe-exclude noderelationships-noderef-multiselect-button noderelationships-noderef-multiselect-reset" title="'. check_plain(t('Reset selection')) .'">'. check_plain(t('Reset')) .'</a><br />';
  $multiselect .=   '<a href="javascript:void(0)" class="modalframe-exclude noderelationships-noderef-multiselect-button noderelationships-noderef-multiselect-save" title="'. check_plain(t('Close dialog and save changes')) .'">'. check_plain(t('Save')) .'</a>';
  $multiselect .=   '<a href="javascript:void(0)" class="modalframe-exclude noderelationships-noderef-multiselect-button noderelationships-noderef-multiselect-cancel" title="'. check_plain(t('Close dialog and discard changes')) .'">'. check_plain(t('Cancel')) .'</a>';
  $multiselect .= '</div>';
  $multiselect .=   '</div>';
  $multiselect .= '</div>';

  $output = '<h2>'. t('Search and reference multiple items at once') .'</h2>';
  $output .= '<div class="noderelationships-noderef-multiselect clear-block">';
  $output .= theme('fieldset', array(
    '#collapsible' => TRUE,
    '#title' => t('Selected items'),
    '#value' => $multiselect,
  ));
  $output .= '</div>'."\n";

  return $output;
}

/**
 * Render search/create tabs.
 */
function theme_noderelationships_noderef_page_tabs($mode, $referrer_type, $field_name) {
  $options = array('attributes' => array('class' => 'modalframe-exclude'));
  $query = noderelationships_querystring();
  if (!empty($query)) {
    $options['query'] = $query;
  }
  $output = '<div class="noderelationships-noderef-page-tabs"><ul>';
  $output .= '<li'. ($mode == 'search' ? ' class="active"' : '') .'>'. l(t('Search and reference'), 'noderelationships/search/'. $referrer_type .'/'. $field_name, $options) .'</li>';
  $output .= '<li'. ($mode == 'create' ? ' class="active"' : '') .'>'. l(t('Create and reference'), 'noderelationships/create/'. $referrer_type .'/'. $field_name, $options) .'</li>';
  $output .= '</ul></div>'."\n";
  return $output;
}

/**
 * Render search/create content area.
 */
function theme_noderelationships_noderef_page_content($content) {
  return theme('noderelationships_noderef_page_prefix') . $content . theme('noderelationships_noderef_page_suffix') ."\n";
}

/**
 * Render search/create content prefix.
 */
function theme_noderelationships_noderef_page_prefix() {
  return '<div class="noderelationships-noderef-page-content">';
}

/**
 * Render search/create content suffix.
 */
function theme_noderelationships_noderef_page_suffix() {
  return '</div>';
}

/**
 * Alter parent node form to attach extra buttons to node reference fields.
 */
function _noderelationships_parent_node_form_alter(&$form, $form_state, $type_name, $noderef_settings) {
  $reference_fields = noderelationships_get_reference_fields($type_name);
  $creatable_types = noderelationships_get_creatable_types($type_name);
  $content_fields = array();

  // Prepare nodereference settings.
  $field_settings = array();
  foreach (array_keys($reference_fields) as $field_name) {
    if (!isset($content_fields[$field_name])) {
      $content_fields[$field_name] = content_fields($field_name, $type_name);
    }
    $field_settings[$field_name] = array(
      'maxAllowedValues' => ($content_fields[$field_name]['multiple'] == 1 ? 0 : (empty($content_fields[$field_name]['multiple']) ? 1 : (int)$content_fields[$field_name]['multiple'])),
      'ahahSearchUrl' => url('noderelationships/ahah/search/'. $type_name .'/'. $field_name),
    );
    if (isset($noderef_settings['edit_reference'][$field_name])) {
      $field_settings[$field_name]['editReference'] = TRUE;
    }
    if (isset($noderef_settings['search_and_reference_view'][$field_name])) {
      $field_settings[$field_name]['searchUrl'] = url('noderelationships/search/'. $type_name .'/'. $field_name);
    }
    if (isset($noderef_settings['create_and_reference'][$field_name]) && isset($reference_fields[$field_name])) {
      // Compute the list of content types that can be referred from this
      // nodereference field and the current user is allowed to create.
      $field_creatable_types = array_intersect($creatable_types, $reference_fields[$field_name]);
      if (!empty($field_creatable_types)) {
        $field_settings[$field_name]['createUrl'] = url('noderelationships/create/'. $type_name .'/'. $field_name);
      }
    }
    $field_settings[$field_name]['viewInNewWindow'] = !empty($noderef_settings['view_in_new_window'][$field_name]);
  }

  // If we're translating a node, then let's find missing translations for
  // fields where the "Translate and reference" option has been enabled.
  if (empty($form_state['submitted']) && isset($form['translation_source']) && isset($noderef_settings['translate_and_reference']) && noderelationships_translation_supported_type($type_name)) {
    module_load_include('inc', 'noderelationships', 'noderelationships.translation');
    _noderelationships_parent_node_form_build_translation_settings($form['#node'], $field_settings, $noderef_settings);
  }

  $form['#noderelationships'] = $field_settings;

  // Install an after_build callback. This one is used to have support for the
  // AHAH request related to the "Add more items" button of multivalue fields.
  if (!isset($form['#after_build'])) {
    $form['#after_build'] = array();
  }
  $form['#after_build'][] = '_noderelationships_parent_node_form_scanner_proxy';

  // Install a pre_render callback. This one is used to have support when form
  // validation errors are found. The after_build callback is not invoked in
  // that case because the form is not rebuilt.
  if (!isset($form['#pre_render'])) {
    $form['#pre_render'] = array();
  }
  $form['#pre_render'][] = '_noderelationships_parent_node_form_scanner_proxy';
}

/**
 * Form pre-render callback for parent node.
 */
function _noderelationships_parent_node_form_scanner($form) {
  static $processed;

  $field_settings = &$form['#noderelationships'];

  if (!isset($processed)) {
    $processed = TRUE;
    // Enable the Modal Frame API for the parent window.
    modalframe_parent_js();

    // Send javascript and stylesheets to the page.
    $module_path = drupal_get_path('module', 'noderelationships');
    drupal_add_css($module_path .'/css/noderelationships.node_form.css');
    drupal_add_js($module_path .'/js/node_form.js');
    $js_settings = array(
      'fieldSettings' => $field_settings,
      'viewUrl' => url('node/nid'),
      'editUrl' => url('node/nid/edit'),
    );
    drupal_add_js(array('nodeRelationships' => $js_settings), 'setting');
  }

  // Scan the form recursively to append CSS classes to node reference fields.
  _noderelationships_parent_node_form_scanner_recursive($form, $field_settings);

  return $form;
}

/**
 * Scan the form recursively to append CSS classes to node reference fields.
 */
function _noderelationships_parent_node_form_scanner_recursive(&$elements, $field_settings) {
  // Proceed only if user has access to this element and children.
  if (isset($elements['#access']) && !$elements['#access']) {
    return;
  }
  if (isset($elements['#field_name']) && isset($field_settings[$elements['#field_name']]) && isset($elements['#type'])) {
    $field_name = $elements['#field_name'];
    if ($elements['#type'] == 'nodereference_autocomplete' && isset($elements['nid']) && isset($elements['nid']['nid'])) {
      _noderelationships_element_append_class($elements['nid']['nid'], 'noderelationships-nodereference-autocomplete noderelationships['. $field_name .']');
    }
  }
  else {
    // Recurse through all children elements.
    foreach (element_children($elements) as $key) {
      if (isset($elements[$key]) && $elements[$key]) {
        _noderelationships_parent_node_form_scanner_recursive($elements[$key], $field_settings);
      }
    }
  }
}

/**
 * Append a class to a form element.
 */
function _noderelationships_element_append_class(&$element, $class_name) {
  if (!isset($element['#attributes'])) {
    $element['#attributes'] = array();
  }
  if (!isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = $class_name;
  }
  elseif (strpos($element['#attributes']['class'], $class_name) === FALSE) {
    $element['#attributes']['class'] .= ' '. $class_name;
  }
}

/**
 * Alter create child node form.
 */
function _noderelationships_child_node_form_alter(&$form, $referrer_type, $field_name) {
  // Install a pre_render callback.
  if (!isset($form['#pre_render'])) {
    $form['#pre_render'] = array();
  }
  $form['#pre_render'][] = '_noderelationships_child_node_form_pre_render_proxy';
  $form['#noderelationships_child'] = array('referrer_type' => $referrer_type, 'field_name' => $field_name);

  // Append our submit handler so we can tell the parent window to close
  // the modal frame and update the node reference field.
  $form['buttons']['submit']['#submit'][] = '_noderelationships_child_node_form_submit_proxy';
}

/**
 * Alter edit child node form.
 */
function _noderelationships_child_edit_form_alter(&$form) {
  // Append our submit handler so we can tell the parent window to close
  // the modal frame and update the node reference field.
  $form['buttons']['submit']['#submit'][] = '_noderelationships_child_node_form_submit_proxy';

  // Disable the delete button.
  if (isset($form['buttons']['delete'])) {
    $form['buttons']['delete']['#access'] = FALSE;
  }
}

/**
 * Pre-render handler for child node form.
 */
function _noderelationships_child_node_form_pre_render($form) {
  static $ready;
  // Be carefull because the node edit form is rendered by theme_node_form(),
  // and theme_node_form() executes drupal_render($form) again, so we're
  // invoked twice. The first time by drupal_render_form(), and the second
  // time by theme_node_form().
  if (!isset($ready)) {
    $ready = TRUE;

    // Send stylesheets and javascript to the page.
    $module_path = drupal_get_path('module', 'noderelationships');
    drupal_add_css($module_path .'/css/noderelationships.noderef_dialog.css');
    drupal_add_js($module_path .'/js/noderef_dialog.js');

    // Wrap the form into our own container.
    $referrer_type = $form['#noderelationships_child']['referrer_type'];
    $field_name = $form['#noderelationships_child']['field_name'];

    // Render tabs when 'Search and reference' option is also enabled.
    $noderef_settings = noderelationships_settings_load($referrer_type, 'noderef');
    if (isset($noderef_settings['search_and_reference_view'][$field_name])) {
      $prefix = theme('noderelationships_noderef_page_tabs', 'create', $referrer_type, $field_name);
    }
    else {
      $prefix = '';
    }
    $prefix .= theme('noderelationships_noderef_page_prefix');
    $suffix = theme('noderelationships_noderef_page_suffix');

    $form['#prefix'] = $prefix . (!empty($form['#prefix']) ? $form['#prefix'] : '');
    $form['#suffix'] = (!empty($form['#suffix']) ? $form['#suffix'] : '') . $suffix;
  }
  return $form;
}

/**
 * Submit handler for the child node form.
 */
function _noderelationships_child_node_form_submit($form, &$form_state) {
  // Read the node title from fresh node so that modules such as Automatic Node
  // Titles [1] can do their own job, and then we can get the resulting title.
  // [1] http://drupal.org/project/auto_nodetitle
  // Also, clear the static storage of node_load() because it could have been
  // executed by another module during this page request, which would bring us
  // a non-updated version of the node.
  $node = node_load($form_state['nid'], NULL, TRUE);
  modalframe_close_dialog(array(
    'operation' => 'updateSingleValue',
    'value' => $node->title .' [nid:'. $form_state['nid'] .']',
  ));
}

/**
 * Menu callback for AHAH management of multiple value fields.
 *
 * @see content_add_more_js()
 */
function noderelationships_noderef_ahah_search($type_name, $field_name, $item_count) {
  // Include node form code from CCK.
  module_load_include('inc', 'content', 'includes/content.node_form');

  $type = content_types($type_name);
  $field = content_fields($field_name, $type['type']);

  if (($field['multiple'] != 1) || empty($_POST['form_build_id'])) {
    // Invalid request.
    drupal_json(array('data' => ''));
    exit;
  }

  // Retrieve the cached form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  if (!$form) {
    // Invalid form_build_id.
    drupal_json(array('data' => ''));
    exit;
  }

  // Define the requested number of "empty" multiple values.
  if (!is_numeric($item_count) || $item_count <= 0) {
    $item_count = 1;
  }
  $_POST[$field_name] = array();
  for ($i = 0; $i < $item_count; $i++) {
    $_POST[$field_name][] = array('_weight' => $i);
  }

  // We don't simply return a new empty widget to append to existing ones, because
  // - ahah.js won't simply let us add a new row to a table
  // - attaching the 'draggable' behavior won't be easy
  // So we resort to rebuilding the whole table of widgets including the existing ones,
  // which makes us jump through a few hoops.

  // The form that we get from the cache is unbuilt. We need to build it so that
  // _value callbacks can be executed and $form_state['values'] populated.
  // We only want to affect $form_state['values'], not the $form itself
  // (built forms aren't supposed to enter the cache) nor the rest of $form_data,
  // so we use copies of $form and $form_data.
  $form_copy = $form;
  $form_state_copy = $form_state;
  $form_copy['#post'] = array();
  form_builder($_POST['form_id'], $form_copy, $form_state_copy);
  // Reset cached ids, so that they don't affect the actual form we output.
  form_clean_id(NULL, TRUE);

  // Sort the $form_state['values'] we just built *and* the incoming $_POST data
  // according to d-n-d reordering.
  unset($form_state['values'][$field_name][$field['field_name'] .'_add_more']);
  $form_state['values'][$field_name] = array();
  foreach ($_POST[$field_name] as $delta => $item) {
    $form_state['values'][$field_name][$delta]['_weight'] = $item['_weight'];
    $form_state['values'][$field_name][$delta]['_remove'] = isset($item['_remove']) ? $item['_remove'] : 0;
  }
  $form_state['values'][$field_name] = _content_sort_items($field, $form_state['values'][$field_name]);
  $_POST[$field_name] = _content_sort_items($field, $_POST[$field_name]);

  // Build our new form element for the whole field, asking for one more element.
  $form_state['item_count'] = array($field_name => count($_POST[$field_name]));
  $form_element = content_field_form($form, $form_state, $field);
  // Let other modules alter it.
  drupal_alter('form', $form_element, array(), 'content_add_more_js');

  // Add the new element at the right place in the (original, unbuilt) form.
  if (module_exists('fieldgroup') && ($group_name = _fieldgroup_field_get_group($type['type'], $field_name))) {
    $form[$group_name][$field_name] = $form_element[$field_name];
  }
  else {
    $form[$field_name] = $form_element[$field_name];
  }

  // Save the new definition of the form.
  $form_state['values'] = array();
  form_set_cache($form_build_id, $form, $form_state);

  // Build the new form against the incoming $_POST values so that we can
  // render the new element.
  $form_state = array('submitted' => FALSE);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $field_form = (!empty($group_name)) ? $form[$group_name][$field_name] : $form[$field_name];
  // We add a div around the new content to receive the ahah effect.
  $field_form['#prefix'] = '<div class="ahah-new-content">'. (isset($field_form['#prefix']) ? $field_form['#prefix'] : '');
  $field_form['#suffix'] = (isset($field_form['#suffix']) ? $field_form['#suffix'] : '') .'</div>';

  // If a newly inserted widget contains AHAH behaviors, they normally won't
  // work because AHAH doesn't know about those - it just attaches to the exact
  // form elements that were initially specified in the Drupal.settings object.
  // The new ones didn't exist then, so we need to update Drupal.settings
  // by ourselves in order to let AHAH know about those new form elements.
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';

  $output = theme('status_messages') . drupal_render($field_form) . $output_js;

  // Using drupal_json() breaks filefield's file upload, because the jQuery
  // Form plugin handles file uploads in a way that is not compatible with
  // 'text/javascript' response type.
  $GLOBALS['devel_shutdown'] =  FALSE;
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}
