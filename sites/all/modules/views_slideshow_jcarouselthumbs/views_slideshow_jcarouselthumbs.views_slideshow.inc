<?php

/**
 * @file
 * The default options available with Views Slideshow: jcarouselthumbs.
 */

function views_slideshow_jcarouselthumbs_views_slideshow_modes() {
  $options = array(
    'views_slideshow_jcarouselthumbs' => t('JCarousel Thumbs'),
  );
  return $options;
}

function views_slideshow_jcarouselthumbs_views_slideshow_option_definition() {
  $options['views_slideshow_jcarouselthumbs'] = array(
    'contains' => array (
      'skin_name' => array('default' => 'tango'),
      'skin_path' => array('default' => drupal_get_path('module', 'views_slideshow_jcarouselthumbs') . '/jcarousel/skins/tango'),
      'scroll' => array('default' => 3),
      'timeout' => array('default' => 5000),
      'sort' => array('default' => 1),
      'effect' => array('default' => 'fade'),
      'speed' => array('default' => 300),
      'fade_value' => array('default' => 0.25),
      'hover_breakout' => array('default' => 'teaser'),
      'teasers_last' => array('default' => TRUE),
      'hover' => array('default' => module_exists('hoverintent') ? 'hoverIntent' : 'hover'),
      'main_fields' => array('default' => array()),
      'breakout_fields' => array('default' => array()),
      'delay' => array('default' => 0),
      'fixed_height' => array('default' => 1),
      'random' => array('default' => 0),
      'pause' => array('default' => 1),
      'pause_on_click' => array('default' => 0),
      'pager_event' => array('default' => 'hover'),
      'controls' => array('default' => 0),
      'image_count' => array('default' => 0),
      'sync' => array('default' => 1),
      'advanced' => array('default' => ''),
      'ie' => array(
        'contains' => array(
          'cleartype' => array('default' => 'true'),
          'cleartypenobg' => array('default' => 'false'),
        ),
      ),
      'advanced' => array('default' => ''),
    ),
  );
  return $options;
}

