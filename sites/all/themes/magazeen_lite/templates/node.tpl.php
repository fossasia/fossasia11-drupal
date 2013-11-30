<?php
// $Id: node.tpl.php,v 1.1 2010/08/21 09:00:49 skounis Exp $
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

    <div class="node-meta clearfix">
    
        <h3 class="node-title left"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h3>
        <span class="submitted node-info right"><?php if ($submitted): ?><?php print $submitted; ?><?php endif; ?></span>
    
    </div><!--/node-meta-->


    <div class="node-box clearfix">
    
    <h2 class="comments-header"><?php print $node->comment_count ?> <?php print t('Comments'); ?></h2>
    
        <div class="node-content clearfix"><?php print $content ?></div><!--/node-content-->
    
        <div class="node-footer clearfix">
        
            <div class="meta">
            
            <?php if ($taxonomy): ?>
            <div class="terms"><?php print $terms ?></div>
            <?php endif;?>
            
            <?php if ($links): ?>
            <div class="links"><?php print $links; ?></div>
            <?php endif; ?>
            
            </div><!--/meta-->
        
        </div><!--/node-footer-->
    
    </div><!--/node-box-->

</div><!--/node-->
