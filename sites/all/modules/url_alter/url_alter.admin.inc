<?php
// $Id: url_alter.admin.inc,v 1.4 2009/03/21 19:19:32 davereid Exp $

/**
 * @file
 * Administrative page callbacks for the url_alter module.
 */

function url_alter_settings() {
  $form['url_alter_inbound'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP code for custom_url_rewrite_inbound()'),
    '#description' => t('The available variables are %variables.', array('%variables' => '&$result, $path, $path_language')),
    '#default_value' => url_alter_var('inbound'),
  );
  $form['url_alter_outbound'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP code for custom_url_rewrite_outbound()'),
    '#description' => t('The available variables are %variables.', array('%variables' => '&$path, &$options, $original_path')),
    '#default_value' => url_alter_var('outbound'),
  );

  return system_settings_form($form);
}

function url_alter_settings_validate($form, $form_state) {
  _url_alter_validate_php('url_alter_inbound', $form_state['values']['url_alter_inbound']);
  _url_alter_validate_php('url_alter_outbound', $form_state['values']['url_alter_outbound']);
}

function _url_alter_validate_php($field, &$code) {
  // Define some variables used in the code.
  $result = $path = $path_language = $options = $original_path = '';
  $error = FALSE;

  $code = trim($code);

  if (!strlen($code)) {
    return;
  }
  elseif (preg_match('/<\?php|\?>/', $code, $regs)) {
    $error = t('Do not use the PHP tag %code in your code.', array('%code' => $regs[0]));
  }
  else {
    ob_start();
    $result = @eval($code); // Prepend 'return TRUE;' to $code?
    ob_end_clean();
    if ($result !== NULL) {
      $error = is_string($result) ? $result : TRUE;
    }
  }

  if ($error) {
    form_set_error($field, is_string($error) ? $error : t('Code did not pass validation'));
  }
}
