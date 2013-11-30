<?php

//Made by Chao Xu(Mgccl) 3/1/07
//www.webdevlogs.com
//V 1.0

// Drupal code style and some restructuring by dopry. http://www.darrelopry.com


define('IMG_FILTER_NEGATE', 0);
define('IMG_FILTER_GRAYSCALE', 1);
define('IMG_FILTER_BRIGHTNESS', 2);
define('IMG_FILTER_CONTRAST', 3);
define('IMG_FILTER_COLORIZE', 4);
define('IMG_FILTER_EDGEDETECT', 5);
define('IMG_FILTER_EMBOSS', 6);
define('IMG_FILTER_GAUSSIAN_BLUR', 7);
define('IMG_FILTER_SELECTIVE_BLUR', 8);
define('IMG_FILTER_MEAN_REMOVAL', 9);
define('IMG_FILTER_SMOOTH', 10);

define('IMAGEAPI_IMAGEFILTER_PHP', 1);
/**
 * walk each pixel in an image applying $callback to it.
 */
function _imageapi_gd_pixel_color_walk(&$im, $callback, $arg1 = NULL, $arg2 = NULL, $arg3 = NULL, $arg4 = NULL) {
  $max_y = imagesy($im);
  $max_x = imagesx($im);
  for ($y=0; $y < $max_y; ++$y) {
    for ($x=0; $x < $max_x; ++$x) {
      $rgb = imagecolorat($im, $x, $y);
      $r = ($rgb >> 16) & 0xFF;
      $g = ($rgb >> 8) & 0xFF;
      $b = ($rgb & 0xFF);
      $a = $rgb >> 24;

      $callback($r, $g, $b, $a, $arg1, $arg2, $arg3, $arg4);

      // sanitize rgb values.
      $r = ($r > 255)? 255 : (($r < 0)? 0:$r);
      $g = ($g > 255)? 255 : (($g < 0)? 0:$g);
      $b = ($b > 255)? 255 : (($b < 0)? 0:$b);
      $a = ($a > 127)? 127 : (($a < 0)? 0:$a);

      if (!$color = imagecolorallocatealpha($im, $r, $g, $b, $a)) {
        $color = imagecolorclosestalpha($im, $r, $g, $b, $a);
      }
      imagesetpixel($im, $x, $y, $color);
    }
  }
}

function _imageapi_gd_pixel_negate(&$r, &$g, &$b, &$a) {
  $r = 255 - $r;
  $g = 255 - $g;
  $b = 255 - $b;
}


function _imageapi_gd_pixel_grayscale(&$r, &$g, &$b, &$a) {
  $r = round($r * 0.299 + $g * 0.587 + $b * 0.114);
  $g = $r;
  $b = $r;
}

function _imageapi_gd_pixel_brightness(&$r, &$g, &$b, &$a, $arg1) {
  $r += $arg1;
  $g += $arg1;
  $b += $arg1;
}

function _imageapi_gd_pixel_contrast(&$r, &$g, &$b, &$a, $contrast) {
  // normalize color value between -0.5 - 0.5
  // multiply by contrast value to accentuate positive/negative value.
  // denormalize to 0-255 range.
  $r = ((($r/255 - 0.5) * $contrast) + 0.5) * 255;
  $g = ((($g/255 - 0.5) * $contrast) + 0.5) * 255;
  $b = ((($b/255 - 0.5) * $contrast) + 0.5) * 255;
}

function _imageapi_gd_pixel_colorize(&$r, &$g, &$b, &$a, $arg1, $arg2, $arg3, $arg4) {
  $r += $arg1;
  $g += $arg2;
  $b += $arg3;
}

