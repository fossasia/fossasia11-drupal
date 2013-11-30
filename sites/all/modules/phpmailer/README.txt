
-- SUMMARY --

Adds SMTP support for sending e-mails using the PHPMailer library.

For a full description of the module, visit the project page:
  http://drupal.org/project/phpmailer
To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/phpmailer


-- REQUIREMENTS --

* Access to an SMTP server

* Libraries API module
  http://drupal.org/project/libraries

* PHPMailer for PHP5/6
  http://phpmailer.codeworxtech.com

Optional:

* To connect to an SMTP server using SSL, your PHP installation needs to have
  OpenSSL support.

* Mime Mail module to send HTML e-mails
  http://drupal.org/project/mimemail

* Personalized E-mails module to adjust the displayed sender name
  http://drupal.org/project/pmail


-- INSTALLATION --

1. Download PHPMailer for PHP5/6 from

     http://sourceforge.net/projects/phpmailer/files/phpmailer%20for%20php5_6/

   and extract the following files to the phpmailer subdirectory of your
   libraries directory:

     class.phpmailer.php
     class.smtp.php

   Be careful NOT to extract the path names contained in the archive.

   It is recommended that you create your libraries directory at one of the
   following locations:

     sites/all/libraries
     sites/<domain>/libraries

   Consult the Libraries API documentation for further information.

2. Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions at Administer >> User management >> Access
  control >> phpmailer module.

  Only users with the "administer phpmailer settings" permission are allowed to
  access the module configuration page.

* Configure your SMTP server settings at Administer >> Site
  configuration >> PHPMailer.

  If used in conjunction with Mime Mail, PHPMailer will show up as an e-mail
  engine in the Mime Mail module settings.  To let Mime Mail apply styles and
  formatting to system e-mails but still use PHPMailer for mail transport,
  uncheck "Use PHPMailer to send e-mails" first on the PHPMailer settings
  page.  Then activate "Use mime mail for all messages" on the Mime Mail
  settings page and choose PHPMailer from the list of available e-mail engines.

* Using Google Mail as SMTP server

  To send e-mails with Google Mail use the following settings:

    SMTP server:     smtp.gmail.com
    SMTP port:       465
    Secure protocol: SSL
    Username:        your_google_mail_name@gmail.com
    Password:        your_google_mail_password

  In Advanced SMTP settings:
    Enable 'Always set "Reply-To" address' checkbox.

  Also note the sending limits for Google Mail accounts:
  http://mail.google.com/support/bin/answer.py?hl=en&answer=22839

* Using other popular freemailers as SMTP server

  See http://phpmailer.worxware.com/index.php?pg=tip_srvrs.

* Errors when trying to send a test email

  If you see the following error messages when trying to send a test email from
  the settings page:

    SMTP -> ERROR: Failed to connect to server: Connection timed out (110)
    SMTP Error: Could not connect to SMTP host. 

  it means the mail server can not be reached, usually because your hosting
  provider is blocking the port by a firewall. The solution is to ask your
  provider to allow outgoing connections to your mail server.

* Debug settings

  PHPMailer supports rerouting all e-mails for debugging purposes, to prevent
  you from accidentally sending out e-mails to real users from a development
  site.  To enable this feature, add the following lines to the end of your
  settings.php (usually located in sites/default):

    $conf['phpmailer_debug_email'] = 'your_debug_email@yoursite.com';

  This will change the recipient of all e-mails to the configured address.


-- CREDITS --

Authors:
* Stefan M. Kudwien (smk-ka) - http://drupal.org/user/48898
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136

This project has been sponsored by UNLEASHED MIND.
