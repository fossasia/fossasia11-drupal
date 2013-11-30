// $Id: node_form.js,v 1.1.2.14 2010/05/16 18:26:01 markuspetrux Exp $

(function ($) {

Drupal.nodeRelationshipsReferenceButtons = Drupal.nodeRelationshipsReferenceButtons || {};

/**
 * Add extra buttons node reference fields.
 */
Drupal.behaviors.nodeRelationshipsReferenceButtons = function(context) {
  var self = Drupal.nodeRelationshipsReferenceButtons;
  var settings = Drupal.settings.nodeRelationships;

  // If we are processing a behavior related to a search and reference request,
  // then we want to set the values of the node reference fields.
  // @see updateMultipleValues()
  if (self.isObject(self.selectedItems)) {
    var selectedItems = self.selectedItems;
    delete self.selectedItems;
  }

  // Process an individual nodereference autocomplete widget.
  $('.noderelationships-nodereference-autocomplete:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    var $nodereference = $(this);
    // Get field specific options.
    var fieldOptions = self.getFieldOptions($nodereference, settings);
    if (!fieldOptions || (!fieldOptions.searchUrl && !fieldOptions.createUrl && !fieldOptions.missingTranslations)) {
      // No extra nodereference options enabled, bye.
      return;
    }

    // Do we want to update the value of this node reference item?
    if (self.isObject(selectedItems) && self.isArray(selectedItems[fieldOptions.fieldName])) {
      var delta = self.getDelta($nodereference.attr('name'));
      if (selectedItems[fieldOptions.fieldName][delta]) {
        $nodereference.val(selectedItems[fieldOptions.fieldName][delta]);
      }
    }

    // Build a wrapper for the extra buttons.
    var $buttonsWrapper = $('<div class="noderelationships-nodereference-buttons-wrapper"/>');
    $nodereference.after($buttonsWrapper);

    // Install the "View in new window" button.
    if (fieldOptions.viewInNewWindow) {
      var $viewButton = $(Drupal.theme('nodeRelationshipsReferenceButton', 'view', Drupal.t('View in new window...')));
      $viewButton.attr('target', 'blank');
      $buttonsWrapper.append($viewButton);
      $nodereference.bind('change', function() {
        var nid = self.getNid($nodereference.val());
        $viewButton.attr('href', (nid > 0 ? settings.viewUrl.replace(/\/nid/, '/'+ nid) : 'javascript:void(0)'));
        if (nid > 0) {
          if ($viewButton.hasClass('noderelationships-nodereference-view-disabled')) {
            $viewButton.removeClass('noderelationships-nodereference-view-disabled').attr('title', Drupal.t('View in new window...'));
          }
        }
        else {
          if (!$viewButton.hasClass('noderelationships-nodereference-view-disabled')) {
            $viewButton.addClass('noderelationships-nodereference-view-disabled').attr('title', Drupal.t('View in new window... [disabled]'));
          }
        }
      }).bind('blur', function() {
        $nodereference.trigger('change');
      }).trigger('change');
      $viewButton.bind('click', function() {
        return (self.getNid($nodereference.val()) > 0);
      }).bind('focus', function() {
        $nodereference.trigger('change');
      });
    }

    // Install the "Edit reference" button.
    if (fieldOptions.editReference) {
      var $editButton = $(Drupal.theme('nodeRelationshipsReferenceButton', 'edit', Drupal.t('Edit reference...')));
      $buttonsWrapper.append($editButton);
      $nodereference.bind('change', function() {
        var nid = self.getNid($nodereference.val());
        if (nid > 0) {
          if ($editButton.hasClass('noderelationships-nodereference-edit-disabled')) {
            $editButton.removeClass('noderelationships-nodereference-edit-disabled').attr('title', Drupal.t('Edit reference...'));
          }
        }
        else {
          if (!$editButton.hasClass('noderelationships-nodereference-edit-disabled')) {
            $editButton.addClass('noderelationships-nodereference-edit-disabled').attr('title', Drupal.t('Edit reference... [disabled]'));
          }
        }
      }).bind('blur', function() {
        $nodereference.trigger('change');
      }).trigger('change');
      $editButton.bind('click', function() {
        var nid = self.getNid($nodereference.val());
        if (nid > 0) {
          var editUrl = settings.editUrl.replace(/\/nid/, '/'+ nid) +'?noderelationships_edit=1';
          self.loadFieldValues(fieldOptions, $nodereference, false);
          self.openDialog(editUrl, fieldOptions, $nodereference);
        }
        return false;
      });
    }

    // Install the "Search and reference" button.
    if (fieldOptions.searchUrl) {
      var $searchButton = $(Drupal.theme('nodeRelationshipsReferenceButton', 'search', Drupal.t('Search and reference...')));
      $buttonsWrapper.append($searchButton);
      $searchButton.bind('click', function() {
        self.loadFieldValues(fieldOptions, $nodereference, false);
        self.openDialog(fieldOptions.searchUrl, fieldOptions, $nodereference);
        return false;
      });

      // @todo: Show the multiple selection button for fields defined with
      // an arbitrary number of maximum values.
      // If we show this button, then we also need to code the way items are
      // updated in the parent window. Note we don't have add more button.

      // Build wrapper for add more button.
      if (fieldOptions.maxAllowedValues == 0 && self.isObject(fieldOptions.addMoreElement)) {
        var $addMoreElement = $(fieldOptions.addMoreElement);
        if (!$addMoreElement.hasClass('noderelationships-nodereference-add-more')) {
          var $multiButton = $(Drupal.theme('nodeRelationshipsReferenceButton', 'multi', Drupal.t('Search and reference multiple items at once...')));
          $addMoreElement.addClass('noderelationships-nodereference-add-more').after($multiButton);
          $multiButton.bind('click', function() {
            self.loadFieldValues(fieldOptions, $nodereference, true);
            self.openDialog(fieldOptions.searchUrl +'/multiselect', fieldOptions, $nodereference, $multiButton);
            return false;
          });
        }
      }
    }

    // Install the "Create and reference" button.
    if (fieldOptions.createUrl) {
      var $createButton = $(Drupal.theme('nodeRelationshipsReferenceButton', 'create', Drupal.t('Create and reference...')));
      $buttonsWrapper.append($createButton);
      $createButton.bind('click', function() {
        self.loadFieldValues(fieldOptions, $nodereference, false);
        self.openDialog(fieldOptions.createUrl, fieldOptions, $nodereference);
        return false;
      });
    }

    // Install the "Translate and reference" feature.
    if (fieldOptions.missingTranslations) {
      var delta = self.getDelta($nodereference.attr('name'));
      if (fieldOptions.missingTranslations[delta] && $nodereference.val() == '') {
        var $translationWarning = $(Drupal.theme('nodeRelationshipsTranslationWarning', fieldOptions.missingTranslations[delta]));
        $nodereference.before($translationWarning);
        $translationWarning.find('a.noderelationships-translate').bind('click', function() {
          var $link = $(this);
          self.loadFieldValues(fieldOptions, $nodereference, false);
          self.openDialog($link.attr('href'), fieldOptions, $nodereference);
          return false;
        });
        $nodereference.bind('translationWarning.change', function() {
          $translationWarning.hide('fast');
          $nodereference.unbind('translationWarning.change');
        });
      }
    }
  });
};

/**
 * Open the popup dialog.
 */
Drupal.nodeRelationshipsReferenceButtons.openDialog = function(url, fieldOptions, $nodereference, $multiButton) {
  var self = this;

  // onSubmit callback for modal frame dialogs.
  function onSubmitCallback(args, statusMessages) {
    if (args && typeof args.operation == 'string') {
      if (args.operation == 'updateSingleValue') {
        $nodereference.val(args.value);
        $nodereference.trigger('change');
      }
      else if (args.operation == 'updateMultipleValues') {
        self.updateMultipleValues(args.values, fieldOptions, $nodereference, $multiButton);
      }
    }
  }

  // Pass extra query string arguments in parent window to child windows.
  var queryString = self.queryString(), args = [];
  $.each(queryString, function(name, value) {
    args.push(name +'='+ value);
  });
  if (args.length > 0) {
    url += (url.indexOf('?') == -1 ? '?' : '&') + args.join('&');
  }

  // Build modal frame options.
  var modalOptions = {
    url: url,
    width: $(window).width() - 30,
    height: $(window).height() - 30,
    autoFit: true,
    onSubmit: onSubmitCallback
  };

  // Open the modal frame.
  Drupal.modalFrame.open(modalOptions);
};

/**
 * Operation to update a node reference widget with multiple values.
 *
 * Updating multiple values means triggering the AHAH request related to
 * the "Add more items" button, but we need to tell how many items we
 * need. Once we have the correct number of items in the form, we can update
 * them with the new values coming from the modal frame dialog.
 */
Drupal.nodeRelationshipsReferenceButtons.updateMultipleValues = function(selectedItems, fieldOptions, $nodereference, $multiButton) {
  var self = this;

  // Hide the multiple selection button while performing the AHAH request.
  $multiButton.hide();

  // Build the list of selected items. Note these values will be applied
  // when the AHAH request triggers Drupal behaviors on the new content.
  self.selectedItems = {};
  self.selectedItems[fieldOptions.fieldName] = [];
  for (var nid in selectedItems) {
    self.selectedItems[fieldOptions.fieldName].push(selectedItems[nid]);
  }
  var selectedItemsCount = self.selectedItems[fieldOptions.fieldName].length;

  // Perform the AHAH request to rebuild the items list.
  var addMoreSettings = Drupal.settings.ahah[fieldOptions.addMoreBase];
  addMoreSettings.url = fieldOptions.ahahSearchUrl +'/'+ selectedItemsCount;
  addMoreSettings.element = fieldOptions.addMoreElement;
  addMoreSettings.event = 'noderelationships.customClick';
  var ahah = new Drupal.ahah(fieldOptions.addMoreBase, addMoreSettings);
  ahah.oldSuccess = ahah.success
  ahah.success = function(response, status) {
    ahah.oldSuccess(response, status);
    $(addMoreSettings.element).unbind(addMoreSettings.event);
    $multiButton.show('fast');
    delete ahah;
  };
  ahah.oldError = ahah.error;
  ahah.error = function(response, uri) {
    ahah.oldError(response, uri);
    $(addMoreSettings.element).unbind(addMoreSettings.event);
    delete ahah;
  };
  $(addMoreSettings.element).trigger(addMoreSettings.event);
};

/**
 * Get field specific options.
 *
 * @param $element
 *   The jQuery element being processed.
 * @param settings
 *   The nodeRelationships settings object.
 *
 * The CCK field_name is located in the element class in the form
 * noderelationships[field_name].
 */
Drupal.nodeRelationshipsReferenceButtons.getFieldOptions = function($element, settings) {
  var self = this;
  var className = $element.attr('className');
  if (className) {
    var regExp = /^.*noderelationships\[(.*?)\].*$/;
    if (regExp.test(className)) {
      // Extract fielName from the element's class.
      var fieldName = className.replace(regExp, '$1');
      if (self.isObject(settings.fieldSettings[fieldName])) {
        // Build field options object.
        var fieldOptions = $.extend({fieldName: fieldName}, settings.fieldSettings[fieldName]);
        if (fieldOptions.maxAllowedValues != 1) {
          // Get information about multiple value widgets.
          var $addMoreButton = $('input[name='+ fieldOptions.fieldName +'_add_more]');
          if ($addMoreButton.size()) {
            var addMoreBase = $addMoreButton.attr('id');
            if (self.isObject(Drupal.settings.ahah[addMoreBase])) {
              fieldOptions.addMoreBase = addMoreBase;
              fieldOptions.addMoreElement = $addMoreButton[0];
            }
          }
        }
        return fieldOptions;
      }
    }
  }
  return false;
};

/**
 * Load list of field values from the form.
 */
Drupal.nodeRelationshipsReferenceButtons.loadFieldValues = function(fieldOptions, $nodereference, multiSelect) {
  var self = this;

  // Single selection mode.
  if (fieldOptions.maxAllowedValues == 1 || !multiSelect) {
    var value = $nodereference.val(), nid = self.getNid(value);
    self.currentFieldValues = {singleValue: (nid > 0 ? value : null)};
    return;
  }

  // Multiple selection mode.
  self.currentFieldValues = [];
  $nodereference.parents('.content-multiple-table:first').find('tbody tr').each(function() {
    var $row = $(this), $field = $('.form-autocomplete', $row);
    if ($field.size() == 1 && self.getDelta($field.attr('name')) >= 0) {
      // Ignore items that are flagged for removal (support for CCK 6.x-3.x).
      if ($('.content-multiple-remove-cell .content-multiple-remove-checkbox', $row).is(':checked')) {
        return;
      }
      // Take the value only if we can extract a nid.
      var value = $field.val(), nid = self.getNid(value);
      if (nid > 0) {
        self.currentFieldValues.push({nid: nid, value: value});
      }
    }
  });
};

/**
 * Extract the delta from the given element name.
 */
Drupal.nodeRelationshipsReferenceButtons.getDelta = function(elementName) {
  var delta = elementName.replace(new RegExp('^[-_a-z0-9]+\\[([0-9]+)\\]\\[nid\\]\\[nid\\]$'), '$1');
  return (/[0-9]+/.test(delta) ? parseInt(delta) : -1);
};

/**
 * Extract the nid from the given nodereference value.
 */
Drupal.nodeRelationshipsReferenceButtons.getNid = function(value) {
  var regExp = /^.*\[\s*nid\s*:\s*([0-9]+)\s*\]\s*$/;
  if (!regExp.test(value)) {
    return -1;
  }
  var nid = value.replace(regExp, '$1');
  return (nid.length > 0 ? parseInt(nid) : -1);
};

/**
 * Check if the given variable is an object.
 */
Drupal.nodeRelationshipsReferenceButtons.isObject = function(something) {
  return (something !== null && typeof something === 'object');
};

/**
 * Check if the given variable is an array.
 */
Drupal.nodeRelationshipsReferenceButtons.isArray = function(something) {
  var self = this;
  return (self.isObject(something) && something.length);
};

/**
 * Parse the query string of the current window.
 */
Drupal.nodeRelationshipsReferenceButtons.queryString = function() {
  var qs = {}, excludeArgs = ['q', 'destination', 'pass', 'translation', 'language'];
  if (window.location.search && window.location.search.length) {
    // Remove leading question mark and splits the string into query arguments.
    var q = window.location.search.substring(1).split('&');
    for (var i = 0; i < q.length; i++) {
      var pair = q[i].split('=');
      if (pair.length == 2) {
        var name = decodeURIComponent(pair[0]);
        if ($.inArray(name, excludeArgs) == -1) {
          qs[name] = decodeURIComponent(pair[1]);
        }
      }
    }
  }
  return qs;
};

/**
 * Theme the specified button for an autocomplete widget.
 */
Drupal.theme.prototype.nodeRelationshipsReferenceButton = function(type, title) {
  return '<a href="javascript:void(0)" class="noderelationships-nodereference-'+ type +'-button" title="'+ title +'"></a>';
};

/**
 * Theme the specified button for an autocomplete widget.
 */
Drupal.theme.prototype.nodeRelationshipsTranslationWarning = function(message) {
  return '<div class="warning"><span class="warning">*</span> '+ message +'</div>';
};

})(jQuery);
