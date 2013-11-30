
/**
 * @file
 * A simple jQuery ThumbnailHover Div Slideshow Rotator.
 */

(function ($) {
  /**
   * This will set our initial behavior, by starting up each individual slideshow.
   */
  Drupal.behaviors.viewsSlideshowThumbnailHover = function (context) {
    $('.views_slideshow_thumbnailhover_main:not(.viewsSlideshowThumbnailHover-processed)', context).addClass('viewsSlideshowThumbnailHover-processed').each(function() {
      var fullId = '#' + $(this).attr('id');
      if (!Drupal.settings.viewsSlideshowThumbnailHover || !Drupal.settings.viewsSlideshowThumbnailHover[fullId]) {
        return;
      }

      var settings = Drupal.settings.viewsSlideshowThumbnailHover[fullId];
      settings.targetId = '#' + $(fullId + " :first").attr('id');
      settings.paused = false;

      settings.opts = {
        speed:settings.speed,
        timeout:parseInt(settings.timeout),
        delay:parseInt(settings.delay),
        sync:settings.sync==1,
        random:settings.random==1,
        pause:false,
        allowPagerClickBubble:(settings.pager_event=='click')? false : true,
        pager:(settings.pager_event == 'hoverIntent') ? null : '#views_slideshow_breakout_teasers_' + settings.vss_id,
        nowrap:parseInt(settings.nowrap),
        pagerAnchorBuilder:(settings.pager_event == 'hoverIntent') ? null : function(idx, slide) {
          return '#views_slideshow_thumbnailhover_div_breakout_teaser_' + settings.vss_id + '_' + idx;
        },
        after:function(curr, next, opts) {
          // Used for Image Counter.
          if (settings.image_count) {
            $('#views_slideshow_thumbnailhover_image_count_' + settings.vss_id + ' span.num').html(opts.currSlide + 1);
            $('#views_slideshow_thumbnailhover_image_count_' + settings.vss_id + ' span.total').html(opts.slideCount);
          }
        },
        before:function(current, next, opts) {
          // Remember last slide.
          if (settings.remember_slide) {
            createCookie(settings.view_id, opts.currSlide + 1, settings.remember_slide_days);
          }

          // Make variable height.
          if (settings.fixed_height == 0) {
            //get the height of the current slide
            var $ht = $(this).height();
            //set the container's height to that of the current slide
            $(this).parent().animate({height: $ht});
          }

          var currId = (currId=$(current).attr('id')).substring(currId.lastIndexOf('_')+1)
          var nextId = (nextId=$(next).attr('id')).substring(nextId.lastIndexOf('_')+1)
          $('#views_slideshow_thumbnailhover_div_breakout_teaser_' + settings.vss_id + '_' + currId).removeClass('activeSlide');
          $('#views_slideshow_thumbnailhover_div_breakout_teaser_' + settings.vss_id + '_' + nextId).addClass('activeSlide');
        },
        pagerEvent: (settings.pager_event == 'hoverIntent') ? null : settings.pager_event,
        prev:(settings.controls > 0)?'#views_slideshow_thumbnailhover_prev_' + settings.vss_id:null,
        next:(settings.controls > 0)?'#views_slideshow_thumbnailhover_next_' + settings.vss_id:null,
        cleartype:(settings.ie.cleartype == 'true')? true : false,
        cleartypeNoBg:(settings.ie.cleartypenobg == 'true')? true : false
      };

      // Set the starting slide if we are supposed to remember the slide
      if (settings.remember_slide) {
        var startSlide = readCookie(settings.view_id);
        if (startSlide == null) {
          startSlide = 0;
        }
        settings.opts.startingSlide =  startSlide;
      }

      if (settings.effect == 'none') {
        settings.opts.speed = 1;
      }
      else {
        settings.opts.fx = settings.effect;
      }

      // Pause on hover.
      if (settings.pause == 1) {
        $('#views_slideshow_thumbnailhover_teaser_section_' + settings.vss_id).hover(function() {
          $(settings.targetId).cycle('pause');
        }, function() {
          if (settings.paused == false) {
            $(settings.targetId).cycle('resume');
          }
        });
      }

      // Pause on clicking of the slide.
      if (settings.pause_on_click == 1) {
        $('#views_slideshow_thumbnailhover_teaser_section_' + settings.vss_id).click(function() {
          viewsSlideshowThumbnailHoverPause(settings);
        });
      }

      // Add additional settings.
      if (settings.advanced  && settings.advanced != "\n") {
        settings.advanced = settings.advanced.toString();
        var advanced = settings.advanced.split("\n");
        for (i=0; i<advanced.length; i++) {
          var prop = '';
          var value = '';
          var property = advanced[i].split(":");
          for (j=0; j<property.length; j++) {
            if (j == 0) {
              prop = property[j];
            }
            else if (j == 1) {
              value = property[j];
            }
            else {
              value += ":" + property[j];
            }
          }

          // Need to evaluate so true, false and numerics aren't a string.
          if (value == 'true' || value == 'false' || IsNumeric(value)) {
            value = eval(value);
          }
          else {
            // Parse strings into functions.
            var func = value.match(/function\s*\((.*?)\)\s*\{(.*)\}/i);
            if (func) {
              value = new Function(func[1].match(/(\w+)/g), func[2]);
            }
          }

          // Call both functions if prop was set previously.
          if (typeof(value) == "function" && prop in settings.opts) {
            var callboth = function(before_func, new_func) {
              return function() {
                before_func.apply(null, arguments);
                new_func.apply(null, arguments);
              };
            };
            settings.opts[prop] = callboth(settings.opts[prop], value);
          }
          else {
            settings.opts[prop] = value;
          }
        }
      }

      $(settings.targetId).cycle(settings.opts);

      // Start Paused
      if (settings.start_paused) {
        viewsSlideshowThumbnailHoverPause(settings);
      }

      // Pause if hidden.
      if (settings.pause_when_hidden) {
        var checkPause = function(settings) {
          // If the slideshow is visible and it is paused then resume.
          // otherwise if the slideshow is not visible and it is not paused then
          // pause it.
          var visible = viewsSlideshowThumbnailHoverIsVisible(settings.targetId, settings.pause_when_hidden_type, settings.amount_allowed_visible);
          if (visible && settings.paused) {
            viewsSlideshowThumbnailHoverResume(settings);
          }
          else if (!visible && !settings.paused) {
            viewsSlideshowThumbnailHoverPause(settings);
          }
        }

        // Check when scrolled.
        $(window).scroll(function() {
         checkPause(settings);
        });

        // Check when window is resized.
        $(window).resize(function() {
          checkPause(settings);
        });
      }

      // Show image count for people who have js enabled.
      $('#views_slideshow_thumbnailhover_image_count_' + settings.vss_id).show();

      if (settings.pager_event == 'hoverIntent') {
        $('#views_slideshow_thumbnailhover_breakout_teasers_' + settings.vss_id + ' .views_slideshow_thumbnailhover_div_breakout_teaser').each(function(i,obj) {
          $(obj).hoverIntent(
            function() {
              $('.views_slideshow_thumbnailhover_div_breakout_teaser').removeClass('activeSlide');
              var id = $(this).attr('id');
              id = parseInt(id.substring(id.lastIndexOf('_')+1));
              $(settings.targetId).cycle(id);
              $('#views_slideshow_thumbnailhover_div_breakout_teaser_' + settings.vss_id + '_' + id).addClass('activeSlide');
              $(settings.targetId).cycle('stop');
            },
            function() {
              var id = $(this).attr('id');
              settings.opts.startingSlide = parseInt(id.substring(id.lastIndexOf('_')+1));
              $(settings.targetId).cycle(settings.opts);
            }
          );
        });
      }

      if (settings.controls > 0) {
        // Show controls for people who have js enabled browsers.
        $('#views_slideshow_thumbnailhover_controls_' + settings.vss_id).show();

        $('#views_slideshow_thumbnailhover_playpause_' + settings.vss_id).click(function(e) {
          if (settings.paused) {
            viewsSlideshowThumbnailHoverResume(settings);
          }
          else {
            viewsSlideshowThumbnailHoverPause(settings);
          }
          e.preventDefault();
        });
      }
    });
  }

  // Pause the slideshow
  viewsSlideshowThumbnailHoverPause = function (settings) {
    //make Resume translatable
    var resume = Drupal.t('Resume');

    $(settings.targetId).cycle('pause');
    if (settings.controls > 0) {
      $('#views_slideshow_thumbnailhover_playpause_' + settings.vss_id)
        .addClass('views_slideshow_thumbnailhover_play')
        .addClass('views_slideshow_play')
        .removeClass('views_slideshow_thumbnailhover_pause')
        .removeClass('views_slideshow_pause')
        .text(resume);
    }
    settings.paused = true;
  }

  // Resume the slideshow
  viewsSlideshowThumbnailHoverResume = function (settings) {
    // Make Pause translatable
    var pause = Drupal.t('Pause');

    $(settings.targetId).cycle('resume');
    if (settings.controls > 0) {
      $('#views_slideshow_thumbnailhover_playpause_' + settings.vss_id)
        .addClass('views_slideshow_thumbnailhover_pause')
        .addClass('views_slideshow_pause')
        .removeClass('views_slideshow_thumbnailhover_play')
        .removeClass('views_slideshow_play')
        .text(pause);
    }
    settings.paused = false;
  }

  // Verify that the value is a number.
  function IsNumeric(sText) {
    var ValidChars = "0123456789";
    var IsNumber=true;
    var Char;

    for (var i=0; i < sText.length && IsNumber == true; i++) {
      Char = sText.charAt(i);
      if (ValidChars.indexOf(Char) == -1) {
        IsNumber = false;
      }
    }
    return IsNumber;
  }

  /**
   * Cookie Handling Functions
   */
  function createCookie(name,value,days) {
    if (days) {
      var date = new Date();
      date.setTime(date.getTime()+(days*24*60*60*1000));
      var expires = "; expires="+date.toGMTString();
    }
    else {
      var expires = "";
    }
    document.cookie = name+"="+value+expires+"; path=/";
  }

  function readCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) {
      var c = ca[i];
      while (c.charAt(0)==' ') c = c.substring(1,c.length);
      if (c.indexOf(nameEQ) == 0) {
        return c.substring(nameEQ.length,c.length);
      }
    }
    return null;
  }

  function eraseCookie(name) {
    createCookie(name,"",-1);
  }

  /**
   * Checks to see if the slide is visible enough.
   * elem = element to check.
   * amountVisible = amount that should be visible. Either in percent or px. If
   *                it's not defined then all of the slide must be visible.
   *
   * Returns true or false
   */
  function viewsSlideshowThumbnailHoverIsVisible(elem, type, amountVisible) {
    // Get the top and bottom of the window;
    var docViewTop = $(window).scrollTop();
    var docViewBottom = docViewTop + $(window).height();
    var docViewLeft = $(window).scrollLeft();
    var docViewRight = docViewLeft + $(window).width();

    // Get the top, bottom, and height of the slide;
    var elemTop = $(elem).offset().top;
    var elemHeight = $(elem).height();
    var elemBottom = elemTop + elemHeight;
    var elemLeft = $(elem).offset().left;
    var elemWidth = $(elem).width();
    var elemRight = elemLeft + elemWidth;
    var elemArea = elemHeight * elemWidth;

    // Calculate what's hiding in the slide.
    var missingLeft = 0;
    var missingRight = 0;
    var missingTop = 0;
    var missingBottom = 0;

    // Find out how much of the slide is missing from the left.
    if (elemLeft < docViewLeft) {
      missingLeft = docViewLeft - elemLeft;
    }

    // Find out how much of the slide is missing from the right.
    if (elemRight > docViewRight) {
      missingRight = elemRight - docViewRight;
    }

    // Find out how much of the slide is missing from the top.
    if (elemTop < docViewTop) {
      missingTop = docViewTop - elemTop;
    }

    // Find out how much of the slide is missing from the bottom.
    if (elemBottom > docViewBottom) {
      missingBottom = elemBottom - docViewBottom;
    }

    // If there is no amountVisible defined then check to see if the whole slide
    // is visible.
    if (type == 'full') {
      return ((elemBottom >= docViewTop) && (elemTop <= docViewBottom)
      && (elemBottom <= docViewBottom) &&  (elemTop >= docViewTop)
      && (elemLeft >= docViewLeft) && (elemRight <= docViewRight)
      && (elemLeft <= docViewRight) && (elemRight >= docViewLeft));
    }
    else if(type == 'vertical') {
      var verticalShowing = elemHeight - missingTop - missingBottom;

      // If user specified a percentage then find out if the current shown percent
      // is larger than the allowed percent.
      // Otherwise check to see if the amount of px shown is larger than the
      // allotted amount.
      if (amountVisible.indexOf('%')) {
        return (((verticalShowing/elemHeight)*100) >= parseInt(amountVisible));
      }
      else {
        return (verticalShowing >= parseInt(amountVisible));
      }
    }
    else if(type == 'horizontal') {
      var horizontalShowing = elemWidth - missingLeft - missingRight;

      // If user specified a percentage then find out if the current shown percent
      // is larger than the allowed percent.
      // Otherwise check to see if the amount of px shown is larger than the
      // allotted amount.
      if (amountVisible.indexOf('%')) {
        return (((horizontalShowing/elemWidth)*100) >= parseInt(amountVisible));
      }
      else {
        return (horizontalShowing >= parseInt(amountVisible));
      }
    }
    else if(type == 'area') {
      var areaShowing = (elemWidth - missingLeft - missingRight) * (elemHeight - missingTop - missingBottom);

      // If user specified a percentage then find out if the current shown percent
      // is larger than the allowed percent.
      // Otherwise check to see if the amount of px shown is larger than the
      // allotted amount.
      if (amountVisible.indexOf('%')) {
        return (((areaShowing/elemArea)*100) >= parseInt(amountVisible));
      }
      else {
        return (areaShowing >= parseInt(amountVisible));
      }
    }
  }
})(jQuery);
