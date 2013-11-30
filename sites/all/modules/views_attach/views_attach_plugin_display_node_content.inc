<?php
// $Id: views_attach_plugin_display_node_content.inc,v 1.1.2.7.2.7 2009/10/02 21:12:01 crell Exp $

/**
 * The plugin that handles node-attached views.
 */
class views_attach_plugin_display_node_content extends views_plugin_display {
  function option_definition () {
    $options = parent::option_definition();

    $options['types'] = array('default' => array());
    $options['modes'] = array('default' => array('full'));
    $options['argument_mode'] = array('default' => 'nid');
    $options['default_argument'] = array('default' => '');
    $options['show_title'] = 0;

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['node_content'] = array(
      'title' => t('Node content settings'),
    );

    $types = $this->get_option('types');
    if (empty($types)) {
      $types = array('story');
    }

    $options['types'] = array(
      'category' => 'node_content',
      'title' => t('Node types'),
      'value' => implode(', ', $types),
    );

    $used_modes = $this->get_option('modes');
    // Get the human readable names.
    $modes = views_attach_build_modes();
    foreach ($used_modes as $mode) {
      $display_modes[] = $modes[$mode];
    }
    $options['modes'] = array(
      'category' => 'node_content',
      'title' => t('Build modes'),
      'value' => implode(', ', $display_modes),
    );

    $weight = $this->get_option('weight');
    if (empty($weight)) {
      $weight = 10;
    }

    $argument_mode = $this->get_option('argument_mode');
    $options['arguments'] = array(
      'category' => 'node_content',
      'title' => t('Arguments'),
      'value' => empty($argument_mode) ? t('None') : check_plain($argument_mode),
    );

    $options['show_title'] = array(
      'category' => 'node_content',
      'title' => t('Show title'),
      'value' => $this->get_option('show_title') ? t('Yes') : t('No'),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'types':
        $form['#title'] .= t('Node types');
        $form['types'] = array(
          '#type' => 'checkboxes',
          '#multiple' => TRUE,
          '#required' => TRUE,
          '#title' => t("Embed this display in the following node types"),
          '#options' => node_get_types('names'),
          '#default_value' => $this->get_option('types'),
        );
        break;

      case 'modes':
        $form['#title'] .= t('Build modes');
        $form['modes'] = array(
          '#type' => 'checkboxes',
          '#title' => t("Embed this display in the following build modes"),
          '#options' => views_attach_build_modes(),
          '#default_value' => $this->get_option('modes'),
         );
         break;

      case 'arguments':
        $form['#title'] .= t('Arguments');
        $default = $this->get_option('argument_mode');
        $options = array(
          'none' => t("No special handling"),
          'nid' =>  t("Use the ID of the node the view is attached to"),
        );

        $form['argument_mode'] = array(
          '#type' => 'radios',
          '#title' => t("How should this display populate the view's arguments?"),
          '#options' => $options,
          '#default_value' => $default,
        );

        // Add the extra option for Tokens if the module is enabled.
        // If it isn't, ensure that we dont' default to 'token'.
        if (module_exists('token')) {
          $form['argument_mode']['#options']['token'] = t("Use tokens from the node the view is attached to");
          $form['token_prefix'] = array(
            '#id' => 'views-attached-token-arguments',
            '#type' => 'hidden',
            '#prefix' => '<div><div id="views-attached-token-arguments">',
            '#process' => array('views_process_dependency'),
            '#dependency' => array(
              'radio:argument_mode' => array('token')
            ),
          );
          $form['default_argument'] = array(
            '#type' => 'textfield',
            '#default_value' => $this->get_option('default_argument'),
            '#description' => t('You may use token replacement to provide arguments based on the current node. Separate arguments with "/".'),
          );
          $form['token_help'] = array(
            '#type' => 'fieldset',
            '#title' => t('Replacement tokens'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#description' => theme('token_help', 'node'),
          );
          $form['token_suffix'] = array(
            '#value' => '</div></div>',
          );
        }
        elseif ($default == 'token') {
          $form['argument_mode']['#default_value'] = 'none';
        }
        break;

      case 'show_title':
        $form['#title'] .= t('Show title');
        $form['show_title'] = array(
          '#type' => 'checkbox',
          '#title' => t('Show the title of the view above the attached view.'),
          '#default_value' => $this->get_option('show_title'),
        );
        break;
    }

  }

  function options_submit($form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'types':
        $this->set_option('types', array_filter($form_state['values']['types']));
        break;
      case 'modes':
        $this->set_option('modes', array_values(array_filter($form_state['values']['modes'])));
        break;
      case 'arguments':
        $this->set_option('argument_mode', $form_state['values']['argument_mode']);
        if ($form_state['values']['argument_mode'] == 'token') {
          $this->set_option('default_argument', $form_state['values']['default_argument']);
        }
        else {
          $this->set_option('default_argument', NULL);
        }
        break;
      case 'show_title':
        $this->set_option('show_title', $form_state['values']['show_title']);
        break;
    }
  }

  /**
   * Node content views use exposed widgets only if AJAX is set.
   */
  function uses_exposed() {
    if ($this->use_ajax()) {
      return parent::uses_exposed();
    }
    return FALSE;
  }

  /**
   * We have to run token replacement before the arguments are used.
   */
  function pre_execute() {
    // Call the parent setup function so we do not lose data.
    parent::pre_execute();

    $node = $this->view->current_node;
    $node_types = $this->view->display_handler->get_option('types');
    if (!in_array($node->type, $node_types)) {
      return;
    }

    $arg_mode = $this->view->display_handler->get_option('argument_mode');
    if (module_exists('token') && $arg_mode == 'token') {
      if ($token_string = $this->view->display_handler->get_option('default_argument')) {
        // Now do the token replacement.
        $token_values = views_attach_get_arguments_from_token_string($token_string, 'node', $node);
        $new_args = array();
        // We have to be careful to only replace arguments that have tokens.
        foreach ($token_values as $key => $value) {
          $new_args[$key] = $value;
        }

        $this->view->args = $new_args;
      }
    }
    elseif ($arg_mode == 'nid') {
      $this->view->args = array($node->nid);
    }
  }

  /**
   * The display block handler returns the structure necessary for a block.
   */
  function execute() {
    // Prior to this being called, the $view should already be set to this
    // display, and arguments should be set on the view.
    if (!isset($this->view->override_path)) {
      $this->view->override_path = $_GET['q'];
    }

    $data = $this->view->render();
    if (!empty($this->view->result) || $this->get_option('empty') || !empty($this->view->style_plugin->definition['even empty'])) {
      return $data;
    }
  }
}
