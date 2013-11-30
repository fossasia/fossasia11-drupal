
/**
 *  @file
 *  Set Views Slideshow DDblock jQuery Cycle plugin functionality 
 */
Drupal.behaviors.viewsSlideshowDdblockCycle = function (context) {

  //helper function to clone the options object
  function CloneObject(inObj) {
    for (i in inObj)
    {
        this[i] = inObj[i];
    }
  }

  // cycle Plugin onBefore function to add functionality before the next slide shows up
  // can be used to add the following effects to slide-text
  // fadeOut - Fade out all matched elements by adjusting their opacity and firing an optional callback after completion.
  // slideUp - Hide all matched elements by adjusting their height and firing an optional callback after completion.
  // hide - Hide all matched elements using a graceful animation and firing an optional callback after completion.
  function onBefore(curr, next, opts, fwd) {
    if (opts.slideTextjQuery == 1){
      if (opts.slideTextEffectBeforeSpeed == 0) {
        opts.slideTextEffectBeforeSpeed = 1;
      };
      switch (opts.slideTextEffectBefore) {
      case "fadeOut":
        $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' ' + opts.slideTextContainer + '-' + opts.slideTextPosition).fadeOut(opts.slideTextEffectBeforeSpeed);
      break;
      case "slideUp":
        $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' ' + opts.slideTextContainer + '-' + opts.slideTextPosition).slideUp(opts.slideTextEffectBeforeSpeed);
      break;
      default:
        $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' ' + opts.slideTextContainer + '-' + opts.slideTextPosition).hide(opts.slideTextEffectBeforeSpeed);
      }
    }  
  }

  // cycle Plugin onAfter function to add functionality after the next slide shows up
  // can be used to add the following effects to slide-text
  // fadein - Fade in all matched elements by adjusting their opacity and firing an optional callback after completion.
  // slideDown - Reveal all matched elements by adjusting their height and firing an optional callback after completion.
  // show - Show all matched elements using a graceful animation and firing an optional callback after completion.
  function onAfter(curr, next, opts, fwd) {
    if (opts.slideTextjQuery == 1){
      if (opts.slideTextEffectAfterSpeed == 0) {
        opts.slideTextEffectAfterSpeed = 1;
      };
      switch (opts.slideTextEffectAfter) {
      case "fadeIn":
       $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' ' + opts.slideTextContainer + '-'  + opts.slideTextPosition).fadeIn(opts.slideTextEffectAfterSpeed);
      break;
      case 'slideDown':
       $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' ' + opts.slideTextContainer + '-' + opts.slideTextPosition).slideDown(opts.slideTextEffectAfterSpeed);
      break;
      default:
       $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' ' + opts.slideTextContainer + '-' + opts.slideTextPosition).show(opts.slideTextEffectAfterSpeed);
      }
    }  
    
    // stop slideshow when video is started used with flowplayer
    //$f(opts.currSlide).onStart(function() { 
      //alert('stop slideshow ('+opts.currSlide+')');
    //  $container.cycle('pause');
    //});

    //when scrollable pager is used set active pager-item to current slide
    if (opts.pager1 == 'scrollable-pager' ){
      opts.myScrollable.click(opts.currSlide);
    }

    // show pager count (0 of x)
    $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' ' + 'a.count').html((opts.currSlide + 1) + " " + Drupal.t("of") + " " + opts.slideCount);

    // For prev/next pager in the pager. Only show prev if previous slide exist - Only show next if next slide exist
    var index = $(this).parent().children().index(this);
    if (opts.pager2PagerHide == 1) {
      $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' li.pager-prev ' + 'a.prev')[index == 0 ? 'hide' : 'show']();
      $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' li.pager-next ' + 'a.next')[index == opts.slideCount - 1 ? 'hide' : 'show']();
    }

    // For prev/next pager in the slides. Only show prev if previous slide exist - Only show next if next slide exist
    var index = $(this).parent().children().index(this);
    if (opts.pager2SlideHide == 1) {
      $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' div.prev-container ' + 'a.prev')[index == 0 ? 'hide' : 'show']();
      $("#views-slideshow-ddblock-"+ opts.ddblocknr + ' div.next-container ' + 'a.next')[index == opts.slideCount - 1 ? 'hide' : 'show']();
    }
  }

  i=0;
  for (var base in Drupal.settings.viewsSlideshowDdblockContent) {
    // new options var for every block
    var options = new CloneObject($.fn.cycle.defaults);

    // simplify variable name
    var ViewsSlideshowDdblockSettings = Drupal.settings.viewsSlideshowDdblockContent[base];
    var block = ViewsSlideshowDdblockSettings.block;
    //alert(ViewsSlideshowDdblockSettings.nrOfItems);
    var custom = ViewsSlideshowDdblockSettings.custom;
    var pager = ViewsSlideshowDdblockSettings.pager;
    //alert(base);
    var pager2 = ViewsSlideshowDdblockSettings.pager2;
    var contentContainer = ViewsSlideshowDdblockSettings.contentContainer;
    var pagerContainer = ViewsSlideshowDdblockSettings.pagerContainer;

    // if not processed
    if (!$('#views-slideshow-ddblock-' + block + '.ddblock-processed', context).size()) {

      // set transition option
      options.fx = ViewsSlideshowDdblockSettings.fx;

      //set delay option for the blocks at different values so they less interfere with eachother
      options.delay = i * -1000;

      // set pager. You can have only one pager per block this way
      if (pager == 'image-pager' || pager == 'number-pager' || pager == 'custom-pager' || pager == 'scrollable-pager') {
        // number pager, image pager and custom pager and scrollable pager
        options.pager = "#views-slideshow-ddblock-" + pager + "-" + block;
        //store pager1 
        options.pager1 = pager;
        if (pager == 'number-pager') {
          options.pagerAnchorBuilder = function(idx, slide) {
            // return selector string for existing anchor
            return "#views-slideshow-ddblock-" + pager + "-" + block + " li.number-pager-item:eq(" + idx + ") a.pager-link";
          }
        }
        if (pager == 'image-pager') {
          options.pagerAnchorBuilder = function(idx, slide) {
            // return selector string for existing anchor
            return "#views-slideshow-ddblock-" + pager + "-" + block + " li.image-pager-item:eq(" + idx + ") a.pager-link";
          }
        }
        if (pager == 'custom-pager') {
          options.pagerAnchorBuilder = function(idx, slide) {
            // return selector string for existing anchor
            return "#views-slideshow-ddblock-" + pager + "-" + block + " " 
            + pagerContainer + ":eq(" + idx + ") a.pager-link";
          }
        }
        if (pager == 'scrollable-pager') {
          options.pagerAnchorBuilder = function(idx, slide) {
            // return selector string for existing anchor
            return "#views-slideshow-ddblock-" + pager + "-" + block + " " 
            + pagerContainer + ":eq(" + idx + ") a.pager-link";
          }
        }
      }

      //set event which drives the pager navigation
      options.pagerEvent = ViewsSlideshowDdblockSettings.pagerEvent;
	    options.prevNextEvent = ViewsSlideshowDdblockSettings.pager2Event;

      // If pager fast set use fastOnEvent pager
      options.fastOnEvent = (ViewsSlideshowDdblockSettings.pagerFast == 1) ? 1 : 0;
          
      // pause slideshow on pager hover
      options.pauseOnPagerHover = (ViewsSlideshowDdblockSettings.pagerPause == 1) ? 1 : 0;

      // disable click if pager is mouseover
      if (ViewsSlideshowDdblockSettings.pagerEvent == 'mouseover') {
          $("#views-slideshow-ddblock-" + pager + "-" + ViewsSlideshowDdblockSettings.block + " a.pager-link").click(function() {
            return false;
          });
      }
       
      // disable click if prev/next pager is mouseover
      if (ViewsSlideshowDdblockSettings.pager2Event == 'mouseover') {
          $("#views-slideshow-ddblock-"+ ViewsSlideshowDdblockSettings.block + ' a.prev').click(function() {
            return false;
          });
          $("#views-slideshow-ddblock-"+ ViewsSlideshowDdblockSettings.block + ' a.next').click(function() {
            return false;
          });
      }

      //add prev next pager
      if (pager2 == 1) {
        options.prev = "#views-slideshow-ddblock-"+ ViewsSlideshowDdblockSettings.block + ' a.prev';
        options.next = "#views-slideshow-ddblock-"+ ViewsSlideshowDdblockSettings.block + ' a.next';
      } 
      else {
        //set next
        if (ViewsSlideshowDdblockSettings.next) {
            options.next = "#views-slideshow-ddblock-"+ ViewsSlideshowDdblockSettings.block + ' ' + contentContainer;
        }
      }
      
      //set expression for selecting slides (if something other than all children is required)
      options.slideExpr = contentContainer;

      //set speed of the transition (any valid fx speed value)
      options.speed = ViewsSlideshowDdblockSettings.speed;
      if (options.speed == 0) {
        options.speed = 1;
      };

      //set timeout in milliseconds between slide transitions (0 to disable auto advance)
      options.timeout = ViewsSlideshowDdblockSettings.timeOut;

      //set pause, true to enable "pause on hover"
      if (ViewsSlideshowDdblockSettings.pause) {
        options.pause = ViewsSlideshowDdblockSettings.pause;
      }

      //set custom options
      if (custom) {
        // get the \r\n from the string
        var custom1 = custom.replace(/\r\n/gi,"");

        // parse into JSON object
        var custom2 = JSON.parse(JSON.stringify(custom1));

        // merge custom2 with options object
        jQuery.extend(true, options, custom2);
      }

      // redefine Cycle's updateActivePagerLink function
      $.fn.cycle.updateActivePagerLink = function(pager, currSlide) {
        $(pager)
          .find('a.pager-link')
          .removeClass('activeSlide')
          .filter('a.pager-link:eq('+currSlide+')')
          .addClass('activeSlide');
        $(pager)
          .find('.custom-pager-item')
          .removeClass('active-pager-item')
          .filter('.custom-pager-item:eq('+currSlide+')')
          .addClass('active-pager-item');
        $(pager)
          .find('.scrollable-pager-item')
          .removeClass('active-pager-item')
          .filter('.scrollable-pager-item:eq('+currSlide+')')
          .addClass('active-pager-item');
      };

      options.pager2PagerHide = ViewsSlideshowDdblockSettings.pager2PagerHide;
      options.pager2SlideHide = ViewsSlideshowDdblockSettings.pager2SlideHide;
      options.ddblocknr = block;
      options.before = onBefore;
      options.after = onAfter;
      
      if (ViewsSlideshowDdblockSettings.slideText == 1) {
        //set slidetext options
        options.slideTextContainer = ViewsSlideshowDdblockSettings.slideTextContainer;
        options.slideTextPosition = ViewsSlideshowDdblockSettings.slideTextPosition;
        options.slideTextEffectBefore = ViewsSlideshowDdblockSettings.slideTextEffectBefore;
        options.slideTextEffectBeforeSpeed = ViewsSlideshowDdblockSettings.slideTextEffectBeforeSpeed;
        options.slideTextEffectAfter = ViewsSlideshowDdblockSettings.slideTextEffectAfter;
        options.slideTextEffectAfterSpeed = ViewsSlideshowDdblockSettings.slideTextEffectAfterSpeed;
        options.slideTextjQuery = ViewsSlideshowDdblockSettings.slideTextjQuery;
      }
 
      options.pagerContainer = ViewsSlideshowDdblockSettings.pagerContainer;
     
      if (pager == 'scrollable-pager') {
        // set scrollableVertical to true when pager at left of right right, otherwise the scrollableVertical to false
        if (ViewsSlideshowDdblockSettings.pagerPosition == 'left' || ViewsSlideshowDdblockSettings.pagerPosition == 'right') {
          scrollableVertical = true;
        } 
        else {
          scrollableVertical = false;
        }
        // create one scrollable element and return the API by enabling the "api" property
        var myScrollable = $('#views-slideshow-ddblock-scrollable-pager-' + 
                              block + 
                              ' div.vsd-scrollable-pager').scrollable({ 
 
          // number of items vissible in scrollable pager 
          size: ViewsSlideshowDdblockSettings.nrOfPagerItems,
          
          //vertical slideshow
          vertical:scrollableVertical,
          
          //nextitem navigation, default used
          //next: '.next',
          
          //previtem navigation, default used
          //prev: '.prev',

          //enable api property
          api:true
   
        });
        //activate slide 1
        myScrollable.click(0);
        
        //set scrollable pager option
        options.myScrollable = myScrollable;
      }
      
      //start with slidenr from URL
      function getUrlVars() {
        var vars = [], hash;
        var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
        for(var i = 0; i < hashes.length; i++) {
          hash = hashes[i].split('=');
          vars.push(hash[0]);
          vars[hash[0]] = hash[1];
        }
        return vars;
      }  
      
      var startThisSlide = getUrlVars()["slidenum"];
      if (startThisSlide > 0 ){
        options.startingSlide = startThisSlide;
      }
      
      //Use the parent of the slides as the parent container so the children function can be used for the second pager
      if (ViewsSlideshowDdblockSettings.nrOfItems == 1) {
        var $container = $('#views-slideshow-ddblock-' + block + ' ' + contentContainer).parent();
        $container
        .css('visibility', 'visible')
        .addClass('ddblock-processed');
        $(contentContainer, $container).css('display', 'block');
        var $slideshowContainer = $('#views-slideshow-ddblock-' + block);
        //hide the pager
        $('div.views-slideshow-ddblock-pager', $slideshowContainer).css('display', 'none');
        //hide the pager on the slide
        $('div.views-slideshow-ddblock-prev-next-slide', $slideshowContainer).css('display', 'none');
      }
      
      else {
        var $container = $('#views-slideshow-ddblock-' + block + ' ' + contentContainer).parent();
        $container
        .cycle(options)
        .css('visibility', 'visible')
        .addClass('ddblock-processed');
      
        // stop slideshow when hover on scrollable pager container
        if (pager == 'scrollable-pager') {
          if (ViewsSlideshowDdblockSettings.pagerPause == 1) {
            $('#views-slideshow-ddblock-' + block + ' .scrollable-pager').hover(
              function() {
                $container.cycle('pause');
              },
              function() {
                $container.cycle('resume');
              }
            );
          }
        }
      }  

        
/*        $('#views-slideshow-ddblock-' + block + ' ' + 'a.pause').click(function() { 
          $('#views-slideshow-ddblock-' + block + ' ' + contentContainer).parent().cycle(2); 
          return false;
        }); 

        $('#views-slideshow-ddblock-' + block + ' ' + 'a.play').click(function() { 
          $('#views-slideshow-ddblock-' + block + ' ' + contentContainer).parent().cycle('resume', true);
          return false;          
        });
 
        $container.hover(
          function() { $('#views-slideshow-ddblock-' + block + ' ' +'.controls').fadeIn(); },
          function() { $('#views-slideshow-ddblock-' + block + ' ' +'.controls').fadeOut(); }
        );
 */
    }  
    i++;
  }
};



