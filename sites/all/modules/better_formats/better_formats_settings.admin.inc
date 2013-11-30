<?php
// $Id: better_formats_settings.admin.inc,v 1.6.2.3 2009/02/25 06:44:17 dragonwize Exp $

/**
 * @file
 * Contains FAPI and theme functions for the format settings form.
 */

/**
 * FAPI form builder for admin/settings/filters/settings page.
 *
 * @see better_formats_menu()
 */
function better_formats_settings_admin_form() {
  $form = array();

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display'),
  );
  $form['display']['better_formats_fieldset_title'] = array(
    '#type'  => 'textfield',
    '#default_value' => variable_get('better_formats_fieldset_title', ''),
    '#title' => t('Selection title'),
    '#description' => t('Change the format selection title. Defaults to "Input format"'),
  );
  $form['display']['better_formats_long_tips_link_text'] = array(
    '#type'  => 'textfield',
    '#default_value' => variable_get('better_formats_long_tips_link_text', ''),
    '#title' => t('More format tips link text'),
    '#description' => t('Change the format full tips link text. Defaults to "More information about formatting options"'),
  );


  $form['control'] = array(
    '#type' => 'fieldset',
    '#title' => t('Control'),
  );
  $form['control']['better_formats_per_node_type'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Control formats per node type'),
    '#description' => t('Control formats allowed and default formats per node type. Global settings will be used until a content type admin page is saved.'),
    '#default_value' => variable_get('better_formats_per_node_type', 0),
  );

  return system_settings_form($form);
}
