<?php
// $Id: lightbox2.admin.inc,v 1.1.2.38 2010/09/24 08:41:02 snpower Exp $

/**
 * @file
 * Administrative page callbacks for the lightbox2 module.
 */

/**
 * The default lightbox2 settings page.
 */
function lightbox2_settings_page($op = NULL) {
  $output = drupal_get_form('lightbox2_general_settings_form');
  return $output;
}

/**
 * General configuration form for controlling the lightbox behaviour.
 */
function lightbox2_general_settings_form() {

  // Add the javascript which disables / enables form elements.
  drupal_add_js(drupal_get_path('module', 'lightbox2') .'/js/lightbox2.js', 'module');

  // Enable translation of default strings for potx.
  $default_strings = array(t('View Image Details'), t('Image !current of !total'), t('Page !current of !total'), t('Video !current of !total'), t('Download Original'));

  // Define Lightbox2 layout fieldset.
  /* --------------------------------- */
  $form['layout_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Define Lightbox2 Lite fieldset.
  /* ------------------------------- */
  $use_lite = variable_get('lightbox2_lite', FALSE);
  $form['layout_fieldset']['lightbox2_lite_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lightbox2 lite'),
    '#collapsible' => TRUE,
    '#collapsed' => !$use_lite,
  );

  // Add checkbox for Lightbox2 Lite.
  $form['layout_fieldset']['lightbox2_lite_options']['lightbox2_lite']  = array(
    '#type' => 'checkbox',
    '#title' => t('Use lightbox2 lite'),
    '#description' => t('Checking this box will enable Lightbox2 Lite and will disable all of the automatic image URL re-formatting features.  It also disables all grouping features.'),
    '#default_value' => $use_lite,
  );


  // Add checkbox for alternative layout.
  $form['layout_fieldset']['lightbox2_use_alt_layout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use alternative layout'),
    '#description' => t('Enabling this option alters the layout of the lightbox elements. In the alternative layout the navigational links appear under the image with the caption text, instead of being overlayed on the image itself.  This doesn\'t apply when using Lightbox Lite.'),
    '#default_value' => variable_get('lightbox2_use_alt_layout', FALSE),
  );

  // Add checkbox for force navigation display.
  $form['layout_fieldset']['lightbox2_force_show_nav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force visibility of navigation links'),
    '#description' => t('When viewing grouped images, the navigational links to the next and previous images are only displayed when you hover over the image.  Checking this box forces these links to be displayed all the time.'),
    '#default_value' => variable_get('lightbox2_force_show_nav', FALSE),
  );

  // Show caption
  $form['layout_fieldset']['lightbox2_show_caption'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show image caption'),
    '#description' => t('Unset this to always hide the image caption (usually the title).'),
    '#default_value' => variable_get('lightbox2_show_caption', TRUE),
  );

  // Add checkbox for "looping through images'.
  $form['layout_fieldset']['lightbox2_loop_items'] = array(
    '#type' => 'checkbox',
    '#title' => t('Continuous galleries'),
    '#description' => t('When viewing grouped images, the Next button on the last image will display the first image, while the Previous button on the first image will display the last image.'),
    '#default_value' => variable_get('lightbox2_loop_items', FALSE),
  );

  // Add checkbox for node link target.
  $form['layout_fieldset']['lightbox2_node_link_target'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open image page in new window'),
    '#description' => t('This controls whether the link to the image page underneath the image is opened in a new window or the current window.'),
    '#default_value' => variable_get('lightbox2_node_link_target', FALSE),
    '#return_value' => '_blank',
  );

  // Define Lightbox2 text settings.
  /* ------------------------------- */
  $form['lightbox2_text_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Add text box for link text to node.
  $form['lightbox2_text_settings']['lightbox2_node_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for image page link'),
    '#description' => t('This is the text that will appear as the link to the image page underneath the image in the lightbox.  Leave this blank for the link not to appear.'),
    '#default_value' => variable_get('lightbox2_node_link_text', 'View Image Details'),
  );

  // Add text box for link text to node.
  $form['lightbox2_text_settings']['lightbox2_download_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for image original link'),
    '#description' => t('This is the text that will appear as the link to the original file underneath the image in the lightbox.  Leave this blank for the link not to appear.  It will only appear for images uploaded via the "image" or "imagefield" modules.  Users will need the "download original image" permission, but also the "view original images" permission if using the "image" module.'),
    '#default_value' => variable_get('lightbox2_download_link_text', 'Download Original'),
  );

  // Add text box for image count for grouping.
  $form['lightbox2_text_settings']['lightbox2_image_count_str'] = array(
    '#type' => 'textfield',
    '#title' => t('Image count text'),
    '#description' => t('This text is used to display the image count underneath the image in the lightbox when image grouping is enabled.  Use !current as a placeholder for the number of the current image and !total for the total number of images in the group.  For example, "Image !current of !total".  Leave blank for text not to appear.'),
    '#default_value' => variable_get('lightbox2_image_count_str', 'Image !current of !total'),
  );

  // Add text box for page count for grouping.
  $form['lightbox2_text_settings']['lightbox2_page_count_str'] = array(
    '#type' => 'textfield',
    '#title' => t('Page count text'),
    '#description' => t('This text is used to display the page count underneath HTML content displayed in the lightbox when using groups.  Use !current as a placeholder for the number of the current page and !total for the total number of pages in the group.  For example, "Page !current of !total".  Leave blank for text not to appear.'),
    '#default_value' => variable_get('lightbox2_page_count_str', 'Page !current of !total'),
  );

  // Add text box for video count for grouping.
  $form['lightbox2_text_settings']['lightbox2_video_count_str'] = array(
    '#type' => 'textfield',
    '#title' => t('Video count text'),
    '#description' => t('This text is used to display the count underneath video content displayed in the lightbox when using groups.  Use !current as a placeholder for the number of the current video and !total for the total number of videos in the group.  For example, "Video !current of !total".  Leave blank for text not to appear.'),
    '#default_value' => variable_get('lightbox2_video_count_str', 'Video !current of !total'),
  );

  // Add text box for video count for grouping.
  $form['lightbox2_text_settings']['lightbox2_filter_xss_allowed_tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed HTML tags'),
    '#description' => t('A list of comma separated HTML tags which are allowed to be used in the caption text area.'),
    '#default_value' => variable_get('lightbox2_filter_xss_allowed_tags', 'p, br, a, em, strong, cite, code, ul, ol, li, dl, dt, dd'),
  );


  // Define Lightbox2 zoom fieldset.
  /* -------------------------------- */
  $form['zoom_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image resize settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Add checkbox for resize image.
  $form['zoom_fieldset']['lightbox2_disable_resize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable resizing feature'),
    '#description' => t('By default, when the image being displayed in the lightbox is larger than the browser window, it is resized to fit within the window and a zoom button is provided for users who wish to view the image in its original size.  Checking this box will disable this feature and all images will be displayed without any resizing.'),
    '#default_value' => variable_get('lightbox2_disable_resize', FALSE),
  );

  // Add checkbox for zoom image.
  $form['zoom_fieldset']['lightbox2_disable_zoom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable zoom in / out feature'),
    '#description' => t('When the image being displayed in the lightbox is resized to fit in the browser window, a "zoom in" button is shown.  This allows the user to zoom in to see the original full size image.  They will then see a "zoom out" button which will allow them to see the smaller resized version.  Checking this box will prevent these buttons from appearing.'),
    '#default_value' => variable_get('lightbox2_disable_zoom', FALSE),
  );

  // Define Lightbox2 modal form fieldset.
  /* -------------------------------------- */
  $form['modal_form_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modal form settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Add checkbox for login support.
  $form['modal_form_fieldset']['lightbox2_enable_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable login support'),
    '#description' => t('Enabling this option will modify all login links so that the login form appears in a lightbox.'),
    '#default_value' => variable_get('lightbox2_enable_login', FALSE),
  );

  // Add checkbox for contact form support.
  $form['modal_form_fieldset']['lightbox2_enable_contact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable contact form support'),
    '#description' => t('Enabling this option will modify all contact links so that the contact form appears in a lightbox.'),
    '#default_value' => variable_get('lightbox2_enable_contact', FALSE),
  );

  // Define Lightbox2 video fieldset.
  /* -------------------------------- */
  $form['video_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Add checkbox for video support.
  $form['video_fieldset']['lightbox2_enable_video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable video support'),
    '#description' => t('By default, video support is disabled in order to reduce the amount of javascript needed.  Checking this box will enable it.'),
    '#default_value' => variable_get('lightbox2_enable_video', FALSE),
  );

  // Add textfield for FLV Player path.
  $form['video_fieldset']['lightbox2_flv_player_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to FLV Player'),
    '#description' => t('The path to the FLV player, relative to Drupal root directory. No leading slashes.'),
    '#default_value' => variable_get('lightbox2_flv_player_path', 'flvplayer.swf'),
  );

  // Add textfield for FLV Player flashvars.
  $form['video_fieldset']['lightbox2_flv_player_flashvars'] = array(
    '#type' => 'textfield',
    '#title' => t('FLV Player flashvars'),
    '#description' => t('Flashvars for the FLV Player where supported, e.g. "autoplay=1&playerMode=normal".'),
    '#default_value' => variable_get('lightbox2_flv_player_flashvars', ''),
  );

  // Define Lightbox2 page specific settings fieldset.
  /* ------------------------------------------------ */
  $form['lightbox2_page_init'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific lightbox2 settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add radio buttons for the actions to take for the listed pages, i.e.
  // disable or enable the lightbox functionality.
  $page_options = array(
    'page_enable' => t('Load only on the listed pages.'),
    'page_disable' => t('Load on every page except the listed pages.'),
  );
  $form['lightbox2_page_init']['lightbox2_page_init_action'] = array(
    '#type' => 'radios',
    '#options' => $page_options,
    '#title' => t('Enable lightbox2 on specific pages'),
    '#default_value' => variable_get('lightbox2_page_init_action', 'page_disable'),
  );
  // Add text input for list of pages to take specific action on.
  $form['lightbox2_page_init']['lightbox2_page_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#description' => t('List one page per line as Drupal paths.  The * character is a wildcard.  Example paths are "node/add/page" and "node/add/*".  Use &lt;front&gt; to match the front page.'),
    '#default_value' => variable_get('lightbox2_page_list', ''),
  );

  // Define cck settings fieldset.
  /* ------------------------------------ */
  if ((module_exists('emfield') && module_exists('emimage')) || (module_exists('imagefield') && module_exists('imagecache'))) {
    $form['cck_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('CCK display settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $cck_options = array(
      0 => t('No grouping'),
      1 => t('Group by field name'),
      2 => t('Group by node id'),
      3 => t('Group by field name and node id'),
      4 => t('Group all nodes and fields'),
    );

    // Add checkbox for emfield emimage grouping.
    if (module_exists('emfield') && module_exists('emimage')) {
      $form['cck_options']['lightbox2_image_ncck_group_node_id'] = array(
        '#type' => 'select',
        '#title' => t('Select Emfield Image grouping in Views'),
        '#description' => t('By default, emfield images in views are grouped by the field name they appear in the view in.  You can override that grouping here.'),
        '#options' => $cck_options,
        '#default_value' => variable_get('lightbox2_image_ncck_group_node_id', 1),
      );
    }

    // Add checkbox for imagefield grouping.
    if (module_exists('imagefield') && module_exists('imagecache')) {
      $form['cck_options']['lightbox2_imagefield_group_node_id'] = array(
        '#type' => 'select',
        '#title' => t('Select Imagefield grouping in Views'),
        '#description' => t('By default, imagefields in views are grouped by the field name they appear in the view in.  You can override that grouping here.'),
        '#options' => $cck_options,
        '#default_value' => variable_get('lightbox2_imagefield_group_node_id', 1),
      );
      $form['cck_options']['lightbox2_imagefield_use_node_title'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use node title as caption'),
        '#description' => t('By default, the caption for imagefields is the image title text configured.  If no title is configured, then the alt text will be used.  This option allows you to override that and always display the node title as the image caption.'),
        '#default_value' => variable_get('lightbox2_imagefield_use_node_title', FALSE),
      );
      $form['cck_options']['lightbox2_view_image_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Text to view image'),
        '#description' => t('This is the text that can appear when linking to the image using CCK imagefields.'),
        '#default_value' => variable_get('lightbox2_view_image_text', 'View image'),
      );

    }
  }


  // Define advanced settings fieldset.
  /* ---------------------------------- */
  $form['advanced_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Allow users to decide where javascript should be loaded - header or footer.
  // Header is recommended so user can click on images before page has finished
  // loading, but footer is needed for sites in IE which use SWFObject.
  $form['advanced_options']['lightbox2_js_location'] = array(
    '#type' => 'select',
    '#title' => t('Location of javscript'),
    '#options' => array('header' => t('Header'), 'footer' => t('Footer')),
    '#description' => t('By default, the lightbox javascript files are loaded in the HTML header.  However, for sites using SWFObject to load their Flash content, the footer setting is recommended to prevent "Operation Aborted" errors in IE.  If using the footer setting, please note that not all themes correctly implement the footer region and may require a small change.'),
    '#default_value' => variable_get('lightbox2_js_location', 'header'),
  );

  $form['advanced_options']['lightbox2_disable_close_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Click on overlay or lightbox to close it'),
    '#description' => t('Enable user to close lightbox by clicking on the lightbox itself or the overlay background.'),
    '#default_value' => variable_get('lightbox2_disable_close_click', TRUE),
  );

  // Define keyboard shortcuts fieldset.
  /* ---------------------------------- */
  $form['advanced_options']['keyboard_shortcuts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Keyboard shortcuts'),
    '#description' => t('Configure the keyboard shortcuts for controlling the lightbox.  These options do not apply to the Lightbox2 Lite version, which uses the default "Close keys" (c, x and esc) to close the lightbox.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced_options']['keyboard_shortcuts']['lightbox2_keys_close'] = array(
    '#type' => 'textfield',
    '#title' => t('Close keys'),
    '#description' => t("A list of keys (or key codes) that a user may use to close the lightbox. Values should be separated by a space. Defaults to 'c x 27' (c, x, or esc)."),
    '#default_value' => variable_get('lightbox2_keys_close', 'c x 27'),
  );

  $form['advanced_options']['keyboard_shortcuts']['lightbox2_keys_previous'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous keys'),
    '#description' => t("A list of keys (or key codes) that a user may use to navigate to the previous item in the lightbox. Values should be separated by a space. Defaults to 'p 37' (p or left arrow)."),
    '#default_value' => variable_get('lightbox2_keys_previous', 'p 37'),
  );

  $form['advanced_options']['keyboard_shortcuts']['lightbox2_keys_next'] = array(
    '#type' => 'textfield',
    '#title' => t('Next keys'),
    '#description' => t("A list of keys (or key codes) that a user may use to navigate to the next item in the lightbox. Values should be separated by a space. Defaults to 'n 39' (n or right arrow)."),
    '#default_value' => variable_get('lightbox2_keys_next', 'n 39'),
  );

  $form['advanced_options']['keyboard_shortcuts']['lightbox2_keys_zoom'] = array(
    '#type' => 'textfield',
    '#title' => t('Zoom keys'),
    '#description' => t("A list of keys (or key codes) that a user may use to zoom in / out of images in the lightbox. Values should be separated by a space. Defaults to 'z'."),
    '#default_value' => variable_get('lightbox2_keys_zoom', 'z'),
  );

  $form['advanced_options']['keyboard_shortcuts']['lightbox2_keys_play_pause'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause / play keys'),
    '#description' => t("A list of keys (or key codes) that a user may use to pause / play the lightbox. Values should be separated by a space. Defaults to '32' (spacebar)."),
    '#default_value' => variable_get('lightbox2_keys_play_pause', '32'),
  );




  // Define border settings fieldset.
  /* ---------------------------------- */
  $form['advanced_options']['skin_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Skin settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced_options']['skin_settings']['lightbox2_border_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Border size'),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Enter the size of the border in pixels to display around the image.'),
    '#default_value' => variable_get('lightbox2_border_size', 10),
  );

  $form['advanced_options']['skin_settings']['lightbox2_box_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Lightbox color'),
    '#field_prefix' => '#',
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Enter a hexadecimal color value for the border.  For example <code>fff</code> or <code>ffffff</code> for white).'),
    '#default_value' => variable_get('lightbox2_box_color', 'fff'),
  );

  $form['advanced_options']['skin_settings']['lightbox2_font_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Font color'),
    '#field_prefix' => '#',
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Enter a hexadecimal color value for the font.  For example <code>000</code> or <code>000000</code> for black).'),
    '#default_value' => variable_get('lightbox2_font_color', '000'),
  );

  $form['advanced_options']['skin_settings']['lightbox2_top_position'] = array(
    '#type' => 'textfield',
    '#title' => t('Distance from top'),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Enter the position of the top of the lightbox in pixels.  Leave blank for automatic calculation.'),
    '#default_value' => variable_get('lightbox2_top_position', ''),
  );



  // Define overlay settings fieldset.
  /* ---------------------------------- */
  $form['advanced_options']['overlay_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overlay settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add checkbox for overlay opacity.
  for ($i = 0; $i < 10; $i++) {
    $opacity_options["0.$i"] = "0.$i";
  }
  $opacity_options["1.0"] = "1.0";
  $form['advanced_options']['overlay_settings']['lightbox2_overlay_opacity'] = array(
    '#type' => 'select',
    '#title' => t('Overlay opacity'),
    '#options' => $opacity_options,
    '#description' => t('The overlay opacity setting determines how visible the background page is behind the lightbox.  The opacity value can range from 0.0 to 1.0 where 0.0 is 100% transparent and 1.0 is 100% opaque.'),
    '#default_value' => variable_get('lightbox2_overlay_opacity', 0.8),
  );

  $form['advanced_options']['overlay_settings']['lightbox2_overlay_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Overlay color'),
    '#field_prefix' => '#',
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Enter a hexadecimal color value for the overlay.  For example <code>000</code> or <code>000000</code> for black).'),
    '#default_value' => variable_get('lightbox2_overlay_color', '000'),
  );


  // Define animation settings fieldset.
  /* ----------------------------------- */
  $form['advanced_options']['animation_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Animation settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Set animation help text.
  $form['advanced_options']['animation_settings']['animation_options_help'] = array(
    '#value' => '<p>'. t("These options aren't available when using Lightbox2 Lite.") .'</p>',
  );
  $resize_sequence_options = array(
    t('Simultaneous'),
    t('Width then height'),
    t('Height then width'),
  );
  $form['advanced_options']['animation_settings']['lightbox2_resize_sequence'] = array(
    '#type' => 'select',
    '#title' => t('Resize sequence'),
    '#options' => $resize_sequence_options,
    '#description' => t('The sequence to use for the resizing animation.'),
    '#default_value' => variable_get('lightbox2_resize_sequence', 0),
  );
  $form['advanced_options']['animation_settings']['lightbox2_resize_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Resize duration'),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The duration (in seconds) of the resizing animation.  Enter a value between 0 and 10.'),
    '#default_value' => variable_get('lightbox2_resize_speed', 0.4),
  );
  $form['advanced_options']['animation_settings']['lightbox2_fadein_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Appearance duration'),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The duration (in seconds) of the lightbox appearance animation.  Enter a positive number less than 10.'),
    '#default_value' => variable_get('lightbox2_fadein_speed', 0.4),
  );


  $form['advanced_options']['animation_settings']['lightbox2_slidedown_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption slide down duration'),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The duration (in seconds) of the caption sliding-down animation.  Enter a value between 0 and 10.'),
    '#default_value' => variable_get('lightbox2_slidedown_speed', 0.6),
  );

  return system_settings_form($form);
}

