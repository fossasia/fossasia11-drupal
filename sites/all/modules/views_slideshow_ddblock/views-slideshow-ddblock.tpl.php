<?php

/**
 * @file
 * Views Slideshow Dynamic display block module template: vsd-default - content template
 *
 * Available variables:
 * - $settings['delta']: Block number of the block.
 *
 * - $settings['template']: template name
 * - $settings['output_type']: type of content
 *
 * - $views_slideshow_ddblock_slider_items: array with slidecontent
 * - $settings['slide_text_position']: of the text in the slider (top | right | bottom | left)
 * - $settings['slide_direction']: direction of the text in the slider (horizontal | vertical )
 * -
 * - $settings['pager_content']: Themed pager content
 * - $settings['pager_position']: position of the pager (top | bottom)
 *
 * notes: don't change the ID names, they are used by the jQuery script.
 */
$settings = $views_slideshow_ddblock_slider_settings;
// add Cascading style sheet
drupal_add_css(drupal_get_path('module', 'views_slideshow_ddblock') . '/views-slideshow-ddblock-cycle-vsd-default.css', 'module', 'all', FALSE);
?>
<!-- dynamic display block slideshow -->
<div id="views-slideshow-ddblock-<?php print $settings['delta'] ?>" class="views-slideshow-ddblock-cycle-vsd-default clear-block">
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
            <div class="slide-title slide-title-<?php print $settings['slide_direction'] ?> clear-block border">
             <div class="slide-title-inner clear-block border">
              <h2><?php print $slider_item['slide_title'] ?></h2>
             </div> <!-- slide-title-inner-->
            </div>  <!-- slide-title-->
            <div class="slide-body-<?php print $settings['slide_direction'] ?> clear-block border">
             <div class="slide-body-inner clear-block border">
              <p><?php print $slider_item['slide_text'] ?></p>
             </div> <!-- slide-body-inner-->
            </div>  <!-- slide-body-->
           <?php endif; ?>
           <div class="slide-read-more slide-read-more-<?php print $settings['slide_direction'] ?> clear-block border">
            <p><?php print $slider_item['slide_read_more'] ?></p>
	         </div><!-- slide-read-more-->
          </div> <!-- slide-text-inner-->
         </div>  <!-- slide-text-->
        </div> <!-- slide-inner-->
       </div>  <!-- slide-->
      <?php endforeach; ?>
     <?php endif; ?>
    </div> <!-- slider-inner-->
   </div>  <!-- slider-->
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
  </div> <!-- container-inner-->
 </div> <!--container-->
</div> <!--  template -->
