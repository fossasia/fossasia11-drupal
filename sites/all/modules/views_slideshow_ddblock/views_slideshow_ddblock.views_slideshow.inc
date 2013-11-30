<?php

/**
 *  @file
 *  The default options available with Views Slideshow: ddblock.
 */

/**
 *  Implements hook_views_slideshow_modes().
 */
function views_slideshow_ddblock_views_slideshow_modes() {
  // Expose the mode for ddblock slide shows.
  $options = array(
    'views_slideshow_ddblock' => t('DDblock'),
  );
  return $options;
}

/**
 *  Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_ddblock_views_slideshow_option_definition() {
  // Set our default options.
  $options = array(
    'views_slideshow_ddblock' => array(
      'default' => array(
        'widget' => 'cycle',
        'debug_info' => 'none',
        'mapping_wrapper' => array(
          'mappings' => array(
            0 => array(
              'target' => 'node_id',
              'source' => '',
            ),
            1 => array(
              'target' => 'slide_image',
              'source' => '',
            ),
            2 => array(
              'target' => 'slide_title',
              'source' => '',
            ),
            3 => array(
              'target' => 'slide_text',
              'source' => '',
            ),
            4 => array(
              'target' => 'slide_read_more',
              'source' => '',
            ),
            5 => array(
              'target' => 'pager_image',
              'source' => '',
            ),
            6 => array(
              'target' => 'pager_text',
              'source' => '',
            ),
            7 => array('target' => '', 'source' => '', ),
            8 => array('target' => '', 'source' => '', ),
            9 => array('target' => '', 'source' => '', ),
            10 => array('target' => '', 'source' => '', ),
            11 => array('target' => '', 'source' => '', ),
            12 => array('target' => '', 'source' => '', ),
            13 => array('target' => '', 'source' => '', ),
            14 => array('target' => '', 'source' => '', ),
            15 => array('target' => '', 'source' => '', ),
          ),
        ),
        'template' => 'default',
        'template_size_wrapper' => array(
          'template_size' => 'size_700',
        ),
        'custom_template_size' => '',
        'custom_template' => '',
        'content_container' => array(
          'container' => 'div.slide',
          'overfow' => 0,
        ),
        'settings' => array(
          'fx' => 'fade',
          'speed' => 500,
          'timeout' => 5000,
          'order' => 'none',
          'pause' => 1,
          'next' => 0,
          'pager_toggle' => 1,
          'pager_settings' => array(
            'pager' => 'number-pager',
            'pager_container' => '.custom-pager-item',
            'nr_of_pager_items' => 3,
            'pager_position' => 'top',
            'pager_event' => 'click',
            'pager_fast' => 1,
            'pager_pause' => 1,
          ),
          'pager2' => 0,
          'pager2_settings' => array(
            'pager2_event' => 'click',
            'pager2_position' => array('slide' => 'slide',
                                       'pager' => 0),
            'pager2_pager' => array(
              'pager2_pager_prev' => t('prev'),
              'pager2_pager_next' => t('next'),
              'pager2_pager_hide' => 1,
            ),
            'pager2_slide' => array(
              'pager2_slide_prev' => '',
              'pager2_slide_next' => '',
              'pager2_slide_hide' => 1,
            ),
          ),
          'slide_text' => 1,
          'slide_text_settings' => array(
            'slide_text_container' => 'div.slide-text',
            'slide_text_position' => 'bottom',
            'slide_text_jquery' => 0,
            'slide_jquery' => array(
              'slide_text_before_effect' => 'hide',
              'slide_text_before_speed' => 500,
              'slide_text_after_effect' => 'show',
              'slide_text_after_speed' => 500,
            ),
          ),
          'custom' => array(
            'custom_jquery' => '',
          ),
        ),
      ),
    ),
  );
  return $options;
}

/**
 *  Implements hook_views_slideshow_options_form().
 */