/**
 * Validation function for the general configuration form.
 *
 * Ensure that the hex color value submitted is valid and that the speeds are
 * numeric.
 */
function lightbox2_general_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Save configuration')) {
    $border_size = $form_state['values']['lightbox2_border_size'];
    $box_hex_colour = $form_state['values']['lightbox2_box_color'];
    $font_hex_colour = $form_state['values']['lightbox2_font_color'];
    $top_position = $form_state['values']['lightbox2_top_position'];
    $overlay_hex_colour = $form_state['values']['lightbox2_overlay_color'];
    $resize_speed = $form_state['values']['lightbox2_resize_speed'];
    $fadein_speed = $form_state['values']['lightbox2_fadein_speed'];
    $slide_down_speed = $form_state['values']['lightbox2_slidedown_speed'];
    $flv_player_path = $form_state['values']['lightbox2_flv_player_path'];

    if (!empty($flv_player_path) && $form_state['values']['lightbox2_enable_video']) {
      if (strpos($flv_player_path, base_path()) === 0) {
        $flv_player_path = drupal_substr($flv_player_path, drupal_strlen(base_path()));
      }
      if (!file_exists($flv_player_path)) {
        form_set_error('lightbox2_flv_player_path', t("FLV player path doesn't exist."));
      }
    }

    if (!_lightbox2_validate_hex_color($overlay_hex_colour)) {
      form_set_error('lightbox2_overlay_color', t('You must enter a properly formed hex value.'));
    }

    if (!$form_state['values']['lightbox2_lite']) {
      if (!is_numeric($border_size) || $border_size < 0) {
        form_set_error('lightbox2_border_size', t('You must enter a size greater than 0 pixels.'));
      }

      if (!_lightbox2_validate_hex_color($box_hex_colour)) {
        form_set_error('lightbox2_box_color', t('You must enter a properly formed hex value.'));
      }

      if (!_lightbox2_validate_hex_color($font_hex_colour)) {
        form_set_error('lightbox2_font_color', t('You must enter a properly formed hex value.'));
      }

      if (!empty($top_position) && (!is_numeric($top_position) || $top_position < 0)) {
        form_set_error('lightbox2_top_position', t('You must enter a size greater than 0 pixels.  Leave blank for default positioning.'));
      }

      if (!is_numeric($resize_speed) || $resize_speed <= 0 || $resize_speed >= 10) {
        form_set_error('lightbox2_resize_speed', t('You must enter a duration between 0 and 10 seconds.'));
      }

      if (!is_numeric($fadein_speed) || $fadein_speed < 0 || $fadein_speed >= 10) {
        form_set_error('lightbox2_fadein_speed', t('You must enter a duration between 0 and 10 seconds.'));
      }

      if (!is_numeric($slide_down_speed) || $slide_down_speed <= 0 || $slide_down_speed >= 10) {
        form_set_error('lightbox2_slidedown_speed', t('You must enter a duration between 0 and 10 seconds.'));
      }
    }
  }
}

