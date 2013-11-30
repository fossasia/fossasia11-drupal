<?php
/**
 * @file
 * Bouncer module integration.
 */

/**
 * Implements hook_sexybookmarks_config_alter() on behalf of bouncer.module.
 */
function bouncer_sexybookmarks_config_alter($config, $variables) {
  $variables[0]['url'] = isset($variables[0]['url']) ? $variables[0]['url'] : '';
  $variables[0]['url'] = bouncer_url($variables[0]['url']) . "&c=sexybookmarks";
}
