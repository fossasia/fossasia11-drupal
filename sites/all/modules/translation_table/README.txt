$Id: README.txt,v 1.2.2.2 2010/10/20 12:19:27 pasqualle Exp $


UI for quick translation of taxonomies and menus. The i18n module allows for translation of taxonomy terms and menu items, but the process is tedious - you need to look up strings you want to translate, there is no overview of what's already translated etc.

This module presents your taxonomy terms or menu items in a table, and each language has a corresponding column.

Requires the i18n module.

Can handle translation of:

* menu item titles
* vocabulary names and taxonomy terms
* CCK field names and descriptions
* content type names and descriptions
* quicktab titles

INSTALLATION:

Just enable the module. You'll find the interface at: 

/admin/build/translate/table

Be sure the corresponding i18n modules are enabled too (taxonomy translation, CCK translation etc.)
