<?php

/**
 * Theme setting defaults
 */
function fusion_core_default_theme_settings() {
  $defaults = array(
    'user_notverified_display'              => 1,
    'breadcrumb_display'                    => 1,
    'search_snippet'                        => 1,
    'search_info_type'                      => 1,
    'search_info_user'                      => 1,
    'search_info_date'                      => 1,
    'search_info_comment'                   => 1,
    'search_info_upload'                    => 1,
    'rebuild_registry'                      => 0,
    'fix_css_limit'                         => 0,
    'block_config_link'                     => 1,
    'grid_mask'                             => 0,
    'theme_grid'                            => 'grid16-960',
    'fluid_grid_width'                      => 'fluid-100',
    'sidebar_layout'                        => 'sidebars-split',
    'sidebar_first_width'                   => 3,
    'sidebar_last_width'                    => 3,
    'theme_font'                            => 'none',
    'theme_font_size'                       => 'font-size-13',
    'primary_menu_dropdown'                 => 1,
  );

  // Add site-wide theme settings
  $defaults = array_merge($defaults, theme_get_settings());

  return $defaults;
}


/**
 * Initialize theme settings if needed
 */
function fusion_core_initialize_theme_settings($theme_name) {
  $theme_settings = theme_get_settings($theme_name);
  if (!isset($theme_settings['primary_menu_dropdown']) || $theme_settings['rebuild_registry'] == 1) {
    static $registry_rebuilt = false;   // avoid multiple rebuilds per page

    // Rebuild theme registry & notify user
    if(isset($theme_settings['rebuild_registry']) && $theme_settings['rebuild_registry'] == 1 && !$registry_rebuilt) {
      drupal_rebuild_theme_registry();
      drupal_set_message(t('Theme registry rebuild completed. <a href="!link">Turn off</a> this feature for production websites.', array('!link' => url('admin/build/themes/settings/' . $GLOBALS['theme']))), 'warning');
      $registry_rebuilt = true;
    }

    // Retrieve saved or site-wide theme settings
    $theme_setting_name = str_replace('/', '_', 'theme_'. $theme_name .'_settings');
    $settings = (variable_get($theme_setting_name, FALSE)) ? theme_get_settings($theme_name) : theme_get_settings();

    // Skip toggle_node_info_ settings
    if (module_exists('node')) {
      foreach (node_get_types() as $type => $name) {
        unset($settings['toggle_node_info_'. $type]);
      }
    }

    // Combine default theme settings from .info file & theme-settings.php
    $theme_data = list_themes();   // get theme data for all themes
    $info_theme_settings = ($theme_name && isset($theme_data[$theme_name]->info['settings'])) ? $theme_data[$theme_name]->info['settings'] : array();
    $defaults = array_merge(fusion_core_default_theme_settings(), $info_theme_settings);

    // Set combined default & saved theme settings
    variable_set($theme_setting_name, array_merge($defaults, $settings));

    // Force theme settings refresh
    theme_get_setting('', TRUE);
  }
}


