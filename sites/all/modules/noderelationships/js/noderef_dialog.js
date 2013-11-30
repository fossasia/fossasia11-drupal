// $Id: noderef_dialog.js,v 1.1.2.7 2009/09/28 07:56:42 markuspetrux Exp $

(function ($) {

/**
 * Private namespace.
 */
Drupal.nodeRelationshipsReferenceDialog = Drupal.nodeRelationshipsReferenceDialog || {};

/**
 * Modal Frame behavior.
 */
Drupal.modalFrameChild = Drupal.modalFrameChild || {};
Drupal.modalFrameChild.behaviors = Drupal.modalFrameChild.behaviors || {};
Drupal.modalFrameChild.behaviors.nodeRelationshipsReferenceDialog = function(context) {
  $('.noderelationships-noderef-page-content:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    var contentWrapper = this;
    // Drive the focus to the first tabbable element in the active content area (using separate thread).
    setTimeout(function() { $(':tabbable:first', contentWrapper).focus(); }, 1000);
  });
};

/**
 * Drupal behavior.
 */
Drupal.behaviors.nodeRelationshipsReferenceDialog = function(context) {
  var self = Drupal.nodeRelationshipsReferenceDialog;
  self.settings = Drupal.settings.nodeRelationships;

  // Make sure we can reach the parent window.
  if (!self.isObject(parent.Drupal) || !self.isObject(parent.Drupal.nodeRelationshipsReferenceButtons)) {
    $('.noderelationships-noderef-multiselect:visible', context).hide();
    return;
  }

  // Find and parse view using table style.
  $('table.views-table:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    self.initializeTable(this);
  });

  // Find and parse view using grid style.
  $('table.views-view-grid:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    self.initializeGrid(this, 'td');
  });

  // Find and parse view using fluid grid style.
  $('div.views-fluid-grid:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    self.initializeGrid(this, 'li');
  });

  // Initialize selected values from parent window.
  if ($('.noderelationships-noderef-singleselect').size()) {
    // Single selection: reflect currently selected item in screen.
    $('.noderelationships-noderef-singleselect span').html(self.getSingleValue());
  }
  else {
    // Multiple selection: setup widget.
    $('.noderelationships-noderef-multiselect:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
      self.setupMultiselectWidget(context);
    });
  }
};

/**
 * Initialize a view using the table style.
 */
Drupal.nodeRelationshipsReferenceDialog.initializeTable = function(table) {
  var self = this;
  $('td.views-field-noderelationships-nid', table).each(function() {
    var $nidCell = $(this), $row = $nidCell.parents('tr:first'), $titleCell = $('td.views-field-noderelationships-title', $row);

    // Try to extract the nid and title from the corresponding cells.
    var nid = $.trim($nidCell.text()), title = $.trim($titleCell.text());
    if (nid.length <= 0 || !/^[0-9]+$/.test(nid) || title.length <= 0) {
      return;
    }

    // Attach hover and click events to the cell.
    var $rowCells = $row.find('td');
    self.makeSelectable(nid, title, $row, $([]).add($row).add($rowCells));
  });
};

/**
 * Initialize a view using the grid style.
 */
Drupal.nodeRelationshipsReferenceDialog.initializeGrid = function(context, parentType) {
  var self = this;
  $('.views-field-noderelationships-nid', context).each(function() {
    var $cell = $(this).parents(parentType +':first');

    // Try to extract the nid and title for the current cell.
    var nid = $cell.find('.views-field-noderelationships-nid .field-content').text();
    var title = $cell.find('.views-field-noderelationships-title .field-content').text();

    // Attach hover and click events to the cell.
    self.makeSelectable(nid, title, $cell, $cell);
  });
};

/**
 * Make a views item selectable.
 */
