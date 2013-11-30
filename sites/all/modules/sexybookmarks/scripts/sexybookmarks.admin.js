/**
 * @file
 */

(function($) {
  Drupal.behaviors.sexyBookmarksAdmin = function(context) {
    // Drag'n'Drop sorting of SexyBookmarks services.
    $('#sexybookmarks-active, #sexybookmarks-inactive').sortable({
      connectWith: '.shr-socials',
      stop: function(event, ui) {
        services = Array();
        $('#sexybookmarks-active li').each(function() {
          services.push($(this).attr('id').substr(4));
        });
        $('#edit-config-service').val(services.join(','));
      }
    }).disableSelection();

    // Farbtastic color picker.
    $('#edit-config-tip-bg-color').parent().append('<div id="placeholder" style="float: right; position: relative; z-index: 10;"></div>');
    var farb = $.farbtastic('#placeholder');
    $('#edit-config-tip-bg-color, #edit-config-tip-text-color').each(function() {
      farb.linkTo(this);
      $(this).focus(function() {
        farb.linkTo(this);
      });
    });
  };
})(jQuery);