function views_slideshow_ddblock_views_slideshow_options_form(&$form, &$form_state, &$view, $form_state_values = array()) {

  // add views_slideshow_ddblock.admin. js file
  drupal_add_js(drupal_get_path('module', 'views_slideshow_ddblock') . '/js/views_slideshow_ddblock.admin.js');

  // hide the unordered list/ordered list type
  $form['type'] = NULL;

  // widget setting: Enable the dynamic display block setting for this block.
  $options = array(
    'default' => t('Default'),
    'cycle' => t('Cycleblock'),
  );

  // select widget
  $form['views_slideshow_ddblock']['widget'] = array(
    '#type' => 'radios',
    '#title' => t('Display Method'),
    '#default_value' => $view->options['views_slideshow_ddblock']['widget'],
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t("Choose a way to display content."),
    '#weight' => -15,
  );

  // debug settings: Show debug info.
  $options = array(
    'none' => t('None'),
    'drupal' => t('Using drupal_set_message'),
  );
  if (module_exists('devel')) {
    $options['devel'] = t('Using devel module');
  }

  // Select how to show debug info
  $form['views_slideshow_ddblock']['debug_info'] = array(
    '#type' => 'select',
    '#title' => t('Show debug info'),
    '#default_value' => $view->options['views_slideshow_ddblock']['debug_info'],
    '#options' => $options,
    '#required' => FALSE,
    '#description' => t("Show debug info to determine view name/display id, fieldnames, settings, number of items"),
    '#weight' => -14,
  );

  // select themes from [Theme_name]/custom/modules/views_slideshow_ddblock
  $path_to_themes =  _views_slideshow_ddblock_get_theme_path() . '/custom/modules/views_slideshow_ddblock';

  $dirs = _dir_scan_directory($path_to_themes, 'vsd');

  $options = array();
  $options['default'] = 'Default';
  if (!empty($dirs)) {
    asort($dirs);
    foreach ($dirs as $dir) {
      $options[$dir] = $dir;
    }
  }

  $options['custom'] = 'Custom';

  // Set the cache to true to create a $form_state cache on submit
  $form['#cache'] = TRUE;

  // Template.
  $form['views_slideshow_ddblock']['template'] = array(
    '#type' => 'select',
    '#title' => t('Template'),
    '#default_value' => $view->options['views_slideshow_ddblock']['template'],
    '#options' => $options,
    '#multiple' => FALSE,
    '#required' => FALSE,
    '#description' => t("Template for themable output. Template name will become:<br />views-slideshow-ddblock-cycle-block-content-[TEMPLATENAME].tpl.php"),
    '#weight' => -13,
    '#ahah' => array(
      'path' => 'views_slideshow_ddblock/js', // a path defined in hook_menu
      'wrapper' => 'select-template-size-wrapper', // the HTML that wraps the element that needs to be changed
      'method' => 'replace', // what needs to be done: replacing the element with something else
      'event' => 'change',
      'effect' => 'none', // the effect used when replacing the element (try fade!)
    ),
  );

  // Custom template.
  $form['views_slideshow_ddblock']['custom_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom template'),
    '#default_value' => $view->options['views_slideshow_ddblock']['custom_template'],
    '#required' => FALSE,
    '#prefix' => '<div id="ddblock-custom-template-settings-wrapper">',
    '#suffix' => '</div>',
    '#description' => t("Custom template for themable output. Template name will become: <br />views-slideshow-ddblock-cycle-block-content-[TEMPLATENAME].tpl.php"),
    '#weight' => -12,
  );

  if (isset($form_state['form_state_values'])) {
    $form_state_values = $form_state['form_state_values'] + (array)$form_state_values;
  }

  $template = (isset($form_state_values['style_options']['views_slideshow_ddblock']['template'])) ? $form_state_values['style_options']['views_slideshow_ddblock']['template'] : $view->options['views_slideshow_ddblock']['template'];

  $template_sizes = _views_slideshow_ddblock_get_template_size($template);


  // Since a #select element can't have it's own #submit handler, we have to create an extra
  // submit button to which we attach the correct callback function that's going to be called on a change
  // We'll hide this with '#access' => FALSE.
  $form['views_slideshow_ddblock']['template_submit'] = array(
    '#type' => 'submit',
    '#value' => t('get template sizes'),
    '#submit' => array('views_slideshow_ddblock_views_slideshow_options_form_submit'),
    '#weight' => -11,
//  '#access'=> FALSE, // hide the button
    '#description' => t('Get the possible themesizes after selecting a theme'),
  );

  // We define a <div> wrapper that contains the select box that needs to be refilled with data
  $form['views_slideshow_ddblock']['template_size_wrapper'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="select-template-size-wrapper">',
    '#suffix' => '</div>',
    '#weight' => -11,
  );

  // This is the actual select that needs to be refreshed depending on what happens in the 'template' select
  $form['views_slideshow_ddblock']['template_size_wrapper']['template_size'] = array(
    '#type' => 'select',
    '#title' => t('Template size'),
    '#options' => $template_sizes,
    '#default_value' =>  $view->options['views_slideshow_ddblock']['template_size_wrapper']['template_size'],
    '#description' => t('The slideshow template_size css file to use'),
    '#attributes' => array('class' => 'template-size-select'),
    '#prefix' => '<div id="select-template-size">',
    '#suffix' => '</div>',
  );

  // Custom template size.
  $form['views_slideshow_ddblock']['custom_template_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom template size'),
    '#default_value' => $view->options['views_slideshow_ddblock']['custom_template_size'],
    '#required' => FALSE,
    '#prefix' => '<div id="ddblock-custom-template-size-settings-wrapper">',
    '#suffix' => '</div>',
    '#description' => t("Custom size css file for the theme"),
    '#weight' => -10,
  );

  // get the possible source fields form the view
  $source_fields = array('' => t('<None>'));
  if ($view->row_plugin->uses_fields()) {
    foreach ($view->display->handler->get_handlers('field') as $field => $handler) {
      $format = isset($handler->options['format']) ? ' - ' . $handler->options['format'] : '';
      if ($label = $handler->label()) {
        $source_fields[$field] = $label . $format;
      }
      else {
        $source_fields[$field] = $handler->ui_name() . $format;
      }
    }
  }

  // Wrapper in which to place both the mapped fields and the 'add mapping' form.
  $form['views_slideshow_ddblock']['mapping_wrapper'] = array(
    '#tree' => TRUE,
    '#weight' => -5,
    '#prefix' => '<div class="clear-block" id="views-slideshow-ddblock-mapping-wrapper">',
    '#suffix' => '</div>',
  );

    // Wrapper in which to place both the mapped fields and the 'add mapping' form.
  $form['views_slideshow_ddblock']['mapping_wrapper']['helptext'] = array(
    '#type' => 'item',
    '#title' => t('Field mappings'),
    '#description' => t('In this part you map available fields to ddblock theme fields.<br />The ddblock theme fields are the target.<br />Source fields are fields available from your view.'),
  );

  // Wrapper to display existing mappingss.
  $form['views_slideshow_ddblock']['mapping_wrapper']['mappings'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="views-slideshow-ddblock-mappings">',
    '#suffix' => '</div>',
    '#theme' => 'views_slideshow_ddblock_mappings_table',
  );

  $mappings = $view->options['views_slideshow_ddblock']['mapping_wrapper']['mappings'];
  // Get number of mappings.
  $mapping_count = empty($mappings) ? 0 : count($mappings);
  // Add the existing mappings to the form.
  for ($delta = 0; $delta < $mapping_count; $delta++) {
    $target = isset($mappings[$delta]['target']) ? $mappings[$delta]['target'] : '';
    $source = isset($mappings[$delta]['source']) ? $mappings[$delta]['source'] : '';
    // Display existing mappings using helper function views_slideshow_ddblock_mapping_display_form().
    $form['views_slideshow_ddblock']['mapping_wrapper']['mappings'][$delta] = views_slideshow_ddblock_mapping_display_form($delta, $target, $source, $source_fields);
  }

  if ($widget == 'default') {
    $collapsed = TRUE;
  }
  else {
    $collapsed = FALSE;
  }

  // content container settings.
  $form['views_slideshow_ddblock']['content_container'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Content container settings'),
    '#weight' => -2,
  );

  $form['views_slideshow_ddblock']['content_container']['container'] = array(
    '#type' => 'textfield',
    '#title' => t('Content container'),
    '#default_value' => $view->options['views_slideshow_ddblock']['content_container']['container'],
    '#required' => FALSE,
    '#description' => t("Container of the content to slide, eg. CSS selector img, to show images.<br />This can be any CSS selector containing a slide. e.g div.slide"),
  );

  $form['views_slideshow_ddblock']['content_container']['overflow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overflow hidden'),
    '#default_value' => $view->options['views_slideshow_ddblock']['content_container']['overflow'],
    '#prefix' => '<div id="ddblock-advanced-content-container-overflow-wrapper"'. $extra .'>',
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#description' => t("Hide the overflow of the container"),
  );
  // content settings: what to use as content for the dynamic display block.
  $form['views_slideshow_ddblock']['settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
    '#title' => t('Dynamic display block settings'),
    '#weight' => 1,
  );

  $options = _views_slideshow_ddblock_get_effects();
  $form['views_slideshow_ddblock']['settings']['fx'] = array(
    '#type' => 'select',
    '#title' => t('Transition Effect'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['fx'],
    '#options' => $options,
    '#multiple' => FALSE,
    '#required' => TRUE,
    '#description' => t("The transition effect between content. Multiple effects can be set in the Custom jQuery Cycle Plugin Settings."),
  );

  $options = drupal_map_assoc(array(0, 250, 500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000, 4000, 5000));
  $form['views_slideshow_ddblock']['settings']['speed'] = array(
    '#type' => 'select',
    '#title' => t('Speed'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['speed'],
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t("Speed of the transitions (1000 = 1 second, 0 = direct)."),
  );

  $options = drupal_map_assoc(array(0, 250, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 15000, 20000, 30000));
  $form['views_slideshow_ddblock']['settings']['timeout'] = array(
    '#type' => 'select',
    '#title' => t('Timeout'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['timeout'],
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t("The time (in milliseconds) between transitions (1000 = 1 second, 0 to disable auto advance)."),
  );

  $form['views_slideshow_ddblock']['settings']['order'] = array(
    '#type' => 'select',
    '#title' => t('Sort Order'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['order'],
    '#options' => array(
      'none' => t('None'),
      'asc' => t('Ascending'),
      'desc' => t('Descending'),
      'random' => t('Random'),
    ),
    '#multiple' => FALSE,
    '#required' => TRUE,
    '#description' => t("The display order of the content. None for using the original content order."),
  );

  $form['views_slideshow_ddblock']['settings']['pause'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pause'],
    '#description' => t("Enable users to pause the cycle by hovering on the content."),
  );

  $form['views_slideshow_ddblock']['settings']['next'] = array(
    '#type' => 'checkbox',
    '#title' => t('Next'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['next'],
    '#description' => t("Enable users to advanced to the next content by clicking on the content."),
  );

  // pager settings togg;e
  $form['views_slideshow_ddblock']['settings']['pager_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Pager'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_toggle'],
    '#required' => FALSE,
    '#description' => t("Use a pager to select slides"),
  );

  // pager settings.
  //show fields when using pager
  //$extra = (empty($view->options['views_slideshow_ddblock']['pager_toggle'])) ? ' style="display:none"' : '';
  $form['views_slideshow_ddblock']['settings']['pager_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="ddblock-pager-settings-wrapper">',//. $extra .'>',
    '#suffix' => '</div>',
    '#title' => t('Pager settings'),
  );

  $options = array(
    'none'            => t('None'),
    'number-pager'    => t('Number pager'),
    'prev-next-pager' => t('Prev next pager'),
    'custom-pager'    => t('Custom pager'),
    'scrollable-pager' => t('Scrollable pager'),
  );

  $form['views_slideshow_ddblock']['settings']['pager_settings']['pager'] = array(
    '#type' => 'select',
    '#title' => t('Pager'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_settings']['pager'],
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t("Type of pager to add."),
  );

  $form['views_slideshow_ddblock']['settings']['pager_settings']['pager_container'] = array(
    '#type' => 'textfield',
    '#title' => t('Pager container'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_settings']['pager_container'],
    '#required' => FALSE,
    '#description' => t("Container of a pager-item, eg. CSS selector li.<br />This can be any CSS selector containing a pager-item.<br /> For a custom pager this needs to be .custom-pager-item<br />For a scrollable pager this needs to be .scrollable-pager-item"),
  );

  $form['views_slideshow_ddblock']['settings']['pager_settings']['nr_of_pager_items'] = array(
    '#type' => 'textfield',
    '#title' => t('Nr of pager items'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_settings']['nr_of_pager_items'],
    '#required' => FALSE,
    '#prefix' => '<div id="ddblock-nr-of-pager-items-wrapper">',
    '#suffix' => '</div>',
    '#description' => t("Number of pager items used in a scrollable pager."),
  );

  $options = array(
    'top' => t('Top'),
    'right' => t('Right'),
    'bottom' => t('Bottom'),
    'left' => t('Left'),
//    'both' => t('Both'),
  );

  $form['views_slideshow_ddblock']['settings']['pager_settings']['pager_position'] = array(
    '#type' => 'select',
    '#title' => t('Pager position'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_settings']['pager_position'],
    '#prefix' => '<div id="ddblock-pager-position-wrapper">',
    '#suffix' => '</div>',
    '#options' => $options,
    '#required' => FALSE,
    '#description' => t("Possible position for the pager.<br />The position must be supported by the template used to be effective."),
  );

  $options = array(
    'click' => t('Click'),
    'mouseover' => t('Mouseover'),
  );

  $form['views_slideshow_ddblock']['settings']['pager_settings']['pager_event'] = array(
    '#type' => 'select',
    '#title' => t('Pager event'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_settings']['pager_event'],
    '#prefix' => '<div id="ddblock-pager-event-wrapper">',
    '#suffix' => '</div>',
    '#options' => $options,
    '#required' => FALSE,
    '#description' => t("The event on which the pager reacts."),
  );

  $form['views_slideshow_ddblock']['settings']['pager_settings']['pager_fast'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fast Pager'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_settings']['pager_fast'],
    '#prefix' => '<div id="ddblock-pager-fast-wrapper">',
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#description' => t("Use fast pager event when clicked or hovered."),
  );

  $form['views_slideshow_ddblock']['settings']['pager_settings']['pager_pause'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pager pause'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager_settings']['pager_pause'],
    '#prefix' => '<div id="ddblock-pager-pause-wrapper"'. $extra .'>',
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#description' => t("Pause the slideshow when pager hovered."),
  );

  // pager2 settings toggle
  $form['views_slideshow_ddblock']['settings']['pager2'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Prev/Next Pager'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2'],
    '#required' => FALSE,
    '#description' => t("Use a Prev/Next Pager to select slides"),
  );

  // pager2 settings.
  //show fields when using pager
  $extra = (empty($view->options['views_slideshow_ddblock']['settings']['pager2'])) ? ' style="display:none"' : '';
  $form['views_slideshow_ddblock']['settings']['pager2_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="ddblock-pager2-settings-wrapper"'. $extra .'>',
    '#suffix' => '</div>',
    '#title' => t('Prev/next pager settings'),
  );

  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_event'] = array(
    '#type' => 'select',
    '#title' => t('Pager event'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_event'],
    '#prefix' => '<div id="ddblock-pager2-slide-event-wrapper">',
    '#suffix' => '</div>',
    '#options' => $options,
    '#required' => FALSE,
    '#description' => t("The event on which the prev/next pager reacts."),
  );

  $options = array(
    'pager' => t('Pager'),
    'slide' => t('Slide'),
  );

  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_position'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Pager position'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_position'] ? $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_position'] : array(),
    '#prefix' => '<div id="ddblock-pager2-position-wrapper">',
    '#suffix' => '</div>',
    '#options' => $options,
    '#multiple' => TRUE,
    '#required' => FALSE,
    '#description' => t("Possible position for the prev/next pager.<br />The position must be supported by the template used to be effective."),
  );

  // pager2 settings pager.
  //show fields when using pager pager
  $extra = (empty($view->options['views_slideshow_ddblock']['settings']['pager2'])) ? ' style="display:none"' : '';
  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_pager'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="ddblock-pager2-pager-settings-wrapper"'. $extra .'>',
    '#suffix' => '</div>',
    '#title' => t('Pager Pager'),
  );

  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_pager']['pager2_pager_prev'] = array(
    '#type' => 'textfield',
    '#title' => t('Prev text in pager'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_pager']['pager2_pager_prev'],
    '#prefix' => '<div id="ddblock-pager2-pager-prev-text-wrapper">',
    '#size' => 30,
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#description' => t("Caption for the prev pager in the pager.<br />Can also be empty if you use an image to go to the previous slide."),
  );

  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_pager']['pager2_pager_next'] = array(
    '#type' => 'textfield',
    '#title' => t('Next text in pager'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_pager']['pager2_pager_next'],
    '#prefix' => '<div id="ddblock-pager2-pager-next-text-wrapper">',
    '#size' => 30,
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#description' => t("Caption for the next pager in the pager.<br />Can also be empty if you use an image to go to the next slide."),
  );

  // hide prev/next pager when no prev/next slide available
  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_pager']['pager2_pager_hide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prev/Next Hide'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_pager']['pager2_pager_hide'],
    '#required' => FALSE,
    '#description' => t("Hide Prev/Next Pager when no slide available."),
  );

  // pager2 settings slide.
  //show fields when using slide pager
  $extra = (empty($view->options['views_slideshow_ddblock']['settings']['pager2'])) ? ' style="display:none"' : '';
  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_slide'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="ddblock-pager2-slide-settings-wrapper"'. $extra .'>',
    '#suffix' => '</div>',
    '#title' => t('Slide Pager'),
  );

  $options = array(
    'click' => t('Click'),
    'mouseover' => t('Mouseover'),
  );

  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_slide']['pager2_slide_prev'] = array(
    '#type' => 'textfield',
    '#title' => t('Prev text on slide'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_slide']['pager2_slide_prev'],
    '#prefix' => '<div id="ddblock-pager2-slide-prev-text-wrapper">',
    '#size' => 30,
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#description' => t("Caption for the prev pager on the slide.<br />Can also be empty if you use an image to go to the previous slide."),
  );

  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_slide']['pager2_slide_next'] = array(
    '#type' => 'textfield',
    '#title' => t('Next text on slide'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_slide']['pager2_slide_next'],
    '#prefix' => '<div id="ddblock-pager2-slide-next-text-wrapper">',
    '#size' => 30,
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#description' => t("Caption for the next pager on the slide.<br />Can also be empty if you use an image to go to the next slide."),
  );

  // hide prev/next pager when no prev/next slide available
  $form['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_slide']['pager2_slide_hide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prev/Next Hide'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['pager2_settings']['pager2_slide']['pager2_slide_hide'],
    '#required' => FALSE,
    '#description' => t("Hide Prev/Next Pager when no slide available."),
  );

  $form['views_slideshow_ddblock']['settings']['slide_text'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use slide text'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text'],
    '#required' => FALSE,
    '#description' => t("Show slide text when available in content."),
  );

  $form['views_slideshow_ddblock']['settings']['slide_text_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Slide text settings'),
    '#prefix' => '<div id="ddblock-slide-text-settings-wrapper">',
    '#suffix' => '</div>',
  );

  // slide text container field.
  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_text_container'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide text container'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_text_container'],
    '#required' => FALSE,
    '#description' => t("Container of the slide text."),
  );

  $options = array(
    'top' => t('Top'),
    'right' => t('Right'),
    'bottom' => t('Bottom'),
    'left' => t('Left'),
  );

  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_text_position'] = array(
    '#type' => 'select',
    '#title' => t('Slide text position'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_text_position'],
    '#options' => $options,
    '#multiple' => FALSE,
    '#required' => FALSE,
    '#description' => t("Position of the slide text."),
  );

  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_text_jquery'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use jQuery effects for text of a slide'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_text_jquery'],
    '#required' => FALSE,
    '#description' => t("The jQuery effects will be added to the text in the Slide text container"),
    '#weight' => 6,
  );

  //show fields when using jQuery effects for text of a slide
  $extra = (empty($view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_text_jquery'])) ? ' style="display:none"' : '';
  // if the checkbox: jQuery effects for text of a slide is not checked, hide the advanced settings
  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="ddblock-slide-jquery-settings-wrapper"'. $extra .'>',
    '#suffix' => '</div>',
    '#title' => t('Slide text jquery settings'),
    '#weight' => 7,
  );

  $before_effect_options = array(
    'hide' => t('Basics - Hide'),
    'fadeOut' => t('Fading - Fade Out'),
    'slideUp' => t('Sliding - Slide Up'),
  );

  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_before_effect'] = array(
    '#type' => 'select',
    '#title' => t('Before effect'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_before_effect'],
    '#options' => $before_effect_options,
    '#multiple' => FALSE,
    '#required' => FALSE,
    '#description' => t("Before effect of the slide text"),
  );

  $speed_options = drupal_map_assoc(array(0, 250, 500, 1000, 2000, 3000, 4000, 5000));

  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_before_speed'] = array(
    '#type' => 'select',
    '#title' => t('Speed before effect'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_before_speed'],
    '#options' => $speed_options,
    '#required' => FALSE,
    '#description' => t("Speed of the before effect (1000 = 1 second, 0 = direct)."),
  );

  $after_effect_options = array(
    'show' => t('Basics - Show'),
    'fadeIn' => t('Fading - Fade In'),
    'slideDown' => t('Sliding - Slide Down'),
  );

  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_after_effect'] = array(
    '#type' => 'select',
    '#title' => t('After effect'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_after_effect'],
    '#options' => $after_effect_options,
    '#multiple' => FALSE,
    '#required' => FALSE,
    '#description' => t("After effect of the slide text"),
  );

  $form['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_after_speed'] = array(
    '#type' => 'select',
    '#title' => t('Speed after effect'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['slide_text_settings']['slide_jquery']['slide_text_after_speed'],
    '#options' => $speed_options,
    '#required' => FALSE,
    '#description' => t("Speed of the before effect (1000 = 1 second, 0 = direct)."),
  );

  $form['views_slideshow_ddblock']['settings']['custom'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
    '#title' => t('Custom jQuery Cycle Plugin Settings'),
    '#description' => t('If you use custom jQuery options, they will override your other settings.'),
  );

  if ($view->options['views_slideshow_ddblock']['settings']['custom']['custom_jquery']) {
    $collapsed = FALSE;
  }
  else {
    $collapsed = TRUE;
  }

  $form['views_slideshow_ddblock']['settings']['custom']['custom_jquery'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom Options'),
    '#default_value' => $view->options['views_slideshow_ddblock']['settings']['custom']['custom_jquery'],
    '#cols' => 60,
    '#rows' => 10,
    '#required' => FALSE,
    '#description' => t('Use valid JSON syntax, with double quotes for key/and string value pairs.<br />The total script needs to be enclosed in curly brackets.<br />No comma allowed after the last statement like in an array.<br />e.g.<br />{"fx":"fade",<br />"startingSlide":2,<br />"autostop":1}'),
  );
}

/**
 * AHAH callback for the 'template' select.
 * This function  deletes the element which needs replacing from the $form and $form_state arrays
 * and replace it with a fresh one.
 */
function views_slideshow_ddblock_select_template_ahah() {
  // this part is used to set up $form_state.
  // In Drupal 7, these next 11 lines will be put in a core utility function.
  // Just remember you'll need them in D6 when you do AHAH!
  // $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_state = array('storage' => NULL, 'rebuild' => TRUE);
  $form_build_id = $_POST['form_build_id'];

  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);

  $form['#post']       = $_POST;
  $form['#redirect']   = FALSE;
  $form['#programmed'] = FALSE;
  $form_state['post']  = $_POST;

  // Prevents _form_builder_ie_cleanup() from incorrectly assigning the
  // first button in the form as the clicked button.
  // Wim Leers' AHAH Helper Module has more in-depth information.
  // @see the ahah_helper project
  $form_state['submitted'] = TRUE;

  drupal_process_form($form_id, $form, $form_state);

  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  // From here on, we start our own code.
  // We just get the element of $form that needs to be refreshed, and just resubmit that
  // part through the json call. In this case we want to replace the 'template_size' <div> wrapper and it's
  // containing select box
  $changed_elements = $form['style_options']['views_slideshow_ddblock']['template_size_wrapper'];
  unset($changed_elements['#prefix'], $changed_elements['suffix']); // we'll unset the div to make sure it won't be repeated!
  // the actual JSON call
  $javascript = drupal_add_js(NULL, NULL, 'header');
  drupal_json(array(
    'status'   => TRUE,
    'data'     => theme('status_messages') . drupal_render($changed_elements), // rebuild just the part that needs to be changed
    'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
  ));
}

