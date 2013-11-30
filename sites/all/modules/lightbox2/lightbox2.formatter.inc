<?php
// $Id: lightbox2.formatter.inc,v 1.1.2.30 2010/09/22 10:47:15 snpower Exp $

/**
 * @file
 * Lightbox2 formatter hooks and callbacks.
 */


/**
 * Theme function for displaying the lightbox2 trigger image in an imagefield.
 *
 * @param $path
 *   The path to the image to be displayed.
 * @param $alt
 *   The image alternative text.
 * @param $title
 *   The image title.
 * @param $attributes
 *   An array of image attributes, e.g. class name.
 * @return
 *   HTML output for displaying the image.
 */
function theme_lightbox2_image($path, $alt = '', $title = '', $attributes = array()) {
  $attributes_html = drupal_attributes($attributes);
  $image = '<img src="'. file_create_url($path) .'" alt="'.  check_plain($alt) .'" title="'. check_plain($title) .'" '. $attributes_html .' />';
  return $image;
}

/**
 * Handler for Lightbox2 display of imagecache + imagefield CCK fields.
 *
 * The compact view shows only the first image in a multiple imagefield but
 * shows all images in the lightbox.
 *
 * @param $element
 *   The CCK field element.
 * @return
 *   HTML output for displaying the image and link.
 */
function theme_lightbox2_formatter_imagefield($element) {
  if (!module_exists("imagecache") || !module_exists("imagefield")) {
    return;
  }

  $field_name = $element['#field_name'];
  $item = $element['#item'];
  $formatter = $element['#formatter'];
  $node = node_load($element['#item']['nid']);

  if (strpos($formatter, '__lightbox2__') !== FALSE || strpos($formatter, '__lightshow2__') !== FALSE || strpos($formatter, '__lightframe2__') !== FALSE
    || strpos($formatter, '__lightbox2_compact__') !== FALSE || strpos($formatter, '__lightshow2_compact__') !== FALSE || strpos($formatter, '__lightframe2_compact__') !== FALSE) {
    list($tmp, $lightbox_type, $view_preset, $lightbox_preset) = explode('__', $formatter, 4);
    return lightbox2_imagefield_image_imagecache($field_name, $item, $formatter, $node, $view_preset, $lightbox_preset);
  }
}

/**
 * Function to set up the data needed for
 * theme_imagefield_image_imagecache_lightbox2().
 *
 * @param $field_name
 *   The field name the action is being performed on.
 * @param $item
 *   An array, keyed by column, of the data stored for this item in this field.
 * @param $formatter
 *   The formatter to use for the field.
 * @param $node
 *   The node object.
 * @param $view_preset
 *   The imagecache preset to be displayed on the node or in the view.
 * @param $lightbox_preset
 *   The imagecache preset to be displayed in the lightbox.
 * @return
 *   The themed imagefield + imagecache image and link.
 */
function lightbox2_imagefield_image_imagecache($field_name, $item, $formatter, $node, $view_preset, $lightbox_preset, $args = array()) {
  // Load file data if missing
  if (!isset($item['filepath']) && !empty($item['fid'])) {
    $file = field_file_load($item['fid']);
    $item = array_merge($item, (array)$file);
  }
  elseif (!isset($item['filepath'])) {
    // No file in field, return.
    return '';
  }

  $args['lightbox_preset'] = $lightbox_preset;

  // Check view_preset for existence.
  $rules = array();
  if (function_exists('imagecache_presets')) {
    $presets = imagecache_presets();
    foreach ($presets as $preset_id => $preset_info) {
      $rules[$preset_id] = $preset_info['presetname'];
    }
  }
  else {
    $rules = _imagecache_get_presets();
  }
  if ($view_preset == 'link' || $view_preset == 'original' || in_array($view_preset, (array) $rules)) {
    $rel = 'lightbox';
    if (strpos($formatter, '__lightshow2__') !== FALSE) {
      $rel = 'lightshow';
    }
    elseif (strpos($formatter, '__lightshow2_compact__') !== FALSE) {
      $rel = 'lightshow';
      $args['compact'] = TRUE;
    }
    elseif (strpos($formatter, '__lightframe2__') !== FALSE) {
      $rel = 'lightframe';
    }

    // Check if this is a compact display.
    list($tmp, $lightbox_type, $tmp) = explode('__', $formatter, 3);
    if ($lightbox_type == 'lightbox2_compact') {
      $args['compact'] = TRUE;
    }
    return theme('imagefield_image_imagecache_lightbox2', $view_preset, $field_name, $item, $node, $rel, $args);
  }
}

