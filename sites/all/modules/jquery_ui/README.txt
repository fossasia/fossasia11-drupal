
-- SUMMARY --

jQuery UI (http://ui.jquery.com/) is a set of cool widgets and effects that
developers can use to add some pizazz to their modules.

This module is more-or-less a utility module that should simply be required by
other modules that depend on jQuery UI being available. It doesn't do anything
on its own.

For a full description of the module, visit the project page:
  http://drupal.org/project/jquery_ui

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/jquery_ui


-- REQUIREMENTS --

* The jQuery UI library.


-- INSTALLATION --

* Copy the jquery_ui module directory to your sites/all/modules directory, so it
  is located in sites/all/modules/jquery_ui/.

* Download the jQuery UI 1.6 release from:

    http://code.google.com/p/jquery-ui/downloads/list?q=1.6

* Put the downloaded archive into the directory:

    /sites/all/libraries/jquery.ui-1.6.zip

* Extract the archive.  This will create the following sub-directory:

    /sites/all/libraries/jquery.ui-1.6/

* Rename the sub-directory into "jquery.ui":

    /sites/all/libraries/jquery.ui/

  so the actual jQuery UI JavaScript files are located in:

    /sites/all/libraries/jquery.ui/ui/*.js

* Enable the module at Administer >> Site building >> Modules.


-- JQUERY UI 1.7 --

The jQuery UI module uses jQuery UI 1.6 because jQuery UI 1.7 requires at least
jQuery 1.3, which is not shipped with Drupal 6. If you absolutely need to move
to jQuery UI 1.7, you can get around this by doing the following:

* Download and install the corresponding jQuery Update module from:

    http://drupal.org/project/jquery_update

* Download the latest jQuery UI 1.7 release from:

    http://code.google.com/p/jquery-ui/downloads/list?q=1.7

* Put the downloaded archive into the directory:

    /sites/all/libraries/jquery.ui-1.7.zip

* Extract the archive.  This will create the following sub-directory:

    /sites/all/libraries/jquery.ui-1.7/

* Rename the sub-directory into "jquery.ui":

    /sites/all/libraries/jquery.ui/

  so the actual jQuery UI JavaScript files are located in:

    /sites/all/libraries/jquery.ui/ui/*.js

* Enable the module at Administer >> Site building >> Modules.


-- API --

Developers who wish to use jQuery UI effects in their modules need only make
the following changes:

* In your module's .info file, add the following line:

    dependencies[] = jquery_ui

  This will force users to have the jQuery UI module installed before they can
  enable your module.

* In your module, call the following function:

    jquery_ui_add($files);

  For example:

    jquery_ui_add(array('ui.draggable', 'ui.droppable', 'ui.sortable'));

    jquery_ui_add('ui.sortable');  // For a single file

  See the contents of the jquery.ui-X.X sub-directory for a list of available
  files that may be included, and see http://ui.jquery.com/docs for details on
  how to use them. The required ui.core file is automatically included, as is
  effects.core if you include any effects files.

-- CONTACT --

Current maintainers:
* Jeff Robbins (jjeff)
* Angela Byron (webchick)
* Addison Berry (add1sun)
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136

