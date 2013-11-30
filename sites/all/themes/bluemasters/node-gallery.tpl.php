<?php phptemplate_comment_wrapper(NULL, $node->type); ?>
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js" type="text/javascript"></script>
<script type="text/javascript" src="<?php print drupal_get_path('theme', 'skodaxanthifc') . '/lib/popeye/jquery.popeye-2.0.4.min.js'?>"></script>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

    <?php if ((arg(0)=="taxonomy" && arg(1)=="term") || $page==0): ?>
    <h2 class="page-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    <?php endif; ?>
    
    <?php if (!((arg(0)=="taxonomy" && arg(1)=="term") || $page==0)): ?>
    <h2 class="page-title"><?php print $title?></h2>
    <?php endif; ?>
    
    <?php if ($submitted): ?>
    <!--<span class="submitted"><?php //print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>-->
    <?php endif; ?>
    
 
<div class="ppy" id="ppy1">
            <ul class="ppy-imglist">

			<?php $files = upload_load(node_load($nid));
            
            $rows = array();
            
            foreach ($files as $file) {
            if ($file->list) {
                $mime = explode('/', file_get_mimetype($file->filename));
                $type= $mime[0];
                    switch($type){
                        case 'image':
                        $href = $file->fid ? file_create_url($file->filepath) : url(file_create_filename($file->filename, file_create_path())); ?> 
                        <li>
                        <a href="<?php print $href; ?>">
                            <img src="<?php print $href; ?>" alt="<?php print $file->description; ?>" />
                        </a>
                        <span class="ppy-extcaption">
                            <?php print $file->description; ?>
                        </span>
                    </li>
            <?php } } } ?>

            </ul>
            
            <div class="ppy-outer">
                <div class="ppy-stage">
                    <div class="ppy-nav">
                        <a class="ppy-prev" title="Previous image">Previous image</a>
                        <a class="ppy-switch-enlarge" title="Enlarge">Enlarge</a>
                        <a class="ppy-switch-compact" title="Close">Close</a>
                        <a class="ppy-next" title="Next image">Next image</a>
                    </div>
                </div>
            </div>
            <div class="ppy-caption">
                <div class="ppy-counter">
                    Image <strong class="ppy-current"></strong> of <strong class="ppy-total"></strong> 
                </div>
                <span class="ppy-text"></span>
            </div>
            
            
        </div>

     <div class="content">
    <?php print $content ?>
    </div>
    
    <div class="clear-block clear">
        <div class="meta">
        <?php if ($taxonomy): ?>
        <div class="terms"><?php print $terms ?></div>
        <?php endif;?>
        </div>
        
        <?php if ($links): ?>
        <div class="links"><?php print $links; ?></div>
        <?php endif; ?>
    </div>
    
   
    
</div>
<!-- [example js] begin -->
<script type="text/javascript">
    <!--//<![CDATA[
    
    $(document).ready(function () {
        var options1 = {
            direction:  'right'
        }
    
        $('#ppy1').popeye(options1);
    });
    
    //]]>-->
</script>
<!-- [example js] end -->



