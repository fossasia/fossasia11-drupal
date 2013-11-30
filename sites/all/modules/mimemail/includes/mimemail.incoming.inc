<?php /* $Id: mimemail.incoming.inc,v 1.1 2009/02/22 20:55:46 vauxia Exp $ */

/**
 * @file
 * Functions that handle inbound messages to mimemail.
 */

/**
 * Receive messages POSTed from an external source.
 *
 * This function enables messages to be sent via POST or some other RFC822
 * source input (e.g. directly from a mail server).
 *
 * @return
 *   The POSTed message.
 */
function mimemail_post() {
  $message = $_POST['message'];
  $token   = $_POST['token'];
  $hash    = md5(variable_get('mimemail_key', '**') . $message );

  if ($hash != $token) {
    watchdog('access denied', 'Authentication error for POST e-mail', WATCHDOG_WARNING);
    return drupal_access_denied();
  }
  return mimemail_incoming($message);
}

/**
 * Parses an externally received message.
 *
 * @param $message
 *   The message to parse.
 */
function mimemail_incoming($message) {
  require_once dirname(__FILE__) .'/mimemail.inc';
  $mail = mimemail_parse($message);

  foreach (module_implements('mimemail_incoming_alter') as $module) {
    call_user_func_array($module .'_mimemail_incoming_alter', $mail);
  }

  module_invoke_all('mimemail_incoming', $mail);
}
