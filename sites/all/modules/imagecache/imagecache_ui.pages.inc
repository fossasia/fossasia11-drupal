<?php

/**
 * Preset Admin callbacks and required functions.
 */
function imagecache_ui_preset_overview() {
  $header = array(t('Preset Name'), t('Storage'), t('Actions'));
  $rows = array();
  // Always clear the preset cache on this display.
  foreach (imagecache_presets(TRUE) as $preset) {
    $row = array();
    $row[] = l($preset['presetname'], 'admin/build/imagecache/'. $preset['presetid']);
    $links = array();
    switch ($preset['storage']) {
      case IMAGECACHE_STORAGE_DEFAULT:
        $row[] = t('Default');
        $links[] = l(t('View'), 'admin/build/imagecache/'. $preset['presetid']);
        $links[] = l(t('Flush'), 'admin/build/imagecache/'. $preset['presetid'] .'/flush' );
        break;
      case IMAGECACHE_STORAGE_OVERRIDE:
        $row[] = t('Override');
        $links[] = l(t('Edit'), 'admin/build/imagecache/'. $preset['presetid']);
        $links[] = l(t('Revert'), 'admin/build/imagecache/'. $preset['presetid'] .'/delete');
        $links[] = l(t('Flush'), 'admin/build/imagecache/'. $preset['presetid'] .'/flush' );
        $links[] = l(t('Export'), 'admin/build/imagecache/'. $preset['presetid'] .'/export' );
        break;
      case IMAGECACHE_STORAGE_NORMAL:
        $row[] = t('Normal');
        $links[] = l(t('Edit'), 'admin/build/imagecache/'. $preset['presetid']);
        $links[] = l(t('Delete'), 'admin/build/imagecache/'. $preset['presetid'] .'/delete');
        $links[] = l(t('Flush'), 'admin/build/imagecache/'. $preset['presetid'] .'/flush' );
        $links[] = l(t('Export'), 'admin/build/imagecache/'. $preset['presetid'] .'/export' );
        break;
    }
    $row[] = implode('&nbsp;&nbsp;&nbsp;&nbsp;', $links);
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows);
  return $output;
}

function imagecache_ui_preset_delete_form($form_state, $preset = array()) {
  if (empty($preset)) {
    drupal_set_message(t('The specified preset was not found'), 'error');
    drupal_goto('admin/build/imagecache');
  }

  $form = array();
  $form['presetid'] = array('#type' => 'value', '#value' => $preset['presetid']);
  return confirm_form(
    $form,
    t('Are you sure you want to delete the preset %preset?',
      array('%preset' => $preset['presetname'])
    ),
    'admin/build/imagecache',
    t('This action cannot be undone.'),
    t('Delete'),  t('Cancel')
  );
}

function imagecache_ui_preset_delete_form_submit($form, &$form_state) {
  $preset = imagecache_preset($form_state['values']['presetid']);
  imagecache_preset_delete($preset);
  drupal_set_message(t('Preset %name (ID: @id) was deleted.', array('%name' => $preset['presetname'], '@id' => $preset['presetid'])));
  $form_state['redirect'] = 'admin/build/imagecache';
}


function imagecache_ui_preset_flush_form(&$form_state, $preset = array()) {
  if (empty($preset)) {
    drupal_set_message(t('The specified preset was not found'), 'error');
    $form_state['redirect'] = 'admin/build/imagecache';
  }

  $form = array();
  $form['presetid'] = array('#type' => 'value', '#value' => $preset['presetid']);
  return confirm_form(
    $form,
    t('Are you sure you want to flush the preset %preset?',
      array('%preset' => $preset['presetname'])
    ),
    'admin/build/imagecache',
    t('This action cannot be undone.'),
    t('Flush'),  t('Cancel')
  );
}

function imagecache_ui_preset_flush_form_submit($form, &$form_state) {
  $preset = imagecache_preset($form_state['values']['presetid']);
  imagecache_preset_flush($preset);
  drupal_set_message(t('Preset %name (ID: @id) was flushed.', array('%name' => $preset['presetname'], '@id' => $preset['presetid'])));
  $form_state['redirect'] = 'admin/build/imagecache';
}


