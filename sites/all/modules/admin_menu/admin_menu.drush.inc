<?php

/**
 * Implments hook_drush_exit()
 *
 * This is necessary because drush skips the normal hook_exit() where
 * we build the links.
 */
function admin_menu_drush_exit() {
  if (function_exists('admin_menu_exit')) {
    admin_menu_exit();
  }
}

