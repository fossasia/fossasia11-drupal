<?php
// $Id: block.tpl.php,v 1.1 2010/08/05 07:51:20 antsin Exp $

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

<div class="<?php print $classes; ?> <?php if (function_exists(block_class)) print block_class($block); ?>">
  <div class="block-inner clearfix">
    <?php if (($block->subject) && (($block->region !='header')&&($block->region !='left')&&($block->region !='footer_one')&&($block->region !='footer_two')&&($block->region !='footer_three')&&($block->region !='footer_four'))): ?>
      <?php $firstword = wordlimit($block->subject, 1, "");
      $block->subject = str_replace($firstword, "<span class=\"first-word\">" . $firstword . "</span>", $block->subject); ?>
      <h2 class="title"><?php print $block->subject; ?></h2>
	  <div class="makeup"></div>
    <?php elseif ($block->subject): ?>
      <h2 class="title"><?php print $block->subject; ?></h2>
    <?php endif;?>
    <div class="content">
      <?php print $block->content; ?>
    </div>
  </div>
</div> <!-- /block-inner, /block -->