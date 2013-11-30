<?php 
// $Id: mimemail-message.tpl.php,v 1.3 2010/09/12 16:35:48 sgabe Exp $

/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[mailkey].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $mailkey: The message identifier
 *
 * @see template_preprocess_mimemail_message()
 */
?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <?php if ($css): ?>
    <style type="text/css">
      <!--
      <?php print $css ?>
      -->
    </style>
    <?php endif; ?>
  </head>
  <body id="mimemail-body" <?php if ($mailkey): print 'class="'. $mailkey .'"'; endif; ?>>
    <div id="center">
      <div id="main">
        <?php print $body ?>
      </div>
    </div>
  </body>
</html>