/**
 * Generate the HTML output for imagefield + imagecache images so they can be
 * opened in a lightbox by clicking on the image on the node page or in a view.
 *
 * This actually also handles filefields + imagecache images too.
 *
 * @param $view_preset
 *   The imagecache preset to be displayed on the node or in the view.
 * @param $field_name
 *   The field name the action is being performed on.
 * @param $item
 *   An array, keyed by column, of the data stored for this item in this field.
 * @param $node
 *   The node object.
 * @param $rel
 *   The type of lightbox to open: lightbox, lightshow or lightframe.
 * @param $args
 *   Args may override internal processes: caption, rel_grouping.
 * @return
 *   The themed imagefield + imagecache image and link.
 */
function theme_imagefield_image_imagecache_lightbox2($view_preset, $field_name, $item, $node, $rel = 'lightbox', $args = array()) {
  if (!isset($args['lightbox_preset'])) {
    $args['lightbox_preset'] = 'original';
  }
  // Can't show current node page in a lightframe on the node page.
  // Switch instead to show it in a lightbox.
  $on_image_node = (arg(0) == 'node') && (arg(1) == $node->nid);
  if ($rel == 'lightframe' && $on_image_node) {
    $rel = 'lightbox';
  }
  $orig_rel = $rel;

  // Unserialize into original - if sourced by views.
  $item_data = $item['data'];
  if (is_string($item['data'])) {
    $item_data = unserialize($item['data']);
  }

  // Set up the title.
  $image_title = $item_data['description'];
  $image_title = (!empty($image_title) ? $image_title : $item_data['title']);
  $image_title = (!empty($image_title) ? $image_title : $item_data['alt']);
  if (empty($image_title) || variable_get('lightbox2_imagefield_use_node_title', FALSE)) {
    $node = node_load($node->nid);
    $image_title = $node->title;
  }

  $image_tag_title = '';
  if (!empty($item_data['title'])) {
    $image_tag_title = $item_data['title'];
  }


  // Enforce image alt.
  $image_tag_alt = '';
  if (!empty($item_data['alt'])) {
    $image_tag_alt = $item_data['alt'];
  }
  elseif (!empty($image_title)) {
    $image_tag_alt = $image_title;
  }

  // Set up the caption.
  $node_links = array();
  if (!empty($item['nid'])) {
    $attributes = array();
    $attributes['id'] = 'lightbox2-node-link-text';
    $target = variable_get('lightbox2_node_link_target', FALSE);
    if (!empty($target)) {
      $attributes['target'] = $target;
    }
    $node_link_text = variable_get('lightbox2_node_link_text', 'View Image Details');
    if (!$on_image_node && !empty($node_link_text)) {
      $node_links[] = l($node_link_text, 'node/'. $item['nid'], array('attributes' => $attributes));
    }
    $download_link_text = check_plain(variable_get('lightbox2_download_link_text', 'Download Original'));
    if (!empty($download_link_text) && user_access('download original image')) {
      $node_links[] = l($download_link_text, file_create_url($item['filepath']), array('attributes' => array('target' => '_blank', 'id' => 'lightbox2-download-link-text')));
    }
  }

  $caption = $image_title;
  if (count($node_links)) {
    $caption .= '<br /><br />'. implode(" - ", $node_links);
  }
  if (isset($args['caption'])) {
    $caption = $args['caption']; // Override caption.
  }

  if ($orig_rel == 'lightframe') {
    $frame_width = variable_get('lightbox2_default_frame_width', 600);
    $frame_height = variable_get('lightbox2_default_frame_height', 400);
    $frame_size = 'width:'. $frame_width .'px; height:'. $frame_height .'px;';
    $rel = preg_replace('/\]$/', "|$frame_size]", $rel);
  }

  // Set up the rel attribute.
  $full_rel = '';
  $imagefield_grouping = variable_get('lightbox2_imagefield_group_node_id', 1);
  if (isset($args['rel_grouping'])) {
    $full_rel = $rel .'['. $args['rel_grouping'] .']['. $caption .']';
  }
  elseif ($imagefield_grouping == 1) {
    $full_rel = $rel .'['. $field_name .']['. $caption .']';
  }
  elseif ($imagefield_grouping == 2 && !empty($item['nid'])) {
    $full_rel = $rel .'['. $item['nid'] .']['. $caption .']';
  }
  elseif ($imagefield_grouping == 3 && !empty($item['nid'])) {
    $full_rel = $rel .'['. $field_name .'_'. $item['nid'] .']['. $caption .']';
  }
  elseif ($imagefield_grouping == 4 ) {
    $full_rel = $rel .'[allnodes]['. $caption .']';
  }
  else {
    $full_rel = $rel .'[]['. $caption .']';
  }
  $class = '';
  if ($view_preset != 'original') {
    $class = 'imagecache imagecache-' . $field_name . ' imagecache-' . $view_preset . ' imagecache-' . $field_name . '-' . $view_preset;
  }
  $link_attributes = array(
    'rel' => $full_rel,
    'class' => 'imagefield imagefield-lightbox2 imagefield-lightbox2-' . $view_preset . ' imagefield-' . $field_name . ' ' . $class,
  );

  $attributes = array();
  if (isset($args['compact']) && $item['#delta']) {
    $image = '';
  }
  elseif ($view_preset == 'original') {
    $image = theme('lightbox2_image', $item['filepath'], $image_tag_alt, $image_tag_title, $attributes);
  }
  elseif ($view_preset == 'link') {
    // Not actually an image, just a text link.
    $image = variable_get('lightbox2_view_image_text', 'View image');
  }
  else {
    $image = theme('imagecache', $view_preset, $item['filepath'], $image_tag_alt, $image_tag_title, $attributes);
  }

  if ($args['lightbox_preset'] == 'node') {
    $output = l($image, 'node/'. $node->nid .'/lightbox2', array('attributes' => $link_attributes, 'html' => TRUE));
  }
  elseif ($args['lightbox_preset'] == 'original') {
    $output = l($image, file_create_url($item['filepath']), array('attributes' => $link_attributes, 'html' => TRUE));
  }
  else {
    $output = l($image, imagecache_create_url($args['lightbox_preset'], $item['filepath']), array('attributes' => $link_attributes, 'html' => TRUE));
  }

  return $output;
}


