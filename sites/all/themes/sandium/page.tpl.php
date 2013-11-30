<?php
// $Id$
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
<?php print $head ?>
<title><?php print $head_title ?></title>
<?php print $styles ?>
<?php print $scripts ?>
<script type="text/javascript">
    $(document).ready(function() {
        $('ul.sf-menu').superfish({
            delay:       700,                            // one second delay on mouseout
            animation:   {opacity:'show'},  // fade-in and slide-down animation
            speed:       'fast',                          // faster animation speed
            autoArrows:  false,                           // disable generation of arrow mark-up
            dropShadows: false                            // disable drop shadows
        });
    });
</script>

</head>
<body>
    <div id="container">
      <div id="header">
        <div id="site-info">
        <?php if (!empty($logo)): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a> <?php endif; ?>
          <div id="site-title"><h1><a href="<?php print base_path() ?>"><?php print $site_name; ?></a></h1>
          <div id="slogan"><?php print $site_slogan; ?></div>
          </div>

        </div>
        <div id="search-box"><?php print $search_box; ?><?php if($search): ?><img class="catch-me" src="<?php print base_path() . path_to_theme() ?>/images/search.png" /><?php endif; ?></div>
      </div>
      
      <div id="nav">
<ul class='sf-menu'>
<?php
$tree = menu_tree_page_data('primary-links');

  $output = '';
  $items = array();

  // Pull out just the menu items we are going to render so that we
  // get an accurate count for the first/last classes.
  foreach ($tree as $data) {
    if (!$data['link']['hidden']) {
      $items[] = $data;
    }
  }

  $num_items = count($items);
  foreach ($items as $i => $data) {
    $extra_class = NULL;
    if ($i == 0) {
      $extra_class = 'first';
    }
    if ($i == $num_items - 1) {
      $extra_class = 'last';
    }
    $link = theme('menu_item_link', $data['link']);
    if ($data['below']) {
      $output .= theme('menu_item', $link, $data['link']['has_children'], menu_tree_output($data['below']), $data['link']['in_active_trail'], $extra_class);
    }
    else {
      $output .= theme('menu_item', $link, $data['link']['has_children'], '', $data['link']['in_active_trail'], $extra_class);
    }
  }

print $output;
?>
</ul>
        </div>
      <div id="content-wrapper">
        <?php if($slash): ?><div id="slash"><?php print $slash ?></div><?php endif; ?>
        <div class="clear"></div>
        <?php if ($top_left || $top_middle || $top_right): ?>
        <div id="content-top">
          <div id="top-left"><?php print $top_left ?></div>
          <div id="top-middle"><?php print $top_middle ?></div>
          <div id="top-right"><?php print $top_right ?></div>
          <div class="clear"></div>
        </div>
        <?php endif; ?>
        <div class="clear"></div>

        <div id="content-bottom">
          <div id="leftside" class="<?php if (!$rightside) print 'full' ?>">
            <?php print $leftside ?>
            <?php if ($messages): ?><h5><i><?php print $messages ?></i></h5><?php endif; ?>
            <?php if ($tabs): ?><div class="tabs"><?php print $tabs ?></div><?php endif; ?>
            <?php if($tabs2): ?><div class="tabs2"><?php print $tabs2 ?></div><?php endif; ?>
            <?php print $content ?>
          </div>
          <?php if ($rightside): ?>
          <div id="rightside">
            <?php print $rightside ?>
          </div>
          <?php endif; ?>
          <div class="clear"></div>
        </div>
        <?php if ($bottom_left || $bottom_middle || $bottom_right): ?>
        <div id="content-verybottom">
          <?php if($bottom_left): ?><div id="bottom-left" class="class="<?php print $body_classes; ?>""><?php print $bottom_left ?></div><?php endif; ?>
          <?php if($bottom_middle): ?><div id="bottom-middle" class="<?php print $body_classes; ?>"><?php print $bottom_middle ?></div><?php endif; ?>
          <?php if($bottom_right): ?><div id="bottom-right" class="<?php print $body_classes; ?>"><?php print $bottom_right ?></div><?php endif; ?>
        </div>
        <?php endif; ?>
        <div class="clear"></div>
        
      </div><!-- end of content-wrapper -->
          <div id="footer">
            <?php if ($footer_left || $footer_right): ?>
            <div id="footer-left"><?php print $footer_left ?></div>
            <div class="fasel"></div>
            <div id="footer-right"><?php print $footer_right ?></div>
            <div class="clear"></div>
            <?php endif; ?>
            <div class="footer-message"><?php print $footer_message ?></div>
          </div>
    </div><!-- end of container -->
  </body>
    <?php print $closure ?>
</html>
