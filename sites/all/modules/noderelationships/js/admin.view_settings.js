// $Id: admin.view_settings.js,v 1.1.2.5 2009/09/20 17:59:03 markuspetrux Exp $

(function ($) {

/**
 * Add a button to link to view settings.
 */
Drupal.behaviors.nodeRelationshipsViewSettings = function(context) {
  $('table#noderelationships-settings-table:not(.noderelationships-view-settings-processed)', context).addClass('noderelationships-view-settings-processed').each(function() {
    var settings = Drupal.settings.nodeRelationships;
    $('td.noderelationships-cell-options .form-select', this).each(function() {
      var $select = $(this);
      var $button = $(Drupal.theme('nodeRelationshipsViewSettingsButton'));
      $select.after($button);
      $select.bind('change', function() {
        var value = $select.val();
        if (settings.mode == 'noderef') {
          $button.css('visibility', (value.length > 0 ? 'visible' : 'hidden'));
        }
        if (settings.mode == 'backref' || value.length > 0) {
          $button.attr('href', settings.viewSettingsUrl +'/'+ (value.length > 0 ? value.replace(/:.*$/, '') : settings.defaultViewName));
        }
      }).trigger('change');
    });
  });
};

/**
 * Theme the edit view settings button.
 */
Drupal.theme.prototype.nodeRelationshipsViewSettingsButton = function() {
  return '<a href="#" class="noderelationships-view-settings-button" target="_blank" title="'+ Drupal.t('Edit view settings in new window...') +'"></a>';
};

})(jQuery);
