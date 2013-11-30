<?php
// $Id: views-slideshow-ddblock-cycle-block-content-fever.tpl.php,v 1.1 2010/08/05 07:52:03 antsin Exp $

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

$settings = $views_slideshow_ddblock_slider_settings;
// add Cascading style sheet
drupal_add_css($directory .'/custom/modules/views_slideshow_ddblock/'.$settings['custom_template']. '/views-slideshow-ddblock-cycle-'.$settings['custom_template']. '.css', 'template', 'all', FALSE);
?>
<!-- dynamic display block slideshow -->
<div id="views-slideshow-ddblock-<?php print $settings['delta'] ?>" class="views-slideshow-ddblock-cycle-<?php print $settings['custom_template'] ?> clear-block">
 <div class="container clear-block border">
  <div class="container-inner clear-block border"> 
   <!-- slider content -->
   <div class="slider clear-block border">
    <div class="slider-inner clear-block border">
     <?php if ($settings['output_type'] == 'view_fields') : ?>
      <?php foreach ($views_slideshow_ddblock_slider_items as $slider_item): ?>
       <div class="slide clear-block border">
        <div class="slide-inner clear-block border">
         <?php print $slider_item['slide_image']; ?>
          <div class="slide-text slide-text-<?php print $settings['slide_direction'] ?> slide-text-<?php print $settings['slide_text_position'] ?> clear-block border">
           <div class="slide-text-inner clear-block border">
           <?php if ($settings['slide_text'] == 1) :?>
            <div class="slide-body-<?php print $settings['slide_direction'] ?> clear-block border">
             <div class="slide-body-inner clear-block border">
              <h2><?php print $slider_item['slide_title'] ?></h2>
              <p><?php print strip_tags($slider_item['slide_text'], '<a>');?><span class="read-more"><?php print $slider_item['slide_read_more'] ?></span></p>
             </div> <!-- slide-body-inner-->
            </div>  <!-- slide-body-->
           <?php endif; ?>
          </div> <!-- slide-text-inner-->
         </div>  <!-- slide-text-->
        </div> <!-- slide-inner-->
       </div>  <!-- slide-->
      <?php endforeach; ?>
     <?php endif; ?>
    </div> <!-- slider-inner-->
   </div>  <!-- slider-->
    <!-- custom pager images--> 
    <?php print $views_slideshow_ddblock_pager_content ?>
  </div> <!-- container-inner-->
 </div> <!--container-->
</div> <!--  template -->