/**
 * This submit handler is hooked to the hidden submit button and is executed on the AHAH call
 */
function views_slideshow_ddblock_views_slideshow_options_form_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state_values = $form_state['values'];
  $form_state['form_state_values'] = $form_state_values;
  $form_state['rebuild'] = TRUE;
  return $form_state_values;
}

/**
 * Form validation
 */
function views_slideshow_ddblock_views_slideshow_options_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_ddblock']['settings']['pager_settings']['nr_of_pager_items'])) {
    form_error($form['views_slideshow_ddblock']['settings']['pager_settings']['nr_of_pager_items'], t('!setting must be numeric!', array('!setting' => 'nr_of_pager_items')));
  }
}



/**
 * Get theme sizes of a theme.
 *
 * @param $theme
 *   Slidehsow theme.
 * @return
 *   An array containing the theme sizes.
 */
function _views_slideshow_ddblock_get_template_size($theme) {
 
  $path_to_themes = _views_slideshow_ddblock_get_theme_path() . '/custom/modules/views_slideshow_ddblock/' . $theme;

  $dirs = _dir_scan_directory($path_to_themes, 'siz');

  if (!empty($dirs)) {
    asort($dirs);
    foreach ($dirs as $dir) {
      $options[$dir] = $dir;
    }
  }
  else {
    $options['default'] = 'Default';
  }

  $options['custom'] = 'Custom';
  return $options;
}

