<?php // $Id$   ?>
<div id="block-<?php print $block->module.'-'.$block->delta; ?>" class="clear-block block block-<?php print $block->module . ' ' . $zebra; ?>">
  <?php if (!empty($block->subject)): ?>
    <h2><?php print $block->subject; ?><?php if (user_access('administer blocks')) :?><?php print '<span class="edit">' . l('Edit', 'admin/build/block/configure/'.$block->module.'/'.$block->delta, array('query' => drupal_get_destination())) . '</span>'; ?><?php endif; ?></h2>
  <?php endif;?>
  <div class="content<?php if($block->subject): print ' with-subject'; endif; ?>">
    <?php print $block->content; ?>
  </div>
</div>


