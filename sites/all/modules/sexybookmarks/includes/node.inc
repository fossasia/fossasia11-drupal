<?php
/**
 * @file
 * Node module integration.
 */

/**
 * Implements hook_sexybookmarks_form_alter_alter() on behalf of node.module.
 */
function node_sexybookmarks_form_alter_alter($form, $form_state, $form_id) {
  if ($form_id == 'node_type_form') {
    if (!module_exists('ctools')) {
      return;
    }

    $form['sexybookmarks'] = array(
      '#type' => 'fieldset',
      '#title' => t('SexyBookmarks settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $options = array('' => '- None -');
    ctools_include('export');
    foreach (ctools_export_crud_load_all('sexybookmarks_profiles') as $profile) {
      if (empty($profile->disabled)) {
        $options[$profile->name] = $profile->name;
      }
    }
    $form['sexybookmarks']['node_sexybookmarks_profile'] = array(
      '#type' => 'select',
      '#title' => t('Profile'),
      '#options' => $options,
      '#default_value' => variable_get("node_sexybookmarks_profile_{$form['#node_type']->type}", 'default'),
    );

    $form['sexybookmarks']['node_sexybookmarks_display'] = array(
      '#type' => 'select',
      '#title' => t('Display on'),
      '#options' => array(
        '- None -',
        'Teaser only',
        'Full node only',
        'Teaser and full node'
      ),
      '#default_value' => variable_get("node_sexybookmarks_display_{$form['#node_type']->type}", 3),
    );
  }
}

/**
 * Implements hook_nodeapi().
 */
function sexybookmarks_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
  switch ($op) {
    case 'view':
      $display_mode = variable_get("node_sexybookmarks_display_{$node->type}", 3);
      if (($a3 == 0 && $display_mode & 2) || ($a3 == 1 && $display_mode & 1)) {
        $node->content['sexybookmarks'] = array(
          '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'sexybookmarks') : 100,
          '#value' => theme('sexybookmarks', array(
            'profile' => variable_get("node_sexybookmarks_profile_{$node->type}", 'default'),
            'url' => url("node/{$node->nid}", array('absolute' => TRUE)),
            'title' => $node->title,
          )),
        );
      }
      break;
  }
}