/**
 * Submit function for the general configuration form.
 */
function lightbox2_general_settings_form_submit($form, &$form_state) {
  cache_clear_all();
}

/**
 * Configuration form for controlling the slideshow behaviour.
 */
function lightbox2_slideshow_settings_form() {

  // Add the javascript which disables / enables form elements.
  drupal_add_js(drupal_get_path('module', 'lightbox2') .'/js/lightbox2.js', 'module');

  // Set up a hidden variable.
  $form['lightbox2_lite'] = array(
    '#type' => 'hidden',
    '#value' => variable_get('lightbox2_lite', FALSE),
  );


  // Add text box for slideshow interval.
  $form['lightbox2_slideshow_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Interval seconds'),
    '#description' => t('The slideshow interval is the length of time in seconds an image is visible before the slideshow shows the next image.'),
    '#default_value' => variable_get('lightbox2_slideshow_interval', 5),
    '#size' => 20,
  );

   // Add checkbox for slideshow automatic start.
  $form['lightbox2_slideshow_automatic_start'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically start slideshow'),
    '#description' => t('When enabled the slideshow will automatically start.'),
    '#default_value' => variable_get('lightbox2_slideshow_automatic_start', TRUE),
  );

  // Add checkbox for slideshow automatic exit.
  $form['lightbox2_slideshow_automatic_exit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically exit slideshow'),
    '#description' => t('When enabled the lightbox will automatically close after displaying the last image.'),
    '#default_value' => variable_get('lightbox2_slideshow_automatic_exit', TRUE),
  );

  // Add checkbox for showing hte play / pause button.
  $form['lightbox2_slideshow_show_play_pause'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show play / pause button'),
    '#description' => t('When enabled, a play / pause button will be shown in the slideshow allowing the user more control over their viewing experience.'),
    '#default_value' => variable_get('lightbox2_slideshow_show_play_pause', TRUE),
  );

  // Add checkbox for "pausing on next click".
  $form['lightbox2_slideshow_pause_on_next_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause slideshow on "Next Image" click'),
    '#description' => t('When enabled the slideshow is automatically paused, and the following image shown, when the "Next" button is clicked.'),
    '#default_value' => variable_get('lightbox2_slideshow_pause_on_next_click', FALSE),
  );

  // Add checkbox for "pausing on prev click".
  $form['lightbox2_slideshow_pause_on_previous_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause slideshow on "Previous Image" click'),
    '#description' => t('When enabled the slideshow is automatically paused, and the previous image shown, when the "Previous" button is clicked.'),
    '#default_value' => variable_get('lightbox2_slideshow_pause_on_previous_click', TRUE),
  );

  // Add checkbox for "looping through slides'.
  $form['lightbox2_loop_slides'] = array(
    '#type' => 'checkbox',
    '#title' => t('Continuous loop'),
    '#description' => t('When enabled the slideshow will automatically start over after displaying the last slide.  This prevents the slideshow from automatically exiting when enabled.'),
    '#default_value' => variable_get('lightbox2_loop_slides', FALSE),
  );

  return system_settings_form($form);
}