/**
 * Theme function for the lightbox iframe filefield formatter.
 *
 * @param $element
 *   The CCK field element.
 * @return
 *   The themed link to the file, with lightframe support.
 */
function theme_lightbox2_formatter_filefield_lightframe($element) {
  $file = $element['#item'];
  $field = content_fields($element['#field_name']);
  if (!filefield_view_access($field['field_name']) || empty($file['list'])) {
    return '';
  }
  if (empty($file['fid']) || !is_file($file['filepath'])) {
    return '';
  }
  $file = (object)$file;
  if (!lightbox2_check_filefield_extension($file, $field)) {
    return '';
  }
  drupal_add_css(drupal_get_path('module', 'filefield') .'/filefield.css');
  return '<div class="filefield-item">'. theme('lightbox2_file_formatter_lightbox2_iframe', $file, $field, NULL) .'</div>';
}

/**
 * Theme function for the lightbox iframe filefield formatter.
 *
 * @param $file
 *   Filefield file object.
 * @param $field
 *   The CCK field the action is being performed on.
 * @param $file_formatter_settings
 *   File formatter settings; ignored.
 * @return
 *   Themed link to filefield.
 */
function theme_lightbox2_file_formatter_lightbox2_iframe($file, $field, $file_formatter_settings) {
  $path = $file->filepath;
  $url = file_create_url($path);
  $icon = theme('filefield_icon', $file);
  $file_data = $file->data;
  if (is_string($file_data)) {
    $file_data = unserialize($file->data);
  }
  $description = $file_data['description'];
  if (empty($description)) {
    $description = $file->filename;
  }
  $attributes = array();
  $attributes['id'] = 'lightbox2-node-link-text';
  $target = variable_get('lightbox2_node_link_target', FALSE);
  if (!empty($target)) {
    $attributes['target'] = $target;
  }
  $node_link = '';
  $node_link_text = check_plain(variable_get('lightbox2_node_link_text', 'View Image Details'));
  if (!empty($node_link_text)) {
    $node_link .= '<br /><br />'. l($node_link_text, $url, array('attributes' => $attributes));
  }

  // Only files with supported extensions make it this far, so no need to check here.
  $link_attributes = array('rel' => 'lightframe[]['. $description . $node_link .']');

  return '<div class="filefield-formatter-lightbox2-iframe">'. $icon . l($description, $url, array('attributes' => $link_attributes)) .'</div>';
}

