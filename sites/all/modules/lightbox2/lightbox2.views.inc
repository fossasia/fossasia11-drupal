<?php
// $Id: lightbox2.views.inc,v 1.1.2.1 2009/09/05 09:52:26 snpower Exp $

/**
 * @file
 * lightbox2.views.inc
 */

/**
 * Implementation of hook_views_data()
 */
function lightbox2_views_data() {

  $data['lightbox2']['table']['group'] = t('Lightbox');

  $data['lightbox2']['table']['join'] = array(
    '#global' => array(),
  );

  $data['lightbox2']['lightbox2'] = array(
    'group' => t('Lightbox'),
    'field' => array(
      'title' => t('Lightbox trigger'),
      'help' => t('Provide custom text or link.'),
      'handler' => 'lightbox2_handler_field_lightbox2',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function lightbox2_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'lightbox2'),
    ),
    'handlers' => array(
      'lightbox2_handler_field_lightbox2' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
