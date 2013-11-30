// $Id: README.txt,v 1.5 2009/12/01 22:55:43 xuriz Exp $

MENU BREADCRUMBS
================

Introduction
------------
By default, Drupal 6 will use the Navigation menu for the breadcrumb. This module allows you to use the menu the current page belongs to for the breadcrumb.

As an added bonus, it also allows you to append the page title to the breadcrumb (either as a clickable url or not) and hide the breadcrumb if it only contains the link to the front page.

Installation
------------
1. Copy the menu_breadcrumb folder to your sites/all/modules directory.
2. At Administer -> Site building -> Modules (admin/build/modules) enable the module.
3. Configure the module settings at Administer -> Site configuration -> Menu breadcrumb (admin/settings/menu_breadcrumb).

Upgrading
---------
Replace the older menu_breadcrumb folder with the newer version, and then run update.php in case there are any database updates to apply.

Features
--------
- Allows you to use the menu the node belongs to for the breadcrumb on node pages.
- Append the page title to the breadcrumb.
- Optionally have the appended page title be an URL.

Issues / Feature requests
-------------------------
If you find a bug, or have a feature request, please go to :

http://drupal.org/project/issues/menu_breadcrumb

Contact
-------
This module is being maintained by me, Geoffrey de Vlugt (gdevlugt). 

If you need to contact me, use the contact form at :

http://drupal.org/user/167273/contact 
