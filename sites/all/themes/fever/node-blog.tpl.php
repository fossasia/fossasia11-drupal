<?php
// $Id: node-blog.tpl.php,v 1.1 2010/08/05 07:51:20 antsin Exp $

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

<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>">
  <div class="node-inner clear-block">
    <?php if ($teaser): ?>
	  <div class="blog-image">
	    <?php preg_match("/<img[^>]+\>/i", $content, $matches);
	      print $matches[0]; ?>
	  </div>
	  <div class="blog-entry">
	    <div class="blog-date">
          <div class="month"><?php echo date("M", $created);?></div>
  	      <div class="date"><?php echo date("d", $created);?></div>
		</div>
	    <h2 class="title"><a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a></h2>	
	    <?php if ($unpublished): ?>
          <div class="unpublished"><?php print t('Unpublished'); ?></div>
        <?php endif; ?>
        <div class="meta">
	      <?php if ($submitted): ?>
	        <span class="submitted"><?php echo t('Written by ').$name; ?></span>
          <?php endif; ?>	  
	      <?php if ($terms): ?>		
	        <span class="terms"><?php print ' Tag: ' . $node_terms; ?></span>
          <?php endif; ?>
        </div>	 
        <div class="content">
          <?php print strip_tags($content, '<p><a></div>');?>
        </div>
	    <?php if ($links): ?><?php print $links; ?><?php endif; ?>
	  </div>
    <?php endif; ?>

    <?php if ($page): ?>
	  <div class="blog-date">
        <div class="month"><?php echo date("M", $created);?></div>
  	    <div class="date"><?php echo date("d", $created);?></div>
      </div>
      <h2 class="title"><?php print $title; ?></h2>
      <?php if ($unpublished): ?>
        <div class="unpublished"><?php print t('Unpublished'); ?></div>
      <?php endif; ?>
      <div class="meta">
	    <?php if ($submitted): ?>
	      <span class="submitted"><?php echo t('Written by ').$name; ?></span>
        <?php endif; ?>	  
	    <?php if ($terms): ?>		
	      <span class="terms"><?php print ' Tag: ' . $node_terms; ?></span>
        <?php endif; ?>
  	    <?php if ($links): ?><?php print $links; ?><?php endif; ?>
      </div>	 
      <div class="content">
        <?php print $content; ?>
      </div>
    <?php endif; ?>
  </div>
</div> <!-- /node-inner, /node -->
