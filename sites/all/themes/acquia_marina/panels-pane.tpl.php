<?php
// $Id: panels-pane.tpl.php 7156 2010-04-24 16:48:35Z chris $
/**
 * @file panels-pane.tpl.php
 * Main panel pane template
 *
 * Variables available:
 * - $pane->type: the content type inside this pane
 * - $pane->subtype: The subtype, if applicable. If a view it will be the
 *   view name; if a node it will be the nid, etc.
 * - $title: The title of the content
 * - $content: The actual content
 * - $links: Any links associated with the content
 * - $more: An optional 'more' link (destination only)
 * - $admin_links: Administrative links associated with the content
 * - $feeds: Any feed icons or associated with the content
 * - $display: The complete panels display object containing all kinds of
 *   data including the contexts and all of the other panes being displayed.
 */

/**
 * $skinr variable, <div class="inner">, and 'content' in
 * <div class="pane-content content"> added for Fusion theming
 */

?>
<div class="<?php print $classes; ?> <?php print $skinr; ?>" <?php print $id; ?>>
  <div class="inner">
    <div class="corner-top"><div class="corner-top-right corner"></div><div class="corner-top-left corner"></div></div>
  	<div class="inner-wrapper">
      <div class="inner-inner">
        <?php if ($admin_links): ?>
          <div class="admin-links panel-hide">
            <?php print $admin_links; ?>
          </div>
        <?php endif; ?>
    
        <?php if ($title): ?>
          <div class="block-icon pngfix"></div>
          <h2 class="pane-title block-title"><?php print $title; ?></h2>
        <?php endif; ?>
    
        <?php if ($feeds): ?>
          <div class="feed">
            <?php print $feeds; ?>
          </div>
        <?php endif; ?>
    
        <div class="pane-content content">
          <?php print $content; ?>
        </div>
    
        <?php if ($links): ?>
          <div class="links">
            <?php print $links; ?>
          </div>
        <?php endif; ?>
    
        <?php if ($more): ?>
          <div class="more-link">
            <?php print $more; ?>
          </div>
        <?php endif; ?>
      </div><!-- /inner-inner -->
	  </div><!-- /inner-wrapper -->
    <div class="corner-bottom"><div class="corner-bottom-right corner"></div><div class="corner-bottom-left corner"></div></div>
  </div><!-- /inner -->
</div><!-- /block -->