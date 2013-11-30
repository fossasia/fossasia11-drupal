Better formats is a module to add more flexibility to Drupal's core input format system.
Features

    * Set the default format per role.
    * Set the default format per content type.
    * Control allowed formats per content type.
    * Hide format tips.
    * Hide format selection, forcing the default to be used.
    * Expand the selection fieldset by default.
    * Disable the expand/collapse of the selection fieldset.
    * Set selection fieldset title.
    * Set default formats for nodes, comments, and blocks separately.
    * Works with CCK textareas.
    * Panels comment support.
    * I18n module support.
    * and more.

-------------------------------------------------------------------

Installation:

1. Copy the module folder to your server.
2. Enable the module via the modules page.

-------------------------------------------------------------------

Simple 4-step usage:

1. Go to user permissions (/admin/user/permissions) and set your permissions.
2. Navigate to Site Configuration > Input formats (/admin/settings/filters)
3. There you will find 2 tabs where you can change your settings.
    Defaults (/admin/settings/filters/defauts)
    Settings (/admin/settings/filters/settings)
4. If you enable the "Control formats per node type" option. Go to your content
   type admin page to set those settings (example /admin/content/node-type/page).
   The settings are under the Input format settings fieldset.

-------------------------------------------------------------------

Important:

When setting default formats ensure that you arranged the roles correctly
placing roles in their order of precedence. This is used to determine what
default a user will get when they have more than 1 role.

NOTE:
All logged in users are automatically assigned the authenticated user role
so this role must be below all other roles that you want to set a default for or
they will get the authenticated user role default instead.

Example:
Let's say you have the 2 roles that come with Drupal and have added an
'admin' role. You would most likely want to arrange the roles in this order:

  admin
  authenticated user
  anonymous user

-------------------------------------------------------------------

Extended usage and notes:

* The default format will only be set on NEW nodes and comments. The format
  selected when the form is submitted is used for future editing purposes.

* The module is designed to always fall back to default settings when needed.
  This means that when you enable the module before you change any settings,
  it will use your current Drupal settings. Also when you enable conrol per node
  type it will use your global settings until you save the content type with new
  settings.

* The permissions "collapse format fieldset by default" and
  "collapsible format selection" will only work if "Show format selection" is
  also given. This is because those 2 perms only have an effect when there is
  a format selection.

* The permission "collapse format fieldset by default" will only work if
  "collapsible format selection" is also given. This is because the
  fieldset can only be collapsed by default if it is collapsible.

* If you dis-allow a format that is already being used by content, the module
  will do its best to set the correct format. The precidence of the formats are:
  1. Existing format selected when last saved
  2. Content type default format
  3. Global default format
  4. First allowed format
  5. Drupal core site default format

* User 1 is treated the same as all other users when it comes to a default
  format. If user 1 has not been assigned any roles then it will be assigned
  the authenticated user role's default format. If you want user 1 to have the
  default of another role assign that role to user 1.

* Ensure you read the important notes in the previous section marked important.
  It explains how you must order your roles to effectively get your defaults.
