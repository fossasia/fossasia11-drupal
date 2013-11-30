<?php // $Id$   ?>
<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">
  <?php if ($submitted): ?>
  	<div class="date">
      	<div class="textdate">
        		<div class="day"><?php print format_date($comment->timestamp, 'custom', 'j'); ?></div>
        		<div class="month"><?php print format_date($comment->timestamp, 'custom', 'M'); ?></div>
  	  	</div>
      </div>
  <?php endif; ?>
  <h3><?php print $title ?></h3>
  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; ?></span>
  <?php endif; ?>
	<div class="clear-block">
    <?php if ($comment->new) : ?>
      <span class="new"><?php print drupal_ucfirst($new) ?></span>
    <?php endif; ?>

  	<?php if($picture) { ?>
  	  <div id="userpicture">
  		  <?php print $picture ?>
  		</div>
  	<?php } ?>
	
		<div id="commentcontent">
      <div class="content">
        <?php print $content ?>
      	<?php if ($signature): ?>
      	  <div class="clear-block">
        	  <div>—</div>
        		<?php print $signature ?>
      		</div>
      	<?php endif; ?>
    	</div>
  	</div>
  	
  	<div class="permalink">
      <?php print l('#'. $comment->cid, 'node/'. $comment->nid, array('fragment' => 'comment-'. $comment->cid)); ?>
    </div>
    
		<?php if ($links): ?>
      <div class="links"><?php print $links ?></div>
  	<?php endif; ?>	
	</div>
</div>