<?php

function imagecache_resize_form($action) {
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => isset($action['width']) ? $action['width'] : '100%',
    '#description' => t('Enter a width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => isset($action['height']) ? $action['height'] : '100%',
    '#description' => t('Enter a height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  return $form;
}

function imagecache_resize_image(&$image, $data) {
  if (!imageapi_image_resize($image, $data['width'], $data['height'])) {
    watchdog('imagecache', 'imagecache_resize_image failed. image: %image, data: %data.', array('%path' => $image, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}

function theme_imagecache_resize($element) {
  $data = $element['#value'];
  if ($data['width'] && $data['height']) {
    return check_plain($data['width']) . 'x' . check_plain($data['height']);
  }
  return ($data['width']) ? t('width @width', array('@width' => $data['width'])) : t('height @height', array('@height' => $data['height']));
}

/**
 * ImageCache Scale
 */
function imagecache_scale_form($data = array()) {
  $form = imagecache_resize_form($data);
  $form['upscale'] = array(
    '#type' => 'checkbox',
    '#default_value' => (isset($data['upscale'])) ? $data['upscale'] : 0,
    '#title' => t('Allow Upscaling'),
    '#description' => t('Let scale make images larger than their original size'),
  );
  return $form;
}

function theme_imagecache_scale($element) {
  return theme_imagecache_resize($element) . ' ' . ($element['#value']['upscale'] ? '(' . t('upscaling allowed') . ')' : '');
}

function imagecache_scale_image(&$image, $data) {
  // Set impossibly large values if the width and height aren't set.
  $data['width'] = $data['width'] ? $data['width'] : 9999999;
  $data['height'] = $data['height'] ? $data['height'] : 9999999;
  if (!imageapi_image_scale($image, $data['width'], $data['height'], $data['upscale'])) {
    watchdog('imagecache', 'imagecache_scale_image failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}


/**
 * ImageCache Scale and Crop
 */
function imagecache_scale_and_crop_form($data = array()) {
  return imagecache_resize_form($data);
}

function theme_imagecache_scale_and_crop($element) {
  return theme_imagecache_resize($element);
}


function imagecache_scale_and_crop_image(&$image, $data) {
  if (!imageapi_image_scale_and_crop($image, $data['width'], $data['height'])) {
    watchdog('imagecache', 'imagecache_scale_and_crop failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}



/**
 * ImageCache Deprecated Scale.
 * This will be removed in imagecache 2.1
 */
function imagecache_deprecated_scale_form($data = array()) {
  $helptext = array();
  $helptext['inside'] = t('<strong>Inside dimensions</strong>: Final dimensions will be less than or equal to the entered width and height. Useful for ensuring a maximum height and/or width.');
  $helptext['outside'] = t('<strong>Outside dimensions</strong>: Final dimensions will be greater than or equal to the entered width and height. Ideal for cropping the result to a square.');
  $description = '<ul><li>'. implode('</li><li>', $helptext) .'</li><ul>';

  $form['fit'] = array(
    '#type' => 'select',
    '#title' => t('Scale to fit'),
    '#options' => array('inside' => t('Inside dimensions'), 'outside' => t('Outside dimensions')),
    '#default_value' => isset($data['fit']) ? $data['fit'] : NULL,
    '#weight' => 1,
    '#description' => $description,
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => isset($data['width']) ? $data['width'] : '',
    '#description' => t('Enter a width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => isset($data['height']) ? $data['height'] : '',
    '#description' => t('Enter a height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  return $form;
}

function theme_imagecache_deprecated_scale($element) {
  $data = $element['#value'];
  $fits = array('inside' => t('Inside dimensions'), 'outside' => t('Outside dimensions'));
  return t('width: @width, height: @height, fit: @fit', array('@width' => $data['width'], '@height' => $data['height'], '@fit' => $fits[$data['fit']]));
}

function imagecache_deprecated_scale_image(&$image, $data) {
  if ($data['fit'] == 'outside' && $data['width'] && $data['height']) {
    $ratio = $image->info['width'] / $image->info['height'];
    $new_ratio = $data['width']/$data['height'];
    $data['width'] = $ratio > $new_ratio ? 0 : $data['width'];
    $data['height'] = $ratio < $new_ratio ? 0 : $data['height'];
  }
  // Set impossibly large values if the width and height aren't set.
  $data['width'] = $data['width'] ? $data['width'] : 9999999;
  $data['height'] = $data['height'] ? $data['height'] : 9999999;
  if (!imageapi_image_scale($image, $data['width'], $data['height'])) {
     watchdog('imagecache', 'imagecache_deprecated_scale failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}



/**
 * ImageCache Crop
 */
function imagecache_crop_form($data = array()) {
  $data += array(
    'width' => '',
    'height' => '',
    'xoffset' => '',
    'yoffset' => '',
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $data['width'],
    '#description' => t('Enter a width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $data['height'],
    '#description' => t('Enter a height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['xoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('X offset'),
    '#default_value' => $data['xoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
  );
  $form['yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Y offset'),
    '#default_value' => $data['yoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>.'),
  );
  return $form;
}

function theme_imagecache_crop($element) {
  $data = $element['#value'];
  return t('width: @width, height: @height, xoffset: @xoffset, yoffset: @yoffset', array(
    '@width' => $data['width'],
    '@height' => $data['height'],
    '@xoffset' => $data['xoffset'],
    '@yoffset' => $data['yoffset'],
  ));
}

function imagecache_crop_image(&$image, $data) {
  if (!imageapi_image_crop($image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
      watchdog('imagecache', 'imagecache_crop failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}


/**
 * ImageCache Desaturate
 */
function imagecache_desaturate_form($data = array()) {
  return array();
}

function theme_imagecache_desaturate($element) {
  return '';
}


function imagecache_desaturate_image(&$image, $data = array()) {
  if (!imageapi_image_desaturate($image)) {
    watchdog('imagecache', 'imagecache_desaturate failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}



/**
 * ImageCache Rotate
 */
function imagecache_rotate_form($data = array()) {
  $form['degrees'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($data['degrees'])) ? $data['degrees'] : 0,
    '#title' => t('Rotation angle'),
    '#description' => t('The number of degrees the image should be rotated. Positive numbers are clockwise, negative are counter-clockwise.'),
  );
  $form['random'] = array(
    '#type' => 'checkbox',
    '#default_value' => (isset($data['random'])) ? $data['random'] : 0,
    '#title' => t('Randomize'),
    '#description' => t('Randomize the rotation angle for each image. The angle specified above is used as a maximum.'),
  );
  $form['bgcolor'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($data['bgcolor'])) ? $data['bgcolor'] : '',
    '#title' => t('Background color'),
    '#description' => t('The background color to use for exposed areas of the image. Use web-style hex colors (#FFFFFF for white, #000000 for black). An empty value will cause images that support transparency to have transparent backgrounds, otherwise it will be white.'),
  );
  return $form;
}

function theme_imagecache_rotate($element) {
  $data = $element['#value'];
  if ($data['random']) {
    return t('random between -@degrees&deg and @degrees&deg', array('@degrees' => $data['degrees']));
  }
  return t('@degrees&deg', array('@degrees' => $data['degrees']));
}

function imagecache_rotate_image(&$image, $data) {
  // Merge in default values.
  $data += array(
    'degrees' => '0',
    'random' => FALSE,
    'bgcolor' => '',
  );

  // Set sane default values.
  if (strlen(trim($data['bgcolor']))) {
    $data['bgcolor'] = hexdec(str_replace('#', '', $data['bgcolor']));
  }
  else {
    $data['bgcolor'] = NULL;
  }

  if ($data['random']) {
    $degrees = abs((float)$data['degrees']);
    $data['degrees'] = rand(-1 * $degrees, $degrees);
  }

  if (!imageapi_image_rotate($image, $data['degrees'], $data['bgcolor'])) {
    watchdog('imagecache', 'imagecache_rotate_image failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}

/**
 * ImageCache Sharpen
 */
function imagecache_sharpen_form($data) {
  $form['info'] = array(
    '#value' => t('<strong>NOTE:</strong> The sigma parameter below is currently <em>only</em> used when the Imagemagick toolkit is active.'),
  );
  $form['radius'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($data['radius'])) ? $data['radius'] : '0.5' ,
    '#title' => t('Radius'),
    '#description' => t('The radius of the gaussian, in pixels, not counting the center pixel. If you\'re using Imagemagick, you can set this to 0 to let Imagemagick select a suitable radius. Typically 0.5 to 1 for screen resolutions. (default 0.5)'),
  );
  $form['sigma'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($data['sigma'])) ? $data['sigma'] : '0.5' ,
    '#title' => t('Sigma'),
    '#description' => t('The standard deviation of the gaussian, in pixels. General rule of thumb: if radius < 1 then sigma = radius, else sigma = sqrt(radius). (default 0.5)'),
  );
  $form['amount'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($data['amount'])) ? $data['amount'] : '100' ,
    '#title' => t('Amount'),
    '#description' => t('The percentage of the difference between the original and the blur image that is added back into the original. Typically 50 to 200. (default 100).'),
  );
  $form['threshold'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($data['threshold'])) ? $data['threshold'] : '0.05' ,
    '#title' => t('Threshold'),
    '#description' => t('The threshold, as a fraction of max RGB levels, needed to apply the difference amount.  Typically 0 to 0.2. (default 0.05).'),
  );
  return $form;
}

function theme_imagecache_sharpen($element) {
  $data = $element['#value'];
  return t('radius: @radius, sigma: @sigma, amount: @amount, threshold: @threshold', array(
    '@radius' => $data['radius'],
    '@sigma' => $data['sigma'],
    '@amount' => $data['amount'],
    '@threshold' => $data['threshold'],
  ));
}

function imagecache_sharpen_image(&$image, $data) {
  // Set sane default values.
  $data['radius'] = $data['radius'] ? $data['radius'] : "0.5";
  $data['sigma'] = $data['sigma'] ? $data['sigma'] : "0.5";
  $data['amount'] = $data['amount'] ? $data['amount'] : "100";
  $data['threshold'] = $data['threshold'] ? $data['threshold'] : "0.05";

  if (!imageapi_image_sharpen($image, $data['radius'], $data['sigma'], $data['amount'], $data['threshold'])) {
    watchdog('imagecache', 'imagecache_sharpen_image failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}
