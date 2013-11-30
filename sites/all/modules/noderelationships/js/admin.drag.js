// $Id: admin.drag.js,v 1.1.2.4 2009/09/20 17:59:03 markuspetrux Exp $

(function ($) {

/**
 * Drag'n'drop interface for back reference regions.
 *
 * Inspired by modules/block/block.js
 */
Drupal.behaviors.nodeRelationshipsDrag = function(context) {
  var $table = $('table#noderelationships-settings-table', context);
  var tableDrag = Drupal.tableDrag['noderelationships-settings-table'];

  // Make sure this behavior is not executed more than once.
  if ($table.hasClass('noderelationships-processed')) {
    return;
  }
  $table.addClass('noderelationships-processed');

  // Append the disable button to all rows.
  $('tr', $table).each(function() {
    var $row = $(this);
    if ($('th', $row).size()) {
      $row.append($('<th class="noderelationships-cell-center">'+ Drupal.t('Disable') +'</th>'));
    }
    else {
      if ($row.hasClass('region') || $row.hasClass('region-message')) {
        var $cell = $('td:first', $row);
        $cell.attr('colspan', $cell.attr('colspan') + 1);
      }
      else {
        var $select = $('select.noderelationships-region-select', $row);
        var $disableButton = $(Drupal.theme('nodeRelationshipsDisableButton'));
        $disableButton.bind('click', function() {
          $select.val('none').trigger('change');
          return false;
        });
        $row.append($disableButton);
      }
    }
  });

  // Add a handler for when a row is swapped, update empty regions.
  tableDrag.row.prototype.onSwap = function(swappedRow) {
    checkEmptyRegions($table, this);
  };

  // Add a validation handler to enforce additional swapping conditions.
  tableDrag.row.prototype._isValidSwap = tableDrag.row.prototype.isValidSwap;
  tableDrag.row.prototype.isValidSwap = function(row) {
    if (!this._isValidSwap(row)) {
      return false;
    }
    // Back reference fields cannot be rearranged. Use "Manage fields" page instead.
    if ($(this.element).hasClass('noderelationships-region-field') && $(row).hasClass('noderelationships-region-field')) {
      return false;
    }
    return true;
  };

  // A custom message for the node relationships settings page specifically.
  Drupal.theme.tableDragChangedWarning = function () {
    return '<div class="warning">' + Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t("Changes will not be saved and Back reference fields will not be created until the <em>Save settings</em> button is clicked.") + '</div>';
  };

  // Add a handler so when a row is dropped, update fields dropped into new regions.
  tableDrag.onDrop = function() {
    var rowObject = this.rowObject;
    if ($(rowObject.element).prev('tr').is('.region-message')) {
      var regionRow = $(rowObject.element).prev('tr').get(0);
      var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
      var regionField = $('select.noderelationships-region-select', rowObject.element);
      var weightField = $('input.noderelationships-weight', rowObject.element);
      var oldRegionName = weightField[0].className.replace(/([^ ]+[ ]+)*noderelationships-weight-([^ ]+)([ ]+[^ ]+)*/, '$2');

      if (!regionField.is('.noderelationships-region-'+ regionName)) {
        regionField.removeClass('noderelationships-region-' + oldRegionName).addClass('noderelationships-region-' + regionName);
        weightField.removeClass('noderelationships-weight-' + oldRegionName).addClass('noderelationships-weight-' + regionName);
        regionField.val(regionName);
      }
    }
  };

  // Add the behavior to each region select list.
  $('select.noderelationships-region-select:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    var $select = $(this);
    $select.change(function(event) {
      // Clone the row, empty cloned cells, append clone after source row, and hide source row.
      var $sourceRow = $select.parents('tr:first');
      var $clonedRow = $sourceRow.clone();
      $clonedRow.find('td').html('').css('padding', '0');
      $sourceRow.find('td:visible').addClass('noderelationships-cell-transfer');
      $sourceRow.after($clonedRow);
      // Get the target region.
      var targetRegion = $select.val();

      // Find the correct region and insert the row as the first in the region.
      $('tr.region-message', $table).each(function() {
        if ($(this).is('.region-' + targetRegion + '-message')) {
          // Move the source row.
          $(this).after($sourceRow);

          // Proceed with transfer effect.
          $clonedRow.effect('transfer', {to: $sourceRow, className: 'noderelationships-effects-transfer'}, 'normal', function() {
            // Remove the cloned row.
            $clonedRow.remove();

            // Manually update weights and restripe.
            tableDrag.rowObject = new tableDrag.row($sourceRow);
            tableDrag.updateFields($sourceRow.get(0));
            tableDrag.rowObject.changed = true;
            if (tableDrag.oldRowElement) {
              $(tableDrag.oldRowElement).removeClass('drag-previous');
            }
            tableDrag.oldRowElement = $sourceRow;
            tableDrag.restripeTable();
            tableDrag.rowObject.markChanged();
            $sourceRow.addClass('drag-previous');

            // Modify empty regions with added or removed fields.
            checkEmptyRegions($table, $sourceRow);

            // Assign the region class to the row.
            var oldRegionName = $sourceRow.get(0).className.replace(/([^ ]+[ ]+)*noderelationships-region-([^ ]+)([ ]+[^ ]+)*/, '$2');
            $sourceRow.removeClass('noderelationships-region-' + oldRegionName).addClass('noderelationships-region-' + targetRegion);

            // Show the row contents.
            $sourceRow.find('td.noderelationships-cell-transfer').removeClass('noderelationships-cell-transfer');

            // Keep the focus on the selectbox.
            $select.focus();
          });
        }
      });
    });
  });

  var checkEmptyRegions = function($table, rowObject) {
    $('tr.region-message', $table).each(function() {
      // If the dragged row is in this region, but above the message row, swap it down one space.
      if ($(this).prev('tr').get(0) == rowObject.element) {
        // Prevent a recursion problem when using the keyboard to move rows up.
        if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
          rowObject.swap('after', this);
        }
      }
      // This region has become empty.
      if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) {
        $(this).removeClass('region-populated').addClass('region-empty');
      }
      // This region has become populated.
      else if ($(this).is('.region-empty')) {
        $(this).removeClass('region-empty').addClass('region-populated');
      }
    });

    // Assign the region class to the rowObject element.
    var rowFound = false, regionName = '';
    $('tr', $table).each(function() {
      if (!rowFound) {
        if ($(this).is('.region-message')) {
          regionName = this.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
        }
        else if (this == rowObject.element) {
          var oldRegionName = this.className.replace(/([^ ]+[ ]+)*noderelationships-region-([^ ]+)([ ]+[^ ]+)*/, '$2');
          $(rowObject.element).removeClass('noderelationships-region-' + oldRegionName).addClass('noderelationships-region-' + regionName);
          rowFound = true;
        }
      }
    });
  };
};

Drupal.theme.prototype.nodeRelationshipsDisableButton = function () {
  return '<td class="noderelationships-cell-center"><a href="javascript:void(0)" class="noderelationships-disable-button" title="'+ Drupal.t('Click here to disable this item') +'"></a></td>';
};

})(jQuery);
