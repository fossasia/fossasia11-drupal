
/**
 * @file
 * A simple jQuery JCarouselThumbs Div Slideshow Rotator.
 */

/**
 * This will set our initial behavior, by starting up each individual slideshow.
 */
Drupal.behaviors.viewsSlideshowJCarouselThumbs = function (context) {
  $('.views_slideshow_jcarouselthumbs_main:not(.viewsSlideshowJCarouselThumbs-processed)', context).addClass('viewsSlideshowJCarouselThumbs-processed').each(function() {
    var fullId = '#' + $(this).attr('id');
    var settings = Drupal.settings.viewsSlideshowJCarouselThumbs[fullId];
    settings.targetId = '#' + $(fullId + " :first").attr('id');
    settings.paused = false;

    settings.opts = {
      speed:settings.speed,
      timeout:parseInt(settings.timeout),
      delay:parseInt(settings.delay),
      sync:settings.sync==1,
      random:settings.random==1,
      pause:false,
      pager:(settings.pager_event == 'hoverIntent') ? null : '#views_slideshow_breakout_teasers_' + settings.id,
      nowrap:parseInt(settings.nowrap),
      pagerAnchorBuilder:(settings.pager_event == 'hoverIntent') ? null : function(idx, slide) { 
        return '#views_slideshow_jcarouselthumbs_div_breakout_teaser_' + settings.id + '_' + idx; 
      },
      after:function(curr, next, opts) {
        // Used for Image Counter.
        if (settings.image_count) {
          $('#views_slideshow_jcarouselthumbs_image_count_' + settings.id + ' span.num').html(opts.currSlide + 1);
          $('#views_slideshow_jcarouselthumbs_image_count_' + settings.id + ' span.total').html(opts.slideCount);
        }
      },
      before:function(current, next) {
        if (settings.fixed_height == 0) {
          //get the height of the current slide
          var $ht = $(this).height();
          //set the container's height to that of the current slide
          $(this).parent().animate({height: $ht});
        }
        var currId = (currId=$(current).attr('id')).substring(currId.lastIndexOf('_')+1)
        var nextId = (nextId=$(next).attr('id')).substring(nextId.lastIndexOf('_')+1)
        $('#views_slideshow_jcarouselthumbs_div_breakout_teaser_' + settings.id + '_' + currId).removeClass('activeSlide');
        $('#views_slideshow_jcarouselthumbs_div_breakout_teaser_' + settings.id + '_' + nextId).addClass('activeSlide');

        //scroll the carousel if necessary        
        var temp = $('#views_slideshow_jcarouselthumbs_div_breakout_teaser_' + settings.id + '_' + nextId);
        var doScroll = false;
        var pos = temp.position();
        var clip = $('#views_slideshow_jcarouselthumbs_breakout_teasers_' + settings.id + ' .jcarousel-clip');
        var posTarget =  $('#views_slideshow_jcarouselthumbs_breakout_teasers_' + settings.id + " .carousel-target");

        if (settings.jcarousel_settings.vertical) {
          var offsetTop = pos.top;
          var itemHeight = temp.height();
          var clipHeight = clip.height();
          var clipTop = posTarget.position().top * -1;
          doScroll = (offsetTop + itemHeight > clipHeight + clipTop|| offsetTop < clipTop)
        } else {
          var offsetLeft = pos.left;
          var itemWidth = temp.width();
          var clipWidth = clip.width();
          var clipLeft = posTarget.position().left * -1;
          doScroll = (offsetLeft + itemWidth > clipWidth + clipLeft || offsetLeft < clipLeft)
        }

        if (doScroll) {
          settings.carousel.scroll(parseInt(nextId)+1,true)
        }
      },
      pagerEvent: (settings.pager_event == 'hoverIntent') ? null : settings.pager_event,
      prev:(settings.controls > 0)?'#views_slideshow_jcarouselthumbs_prev_' + settings.id:null,
      next:(settings.controls > 0)?'#views_slideshow_jcarouselthumbs_next_' + settings.id:null,
      cleartype:eval(settings.ie.cleartype),
      cleartypeNoBg:eval(settings.ie.cleartypenobg)
    };

    if (settings.effect == 'none') {
      settings.opts.speed = 1;
    }
    else {
      settings.opts.fx = settings.effect;
    }
    var jcarousel_settings = {};
    jcarousel_settings.scroll = eval(settings.scroll);
    if (settings.jcarousel_advanced.length > 0 && settings.jcarousel_advanced != "\n") {
      viewsSlideshowAdvancedSettings(settings.jcarousel_advanced, jcarousel_settings);
    }

    settings.jcarousel_settings = jcarousel_settings;

    $('#views_slideshow_jcarouselthumbs_breakout_teasers_' + settings.id + " .carousel-target").jcarousel(
      $.extend({
        initCallback:function(c) {
          settings.carousel = c;
          $(c.buttonNext).click(function() {
            viewsSlideshowJCarouselThumbsPause(settings);
          });
          $(c.buttonPrev).click(function() {
            viewsSlideshowJCarouselThumbsPause(settings);
          });
        }
      }, jcarousel_settings));		

    // Pause on hover.
    if (settings.pause == 1) {
      $('#views_slideshow_jcarouselthumbs_teaser_section_' + settings.id).hover(function() {
        $(settings.targetId).cycle('pause');
      }, function() {
        if (settings.paused == false) {
          $(settings.targetId).cycle('resume');
        }
      });
    }

    // Pause on clicking of the slide.
    if (settings.pause_on_click == 1) {
      $('#views_slideshow_jcarouselthumbs_teaser_section_' + settings.id).click(function() { 
        viewsSlideshowJCarouselThumbsPause(settings);
      });
    }

    // Add additional settings.
    if (settings.advanced.length > 0 && settings.advanced != "\n") {
      viewsSlideshowAdvancedSettings(settings.advanced, settings.opts);
    }

    $(settings.targetId).cycle(settings.opts);

    // Show image count for people who have js enabled.
    $('#views_slideshow_jcarouselthumbs_image_count_' + settings.id).show();


    $('#views_slideshow_jcarouselthumbs_breakout_teasers_' + settings.id + ' .views_slideshow_jcarouselthumbs_div_breakout_teaser').each(function(i,obj) {
      if (settings.pager_event == 'hoverIntent') {
        $(obj).hoverIntent(
          function() {
            $('.views_slideshow_jcarouselthumbs_div_breakout_teaser').removeClass('activeSlide');
            var id = $(this).attr('id');
            id = parseInt(id.substring(id.lastIndexOf('_')+1));
            $(settings.targetId).cycle(id);
            $('#views_slideshow_jcarouselthumbs_div_breakout_teaser_' + settings.id + '_' + id).addClass('activeSlide');
            viewsSlideshowJCarouselThumbsPause(settings);
          },
          function() {
            var id = $(this).attr('id');
            settings.opts.startingSlide = parseInt(id.substring(id.lastIndexOf('_')+1));
            $(settings.targetId).cycle(settings.opts);
          }
        );
      } else {
        $(obj).bind(settings.pager_event, function(e) {
          viewsSlideshowJCarouselThumbsPause(settings);
        });
      }
    });


    if (settings.controls > 0) {
      // Show controls for people who have js enabled browsers.
      $('#views_slideshow_jcarouselthumbs_controls_' + settings.id).show();

      $('#views_slideshow_jcarouselthumbs_playpause_' + settings.id).click(function(e) {
        if (settings.paused) {
          viewsSlideshowJCarouselThumbsResume(settings);
        }
        else {
          viewsSlideshowJCarouselThumbsPause(settings);
        }
        e.preventDefault();
      });
    }
  });
}

