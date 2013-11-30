<?php
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<?php global $base_url;?>
<title><?php print $head_title ?></title>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts; ?>

<script src="misc/jquery.js" type="text/javascript"></script>
<script src="<?php print drupal_get_path('theme', 'bluemasters') . '/js/bluemasters.js'?>" type="text/javascript"></script>

<script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.7.2/jquery-ui.js" type="text/javascript"></script>
<script src="http://ajax.googleapis.com/ajax/libs/swfobject/2.1/swfobject.js" type="text/javascript"></script>

</head>
<body>

<div id="page">

<div id="header-top"><!--header-top-->
    <div id="header-top-inside" class="clearfix">
        <div id="header-top-inside-left">Subscribe to: <div id="header-top-inside-left-content"><?php print $header; ?> </div></div>
        <div id="header-top-inside-left-feed"><?php print $feed_icons ?></div>
        <div id="header-top-inside-right"><?php print $search_box; ?></div>  
    </div>
</div><!--/header-top-->

<div id="wrapper">

	<div id="header" class="clearfix">
	
	    <div id="logo"> 
			<?php
	        // Prepare header
	        $site_fields = array();
	        if ($site_name) {
	        	$site_fields[] = check_plain($site_name);
	        }
	        if ($site_slogan) {
	        	$site_fields[] = check_plain($site_slogan);
	        }
	        $site_title = implode(' ', $site_fields);
	        if ($site_fields) {
	        	$site_fields[0] = '<span>'. $site_fields[0] .'</span>';
	        }
	        $site_html = implode(' ', $site_fields);
	        
	        if ($logo || $site_title) {
	        	print '<a href="'. check_url($front_page) .'" title="'. $site_title .'">';
	        if ($logo) {
	        	print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" id="logo-image" />';
	        }
	        print '<div style="display:none">'.$site_html .'</div></a>';
	        }
	        ?>
	    </div> <!--logo-->
	    
	    <div id="navigation">
	    	<?php //if (isset($primary_links)) { ?><?php //print theme('links', $primary_links, array('class' =>'links', 'id' => 'primary-links')) ?><?php //} ?>
	        <?php print menu_tree($menu_name = 'primary-links'); ?>
	    </div><!--navigation-->
	
	</div><!--header-->


	<div id="banner" class="clearfix">
		<?php //print $banner;?>
		<div class="main_view">
			<div class="window">
				<div class="image_reel">
					<a href="?q=node/1"><img src="sites/all/themes/bluemasters/images/slide-image-1.jpg"></a>
					<a href="?q=node/2"><img src="sites/all/themes/bluemasters/images/slide-image-2.jpg"></a>
					<a href="?q=node/3"><img src="sites/all/themes/bluemasters/images/slide-image-3.jpg"></a>
				</div>
				<div class="descriptions">
					<div class="desc" style="display: none;">Our Portfolio</div>
					<div class="desc" style="display: none;">Creation of Beaches</div>
					<div class="desc" style="display: none;">About iPadMasters</div>
				</div>
				
			</div>
		
			<div class="paging" style="display: block;">
				<a rel="1" href="#" class="">1</a>
				<a rel="2" href="#" class="">2</a>
				<a rel="3" href="#" class="">3</a>
			</div>
		</div>
		
	</div><!--banner-->
	
	<div id="slide-navigation"></div>

	<div id="home-blocks-area" class="clearfix">
		<div id="home-block-1" class="home-block">
	    	<?php print $home_area_1;?> 		
	    </div>
	    <div id="home-block-2" class="home-block">
	    	<?php print $home_area_2;?> 
	    </div>
	    <div id="home-block-3" class="home-block">
	    	<?php print $home_area_3;?> 
	    	<div id="home-block-3-b">
	    		<?php print $home_area_3_b;?> 
	    	</div>
	    </div>
	</div>    
	
	<?php 
	// uncomment this to get news feed in your home page
	// you have to take care about the look'n'feel 
	// print $content;
	?> 
     
</div><!-- /wrapper-->

<div id="footer">
    <div id="footer-inside" class="clearfix">
    	<div id="footer-left">
    		<div id="footer-left-1">
    			<?php print $footer_left_1;?>
    		</div>
    		<div id="footer-left-2">
    			<?php print $footer_left_2;?>
    		</div>
        </div>
        <div id="footer-center">
        	<?php print $footer_center;?>
        </div>
        <div id="footer-right">
        	<?php print $footer_right;?>
        </div>
    </div>
</div>

<div id="footer-bottom">
    <div id="footer-bottom-inside" class="clearfix">
    	<div style="float:left">
    		<?php print $footer_message;?> 
    	</div>
    	<div style="float:right">
	        <?php if (isset($secondary_links)) : ?>
	          <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')) ?>
	        <?php endif; ?>      	
    	</div>
    	<div class="credits-container" style="clear:both; padding-top:12px;">Ported to Drupal for the Open Source Community by <a href="http://www.drupalizing.com">Drupalizing</a>, a Project of <a href="http://www.morethanthemes.com">More than Themes</a></div>     
    </div>
    <div class="credits-container clearfix">
        <a title="Brought To You By: www.SmashingMagazine.com" class="smashing" href="http://www.smashingmagazine.com" target="_blank">Brought to you By: www.SmashingMagazine.com</a>
        <a title="Drupalizing" class="drupalizing" href="http://www.drupalizing.com" target="_blank">Drupalizing</a> 
    </div>  
</div>
    
<?php print $closure ?>


</div><!-- /page-->

</body>
</html>
