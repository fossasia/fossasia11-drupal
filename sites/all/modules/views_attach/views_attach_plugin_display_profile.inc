<?php
// $Id: views_attach_plugin_display_profile.inc,v 1.1.2.1.2.9 2009/10/08 22:03:22 crell Exp $

/**
 * The plugin that handles a user profile.
 */
class views_attach_plugin_display_profile extends views_plugin_display {
  function option_definition () {
    $options = parent::option_definition();

    $options['weight'] = array('default' => 10);
    $options['category'] = array('default' => NULL);
    $options['argument_mode'] = array('default' => 'uid');
    $options['default_argument'] = array('default' => '');

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

    $categories['profile'] = array(
      'title' => t('Profile settings'),
    );

    $category = $this->get_option('category');
    if (empty($category)) {
      $category = t('Default');
    }

    $options['category'] = array(
      'category' => 'profile',
      'title' => t('Category'),
      'value' => $category,
    );

    $weight = $this->get_option('weight');
    if (empty($weight)) {
      $weight = 10;
    }

    $options['weight'] = array(
      'category' => 'profile',
      'title' => t('Weight'),
      'value' => $weight,
    );

    $argument_mode = $this->get_option('argument_mode');
    $options['arguments'] = array(
      'category' => 'profile',
      'title' => t('Arguments'),
      'value' => empty($argument_mode) ? t('None') : check_plain($argument_mode),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'weight':
        $form['#title'] .= t('Weight');
        $form['weight'] = array(
          '#type' => 'weight',
          '#description' => t('The position of this view in relation to other profile elements.'),
          '#default_value' => $this->get_option('weight'),
        );
        break;
      case 'category':
        $form['#title'] .= t('Category');
        $form['category'] = array(
          '#type' => 'textfield',
          '#description' => t('The name of the profile tab this view should be listed in. If empty, it will be displayed on the main profile tab.'),
          '#default_value' => $this->get_option('category'),
        );
        break;
      case 'arguments':
        $form['#title'] .= t('Arguments');
        $default = $this->get_option('argument_mode');
        $options = array(
          'none' => t("No special handling"),
          'uid' =>  t("Use the ID of the user the view is attached to"),
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
          $form['argument_mode']['#options']['token'] = t("Use tokens from the user the view is attached to");
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
            '#description' => t('You may use token replacement to provide arguments based on the current user profile. Separate arguments with "/".'),
          );
          $form['token_help'] = array(
            '#type' => 'fieldset',
            '#title' => t('Replacement tokens'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#description' => theme('token_help', 'user'),
          );
          $form['token_suffix'] = array(
            '#value' => '</div></div>',
          );
        }
        elseif ($default == 'token') {
          $form['argument_mode']['#default_value'] = 'none';
        }
        break;
    }
  }

  function options_submit($form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'weight':
        $this->set_option('weight', $form_state['values']['weight']);
        break;
      case 'category':
        $this->set_option('category', $form_state['values']['category']);
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
    }
  }

  /**
   * Profile views use exposed widgets only if AJAX is set.
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
    $account = $this->view->current_account;
    $arg_mode = $this->view->display_handler->get_option('argument_mode');
    if (module_exists('token') && $arg_mode == 'token') {
      if ($token_string = $this->view->display_handler->get_option('default_argument')) {
        // Now do the token replacement.
        $token_values = views_attach_get_arguments_from_token_string($token_string, 'user', $account);
        $new_args = array();
        // We have to be careful to only replace arguments that have tokens.
        foreach ($token_values as $key => $value) {
          $new_args[$key] = $value;
        }

        $this->view->args = $new_args;
      }
    }
    elseif ($arg_mode == 'uid') {
      $this->view->args = array($account->uid);
    }
  }

  /**
   * The display block handler returns the structure necessary for a block.
   */
  function execute() {
    // Prior to this being called, the $view should already be set to this
    // display, and arguments should be set on the view.
    $data = $this->view->render();
    if (!empty($this->view->result) || $this->get_option('empty') || !empty($this->view->style_plugin->definition['even empty'])) {
      return $data;
    }
  }
}
