<?php
// $Id: comment-wrapper.tpl.php,v 1.1 2010/08/05 07:51:20 antsin Exp $

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
?>

<?php if ($content): ?>
  <div id="comments">
    <?php if ($node->type != 'forum'): ?>
      <h2 id="comments-title"><?php print t('Comments'); ?></h2>
    <?php endif; ?>
    <?php print $content; ?>
  </div>
<?php endif; ?>
