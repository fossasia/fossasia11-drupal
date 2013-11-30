<?php
// $Id: block.tpl.php,v 1.1 2010/08/21 09:00:49 skounis Exp $
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">

<?php if (!empty($block->subject)): ?>
  <h2><?php print $block->subject ?></h2>
<?php endif;?>

  <div class="content clearfix"><?php print $block->content ?></div>
</div>
