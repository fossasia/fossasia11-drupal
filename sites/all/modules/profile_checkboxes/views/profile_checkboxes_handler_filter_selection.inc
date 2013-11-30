<?php

/**
 * @file
 * Custom views handler for profile_checkboxes.views.inc.
 */

class profile_checkboxes_handler_filter_selection extends views_handler_filter_profile_selection {
  function op_simple() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    // Group type is different for different operators.  
    $this->options['group'] = $this->query->set_where_group(($this->operator == 'in' ? 'OR' : 'AND'));

    // Add clause for every option value
    foreach ($this->value as $value) {
      $placeholder = is_numeric($value) ? ' \'%%:%d;%%\'' : ' \'%%:\"%s\";%%\'';
      $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . ($this->operator == 'in' ? 'LIKE' : 'NOT LIKE' ) . $placeholder, $value);
    }
  }
}