/**
 * Generate the HTML output to open embedded cck images in a lightbox.
 *
 * @param $element
 *   The CCK field element.
 * @return
 *   The themed link to the embedded image.
 */
function theme_lightbox2_formatter_emimage($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  $item = $element['#item'];
  $formatter = $element['#formatter'];
  $node = node_load($element['#node']->nid);

  list($tmp, $lightbox_type) = explode('_', $formatter, 2);
  $field['lightbox_type'] = $lightbox_type;
  $formatter = "emimage";

  return module_invoke('emfield', 'emfield_field_formatter', $field, $item, $formatter, $node, 'lightbox2');
}

/**
 * Generate the HTML output to open embedded cck images in a lightbox.
 *
 * @param $field
 *   The CCK field the action is being performed on.
 * @param $item
 *   An array, keyed by column, of the data stored for this item in this field.
 * @param $formatter
 *   The formatter to use for the field.
 * @param $node
 *   The node object.
 * @param $args
 *   Args may override internal processes: caption, rel_grouping.
 * @return
 *   Themed embedded media field image and link.
 */
function theme_lightbox2_emimage($field, $item, $formatter, $node, $args = array()) {

  $lightbox_type = $field['lightbox_type'];

  if ($item['value'] && $item['provider']) {
    $rel = 'lightbox';
    if ($lightbox_type == 'lightshow2') {
      $rel = 'lightshow';
    }
    elseif ($lightbox_type == 'lightframe2') {
      $rel = 'lightframe';
    }
    if ($rel == 'lightframe' && arg(0) == 'node' && arg(1) == $node->nid) {
      $rel = 'lightbox';
    }
    $orig_rel = $rel;

    $code = $item['value'];
    $width = $field['widget']['thumbnail_width'] == '' ? variable_get('emimage_default_full_width', EMIMAGE_DEFAULT_FULL_WIDTH) : $field['widget']['thumbnail_width'];
    $height = $field['widget']['thumbnail_height'] == '' ? variable_get('emimage_default_full_height', EMIMAGE_DEFAULT_FULL_HEIGHT) : $field['widget']['thumbnail_height'];
    $link = $field['widget']['thumbnail_link'] ? $field['widget']['thumbnail_link'] : variable_get('emimage_default_thumbnail_link', EMIMAGE_DEFAULT_THUMBNAIL_LINK);
    if ($link == EMIMAGE_LINK_CONTENT) {
      $link = 'node/'. $node->nid;
    }
    elseif ($link == EMIMAGE_LINK_PROVIDER) {
      $link = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'embedded_link', $code, $item['data']);
    }
    else {
      $link = NULL;
    }
    $attributes = array();
    $attributes['id'] = 'lightbox2-node-link-text';
    if ($width) {
      $attributes['width'] = $width;
    }
    if ($height) {
      $attributes['height'] = $height;
    }
    $target = variable_get('lightbox2_node_link_target', FALSE);
    if (!empty($target)) {
      $attributes['target'] = $target;
    }

    // Set up the title.
    $title = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'image_title', $code, $item['data']);

    // Set up the caption.
    $node_link = '';
    $node_link_text = variable_get('lightbox2_node_link_text', 'View Image Details');
    if (!empty($node_link_text) && !empty($link)) {
      $node_link = '<br /><br />'. l($node_link_text, $link, array('attributes' => $attributes));
    }
    // @TODO original download link possible with emfield?

    $caption = $title . $node_link;
    if (isset($args['caption'])) {
      $caption = $args['caption']; // Override caption.
    }

    // Set up url and image.
    $url = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'image_url', $code, $width, $height, "emimage", $field, $item, $node);
    $image = theme('image', $url, $title, $title, $attributes, FALSE);

    // Set up full rel attribute.
    $image_grouping = variable_get('lightbox2_emimage_group_node_id', 1);
    if ($image_grouping == 1) {
      $full_rel = $rel .'['. $field['field_name'] .']['. $caption .']';
    }
    elseif ($image_grouping == 2 && !empty($node->nid)) {
      $full_rel = $rel .'['. $node->nid .']['. $caption .']';
    }
    elseif ($image_grouping == 3 && !empty($node->nid)) {
      $full_rel = $rel .'['. $field['field_name'] .'_'. $node->nid .']['. $caption .']';
    }
    elseif (isset($args['rel_grouping'])) {
      $full_rel = $rel .'['. $args['rel_grouping'] .']['. $caption .']';
    }
    else {
      $rel = $full_rel .'[]['. $caption .']';
    }

    if ($orig_rel != 'lightframe') {
      $link_attributes = array('rel' => $full_rel);
      $full_width = $field['widget']['full_width'] == '' ? variable_get('emimage_default_full_width', EMIMAGE_DEFAULT_FULL_WIDTH) : $field['widget']['full_width'];
      $full_height = $field['widget']['full_height'] == '' ? variable_get('emimage_default_full_height', EMIMAGE_DEFAULT_FULL_HEIGHT) : $field['widget']['full_height'];
      $full_image_url = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'image_url', $code, $full_width, $full_height, "emimage", $field, $item, $node);
      $output = l($image, $full_image_url, array('attributes' => $link_attributes, 'html' => TRUE));
    }
    else {
      $frame_width = variable_get('lightbox2_default_frame_width', 600);
      $frame_height = variable_get('lightbox2_default_frame_height', 400);
      $frame_size = 'width:'. $frame_width .'px; height:'. $frame_height .'px;';
      $full_rel = preg_replace('/\]\[/', "|$frame_size][", $full_rel);
      $link_attributes = array('rel' => $full_rel);
      $output = l($image, $link .'/lightbox2', array('attributes' => $link_attributes, 'html' => TRUE));
    }
  }

  return $output;
}

