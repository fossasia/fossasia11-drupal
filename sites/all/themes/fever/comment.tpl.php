<?php
// $Id: comment.tpl.php,v 1.1 2010/08/05 07:51:20 antsin Exp $

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

<div class="<?php print $classes; ?>"><div class="comment-inner clear-block">
  <?php if ($title): ?>
    <h3 class="title"><!--<?php print $title; ?>--></h3> 
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?> 

  <div class="content">
    <div class="content-inner">    
	  <?php if ($links): ?>
        <div class="links"><?php print $links; ?></div>
      <?php endif; ?> 
      <div class="author"><?php print $author; ?></div>
	  <div class="date"><?php print $date; ?></div>
	  <?php print $picture;?>
      <?php print $content; ?>
	</div>
    <?php if ($signature): ?>
    <div class="user-signature clear-block">
      <?php print $signature; ?>
    </div>
    <?php endif; ?> 
  </div>
</div></div> <!-- /comment-inner, /comment -->
