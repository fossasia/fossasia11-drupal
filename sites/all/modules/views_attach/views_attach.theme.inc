<?php

function template_preprocess_views_attach_display_node_content_view(&$vars) {
  template_preprocess_views_view($vars);
  $view = $vars['view'];
  $display = $view->display_handler;
  if ($display->get_option('show_title')) {
    $vars['title'] = filter_xss_admin($view->get_title());
  }
}

function template_preprocess_views_attach_display_profile_view(&$vars) {
  template_preprocess_views_view($vars);
  $view = $vars['view'];
  $display = $view->display_handler;
  if ($display->get_option('show_title')) {
    $vars['title'] = filter_xss_admin($view->get_title());
  }
}