/**
 * Generate the HTML output to open embedded cck videos in a lightbox.
 *
 * @param $element
 *   The CCK field element.
 * @return
 *   HTML output for displaying the video and link.
 */
function theme_lightbox2_formatter_emvideo_lightvideo($element) {
  if (!variable_get('lightbox2_enable_video', FALSE)) {
    return;
  }
  $field = content_fields($element['#field_name'], $element['#type_name']);
  $item = $element['#item'];
  $formatter = "emvideo";
  $node = node_load($element['#node']->nid);

  return module_invoke('emfield', 'emfield_field_formatter', $field, $item, $formatter, $node, 'lightbox2');
}

/**
 * Generate the HTML output to open embedded cck videos in a lightbox.
 *
 * @param $field
 *   The CCK field the action is being performed on.
 * @param $item
 *   An array, keyed by column, of the data stored for this item in this field.
 * @param $formatter
 *   The formatter to use for the field.
 * @param $node
 *   The node object.
 * @param $options
 *   Options array.
 * @return
 *   Themed link to the embedded media field video.
 */
function theme_lightbox2_emvideo($field, $item, $formatter, $node, $options = array()) {
  $thumbnail = isset($options['thumbnail']) ? $options['thumbnail'] : theme('emvideo_video_thumbnail', $field, $item, 'video_thumbnail', $node, TRUE, $options);
  // if options title set use that, otherwise use video title if set in node,
  // otherwise try node title, then widget thumbnail setting and fallback to
  // default string.
  $title = isset($options['title']) ? $options['title'] : (isset($item['data']['emthumb']['emthumb_title']) ? $item['data']['emthumb']['emthumb_title'] : (isset($node->title) ? $node->title : (isset($field['widget']['thumbnail_link_title']) ? $field['widget']['thumbnail_link_title'] : variable_get('emvideo_default_thumbnail_link_title', t('See video')))));
  
  $destination = 'video-cck/lightbox2/'. $node->nid .'/'. $field['widget']['video_width'] .'/'. $field['widget']['video_height'] .'/'. $field['field_name'] .'/'. $item['provider'] .'/'. $item['value'];
  $width = $field['widget']['video_width'] + 20;
  $height = $field['widget']['video_height'] + 20;
  if (isset($_SERVER['HTTP_USER_AGENT']) && (strpos($_SERVER['HTTP_USER_AGENT'], 'MSIE') !== FALSE)) {
    $width = $field['widget']['video_width'] + 40;
    $height = $field['widget']['video_height'] + 40;
  }
  $rel_full = 'lightframe['. $field['type_name'] . '|width:'. $width .'px; height:'. $height .'px; overflow:visible;]';

  $attributes = array(
    'attributes' => array(
      'title' => $title,
      'class' => $field['type_name'],
      'rel' => $rel_full,
    ),
    'query' => NULL,
    'fragment' => NULL,
    'absolute' => FALSE,
    'html' => TRUE,
  );

  $output = l($thumbnail, $destination, $attributes);
  return $output;
}

