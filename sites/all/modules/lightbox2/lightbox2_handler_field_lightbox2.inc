<?php
// $Id: lightbox2_handler_field_lightbox2.inc,v 1.1.2.4 2010/03/07 21:38:39 snpower Exp $

/**
 * A handler to provide a field that is completely custom by the administrator.
 *
 * @ingroup views_field_handlers
 */
class lightbox2_handler_field_lightbox2 extends views_handler_field {
  function query() {
    // Do nothing, as this handler does not need to do anything to the query itself.
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['trigger_field'] = array('default' => '');
    $options['popup'] = array('default' => '');
    $options['caption'] = array('default' => '');
    $options['rel_group'] = array('default' => TRUE);
    $options['custom_group'] = array('default' => '');
    $options['height'] = array('default' => '400px');
    $options['width'] = array('default' => '600px');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $fields = array('trigger_field' => t('<None>'));
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      // We only use fields up to this one.  Obviously we can't use this handler
      // as the trigger handler.
      if ($field == $this->options['id']) {
        break;
      }

      $fields[$field] = $handler->definition['title'];
    }

    $form['trigger_field'] = array(
      '#type' => 'select',
      '#title' => t('Trigger field'),
      '#description' => t('Select the field that should be turned into the trigger for the lightbox.  Only fields that appear before this one in the field list may be used.'),
      '#options' => $fields,
      '#default_value' => $this->options['trigger_field'],
      '#weight' => -12,
    );

    $form['popup'] = array(
      '#type' => 'textarea',
      '#title' => t('Popup'),
      '#description' => t('Combine tokens from the "Replacement patterns" below and html to create what the lightbox popup will become.'),
      '#default_value' => $this->options['popup'],
      '#weight' => -11,
    );

    $form['caption'] = array(
      '#type' => 'textfield',
      '#title' => t('Caption'),
      '#description' => t('Combine tokens from the "Replacement patterns" below and html to create the caption shown under the lightbox. Leave empty for no caption.'),
      '#default_value' => $this->options['caption'],
      '#weight' => -10,
    );

    $form['rel_group'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatic generated Lightbox group'),
      '#description' => t('Enable Lightbox grouping using a generated group name for this view.'),
      '#default_value' => $this->options['rel_group'],
      '#weight' => -9,
    );

    $form['custom_group'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom Lightbox group'),
      '#description' => t('Enable Lightbox grouping with a given string as group. Overrides the automatically generated group name above.'),
      '#default_value' => $this->options['custom_group'],
      '#weight' => -8,
    );

    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#description' => t('Specify the height of the lightbox2 popup window.  Because the content is dynamic, we cannot detect this value automatically.'),
      '#default_value' => $this->options['height'],
      '#weight' => -7,
    );

    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#description' => t('Specify the width of the lightbox2 popup window.  Because the content is dynamic, we cannot detect this value automatically.'),
      '#default_value' => $this->options['width'],
      '#weight' => -6,
    );



    // Remove the checkboxs and other irrelevant controls.
    unset($form['alter']['alter_text']);
    unset($form['alter']['make_link']);
    unset($form['alter']['text']);
    unset($form['alter']['path']);
    unset($form['alter']['alt']);
    unset($form['alter']['prefix']);
    unset($form['alter']['suffix']);
    unset($form['alter']['text']['#dependency']);
    unset($form['alter']['text']['#process']);
  }

  /**
   * Render the trigger field and its linked popup information.
   */
  function render($values) {
    // We need to have multiple unique IDs, one for each record.
    static $i = 0;

    static $link;

    if (!empty($this->options['trigger_field'])) {

      // We don't actually use the link, but we need it there for lightbox to function.
      if (empty($link)) {
        // Get the path name.
        $path = isset($_GET['q']) ? $_GET['q'] : '<front>';
        $link = url($path, array('absolute' => TRUE));
      }

      // Get the token information and generate the value for the popup and the
      // caption.
      $tokens = $this->get_render_tokens($this->options['alter']);
      $popup = filter_xss_admin($this->options['popup']);
      $caption = filter_xss_admin($this->options['caption']);
      $popup = strtr($popup, $tokens);
      $caption = strtr($caption, $tokens);

      $i++;

      // The outside div is there to hide all of the divs because if the specific lightbox
      // div is hidden it won't show up as a lightbox.  We also specify a group
      // in the rel attribute in order to link the whole View together for paging.
      $group_name = !empty($this->options['custom_group']) ? $this->options['custom_group'] : ($this->options['rel_group'] ? 'lightbox-popup-' . $this->view->name . '-' . implode('/', $this->view->args) : '');
      return "<a href='$link #lightbox-popup-{$i}'  rel='lightmodal[{$group_name}|width:" . ($this->options['width'] ? $this->options['width'] : '600px') . ';height:' . ($this->options['height'] ? $this->options['height'] : '600px') . "][" . $caption . "]'>". $tokens["[{$this->options['trigger_field']}]"] ."</a>
                <div style='display: none;'><div id='lightbox-popup-{$i}' class='lightbox-popup'>$popup</div></div>";
    }
    else {
      return;
    }
  }
}