Drupal.nodeRelationshipsReferenceDialog.makeSelectable = function(nid, title, $viewItem, $highlightItems) {
  var self = this;

  // Append a selector for making it easy to highlight element for hover
  // and item selection states.
  $highlightItems.addClass('noderelationships-highlight-nid-'+ nid);

  // Highlight views item if it is already selected.
  if ($('li.noderelationships-item-'+ nid).size() && !$highlightItems.hasClass('noderelationships-noderef-selected')) {
    $highlightItems.addClass('noderelationships-noderef-selected');
  }

  // Make a views item selectable.
  $viewItem.attr('title', Drupal.t('Select: @value', {'@value': self.buildValue(nid, title)}))
    .addClass('noderelationships-noderef-selectable-nid-'+ nid)
    .addClass('noderelationships-noderef-selectable')
    .hover(
      function() {
        $highlightItems.addClass('noderelationships-noderef-hover');
      },
      function() {
        $highlightItems.removeClass('noderelationships-noderef-hover');
      }
    ).bind('click', function(event) {
      // Ignore the click event if the target element is a link.
      if (event.target && event.target.nodeName && event.target.nodeName.toLowerCase() !== 'a') {
        if (!self.isSelectionLocked()) {
          self.selectNode(nid, title, $viewItem);
        }
        return false;
      }
    });
};

/**
 * Select a node.
 */
Drupal.nodeRelationshipsReferenceDialog.selectNode = function(nid, title, $viewItem) {
  var self = this;
  var value = self.buildValue(nid, title);

  // See if we are operating in single selection mode.
  if ($('.noderelationships-noderef-singleselect').size()) {
    $viewItem.effect('transfer', {to: '.noderelationships-noderef-singleselect span', className: 'noderelationships-effects-transfer'}, 'fast', function() {
      $('.noderelationships-noderef-singleselect span').html(value);
      setTimeout(function() { parent.Drupal.modalFrame.close({operation: 'updateSingleValue', value: value}); }, 100);
    });
    return;
  }

  // Check maximum number of allowed values.
  if (self.settings.maxAllowedValues > 0 && self.settings.maxAllowedValues >= self.getSelectedItemsCount()) {
    alert(Drupal.t('Sorry, you can only select @max values maximum.', {'@max': self.settings.maxAllowedValues}));
    return;
  }

  // Multiple selection mode.
  var $selectedItem = $('li.noderelationships-item-'+ nid);
  if ($selectedItem.size()) {
    if ($('.noderelationships-noderef-multiselect-items:visible').size()) {
      if (self.isScrollableElementVisible($selectedItem, true)) {
        self.removeItem(nid, $selectedItem);
      }
      else {
        self.removeItem(nid, $('.noderelationships-noderef-multiselect-items label'));
      }
    }
    else {
      self.removeItem(nid, $('.noderelationships-noderef-multiselect legend a'));
    }
  }
  else {
    self.appendItem(nid, value);
  }
};

/**
 * Make the given item resizable (based on misc/textarea.js).
 */
Drupal.nodeRelationshipsReferenceDialog.makeResizable = function($element) {
  var staticOffset = null;

  // When wrapping the text area, work around an IE margin bug.  See:
  // http://jaspan.com/ie-inherited-margin-bug-form-elements-and-haslayout
  $element.wrap('<div class="resizable-textarea"><span></span></div>')
    .parent().append($('<div class="grippie"></div>').mousedown(startDrag));

  var grippie = $('div.grippie', $element.parent())[0];
  grippie.style.marginRight = (grippie.offsetWidth - $element[0].offsetWidth) +'px';

  function startDrag(e) {
    staticOffset = $element.height() - e.pageY;
    $element.css('opacity', 0.25);
    $(document).mousemove(performDrag).mouseup(endDrag);
    return false;
  }

  function performDrag(e) {
    $element.height(Math.max(60, staticOffset + e.pageY) + 'px');
    return false;
  }

  function endDrag(e) {
    $(document).unbind('mousemove', performDrag).unbind('mouseup', endDrag);
    $element.css('opacity', 1);
  }
};

/**
 * Get the value of a single node reference.
 */
Drupal.nodeRelationshipsReferenceDialog.getSingleValue = function() {
  var self = this;
  var currentFieldValues = parent.Drupal.nodeRelationshipsReferenceButtons.currentFieldValues;
  if (currentFieldValues.singleValue) {
    return currentFieldValues.singleValue;
  }
  return Drupal.t('none');
};

/**
 * Initialize multiple selection widget.
 */
