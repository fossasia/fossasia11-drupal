<?php
// $Id: noderelationships.translation.inc,v 1.1.2.3 2010/03/07 19:08:17 markuspetrux Exp $

/**
 * @file
 * Translation support for Node Relationships module.
 */

/**
 * Implementation of hook_nodeapi('prepare translation').
 *
 * Here we can assume the following:
 * - Translation module is enabled.
 * - Translation for this content type is enabled.
 * - The current user is allowed to translate.
 * - The "Translate and reference" feature is enabled for this type.
 *
 * What do we have here?
 * - CCK copies all fields from source translation to new node.
 * - Nodereference module replaces the references for the corresponding
 *   translations, but also removes the values for those references that
 *   do not have a translation.
 *   Also, deltas for multiple value fields are destroyed, meaning
 *   nodereference module shifts deltas when removing references that
 *   do not have translations, and this breaks fields located in
 *   multigroups causing them to loose sync with other fields.
 *
 * How do we want it to be?
 * - We want to translate references as nodereference module does, but...
 *   for multiple value fields, we still want to keep the form elements for
 *   those that do not have translation because we'll provide a method to
 *   "Translate and reference" them, hence we need a form element to store
 *   the translated reference to.
 * - Also, we do not want to break nodereference fields in multigroups.
 *
 * @see translation_nodeapi()
 * @see i18n_db_rewrite_sql()
 * @see i18n_selection_mode()
 * @see content_prepare_translation()
 * @see content_field()
 * @see nodereference_field()
 * @see _noderelationships_parent_node_form_alter()
 */
function _noderelationships_nodeapi_prepare_translation(&$node, $noderef_settings) {
  // Let's concentrate only on fields where "Translate and reference" is enabled.
  foreach (array_keys($noderef_settings['translate_and_reference']) as $field_name) {
    // Skip this field if it does not exist in translation source.
    if (!isset($node->translation_source->{$field_name}) || !is_array($node->translation_source->{$field_name})) {
      continue;
    }

    // If the i18n module is NOT enabled, then the Allowed Values related
    // queries in nodereference module are NOT filtered by language, hence
    // all references in source translation are still valid in new node.
    // @todo: In fact, this could really be fixed in nodereference module.
    if (!module_exists('i18n')) {
      $node->{$field_name} = $node->translation_source->{$field_name};
      continue;
    }

    // When the i18n module is enabled, queries against the node table are
    // filtered to match the "Content selection mode" option enabled in the
    // site. Here, there are several options that allow language neutral
    // references, or options that allow only current language, default
    // language or both. However, when "Content selection mode" is disabled
    // all references in source translation are still valid in new node.
    // @todo: In fact, this could really be fixed in nodereference module.
    if (i18n_selection_mode() == 'off') {
      $node->{$field_name} = $node->translation_source->{$field_name};
      continue;
    }

    // When the i18n module is enabled, and "Content selection mode" is not
    // disabled, then the i18n module applies a complex logic to alters node
    // related queries to enforce language selection options.
    // The easiest way to see if references in source translation are still
    // valid for new node is perform a query using n.nid IN (...) with the
    // help of the nodereference code used to check for valid allowed values.
    // So, let's extract references in the translation source that are still
    // valid for this translation.
    $ids = array();
    foreach ($node->translation_source->{$field_name} as $delta => $item) {
      if (is_array($item) && !empty($item['nid']) && is_numeric($item['nid'])) {
        $ids[] = $item['nid'];
      }
    }
    $field = content_fields($field_name, $node->type);
    $valid_references = _nodereference_potential_references($field, '', NULL, $ids);

    // Ok, now we can try to rebuild the items of this nodereference field
    // while still keeping original deltas.
    $node->{$field_name} = array();
    foreach ($node->translation_source->{$field_name} as $delta => $reference) {
      // Keep delta even when the reference does not exist.
      if (empty($reference['nid']) || !is_numeric($reference['nid'])) {
        $node->{$field_name}[$delta] = array('nid' => NULL);
        continue;
      }

      // If the reference is valid for the translation, just get it as-is.
      if (isset($valid_references[$reference['nid']])) {
        $node->{$field_name}[$delta] = $reference;
        continue;
      }

      // Here we have a reference that is not valid for the translation due to
      // content selection rules enforced by i18n module. Let's try to find
      // translations for these references. To do so, we first need to read
      // the original referenced node.
      $reference_node = node_load($reference['nid']);

      // If translation is not supported by reference in source translation,
      // we can only warn the user about it. It makes no sense to offer them
      // a method to translate something that cannot be translated.
      if (!translation_supported_type($reference_node->type)) {
        $node->{$field_name}[$delta] = array(
          'translate_and_reference' => array(
            'translation_supported' => FALSE,
            'translation_source' => $reference_node,
          ),
          'nid' => NULL,
        );
        continue;
      }

      // Ok, let's see if we already have a translation for this reference
      // and use it if it exists. Note that we are creating a translation
      // here, so that means the node being translated has its language
      // attribute populated, and it is a valid language code.
      $translations = translation_node_get_translations($reference_node->tnid);
      if (!empty($translations) && isset($translations[$node->language])) {
        $node->{$field_name}[$delta] = array('nid' => $translations[$node->language]->nid);
        continue;
      }

      // Finally, we have a translatable reference that has not already been
      // translated. This is where our "Translate and reference" option can
      // add real value to the translation workflow. Let's provide a method
      // to easily translate this reference from the new node.
      $node->{$field_name}[$delta] = array(
        'translate_and_reference' => array(
          'translation_supported' => TRUE,
          'translation_source' => $reference_node,
        ),
        'nid' => NULL,
      );
    }
  }
}

