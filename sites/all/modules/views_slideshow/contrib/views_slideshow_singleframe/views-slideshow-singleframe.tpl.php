<?php

/**
 * @file
 * Views Slideshow: Single Frame template file.
 */
?>

<?php if ($controls_top || $pager_top || $image_count_top): ?>
  <div class="views-slideshow-controls-top clear-block">
    <?php print $controls_top; ?>
    <?php print $pager_top; ?>
    <?php print $image_count_top; ?>
  </div>
<?php endif; ?>

<?php print $slideshow; ?>

<?php if ($controls_bottom || $pager_bottom || $image_count_bottom): ?>
  <div class="views-slideshow-controls-bottom clear-block">
    <?php print $controls_bottom; ?>
    <?php print $pager_bottom; ?>
    <?php print $image_count_bottom; ?>
  </div>
<?php endif; ?>
