<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">
    
    <?php print $picture ?>
    
    <?php if ((arg(0)=="taxonomy" && arg(1)=="term") || $page==0): ?>
    <h2 class="page-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    <?php endif; ?>
    
    <?php if (!((arg(0)=="taxonomy" && arg(1)=="term") || $page==0)): ?>
    <h2 class="page-title"><?php print $title?></h2>
    <?php endif; ?>
    
    <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; //print format_date($node->created, 'custom', "d.m.Y"); ?></span>
    <?php endif; ?>
        
    <div class="content">
    <?php print $content ?>
    </div>
    
    <div class="clear-block clear" style="clear:both;">
    <div class="meta">
    <?php if ($taxonomy): ?>
    <div class="terms">Tags: <?php print $terms ?></div>
    <?php endif;?>
    </div>
    
    <?php if ($links): ?>
    <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
    </div>
 
</div>