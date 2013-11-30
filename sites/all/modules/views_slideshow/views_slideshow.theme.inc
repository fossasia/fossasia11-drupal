<?php

/**
 * @file
 * The theme system, which controls the output of views slideshow.
 *
 * This just adds a wrapper div to the slideshow.
 */

/**
 * The current element of the slideshow.
 *
 * @ingroup themeable
 */
function theme_views_slideshow_main_section($id, $hidden_elements, $plugin) {
  $attributes['id'] = "${plugin}_main_${id}";
  $attributes['class'] = "${plugin}_main views_slideshow_main";
  $attributes = drupal_attributes($attributes);

  return "<div$attributes>$hidden_elements</div>";
}
