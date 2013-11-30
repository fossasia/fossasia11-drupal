<?php

/**
 * @file
 * Custom views handler for profile_checkboxes.views.inc.
 */

class profile_checkboxes_handler_field_unserialize_selection extends views_handler_field {
  function render($values) {
    if ($value = unserialize($values->{$this->field_alias})) {
      return implode($this->definition['delimiter'], $value);
    }
    else {
      return parent::render($values);
    }
  }
}