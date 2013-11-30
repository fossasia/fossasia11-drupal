<?php
// $Id: comment-wrapper.tpl.php,v 1.1 2010/04/24 13:24:34 antsin Exp $

/*
+----------------------------------------------------------------+
|   BlogBuzz for Dupal 6.x - Version 2.0                         |
|   Copyright (C) 2010 Antsin.com All Rights Reserved.           |
|   @license - GNU GENERAL PUBLIC LICENSE                        |
|----------------------------------------------------------------|
|   Theme Name: BlogBuzz                                         |
|   Description: BlogBuzz by Antsin                              |
|   Author: Antsin.com                                           |
|   Website: http://www.antsin.com/                              |
|----------------------------------------------------------------+
*/ 
?>

<?php if ($content): ?>
  <div id="comments">
    <?php if ($node->type != 'forum'): ?>
      <h2 class="title"><?php print t('Comments'); ?></h2>
    <?php endif; ?>
    <?php print $content; ?>
  </div>
<?php endif; ?>
