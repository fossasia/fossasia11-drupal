<?php // $Id$   ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if IE]>
    <link type="text/css" rel="stylesheet" media="all" href="<?php print base_path() . path_to_theme(); ?>/ie_styles.css" />
  <![endif]-->
</head>
<body class="<?php print $body_node_classes; ?>">
  
  <div id="header">
    <?php if($header_top): ?>
      <div id="headerTop" class="blockregion">
        <?php print $header_top; ?>
      </div>
    <?php endif; ?>
    
    <div id="headerWrapper">
      <?php if (!empty($secondary_links)): ?>
        <div id="topMenu">
          <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')); ?>
        </div>
      <?php endif; ?>
        
      <?php if($search_box): ?>
        <div id="searchBox"><?php print $search_box; ?></div>
      <?php endif; ?>
      
      <div id="siteName">
        <?php if ($logo): print '<a href="' . $front_page . '" title="' . t('Home') . '"> <img src="' . check_url($logo) . '" alt="' . $site_title . '" id="logo" /></a>'; endif; ?>
        <div id="siteInfo">
          <?php if (!empty($site_name)): ?>
            <h1 id="siteTitle">
              <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
            </h1>
          <?php endif; ?>
          
          <?php if (!empty($site_slogan)): ?>
            <div id="siteSlogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div><!-- /siteInfo -->
      </div> <!-- /siteName-->
        
      <?php if($header): ?>
        <div id="header-region" class="blockregion">
          <?php print $header; ?>
        </div>
      <?php endif; ?>
        
    </div><!-- /headerWrapper -->
  </div><!-- /header -->

  <div id="container">
    <div id="inner">  
      <div id="contentWrapper">
        <?php if (!empty($primary_links)): ?>
          <div id="menuLeft"></div>
          <div id="primaryMenu">
            <?php print $primary_links_tree; ?>
          </div>
          <div id="menuRight"></div>
        <?php endif; ?> 
        
        <?php if($preface_top): ?>
          <div id="preface_top" class="blockregion">
            <?php print $preface_top; ?>
          </div>
        <?php endif; ?>
        
        <?php if($sidebar_first): ?>
          <div id="sidebar_first" class="sidebar">
            <?php print $sidebar_first; ?>
          </div>
        <?php endif; ?>
       
        <div id="center">
          <?php if (!empty($breadcrumb)): ?>
            <div id="breadcrumb">
              <?php print $breadcrumb; ?>
            </div>
          <?php endif; ?>
        
          <?php if (!empty($mission)): ?>
            <div id="mission">
              <?php print $mission; ?>
            </div>
          <?php endif; ?>
        
          <?php if($content_top): ?>
            <div id="content_top" class="blockregion">
              <?php print $content_top; ?>
            </div>
          <?php endif; ?>
        
          <div id="content">        
            <?php if (!empty($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
            <?php if (!empty($messages)): print $messages; endif; ?>
            <?php if (!empty($help)): print $help; endif; ?>
            <?php if (!empty($title)): ?><h2 class="title" id="page-title"><?php print $title; ?></h2><?php endif; ?>
            <?php print $content; ?>
            <?php print $feed_icons; ?>
          </div>
        
          <?php if($content_bottom): ?>
            <div id="content_bottom" class="blockregion">
              <?php print $content_bottom; ?>
            </div>
          <?php endif; ?>   
        </div><!-- /center --> 
    
        <?php if($sidebar_last): ?>
          <div id="sidebar_last" class="sidebar">
            <?php print $sidebar_last; ?>
          </div>
        <?php endif; ?>
      
        <?php if($postscript_bottom): ?>
          <div id="postscript_bottom" class="blockregion">
            <?php print $postscript_bottom; ?>
          </div>
        <?php endif; ?> 
      </div><!-- /contentWrapper -->
      
    </div><!-- /Inner -->
    
  </div><!-- /container -->
  
  <div id="footer">
    <div class="footer-text">Theme designed by <a href="http://www.carettedonny.be" title="Donny Carette">Donny Carette</a>
      <?php if($footer_message): ?>
        | <?php print $footer_message; ?>
      <?php endif; ?>
    </div>
                    
    <?php if($footer): ?>
      <div id="footer_bottom" class="blockregion">
        <?php print $footer; ?>
      </div>
    <?php endif; ?> 
  </div><!-- /footer -->
  
  <?php print $closure; ?>
</body>
</html>