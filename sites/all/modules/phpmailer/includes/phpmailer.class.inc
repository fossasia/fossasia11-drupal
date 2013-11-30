<?php

/**
 * @file
 * Implements the base PHPMailer for Drupal class.
 */

/**
 * Base PHPMailer for Drupal implementation with support for SMTP keep-alive
 * and setting a custom Return-Path.
 */
class DrupalPHPMailer extends PHPMailer {
  /**
   * Stores the Return-Path, which may be different from Sender.
   */
  public $ReturnPath = '';

  /**
   * Constructor.
   */
  public function __construct() {
    // Throw exceptions instead of dying (since 5.0.0).
    if (method_exists(get_parent_class($this), '__construct')) {
      parent::__construct(TRUE);
    }

    $this->IsSMTP();
    $this->Reset();

    $this->Host = variable_get('smtp_host', '');
    if ($backup = variable_get('smtp_hostbackup', '')) {
      $this->Host .= ';' . $backup;
    }
    $this->Port = variable_get('smtp_port', '25');
    $this->SMTPSecure = variable_get('smtp_protocol', '');

    // Use SMTP authentication if both username and password are given.
    $this->Username = variable_get('smtp_username', '');
    $this->Password = variable_get('smtp_password', '');
    $this->SMTPAuth = (bool)($this->Username != '' && $this->Password != '');

    $this->SMTPKeepAlive = variable_get('smtp_keepalive', 0);
    $this->SMTPDebug = variable_get('smtp_debug', 0);

    // Adjust path to SMTP class.
    $this->PluginDir = './'. phpmailer_get_path() .'/';
  }

  /**
   * Send mail via SMTP.
   *
   * Wrapper around PHPMailer::SmtpSend() with exception handling.
   */
  public function SmtpSend($header, $body) {
    if ($this->SMTPDebug) {
      ob_start();
    }

    try {
      $result = parent::SmtpSend($header, $body);

      // Close connection when not using SMTP keep-alive.
      if (!$this->SMTPKeepAlive) {
        $this->SmtpClose();
      }
    }
    catch (phpmailerException $exception) {}

    if ($this->SMTPDebug) {
      if ($debug = ob_get_contents()) {
        drupal_set_message($debug);
      }
      ob_end_clean();
    }

    // Reinitialize properties.
    $this->Reset();

    if (isset($exception)) {
      // Pass exception to caller.
      throw $exception;
    }
    return $result;
  }

  /**
   * (Re-)initialize properties after sending mail.
   */
  public function Reset() {
    $this->ClearAllRecipients();
    $this->ClearReplyTos();
    $this->ClearAttachments();
    $this->ClearCustomHeaders();

    $this->Priority    = 3;
    $this->CharSet     = variable_get('smtp_charset', 'utf-8');
    $this->ContentType = 'text/plain';
    $this->Encoding    = '8bit';

    // Set default From name.
    $from_name = variable_get('smtp_fromname', '');
    if ($from_name == '') {
      // Fall back on the site name.
      $from_name = variable_get('site_name', 'Drupal');
    }
    $this->FromName   = $from_name;
    $this->Sender     = '';
    $this->MessageID  = '';
    $this->ReturnPath = '';
  }

  /**
   * Destructor.
   */
  public function __destruct() {
    // Be nice and close the connection when using SMTP keep-alive.
    if ($this->SMTPKeepAlive) {
      $this->SmtpClose();
    }
  }

  /**
   * Provide more user-friendly error messages.
   *
   * Note: messages should not end with a dot.
   */
  public function SetLanguage($langcode = 'en', $lang_path = 'language/') {
    $this->language = array(
      'provide_address'     => t('You must provide at least one recipient e-mail address'),
      'encoding'            => t('Unknown encoding: '),
      'file_open'           => t('Could not open file: '),
      'signing'             => t('Signing error: '),
      'empty_message'       => t('Message body empty'),
      'tls'                 => t('SMTP error: STARTTLS not accepted from server'),
      'authenticate'        => t('SMTP error: could not authenticate'),
      'smtp_connect_failed' => t('SMTP error: could not connect to SMTP host'),
      'connect_host'        => t('SMTP error: could not connect to SMTP host'),
      'from_failed'         => t('The following sender address failed: '), // non-admin
      'recipients_failed'   => t('The following recipient addresses failed: '), // non-admin
      'data_not_accepted'   => t('SMTP error: data not accepted'),
      'smtp_error'          => t('SMTP server error: '),

      // Unused messages.
      //'execute'           => t('Could not execute: '),
      //'instantiate'       => t('Could not instantiate mail() function.'),

      // Messages used during email generation.
      'file_access'         => t('Could not access file: '),
      'invalid_address'     => t('Invalid address'),
      'variable_set'        => t('Cannot set or reset variable: '),
    );
    return TRUE;
  }

  /**
   * Assemble the message header.
   *
   * PHPMailer always sets Return-Path to Sender, we want more flexibility.
   */
  public function CreateHeader() {
    $old_sender = $this->Sender;
    if ($this->ReturnPath != '') {
      $this->Sender = $this->ReturnPath;
    }
    $result = parent::CreateHeader();
    // Restore sender for use in MAIL FROM command.
    $this->Sender = $old_sender;
    return $result;
  }

  /**
   * Public wrapper around PHPMailer::RFCDate().
   */
  public static function RFCDate() {
    $tz = date('Z');
    $tzs = ($tz < 0) ? '-' : '+';
    $tz = abs($tz);
    $tz = (int)($tz / 3600) * 100 + ($tz % 3600) / 60;
    $result = sprintf("%s %s%04d", date('D, j M Y H:i:s'), $tzs, $tz);

    return $result;
  }
}

