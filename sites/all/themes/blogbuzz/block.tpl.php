<?php
// $Id: block.tpl.php,v 1.2 2010/04/24 13:19:56 antsin Exp $

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

<div class="block block-<?php print $block->module ?> <?php if (function_exists(block_class)) print block_class($block); ?>">
  <div class="block-inner clear-block">
    <?php if ($block->subject): ?>
      <div class="title"><h2><?php print $block->subject; ?></h2></div>
    <?php endif; ?>
	<div class="content-wrapper"><div class="content">
      <?php print $block->content; ?>
    </div></div>
  </div>
</div> <!-- /block-inner, /block -->