/**
 * Build translation settings for the current node edit form.
 */
function _noderelationships_parent_node_form_build_translation_settings(&$node, &$field_settings, $noderef_settings) {
  foreach ($field_settings as $field_name => $field_options) {
    // Skip if "Translate and reference" feature has not been enabled for this field.
    if (!isset($noderef_settings['translate_and_reference'][$field_name])) {
      continue;
    }

    // Skip if this field does not exist in current node.
    if (!isset($node->{$field_name}) || !is_array($node->{$field_name})) {
      continue;
    }

    // Scan all items in this nodereference field.
    foreach ($node->{$field_name} as $delta => $reference) {
      // Skip if manual translation for this reference if not required.
      if (!isset($reference['translate_and_reference']) || empty($reference['translate_and_reference']['translation_source'])) {
        continue;
      }

      $reference_source = $reference['translate_and_reference']['translation_source'];

      // Generate a warning message for the user to note a translation for this
      // reference is missing.
      // Also, allow them to review the source reference on new window.
      $field_settings[$field_name]['missingTranslations'][$delta] = t('Missing translation for !title.', array(
        '!title' => l($reference_source->title .' [nid: '. $reference_source->nid .']', 'node/'. $reference_source->nid, array(
          'attributes' => array(
            'target' => '_blank',
            'title' => t('View @title [nid: @nid] in new window...', array(
              '@title' => $reference_source->title,
              '@nid' => $reference_source->nid,
            )),
          ),
        )),
      ));

      // Provide a link to "Translate and reference" when possible.
      if ($reference['translate_and_reference']['translation_supported']) {
        $language_list = language_list();
        $language = (isset($_GET['language']) && isset($language_list[$_GET['language']]) ? $_GET['language'] : $GLOBALS['language']->language);
        $field_settings[$field_name]['missingTranslations'][$delta] .= ' '. t('You can !translate now.', array(
          '!translate' => l(t('translate and reference'), 'noderelationships/create/'. $node->type .'/'. $field_name, array(
            'query' => 'translation='. $reference_source->nid .'&language='. $language,
            'attributes' => array(
              'class' => 'noderelationships-translate',
              'title' => t('Click here to translate and reference @title [nid: @nid] now.', array(
                '@title' => $reference_source->title,
                '@nid' => $reference_source->nid,
              )),
            ),
          )),
        ));
      }
      else {
        $field_settings[$field_name]['missingTranslations'][$delta] .= ' '. t('Translation of this reference is not supported.');
      }
    }
  }
}
