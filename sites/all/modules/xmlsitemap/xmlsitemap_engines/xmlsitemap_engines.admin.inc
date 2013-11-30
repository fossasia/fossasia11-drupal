<?php

/**
 * @file
 * Administrative page callbacks for the xmlsitemap_engines module.
 */

/**
 * Form builder; Administration settings form.
 */
function xmlsitemap_engines_settings_form() {
  // Build the list of support engines for the checkboxes options.
  $engines = xmlsitemap_engines_get_engine_info();
  $engine_options = array();
  foreach ($engines as $engine => $engine_info) {
    $engine_options[$engine] = $engine_info['name'];
  }
  asort($engine_options);

  $form['xmlsitemap_engines_engines'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Submit the sitemap to the following engines'),
    '#default_value' => variable_get('xmlsitemap_engines_engines', array()),
    '#options' => $engine_options,
  );
  $form['xmlsitemap_engines_minimum_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Do not submit more often than every'),
    '#options' => drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 604800 * 2, 604800 * 4), 'format_interval'),
    '#default_value' => variable_get('xmlsitemap_engines_minimum_lifetime', 86400),
  );
  $form['xmlsitemap_engines_submit_updated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only submit if the sitemap has been updated since the last submission.'),
    '#default_value' => variable_get('xmlsitemap_engines_submit_updated', TRUE),
  );
  $form['xmlsitemap_engines_custom_urls'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom submission URLs'),
    '#description' => t('Enter one URL per line. The token [sitemap] will be replaced with the URL to your sitemap. For example: %example-before would become %example-after.', array('%example-before' => 'http://example.com/ping?[sitemap]', '%example-after' => xmlsitemap_engines_prepare_url('http://example.com/ping?[sitemap]', url('sitemap.xml', array('absolute' => TRUE))))),
    '#default_value' => variable_get('xmlsitemap_engines_custom_urls', ''),
    '#rows' => 2,
    '#wysiwyg' => FALSE,
    '#element_validate' => array('xmlsitemap_engines_validate_custom_urls'),
  );

  // Ensure the xmlsitemap_engines variable gets filterd to a simple array.
  $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);

  return system_settings_form($form);
}

/**
 * Validate the custom submission URL element.
 */
function xmlsitemap_engines_validate_custom_urls($element, &$form_state) {
  $custom_urls = preg_split('/[\r\n]+/', $element['#value'], -1, PREG_SPLIT_NO_EMPTY);
  foreach ($custom_urls as $custom_url) {
    $url = xmlsitemap_engines_prepare_url($custom_url, '');
    if (!valid_url($url, TRUE)) {
      form_error($element, t('Invalid URL %url.', array('%url' => $custom_url)));
    }
  }
  $form_state['values']['xmlsitemap_engines_custom_urls'] = implode("\n", $custom_urls);
}
