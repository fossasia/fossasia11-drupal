
Views Slideshow: ddblock
------------------------
Note: This is the development version of the module. Please test and review the module and post
your issues in the issue queue at http://drupal.org/project/issues/views_slideshow_ddblock to
help getting a first release candidate of the module. Although we did test the module ourselves, 
use this release only to test the module. Don't use it in production environments.

SUMMARY
-------
The Views Slideshow Dynamic display block module enables you to present content in a 
dynamic way. For creating slideshow effects it uses the jQuery Cycle plug-in.

Several effects and other settings can be set in the configuration settings of the Views
Slideshow: ddblock module.

REQUIREMENTS
------------
* Views and Views_slideshow are required modules to run the views slideshow: ddblock module.
* CCK, Imagefield, ImageAPI, Filefield are required to run the example module. 
* Imagecache is recommended to use optimized images for the slideshow.

Optional:
* Emfield can be used to show videos in the views_slideshow_ddblock module.
* jQuery easing plug-in <http://plugins.jquery.com/files/jquery.easing.1.1.1.js>
  This jQuery plug-in is not included in the Drupal distribution because it is not licensed
  under GPL which is required by Drupal.

INSTALLATION
------------
1. Install required module first.
2. Extract the contents of the project into your modules directory, probably at
   /sites/all/modules/views_slideshow_ddblock.
3. Enable the Views slideshow: DDblock module.
4. Enable the Views slideshow: DDblock Examples module. 
  (For using the example module you need to install the vsd-upright10p-60p slideshow 
   themes, see below)

JQUERY EASING PLUG-IN INSTALLATION
----------------------------------
1. Download version 1.1.1 at http://plugins.jquery.com/project/Easing.
2. Copy jquery.easing.1.1.1.js to sites/all/modules/views_slideshow_ddblock/js.

STEPS TO CREATE A FIRST SLIDESHOW AFTER INSTALLATION OF THE MODULES
-------------------------------------------------------------------
When using the example module:  
   1. Install the vsd-upright-10 - 60 views_slideshow_ddblock themes (see below)
   2. Place one of the slideshow example blocks
      (ddblock_if_example_slideshows: uprightxx) in a region or in a node.

When using your own content type and view:
   1a. Install the vsd-upright-10 - 60 views_slideshow_ddblock themes (see below)
      And use one of the vsd-upright-xxp themes in step 6.
   or
   1b. When you don't install the vsd-upright-10 - 60 themes you will only be able to 
       select the default theme in step 6

   2. Create or use an existing block display for your view with an imagefield.
   3. Choose slideshow for the style of the block display.
   4. Change the setting for the slideshow style (click on the button after the style).
   5. Choose ddblock for the slideshow mode.
   6. Choose the slideshow theme.
   7. Create the mapping for theme fields.
   8. Leave all other default settings. (you can adjust them later, as needed)
   9. Save the view.
   10. Place the block in a region in the block configuration page.
   
INSTALL VIEWS SLIDESHOW DYNAMIC DISPLAY BLOCK THEMES
----------------------------------------------------
The views slideshow: ddblock module comes with several free themes, other commercial themes
will also be available in the future. (have a look at themes.myalbums.biz for examples of
commercial themes for the ddblock module which will also become available for
the views slideshow: ddblock module).

You can download the free themes from http://ddblock.myalbums.biz/download

* Download the theme package (vsd-upright10-60-V2-2.zip) from http://ddblock.myalbums.biz/download
  Make sure you use the theme package for the views slideshow: ddblock module (Version 2.x)

* Extract the zip file to a temporary directory

* Copy the custom directory with all subdirectories to the theme directory of the
  theme you use. (which is probably at sites/all/themes/[YOUR_THEME_NAME])


DOCUMENTATION
-------------
Documentation is available at http://ddblock.myalbums.biz 
Views slideshow: Dynamic display block tutorial (version 2.x)

SUPPORT
-------
Please post issues (bugs, support requests, feature request) in the issue queue 
(http://drupal.org/project/issues/views_slideshow_ddblock) of the module.

CONTACT
-------
* Philip Blaauw (ppblaauw) - http://drupal.org/user/155138

PAID SERVICES
-------------
We also offer paid services: installation, development, theming, customization.
You can contact us via the contact form on http://ddblock.myalbums.biz.
or via email to ppblaauw (at) gmail.com
