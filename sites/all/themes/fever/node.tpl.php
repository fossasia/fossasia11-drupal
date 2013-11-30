<?php
// $Id: node.tpl.php,v 1.1 2010/08/05 07:51:20 antsin Exp $

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

<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"><div class="node-inner clear-block">
  <?php if (!$page): ?>
    <h2 class="title">
      <a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>
  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <div class="meta">
    <?php if ($submitted): ?>
	  <span class="submitted"><?php echo t('Posted on ').date("d. M, Y", $created).t(' by ').$name; ?></span>
    <?php endif; ?>
	<?php if ($terms): ?>		
	  <span class="terms"><?php print ' Tag: ' . $node_terms; ?></span>
    <?php endif; ?>
	<?php if ($page): ?><?php print $links; ?><?php endif; ?>
  </div>

  <div class="content">
    <?php print $content; ?>
  </div>

</div></div> <!-- /node-inner, /node -->