/**
 * Validation function for the slideshow configuration form.
 *
 * Ensure that the slideshow interval submitted is numeric.
 */
function lightbox2_slideshow_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['lightbox2_slideshow_interval'])) {
    form_set_error('lightbox2_slideshow_interval', t('The "interval seconds" value must be numeric.'));
  }
}

/**
 * Configuration form for controlling the iframe behaviour.
 */
function lightbox2_iframe_settings_form() {

  // Add the javascript which disables / enables form elements.
  drupal_add_js(drupal_get_path('module', 'lightbox2') .'/js/lightbox2.js', 'module');

  // Set up a hidden variable.
  $form['lightbox2_lite'] = array(
    '#type' => 'hidden',
    '#value' => variable_get('lightbox2_lite', FALSE),
  );


  // Add text box for iframe width.
  $form['lightbox2_default_frame_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Default width'),
    '#description' => t('The default width of the iframe in pixels.'),
    '#default_value' => variable_get('lightbox2_default_frame_width', 600),
    '#size' => 20,
  );

  // Add text box for iframe height.
  $form['lightbox2_default_frame_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Default height'),
    '#description' => t('The default height of the iframe in pixels.'),
    '#default_value' => variable_get('lightbox2_default_frame_height', 400),
    '#size' => 20,
  );

  // Add option for iframe border.
  $form['lightbox2_frame_border'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable border'),
    '#description' => t('Enable iframe border.  You can modify the border style in your theme\'s css file using the iframe\'s id "lightboxFrame".'),
    '#default_value' => variable_get('lightbox2_frame_border', 1),
  );

  return system_settings_form($form);
}

