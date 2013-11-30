// $Id: README.txt,v 1.5 2009/10/24 06:16:43 davereid Exp $

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Developers
 * Frequently Asked Questions (FAQ)
 * Known Issues
 * More Information
 * How Can You Contribute?


INTRODUCTION
------------

Current Maintainer: Dave Reid <dave@davereid.net>

Utility module that adds new hook_url_alter() hooks for other modules to
implement. Also replaces the need for custom_url_rewrite() functions in
settings.php.


INSTALLATION
------------

See http://drupal.org/getting-started/5/install-contrib for instructions on
how to install or update Drupal modules.

If your site has custom_url_rewrite functions in your site's settings.php, you
should remove them before installing the module. It should warn you if you have
the functions needing removal. Once you've removed the functions, you should
enter the code from inside the functions into the Url alter module settings at
admin/settings/url-alter.


DEVELOPERS
----------

Instead of implementing custom_url_rewrite_outbound() and
custom_url_rewrite_inbound(), your module should implement hook_url_*_alter().
This will make your modules compatible with other modules that need to rewrite
paths. For the hook documentation, see the include url_alter.api.php.

You can even write your code so that url_alter is used if it is active,
otherwise fallback to the custom_url_rewrite equivalents. For example put the
following code in your module's .module file. Adjust appropriately for
custom_url_rewrite_inbound().

// Define the custom_url_rewrite_outbound() function if not already defined.
if (!function_exists('custom_url_rewrite_outbound')) {
  function custom_url_rewrite_outbound(&$path, &$options, $original_path) {
    mymodule_url_outbound_alter($path, $options, $original_path);
  }
}

/**
 * Implementation of hook_url_outbound_alter().
 */
function mymodule_url_outbound_alter(&$path, &$options, $original_path) {
  // Perform your alterations here.
}


FREQUENTLY ASKED QUESTIONS (FAQ)
--------------------------------

Q: What if I have my own custom_url_rewrite functions in my settings.php?
A: Url alter will not work unless you remove those functions. Luckily, you can
   copy the code inside those functions and paste them into the Url alter
   module settings (admin/settings/url-alter).

Q: Help! I put in invalid PHP code in admin/settings/url-alter!
A: If you add ?url-alter-kill to any URL on your site, it should temporarily
   disable the module. You can then go to
   admin/settings/url-alter?url-alter-kill to adjust your PHP code.


KNOWN ISSUES
------------

- There are no known issues at this time.


MORE INFORMATION
----------------

- To issue any bug reports, feature or support requests, see the module issue
  queue at http://drupal.org/project/issues/url_alter.


HOW CAN YOU CONTRIBUTE?
---------------------

- Write a review for this module at drupalmodules.com.
  http://drupalmodules.com/module/url-alter

- Help translate this module.
  http://localize.drupal.org/translate/projects/url_alter

- Report any bugs, feature requests, etc. in the issue tracker.
  http://drupal.org/project/issues/url_alter

- Contact the maintainer with any comments, questions, or feedback.
  http://davereid.net/contact