/**
 * Return available effect for the views slideshow dynamic display block module.
 *
 * @return
 *   An associative array containing the available effect for the views slideshow dynamic display block module.
 */
function _views_slideshow_ddblock_get_effects() {
  // effects.
  $_fx = array(
    'all' => t('Random'),
    'blindX' => t('blindX'),
    'blindY' => t('blindY'),
    'blindZ' => t('blindZ'),
    'cover' => t('cover'),
    'curtainX' => t('curtainX'),
    'curtainY' => t('curtainY'),
    'fade' => t('fade'),
    'fadeZoom' => t('fadeZoom'),
    'growX' => t('growX'),
    'growY' => t('growY'),
    'scrollUp' => t('scrollUp'),
    'scrollDown' => t('scrollDown'),
    'scrollLeft' => t('scrollLeft'),
    'scrollRight' => t('scrollRight'),
    'scrollHorz' => t('scrollHorz'),
    'scrollVert' => t('scrollVert'),
    'shuffle' => t('shuffle'),
    'slideX' => t('slideX'),
    'slideY' => t('slideY'),
    'toss' => t('soss'),
    'turnUp' => t('turnUp'),
    'turnDown' => t('turnDown'),
    'turnLeft' => t('turnLeft'),
    'turnRight' => t('turnRight'),
    'uncover' => t('uncover'),
    'wipe' => t('wipe'),
    'zoom' => t('zoom'),
  );

  return $_fx;
}

