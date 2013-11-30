<div class="views-field-<?php print views_css_safe($view->field[$field]->field); ?>">
  <?php if ($view->field[$field]->label()) { ?>
    <label class="view-label-<?php print views_css_safe($view->field[$field]->field); ?>">
      <?php print $view->field[$field]->label(); ?>:
    </label>
  <?php } ?>
  <div class="views-content-<?php print views_css_safe($view->field[$field]->field); ?>">
    <?php print $view->style_plugin->rendered_fields[$count][$field]; ?>
  </div>
</div>
