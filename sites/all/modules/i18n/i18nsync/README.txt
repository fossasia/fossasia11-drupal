
README.txt
==========
Drupal module: i18nsync (Synchronization)

This module will handle content synchronization accross translations.

The available list of fields to synchronize will include standard node fields and cck fields.
To have aditional fields, add the list in a variable in the settings.php file, like this:

// Available fields for synchronization, for all node types.
$conf['i18nsync_fields_node'] = array(
  'field1' => t('Field 1 name'),
  'field2' => t('Field 2 name'),
  ...
);

// More fields for a specific content type 'nodetype' only.
$conf['i18nsync_fields_node_nodetype'] = array(
  'field3' => t('Field 3 name'),
   ...
);