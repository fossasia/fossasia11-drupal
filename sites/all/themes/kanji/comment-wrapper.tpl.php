<?php // $Id$   ?>
<div id="comments">
  <div class="comment-count">
    <?php if ($node->comment == 1 || $node->comment == 2): ?>
      <?php if ($node->comment_count == 0): ?>
        <h2><?php print t('No comments available.'); ?></h2>
      <?php elseif ($node->comment_count == 1): ?>
        <h2><?php print t('1 comment'); ?></h2>
      <?php else: ?>
        <h2><?php print $node->comment_count . ' ' . 'comments'; ?></h2>
      <?php endif; ?>
   <?php endif; ?>
	</div>
	<?php print $content; ?>
</div>