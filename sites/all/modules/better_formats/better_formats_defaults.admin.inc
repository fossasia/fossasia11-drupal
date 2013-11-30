<?php
// $Id: better_formats_defaults.admin.inc,v 1.3.2.11 2010/02/05 13:52:48 dragonwize Exp $

/**
 * @file
 * Contains FAPI and theme functions for the format defaults form.
 */


/**
 * Builds the form for the filters admin.
 *
 * @return
 *  FAPI array
 *
 * @see better_formats_defaults_admin_form_validate()
 * @see better_formats_defaults_admin_form_submit()
 */
function better_formats_defaults_admin_form() {
  $form = array(
    '#tree' => TRUE,
  );

  $nform = better_formats_get_role_default_fields('node');
  $cform = better_formats_get_role_default_fields('comment');
  $bform = better_formats_get_role_default_fields('block');
  $form  = array_merge($form, $nform, $cform, $bform);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save defaults'),
  );

  return $form;
}

/**
 * Validates better_formats_admin_filter_form.
 *
 * @see better_formats_defaults_admin_form()
 * @see better_formats_defaults_admin_form_submit()
 */
function better_formats_defaults_admin_form_validate($form, &$form_state) {
  $formats = filter_formats();
  foreach ($formats as $fid => $format) {
    $roles[$fid] = explode(',', $format->roles);
  }
  // Get roles that have administer filters permission.
  $admin_roles = better_formats_get_roles_by_perm('administer filters');

  foreach ($form_state['values'] as $key => $values) {
    if (strpos($key, 'node-') === 0 || strpos($key, 'comment-') === 0 || strpos($key, 'block-') === 0) {
      list($type, $rid) = explode('-', $key);
      if (in_array($rid, $admin_roles)) {
        // Role has the 'administer filters' permission so it can use all formats.
        continue;
      }
      $fid = $values['format'];
      $site_default = filter_resolve_format(FILTER_FORMAT_DEFAULT);
      if ($fid != 0 && !in_array($rid, $roles[$fid]) && $fid !== $site_default) {
        form_set_error($key, t('Role does not have access to selected format.'));
      }
    }
  }
}

/**
 * Updates database from better_formats_admin_filter_form.
 *
 * @see better_formats_defaults_admin_form()
 * @see better_formats_defaults_admin_form_validate()
 */
function better_formats_defaults_admin_form_submit($form, &$form_state) {
  // Update DB.
  $sql = "UPDATE {better_formats_defaults}
          SET format=%d, weight=%d
          WHERE rid=%d AND type='%s'";

  foreach ($form_state['values'] as $key => $values) {
    if (strpos($key, 'node-') === 0 || strpos($key, 'comment-') === 0 || strpos($key, 'block-') === 0) {
      list($type, $rid) = explode('-', $key);
      db_query($sql, $values['format'], $values['weight'], $rid, $type);
    }
  }

  drupal_set_message(t('Defaults have been saved.'));
}

/**
 * Builds FAPI form elements for the default format selection.
 *
 * @param $mode
 *  'node', 'comment', or 'block'. Top most level type for requested default.
 * @param $node_type
 *  Type of node this request is for.
 * @return
 *  FAPI array for the default select field.
 */
function better_formats_get_role_default_fields($mode, $node_type = '') {
  $form = array();
  $format_options = better_formats_get_formats_per_role();
  $type = $types = $mode;
  $per_node_type = variable_get('better_formats_per_node_type', FALSE);

  if ($per_node_type && $node_type) {
    $type = $mode . '/' . $node_type;
    $types = $type . "','" . $mode;
  }

  // get data from db
  $sql = "SELECT bf.*, role.name
          FROM {better_formats_defaults} AS bf
          INNER JOIN {role} AS role
          ON bf.rid = role.rid
          WHERE bf.type IN ('$types')
          ORDER BY bf.type_weight DESC, bf.weight, role.rid";
  $result = db_query($sql);

  $roles_set = array();

  while ($role = db_fetch_object($result)) {
    if (in_array($role->rid, $roles_set)) {
      continue;
    }

    $roles_set[] = $role->rid;
    $key = $mode . '-' . $role->rid;

    $form[$key]['role'] = array(
      '#value' => $role->name,
    );
    $form[$key]['format'] = array(
      '#type' => 'select',
      '#options' => $format_options[$role->rid],
      '#default_value' => $role->format,
      '#attributes' => array('class' => 'bf-default-formats'),
    );
    $form[$key]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 25,
      '#default_value' => $role->weight,
    );
  }

  return $form;
}

/**
 * Retrieve the formats available to users by role.
 *
 * Gets all formats then creates an array keyed by role IDs
 * that lists the formats available to that role. This is determined
 * by Drupal core's format permissions set at
 * admin/settings/filters/[filter_id].
 *
 * @return
 *  Multi-dim array with role IDs for keys and list of allowed formats.
 *
 * @see better_formats_get_role_default_fields()
 */
function better_formats_get_formats_per_role() {
  $formats = filter_formats();
  $roles   = user_roles();

  // Get roles that have administer filters permission.
  $admin_roles = better_formats_get_roles_by_perm('administer filters');

  $site_default_format = filter_resolve_format(FILTER_FORMAT_DEFAULT);

  foreach ($formats as $format) {
    $roles_allowed = $format->roles ? explode(',', trim($format->roles, ',')) : array();
    foreach ($roles as $rid => $role) {
      $format_options[$rid][0] = t('Site default');
      if ($format->format == $site_default_format || in_array($rid, $admin_roles) || in_array($rid, $roles_allowed)) {
        $format_options[$rid][$format->format] = $format->name;
      }
    }
  }

  return $format_options;
}

/**
 * Get a list of roles that have a permission.
 *
 * @param $perm
 *  Permission string to get roles for.
 * @param $reset
 *  Boolean to clear static cache.
 * @return
 *  An array of role IDs that have the requested permission.
 */
function better_formats_get_roles_by_perm($perm, $reset = FALSE) {
  static $roles;
  if ($reset || !isset($roles[$perm])) {
    $sql = "SELECT rid
            FROM {permission}
            WHERE perm LIKE '%$perm%'
            ORDER BY rid";
    $result = db_query($sql);
    $roles[$perm] = array();
    while ($row = db_fetch_object($result)) {
      $roles[$perm][] = $row->rid;
    }
  }
  return $roles[$perm];
}

/**
 * Process variables for better-defaults-admin-form.tpl.php.
 *
 * @param $vars
 *  The $variables array contains the following arguments:
 *    - $form
 */
function template_preprocess_better_formats_defaults_admin_form(&$vars) {
  foreach (element_children($vars['form']) as $key) {
    $form_row = &$vars['form'][$key];

    //$type = strpos($key, 'node-') === 0 ? 'node' : 'comment';
    $type = substr($key, 0, strpos($key, '-'));

    if (isset($form_row['role'])) {
      // Set special classes needed for table drag and drop.
      $form_row['weight']['#attributes']['class'] = 'better-formats-role-' . $type . '-weight';

      $row = new stdClass();
      $row->role = drupal_render($form_row['role']);
      $row->format_select = drupal_render($form_row['format']);
      $row->weight_select = drupal_render($form_row['weight']);

      $vars[$type . '_default_rows'][$key] = $row;
    }
  }

  $vars['form_submit'] = drupal_render($vars['form']);
}
