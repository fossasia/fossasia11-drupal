<?php

/**
 * @file
 * provides the imagerotate function for php-gd extensions compiled with the
 * upstream libgd instead of the libgd bundled with php.
 */


// Define as included.
define('IMAGEAPI_IMAGEROTATE_PHP', 1);

function imagerotate(&$im, $angle, $bgcolor) {
  if ($angle === 0) {
    return $im;
  }
  // imagerotate() in php's libgd rotates the image counterclockwise,
  // this implementation rotates clockwise. The angle needs to be
  // inverted to give the same behaviour between these implementations.
  $angle = 360 - $angle;

  $width  = imagesx($im);
  $height = imagesy($im);
  // background color.
  list($r, $g, $b, $a) = imageapi_hex2rgba($bgcolor);

  switch ($angle) {
    case 270:
    case 90:
      // flip dimensions.
      $rot_width = $height;
      $rot_height = $width;
      break;
    case 180:
      // maintain dims.
      $rot_width = $width;
      $rot_height = $height;
      break;

    default:
      // well it won't be easy but we'll actually rotate this
      // puppy ourselves.. gonna require a little trig.
      // @todo: convert to a polar equation and use 1/2 length of hypoteneus.
      $center_x = floor($width/2);
      $center_y = floor($height/2);

      // convert to radians and precompute ...
      $cosangle = cos(deg2rad($angle+180));
      $sinangle = sin(deg2rad($angle+180));

      // caluculate new width and height.
      $corners=array(array(0, 0), array($width, 0), array($width, $height), array(0, $height));
      $max_x = $min_x = $max_y = $min_y = 0;
      foreach ($corners as $key => $value) {
        $value[0] -= $center_x;        //Translate coords to center for rotation
        $value[1] -= $center_y;
        $x = $value[0] * $cosangle + $value[1] * $sinangle;
        $y = $value[1] * $cosangle - $value[0] * $sinangle;
        $max_x = max($x, $max_x);
        $min_x = min($x, $min_x);
        $max_y = max($y, $max_y);
        $min_y = min($y, $min_y);
      }
      $rot_width = (int)$max_x - $min_x;
      $rot_height = (int)$max_y - $min_y;
      $rot_center_x = floor($rot_width/2);
      $rot_center_y = floor($rot_height/2);
  }

  $rotate = imagecreatetruecolor($rot_width, $rot_height);
  $bg = imagecolorallocatealpha($rotate, $r, $g, $b, $a);
  imagefilledrectangle($rotate, 0, 0, $rot_width, $rot_height, $bg);
  imagealphablending($rotate, FALSE);
  imagesavealpha($rotate, TRUE);

  switch ($angle) {
    case 90:
      $rot_width--;
      for ($y = 0; $y < $height; ++$y)
        for ($x = 0; $x < $width; ++$x)
          imagesetpixel($rotate, $rot_width - $y, $x, imagecolorat($im, $x, $y));
      break;
    case 270:
      $rot_height--;
      for ($y = 0; $y < $height; ++$y)
        for ($x = 0; $x < $width; ++$x)
          imagesetpixel($rotate, $y, $rot_height - $x, imagecolorat($im, $x, $y));
      break;
    case 180:
      $rot_width--;
      $rot_height--;
      for ($y = 0; $y < $height; ++$y)
        for ($x = 0; $x < $width; ++$x)
          imagesetpixel($rotate, $rot_width - $x, $rot_height - $y, imagecolorat($im, $x, $y));
      break;

    default:
      for ($y = 0; $y < $rot_height; ++$y) {
        for ($x = 0; $x < $rot_width; ++$x) {
          $mod_y = $rot_center_y-$y;
          $mod_x = $rot_center_x-$x;
          $old_x = $mod_x * $cosangle + $mod_y * $sinangle + $center_x;
          $old_y = $mod_y * $cosangle - $mod_x * $sinangle + $center_y;
          if ($old_x >= 0 && $old_x < $width && $old_y >= 0 && $old_y < $height) {
            $color = imagecolorat($im, $old_x, $old_y);
            imagesetpixel($rotate, $x, $y,  $color);
          }
        }
      }
  }
  return $rotate;
}

