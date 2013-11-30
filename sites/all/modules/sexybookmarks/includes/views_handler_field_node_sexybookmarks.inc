<?php
/**
 * @file
 * Field handler to present SexyBookmarks.
 */

class views_handler_field_node_sexybookmarks extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['title'] = 'title';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['profile'] = array(
      'default' => 'default',
      'translatable' => FALSE
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    if (!module_exists('ctools')) {
      return;
    }

    $options = array();
    ctools_include('export');
    foreach (ctools_export_crud_load_all('sexybookmarks_profiles') as $profile) {
      if (empty($profile->disabled)) {
        $options[$profile->name] = $profile->name;
      }
    }
    $form['profile'] = array(
      '#type' => 'select',
      '#title' => t('SexyBookmarks profile'),
      '#empty_value' => '',
      '#options' => $options,
      '#default_value' => $this->options['profile'],
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    return theme('sexybookmarks', array(
      'url' => url('node/' . $values->{$this->aliases['nid']}, array('absolute' => TRUE)),
      'title' => $values->{$this->aliases['title']},
      'profile' => $this->options['profile'],
    ));
  }
}
