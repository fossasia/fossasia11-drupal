<?php
// $Id: comment.tpl.php,v 1.2 2010/04/24 13:19:56 antsin Exp $

/*
+----------------------------------------------------------------+
|   BlogBuzz for Dupal 6.x - Version 2.0                         |
|   Copyright (C) 2010 Antsin.com All Rights Reserved.           |
|   @license - GNU GENERAL PUBLIC LICENSE                        |
|----------------------------------------------------------------|
|   Theme Name: BlogBuzz                                         |
|   Description: BlogBuzz by Antsin                              |
|   Author: Antsin.com                                           |
|   Website: http://www.antsin.com/                              |
|----------------------------------------------------------------+
*/  
?>

<div class="<?php print $classes; ?>"><div class="comment-inner clear-block">
  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>  
  <?php print $picture;?>
  <div class="submitted"><span class="author"><?php print $author; ?></span><span><?php print $date; ?></span></div>
  <div class="content">
    <?php if ($title): ?>
      <h3 class="title"><?php print $title; ?></h3> 
    <?php endif; ?>
    <img src="<?php global $base_url; print $base_url .'/' . $directory; ?>/images/comment_arrow.png" class="comment_arrow" />
    <?php print $content; ?>
    <?php if ($signature): ?>
      <div class="user-signature clear-block"><?php print $signature; ?></div>
    <?php endif; ?> 
  </div>
  <?php if ($links): ?>
    <div class="links"><?php print $links; ?></div>
  <?php endif; ?>
</div></div> <!-- /comment-inner, /comment -->
