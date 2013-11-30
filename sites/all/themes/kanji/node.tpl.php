<?php // $Id$   ?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clear-block<?php print ' ' . $node->type; ?>">
  <?php if($submitted && !$page): ?>
    <div class="date">
      <div class="textdate">
        <div class="day"><?php print format_date($created, 'custom', 'j'); ?></div>
        <div class="month"><?php print format_date($created, 'custom', 'M'); ?></div>
      </div>
    </div>
  <?php endif; ?>
  <?php if (!$page): ?>
    <h2 class="title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php endif; ?>
  
  <?php print $picture ?>

  <div class="meta">
    <?php if ($submitted): ?>
      <span class="submitted"><?php print $submitted ?></span>
    <?php endif; ?>
  </div>

  <div class="content">
    <?php print $content ?>
  </div>
  <?php if ($terms): ?>
    <div class="terms"><?php print '<strong>' . t('Tags') . ":</strong> " . $terms ?></div>
  <?php endif;?>
  <div class="node-links"><?php print $links; ?></div>
</div>