viewsSlideshowAdvancedSettings = function(settings, opts) {
  var advanced = settings.split("\n");
  for (var i=0; i<advanced.length; i++) {
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
    if (typeof(value) == "function" && prop in opts) {
      var callboth = function(before_func, new_func) {
        return function() {
          before_func.apply(null, arguments);
          new_func.apply(null, arguments);
        };
      };
      opts[prop] = callboth(opts[prop], value);
    }
    else {
      opts[prop] = value;
    }
  }
  return opts;
}

// Pause the slideshow 
viewsSlideshowJCarouselThumbsPause = function (settings) {
  $(settings.targetId).cycle('pause');
  if (settings.controls > 0) {
    $('#views_slideshow_jcarouselthumbs_playpause_' + settings.id)
      .addClass('views_slideshow_jcarouselthumbs_play')
      .addClass('views_slideshow_play')
      .removeClass('views_slideshow_jcarouselthumbs_pause')
      .removeClass('views_slideshow_pause')
      .text('Resume');
  }
  settings.paused = true;
}

// Resume the slideshow
viewsSlideshowJCarouselThumbsResume = function (settings) {
  $(settings.targetId).cycle('resume');
  if (settings.controls > 0) {
    $('#views_slideshow_jcarouselthumbs_playpause_' + settings.id)
      .addClass('views_slideshow_jcarouselthumbs_pause')
      .addClass('views_slideshow_pause')
      .removeClass('views_slideshow_jcarouselthumbs_play')
      .removeClass('views_slideshow_play')
      .text('Pause');
  }
  settings.paused = false;
}

// Verify that the value is a number.
function IsNumeric(sText) {
  var ValidChars = "0123456789";
  var IsNumber=true;
  var Char;

  for (i=0; i < sText.length && IsNumber == true; i++) { 
    Char = sText.charAt(i); 
    if (ValidChars.indexOf(Char) == -1) {
      IsNumber = false;
    }
  }
  return IsNumber;
}
