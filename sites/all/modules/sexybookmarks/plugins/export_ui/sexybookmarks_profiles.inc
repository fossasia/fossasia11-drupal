<?php
/**
 * @file
 * CTools Export UI plugin for SexyBookmarks profiles.
 */

$plugin = array(
  'schema' => 'sexybookmarks_profiles',
  'access' => 'administer sexybookmarks',
  'menu' => array(
    'menu prefix' => 'admin/settings',
    'menu item' => 'sexybookmarks',
    'menu title' => 'SexyBookmarks',
    'menu description' => 'Administer SexyBookmarks profiles.',
  ),
  'title singular' => t('profile'),
  'title singular proper' => t('Profile'),
  'title plural' => t('profiles'),
  'title plural proper' => t('Profiles'),
  'form' => array(
    'settings' => 'sexybookmarks_profiles_export_ui_form',
    'submit' => 'sexybookmarks_profiles_export_ui_form_submit',
  ),
);

/**
 * SexyBookmarks profiles settings form.
 */
function sexybookmarks_profiles_export_ui_form(&$form, &$form_state) {
  ctools_include('dependent');
  drupal_add_js(drupal_get_path('module', 'sexybookmarks') . '/scripts/sexybookmarks.admin.js');
  drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  drupal_add_js('misc/farbtastic/farbtastic.js');

  $config = $form_state['item']->config;
  $form['config'] = array(
    '#tree' => TRUE,
  );

  $form['config']['service'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($config['service']) ? $config['service'] : '5,7,88,2,3,257,74,52,207,191,236,54',
    '#theme' => 'sexybookmarks_services',
  );

  // Functionality settings.
  $form['config']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Functionality settings'),
    '#tree' => FALSE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['config']['settings']['fbLikeButton'] = array(
    '#type' => 'radios',
    '#title' => t('Include Facebook Like button'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => isset($config['fbLikeButton']) ? $config['fbLikeButton'] : 0,
    '#parents' => array('config', 'fbLikeButton'),
  );
  $form['config']['settings']['fbButtonPos'] = array(
    '#type' => 'select',
    '#title' => t('Facebook Like button location'),
    '#options' => array(
      'bottom-left' => t('Bottom Left'),
      'bottom-right' => t('Bottom Right'),
      'top-left' => t('Top Left'),
      'top-right' => t('Top Right'),
    ),
    '#default_value' => isset($config['fbButtonPos']) ? $config['fbButtonPos'] : 'bottom-right',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('radio:config[fbLikeButton]' => array('1')),
    '#parents' => array('config', 'fbButtonPos'),
  );
  $form['config']['settings']['dontShowShareCount'] = array(
    '#type' => 'radios',
    '#title' => t('Show Share counters'),
    '#options' => array(
      0 => t('Yes (recommended)'),
      1 => t('No'),
    ),
    '#default_value' => isset($config['dontShowShareCount']) ? $config['dontShowShareCount'] : 0,
    '#description' => t('For Facebook, Twitter, Google Buzz and Delicious'),
    '#parents' => array('config', 'dontShowShareCount'),
  );
  $form['config']['settings']['designer_toolTips'] = array(
    '#type' => 'radios',
    '#title' => t('Use Designer Tooltips'),
    '#options' => array(
      1 => t('Yes (recommended)'),
      0 => t('No'),
    ),
    '#default_value' => isset($config['designer_toolTips']) ? $config['designer_toolTips'] : 1,
    '#parents' => array('config', 'designer_toolTips'),
  );
  $form['config']['settings']['tip_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background Color for Tooltips'),
    '#size' => 7,
    '#maxlength' => 7,
    '#default_value' => isset($config['tip_bg_color']) ? $config['tip_bg_color'] : '#000000',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('radio:config[designer_toolTips]' => array('1')),
    '#parents' => array('config', 'tip_bg_color'),
  );
  $form['config']['settings']['tip_text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Text Color for Tooltips'),
    '#size' => 7,
    '#maxlength' => 7,
    '#default_value' => isset($config['tip_text_color']) ? $config['tip_text_color'] : '#FFFFFF',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('radio:config[designer_toolTips]' => array('1')),
    '#parents' => array('config', 'tip_text_color'),
  );
  $form['config']['settings']['rel'] = array(
    '#type' => 'radios',
    '#title' => t('Add Nofollow to Links'),
    '#options' => array(
      'nofollow' => t('Yes'),
      '' => t('No'),
    ),
    '#default_value' => isset($config['rel']) ? $config['rel'] : 'nofollow',
    '#parents' => array('config', 'rel'),
  );
  $form['config']['settings']['target'] = array(
    '#type' => 'radios',
    '#title' => t('Open Links in New Window'),
    '#options' => array(
      '_blank' => t('Yes'),
      '_self' => t('No'),
    ),
    '#default_value' => isset($config['target']) ? $config['target'] : '_blank',
    '#parents' => array('config', 'target'),
  );
  $form['config']['settings']['shrlink'] = array(
    '#type' => 'radios',
    '#title' => t('Show Shareaholic Link'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => isset($config['shrlink']) ? $config['shrlink'] : 1,
    '#parents' => array('config', 'shrlink'),
  );
  $form['config']['settings']['shortener'] = array(
    '#type' => 'select',
    '#title' => t('Which URL Shortener'),
    '#empty_value' => 'none',
    '#options' => array(
      'bitly' => t('bit.ly'),
      'google' => t('goo.gl'),
      'jmp' => t('jmp.li'),
      'supr' => t('su.pr'),
      'tinyurl' => t('tinyurl.com'),
    ),
    '#default_value' => isset($config['shortener']) ? $config['shortener'] : 'google',
    '#parents' => array('config', 'shortener'),
  );

  // Twitter specific settings.
  $form['config']['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter settings'),
    '#tree' => FALSE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['config']['twitter']['twitter_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Configure Custom Tweet Template'),
    '#default_value' => isset($config['twitter_template']) ? $config['twitter_template'] : '${title} - ${short_link} via @Shareaholic',
    '#description' =>
      '<strong>' . t('Configuration Instructions') . ':</strong><br />'
      . '<em>' . t('Using the strings ${title} and ${short_link} you can fully customize your tweet output.') . '</em>'
      . '<p>'
      . '  <strong>' . t('Example Configurations') . ':</strong><br />'
      . '  <em>' . t('${title} - ${short_link} (via @Shareaholic)') . '</em><br />'
      . '  ' . t('or') . '<br />'
      . '  <em>' . t('RT @Shareaholic: ${title} - ${short_link}') . '</em>'
      . '</p>',
    '#parents' => array('config', 'twitter_template'),
  );

  // Aesthetic settings.
  $form['config']['aesthetics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Aesthetic settings'),
    '#tree' => FALSE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['config']['aesthetics']['expand'] = array(
    '#type' => 'radios',
    '#title' => t('Animate-expand multi-lined bookmarks'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => isset($config['expand']) ? $config['expand'] : 1,
    '#parents' => array('config', 'expand'),
  );
  $form['config']['aesthetics']['positioning'] = array(
    '#type' => 'radios',
    '#title' => t('Auto-space/center the bookmarks'),
    '#options' => array(
      'spaced' => t('Space'),
      'center' => t('Center'),
      'no' => t('No'),
    ),
    '#default_value' => isset($config['spaced']) && $config['spaced'] ? 'spaced' : (
      isset($config['center']) && $config['center'] ? 'center' : 'no'
    ),
    '#parents' => array('config', 'positioning'),
  );
}

