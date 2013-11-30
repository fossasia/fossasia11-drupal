<?php
// $Id: menu_block.follow.inc,v 1.4 2010/03/22 14:11:27 johnalbin Exp $

/**
 * @file
 * Provides active menu item pruning.
 */

/**
 * Prune a tree so that it begins at the active menu item.
 *
 * @param $tree
 *   array The menu tree to prune.
 * @param $level
 *   string The level which the tree will be pruned to: 'active' or 'child'.
 * @return
 *   void
 */
function _menu_tree_prune_active_tree(&$tree, $level) {
  do {
    $found_active_trail = FALSE;
    // Examine each element at this level for the active trail.
    foreach (array_keys($tree) AS $key) {
      if ($tree[$key]['link']['in_active_trail']) {
        $found_active_trail = TRUE;
        // If the active trail item has children, examine them.
        if ($tree[$key]['below']) {
          // If we are pruning to the active menu item's level, check if this
          // is the active menu item by checking its children.
          if ($level == 'active') {
            foreach (array_keys($tree[$key]['below']) AS $child_key) {
              if ($tree[$key]['below'][$child_key]['link']['in_active_trail']) {
                // Get the title for the pruned tree.
                menu_block_set_title($tree[$key]['link']);
                $tree = $tree[$key]['below'];
                // Continue in the pruned tree.
                break 2;
              }
            }
            // If we've found the active item, we're done.
            break 2;
          }
          // Set the title for the pruned tree.
          menu_block_set_title($tree[$key]['link']);
          // If we are pruning to the children of the active menu item, just
          // prune the tree to the children of the item in the active trail.
          $tree = $tree[$key]['below'];
          // Continue in the pruned tree.
          break;
        }
        // If the active menu item has no children, we're done.
        else {
          if ($level == 'child') {
            $tree = array();
          }
          break 2;
        }
      }
    }
  } while ($found_active_trail);
}
