<?php

/**
 * @file
 * data import for vsd_if_examples module
 */

/**
 * Create example node.
 *
 * @param $node_type
 *   node_type of the node to add
 * @param $files
 *   files to add to the node
 * @param $slide
 *   Array with slide fields: title, body, slide_text, pager_item_text
 *
 * @return
 *   $node object
 */
function _vsd_if_examples_import_create_node($node_type, $files, $slide) {
  
  global $user;
  // For node_object_prepare()
  module_load_include('inc', 'node', 'node.pages');
  
  $node = new stdClass();
  $node->type = $node_type;
  $node->name = $user->name;
  $node->title = $slide['node_title'];
  $node->body = $slide['node_body'];
  $node->format = 2; // full html
  
  // Prepares a node object for editing.
  // Fills in a few default values from the content type:
  //   $node->status  (0 - unpublished, 1 - published) from node_type - defaults to 0
  //   $node->promote (0 - don't promote to frontpage, 1 - promote to frontpage) from node_type - defaults to 0
  //   $node->sticky  (0 - no, 1 - yes) defaults to 0
  //   $node->uid     = $user-uid
  //   $node->created = time()
  //   $node->revision = defaults to 0 
  // and then invokes hook_prepare() on the node type module, 
  // and hook_node_prepare() on all modules.
  node_object_prepare($node);

  // override $node->status, set to published 
  $node->status = 1; // published

  // set slidetext and pager_item_text values
  $node->field_ddblock_if_slide_text[0] = array('value' => $slide['slide_text']);
  $node->field_ddblock_if_pager_item_text[0] = array('value' => $slide['pager_item_text']);

  // Get the filefield module to do the saving and marking the record as permanent.
  foreach ($files as $field_name => $file) {
    if (isset($node->$field_name)) {
      array_push($node->$field_name, $file);
    }
    else {
      $node->$field_name = array(0 => $file);
    }
  }

  // Prepare node for save and allow modules to make changes
  $node = node_submit($node);
  
  // Save node object into the database.
  node_save($node);

  return $node;
}

/**
 * Determine the widget's files directory
 *
 * @param $field CCK field
 * @return files directory path.
 */
function _vsd_if_examples_import_widget_files_directory($field) {
  $widget_file_path = $field['widget']['file_path'];
  if (module_exists('token')) {
    global $user;
    $widget_file_path = token_replace($widget_file_path, 'user', $user);
  }
  return file_directory_path() .'/'. $widget_file_path;
}

/**
 * @file
 * Slides content
 *
 * @return slides array.
 */