/**
 * SexyBookmarks profiles settings form; submit callback.
 */
function sexybookmarks_profiles_export_ui_form_submit(&$form, &$form_state) {
  $form_state['values']['config']['center'] = $form_state['values']['config']['positioning'] == 'center';
  $form_state['values']['config']['spaced'] = $form_state['values']['config']['positioning'] == 'spaced';
  unset($form_state['values']['config']['positioning']);
}

function theme_sexybookmarks_services($form_element) {
  $output = '';

  // List of SexyBookmarks services.
  $services = array(
    2, 3, 4, 5, 6, 7, 10, 24, 38, 39, 40, 52, 54, 74, 88, 201, 219, 257,
    27, 33, 41, 43, 45, 46, 53, 78, 195, 196, 205, 207, 210, 236, 247, 264,
    44, 48, 61, 77, 80, 100, 105, 140, 191, 202, 218, 238, 240, 266, 269, 280, 289,
    1, 14, 21, 89, 98, 102, 106, 243, 267, 268, 277, 278, 283, 284, 285, 286, 288,
    20, 59, 92, 188, 190, 192, 204, 246, 263, 265, 279, 281, 282, 287, 290, 291, 292, 293, 294,
  );
  $directory = md5(implode(',', $services));

  // Attach Javascript and Styles.
  $api = 'http://www.shareaholic.com/api/sprite/?v=2&apikey=' . SB_APIKEY . '&service=' . implode(',', $services);
  drupal_add_css(sexybookmarks_get_file($api, "{$directory}/sprite.css"));
  $styles = '
    background-image: url("' . file_create_url(sexybookmarks_get_file("{$api}&apitype=png", "{$directory}/sprite.png")) . '");
    cursor: move !important;
  ';
  jquery_ui_add(array('ui.sortable'));

  // Active services list.
  $active = array();
  foreach (explode(',', $form_element['#default_value']) as $item) {
    $active[] = array('data' => '', 'id' => "shr-{$item}", 'class' => "shr-{$item} shareaholic", 'style' => $styles);
  }
  $output .= '<div class="shr-bookmarks clear-block">' . theme('item_list', $active, t('Active services'), 'ul', array('id' => 'sexybookmarks-active', 'class' => 'shr-socials')) . '</div>';

  // Inactive services list.
  $inactive = array();
  foreach (array_diff($services, explode(',', $form_element['#default_value'])) as $item) {
    $inactive[] = array('data' => '', 'id' => "shr-{$item}", 'class' => "shr-{$item} shareaholic", 'style' => $styles);
  }
  $output .= '<div class="shr-bookmarks clear-block">' . theme('item_list', $inactive, t('Inactive services'), 'ul', array('id' => 'sexybookmarks-inactive', 'class' => 'shr-socials')) . '</div>';

  // Hidden field.
  $output .= theme('hidden', $form_element);

  return $output;
}
