The SexyBookmarks for Drupal module is a port of the WordPress plug-in by the
same name.

The sole aim was to stray away from the "in the box" thinking behind most social
bookmarking plugins, and add a little flair that would entice your readers,
rather than deterring them with microscopic icons that get lost in pages heavy
laden with content.

SexyBookmarks for Drupal was ported and is maintained by Stuart Clark
(deciphered).
- http://stuar.tc/lark

SexyBookmarks is owned and operated by Shareaholic.
- http://sexybookmarks.shareaholic.com/


Features
--------------------------------------------------------------------------------

* Support for more than 80 social bookmarking services.
* Multiple configuration profiles - Features exportable.
* Multiple display options:
  * Node item/Psuedo field for Nodes.
  * Display Suite/Node Displays field.
  * CCK/Views formatters:
    * Link module.
    * Node Reference module.
    * plus an example Custom Formatter for extended support.
  * Views module content field.
  * Blocks:
    * Current URL.
    * Site URL.
    * plus an example Boxes block for extended support.
  * Drupal API theme() function.
* Support for:
  * Formatter Settings module.
  * Page Title module.
  * Redirecting Click Bouncer module.
  * Shorten URLs module.


Required Modules
--------------------------------------------------------------------------------

* Chaos Tool suite - http://drupal.org/project/ctools
* jQuery UI - http://drupal.org/project/jquery_ui
* jQuery Update - http://drupal.org/project/jquery_update


Recommended Modules
--------------------------------------------------------------------------------

* Formatter Settings - http://drupal.org/project/formatter_settings
* Redirecting Click Bouncer - http://drupal.org/project/bouncer
* Shorten URLs - http://drupal.org/project/shorten


Configuration
--------------------------------------------------------------------------------

* Setting up profiles:

  Profiles can be managed at:
    Administer > Site configuration > SexyBookmarks
    http://[www.yoursite.com/path/to/drupal]/admin/settings/sexybookmarks


* Displaying via Nodes (pseudo field):

  The profile for each Content type can be set at:
    Administer > Content management > [content type] > Edit
    http://[www.yoursite.com/path/to/drupal]/admin/content/node-type/[content-type]

  The position of the SexyBookmarks widget can be set at:
    Administer > Content management > [content type] > Manage fields
    http://[www.yoursite.com/path/to/drupal]/admin/content/node-type/[content-type]/fields


Help
--------------------------------------------------------------------------------

More help can be found via the Help module:
  Administer > Help > sexybookmarks
  http://[www.yoursite.com/path/to/drupal]/admin/help/sexybookmarks