Drupal.nodeRelationshipsReferenceDialog.setupMultiselectWidget = function(context) {
  var self = this;

  // Make the item list item resizable and sortable.
  $('.noderelationships-noderef-multiselect-items-list ul:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    var $itemsList = $(this);
    self.makeResizable($itemsList);
    $itemsList.sortable(self.sortableOptions);
  });

  // Load widget with values from parent window.
  self.loadMultipleItems();

  // Attach button behaviors.
  $('.noderelationships-noderef-multiselect-save:not(.noderelationships-processed)', context).addClass('noderelationships-processed').bind('click', function() {
    parent.Drupal.modalFrame.close({operation: 'updateMultipleValues', values: self.getSelectedItemsHash()});
    return false;
  });

  $('.noderelationships-noderef-multiselect-sort-desc:not(.noderelationships-processed)', context).addClass('noderelationships-processed').bind('click', function() {
    if (!$(this).hasClass('noderelationships-disabled')) {
      self.sortMultipleItems('DESC');
      self.enableButton('sort-asc');
      self.disableButton('sort-desc');
      self.enableButton('reset');
    }
    return false;
  });
  $('.noderelationships-noderef-multiselect-sort-asc:not(.noderelationships-processed)', context).addClass('noderelationships-processed').bind('click', function() {
    if (!$(this).hasClass('noderelationships-disabled')) {
      self.sortMultipleItems('ASC');
      self.disableButton('sort-asc');
      self.enableButton('sort-desc');
      self.enableButton('reset');
    }
    return false;
  });

  $('.noderelationships-noderef-multiselect-reset:not(.noderelationships-processed)', context).addClass('noderelationships-processed').bind('click', function() {
    if (!$(this).hasClass('noderelationships-disabled')) {
      self.loadMultipleItems();
      self.enableButton('sort-asc');
      self.enableButton('sort-desc');
      self.disableButton('reset');
    }
    return false;
  });
  self.disableButton('reset');

  $('.noderelationships-noderef-multiselect-cancel:not(.noderelationships-processed)', context).addClass('noderelationships-processed').bind('click', function() {
    parent.Drupal.modalFrame.close();
    return false;
  });
};

/**
 * jQuery UI Sortable options.
 *
 * @see setupMultiselectWidget()
 */
Drupal.nodeRelationshipsReferenceDialog.sortableOptions = {
  stop: function(event, ui) {
    // Make sure the action buttons are enabled after a drag'n'drop action.
    var self = Drupal.nodeRelationshipsReferenceDialog;
    self.enableButton('sort-asc');
    self.enableButton('sort-desc');
    self.enableButton('reset');
  },
  placeholder: 'ui-state-highlight',
  forcePlaceholderSize: true,
  items: 'li',
  cancel: 'a',
  containment: '.noderelationships-noderef-multiselect',
  axis: 'y',
  appendTo: 'body',
  helper: function(event, $element) {
    var $helper = $('<div class="noderelationships-noderef-multiselect-ui-sortable-helper">'+ $element.text() +'</div>');
    $helper.width($element.find('div').width());
    $helper.css('left', $element.offset().left +'px');
    return $helper;
  }
};

/**
 * Load widget with values from parent window.
 */
Drupal.nodeRelationshipsReferenceDialog.loadMultipleItems = function() {
  var self = this;
  var currentFieldValues = parent.Drupal.nodeRelationshipsReferenceButtons.currentFieldValues;
  self.lockSelection(currentFieldValues.length > 20 ? 1000 : 500);

  if ($('.noderelationships-noderef-multiselect-items-list ul li').size()) {
    $('.noderelationships-noderef-multiselect-items-list ul li').remove();
    $('.noderelationships-noderef-selected').removeClass('noderelationships-noderef-selected');
  }

  for (var i = 0; i < currentFieldValues.length; i++) {
    var nid = currentFieldValues[i].nid;
    self.createItem(nid, currentFieldValues[i].value, false);
    $('.noderelationships-highlight-nid-'+ nid).addClass('noderelationships-noderef-selected');
  }

  // Use a separate thread to update the selected items counter because
  // the current thread is busy processing Drupal behaviors.
  setTimeout(function() { self.updateSelectedItemsCount(); }, 1);
};