/**
 * Get the sub directories, starting with the mapping string, of a directory.
 *
 * @param $dir
 *   directory.
 * @param $mapping
 *   the start characters of the directory
 * @return
 *   An array of directories.
 */
function _dir_scan_directory($dir, $mapping = '') {
  $dirs = array();

  if (is_dir($dir) && $handle = opendir($dir)) {
    while ($file = readdir($handle)) {
      if ($file[0] != '.') {
        if (is_dir("$dir/$file")) {
          if (!empty($mapping)) {
            if (substr($file, 0, 3) == $mapping)  {
              // add dir.
              $dirs[] = $file;
            }
          }
          else {
            // add dir.
            $dirs[] = $file;
          }
        }
      }
    }
    closedir($handle);
  }

  return $dirs;
}

/**
 * Returns the path to actual site theme in use because path_to_theme is flaky.
 */
function _views_slideshow_ddblock_get_theme_path() {
  // path_to_theme gives the path to the admin theme when a different one is used.
  // As a band-aid solution, this will pull the default theme out of the
  // database and use that. (got this from advanced_forum module, in this case always
  // use the default theme from database)
  static $theme_path;
  
    if (!$theme_path) {
      $default_theme = variable_get('theme_default', 'garland');
      $theme_path = drupal_get_path('theme', $default_theme); 
    }
    return $theme_path;    
}



