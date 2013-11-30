/* $Id: README.txt,v 1.4 2009/11/02 14:13:10 smk Exp $ */

-- SUMMARY --

Drupal's path module matches only full URLs when creating SEO-friendly aliases.
This module extends that behavior by also matching known sub-paths and
replacing them with their respective alias. For example, if node/1 is aliased
to content/title, this module rewrites the link to the node edit page
node/1/edit to use the aliased URL content/title/edit instead. In combination
with the Pathauto module it is possible to get rid of all remaining exposed
internal URLs.

Designed with performance in mind makes this module even suitable for larger
sites (but read FAQ below first).

For a full description of the module, visit the project page:
  http://drupal.org/project/subpath_alias
To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/subpath_alias


-- DEPENDENCIES --

* Path (Drupal core)

* URL Alter - http://drupal.org/project/url_alter

* Optional, but highly recommended:
  Pathauto - http://drupal.org/project/pathauto


-- INSTALLATION --

* NOTE: This module relies on the URL Alter module to perform URL rewrites.
  If you had set up a custom_url_rewrite function in your settings.php and
  NOT used URL Alter before, you HAVE TO modify your code after installation.
  Please consult the URL Alter documentation for more information.

* IMPORTANT: Make sure to download the correct version of URL Alter:
  Sub-path URL Aliases 6.x-1.1 requires URL Alter 6.x-1.2 or later,
  the older 6.x-1.0 release only works with URL Alter 6.x-1.1 and below.

* Install as usual, see http://drupal.org/node/70151 for further information.

* There is no further configuration required.  You might want to try the
  following alias (Site building >> URL aliases >> Add alias):

    Existing system path:  node/add
    Path alias:            create

  and visit http://www.example.com/create afterwards.


-- FAQ --

Q: Why are sub-paths replaced only at the beginning of URLs?
   Couldn't this be enhanced to match sub-paths at arbitrary positions within
   the URL?

A: No, because anchoring at the beginning is the only way to properly make use
   of database indexes.  Everything else will hurt the performance too much.

Q: So this means that this implementation is fast enough for my high-traffic
   site?

A: First, be aware that this module adds a layer of complexity on top of the
   existing URL alias generation.  Having said that, I was trying hard to be as
   resource friendly as possible; the number of additional database lookups
   will increase by about 1/3 compared to path.module alone.

   However, these numbers will likely decrease when #106559 [1] has been
   backported to D6.

   [1] http://drupal.org/node/106559


-- CREDITS --

Author:
Stefan M. Kudwien (smk-ka) - http://drupal.org/user/48898

This project has been sponsored by UNLEASHED MIND.
Specialized in consulting and planning of Drupal powered sites, we offer
installation, development, theming, customization, and hosting to get you
started. Visit http://www.unleashedmind.com for more information.
