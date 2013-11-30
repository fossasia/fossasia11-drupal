<?php

/**
 * @file
 * The default options available with Views Slideshow: SingleFrame.
 */

function views_slideshow_singleframe_views_slideshow_modes() {
  $options = array(
    'views_slideshow_singleframe' => t('SingleFrame'),
  );
  return $options;
}

function views_slideshow_singleframe_views_slideshow_option_definition() {
  $options['views_slideshow_singleframe'] = array(
    'contains' => array(
      'timeout' => array('default' => 5000),
      'sort' => array('default' => 1),
      'effect' => array('default' => 'fade'),
      'speed' => array('default' => 700), //normal
      'start_paused' => array('default', 0),
      'delay' => array('default' => 0),
      'fixed_height' => array('default' => 1),
      'random' => array('default' => 0),
      'pause' => array('default' => 1),
      'pause_on_click' => array('default' => 0),
      'pause_when_hidden' => array('default' => 0),
      'pause_when_hidden_type' => array('default' => 'full'),
      'amount_allowed_visible' => array('default' => ''),
      'remember_slide' => array('default' => 0),
      'remember_slide_days' => array('default' => 1),
      'controls' => array('default' => 0),
      'items_per_slide' => array('default' => 1),
      'pager' => array('default' => 0),
      'pager_type' => array('default' => 0),
      'pager_hover' => array('default' => 2),
      'pager_click_to_page' => array('default' => 0),
      'image_count' => array('default' => 0),
      'nowrap' => array('default' => 0),
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

function views_slideshow_singleframe_views_slideshow_options_form(&$form, &$form_state, &$view) {
  $form['views_slideshow_singleframe']['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timer delay'),
    '#default_value' => $view->options['views_slideshow_singleframe']['timeout'],
    '#description' => t('Amount of time in milliseconds between transitions. Set the value to 0 to not rotate the slideshow automatically.')
  );
  $form['views_slideshow_singleframe']['delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial slide delay offset'),
    '#default_value' => $view->options['views_slideshow_singleframe']['delay'],
    '#description' => t('Amount of time in milliseconds for the first slide to transition. This number will be added to Timer delay to create the initial delay.  For example if Timer delay is 4000 and Initial delay is 2000 then the first slide will change at 6000ms (6 seconds).  If Initial delay is -2000 then the first slide will change at 2000ms (2 seconds).')
  );
  $form['views_slideshow_singleframe']['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#default_value' => $view->options['views_slideshow_singleframe']['speed'],
    '#description' => t('Time in milliseconds that each transition lasts. Numeric only!'),  
  );
  $form['views_slideshow_singleframe']['start_paused'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start Slideshow Paused'),
    '#default_value' => $view->options['views_slideshow_singleframe']['start_paused'],
    '#description' => t('Start the slideshow in the paused state.'),  
  );
  $form['views_slideshow_singleframe']['fixed_height'] = array(
    '#type' => 'select',
    '#title' => t('Make the slide window height fit the largest slide'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $view->options['views_slideshow_singleframe']['fixed_height'],
    '#description' => t('If this is set to no then if the slides are different sizes the height of the slide area will change as the slides change.'),
  );
  $form['views_slideshow_singleframe']['random'] = array(
    '#type' => 'radios',
    '#title' => t('Random'),
    '#description' => t('This option controls the order items are displayed. The default setting, False, uses the views ordering. True will cause the images to display in a random order.'),
    '#options' => array(0 => t('False'), 1 => t('True')),
    '#default_value' => $view->options['views_slideshow_singleframe']['random'],
  );
  $form['views_slideshow_singleframe']['pause'] = array(
    '#type' => 'radios',
    '#title' => t('Pause'),
    '#options' => array(1 => t('Yes'), 2 => t('No')),
    '#default_value' => $view->options['views_slideshow_singleframe']['pause'],
    '#description' => t('Pause when hovering on the slideshow image.'),
  );
  $form['views_slideshow_singleframe']['pause_on_click'] = array(
    '#type' => 'radios',
    '#title' => t('Pause On Click'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $view->options['views_slideshow_singleframe']['pause_on_click'],
    '#description' => t('Pause when the slide is clicked.'),
  );
  $form['views_slideshow_singleframe']['pause_when_hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause When the Slideshow is Not Visible'),
    '#default_value' => $view->options['views_slideshow_singleframe']['pause_when_hidden'],
    '#description' => t('When the slideshow is scrolled out of view or when a window is resized that hides the slideshow, this will pause the slideshow.'),
  );
  $form['views_slideshow_singleframe']['pause_when_hidden_type'] = array(
    '#type' => 'select',
    '#title' => t('How to Calculate Amount of Slide that Needs to be Shown'),
    '#options' => array(
      'full' => t('Entire slide'),
      'vertical' => t('Set amount of vertical'),
      'horizontal' => t('Set amount of horizontal'),
      'area' => t('Set total area of the slide'),
    ),
    '#default_value' => $view->options['views_slideshow_singleframe']['pause_when_hidden_type'],
    '#description' => t('Choose how to calculate how much of the slide has to be shown. Entire Slide: All the slide has to be shown. Vertical: Set amount of height that has to be shown. Horizontal: Set amount of width that has to be shown. Area: Set total area that has to be shown.'),
    '#process' => array('views_process_dependency'),
    '#dependency' => array('edit-style-options-views-slideshow-singleframe-pause-when-hidden' => array(1)),
  );
  $form['views_slideshow_singleframe']['amount_allowed_visible'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount of Slide Needed to be Shown'),
    '#default_value' => $view->options['views_slideshow_singleframe']['amount_allowed_visible'],
    '#description' => t("The amount of the slide that needs to be shown to have it rotate. You can set the value in percentage (ex: 50%) or in pixels (ex: 250). The slidehsow will not rotate until it's height/width/total area, depending on the calculation method you have chosen above, is less than the value you have entered in this field."),
    '#size' => 4,
    '#process' => array('views_process_dependency'),
    '#dependency_count' => 2,
    '#dependency' => array(
      'edit-style-options-views-slideshow-singleframe-pause-when-hidden' => array(1),
      'edit-style-options-views-slideshow-singleframe-pause-when-hidden-type' => array(
        'vertical',
        'horizontal',
        'area',
      ),
    ),
  );
  $form['views_slideshow_singleframe']['remember_slide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start On Last Slide Viewed'),
    '#default_value' => $view->options['views_slideshow_singleframe']['remember_slide'],
    '#description' => t('When the user leaves a page with a slideshow and comes back start them on the last slide viewed.'),
  );
  $form['views_slideshow_singleframe']['remember_slide_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Length of Time to Remember Last Slide'),
    '#default_value' => $view->options['views_slideshow_singleframe']['remember_slide_days'],
    '#description' => t('The number of days to have the site remember the last slide. Default is 1'),
    '#size' => 4,
    '#process' => array('views_process_dependency'),
    '#dependency' => array('edit-style-options-views-slideshow-singleframe-remember-slide' => array(1)),
  );
  $form['views_slideshow_singleframe']['controls'] = array(
    '#type' => 'select',
    '#title' => t('Controls'),
    '#options' => array(0 => t('None'), 1 => t('Top'), 2 => t('Bottom')),
    '#default_value' => $view->options['views_slideshow_singleframe']['controls'],
    '#description' => t('Determine if controls for the slideshow (start/stop/next/previous) should appear, and if so whether they should appear before or after the slideshow.'),
  );
  $form['views_slideshow_singleframe']['pager'] = array(
    '#type' => 'select',
    '#title' => t('Pager'),
    '#options' => array(0 => t('None'), 1 => t('Top'), 2 => t('Bottom')),
    '#default_value' => $view->options['views_slideshow_singleframe']['pager'],
    '#description' => t('Determine if a pager for the slideshow (1|2|3) should appear, and if so whether they should appear before or after the slideshow.'),
  );
  $form['views_slideshow_singleframe']['pager_type'] = array(
    '#type' => 'select',
    '#title' => t('Pager Type'),
    '#options' => array('Numbered' => t('Numbered'), 'Thumbnails' => t('Thumbnails')),
    '#default_value' => $view->options['views_slideshow_singleframe']['pager_type'],
    '#description' => t('What the pager should look like.  Thumbnails grabs the first image in each slideshow item.'),
    '#process' => array('views_process_dependency'),
    '#dependency' => array('edit-style-options-views-slideshow-singleframe-pager' => array(1, 2)),
  );
  $form['views_slideshow_singleframe']['pager_hover'] = array(
    '#type' => 'select',
    '#title' => t('Activate Slide and Pause on Pager Hover'),
    '#options' => array(1 => t('Yes'), 2 => t('No')),
    '#default_value' => $view->options['views_slideshow_singleframe']['pager_hover'],
    '#description' => t('Should the slide be activated and paused when hovering over a pager item.'),
    '#process' => array('views_process_dependency'),
    '#dependency' => array('edit-style-options-views-slideshow-singleframe-pager' => array(1, 2)),
  );
  $form['views_slideshow_singleframe']['pager_click_to_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make Pager Linked to Slide Link'),
    '#default_value' => $view->options['views_slideshow_singleframe']['pager_click_to_page'],
    '#description' => t('Should the pager use the main frame link so when it is clicked it goes to the same url that the main frame goes to when clicked.'),
    '#process' => array('views_process_dependency'),
    '#dependency' => array('edit-style-options-views-slideshow-singleframe-pager' => array(1, 2)),
  );
  $form['views_slideshow_singleframe']['image_count'] = array(
    '#type' => 'select',
    '#title' => t('Image Counter'),
    '#options' => array(0 => t('None'), 1 => t('Top'), 2 => t('Bottom')),
    '#default_value' => $view->options['views_slideshow_singleframe']['image_count'],
    '#description' => t('Determine if the Image Counter (1 of 4) should be displayed and if so whether they should appear before or after the slideshow.'),
  );
  $form['views_slideshow_singleframe']['items_per_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Items per slide'),
    '#default_value' => $view->options['views_slideshow_singleframe']['items_per_slide'],
    '#description' => t('The number of items per slide'),
    '#size' => 4,
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
    'zoom' => 'zoom' 
  );
  $form['views_slideshow_singleframe']['effect'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#options' => $options,
    '#default_value' => $view->options['views_slideshow_singleframe']['effect'],
    '#description' => t('The transition effect that will be used to change between images. Not all options below may be relevant depending on the effect. ' . l('Follow this link to see examples of each effect.', 'http://jquery.malsup.com/cycle/browser.html', array('attributes' => array('target' => '_blank')))),
  );
  $form['views_slideshow_singleframe']['sync'] = array(
    '#type' => 'radios',
    '#title' => t('Sync'),
    '#options' => array(1 => t('Yes'), 2 => t('No')),
    '#default_value' => $view->options['views_slideshow_singleframe']['sync'],
    '#description' => t('The sync option controls whether the slide transitions occur simultaneously. The default is yes which means that the current slide transitions out as the next slide transitions in. By setting the sync option to no you can get some interesting twists on your transitions.'),  
  );
  $form['views_slideshow_singleframe']['nowrap'] = array(
    '#type' => 'select',
    '#title' => t('End slideshow after last slide'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $view->options['views_slideshow_singleframe']['nowrap'],
    '#description' => t('If you select yes the slideshow will end when it gets to the last slide.'),  
  );
  $form['views_slideshow_singleframe']['advanced'] = array(
    '#type' => 'textarea',
    '#title' => t('Advanced Options'),
    '#default_value' => $view->options['views_slideshow_singleframe']['advanced'],
    '#description' => t('Add other jQuery cycle options one per line.  Ex. height: 350  !url', array('!url' => l(t('Click here for the additional options to add.'), 'http://malsup.com/jquery/cycle/options.html'))),
  );
  $form['views_slideshow_singleframe']['ie'] = array(
    '#type' => 'fieldset',
    '#title' => t('Internet Explorer Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['views_slideshow_singleframe']['ie']['cleartype'] = array(
    '#type' => 'radios',
    '#title' => t('ClearType'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
    '#default_value' => $view->options['views_slideshow_singleframe']['ie']['cleartype'],
    '#description' => t('True if clearType corrections should be applied (for IE).  Some background issues could be fixed by setting this to false.'),
  );
  $form['views_slideshow_singleframe']['ie']['cleartypenobg'] = array(
    '#type' => 'radios',
    '#title' => t('ClearType Background'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
    '#default_value' => $view->options['views_slideshow_singleframe']['ie']['cleartypenobg'],
    '#description' => t('Set to true to disable extra cleartype fixing (leave false to force background color setting on slides)'),
  );
}

function views_slideshow_singleframe_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_singleframe']['speed'])) {
    form_error($form['views_slideshow_singleframe']['speed'], t('Speed must be numeric!'));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_singleframe']['timeout'])) {
    form_error($form['views_slideshow_singleframe']['timeout'], t('Timeout must be numeric!'));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_singleframe']['remember_slide_days'])) {
    form_error($form['views_slideshow_singleframe']['remember_slide_days'], t('Slide days must be numeric!'));
  }
}