function imagefilter(&$im, $var, $arg1 = NULL, $arg2 = NULL, $arg3 = NULL, $arg4 = NULL) {

  switch ($var) {
    case IMG_FILTER_NEGATE:
      _imageapi_gd_pixel_color_walk($im, '_imageapi_gd_pixel_negate');
      return TRUE;

    case IMG_FILTER_GRAYSCALE:
      _imageapi_gd_pixel_color_walk($im, '_imageapi_gd_pixel_grayscale');
      return TRUE;

    case IMG_FILTER_BRIGHTNESS:
      _imageapi_gd_pixel_color_walk($im, '_imageapi_gd_pixel_brightness', $arg1);
      return TRUE;

    case IMG_FILTER_CONTRAST:
      // normalize between 0-1, square to keep positive.
      $contrast = pow((100-$arg1)/100, 2);
      _imageapi_gd_pixel_color_walk($im, '_imageapi_gd_pixel_contrast', $contrast);
      return TRUE;

    case IMG_FILTER_COLORIZE:
      $arg1 = (is_null($arg1)) ? 0 : $arg1;
      $arg2 = (is_null($arg2)) ? 0 : $arg2;
      $arg3 = (is_null($arg3)) ? 0 : $arg3;
      $arg4 = (is_null($arg4)) ? 0 : $arg4;
      _imageapi_gd_pixel_color_walk($im, '_imageapi_gd_pixel_colorize', $arg1, $arg2, $arg3, $arg4);
      return TRUE;

    case IMG_FILTER_EDGEDETECT:
      return imageconvolution($im, array(array(-1, 0, -1), array(0, 4, 0), array(-1, 0, -1)), 1, 127);

    case IMG_FILTER_EMBOSS:
      return imageconvolution($im, array(array(1.5, 0, 0), array(0, 0, 0), array(0, 0, -1.5)), 1, 127);

    case IMG_FILTER_GAUSSIAN_BLUR:
      return imageconvolution($im, array(array(1, 2, 1), array(2, 4, 2), array(1, 2, 1)), 16, 0);

    case IMG_FILTER_SELECTIVE_BLUR:
      for ($y = 0; $y<$max_y; ++$y) {
        for ($x = 0; $x<$max_x; ++$x) {
          $flt_r_sum = $flt_g_sum = $flt_b_sum = 0;
          $cpxl = imagecolorat($im, $x, $y);
          for ($j=0; $j<3; ++$j) {
            for ($i=0; $i<3; ++$i) {
              if (($j == 1) && ($i == 1)) {
                $flt_r[1][1] = $flt_g[1][1] = $flt_b[1][1] = 0.5;
              }
              else {
                $pxl = imagecolorat($im, $x-(3>>1)+$i, $y-(3>>1)+$j);
                $new_a = $pxl >> 24;
                //$r = (($pxl >> 16) & 0xFF);
                //$g = (($pxl >> 8) & 0xFF);
                //$b = ($pxl & 0xFF);
                $new_r = abs((($cpxl >> 16) & 0xFF) - (($pxl >> 16) & 0xFF));
                if ($new_r != 0) {
                  $flt_r[$j][$i] = 1/$new_r;
                }
                else {
                  $flt_r[$j][$i] = 1;
                }

                $new_g = abs((($cpxl >> 8) & 0xFF) - (($pxl >> 8) & 0xFF));
                if ($new_g != 0) {
                  $flt_g[$j][$i] = 1/$new_g;
                }
                else {
                  $flt_g[$j][$i] = 1;
                }

                $new_b = abs(($cpxl & 0xFF) - ($pxl & 0xFF));
                if ($new_b != 0) {
                  $flt_b[$j][$i] = 1/$new_b;
                }
                else {
                  $flt_b[$j][$i] = 1;
                }
              }

              $flt_r_sum += $flt_r[$j][$i];
              $flt_g_sum += $flt_g[$j][$i];
              $flt_b_sum += $flt_b[$j][$i];
            }
          }

          for ($j=0; $j<3; ++$j) {
            for ($i=0; $i<3; ++$i) {
              if ($flt_r_sum != 0) $flt_r[$j][$i] /= $flt_r_sum;
              if ($flt_g_sum != 0) $flt_g[$j][$i] /= $flt_g_sum;
              if ($flt_b_sum != 0) $flt_b[$j][$i] /= $flt_b_sum;

              $new_r = $new_g = $new_b = 0;

              for ($j=0; $j<3; ++$j) {
                for ($i=0; $i<3; ++$i) {
                  $pxl = imagecolorat($im, $x-(3>>1)+$i, $y-(3>>1)+$j);
                  $new_r += (($pxl >> 16) & 0xFF) * $flt_r[$j][$i];
                  $new_g += (($pxl >> 8) & 0xFF) * $flt_g[$j][$i];
                  $new_b += ($pxl & 0xFF) * $flt_b[$j][$i];
                }
              }

              $new_r = ($new_r > 255)? 255 : (($new_r < 0)? 0:$new_r);
              $new_g = ($new_g > 255)? 255 : (($new_g < 0)? 0:$new_g);
              $new_b = ($new_b > 255)? 255 : (($new_b < 0)? 0:$new_b);
              $new_pxl = imagecolorallocatealpha($im, (int)$new_r, (int)$new_g, (int)$new_b, $new_a);
              if ($new_pxl == FALSE) {
                $new_pxl = imagecolorclosestalpha($im, (int)$new_r, (int)$new_g, (int)$new_b, $new_a);
              }
              imagesetpixel($im, $x, $y, $new_pxl);
            }
          }
        }
      }
      return TRUE;

    case IMG_FILTER_MEAN_REMOVAL:
      return imageconvolution($im, array(array(-1, -1, -1), array(-1, 9, -1), array(-1, -1, -1)), 1, 0);

    case IMG_FILTER_SMOOTH:
      return imageconvolution($im, array(array(1, 1, 1), array(1, $arg1, 1), array(1, 1, 1)), $arg1 + 8, 0);
  }
}