/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function phptemplate_settings($saved_settings) {
  global $base_url;

  // Get theme name from url (admin/.../theme_name)
  $theme_name = arg(count(arg()) - 1);

  // Combine default theme settings from .info file & theme-settings.php
  $theme_data = list_themes();   // get data for all themes
  $info_theme_settings = ($theme_name && isset($theme_data[$theme_name]->info['settings'])) ? $theme_data[$theme_name]->info['settings'] : array();
  $defaults = array_merge(fusion_core_default_theme_settings(), $info_theme_settings);

  // Combine default and saved theme settings
  $settings = array_merge($defaults, $saved_settings);

  // Create theme settings form widgets using Forms API

  // TNT Fieldset
  $form['tnt_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fusion theme settings'),
    '#description' => t('Use these settings to enhance the appearance and functionality of your Fusion theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // General Settings
  $form['tnt_container']['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Grid settings
  $form['tnt_container']['general_settings']['theme_grid_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Grid type
  // Generate grid type options
  $grid_options = array();
  if (isset($info_theme_settings['theme_grid_options'])) {
    foreach ($info_theme_settings['theme_grid_options'] as $grid_option) {
      $grid_type = (substr($grid_option, 7) == 'fluid') ? t('fluid grid') : t('fixed grid') . ' [' . substr($grid_option, 7) . 'px]';
      $grid_options[$grid_option] = (int)substr($grid_option, 4, 2) . t(' column ') . $grid_type;
    }
  }
  $form['tnt_container']['general_settings']['theme_grid_config']['theme_grid'] = array(
    '#type'          => 'radios',
    '#title'         => t('Select a grid layout for your theme'),
    '#default_value' => $settings['theme_grid'],
    '#options'       => $grid_options,
  );
  $form['tnt_container']['general_settings']['theme_grid_config']['theme_grid']['#options'][$defaults['theme_grid']] .= t(' - Theme Default');
  // Fluid grid width
  $form['tnt_container']['general_settings']['theme_grid_config']['fluid_grid_width'] = array(
    '#type'          => 'select',
    '#title'         => t('Select a width for your fluid grid layout'),
    '#default_value' => $settings['fluid_grid_width'],
    '#options'       => array(
      'fluid-100' => t('100%'),
      'fluid-95' => t('95%'),
      'fluid-90' => t('90%'),
      'fluid-85' => t('85%'),
    ),
  );
  $form['tnt_container']['general_settings']['theme_grid_config']['fluid_grid_width']['#options'][$defaults['fluid_grid_width']] .= t(' - Theme Default');
  // Sidebar layout
  $form['tnt_container']['general_settings']['theme_grid_config']['sidebar_layout'] = array(
    '#type'          => 'radios',
    '#title'         => t('Select a sidebar layout for your theme'),
    '#default_value' => $settings['sidebar_layout'],
    '#options'       => array(
      'sidebars-split' => t('Split sidebars'),
      'sidebars-both-first' => t('Both sidebars first'),
      'sidebars-both-last' => t('Both sidebars last'),
    ),
  );
  $form['tnt_container']['general_settings']['theme_grid_config']['sidebar_layout']['#options'][$defaults['sidebar_layout']] .= t(' - Theme Default');
  // Calculate sidebar width options
  $grid_width = (int)substr($settings['theme_grid'], 4, 2);
  $grid_type = substr($settings['theme_grid'], 7);
  $width_options = array();
  for ($i = 1; $i <= floor($grid_width / 2); $i++) {
    $grid_units = $i . (($i == 1) ? t(' grid unit: ') : t(' grid units: '));
    $width_options[$i] = $grid_units . (($grid_type == 'fluid') ? (round($i * (100 / $grid_width), 2) . '%') : ($i * ((int)$grid_type / $grid_width)) . 'px');
  }
  // Sidebar first width
  $form['tnt_container']['general_settings']['theme_grid_config']['sidebar_first_width'] = array(
    '#type'          => 'select',
    '#title'         => t('Select a different width for your first sidebar'),
    '#default_value' => $settings['sidebar_first_width'],
    '#options'       => $width_options,
  );
  $form['tnt_container']['general_settings']['theme_grid_config']['sidebar_first_width']['#options'][$defaults['sidebar_first_width']] .= t(' - Theme Default');
  // Sidebar last width
  $form['tnt_container']['general_settings']['theme_grid_config']['sidebar_last_width'] = array(
    '#type'          => 'select',
    '#title'         => t('Select a different width for your last sidebar'),
    '#default_value' => $settings['sidebar_last_width'],
    '#options'       => $width_options,
  );
  $form['tnt_container']['general_settings']['theme_grid_config']['sidebar_last_width']['#options'][$defaults['sidebar_last_width']] .= t(' - Theme Default');

  // Theme fonts
  $form['tnt_container']['general_settings']['theme_font_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Typography'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Font family settings
  $form['tnt_container']['general_settings']['theme_font_config']['theme_font_config_font'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Font family'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
   );
  $form['tnt_container']['general_settings']['theme_font_config']['theme_font_config_font']['theme_font'] = array(
    '#type'          => 'radios',
    '#title'         => t('Select a new font family'),
    '#default_value' => $settings['theme_font'],
    '#options'       => array(
      'none' => t('Theme default'),
      'font-family-sans-serif-sm' => '<span class="font-family-sans-serif-sm">' . t('Sans serif - smaller (Helvetica Neue, Arial, Helvetica, sans-serif)') . '</span>',
      'font-family-sans-serif-lg' => '<span class="font-family-sans-serif-lg">' . t('Sans serif - larger (Verdana, Geneva, Arial, Helvetica, sans-serif)') . '</span>',
      'font-family-serif-sm' => '<span class="font-family-serif-sm">' . t('Serif - smaller (Garamond, Perpetua, Nimbus Roman No9 L, Times New Roman, serif)') . '</span>',
      'font-family-serif-lg' => '<span class="font-family-serif-lg">' . t('Serif - larger (Baskerville, Georgia, Palatino, Palatino Linotype, Book Antiqua, URW Palladio L, serif)') . '</span>',
      'font-family-myriad' => '<span class="font-family-myriad">' . t('Myriad (Myriad Pro, Myriad, Trebuchet MS, Arial, Helvetica, sans-serif)') . '</span>',
      'font-family-lucida' => '<span class="font-family-lucida">' . t('Lucida (Lucida Sans, Lucida Grande, Lucida Sans Unicode, Verdana, Geneva, sans-serif)') . '</span>',
      'font-family-tahoma' => '<span class="font-family-tahoma">' . t('Tahoma (Tahoma, Arial, Verdana, sans-serif)') . '</span>',
    ),
  );
  // Font size settings
  $form['tnt_container']['general_settings']['theme_font_config']['theme_font_config_size'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Font size'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  $form['tnt_container']['general_settings']['theme_font_config']['theme_font_config_size']['theme_font_size'] = array(
    '#type'          => 'radios',
    '#title'         => t('Change the base font size'),
    '#description'   => t('Adjusts all text in proportion to your base font size.'),
    '#default_value' => $settings['theme_font_size'],
    '#options'       => array(
      'font-size-10' => t('10px'),
      'font-size-11' => t('11px'),
      'font-size-12' => t('12px'),
      'font-size-13' => t('13px'),
      'font-size-14' => t('14px'),
      'font-size-15' => t('15px'),
      'font-size-16' => t('16px'),
      'font-size-17' => t('17px'),
      'font-size-18' => t('18px'),
    ),
  );
  $form['tnt_container']['general_settings']['theme_font_config']['theme_font_config_size']['theme_font_size']['#options'][$defaults['theme_font_size']] .= t(' - Theme Default');

  // Navigation
  $form['tnt_container']['general_settings']['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Primary menu dropdown
  $form['tnt_container']['general_settings']['navigation']['primary_menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Primary Menu'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['general_settings']['navigation']['primary_menu']['primary_menu_dropdown'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable primary menu as dropdown'),
    '#default_value' => $settings['primary_menu_dropdown'],
  );
  // Breadcrumb
  $form['tnt_container']['general_settings']['navigation']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['general_settings']['navigation']['breadcrumb']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => $settings['breadcrumb_display'],
  );
  
  // Search Settings
  if (module_exists('search')) {
    $form['tnt_container']['general_settings']['search_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search results'),
      '#description' => t('What additional information should be displayed on your search results page?'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_snippet'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display text snippet'),
      '#default_value' => $settings['search_snippet'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display content type'),
      '#default_value' => $settings['search_info_type'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_user'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display author name'),
      '#default_value' => $settings['search_info_user'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_date'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display posted date'),
      '#default_value' => $settings['search_info_date'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_comment'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display comment count'),
      '#default_value' => $settings['search_info_comment'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_upload'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display attachment count'),
      '#default_value' => $settings['search_info_upload'],
    );
  }

  // Username
  $form['tnt_container']['general_settings']['username'] = array(
    '#type' => 'fieldset',
    '#title' => t('Username'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['general_settings']['username']['user_notverified_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "not verified" for unregistered usernames'),
    '#default_value' => $settings['user_notverified_display'],
  );

  // Admin settings
  $form['tnt_container']['admin_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Administrator settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
 $form['tnt_container']['admin_settings']['block_config_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display block configure links for administrators.'),
    '#default_value' => $settings['block_config_link'],
    '#description' => t('This setting provides convenient hover links to block configuration pages directly from the block.'),
  );
 $form['tnt_container']['admin_settings']['grid_mask'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable grid overlay mask for administrators.'),
    '#default_value' => $settings['grid_mask'],
    '#description' => t('This setting enables a "GRID" button in the upper left corner of each page to toggle a grid overlay and block outlines, which can help with visualizing page layout and block positioning.'),
  );

  // Developer settings
  $form['tnt_container']['themedev'] = array(
    '#type' => 'fieldset',
    '#title' => t('Developer settings'),
    '#collapsible' => TRUE,
    '#collapsed' => $settings['rebuild_registry'] ? FALSE : TRUE,
  );
 $form['tnt_container']['themedev']['rebuild_registry'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild theme registry for every page.'),
    '#default_value' => $settings['rebuild_registry'],
    '#description' => t('This setting is useful while developing themes (see <a href="!link">rebuilding the theme registry</a>). However, it <strong>significantly degrades performance</strong> and should be turned off for any production website.', array('!link' => 'http://drupal.org/node/173880#theme-registry')),
  );
 $form['tnt_container']['themedev']['fix_css_limit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Avoid IE stylesheet limit.'),
    '#default_value' => $settings['fix_css_limit'],
    '#description' => t('This setting groups css files so Internet Explorer can see more than 30 of them. This is useful when you cannot use aggregation (e.g., when developing or using private file downloads). But because it degrades performance and can load files out of order, CSS aggregation (<a href="!link">Optimize CSS files</a>) is <strong>strongly</strong> recommended instead for any production website.', array('!link' => $base_url .'/admin/settings/performance')),
  );

  // Return theme settings form
  return $form;
}
