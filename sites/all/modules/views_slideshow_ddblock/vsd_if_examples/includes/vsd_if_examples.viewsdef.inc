<?php

/**
 * @file
 * view export for vsd_if_examples module
 */

/**
 * Helper for implementation of hook_views_default_views().
 */
function _vsd_if_examples_views_default_views() {
  $views = array();

  // Exported view: ddblock_if_example_slideshows
$view = new view;
$view->name = 'ddblock_if_example_slideshows';
$view->description = 'ddblock image field example slideshows';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'nid' => array(
    'label' => 'Nid',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'nid',
    'table' => 'node',
    'field' => 'nid',
    'relationship' => 'none',
  ),
  'title' => array(
    'label' => 'Title',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'field_ddblock_if_image_fid' => array(
    'label' => 'Image',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'label_type' => 'widget',
    'format' => 'image_plain',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_ddblock_if_image_fid',
    'table' => 'node_data_field_ddblock_if_image',
    'field' => 'field_ddblock_if_image_fid',
    'relationship' => 'none',
  ),
  'field_ddblock_if_slide_text_value' => array(
    'label' => 'Slide text',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '100',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 1,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'label_type' => 'widget',
    'format' => 'default',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_ddblock_if_slide_text_value',
    'table' => 'node_data_field_ddblock_if_slide_text',
    'field' => 'field_ddblock_if_slide_text_value',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
  'field_ddblock_if_pager_item_text_value' => array(
    'label' => 'Pager item text',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'label_type' => 'widget',
    'format' => 'plain',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_ddblock_if_pager_item_text_value',
    'table' => 'node_data_field_ddblock_if_pager_item_text',
    'field' => 'field_ddblock_if_pager_item_text_value',
    'relationship' => 'none',
  ),
  'view_node' => array(
    'label' => 'Link',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'text' => 'Read more...',
    'exclude' => 0,
    'id' => 'view_node',
    'table' => 'node',
    'field' => 'view_node',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'ddblock_image_field_example_item' => 'ddblock_image_field_example_item',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('style_plugin', 'slideshow');
$handler->override_option('style_options', array(
  'grouping' => '',
  'mode' => 'views_slideshow_ddblock',
  'views_slideshow_ddblock' => array(
    'widget' => 'cycle',
    'debug_info' => 'drupal',
    'mapping_wrapper' => array(
      'mappings' => array(
        '0' => array(
          'target' => 'node_id',
          'source' => 'nid',
        ),
        '1' => array(
          'target' => 'slide_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '2' => array(
          'target' => 'slide_title',
          'source' => 'title',
        ),
        '3' => array(
          'target' => 'slide_text',
          'source' => 'field_ddblock_if_slide_text_value',
        ),
        '4' => array(
          'target' => 'slide_read_more',
          'source' => 'view_node',
        ),
        '5' => array(
          'target' => 'pager_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '6' => array(
          'target' => 'pager_text',
          'source' => 'field_ddblock_if_pager_item_text_value',
        ),
        '7' => array(
          'target' => '',
          'source' => '',
        ),
        '8' => array(
          'target' => '',
          'source' => '',
        ),
        '9' => array(
          'target' => '',
          'source' => '',
        ),
        '10' => array(
          'target' => '',
          'source' => '',
        ),
        '11' => array(
          'target' => '',
          'source' => '',
        ),
        '12' => array(
          'target' => '',
          'source' => '',
        ),
        '13' => array(
          'target' => '',
          'source' => '',
        ),
        '14' => array(
          'target' => '',
          'source' => '',
        ),
        '15' => array(
          'target' => '',
          'source' => '',
        ),
      ),
    ),
    'template' => 'default',
    'custom_template' => '',
    'content_container' => array(
      'container' => 'div.slide',
      'overflow' => 0,
    ),
    'settings' => array(
      'fx' => 'fade',
      'speed' => '500',
      'timeout' => '5000',
      'order' => 'none',
      'pause' => 1,
      'next' => 0,
      'pager_toggle' => 0,
      'pager_settings' => array(
        'pager' => 'custom-pager',
        'pager_container' => '.custom-pager-item',
        'nr_of_pager_items' => '5',
        'pager_position' => 'top',
        'pager_event' => 'click',
        'pager_fast' => 1,
        'pager_pause' => 1,
      ),
      'pager2' => 1,
      'pager2_settings' => array(
        'pager2_event' => 'click',
        'pager2_position' => array(
          'pager' => '',
          'slide' => 'slide',
        ),
        'pager2_pager' => array(
          'pager2_pager_prev' => 'prev',
          'pager2_pager_next' => 'next',
          'pager2_pager_hide' => 1,
        ),
        'pager2_slide' => array(
          'pager2_slide_prev' => '',
          'pager2_slide_next' => '',
          'pager2_slide_hide' => 1,
        ),
      ),
      'slide_text' => 1,
      'slide_text_settings' => array(
        'slide_text_container' => 'div.slide-text',
        'slide_text_position' => 'bottom',
        'slide_text_jquery' => 0,
        'slide_jquery' => array(
          'slide_text_before_effect' => 'hide',
          'slide_text_before_speed' => '500',
          'slide_text_after_effect' => 'show',
          'slide_text_after_speed' => '500',
        ),
      ),
      'custom' => array(
        'custom_jquery' => '',
      ),
    ),
  ),
));
$handler = $view->new_display('block', 'upright10', 'block_1');
$handler->override_option('style_options', array(
  'mode' => 'views_slideshow_ddblock',
  'views_slideshow_ddblock' => array(
    'widget' => 'cycle',
    'debug_info' => 'none',
    'template' => 'default',
    'custom_template' => '',
    'template_submit' => 'get template sizes',
    'template_size_wrapper' => array(
      'template_size' => 'default',
    ),
    'custom_template_size' => '',
    'mapping_wrapper' => array(
      'mappings' => array(
        '0' => array(
          'target' => 'node_id',
          'source' => 'nid',
        ),
        '1' => array(
          'target' => 'slide_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '2' => array(
          'target' => 'slide_title',
          'source' => 'title',
        ),
        '3' => array(
          'target' => 'slide_text',
          'source' => 'field_ddblock_if_slide_text_value',
        ),
        '4' => array(
          'target' => 'slide_read_more',
          'source' => 'view_node',
        ),
        '5' => array(
          'target' => 'pager_image',
          'source' => '',
        ),
        '6' => array(
          'target' => 'pager_text',
          'source' => '',
        ),
        '7' => array(
          'target' => '',
          'source' => '',
        ),
        '8' => array(
          'target' => '',
          'source' => '',
        ),
        '9' => array(
          'target' => '',
          'source' => '',
        ),
        '10' => array(
          'target' => '',
          'source' => '',
        ),
        '11' => array(
          'target' => '',
          'source' => '',
        ),
        '12' => array(
          'target' => '',
          'source' => '',
        ),
        '13' => array(
          'target' => '',
          'source' => '',
        ),
        '14' => array(
          'target' => '',
          'source' => '',
        ),
        '15' => array(
          'target' => '',
          'source' => '',
        ),
      ),
    ),
    'content_container' => array(
      'container' => 'div.slide',
      'overflow' => 0,
    ),
    'settings' => array(
      'fx' => 'fade',
      'speed' => '500',
      'timeout' => '5000',
      'order' => 'none',
      'pause' => 1,
      'next' => 0,
      'pager_toggle' => 1,
      'pager_settings' => array(
        'pager' => 'custom-pager',
        'pager_container' => '.custom-pager-item',
        'nr_of_pager_items' => '3',
        'pager_position' => 'top',
        'pager_event' => 'click',
        'pager_fast' => 1,
        'pager_pause' => 1,
      ),
      'pager2' => 1,
      'pager2_settings' => array(
        'pager2_event' => 'click',
        'pager2_position' => array(
          'pager' => 'pager',
          'slide' => 'slide',
        ),
        'pager2_pager' => array(
          'pager2_pager_prev' => 'prev',
          'pager2_pager_next' => 'next',
          'pager2_pager_hide' => 1,
        ),
        'pager2_slide' => array(
          'pager2_slide_prev' => '',
          'pager2_slide_next' => '',
          'pager2_slide_hide' => 1,
        ),
      ),
      'slide_text' => 1,
      'slide_text_settings' => array(
        'slide_text_container' => 'div.slide-text',
        'slide_text_position' => 'bottom',
        'slide_text_jquery' => 0,
        'slide_jquery' => array(
          'slide_text_before_effect' => 'hide',
          'slide_text_before_speed' => '500',
          'slide_text_after_effect' => 'show',
          'slide_text_after_speed' => '500',
        ),
      ),
      'custom' => array(
        'custom_jquery' => '',
      ),
    ),
  ),
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'upright20', 'block_2');
$handler->override_option('style_options', array(
  'grouping' => '',
  'mode' => 'views_slideshow_ddblock',
  'views_slideshow_ddblock' => array(
    'widget' => 'cycle',
    'debug_info' => 'none',
    'mapping_wrapper' => array(
      'mappings' => array(
        '0' => array(
          'target' => 'node_id',
          'source' => 'nid',
        ),
        '1' => array(
          'target' => 'slide_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '2' => array(
          'target' => 'slide_title',
          'source' => 'title',
        ),
        '3' => array(
          'target' => 'slide_text',
          'source' => 'field_ddblock_if_slide_text_value',
        ),
        '4' => array(
          'target' => 'slide_read_more',
          'source' => 'view_node',
        ),
        '5' => array(
          'target' => 'pager_image',
          'source' => '',
        ),
        '6' => array(
          'target' => 'pager_text',
          'source' => '',
        ),
        '7' => array(
          'target' => '',
          'source' => '',
        ),
        '8' => array(
          'target' => '',
          'source' => '',
        ),
        '9' => array(
          'target' => '',
          'source' => '',
        ),
        '10' => array(
          'target' => '',
          'source' => '',
        ),
        '11' => array(
          'target' => '',
          'source' => '',
        ),
        '12' => array(
          'target' => '',
          'source' => '',
        ),
        '13' => array(
          'target' => '',
          'source' => '',
        ),
        '14' => array(
          'target' => '',
          'source' => '',
        ),
        '15' => array(
          'target' => '',
          'source' => '',
        ),
      ),
    ),
    'template' => 'vsd-upright-20p',
    'custom_template' => '',
    'content_container' => array(
      'container' => 'div.slide',
      'overflow' => 0,
    ),
    'settings' => array(
      'fx' => 'fade',
      'speed' => '500',
      'timeout' => '5000',
      'order' => 'none',
      'pause' => 1,
      'next' => 0,
      'pager_toggle' => 1,
      'pager_settings' => array(
        'pager' => 'prev-next-pager',
        'pager_container' => '.custom-pager-item',
        'nr_of_pager_items' => '3',
        'pager_position' => 'top',
        'pager_event' => 'click',
        'pager_fast' => 1,
        'pager_pause' => 1,
      ),
      'pager2' => 1,
      'pager2_settings' => array(
        'pager2_event' => 'click',
        'pager2_position' => array(
          'pager' => 'pager',
          'slide' => 'slide',
        ),
        'pager2_pager' => array(
          'pager2_pager_prev' => 'prev',
          'pager2_pager_next' => 'next',
          'pager2_pager_hide' => 1,
        ),
        'pager2_slide' => array(
          'pager2_slide_prev' => '',
          'pager2_slide_next' => '',
          'pager2_slide_hide' => 1,
        ),
      ),
      'slide_text' => 1,
      'slide_text_settings' => array(
        'slide_text_container' => 'div.slide-text',
        'slide_text_position' => 'bottom',
        'slide_text_jquery' => 0,
        'slide_jquery' => array(
          'slide_text_before_effect' => 'hide',
          'slide_text_before_speed' => '500',
          'slide_text_after_effect' => 'show',
          'slide_text_after_speed' => '500',
        ),
      ),
      'custom' => array(
        'custom_jquery' => '',
      ),
    ),
  ),
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'upright30', 'block_3');
$handler->override_option('style_options', array(
  'grouping' => '',
  'mode' => 'views_slideshow_ddblock',
  'views_slideshow_ddblock' => array(
    'widget' => 'cycle',
    'debug_info' => 'none',
    'mapping_wrapper' => array(
      'mappings' => array(
        '0' => array(
          'target' => 'node_id',
          'source' => 'nid',
        ),
        '1' => array(
          'target' => 'slide_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '2' => array(
          'target' => 'slide_title',
          'source' => 'title',
        ),
        '3' => array(
          'target' => 'slide_text',
          'source' => 'field_ddblock_if_slide_text_value',
        ),
        '4' => array(
          'target' => 'slide_read_more',
          'source' => 'view_node',
        ),
        '5' => array(
          'target' => 'pager_image',
          'source' => '',
        ),
        '6' => array(
          'target' => 'pager_text',
          'source' => 'field_ddblock_if_pager_item_text_value',
        ),
        '7' => array(
          'target' => '',
          'source' => '',
        ),
        '8' => array(
          'target' => '',
          'source' => '',
        ),
        '9' => array(
          'target' => '',
          'source' => '',
        ),
        '10' => array(
          'target' => '',
          'source' => '',
        ),
        '11' => array(
          'target' => '',
          'source' => '',
        ),
        '12' => array(
          'target' => '',
          'source' => '',
        ),
        '13' => array(
          'target' => '',
          'source' => '',
        ),
        '14' => array(
          'target' => '',
          'source' => '',
        ),
        '15' => array(
          'target' => '',
          'source' => '',
        ),
      ),
    ),
    'template' => 'vsd-upright-30p',
    'custom_template' => '',
    'content_container' => array(
      'container' => 'div.slide',
      'overflow' => 0,
    ),
    'settings' => array(
      'fx' => 'fade',
      'speed' => '500',
      'timeout' => '5000',
      'order' => 'none',
      'pause' => 1,
      'next' => 0,
      'pager_toggle' => 1,
      'pager_settings' => array(
        'pager' => 'custom-pager',
        'pager_container' => '.custom-pager-item',
        'nr_of_pager_items' => '3',
        'pager_position' => 'top',
        'pager_event' => 'click',
        'pager_fast' => 1,
        'pager_pause' => 1,
      ),
      'pager2' => 1,
      'pager2_settings' => array(
        'pager2_event' => 'click',
        'pager2_position' => array(
          'pager' => 'pager',
          'slide' => 'slide',
        ),
        'pager2_pager' => array(
          'pager2_pager_prev' => 'prev',
          'pager2_pager_next' => 'next',
          'pager2_pager_hide' => 1,
        ),
        'pager2_slide' => array(
          'pager2_slide_prev' => '',
          'pager2_slide_next' => '',
          'pager2_slide_hide' => 1,
        ),
      ),
      'slide_text' => 1,
      'slide_text_settings' => array(
        'slide_text_container' => 'div.slide-text',
        'slide_text_position' => 'bottom',
        'slide_text_jquery' => 0,
        'slide_jquery' => array(
          'slide_text_before_effect' => 'hide',
          'slide_text_before_speed' => '500',
          'slide_text_after_effect' => 'show',
          'slide_text_after_speed' => '500',
        ),
      ),
      'custom' => array(
        'custom_jquery' => '',
      ),
    ),
  ),
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'upright40', 'block_4');
$handler->override_option('style_options', array(
  'grouping' => '',
  'mode' => 'views_slideshow_ddblock',
  'views_slideshow_ddblock' => array(
    'widget' => 'cycle',
    'debug_info' => 'none',
    'mapping_wrapper' => array(
      'mappings' => array(
        '0' => array(
          'target' => 'node_id',
          'source' => 'nid',
        ),
        '1' => array(
          'target' => 'slide_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '2' => array(
          'target' => 'slide_title',
          'source' => 'title',
        ),
        '3' => array(
          'target' => 'slide_text',
          'source' => 'field_ddblock_if_slide_text_value',
        ),
        '4' => array(
          'target' => 'slide_read_more',
          'source' => 'view_node',
        ),
        '5' => array(
          'target' => 'pager_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '6' => array(
          'target' => 'pager_text',
          'source' => '',
        ),
        '7' => array(
          'target' => '',
          'source' => '',
        ),
        '8' => array(
          'target' => '',
          'source' => '',
        ),
        '9' => array(
          'target' => '',
          'source' => '',
        ),
        '10' => array(
          'target' => '',
          'source' => '',
        ),
        '11' => array(
          'target' => '',
          'source' => '',
        ),
        '12' => array(
          'target' => '',
          'source' => '',
        ),
        '13' => array(
          'target' => '',
          'source' => '',
        ),
        '14' => array(
          'target' => '',
          'source' => '',
        ),
        '15' => array(
          'target' => '',
          'source' => '',
        ),
      ),
    ),
    'template' => 'vsd-upright-40p',
    'custom_template' => '',
    'content_container' => array(
      'container' => 'div.slide',
      'overflow' => 0,
    ),
    'settings' => array(
      'fx' => 'fade',
      'speed' => '500',
      'timeout' => '5000',
      'order' => 'none',
      'pause' => 1,
      'next' => 0,
      'pager_toggle' => 1,
      'pager_settings' => array(
        'pager' => 'custom-pager',
        'pager_container' => '.custom-pager-item',
        'nr_of_pager_items' => '3',
        'pager_position' => 'top',
        'pager_event' => 'click',
        'pager_fast' => 1,
        'pager_pause' => 1,
      ),
      'pager2' => 1,
      'pager2_settings' => array(
        'pager2_event' => 'click',
        'pager2_position' => array(
          'pager' => 'pager',
          'slide' => 'slide',
        ),
        'pager2_pager' => array(
          'pager2_pager_prev' => 'prev',
          'pager2_pager_next' => 'next',
          'pager2_pager_hide' => 1,
        ),
        'pager2_slide' => array(
          'pager2_slide_prev' => '',
          'pager2_slide_next' => '',
          'pager2_slide_hide' => 1,
        ),
      ),
      'slide_text' => 1,
      'slide_text_settings' => array(
        'slide_text_container' => 'div.slide-text',
        'slide_text_position' => 'bottom',
        'slide_text_jquery' => 0,
        'slide_jquery' => array(
          'slide_text_before_effect' => 'hide',
          'slide_text_before_speed' => '500',
          'slide_text_after_effect' => 'show',
          'slide_text_after_speed' => '500',
        ),
      ),
      'custom' => array(
        'custom_jquery' => '',
      ),
    ),
  ),
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'upright50', 'block_5');
$handler->override_option('style_options', array(
  'grouping' => '',
  'mode' => 'views_slideshow_ddblock',
  'views_slideshow_ddblock' => array(
    'widget' => 'cycle',
    'debug_info' => 'none',
    'mapping_wrapper' => array(
      'mappings' => array(
        '0' => array(
          'target' => 'node_id',
          'source' => 'nid',
        ),
        '1' => array(
          'target' => 'slide_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '2' => array(
          'target' => 'slide_title',
          'source' => 'title',
        ),
        '3' => array(
          'target' => 'slide_text',
          'source' => 'field_ddblock_if_slide_text_value',
        ),
        '4' => array(
          'target' => 'slide_read_more',
          'source' => 'view_node',
        ),
        '5' => array(
          'target' => 'pager_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '6' => array(
          'target' => 'pager_text',
          'source' => 'field_ddblock_if_pager_item_text_value',
        ),
        '7' => array(
          'target' => '',
          'source' => '',
        ),
        '8' => array(
          'target' => '',
          'source' => '',
        ),
        '9' => array(
          'target' => '',
          'source' => '',
        ),
        '10' => array(
          'target' => '',
          'source' => '',
        ),
        '11' => array(
          'target' => '',
          'source' => '',
        ),
        '12' => array(
          'target' => '',
          'source' => '',
        ),
        '13' => array(
          'target' => '',
          'source' => '',
        ),
        '14' => array(
          'target' => '',
          'source' => '',
        ),
        '15' => array(
          'target' => '',
          'source' => '',
        ),
      ),
    ),
    'template' => 'vsd-upright-50p',
    'custom_template' => '',
    'content_container' => array(
      'container' => 'div.slide',
      'overflow' => 0,
    ),
    'settings' => array(
      'fx' => 'fade',
      'speed' => '500',
      'timeout' => '5000',
      'order' => 'none',
      'pause' => 1,
      'next' => 0,
      'pager_toggle' => 1,
      'pager_settings' => array(
        'pager' => 'custom-pager',
        'pager_container' => '.custom-pager-item',
        'nr_of_pager_items' => '3',
        'pager_position' => 'top',
        'pager_event' => 'click',
        'pager_fast' => 1,
        'pager_pause' => 1,
      ),
      'pager2' => 1,
      'pager2_settings' => array(
        'pager2_event' => 'click',
        'pager2_position' => array(
          'pager' => 'pager',
          'slide' => 'slide',
        ),
        'pager2_pager' => array(
          'pager2_pager_prev' => 'prev',
          'pager2_pager_next' => 'next',
          'pager2_pager_hide' => 1,
        ),
        'pager2_slide' => array(
          'pager2_slide_prev' => '',
          'pager2_slide_next' => '',
          'pager2_slide_hide' => 1,
        ),
      ),
      'slide_text' => 1,
      'slide_text_settings' => array(
        'slide_text_container' => 'div.slide-text',
        'slide_text_position' => 'bottom',
        'slide_text_jquery' => 0,
        'slide_jquery' => array(
          'slide_text_before_effect' => 'hide',
          'slide_text_before_speed' => '500',
          'slide_text_after_effect' => 'show',
          'slide_text_after_speed' => '500',
        ),
      ),
      'custom' => array(
        'custom_jquery' => '',
      ),
    ),
  ),
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'upright60', 'block_6');
$handler->override_option('style_options', array(
  'grouping' => '',
  'mode' => 'views_slideshow_ddblock',
  'views_slideshow_ddblock' => array(
    'widget' => 'cycle',
    'debug_info' => 'none',
    'mapping_wrapper' => array(
      'mappings' => array(
        '0' => array(
          'target' => 'node_id',
          'source' => 'nid',
        ),
        '1' => array(
          'target' => 'slide_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '2' => array(
          'target' => 'slide_title',
          'source' => 'title',
        ),
        '3' => array(
          'target' => 'slide_text',
          'source' => 'field_ddblock_if_slide_text_value',
        ),
        '4' => array(
          'target' => 'slide_read_more',
          'source' => 'view_node',
        ),
        '5' => array(
          'target' => 'pager_image',
          'source' => 'field_ddblock_if_image_fid',
        ),
        '6' => array(
          'target' => 'pager_text',
          'source' => '',
        ),
        '7' => array(
          'target' => '',
          'source' => '',
        ),
        '8' => array(
          'target' => '',
          'source' => '',
        ),
        '9' => array(
          'target' => '',
          'source' => '',
        ),
        '10' => array(
          'target' => '',
          'source' => '',
        ),
        '11' => array(
          'target' => '',
          'source' => '',
        ),
        '12' => array(
          'target' => '',
          'source' => '',
        ),
        '13' => array(
          'target' => '',
          'source' => '',
        ),
        '14' => array(
          'target' => '',
          'source' => '',
        ),
        '15' => array(
          'target' => '',
          'source' => '',
        ),
      ),
    ),
    'template' => 'vsd-upright-60p',
    'custom_template' => '',
    'content_container' => array(
      'container' => 'div.slide',
      'overflow' => 0,
    ),
    'settings' => array(
      'fx' => 'fade',
      'speed' => '500',
      'timeout' => '5000',
      'order' => 'none',
      'pause' => 1,
      'next' => 0,
      'pager_toggle' => 1,
      'pager_settings' => array(
        'pager' => 'scrollable-pager',
        'pager_container' => '.scrollable-pager-item',
        'nr_of_pager_items' => '5',
        'pager_position' => 'top',
        'pager_event' => 'click',
        'pager_fast' => 1,
        'pager_pause' => 1,
      ),
      'pager2' => 1,
      'pager2_settings' => array(
        'pager2_event' => 'click',
        'pager2_position' => array(
          'pager' => 'pager',
          'slide' => 'slide',
        ),
        'pager2_pager' => array(
          'pager2_pager_prev' => 'prev',
          'pager2_pager_next' => 'next',
          'pager2_pager_hide' => 1,
        ),
        'pager2_slide' => array(
          'pager2_slide_prev' => '',
          'pager2_slide_next' => '',
          'pager2_slide_hide' => 1,
        ),
      ),
      'slide_text' => 1,
      'slide_text_settings' => array(
        'slide_text_container' => 'div.slide-text',
        'slide_text_position' => 'bottom',
        'slide_text_jquery' => 0,
        'slide_jquery' => array(
          'slide_text_before_effect' => 'hide',
          'slide_text_before_speed' => '500',
          'slide_text_after_effect' => 'show',
          'slide_text_after_speed' => '500',
        ),
      ),
      'custom' => array(
        'custom_jquery' => '',
      ),
    ),
  ),
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

  $views[$view->name] = $view;

  return $views;
}
