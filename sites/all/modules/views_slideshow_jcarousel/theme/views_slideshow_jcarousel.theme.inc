<?php
/**
 * @file
 * Theme functions for Views Slideshow JCarousel
 */

/**
 * Theme pager fields
 */
function _views_slideshow_jcarousel_preprocess_pager(&$vars) {
  // Build our javascript settings.
  $js_vars = array(
    'viewsSlideshowJCarouselPager' => array(
      $vars['vss_id'] => array(
        $vars['location'] => array(
          'carouselObj' => '',
          'moveOnChange' => $vars['settings']['views_slideshow_jcarousel_pager_move_on_change'],
          'orientation' => $vars['settings']['views_slideshow_jcarousel_pager_orientation'],
          'scroll' => $vars['settings']['views_slideshow_jcarousel_pager_scroll'],
          'visible' => $vars['settings']['views_slideshow_jcarousel_pager_visible'],
          'wrap' => $vars['settings']['views_slideshow_jcarousel_pager_wrap'],
          'animation' => $vars['settings']['views_slideshow_jcarousel_pager_animation'],
        ),
      ),
    ),
  );

  // Add the settings to the page.
  drupal_add_js($js_vars, 'setting');

  // Add our class to the wrapper.
  $vars['attributes']['class'] = (isset($vars['attributes']['class'])) ? $vars['attributes']['class'] . ' views_slideshow_jcarousel_pager jcarousel-skin-' . $vars['settings']['views_slideshow_jcarousel_pager_skin'] : 'views_slideshow_jcarousel_pager jcarousel-skin-' . $vars['settings']['views_slideshow_jcarousel_pager_skin'];

  // Render all the fields unless there is only 1 slide and the user specified
  // to hide them when there is only one slide.
  $vars['rendered_field_items'] = '';
  if (empty($vars['settings']['hide_on_single_slide']) || count($vars['view']->result) > 1) {
    foreach ($vars['view']->result as $count => $node) {
      $rendered_fields = '';
      foreach ($vars['settings']['views_slideshow_jcarousel_pager_fields'] as $field => $use) {
        if ($use !== 0 && is_object($vars['view']->field[$field])) {
          $rendered_fields .= theme('views_slideshow_jcarousel_pager_field', $vars['view'], $field, $count);
        }
      }
      $vars['rendered_field_items'] .= theme('views_slideshow_jcarousel_pager_item', $vars['vss_id'], $rendered_fields, $count, $vars['location']);
    }

    if (module_exists('libraries')) {
      // Load jCarousel
      if ($jcarousel_path = _views_slideshow_jcarousel_library_path()) {
        drupal_add_js($jcarousel_path);
      }

      $jcarousel_path = libraries_get_path('jcarousel');
      drupal_add_css($jcarousel_path . '/skins/' . $vars['settings']['views_slideshow_jcarousel_pager_skin'] . '/skin.css');
      // Load our jcarousel pager js
      $module_path = drupal_get_path('module', 'views_slideshow_jcarousel');
      drupal_add_js($module_path . '/js/views_slideshow_jcarousel.js');
    }
  }
}

/**
 * Views Slideshow jCarousel: pager item.
 *
 * @ingroup themeable
 */
function _views_slideshow_jcarousel_preprocess_pager_item(&$vars) {
  $current = $vars['count'] + 1;
  $classes = array(
    'views_slideshow_jcarousel_pager_item',
  );
  if (!$vars['count']) {
    $classes[] = 'views_slideshow_active_pager_field_item';
  }
  $classes[] = ($vars['count'] % 2) ? 'views-row-even' : 'views-row-odd';

  $vars['attributes']['class'] = implode(' ', $classes);
  $vars['attributes']['id'] = 'views_slideshow_jcarousel_pager_item_' . $vars['location'] . '_' . $vars['vss_id'] . '_' . $vars['count'];
}