/**
 * Imagecache preset export form.
 */
function imagecache_ui_preset_export_form(&$form_state, $preset = array()) {
  if (empty($preset)) {
    drupal_set_message(t('The specified preset was not found'), 'error');
    $form_state['redirect'] = 'admin/build/imagecache';
  }

  if (isset($preset['presetid'])) {
    unset($preset['presetid']);
  }
  if (isset($preset['storage'])) {
    unset($preset['storage']);
  }
  foreach ($preset['actions'] as $id => $action) {
    unset($preset['actions'][$id]['actionid']);
    unset($preset['actions'][$id]['presetid']);
  }
  $exported = '$presets = array();';
  $exported .= "\n";
  $exported .= '$presets[\''. $preset['presetname'] .'\'] = ';
  $exported .= var_export($preset, TRUE) .';';
  $rows = substr_count($exported, "\n") + 1;

  $form = array();
  $form['export'] = array(
    '#type' => 'textarea',
    '#default_value' => $exported,
    '#rows' => $rows,
    '#resizable' => FALSE,
  );
  return $form;
}


function imagecache_ui_preset_form($form_state, $preset = array()) {
  $form = array();

  $form['presetid'] = array(
    '#type' => 'value',
    '#value' => isset($preset['presetid']) ? $preset['presetid'] : '',
  );

  // Browsers don't submit disabled form values so we've got to put two copies
  // of the name on the form: one for the submit handler and one for the user.
  if (isset($preset['storage']) && $preset['storage'] === IMAGECACHE_STORAGE_DEFAULT) {
    $form['presetname'] = array(
      '#type' => 'value',
      '#value' => $preset['presetname'],
    );
    $form['presetname_display'] = array(
      '#type' => 'textfield',
      '#size' => '64',
      '#title' => t('Preset Namespace'),
      '#default_value' => $preset['presetname'],
      '#disabled' => TRUE,
    );
  }
  else {
    $form['presetname'] = array(
      '#type' => 'textfield',
      '#size' => '64',
      '#title' => t('Preset Namespace'),
      '#default_value' => isset($preset['presetname']) ? $preset['presetname'] : '',
      '#description' => t('The namespace is used in URL\'s for images to tell imagecache how to process an image. Please only use alphanumeric characters, underscores (_), and hyphens (-) for preset names.'),
      '#validate' => array('imagecache_element_presetname_validate' => array()),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($preset['storage']) && $preset['storage'] === IMAGECACHE_STORAGE_DEFAULT ? t('Override Defaults') : t('Save Preset'),
    '#weight' => 9,
  );

  // For new presets don't show the preview or all actions to be added.
  if (empty($preset['presetid'])) {
    return $form;
  }

  $form['actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Actions'),
    '#tree' => TRUE,
    '#theme' => 'imagecache_ui_preset_actions',
  );

  foreach ($preset['actions'] as $i => $action) {
    // skip unknown actions...
    if (!$definition = imagecache_action_definition($action['action'])) {
      continue;
    }
    $action_form['name'] = array(
      '#value' => t($definition['name']),
    );

    $action_form['action'] = array(
      '#type' => 'value',
      '#value' => $action['action'],
    );
    $action_form['actionid'] = array(
      '#type' => 'value',
      '#value' => $action['actionid'],
    );
    $action_form['presetid'] = array(
      '#type' => 'value',
      '#value' => $action['presetid'],
    );

    $action_form['settings'] = array(
      '#theme' => $action['action'],
      '#value' => $action['data'],
    );
    $action_form['data'] = array(
      '#type' => 'value',
      '#value' => $action['data'],
    );
    $action_form['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $action['weight'],
      '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
    );
    $action_form['configure'] = array(
      '#value' => l(t('Configure'), 'admin/build/imagecache/'. $action['presetid'] .'/'. $action['actionid'] ),
      '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
    );
    $action_form['remove'] = array(
      '#value' => l(t('Delete'), 'admin/build/imagecache/'. $action['presetid'] .'/'. $action['actionid'] .'/delete'),
      '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
    );
    $form['actions'][$i] = $action_form;
  }

  $form['actions']['new'] = array(
    '#tree' => FALSE,
    '#type' => 'fieldset',
    '#title' => t('New Actions'),
    '#collapsible' => TRUE,
    '#collapsed' => count($preset['actions']) > 0,
    '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
  );
  foreach (imagecache_action_definitions() as $action => $definition) {
    $form['actions']['new'][] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#value' => l(t('Add !action', array('!action' => $definition['name'])),
                    'admin/build/imagecache/'.  $preset['presetid'] .'/add/'. $action) .
                    ' - '. $definition['description'],
    );
  }

  // Display a preview image for this action. Put it below the submit button so
  // users don't have to do a bunch of scrolling.
  $preview_path = file_create_path('imagecache_sample.png');
  if (!file_exists($preview_path)) {
    // Copy of the image into the files directory so rather than generating it
    // from the original so we don't end up creating subdirectories mirroring
    // the path to the this module.
    $preview_path = drupal_get_path('module', 'imagecache_ui') .'/sample.png';
    file_copy($preview_path, 'imagecache_sample.png');
  }
  imagecache_image_flush($preview_path);
  $imagecache_path = imagecache_create_url($preset['presetname'], $preview_path, TRUE);
  $form['preview'] = array(
    '#type' => 'item',
    '#title' => t('Preview'),
    '#value' => l($imagecache_path, $imagecache_path) .'<br />'. l("<img src='$imagecache_path' />", $imagecache_path, array('html' => TRUE)),
    '#weight' => 10,
  );

  return $form;
}

