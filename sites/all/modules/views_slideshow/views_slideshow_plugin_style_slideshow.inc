<?php

/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render each item in a slideshow of an ordered or unordered list.
 *
 * @ingroup views_style_plugins
 */
class views_slideshow_plugin_style_slideshow extends views_plugin_style_list {
  // if the view is still using the old variables replace with the new ones.
  function init(&$view, &$display, $options = NULL) {
    // These are required for the view to continue to work.
    $this->view = &$view;
    $this->display = &$display;

    // Overlay incoming options on top of defaults
    $this->unpack_options($this->options, isset($options) ? $options : $display->handler->get_option('style_options'));

    if ($this->uses_row_plugin() && $display->handler->get_option('row_plugin')) {
      $this->row_plugin = $display->handler->get_plugin('row');
    }

    // Eveything below here is what's needed for views slideshow.
    if (!empty($options['mode']) && in_array($options['mode'], array('singleframe', 'thumbnailhover', 'menu', 'slider', 'ddblock')) && is_array($options[$options['mode']])) {
      foreach ($options[$options['mode']] as $index => $value) {
        $this->options['views_slideshow_' . $options['mode']][$index] = $value;
      }
    }
  }

  // Set default options
  function option_definition() {
    module_load_all_includes('views_slideshow.inc');
    $options = parent::option_definition();

    $options = array_merge($options, module_invoke_all('views_slideshow_option_definition'));
    $options['mode'] = array('default' => '');

    return $options;
  }

  // Render the given style.
  function options_form(&$form, &$form_state) {
    module_load_all_includes('views_slideshow.inc');
    parent::options_form($form, $form_state);

    $modules = module_invoke_all('views_slideshow_modes');

    if ($modules) {
      $form['mode'] = array(
        '#type' => 'select',
        '#title' => t('Slideshow mode'),
        '#options' => $modules,
        '#default_value' => $this->options['mode'],
      );
      foreach (module_implements('views_slideshow_options_form') as $module) {
        // We wrap our fieldsets in a div so we can use dependent.js to
        // show/hide our fieldsets.
        $form[$module . '-prefix'] = array(
          '#type' => 'hidden',
          '#id' => $module . '-options-wrapper',
          '#prefix' => '<div><div id="' . $module .'-options-wrapper">',
          '#process' => array('views_process_dependency'),
          '#dependency' => array('edit-style-options-mode' => array($module)),
        );

        $form[$module] = array(
          '#type' => 'fieldset',
          '#title' => t( $modules[$module] . ' options'),
          '#collapsible' => TRUE,
          '#attributes' => array('class' => $module),
        );
        $function = $module .'_views_slideshow_options_form';
        call_user_func_array($function, array(&$form, &$form_state, &$this));

        $form[$module . '-suffix']  = array(
          '#value' => '</div></div>',
        );
      }
    }
    else {
      $form['enable_module'] = array(
        '#value' => t('There is no Views Slideshow plugin enabled. Go to the !modules and enable a Views Slideshow plugin module. For example Views Slideshow Singleframe.', array('!modules' => l('Modules Page', 'admin/build/modules'))),
      );
    }
  }

  function options_validate(&$form, &$form_state) {
    module_load_all_includes('views_slideshow.inc');
    foreach (module_implements('views_slideshow_options_form_validate') as $module) {
      $function = $module . '_views_slideshow_options_form_validate';
      call_user_func_array($function, array(&$form, &$form_state, &$this));
    }
  }
}
