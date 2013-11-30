<div class="<?php print $classes .' '. $zebra; ?> clearfix">
  <div class="comment-inner">

    <?php if ($title): ?>
      <h3 class="title"><?php print $title ?></h3>
    <?php endif; ?>

    <?php if ($new) : ?>
      <span class="new"><?php print drupal_ucfirst($new); ?></span>
    <?php endif; ?>

    <?php print $picture; ?>

    <div class="submitted">
      <?php print $submitted; ?>
    </div>

    <div class="content">
      <?php print $content ?>
      <?php if ($signature): ?>
        <div class="user-signature clearfix">
          <?php print $signature; ?>
        </div>
      <?php endif; ?>
    </div>

    <?php if ($links): ?>
      <div class="links">
        <?php print $links; ?>
      </div>
    <?php endif; ?>  

  </div> <!-- /comment-inner -->
</div> <!-- /comment -->