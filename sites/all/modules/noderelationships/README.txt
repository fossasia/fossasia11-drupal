;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Node Relationships
;; $Id: README.txt,v 1.1.2.12 2010/05/14 04:23:29 markuspetrux Exp $
;;
;; Original author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CONTENTS OF THIS FILE
=====================
- OVERVIEW
- REQUIREMENTS
- INSTALLATION
- DESCRIPTION
- DEVELOPERS
- CREDITS


OVERVIEW
========

The Node Relationships module provides methods to complete two way relationships
between content types enhancing the features of node reference fields.

It provides the following features:

- Enhancements for node reference fields: "Search and reference view" (single
  and multiple selection based on dynamically configured views), "Create and
  reference" and "Translate and reference" (when the Drupal core translation
  module is enabled, with additional support for Internationalization module).
  These features use the Modal Frame API to provide popup dialogs from buttons
  attached to node reference fields configured to use the autocomplete widget.

- Automatic back references using dynamically configured views that are able
  to extract the relations from existing database information. This views can
  be rendered using a myriad of methods in the node view.

- The Node Relationships module provides default views for each feature that
  can be modified and/or cloned (recommended) should you need to add more
  fields, filters or change any other option to suit your needs.

- Basic entity relations diagram (ERD) that can be used to view the relations
  of each individual content type. The provided diagram can be used to walk the
  relationships of all types in the system.


REQUIREMENTS
============

- CCK and Node Reference.
  http://drupal.org/project/cck

- Views.
  http://drupal.org/project/views

- Modal Frame API.
  http://drupal.org/project/modalframe

- jQuery UI module (with jQuery UI library 1.7.x).
  http://drupal.org/project/jquery_ui


INSTALLATION
============

- Be sure to install all dependent modules.

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Go to Administer -> Site building -> Modules to install module.

- Review the settings of your nodereference fields to make sure related
  content types are explicitly specified in the option "Content types that
  can be referenced". Note that this is optional when you are using a view
  for your nodereference fields, but this information is required by the
  node relationships module.

- Now you can start browsing the "Relationships" tab available in the
  administration section of all content types, next to the "Manage fields"
  and "Display fields" tabs provided by CCK.


DESCRIPTION
===========

The Node Relationships module provides methods to complete two way relationships
between content types enhancing the features of node reference fields.

It provides the following features:

- Node reference extras:

  This module provides several enhancements for node reference fields configured
  to use the autocomplete widget:

  - Search and reference view
  - Create and reference
  - Translate and reference
  - View referenced node on new window

  When these options are enabled, a new button for each one will be rendered
  in the node edit form, next to the corresponding autocomplete widget of the
  node reference field. These buttons will open a popup dialog where each
  feature is available.

  For nodereference fields defined with multiple values, a new button will be
  available next to the "Add more items" button that can be used to search and
  reference several nodes at a time.

  The "Search and reference" feature uses a view that is dynamically configured
  so that a single view can be reused by several node reference fields. A
  default view is provided with basic fields and pages for table or grid styles.
  You can modify and/or clone (recommended) this default view to add more field,
  filters, etc. A filter per node type is dynamically created to match the
  "Content types that can be referenced" option in the global settings section
  of the node reference field. Note that you should configure this option
  explicitly even if you use a view for the node reference field.

  The "Translate and reference" feature is available when the Drupal core
  translation module is enabled, and additional support is provided when
  the Internationalization module is enabled. When the "Translate and reference"
  option is enabled for a particular node reference field, references that
  already have translations will be automatically assigned to the values of
  node reference fields when a node translation is started. For those that do
  not have translations, a message will be displayed on the create translation
  form to warn the user a translation is missing and an option to translate and
  reference will be provided.

- Automatic back references:

  This module provides a method to display back reference views on referred
  nodes. Back reference definitions are taken from the "Content types that can
  be referenced" option in the global settings section of node reference fields.
  Note that you should configure this option explicitly even if you use a view
  for the node reference field.

  These back references do not need additional data stored in the database.
  Instead, views with the proper relationships are used to join the node
  reference field data with the corresponding nodes in the database. These
  views can then be used from the referred content types to provide a list of
  referrer nodes. The Node Relationships module provides a default view that
  you can modify and/or clone (recommended) should you need to add more fields,
  filters or change any other option to suit your needs.

  Each back reference can be displayed using one of several methods to render
  the corresponding view in the referred node itself (Field, Page and Tab).

  - Field: When a back reference is configured as a "Field", the Node
    Relationships module creates a CCK field automatically that provides the
    view output, and you can drag'n'drop this view to any position from the
    "Manage fields" panel of the content type. Note that no input widget is
    provided for this kind of fields, only the view output is provided.

    Available field formatters:
    - Back references view: renders the customized view as usual.
    - Back references count: displays the count of back references.

  - Page: This method provides a fieldset where all back reference fields
    assigned to this region will be rendered. This fieldset can be dragged
    to any position in the node using the "Manage fields" panel of the
    content type.

  - Tab: This method provides an alternative to the "Page" region, but the
    back references are displayed on a new tab "Relationships" added to the
    nodes where this option is enabled.

- Entity relations diagram:

  This is a basic diagram of the relations of current content type with others
  (referred from and refers to). It can be used to walk the relationships of
  all types in the system. This diagram is available per content type from
  Administer -> Content management -> Content types -> [type] -> Relationships.


DEVELOPERS
==========

Aside from the options that can be configured for node reference fields,
back references, views, etc. The Node Relationships module exposes a series
of hooks that provide enhanced methods of programmatic configuration of the
views and output generated.

@todo: Document the hooks provided by the Node Relationships module.


CREDITS
=======

- This module has been sponsored by Gamefilia:

  http://www.gamefilia.com

- Original versions of the icons can be found free from here:

  http://www.famfamfam.com/