function imagecache_ui_preset_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  // Check for duplicates
  foreach (imagecache_presets() as $preset) {
    if ($values['presetname'] == $preset['presetname'] && $values['presetid'] != $preset['presetid']) {
      form_set_error('presetname', t('The preset name you have chosen is already in use.'));
      break;
    }
  }

  // Check for illegal characters in preset names
  if (preg_match('/[^0-9a-zA-Z_\-]/', $values['presetname'])) {
    form_set_error('presetname', t('Please only use alphanumeric characters, underscores (_), and hyphens (-) for preset names.'));
  }
}

function imagecache_ui_preset_form_submit($form, &$form_state) {
  // Save the preset first to retrieve the presetid when overriding
  $preset = imagecache_preset_save($form_state['values']);

  // Populate the presetid as needed for overrides
  if (isset($form_state['values']['actions'])) {
    foreach($form_state['values']['actions'] as $action) {
      if (empty($action['presetid'])) {
        $action['presetid'] = $preset['presetid'];
      }
      imagecache_action_save($action);
    }
  }

  // Push back to the preset form
  $form_state['redirect'] = 'admin/build/imagecache/'. $preset['presetid'];
}

function theme_imagecache_ui_preset_actions($form) {
  $header = array(t('Action'), t('Settings'));
  // $header = array(, t('Weight'), '','');
  $rows = array();
  foreach(element_children($form) as $key) {
    if (!is_numeric($key)) {
      continue;
    }
    $row = array();
    $row[] = drupal_render($form[$key]['name']);
    $row[] = drupal_render($form[$key]['settings']);

    // Check for form access before rendering these portions of the table / header
    if (!empty($form[$key]['weight']['#access'])) {
      if (empty($header['weight'])) {
        $header['weight'] = t('Weight');
      }
      $form[$key]['weight']['#attributes']['class'] = 'imagecache-action-order-weight';
      $row[] = drupal_render($form[$key]['weight']);
    }
    if (!empty($form[$key]['configure']['#access'])) {
      if (empty($header['configure'])) {
        $header['configure'] = '';
      }
      $row[] = drupal_render($form[$key]['configure']);
    }
    if (!empty($form[$key]['remove']['#access'])) {
      if (empty($header['remove'])) {
        $header['remove'] = '';
      }
      $row[] = drupal_render($form[$key]['remove']);
    }
    $rows[] = array(
      'data' => $row,
      'class' => 'draggable',
    );
  }

  $output = '';
  if ($rows) {
    drupal_add_tabledrag('imagecache-preset-actions', 'order', 'sibling', 'imagecache-action-order-weight');
    $output = theme('table', $header, $rows, array('id' => 'imagecache-preset-actions'));
  }
  $output .= drupal_render($form);
  return $output;
}


