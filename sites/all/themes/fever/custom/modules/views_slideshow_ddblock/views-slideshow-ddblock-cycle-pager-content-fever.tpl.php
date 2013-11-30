<?php
// $Id: views-slideshow-ddblock-cycle-pager-content-fever.tpl.php,v 1.1 2010/08/05 07:52:03 antsin Exp $

/*
+----------------------------------------------------------------+
|   Fever for Dupal 6.x - Version 1.0                            |
|   Copyright (C) 2010 Antsin.com All Rights Reserved.           |
|   @license - GNU GENERAL PUBLIC LICENSE                        |
|----------------------------------------------------------------|
|   Theme Name: Fever                                            |
|   Description: Fever by Antsin                                 |
|   Author: Antsin.com                                           |
|   Date: 5th August 2010                                        |
|   Website: http://www.antsin.com/                              |
|----------------------------------------------------------------+
*/ 

 $settings = $views_slideshow_ddblock_pager_settings;
 
// jquery_plugin_add('scrollable');
 $base = drupal_get_path('module', 'views_slideshow_ddblock');
 drupal_add_js($base . '/js/tools.scrollable-1.0.5.min.js', 'theme');
?>

<!-- scrollable pager -->
<div id="views-slideshow-ddblock-scrollable-pager-<?php print $settings['delta'] ?>" class="<?php print $settings['pager'] ?> vsd-scrollable-pager clear-block border">
 <div class="items <?php print $settings['pager'] ?>-inner clear-block border">
  <?php if ($views_slideshow_ddblock_pager_items): ?>
   <?php $item_counter=1; ?>
   <?php foreach ($views_slideshow_ddblock_pager_items as $pager_item): ?>
    <div class="<?php print $settings['pager'] ?>-item <?php print $settings['pager'] ?>-item-<?php print $item_counter ?>">
      <a href="#" title="navigate to topic" class="pager-link"><?php print $pager_item['image'];?><span class="slide-title"><?php print $pager_item['slide_title']; ?></span><br/><?php print $pager_item['text']; ?></a>
    </div> <!-- /custom-pager-item -->
    <?php $item_counter++; ?>
   <?php endforeach; ?>
  <?php endif; ?>
 </div> <!-- /pager-inner-->
</div>  <!-- /scrollable pager-->

<!-- prev/next page on slide -->
<?php if ($settings['pager2'] == 1 && $settings['pager2_position']['slide'] === 'slide'): ?>
 <div class="views-slideshow-ddblock-prev-next-slide">
  <div class="prev-container">
   <a class="prev" href="#"><?php print $settings['pager2_slide_prev']?></a>
  </div>
  <div class="next-container">
   <a class="next" href="#"><?php print $settings['pager2_slide_next'] ?></a>
  </div>
 </div>
<?php endif; ?> 