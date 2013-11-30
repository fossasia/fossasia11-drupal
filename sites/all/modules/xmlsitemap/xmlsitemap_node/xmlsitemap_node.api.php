<?php

/**
 * @file
 * Hooks provided by the XML sitemap node module.
 *
 * @ingroup xmlsitemap
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter user access rules when trying to view, edit or delete a node.
 *
 * Node access modules establish rules for user access to content.
 * hook_node_grants() defines permissions for a user to view, edit or delete
 * nodes by building a $grants array that indicates the permissions assigned to
 * the user by each node access module. This hook is called to allow modules to
 * modify the $grants array by reference, so the interaction of multiple node
 * access modules can be altered or advanced business logic can be applied.
 *
 * This is a backport of the hook with the same name in Drupal 7 core. If this
 * hook is invoked to determine if the anonymous user can access a node
 * regardless of context, the $account->xmlsitemap_node_access will be TRUE.
 *
 * @param &$grants
 *   The $grants array returned by hook_node_grants().
 * @param $account
 *   The user account requesting access to content.
 * @param $op
 *   The operation being performed, 'view', 'update' or 'delete'.
 *
 * @see hook_node_grants()
 */
function hook_node_grants_alter(&$grants, $account, $op) {
  if (!empty($account->xmlsitemap_node_access)) {
    unset($grants['vocabulary']);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
