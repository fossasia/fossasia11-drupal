<?php
// $Id: node-blog.tpl.php,v 1.1 2010/04/24 13:24:34 antsin Exp $

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

<div class="<?php print $classes; ?>">
  <div class="node-inner clear-block">
    <span class="date"><?php echo date("d", $created);?> <?php echo date("M", $created);?></span>
    <?php if (!$page): ?>
      <h1 class="title"><a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a></h1>
    <?php endif; ?>
    <?php if ($unpublished): ?>
      <div class="unpublished"><?php print t('Unpublished'); ?></div>
    <?php endif; ?>
    <?php if ($submitted): ?>
	  <span class="submitted"><?php echo t('Posted by ').$name; ?></span>
    <?php endif; ?>
    <?php if ($terms): ?>
      <span class="terms"><?php print t(' in ') . $node_terms; ?></span>
    <?php endif; ?>
    <div class="content clear-block">
      <?php print $content; ?>
    </div>
    <?php if ((!$page)||($links)): ?>
      <div class="extra-links">
        <?php print $links; ?>
      </div>
	<?php endif; ?>
  </div>
</div> <!-- /node-inner, /node -->