/**
 * Sort selected items alphabetically.
 */
Drupal.nodeRelationshipsReferenceDialog.sortMultipleItems = function(sortDirection) {
  var self = this;
  var selectedItems = self.getSelectedItemsArray();
  self.lockSelection(selectedItems.length > 20 ? 1000 : 500);
  selectedItems.sort();
  if (sortDirection == 'ASC') {
    selectedItems.reverse();
  }

  if ($('.noderelationships-noderef-multiselect-items-list ul li').size()) {
    $('.noderelationships-noderef-multiselect-items-list ul li').remove();
    $('.noderelationships-noderef-selected').removeClass('noderelationships-noderef-selected');
  }

  for (var i = 0; i < selectedItems.length; i++) {
    var value = selectedItems[i];
    var nid = parent.Drupal.nodeRelationshipsReferenceButtons.getNid(value);
    self.createItem(nid, value, false);
    $('.noderelationships-highlight-nid-'+ nid).addClass('noderelationships-noderef-selected');
  }

  // Use a separate thread to update the selected items counter because
  // the current thread is busy processing Drupal behaviors.
  setTimeout(function() { self.updateSelectedItemsCount(); }, 1);
};

/**
 * Create a new item for the selection widget.
 */
Drupal.nodeRelationshipsReferenceDialog.createItem = function(nid, value, animate) {
  var self = this;
  var $newItem = $(Drupal.theme('nodeRelationshipsSelectedItem', nid, value));
  if (animate) {
    $newItem.hide();
  }
  $newItem.find('a').bind('click', function(event) {
    self.removeItem(nid, $newItem);
    return false;
  });
  $('.noderelationships-noderef-multiselect-items-list ul').append($newItem);
  if (animate) {
    $newItem.fadeIn('fast');
  }
};

/**
 * Append a new item to the selection widget.
 */
Drupal.nodeRelationshipsReferenceDialog.appendItem = function(nid, value) {
  var self = this;
  var $viewItem = $('.noderelationships-noderef-selectable-nid-'+ nid);
  if ($viewItem.size()) {
    $('.noderelationships-highlight-nid-'+ nid).addClass('noderelationships-noderef-selected');
    var $tranferElement = ($('.noderelationships-noderef-multiselect-items label').is(':visible') ? $('.noderelationships-noderef-multiselect-items label') : $('.noderelationships-noderef-multiselect legend a'));
    $viewItem.effect('transfer', {to: $tranferElement, className: 'noderelationships-effects-transfer'}, 'fast', function() {
      self.createItem(nid, value, true);
      self.updateSelectedItemsCount();
    });
  }
  else {
    self.createItem(nid, value, true);
    self.updateSelectedItemsCount();
  }
  // Make sure the action buttons are enabled after adding a new item.
  self.enableButton('sort-asc');
  self.enableButton('sort-desc');
  self.enableButton('reset');
};

/**
 * Remove an item from the selection widget.
 */
Drupal.nodeRelationshipsReferenceDialog.removeItem = function(nid, $tranferElement) {
  var self = this;
  var $selectedItem = $('li.noderelationships-item-'+ nid);
  $selectedItem.fadeOut('fast', function() {
    var $viewItem = $('.noderelationships-noderef-selectable-nid-'+ nid);
    if ($viewItem.size()) {
      $selectedItem.css({visibility: 'hidden', display: 'block'});
      $tranferElement.effect('transfer', {to: $viewItem, className: 'noderelationships-effects-transfer'}, 'fast', function() {
        self.updateSelectedItemsCount();
        $('.noderelationships-highlight-nid-'+ nid).removeClass('noderelationships-noderef-selected');
      });
    }
    else {
      self.updateSelectedItemsCount();
      $('.noderelationships-highlight-nid-'+ nid).removeClass('noderelationships-noderef-selected');
    }
    $selectedItem.remove();
  });
  // Make sure the Reset button is enabled after removing an item.
  self.enableButton('reset');
};

/**
 * Update the visible counter of selected items.
 */