function _vsd_if_examples_get_slides() {
  
  $filedir = drupal_get_path('module', 'vsd_if_examples'); // directory with stored files

  // slides array
  $slides = array(
    0 => array(
    'image_path' => $filedir . '/images/slide1.png',
    'node_title' => t('Showcase featured content'),
    'node_body' => t('The Views Slideshow: Dynamic Display Block module is a views_slideshow plugin mainly used to showcase featured content at a prominent place on the frontpage of your Drupal site in an attractive way. A featured content slideshow can contain e.g.: New articles, Most popular content, New products, Portfolios, Videos, etc.

<table>
<tr>
<td valign="top">
<h3>Other usages</h3>
<ul>
<li>Ubercart Products</li>
<li>Image Galleries</li>
<li>Video Galleries</li>
<li>Header Slideshows</li>
<li>Text sliders</li>
</ul>
</td>
<td valign="top">
<h3>Features</h3>
<ul>
<li><a href="#any_content">Any content</a></li>
<li><a href="#content_options">Multiple content delivery options</a></li>
<li><a href="#multiple_slideshows">Multiple slideshows</a></li>
<li><a href="#slideshow_config">Configuration settings per slideshow</a></li>
<li><a href="#pagers">Number-, prev/next-, text-, image, scrollable-pagers</a></li>
<li><a href="#themes">Free and commercial layout themes</a></li>
</ul>
</td>
</table>
<h2>Features</h2>
<a name="any_content"></a>
<h3>Any content</h3>
With the views slideshow dynamic display block module you can show any content in a slideshow.
<table>
<tr>
<td with="50%" valign="top">
<ul>
<li>Images<li>
<li>Video</li>
<li>Text</li>
</ul>
</td>
<td with="50%" valign="top">
<ul>
<li>Buttons (read more, play, view etc.)</li>
<li>Links (links to pages or other sites)</li>
<li>Combination of the above</li>
</ul>
</td>
</tr>
</table>
<a name="content_options"></a>
<h3>Multiple content delivery options</h3>
The views slideshow dynamic display block can get content from:
<ul>
<li>CCK\Views for filtered content</li>
<li>Nodequeue for content in time and custom ordered content</li>
</ul> 
<a name="multiple_slideshows"></a>
<h3>Multiple slideshows</h3>
You can make an unlimited amount of slideshows. The slideshows can also be placed on the same page. Javascript Slideshows are by definition CPU intensive. It is advisable to make the transitions as short as possible and the timeout between the slides as long as possible.
<a name="slideshow_config"></a>
<h3>Configuration per slideshow</h3>
All the settings of each slideshow can be configured separately. Settings are  e.g.:
<table>
<tr>
<td with="50%" valign="top">
<ul>
<li>Content selection</li>
<li>Theme layout selection</li>
<li>Transition effect selection (25 different effects)</li>
<li>Option to add custom transitions</li>
<li>Multiple transitions in one slideshow</li>
<li>Speed: time of the transition</li>
<li>Timeout: time between the transitions</li>
<li>Order of slides</li>
</ul>
</td>
<td with="50%" valign="top">
<ul>
<li>Pause: stop slideshow when hovered</li>
<li>Next: go to next slide clicking slide</li>
<li>Pager selection</li>
<li>Pager position</li>
<li>Pager event: click or mouseover</li>
<li>Custom Cycle plugin settings, for advanced usages</li>
<li>Slidetext Effects</li>
<li>Slidetext Position</li>
<li>Slidetext speed and timeout</li>
</ul>
</td>
</tr>
</table>
<a name="pagers"></a>
<h3>Number-, prev/next-, text-, image, scrollable-pagers</h3>
For selecting the different slides the views slideshow dynamic display block module provides a number pager, a previous/next pager, pagers with text, images and a combination to select the different slides. A scrollable pager can be used when you want to show more slides in a slideshow while limiting the space for the pager items.
<a name="themes"></a>
<h3>Free and commercial layout themes</h3>
With the views slideshow dynamic display block module you get several free theme layouts which are licensed under GPL. Other new themes are developed and you can purchase them at <a href="http://themes.myalbums.biz">Themes.myalbums.biz</a>. If you want to request a theme layout you can contact us via the <a href="http://themes.myalbums.biz/contact">contact</a> form. 
</ul>'),                    
          'slide_text' => t('Showcase featured content slideshows at a prominent place on the frontpage of your Drupal site in an attractive way.'),
          'pager_item_text' => t('Showcase'),
        ),
        1 => array(
          'image_path' => $filedir . '/images/slide1.png',
          'node_title' => t('Theme layouts'),
          'node_body' => t('The views slideshow ddblock module is supplemented with 6 free example themes at the moment. 

The 6 free themes give examples of the possible pagers. 

<h2>Example themes</h2>
<ul>
<li>upright10 - with number pager</li>
<li>upright20 - with prev/next pager</li>
<li>upright30 - with text pager</li>
<li>upright40 - with image pager</li>
<li>upright50 - with text/image pager</li>
<li>upright60 - with scrollable pager</li>
</ul>

<h2>Theme extensions</h2>
When themes have an extension, this means:
<ul>
<li>p - the theme has a portrait layout</li>
<li>l - the theme has a landscape layout</li>
<li>B - the theme is a basic theme</li>
<li>S - the theme has source image and stylesheet width for 460, 700 and 940 slideshow width</li>
</ul>    

<h2>Settings for themes</h2>
<ul>
<li>Vertical or horizontal layout</li>
<li>Configurable slide and slidetext effects</li>
<li>Slide text configurable at top, left, bottom or right side depending on layout</li>
<li>Pager configurable on top, bottom, left and right depending on layout</li>
<li>Number pager, prev/next pager, text pager, image pager or image/text pager, scrollable pager to select slides</li>
<li>Extra prev/next pager in a seperate pager and/or on the slides</li>
</ul> 

<h2>Create custom themes</h2>
You can develop your own lay-outs for the views slideshow ddblock module. Same as with the ddblock module we will create a tutorial page what steps to follow to create your own custom theme.

<h2>Commercial themes</h2> 
With the commercial themes you have a wide range of layouts to choose from for your slideshow. These theme lay-outs are flexible and thoroughly tested in IE 7, Firefox 2, Firefox 3, Safari 3.1, Opera 9.5 and Chrome. Also IE6 is supported (although there are some extra steps which have to be done and there are workarounds for translucency). Commercial themes can be found at: http://themes.myalbums.biz

<h2>Theme requests</h2>
We welcome requests for other theme layouts.
You can make a request by sending us your requirements on the <a href="http://themes.myalbums.biz/contact">Contact form</a> at http://themes.myalbums.biz Please be as detailed as possible. 

We will periodically release new commercial themes.
'),                    
          'slide_text' => t('The slideshow module is supplemented with free and commercial theme layouts.
(http://themes.myalbums.biz)'),
          'pager_item_text' => t('Theme layouts'),
        ),
        2 => array(
          'image_path' => $filedir . '/images/slide1.png',
          'node_title' => t('Support'),
          'node_body' => t('Support for the views slideshow dynamic display block module is given on a daily basis. The <a href="http://drupal.org/project/issues/views_slideshow_ddblock">issue queue</a> of the module is the preferred place to post: support requests, feature request and bugs. Please give detailed descriptions of your issues, so we can help you better. Searching the <a href="http://drupal.org/project/issues/views_slideshow_ddblock">issue queue</a> (search on all issue) and the <a href="http://ddblock.myalbums.biz/faq">FAQ</a> can give you direct answers. 

<h2>Issue queue Tips</h2>

<ul>
<li>Search first (on all existing issues) before posting new issues.</li>
<li>Post one issue per post, so they are useful for other users.</li>
<li>Leave the assigned select box to: Unassigned, if you don\'t solve the issue yourself.</li>
<li>Post detailed information how to reproduce the issue, what behavior is expected and what happened instead.</li>
<li>There is no such thing as a bug report, support request, feature request that is too detailed.</li>
</ul>
For more info, see also the <a href="http://drupal.org/node/317">issue queue handbook</a>

<h2>Documentation</h2>
The views slideshow dynamic display block module will be supported by a complete set of documentation. The installation and configuration tutorial can be found at: <a href="http://ddblock.myalbums.biz/">Views slideshow: Dynamic display block tutorial (version 2.x)</a>

Like the ddblock module this module will have a faq, howto\'s, tips, etc.
'),
          'slide_text' => t('Support is given on a daily basis. In the issue queue at drupal.org you can find and post: support requests, feature request and bugs.'),
          'pager_item_text' => t('Support'),
        ),
        3 => array(
          'image_path' => $filedir . '/images/slide1.png',
          'node_title' => t('Paid services'),
          'node_body' => t('We provide paid services for assistance, consultancy, customizations and other Drupal related services.
For dynamic display block modules related projects please fill out the <a href="http://themes.myalbums.biz/content/design-requirements">design requirements questionnaire</a> at http://themes.myalbums.biz to create an efficient work-flow.

If your project is not dynamic display block modules related, please contact us on the <a href="http://ddblock.myalbums.biz/contact">contact</a> form on the http://ddblock.myalbums.biz site.'),            
          'slide_text' => t('We provide paid services for assistance, consultancy, customizations and other drupal related services.'),
          'pager_item_text' => t('Paid services'),
        ),
        4 => array(
          'image_path' => $filedir . '/images/slide1.png',
          'node_title' => t('How you can help?'),
          'node_body' => t('Below are some examples how you can support the development of the slideshow module:
<ul>
<li>Write a review on <a href="http://drupalmodules.com/module/dynamic-display-block">drupalmodules.com</a>.</li>
<li>Post bugs, feature requests, patches in the <a href="http://drupal.org/project/issues/ddblock">issue queue</a> on the http://drupal.org site.</li>
<li>Review documentation and post issues in the <a href="http://drupal.org/project/issues/ddblock">issue queue</a> on the http://drupal.org site.</li>
<li>Review module code and post snippets of code in the <a href="http://ddblock.myalbums.biz/forum/11">forum</a> to improve the module.</li>
<li>Post your site as an <a href="http://themes.myalbums.biz/sites_using_ddblock">Internet site using the dynamic display block modules</a> on the http://themes.myalbums.biz site.</li>
<li>Any other way you think could be helpful.</li>
</ul>

With your help, we can more effectively develop a useful views slideshow ddblock module for your Drupal site.
'),                    
          'slide_text' => t('<ul>
<li>Post bugs, feature requests, patches in the issue queue</li>
<li>Review module code and documentation</li>
<li>Showcase your site</li>
</ul>'),
          'pager_item_text' => t('How you can help?'),
        ),
        5 => array(
          'image_path' => $filedir . '/images/slide1.png',
          'node_title' => t('Thanks'),
          'node_body' => t('Thank you for your interest in the Views slideshow: Dynamic display block module.<br />Hope the slideshows serve you well on your Drupal site!!!<br /><br />Philip Blaauw<br />Phelsa Information Technology'),                    
          'slide_text' => t('Thank you for your interest in this Drupal slideshow module.<br />Hope it serves you well!!!'),
          'pager_item_text' => t('Thanks'),
        ),
      );  
  return $slides;
}  



