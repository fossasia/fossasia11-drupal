<?php
// $Id: template.php,v 1.1 2010/08/21 09:00:48 skounis Exp $

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
  }
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs. Overridden to split the secondary tasks.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  return menu_primary_local_tasks();
}

/**
 * Returns the themed submitted-by string for the comment.
 */
function phptemplate_comment_submitted($comment) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}

/**
 * Returns the themed submitted-by string for the node.
 */
function phptemplate_node_submitted($node) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $node),
      '!datetime' => format_date($node->created),
    ));
}

/**
  * Theme override for search form.
  */
  function magazeenlite_search_theme_form($form) {
   
    unset($form['search_theme_form']['#title']);
    $form['submit']['#value'] = '';
    $form['search_theme_form']['#value'] = 'Search.';
    $form['search_theme_form']['#attributes'] = array('onblur' => "if (this.value == '') {this.value = 'Search.';}", 'onfocus' => "if (this.value == 'Search.') {this.value = '';}" );

    $output .= drupal_render($form);
    return $output;
}