function views_slideshow_jcarouselthumbs_views_slideshow_options_form(&$form, &$form_state, &$view) {
  if (module_exists('hoverintent')) {
    $options = array('hover' => t('Hover'));
    $options['hoverIntent'] = t('HoverIntent');
    $form['views_slideshow_jcarouselthumbs']['hover'] = array(
      '#type' => 'radios',
      '#title' => t('Mouse hover'),
      '#options' => $options,
      '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['hover'],
      '#description' => t('The !hoverintent, if installed, adds the %hoverintent option here. Selecting it causes a delay when the mouse hovers, to stop quick flybys from triggering the behavior. Selecting %hoverdefault chooses the default mouse behavior.', array('!hoverintent' => l(t('HoverIntent module'), 'http://drupal.org/project/hoverintent'), '%hoverintent' => t('HoverIntent'), '%hoverdefault' => t('Hover'))),
    );
  }  
  $options = array(
    'title' => t('Title'),
    'teaser' => t('Teaser'),
    'full' => t('Full'),
  );
  if ($view->row_plugin->uses_fields()) {
    $options = array();
    foreach ($view->display->handler->get_handlers('field') as $field => $handler) {
      $options[$field] = $handler->ui_name();
    }
    $form['views_slideshow_jcarouselthumbs']['main_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Main frame fields'),
      '#options' => $options,
      '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['main_fields'],
      '#description' => t("Choose the fields that will appear in the main slide."),
    );
    $form['views_slideshow_jcarouselthumbs']['breakout_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Breakout fields'),
      '#options' => $options,
      '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['breakout_fields'],
      '#description' => t("Choose the fields that will appear in the breakout thumbnails."),
    );
  } 
  else {
    $form['views_slideshow_jcarouselthumbs']['hover_breakout'] = array(
      '#type' => 'radios',
      '#title' => t('Hover breakout'),
      '#options' => $options,
      '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['hover_breakout'],
      '#description' => t("Hover breakout determines how to display the breakout element of the %thumbnail mode, either as a teaser or full node when the row style is node.", array('%thumbnail' => t('Thumbnail hover'))),
    );
  }
  $form['views_slideshow_jcarouselthumbs']['skin_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Skin Name'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['skin_name'],
    '#description' => t('The name of skin to use for the JCarousel (i.e. "tango")'),
  );
  $form['views_slideshow_jcarouselthumbs']['skin_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Skin Path'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['skin_path'],
    '#description' => t('Path to the JCarousel skin directory. Do not include skin.css or a trailing slash.'),
  );
  $form['views_slideshow_jcarouselthumbs']['scroll'] = array(
    '#type' => 'textfield',
    '#title' => t('Scroll'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['scroll'],
    '#description' => t('The number of items to scroll by.'),
  );
  $form['views_slideshow_jcarouselthumbs']['jcarousel_advanced'] = array(
    '#type' => 'textarea',
    '#title' => t('Advanced JCarousel Options'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['jcarousel_advanced'],
    '#description' => t('Add other jCarousel options one per line. !url', array('!url' => l('Click here for the additional options to add.', 'http://sorgalla.com/projects/jcarousel/'))),
  );
  $form['views_slideshow_jcarouselthumbs']['teasers_last'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breakout fields last'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['teasers_last'],
    '#description' => t('When the mode is thumbnail hover, this determines whether to print the breakout fields first or second.'),
  );
  $form['views_slideshow_jcarouselthumbs']['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timer delay'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['timeout'],
    '#description' => t('Amount of time in milliseconds between transitions.'),
  );
  $form['views_slideshow_jcarouselthumbs']['delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial slide delay offset'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['delay'],
    '#description' => t('Amount of time in milliseconds for the first slide to transition. This number will be added to Timer delay to create the initial delay.  For example if Timer delay is 4000 and Initial delay is 2000 then the first slide will change at 6000ms (6 seconds).  If Initial delay is -2000 then the first slide will change at 2000ms (2 seconds).')
  );
  $form['views_slideshow_jcarouselthumbs']['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['speed'],
    '#description' => t('Time in milliseconds that each transition lasts. Numeric only!'),  
  );
  $form['views_slideshow_jcarouselthumbs']['fixed_height'] = array(
    '#type' => 'select',
    '#title' => t('Make the slide window height fit the largest slide'),
    '#options' => array(1 => t('Yes'),0 => t('No')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['fixed_height'],
    '#description' => t('If this is set to no then if the slides are different sizes the height of the slide area will change as the slides change.'),
  );
  $form['views_slideshow_jcarouselthumbs']['random'] = array(
    '#type' => 'radios',
    '#title' => t('Random'),
    '#description' => t('This option controls the order items are displayed. The default setting, False, uses the views ordering. True will cause the images to display in a random order.'),
    '#options' => array(0 => t('False'), 1 => t('True')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['random'],
  );
  $form['views_slideshow_jcarouselthumbs']['pause'] = array(
    '#type' => 'radios',
    '#title' => t('Pause'),
    '#options' => array(1 => t('Yes'),2 => t('No')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['pause'],
    '#description' => t('Pause when hovering on the slideshow image.'),
  );
  $form['views_slideshow_jcarouselthumbs']['pause_on_click'] = array(
    '#type' => 'radios',
    '#title' => t('Pause On Click'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['pause_on_click'],
    '#description' => t('Pause when the slide is clicked.'),
  );
  $options = array('mouseover' => t('Hover'), 'click' => t('Click'));
  if ((module_exists('jq') && jq_add('hoverIntent')) || (module_exists('hoverintent') && hoverintent_add())) {
    $options['hoverIntent'] = 'hoverIntent';
  }
  $form['views_slideshow_jcarouselthumbs']['pager_event'] = array(
    '#type' => 'select',
    '#title' => t('Pager Event'),
    '#options' => $options,
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['pager_event'],
    '#description' => t('Which event should be used to select an image from the breakout list.'),
  );
  $form['views_slideshow_jcarouselthumbs']['controls'] = array(
    '#type' => 'select',
    '#title' => t('Controls'),
    '#options' => array(0 => t('None'),1 => t('Top'),2 => t('Bottom')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['controls'],
    '#description' => t('Determine if controls for the slideshow (start/stop/next/previous) should appear, and if so whether they should appear before or after the slideshow.'),
  );
  $form['views_slideshow_jcarouselthumbs']['image_count'] = array(
    '#type' => 'select',
    '#title' => t('Image Counter'),
    '#options' => array(0 => t('None'),1 => t('Top'),2 => t('Bottom')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['image_count'],
    '#description' => t('Determine if Image Counter (1 of 4) should be displayed and if so whether they should appear before or after the slideshow.'),
  );
  $options = array(
    'none' => 'none',
    'blindX' => 'blindX',
    'blindY' => 'blindY',
    'blindZ' => 'blindZ',
    'cover' => 'cover',
    'curtainX' => 'curtainX',
    'curtainY' => 'curtainY',
    'fade' => 'fade',
    'fadeZoom' => 'fadeZoom',
    'growX' => 'growX',
    'growY' => 'growY',
    'scrollUp' => 'scrollUp',
    'scrollDown' => 'scrollDown',
    'scrollLeft' => 'scrollLeft',
    'scrollRight' => 'scrollRight',
    'scrollHorz' => 'scrollHorz',
    'scrollVert' => 'scrollVert',
    'shuffle' => 'shuffle',
    'slideX' => 'slideX',
    'slideY' => 'slideY',
    'toss' => 'toss',
    'turnUp' => 'turnUp',
    'turnDown' => 'turnDown',
    'turnLeft' => 'turnLeft',
    'turnRight' => 'turnRight',
    'uncover' => 'uncover',
    'wipe' => 'wipe',
    'zoom' => 'zoom',
  );
  $form['views_slideshow_jcarouselthumbs']['effect'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#options' => $options,
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['effect'],
    '#description' => t('The transition effect that will be used to change between images. Not all options below may be relevant depending on the effect.'),
  );
  $form['views_slideshow_jcarouselthumbs']['sync'] = array(
    '#type' => 'radios',
    '#title' => t('Sync'),
    '#options' => array(1 => t('Yes'), 2 => t('No')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['sync'],
    '#description' => t('The sync option controls whether the slide transitions occur simultaneously. The default is yes which means that the current slide transitions out as the next slide transitions in. By setting the sync option to no you can get some interesting twists on your transitions.'),  
  );
  $form['views_slideshow_jcarouselthumbs']['nowrap'] = array(
    '#type' => 'select',
    '#title' => t('End slideshow after last slide'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['nowrap'],
    '#description' => t('If you select yes the slideshow will end when it gets to the last slide.'),  
  );
  $form['views_slideshow_jcarouselthumbs']['advanced'] = array(
    '#type' => 'textarea',
    '#title' => t('Advanced Options'),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['advanced'],
    '#description' => t('Add other jQuery cycle options one per line.  Ex. height: 350  !url', array('!url' => l('Click here for the additional options to add.', 'http://malsup.com/jquery/cycle/options.html'))),
  );
  $form['views_slideshow_jcarouselthumbs']['ie'] = array(
    '#type' => 'fieldset',
    '#title' => t('Internet Explorer Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['views_slideshow_jcarouselthumbs']['ie']['cleartype'] = array(
    '#type' => 'radios',
    '#title' => t('ClearType'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['ie']['cleartype'],
    '#description' => t('True if clearType corrections should be applied (for IE).  Some background issues could be fixed by setting this to false.'),
  );
  $form['views_slideshow_jcarouselthumbs']['ie']['cleartypenobg'] = array(
    '#type' => 'radios',
    '#title' => t('ClearType Background'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
    '#default_value' => $view->options['views_slideshow_jcarouselthumbs']['ie']['cleartypenobg'],
    '#description' => t('Set to true to disable extra cleartype fixing (leave false to force background color setting on slides)'),
  );
}
function views_slideshow_jcarouselthumbs_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_jcarouselthumbs']['speed'])) {
    form_error($form['views_slideshow_jcarouselthumbs']['speed'], t('!setting must be numeric!',array('!setting' => 'Speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_jcarouselthumbs']['timeout'])) {
    form_error($form['views_slideshow_jcarouselthumbs']['timeout'],t('!setting must be numeric!',array('!setting' => 'Timeout')));
  }
}