Drupal.nodeRelationshipsReferenceDialog.updateSelectedItemsCount = function() {
  var self = this;
  var $legend = $('.noderelationships-noderef-multiselect legend a');
  var legendText = $legend.text(), regExp = /^(.*)\([0-9]+\)$/, count = self.getSelectedItemsCount();
  if (regExp.test(legendText)) {
    $legend.html(legendText.replace(regExp, '$1('+ count +')'));
  }
  else {
    $legend.html(legendText +' ('+ count +')');
  }
  if (count == 0) {
    self.disableButton('sort-asc');
    self.disableButton('sort-desc');
  }
};

/**
 * Get the number of selected items.
 */
Drupal.nodeRelationshipsReferenceDialog.getSelectedItemsCount = function() {
  return $('.noderelationships-noderef-multiselect-items-list ul li').size();
};

/**
 * Get a collection of all selected items.
 */
Drupal.nodeRelationshipsReferenceDialog.getSelectedItemsHash = function() {
  var selectedItems = {};
  $('.noderelationships-noderef-multiselect-items-list ul li').each(function() {
    var value = $(this).text();
    var nid = parent.Drupal.nodeRelationshipsReferenceButtons.getNid(value);
    selectedItems[nid] = value;
  });
  return selectedItems;
};

/**
 * Get an array of all selected items.
 */
Drupal.nodeRelationshipsReferenceDialog.getSelectedItemsArray = function() {
  var selectedItems = [];
  $('.noderelationships-noderef-multiselect-items-list ul li').each(function() {
    selectedItems.push($(this).text());
  });
  return selectedItems;
};

/**
 * Build the text for a nodereference value.
 */
Drupal.nodeRelationshipsReferenceDialog.buildValue = function(nid, title) {
  return title +' [nid:'+ nid +']';
};

/**
 * Enable the specified button.
 */
Drupal.nodeRelationshipsReferenceDialog.enableButton = function(button) {
  $('.noderelationships-noderef-multiselect-'+ button).removeClass('noderelationships-disabled').removeClass('noderelationships-noderef-multiselect-'+ button +'-disabled');
};

/**
 * Disable the specified button.
 */
Drupal.nodeRelationshipsReferenceDialog.disableButton = function(button) {
  $('.noderelationships-noderef-multiselect-'+ button).addClass('noderelationships-disabled').addClass('noderelationships-noderef-multiselect-'+ button +'-disabled');
};

/**
 * Lock item selection temporarily.
 */
Drupal.nodeRelationshipsReferenceDialog.lockSelection = function(interval) {
  var self = this;
  self._selectionLocked = true;
  setTimeout(function() { delete self._selectionLocked; }, (interval || 500));
};

/**
 * Check if item selection is locked.
 */
Drupal.nodeRelationshipsReferenceDialog.isSelectionLocked = function() {
  var self = this;
  if (self._selectionLocked) {
    return true;
  }
  self.lockSelection();
  return false;
};

/**
 * Check if the given scrollable element is visible.
 */
Drupal.nodeRelationshipsReferenceDialog.isScrollableElementVisible = function($element, checkParent) {
  var $parent = (checkParent ? $element.parent() : $(window));
  var parentTop = $parent.scrollTop();
  var parentBottom = parentTop + $parent.height();
  var elementTop = (checkParent ? ($element.offset().top - $parent.offset().top) : $element.offset().top);
  var elementBottom = elementTop + $element.height();
  return ((elementBottom >= parentTop) && (elementTop <= parentBottom));
};

/**
 * Check if the given variable is an object.
 */
Drupal.nodeRelationshipsReferenceDialog.isObject = function(something) {
  return (something !== null && typeof something === 'object');
};

/**
 * Theme an item for the multiple selection widget.
 */
Drupal.theme.prototype.nodeRelationshipsSelectedItem = function(nid, value) {
  var unselect = '<a href="javascript:void(0)" class="noderelationships-noderef-multiselect-unselect" title="'+ Drupal.t('Unselect') +'"></a>';
  return '<li class="noderelationships-item noderelationships-item-'+ nid +'"><div>'+ unselect +'<span>'+ value +'</span></div></li>';
};

})(jQuery);
