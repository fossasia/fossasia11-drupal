<?php
// $Id: menu_block.book.inc,v 1.2 2010/03/12 16:14:17 johnalbin Exp $

/**
 * @file
 * Provides book integration.
 */

/**
 * Implements hook_get_menus().
 */
function book_get_menus() {
  $menus = array();
  foreach (book_get_books() AS $book) {
    $menus[$book['menu_name']] = $book['title'];
  }
  return $menus;
}
