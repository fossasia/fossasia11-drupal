<?php
// $Id: views-slideshow-ddblock.tpl.php,v 1.1 2010/08/05 07:52:03 antsin Exp $

/*
 * @file
 * Views Slideshow Dynamic display block module template
 *
 */  
?>
<!-- dynamic display block slideshow -->
<p>This preview does just shows the node titles of the nodes which will be show in slideshow</p>
<?php foreach ($view->result as $row): ?>
  <?php print $row->node_title; ?>
  <br />
<?php endforeach; ?>
