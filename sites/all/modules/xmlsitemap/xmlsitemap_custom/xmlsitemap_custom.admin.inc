<?php

/**
 * @file
 * Administrative page callbacks for the xmlsitemap_custom module.
 */

function xmlsitemap_custom_link_links_local_actions() {
  $local_actions['add'] = xmlsitemap_get_operation_link('admin/settings/xmlsitemap/custom/add', array('modal' => TRUE));
  return $local_actions;
}

function xmlsitemap_custom_list_links() {
  $output = '';
  $output .= theme('links', xmlsitemap_custom_link_links_local_actions(), array('class' => 'item-list action-links'));

  $header = array(
    'loc' => array('data' => t('Location'), 'field' => 'loc', 'sort' => 'asc'),
    'priority' => array('data' => t('Priority'), 'field' => 'priority'),
    'changefreq' => array('data' => t('Change frequency'), 'field' => 'changefreq'),
    'language' => array('data' => t('Language'), 'field' => 'language'),
    'operations' => array('data' => t('Operations')),
  );
  // Do not include the language column if locale is disabled.
  if (!module_exists('locale')) {
    unset($header['language']);
  }

  $rows = array();
  $destination = drupal_get_destination();

  $sql = "SELECT * FROM {xmlsitemap} WHERE type = 'custom'";
  $sql .= tablesort_sql($header);
  $query = pager_query($sql, 50);

  while ($link = db_fetch_object($query)) {
    $row = array();
    $row['loc'] = l($link->loc, $link->loc);
    $row['priority'] = number_format($link->priority, 1);
    $row['changefreq'] = $link->changefreq ? drupal_ucfirst(xmlsitemap_get_changefreq($link->changefreq)) : t('None');
    if (isset($header['language'])) {
      $row['language'] = module_invoke('locale', 'language_name', $link->language);
    }
    $operations = array();
    $operations['edit'] = xmlsitemap_get_operation_link('admin/settings/xmlsitemap/custom/edit/' . $link->id, array('title' => t('Edit'), 'modal' => TRUE));
    $operations['delete'] = xmlsitemap_get_operation_link('admin/settings/xmlsitemap/custom/delete/' . $link->id, array('title' => t('Delete'), 'modal' => TRUE));
    $row['operations'] = theme('links', $operations, array('class' => 'links inline nowrap'));
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No custom links available.') . ' ' . l(t('Add custom link'), 'admin/settings/xmlsitemap/custom/add', array('query' => $destination)),
        'colspan' => count($header),
      ),
    );
  }

  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, 50, 0);
  return $output;
}

function xmlsitemap_custom_edit_link_form($form_state, $link = array()) {
  module_load_include('inc', 'xmlsitemap', 'xmlsitemap.admin');
  _xmlsitemap_set_breadcrumb('admin/settings/xmlsitemap/custom');

  $link += array(
    'id' => db_result(db_query("SELECT MAX(id) FROM {xmlsitemap} WHERE type = 'custom'")) + 1,
    'loc' => '',
    'priority' => XMLSITEMAP_PRIORITY_DEFAULT,
    'lastmod' => 0,
    'changefreq' => 0,
    'changecount' => 0,
    'language' => '',
  );

  $form['type'] = array(
    '#type' => 'value',
    '#value' => 'custom',
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $link['id'],
  );
  $form['loc'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to link'),
    '#field_prefix' => url('', array('absolute' => TRUE)),
    '#default_value' => $link['loc'] ? drupal_get_path_alias($link['loc'], $link['language']) : '',
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['priority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#options' => xmlsitemap_get_priority_options(),
    '#default_value' => number_format($link['priority'], 1),
    '#description' => t('The priority of this URL relative to other URLs on your site.'),
  );
  $form['changefreq'] = array(
    '#type' => 'select',
    '#title' => t('Change frequency'),
    '#options' => array(0 => t('None')) + xmlsitemap_get_changefreq_options(),
    '#default_value' => $link['changefreq'],
    '#description' => t('How frequently the page is likely to change. This value provides general information to search engines and may not correlate exactly to how often they crawl the page.'),
  );
  $languages = module_exists('locale') ? locale_language_list() : array();
  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#default_value' => $link['language'],
    '#options' => array('' => t('Language neutral')) + $languages,
    '#access' => $languages,
  );

  $form['actions'] = array(
    '#weight' => 100,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );
  $form['actions']['cancel'] = array(
    '#value' => l(t('Cancel'), isset($_GET['destination']) ? $_GET['destination'] : 'admin/settings/xmlsitemap/custom'),
    '#weight' => 10,
  );

  return $form;
}

function xmlsitemap_custom_edit_link_form_validate($form, &$form_state) {
  $link = &$form_state['values'];

  // Make sure we trim and normalize the path first.
  $link['loc'] = trim($link['loc']);
  $link['loc'] = drupal_get_normal_path($link['loc'], $link['language']);

  // Test anonymous user access to the custom link paths.
  xmlsitemap_switch_user(0);
  $menu_item = menu_get_item($link['loc']);
  xmlsitemap_restore_user();

  // Since the menu item access results are cached, manually check the current path.
  if ($menu_item && strpos($link['loc'], 'admin/settings/xmlsitemap/custom') === 0 && !user_access('administer xmlsitemap', drupal_anonymous_user())) {
    $menu_item['access'] = FALSE;
  }

  if (db_result(db_query_range("SELECT 1 FROM {xmlsitemap} WHERE type <> 'custom' AND loc = '%s' AND status = 1 AND access = 1 AND language IN ('%s', '')", $link['loc'], $link['language'], 0, 1))) {
    form_set_error('loc', t('There is already an existing link in the sitemap with the path %link.', array('%link' => $link['loc'])));
  }
  elseif (empty($menu_item['access']) && !is_readable('./' . $link['loc'])) {
    // @todo Harden this file exists check to make sure we can't link to files
    // like .htaccess.
    form_set_error('loc', t('The custom link %link is either invalid or it cannot be accessed by anonymous users.', array('%link' => $link['loc'])));
  }
}

function xmlsitemap_custom_edit_link_form_submit($form, &$form_state) {
  $link = $form_state['values'];
  xmlsitemap_link_save($link);
  drupal_set_message(t('The custom link for %loc was saved.', array('%loc' => $link['loc'])));
  $form_state['redirect'] = 'admin/settings/xmlsitemap/custom';
}

function xmlsitemap_custom_delete_link_form($form_state, array $link) {
  _xmlsitemap_set_breadcrumb('admin/settings/xmlsitemap/custom');

  $form['link'] = array(
    '#type' => 'value',
    '#value' => $link,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the custom link for %loc?', array('%loc' => $link['loc'])),
    'admin/settings/xmlsitemap/custom',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function xmlsitemap_custom_delete_link_form_submit($form, &$form_state) {
  $link = $form_state['values']['link'];
  xmlsitemap_link_delete('custom', $link['id']);
  drupal_set_message(t('The custom link for %loc has been deleted.', array('%loc' => $link['loc'])));
  watchdog('xmlsitemap', 'The custom link for %loc has been deleted.', array('%loc' => $link['loc']), WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/settings/xmlsitemap/custom';
}
