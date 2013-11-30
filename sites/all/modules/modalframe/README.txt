;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Modal Frame API module for Drupal 6
;; $Id: README.txt,v 1.1.2.11 2010/05/14 03:10:39 markuspetrux Exp $
;;
;; Original author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CONTENTS OF THIS FILE
=====================
- OVERVIEW
- REQUIREMENTS
- INSTALLATION
- API REFERENCE


OVERVIEW
========

This module provides an API to render an iframe within a modal dialog based on
the jQuery UI Dialog plugin. You should not install this module unless another
module requires you to, or you wish to use it for your own custom modules.

It is an alternative to Popups API (Ajax Dialogs) [1] which implements its own
library and API to manage popup dialogs. On the other hand, the Modal Frame API
is based on the jQuery UI Dialog plugin and it is specially built to deal with
iframe elements with very little effort (and no particular AJAX requirements).

For themers, this module provides a default template for rendering child pages
within modal frames (modalframe-page.tpl.php) that can be copied to the theme
directory and customized to suit the particular needs of the site. It is also
pretty easy to adapt the provided stylesheets to match the look of the site.

[1] http://drupal.org/project/popups


REQUIREMENTS
============

- jQuery UI module (with jQuery UI library 1.7.x).
  http://drupal.org/project/jquery_ui

  * Note that you also need jQuery 1.3.x, currently provided by the jQuery
    Update module 6.x-2.x. See http://drupal.org/project/jquery_update

- The onBeforeUnload API is not required but recommended. It enhances
  synchronization of parent/child windows.

  http://drupal.org/project/onbeforeunload

- The Dirty Forms module is not required, but supported. It warns users
  if they are about to close a modal dialog with unsaved changes.

  http://drupal.org/project/dirtyforms


INSTALLATION
============

- Be sure to install all dependent modules.

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Go to Administer -> Site building -> Modules to install module.

- Done.


API REFERENCE
=============

Modal Frame API is pretty simple, but powerful at the same time.

The server-side API looks as follows:

* modalframe_parent_js()

  Adds the Modal Frame javascript and stylesheets to the parent page. You
  should use this when you wish to use the client-side API to manage frames
  within a modal dialog.


* modalframe_child_js()

  Adds the Modal Frame javascript and stylesheets to the child page. This is
  necessary for pages and/or forms that are rendered within a modal frame.


* modalframe_close_dialog($args)

  When a form is rendered within a modal frame (what we would call the child
  window), you can use this function from your form submit handler to close
  the client-side dialog. You can also give it arguments that will be passed
  to an optional onSubmit callback that you can provide when opening the modal
  frame from the parent window.


The client-side API looks as follows:

* Drupal.modalFrame.open(options)

  Use this method to open a modal frame. The argument is a javascript object
  that may contain the following elements:

  o url (Required)

    The URL of the page you wish to render within the modal frame. Note that
    this feature is restricted by browser security contexts, so you can only
    open child pages from the same domain where the parent page is located.

  o width and height (Optional)

    The size in pixels of the dialog.

  o autoFit (Optional, enabled by default)

    This option activates an onResize handler that will automatically resize
    and center the modal frame whenever the browser window is resized or when
    the child document is fully loaded.

  o draggable (Optional)

    This option is enabled by default. Use false to disable.

  o onOpen (Optional)

    This is a callback to invoke when the modal frame is opened.

  o onLoad (Optional)

    This is a callback to invoke when the the child document in the modal frame
    is fully loaded.

  o onSubmit (Optional)

    A function that will be called when the dialog is closed by the server-side
    response to a form submit request. See modalframe_close_dialog($args).

  o customDialogOptions (Optional)

    An object with custom jQuery UI Dialog options.


* There's also a close() method and a few others, but these are used internally.
  You don't need to care about them. Just open a modal dialog and everything
  should be fine as long as the server-side menu callback invokes the function
  modalframe_child_js() when generating pages for the modal dialog.


The package also contains an optional module that can be used as an example. It
is also recommended to check out the inline code documentation as it has a lot
of information on how it all works.