/**
 * Page with form for adding a new action to add to a preset.
 */
function imagecache_ui_action_add_page($preset, $actionname) {
  // Make sure the name is valid.
  if (!$definition = imagecache_action_definition($actionname)) {
    drupal_set_message(t('Unknown action.'), 'error');
    drupal_goto('admin/build/imagecache/'. $preset['presetid']);
  }
  $action = array(
    'presetid' => $preset['presetid'],
    'action' => $actionname,
    'data' => array(),
  );
  return drupal_get_form('imagecache_ui_action_form', $preset, $action);
}


function imagecache_ui_action_form($form_state, $preset, $action) {
  // Do some error checking.
  if (empty($preset['presetid'])) {
    drupal_set_message(t('Unknown preset.'), 'error');
    drupal_goto('admin/build/imagecache');
  }
  if (empty($action['action']) || !$definition = imagecache_action_definition($action['action'])) {
    drupal_set_message(t('Unknown action.'), 'error');
    drupal_goto('admin/build/imagecache/'. $preset['presetid']);
  }
  if ($action['presetid'] != $preset['presetid']) {
    drupal_set_message(t('This %action action is not associated %preset preset.', array('%action' => $action['action'], '%preset' => $preset['presetname'])), 'error');
    drupal_goto('admin/build/imagecache/'. $preset['presetid']);
  }

  $form['#tree'] = TRUE;
  $form['presetid'] = array(
    '#type' => 'value',
    '#value' => $action['presetid'],
  );
  $form['actionid'] = array(
    '#type' => 'value',
    '#value' => isset($action['actionid']) ? $action['actionid']: '',
  );
  $form['action'] = array(
    '#type' => 'value',
    '#value' => $action['action'],
  );

  if (!empty($definition['file'])) {
    require_once($definition['file']);
  }
  if (function_exists($action['action'] .'_form')) {
    $form['data'] = call_user_func($action['action'] .'_form', $action['data']);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($action['actionid']) ? t('Create Action') : t('Update Action'),
  );
  return $form;
}

function imagecache_ui_action_form_submit($form, &$form_state) {
  imagecache_action_save($form_state['values']);
  if (empty($form_state['values']['presetid'])) {
    drupal_set_message(t('The action was succesfully created.'));
  }
  else {
    drupal_set_message(t('The action was succesfully updated.'));
  }
  $form_state['redirect'] = 'admin/build/imagecache/'. $form_state['values']['presetid'];
}


function imagecache_ui_action_delete_form($form_state, $preset = array(), $action = array()) {
  if (empty($action)) {
    drupal_set_message(t('Unknown Action.'), 'error');
    drupal_goto('admin/build/imagecache');
  }
  if (empty($preset)) {
    drupal_set_message(t('Unknown Preset.'), 'error');
    drupal_goto('admin/build/imagecache');
  }

  $form = array();
  $form['actionid'] = array('#type' => 'value', '#value' => $action['actionid']);
  return confirm_form(
    $form,
    t('Are you sure you want to delete the !action action from preset !preset?',
      array('!preset' => $preset['presetname'], '!action' => $action['name'])
    ),
    'admin/build/imagecache',
    t('This action cannot be undone.'),
    t('Delete'),  t('Cancel')
  );
}

function imagecache_ui_action_delete_form_submit($form, &$form_state) {
  $action = imagecache_action($form_state['values']['actionid']);
  imagecache_action_delete($action);
  drupal_set_message(t('The action has been deleted.'));
  $form_state['redirect'] = 'admin/build/imagecache/'. $action['presetid'];
}
