<?php
// $Id: views_attach.views.inc,v 1.1.2.1.2.5 2009/10/02 21:12:01 crell Exp $

/**
 * Implementation of hook_views_plugins().
 */
function views_attach_views_plugins() {
  return array(
    'display' => array(
      'profile' => array(
        'title' => t('Profile'),
        'help' => t('Display the view as a part of a user profile.'),
        'handler' => 'views_attach_plugin_display_profile',
        'theme' => 'views_attach_display_profile_view',
        'theme file' => 'views_attach.theme.inc',
        'uses hook user' => TRUE,
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'use more' => TRUE,
        'accept attachments' => TRUE,
        'admin' => t('Profile'),
        'help topic' => 'display-profile',
      ),
      'node_content' => array(
        'title' => t('Node content'),
        'help' => t("Display the view as part of a node's content."),
        'handler' => 'views_attach_plugin_display_node_content',
        'theme' => 'views_attach_display_node_content_view',
        'theme file' => 'views_attach.theme.inc',
        'theme path' => drupal_get_path('module', 'views_attach'),
        'uses hook nodeapi' => TRUE,
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'use more' => TRUE,
        'accept attachments' => TRUE,
        'admin' => t('Node content'),
        'help topic' => 'display-node-content',
      ),
    ),
  );
}
