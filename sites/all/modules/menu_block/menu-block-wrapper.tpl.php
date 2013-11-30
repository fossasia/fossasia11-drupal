<?php
// $Id: menu-block-wrapper.tpl.php,v 1.8 2010/03/17 06:30:20 johnalbin Exp $

/**
 * @file
 * Default theme implementation to wrap menu blocks.
 *
 * Available variables:
 * - $content: The unordered list containing the menu.
 * - $classes: A string containing the CSS classes for the DIV tag. Includes:
 *   menu-block-DELTA, menu-name-NAME, parent-mlid-MLID, and menu-level-LEVEL.
 * - $classes_array: An array containing each of the CSS classes.
 *
 * The following variables are provided for contextual information.
 * - $settings: An array of the block's configuration settings. Includes
 *   menu_name, parent_mlid, level, follow, depth, expanded, and sort.
 *
 * @see template_preprocess_menu_block_wrapper()
 */
?>
<div class="<?php print $classes; ?>">
  <?php print $content; ?>
</div>
