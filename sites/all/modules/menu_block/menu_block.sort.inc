<?php
// $Id: menu_block.sort.inc,v 1.2 2010/03/13 20:05:53 johnalbin Exp $

/**
 * @file
 * Provides optional sorting of the active trail in the menu tree.
 */

/**
 * Sort the active trail to the top of the tree.
 *
 * @param $tree
 *   array The menu tree to sort.
 * @return
 *   void
 */
function _menu_tree_sort_active_path(&$tree) {
  // To traverse the original tree down the active trail, we use a pointer.
  $current_level =& $tree;

  // Traverse the tree along the active trail.
  do {
    $next_level = $sort = $first_key = FALSE;
    foreach (array_keys($current_level) AS $key) {
      // Save the first key for later use.
      if (!$first_key) {
        $first_key = $key;
      }
      if ($current_level[$key]['link']['in_active_trail'] && $current_level[$key]['below']) {
        // Don't re-sort if its already sorted.
        if ($key != $first_key) {
          // Create a new key that will come before the first key.
          list($first_key, ) = explode(' ', $first_key);
          $first_key--;
          list(, $new_key) = explode(' ', $key, 2);
          $new_key = "$first_key $new_key";
          // Move the item to the new key.
          $current_level[$new_key] = $current_level[$key];
          unset($current_level[$key]);
          $key = $new_key;
          $sort = TRUE; // Flag sorting.
        }
        $next_level = $key; // Flag subtree.
        break;
      }
    }
    // Sort this level.
    if ($sort) {
      ksort($current_level);
    }
    // Continue in the subtree, if it exists.
    if ($next_level) {
      $current_level =& $current_level[$next_level]['below'];
    }
  } while ($next_level);
}
