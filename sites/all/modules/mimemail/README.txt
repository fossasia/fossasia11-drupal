$Id: README.txt,v 1.2 2010/09/12 17:58:46 sgabe Exp $

INSTALLATION
  Hopefully, you know the drill by now :)
  1. Download the module and extract the files.
  2. Upload the entire mimemail folder into your Drupal sites/all/modules/
     or sites/my.site.folder/modules/ directory if you are running a multi-site
     installation of Drupal and you want this module to be specific to a
     particular site in your installation.
  3. Enable the Mime Mail module by navigating to:
     Administer > Site building > Modules
  4. Adjust settings by navigating to:
     Administer > Site configuration > Mime Mail

USAGE
  This module may be required by other modules, but is not terribly
  useful by itself. Once installed, any module can send messages by
  calling the mimemail() function:

  $sender      - a user object, text email address or an array with name, mail
  $recipient   - a user object, text email address or an array with name, mail
  $subject     - subject line
  $body        - body text in HTML format
  $plaintext   - boolean, whether to send messages in plaintext-only (default FALSE)
  $headers     - a keyed array with headers (optional)
  $text        - plaintext portion of a multipart e-mail (optional)
  $attachments - array of arrays with the file's path, MIME type (optional)
  $mailkey     - message identifier

  return       - an array containing the MIME encoded message

  This module creates a user preference for receiving plaintext-only messages.
  This preference will be honored by all calls to mimemail() if the format is not
  explicitly set.

  E-mail messages are formatted using the mimemail-message.tpl.php template.
  This includes a CSS style sheet and uses an HTML version of the text.
  The included CSS is either:
    the mail.css file found in your default theme or
    the combined CSS style sheets of your default theme.

  To create a custom mail template copy the mimemail-message.tpl.php file from
  the mimemail/theme directory into your default theme's folder. Both general and
  by-mailkey theming can be performed:
    mimemail-message.tpl.php (for all messages)
    mimemail-message--[mailkey].tpl.php (for messages with a specific mailkey)
  Note that if you are using a different administration theme than your default theme,
  you should place the same template files into that theme folder too.

  Images with absolute path will be available as remote content. To embed images
  into emails you have to use relative paths.
  For example:
    instead of http://www.mysite.com/sites/default/files/mypicture.jpg
    use /sites/default/files/mypicture.jpg

  Since some email clients (namely Outlook 2007 and GMail) is tend to only regard
  inline CSS, you can use the Compressor to convert CSS styles into inline style
  attributes. It transmogrifies the HTML source by parsing the CSS and inserting the
  CSS definitions into tags within the HTML based on the CSS selectors. To use the
  Compressor, just enable it.

CREDITS

  MAINTAINER: Allie Micka < allie at pajunas dot com >

  * Allie Micka
    Mime enhancements and HTML mail code

  * Gerhard Killesreiter
    Original mail and mime code

  * Robert Castelo
    HTML to Text and other functionality

