<?php
// $Id: page.tpl.php,v 1.3 2010/08/09 03:03:44 antsin Exp $

/*
+----------------------------------------------------------------+
|   Fever for Dupal 6.x - Version 1.0                            |
|   Copyright (C) 2010 Antsin.com All Rights Reserved.           |
|   @license - GNU GENERAL PUBLIC LICENSE                        |
|----------------------------------------------------------------|
|   Theme Name: Fever                                            |
|   Description: Fever by Antsin                                 |
|   Author: Antsin.com                                           |
|   Date: 5th August 2010                                        |
|   Website: http://www.antsin.com/                              |
|----------------------------------------------------------------+
*/ 
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <!--[if IE 7]>
  <link type="text/css" rel="stylesheet" href="<?php print base_path().path_to_theme(); ?>/css/ie.css" media="screen">
  <![endif]-->
  <?php print $scripts; ?>
</head>
<body class="<?php print $body_classes; ?>">
  <div id="page">
    <div id="header"><div id="header-inner" class="clearfix">
	  <?php if ($secondary_links): ?>
        <div id="secondary"><div id="secondary-inner-wrapper"><div id="secondary-inner">
          <?php print theme('links', $secondary_links); ?>
        </div></div></div> <!-- /#secondary -->
      <?php endif; ?>
      <?php if ($logo || $site_name || $site_slogan): ?>
        <div id="logo-title">
          <?php if ($logo): ?>
            <div id="logo"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" /></a></div>
          <?php endif; ?>
          <?php if ($site_name): ?>  
		    <div id="site-name"><h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a></h1></div>
		  <?php endif; ?>
          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div> <!-- /#logo-title -->
      <?php endif; ?>
      <?php if ($header): ?>
        <div id="header-blocks">
          <?php print $header; ?>
        </div> <!-- /#header-blocks -->
      <?php endif; ?>
    </div></div> <!-- /#header-inner, /#header -->

	<div id="navbar"><div id="navbar-inner" class="clearfix">
      <?php if ($search): ?>
        <div id="search">
          <?php print $search; ?>
        </div> <!-- /#search block -->
      <?php endif; ?>
	  <?php if ($primary_links): ?>	    
        <div id="primary-left" <?php if ($search): print 'class="withsearch"';
		  else: print 'class="withoutsearch"'; 
		  endif; ?>>
		  <div id="primary-right"><div id="primary">
          <?php print menu_tree($menu_name = 'primary-links'); ?>
        </div></div></div>
      <?php endif; ?> <!-- /#primary -->
	</div></div>

  	<div id="top-border"></div>
    <div id="main">
	  <div id="main-inner" class="clearfix">
        <?php if ($showcase): ?>
          <div id="showcase"><div id="showcase-inner">
            <?php print $showcase; ?>
		  </div></div>
        <?php endif; ?> <!-- /#showcase -->     
		<div>
		  <div id="content-top">
		    <?php if ($breadcrumb && $breadcrumb != '<div class="breadcrumb"></div>'): print '<div id="breadcrumb-wrapper"><div id="breadcrumb">You are here: '.$breadcrumb.'</div></div>';
		    else: print '<div id="breadcrumb-wrapper"><div id="breadcrumb">You are here: <a href="/">Home</a></div></div>'; 
		    endif; ?>
			<div id="content-top-inner">
			  <div id="content-wrapper" class="clearfix">
		        <div id="content">
  		          <?php if ($title || $tabs || $help || $messages): ?>
                    <div id="content-header">
                      <?php if (($title) && ($node->type!= 'blog')) : ?><h1 class="title"><?php print $title; ?></h1><?php endif; ?>
                      <?php print $messages; ?>
                      <?php if ($tabs): ?>
                        <div class="tabs"><?php print $tabs; ?></div>
                      <?php endif; ?>
                      <?php print $help; ?>
                    </div> <!-- /#content-header -->
                  <?php endif; ?> 
                  <?php print $content; ?>
		        </div>
		        <?php if ($left): ?>
                  <div id="sidebar-left">
                    <?php print $left; ?>
                  </div> <!-- /#sidebar-left -->
                <?php endif; ?>
		      </div>
			</div>
		  </div>
          <?php if ($right): ?>
            <div id="sidebar-right">
              <?php print $right; ?>
            </div> <!-- /#sidebar-right -->
          <?php endif; ?>
        </div>	  

	    <?php if ($content_bottom_one || $content_bottom_two || $content_bottom_three || $content_bottom_four): ?>
          <div id="content-bottom" ><div id="content-bottom-inner" class="<?php print $content_bottom; ?> clearfix">
            <?php if ($content_bottom_one): ?>
              <div id="content-bottom-one" class="column">
                <?php print $content_bottom_one; ?>
              </div><!-- /content-bottom-one -->
            <?php endif; ?>
            <?php if ($content_bottom_two): ?>
              <div id="content-bottom-two" class="column">
                <?php print $content_bottom_two; ?>
              </div><!-- /content-bottom-two -->
            <?php endif; ?>
	    	<?php if ($content_bottom_three): ?>
              <div id="content-bottom-three" class="column">
                <?php print $content_bottom_three; ?>
              </div><!-- /content-bottom-three -->
            <?php endif; ?>
		    <?php if ($content_bottom_four): ?>
              <div id="content-bottom-four" class="column">
                <?php print $content_bottom_four; ?>
              </div><!-- /content-bottom-four -->
            <?php endif; ?>
			<?php if ($content_bottom_five): ?>
              <div id="content-bottom-five">
                <?php print $content_bottom_five; ?>
              </div><!-- /content-bottom-five -->
            <?php endif; ?>
          </div></div> 
	    <?php endif; ?>
      </div>
    </div> <!-- /#main-inner, /#main -->
 	<div id="bottom-border"></div>    
  <div id="closure"><div id="closure-inner" class="clearfix"><div id="designed-by"><small>Site by <a href="http://www.mbm.vn" title="MBM International">MBM</a></small></div><?php print $closure_region; ?></div></div>
  </div> <?php print $closure; ?><!-- /#page -->
</body>
</html>
