<?php
// $Id: block.tpl.php 7156 2010-04-24 16:48:35Z chris $
?>

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?> <?php print $block_zebra; ?> <?php print $position; ?> <?php print $skinr; ?>">
  <div class="inner">
    <div class="corner-top"><div class="corner-top-right corner"></div><div class="corner-top-left corner"></div></div>
    <?php if (isset($edit_links)): ?>
    <?php print $edit_links; ?>
    <?php endif; ?>
  	<div class="inner-wrapper">
      <div class="inner-inner">
        <?php if ($block->subject): ?>
        <div class="block-icon pngfix"></div>
        <h2 class="title block-title"><?php print $block->subject ?></h2>
        <?php endif;?>
        <div class="content clearfix">
          <?php print $block->content ?>
        </div>
      </div><!-- /inner-inner -->
	  </div><!-- /inner-wrapper -->
    <div class="corner-bottom"><div class="corner-bottom-right corner"></div><div class="corner-bottom-left corner"></div></div>
  </div><!-- /inner -->
</div><!-- /block -->