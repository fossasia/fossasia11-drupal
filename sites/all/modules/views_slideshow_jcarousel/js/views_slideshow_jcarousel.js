(function ($) {
  /**
   * Views Slideshow jCarousel Pager
   */

  // Add views slieshow api calls for views slideshow jCarousel pager.
  Drupal.behaviors.viewsSlideshowJcarouselPager = function (context) {
    // Process pause on hover.
    $('.views_slideshow_jcarousel_pager:not(.views-slideshow-jcarousel-pager-processed)', context).addClass('views-slideshow-jcarousel-pager-processed').each(function() {
      // Parse out the unique id from the full id.
      var pagerInfo = $(this).attr('id').split('_');
      var location = pagerInfo[2];
      pagerInfo.splice(0, 3);
      var uniqueID = pagerInfo.join('_');

      $(this).jcarousel({
        vertical: parseInt(Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location].orientation),
        scroll: parseInt(Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location].scroll),
        visible: parseInt(Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location].visible),
        wrap: Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location].wrap,
        animation: (isNaN(Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location].animation)) ? Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location].animation : parseInt(Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location].animation),
        initCallback: function(carousel) {
          Drupal.settings.viewsSlideshowJCarouselPager[uniqueID][location]['carouselObj'] = carousel;
        }
      });

      $(this).find('.views_slideshow_jcarousel_pager_item').each(function(index, pagerItem) {
        $(pagerItem).click(function() {
          Drupal.viewsSlideshow.action({ "action": 'goToSlide', "slideshowID": uniqueID, "slideNum": index });
        });
      });
    });
  };

  Drupal.viewsSlideshowJcarouselPager = Drupal.viewsSlideshowJcarouselPager || {};

  /**
   * Implement the transitionBegin hook for pager jcarousel pager.
   */
  Drupal.viewsSlideshowJcarouselPager.transitionBegin = function (options) {
    for(pagerLocation in Drupal.settings.viewsSlideshowPager[options.slideshowID]) {
      if (Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation].moveOnChange) {
        Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation]['carouselObj'].scroll(options.slideNum);
      }
    }
  };

  /**
   * Implement the goToSlide hook for pager jcarousel pager.
   */
  Drupal.viewsSlideshowJcarouselPager.goToSlide = function (options) {
    for(pagerLocation in Drupal.settings.viewsSlideshowPager[options.slideshowID]) {
      if (Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation].moveOnChange) {
        Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation].scroll(options.slideNum);
      }
    }
  };

  /**
   * Implement the previousSlide hook for pager jCarousel pager.
   */
  Drupal.viewsSlideshowJcarouselPager.previousSlide = function (options) {
    for(pagerLocation in Drupal.settings.viewsSlideshowPager[options.slideshowID]) {
      // Get the current active pager.
      var pagerNum = $('[id^="views_slideshow_jcarousel_pager_item_' + pagerLocation + '_'  + options.slideshowID + '"].active').attr('id').replace('views_slideshow_pager_field_item_' + pagerLocation + '_'  + options.slideshowID + '_', '');

      // If we are on the first pager then activate the last pager.
      // Otherwise activate the previous pager.
      if (pagerNum == 0) {
        pagerNum = $('[id^="views_slideshow_jcarousel_pager_item_' + pagerLocation + '_'  + options.slideshowID + '"]').length() - 1;
      }
      else {
        pagerNum--;
      }

      if (Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation].moveOnChange) {
        Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation].scroll(pagerNum);
      }
    }
  };

  /**
   * Implement the nextSlide hook for pager jcarousel pager.
   */
  Drupal.viewsSlideshowJcarouselPager.nextSlide = function (options) {
    for(pagerLocation in Drupal.settings.viewsSlideshowPager[options.slideshowID]) {
      // Get the current active pager.
      var pagerNum = $('[id^="views_slideshow_jcarousel_pager_item_' + pagerLocation + '_'  + options.slideshowID + '"].active').attr('id').replace('views_slideshow_jcarousel_pager_item_' + pagerLocation + '_'  + options.slideshowID + '_', '');
      var totalPagers = $('[id^="views_slideshow_jcarousel_pager_item_' + pagerLocation + '_'  + options.slideshowID + '"]').length();

      // If we are on the last pager then activate the first pager.
      // Otherwise activate the next pager.
      pagerNum++;
      if (pagerNum == totalPagers) {
        pagerNum = 0;
      }

      if (Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation].moveOnChange) {
        Drupal.settings.viewsSlideshowJCarouselPager[options.slideshowID][pagerLocation].scroll(pagerNum);
      }
    }
  };
})(jQuery);
