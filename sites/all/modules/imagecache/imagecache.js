/*global Drupal,jQuery */

(function($) {
  "use strict";

  Drupal.imagecache = {
    /**
     * Remove the files directory prefix from a path.
     */
    stripFileDirectory: function(path) {
      var filePath = Drupal.settings.imagecache.filesDirectory;
      if (path.substr(0, filePath.length + 1) === filePath + '/') {
        path = path.substr(filePath.length + 1);
      }
      return path;
    },

    createUrl: function(preset, path) {
      var stripped = this.stripFileDirectory(path);
      // If the preset is invalid, return the path to the original image.
      if ($.inArray(preset, Drupal.settings.imagecache.presets) !== -1) {
        return Drupal.settings.imagecache.filesUrl + '/imagecache/' + preset + '/' + stripped;
      }
      return Drupal.settings.imagecache.filesUrl + '/' + stripped;
    }
  };

  Drupal.theme.prototype.imagecache = function(preset, path, alt, title, attributes) {
    var image;
    image = new Image();

    image.onload = function() {
      $(this).attr({
        width: $(image).width(),
        height: $(image).height()
      });
    };

    image.src = Drupal.imagecache.createUrl(preset, path);
    image = $.extend(image, {
      title: title,
      alt: alt
    }, attributes);

    return image;
  };
})(jQuery);

/*jslint browser: true, onevar: true, undef: true */