/**
 * Validation function for the iframe configuration form.
 *
 * Ensure that the iframe width and height submitted are numeric.
 */
function lightbox2_iframe_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['lightbox2_default_frame_width'])) {
    form_set_error('lightbox2_slideshow_interval', t('The "default width" value must be numeric.'));
  }
  if (!is_numeric($form_state['values']['lightbox2_default_frame_height'])) {
    form_set_error('lightbox2_slideshow_interval', t('The "default height" value must be numeric.'));
  }
}


/**
 * Configuration form the automatic image handling functionality.
 */
function lightbox2_auto_image_handling_settings_form() {

  // Add the javascript which disables / enables form elements.
  drupal_add_js(drupal_get_path('module', 'lightbox2') .'/js/lightbox2.js', 'module');

  // Set up a hidden variable.
  $form['lightbox2_lite'] = array(
    '#type' => 'hidden',
    '#value' => variable_get('lightbox2_lite', FALSE),
  );

  $automatic_options = array(
    0 => t('Disabled'),
    1 => t('Lightbox'),
    2 => t('Lightbox grouped'),
    3 => t('Slideshow'),
    4 => t('HTML content'),
    5 => t('HTML content grouped'),
  );

  // Set image node options help text.
  $form['image_node_options'] = array(
    '#value' => t('These options allow automatic URL re-formatting of images.  This removes the need for you to add \'rel="lightbox"\' to each image link throughout your site.  You can select which image sizes will trigger the lightbox and configure a list of image CSS classes which should also have their URLs automatically re-formatted.  This feature is not available when using Lightbox2 Lite.'),
  );

  // Define handler settings fieldset.
  /* --------------------------------- */
  $form['handler_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatic handlers'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Define image nodes settings fieldset.
  /* ------------------------------------- */
  $form['handler_options']['image_node_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image node settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Add checkbox for image nodes.
  $form['handler_options']['image_node_options']['lightbox2_image_node']  = array(
    '#type' => 'select',
    '#title' => t('Automatic handler for image nodes'),
    '#options' => $automatic_options,
    '#description' => t('Choose how URLs for image nodes will be automatically handled.'),
    '#default_value' => variable_get('lightbox2_image_node', 0),
  );

  // Add drop-down for list of available image sizes.
  if (module_exists('image')) {
    $sizes = _image_get_sizes();
    foreach ($sizes as $size_key => $size) {
      if ($size_key == 'original' or $size_key == '_original')  {
        $size_key = 'original';
      }
      $size_options[$size_key] = $size['label'];
    }
    $form['handler_options']['image_node_options']['lightbox2_display_image_size'] = array(
      '#type' => 'select',
      '#title' => t('Lightbox image display size'),
      '#options' => $size_options,
      '#default_value' => variable_get('lightbox2_display_image_size', 'original'),
      '#description' => t('Select which image size will be loaded in the lightbox.  This only applies to images uploaded with the Image module.'),
    );

    $form['handler_options']['image_node_options']['lightbox2_trigger_image_size'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Image trigger size'),
      '#options' => $size_options,
      '#default_value' => variable_get('lightbox2_trigger_image_size', array('thumbnail')),
      '#description' => t('Select which image size, when clicked on, will automatically trigger the lightbox.  This only applies to images uploaded with the Image module.'),
    );

  }

  // Add checkbox for disabling lightbox for gallery lists.
  $form['handler_options']['image_node_options']['lightbox2_disable_nested_galleries']  = array(
    '#type' => 'checkbox',
    '#title' => t('Disable lightbox for gallery lists'),
    '#description' => t('Checking this box will disable the lightbox for images in gallery lists.  This means it is possible to open a gallery by clicking on the teaser image, but the lightbox will still appear when viewing images within the gallery.  This only applies to image galleries created with the "image gallery" module.'),
    '#default_value' => variable_get('lightbox2_disable_nested_galleries', TRUE),
  );

  // Define flickr fieldset.
  /* ----------------------- */
  $form['handler_options']['flickr_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr images'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add checkbox for flickr image support.
  $form['handler_options']['flickr_options']['lightbox2_flickr'] = array(
    '#type' => 'select',
    '#title' => t('Automatic handler for Flickr images'),
    '#options' => $automatic_options,
    '#description' => t('Choose how URLs for Flickr images will be automatically handled.'),
    '#default_value' => variable_get('lightbox2_flickr', 0),
  );

  // Define gallery2 fieldset.
  /* ------------------------- */
  $form['handler_options']['gallery2_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery2 images'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add checkbox for gallery2 block image support.
  $form['handler_options']['gallery2_options']['lightbox2_gallery2_blocks'] = array(
    '#type' => 'select',
    '#title' => t('Automatic handler for Gallery2 block images'),
    '#options' => $automatic_options,
    '#description' => t('Choose how URLs for images, contained within Gallery2 image blocks, will be automatically handled.'),
    '#default_value' => variable_get('lightbox2_gallery2_blocks', 0),
  );

  // Define inline fieldset.
  /* ----------------------- */
  $form['handler_options']['inline_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inline module images'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add checkbox for inline image support.
  $form['handler_options']['inline_options']['lightbox2_inline'] = array(
    '#type' => 'select',
    '#title' => t('Automatic handler for Inline module images'),
    '#options' => $automatic_options,
    '#description' => t('Choose how URLs for Inline module images will be automatically handled.'),
    '#default_value' => variable_get('lightbox2_inline', 0),
  );

  // Define image assist fieldset.
  /* ----------------------------- */
  $form['handler_options']['img_assist_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image Assist images'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add checkbox for image assist custom size images.
  $form['handler_options']['img_assist_options']['lightbox2_image_assist_custom'] = array(
    '#type' => 'select',
    '#title' => t('Automatic handler for Image Assist custom size images'),
    '#options' => $automatic_options,
    '#description' => t('Choose how URLs for custom size images, displayed by the Image Assist module, will be automatically handled.'),
    '#default_value' => variable_get('lightbox2_image_assist_custom', 0),
  );

  // Define custom fieldset.
  /* ----------------------- */
  $form['handler_options']['custom_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom class images'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add checkbox for custom class image support.
  $form['handler_options']['custom_options']['lightbox2_custom_class_handler'] = array(
    '#type' => 'select',
    '#title' => t('Automatic handler for custom class images'),
    '#options' => $automatic_options,
    '#description' => t('Choose how URLs for custom class images will be automatically handled.'),
    '#default_value' => variable_get('lightbox2_custom_class_handler', 0),
  );
  // Add text box for custom trigger classes.
  $form['handler_options']['custom_options']['lightbox2_custom_trigger_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom image trigger classes'),
    '#description' => t('List the image classes which should trigger the lightbox when clicked on.  Put each class on a separate line.'),
    '#default_value' => variable_get('lightbox2_custom_trigger_classes', ''),
  );

  // Add checkbox for disabling lightbox for acidfree gallery lists.
  if (module_exists("acidfree")) {
    // Define acidfree settings fieldset.
    /* ---------------------------------- */
    $form['handler_options']['lightbox2_acidfree_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Acidfree settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['handler_options']['lightbox2_acidfree_options']['lightbox2_disable_nested_acidfree_galleries'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable lightbox for Acidfree gallery lists'),
      '#description' => t('Checking this box will disable the lightbox for images in gallery lists.  This means it is possible to open a gallery by clicking on the teaser image, but the lightbox will still appear when viewing images within the gallery.  This only applies to image galleries created with the "acidfree" module.'),
      '#default_value' => variable_get('lightbox2_disable_nested_acidfree_galleries', TRUE),
    );
    // Add checkbox for enabling lightbox for acidfree videos.
    if (module_exists("video")) {
      $form['handler_options']['lightbox2_acidfree_options']['lightbox2_enable_acidfree_videos'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable lightbox for Acidfree videos'),
        '#description' => t('Checking this box will enable the display of acidfree videos in a lightbox.  This only applies to videos created by the "video" module and which appear in an acidfree gallery.'),
        '#default_value' => variable_get('lightbox2_enable_acidfree_videos', FALSE),
      );
    }
  }

  return system_settings_form($form);
}

/**
 * Validate a hex color value.
 *
 * @param $hex
 *   The hex color string to validate.
 * @return
 *   TRUE if $hex is a valid hex color value.
 */
function _lightbox2_validate_hex_color($hex) {
  return preg_match('!^[a-f0-9]{3}([a-f0-9]{3})?$!i', $hex);
}

