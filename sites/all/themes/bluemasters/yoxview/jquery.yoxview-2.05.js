/*!
 * jquery.yoxview
 * jQuery image gallery viewer
 * http://yoxigen.com/yoxview
 *
 * Copyright (c) 2010 Yossi Kolesnicov
 *
 * Licensed under the MIT license.
 * http://www.opensource.org/licenses/mit-license.php
 *
 * Date: 9th August, 2010
 * Version : 2.0
 */ 

var yoxviewApi;

(function($){
    var yoxviewPath;
    
    $.yoxviewUnload = function()
    {
        if (yoxviewApi)
        {
            yoxviewApi.unload();
            yoxviewApi = null;
        }
    }
    $(window).unload(function(){
        $.yoxviewUnload();
    });
    
    $.fn.extend({
        yoxview: function(opt) 
        {
            if (this.length == 0)
                return this;

            var self = this;
            
            if (!yoxviewPath)
                yoxviewPath = typeof(_yoxviewPath) != "undefined" ? _yoxviewPath : Yox.getPath(/(.*)jquery.yoxview.*/i);

            // Load a skin, if specified:
            this.loadSkin = function(options, callback)
            {
                var skinName = options.skin;
                if (!yoxviewSkins[skinName])
                {
                    var skinUrl = yoxviewPath + "skins/" + skinName + "/yoxview." + skinName;
                    $.ajax({
                        url: skinUrl + ".js",
                        dataType: "script",
                        success: function(data)
                        {
                            eval(data);
                            $.extend(options, yoxviewSkins[skinName].options);
                            if (yoxviewSkins[skinName].css !== false)
                                Yox.addStylesheet(parent.document, skinUrl + ".css");
                                
                            self.loadLanguage(options, callback);
                        },
                        error: function(){
                            alert("Error loading skin file " + skinUrl + ".js");
                        }
                    });
                }
            }
            
             // Load the language file if not already loaded:
            this.loadLanguage = function(options, callBack)
            { 
                var lang = options.lang;
                if (!yoxviewLanguages[lang])
                {
                    yoxviewLanguages[lang] = {};
                    $.ajax({
                        url : yoxviewPath + "lang/" + lang + ".js",
                        async : false,
                        dataType : "json",
                        success: function(data){
                            yoxviewLanguages[lang] = data;
                            Yox.loadDataSource(options, callBack, self);
                        }
                    });
                }
                else
                    Yox.loadDataSource(options, callBack, self);
            }
            
            var defaults = {
                autoHideInfo: true, // If false, the info bar (with image count and title) is always displayed.
                autoPlay: false, // If true, slideshow mode starts when the popup opens
                backgroundColor: "#000000",
                backgroundOpacity: 0.8,
                buttonsFadeTime: 300, // The time, in milliseconds, it takes the buttons to fade in/out when hovered on. Set to 0 to force the Prev/Next buttons to remain visible.
                cacheBuffer: 5, // The number of images to cache after the current image (directional, depends on the current viewing direction)
                cacheImagesInBackground: true, // If true, full-size images are cached even while the gallery hasn't been opened yet.
                controlsInitialFadeTime: 1500, // The time, in milliseconds, it takes the menu and prev/next buttons to fade in and out when the popup is opened.
                controlsInitialDisplayTime: 1000, // The time, in milliseconds, to display the menu and prev/next buttons when the popup is opened. Set to 0 to not display them by default
                dataFolder: yoxviewPath + "data/",
                defaultDimensions: { // Default sizes for different types of media, in case none was specified
                    flash: { width: 720, height: 560 },
                    iframe: { width: 1024 }
                },
                flashVideoPlayer: "jwplayer", // The default player for Flash video files
                imagesFolder: yoxviewPath + "images/",
                infoBackColor: "#000000",
                infoBackOpacity: 0.5,
                isRTL : false, // Switch direction. For RTL languages such as Hebrew or Arabic, for example.
                lang: "en", // The language for texts. The relevant language file should exist in the lang folder.
                langFolder: yoxviewPath + "lang/",
                loopPlay: true, // If true, slideshow play starts over after the last image
                parentElement: $(parent.document.body), // The element which holds the popup
                playDelay: 3000, // Time in milliseconds to display each image
                popupMargin: 20, // the minimum margin between the popup and the window
                popupResizeTime: 600, // The time in milliseconds it takes to make the resize transition from one image to the next.
                renderButtons: true, // Set to false if you want to implement your own Next/Prev buttons, using the API.
                renderMenu: true, // Set to false if you want to implement you own menu (Play/Help/Close).
                showBarsOnOpen: true, // If true, displays the top (help) bar and bottom (info) bar momentarily when the popup opens.
                showButtonsOnOpen: true, // If true, displays the Prev/Next buttons momentarily when the popup opens.
                titleAttribute: "title",
                titleDisplayDuration: 2000 // The time in ms to display the image's title, after which it fades out.
            };

            var options = $.extend(true, defaults, opt); 

            var loadCallback = function(views)
            {
                if (!yoxviewApi)
                    yoxviewApi = new YoxView(views, options);
                else
                    yoxviewApi.AddViews(views, options);
            }
            var loadFunction = options.skin ? this.loadSkin : this.loadLanguage;
            loadFunction.apply(this, [ options, loadCallback ]);
  
            return this;
        }
    });
})(jQuery);

var yoxviewLanguages = new Array();
var yoxviewSkins = new Array();

function YoxView(_views, _options)
{  
    var yoxviewApi = this;
    var $ = jQuery; // Ensure the dollar sign stands for jQuery, in case other JS libraries are loaded, that use it.
    
    var ajaxLoader;
    var cacheBufferLastIndex;
    var cacheComplete = false;
    var cachedImagesCount = 0;
    var cacheDirectionForward = true;
    var cacheImg = new Image();
    var countDisplay;
    var ctlButtons; // next and prev buttons
    var currentCacheImg = 0;
    var elementCount = 0;
    var currentItemIndex = 0;
    var currentLanguage = {};
    var currentMaxSize = {};
    var currentViewIndex = 0;
    var defaultOptions = _options;
    var disableInfo = false;
    var flashDefaults = { wmode: "transparent", width: "100%", height: "100%", allowfullscreen: "true", allowscriptaccess: "true", hasVersion: 9 };
    var firstImage = true;
    var helpPanel;
    var hideInfoTimeout;
    var hideMenuTimeout;
    var image1;
    var image2;
    var images;
    var imagesCount = 0;
    this.infoButtons = {};
    var infoPanel;
    var infoPanelContent;
    var infoPanelLink;
    var infoPanelMinHeight = 30;
    var infoPanelWrap;
    var infoPinLink;
    var infoPinLinkImg;
    var infoText;
    this.isOpen = false;
    var isFirstPanel = false;
    var isImageMode = true;
    var isPlaying = false;
    var isResizing = false;
    var itemVar;
    var loaderTimeout;
    var loading = false;
    var mediaButtonsSize = {width: 100, height: 100};
    var mediaLoader;
    var mediaPanelClass = "yoxview_mediaPanel";
    var mediaProviderUrls = {
        vimeo: "http://vimeo.com/api/oembed.json",
        myspace: "http://vids.myspace.com/index.cfm?fuseaction=oembed"
    };
    var menuHidePosition = -42;
    var menuPanel;
    var nextBtn;
    var notification;
    var onOpenCallback;
    var options = defaultOptions;
    var panel1;
	var panel2;
	var playBtnText;
    var popup;
    var popupBackground;
    var popupMargins = {};
    var popupTimeout;
    var popupWrap;
    var prevBtn;
    var resumePlay = false;
    var tempImg = new Image();
    var thumbnail;
    var thumbnailImg;
    var thumbnailPos;
    var thumbnailProperties;
    var views = new Array();
    var windowDimensions;

    var keyCodes = {
	    40: 'DOWN',
	    35: 'END',
	    13: 'ENTER',
	    36: 'HOME',
	    37: 'LEFT',
	    39: 'RIGHT',
	    32: 'SPACE',
	    38: 'UP',
	    72: 'h',
	    27: 'ESCAPE'
    };
    var keyMappings = {
        RIGHT: options.isRTL ? 'prev' : 'next',
        DOWN: 'next',
        UP: 'prev',
        LEFT: options.isRTL ? 'next' : 'prev',
        ENTER: 'play',
        HOME: 'first',
        END: 'last',
        SPACE: 'next',
        h: 'help',
        ESCAPE: 'close'
    };
    var sprites = new Yox.Sprites({
        notifications: {
            width: 59,
            height: 59,
            sprites: [ 'empty', 'playRTL', 'play', 'pause', 'last', 'first' ]
        },
        icons: {
            width: 18,
            height: 18,
            sprites: ['close', 'help', 'playpause', 'link', 'pin', 'unpin', 'play', 'pause', 'right', 'left']
        },
        menu: {
            height: 42,
            sprites: ['back']
        }
    }, options.imagesFolder + "sprites.png", options.imagesFolder + "empty.gif");

    this.AddViews = function(_views, options)
    {
        var popupIsCreated = this.firstViewWithImages != undefined;
        jQuery.each(_views, function(){
            setView(this, views.length, options);
            views.push(this);
            if (!yoxviewApi.firstViewWithImages)
            {
                var viewImages =  $(this).data("yoxview").images;   
                if (viewImages && viewImages.length != 0)
                    yoxviewApi.firstViewWithImages = this;
            }
        });

        if (!popupIsCreated && this.firstViewWithImages)
        {
            loadViewImages(this.firstViewWithImages);
            createPopup();

            if(options.cacheImagesInBackground && imagesCount != 0)
            {
                calculateCacheBuffer();
                cacheImages(0);
            }
            popupIsCreated = true;
        }
    }
    this.SetImages = function(images)
    {
        imagesCount = images.length;
    }
    function resetPopup()
    {
        if (popup)
        {
            popupWrap.remove();
            popup = undefined;
            prevBtn = undefined;
            nextBtn = undefined;
            image1 = undefined;
            image2 = undefined;
			panel1 = undefined;
			panel2 = undefined;
            currentItemIndex = 0;
            currentCacheImg = 0;
            cachedImagesCount = 0;
            cacheComplete = false;
			yoxviewApi.infoButtons = {};
        }
        createPopup();
    }
    function loadViewImages(_view)
    {
        var viewData = $(_view).data("yoxview");
        if (!images || currentViewIndex != viewData.viewIndex)
        {
            images = viewData.images;
            imagesCount = images.length;
            currentViewIndex = viewData.viewIndex;
            cachedImagesCount = viewData.cachedImagesCount;
            cacheDirectionForward = viewData.cacheDirectionForward;
            cacheBufferLastIndex = viewData.cacheBufferLastIndex;
            
            var isResetPopup = false;

            if (viewData.options && !Yox.compare(options, viewData.options))
            {
                options = viewData.options;
                isResetPopup = true;
            }
            else if (!viewData.options && !Yox.compare(options, defaultOptions))
            {
                options = defaultOptions;
                isResetPopup = true;
            }
            else if ((prevBtn && imagesCount == 1) || (popup && !prevBtn && imagesCount > 0))
                isResetPopup = true;

            if (isResetPopup)
                resetPopup();
        }
    }
    
    function getElementDimensions(type, urlData)
    {
        var size = urlData.queryFields && (urlData.queryFields.width || urlData.queryFields.height)
            ? { width: parseInt(urlData.queryFields.width), height: parseInt(urlData.queryFields.height) }
            : options.defaultDimensions[type];
        
        if (isNaN(size.width))
            size.width = null;
        if (isNaN(size.height))
            size.height = null;
        
        if (urlData.queryFields)
        {
            urlData.queryFields.width = undefined;
            urlData.queryFields.height = undefined;
        }
        return size;
    }
    var supportedTypes = {
        image: function(thumbnail, thumbnailHref, thumbImg, options)
        {
            var imageData = null;
            for(var i=0; i<options.allowedImageUrls.length && !imageData; i++)
            {
                if (thumbnailHref.match(options.allowedImageUrls[i]))
                {
                    imageData = {
                        src : thumbnail.attr("href"),
                        title : thumbImg.attr(options.titleAttribute),
                        alt : thumbImg.attr("alt")
                    };
                }
            }
            return imageData;
        },
        flash: function(thumbnail, thumbnailHref, thumbImg, options)
        {
            var imageData = null;
            var matchFlash = thumbnailHref.match(Yox.Regex.flash);
            var matchFlashVideo = matchFlash ? null : thumbnailHref.match(Yox.Regex.flashvideo);
            
            if (matchFlash || matchFlashVideo)
            {
                var urlData = Yox.getUrlData(thumbnailHref);
                var flashPanel = $("<div>", {
                    className: "yoxview_element",
                    html: "<div class='yoxview_error'>Please install the latest version of the <a href='http://www.adobe.com/go/getflashplayer' target='_blank'>Flash player</a> to view content</div>"
                });
                var flashData = matchFlashVideo 
                    ? Yox.flashVideoPlayers[options.flashVideoPlayer](
                        options.flashVideoPlayerPath, urlData.path,
                        (urlData.queryFields && urlData.queryFields.image) ? urlData.queryFields.image : thumbImg.attr("src"), 
                        thumbImg.attr(options.titleAttribute))
                    : urlData.queryFields || {};
                
                if (matchFlash)
                    flashData.swf = urlData.path;
                    
                $.extend(flashData, flashDefaults);            
                
                flashPanel.flash(flashData);
                imageData = {
                    "element": flashPanel,
                    title: thumbImg.attr(options.titleAttribute)
                };
                $.extend(imageData, getElementDimensions("flash", urlData));
            }
            
            return imageData;
        },
        ooembed: function(thumbnail, thumbnailHref, thumbImg, options)
        {
            var imageData = null;
            for(videoProvider in Yox.Regex.video)
            {
                if (thumbnailHref.match(Yox.Regex.video[videoProvider]))
                {
                    imageData = {
                        provider: videoProvider,
                        url: thumbnailHref
                    };
                    break;
                }
            }
            return imageData;
        },
        inline: function(thumbnail, thumbnailHref, thumbImg, options)
        {
            if (!options.allowInternalLinks)
                return null;
                
            var imageData = null;
            var urlData = Yox.getUrlData(thumbnailHref);
            if (urlData && urlData.anchor)
            {
                var element = $("#" + urlData.anchor);
                if (element.length != 0)
                {
                    var elementSize = { width: parseInt(element.css("width")), height: parseInt(element.css("height")) };
                    
                    element.css({
		                position: "absolute",
		                top: 0,
		                left: 0,
		                width: "100%",
		                height: "100%",
		                display: "block"
		            });
		            
                    imageData = {
                        type: "inlineElement",
                        "element": element,
                        title: element.attr("title")
                    };
                    var padding = { 
                        horizontal: parseInt(element.css("padding-right")) + parseInt(element.css("padding-left")),
                        vertical: parseInt(element.css("padding-top")) + parseInt(element.css("padding-bottom"))
                    };
                    
                    elementSize.width = isNaN(elementSize.width) ? null : elementSize.width + padding.horizontal;
                    elementSize.height = isNaN(elementSize.height) ? null : elementSize.height + padding.vertical;
                    
                    $.extend(imageData, elementSize);
                    if (padding.horizontal != 0 || padding.vertical != 0)
                        imageData.padding = padding;
                        
                    element.remove();
                }
            }
            
            return imageData;
        },
        iframe: function(thumbnail, thumbnailHref, thumbImg, options)
        {
            var imageData = null;
            var thumbnailTarget = thumbnail.attr("target");
            if (thumbnailTarget && thumbnailTarget == "yoxview")
            {
                var urlData = Yox.getUrlData(thumbnailHref);
                if (urlData && urlData.path)
                {
                    var iframeSize = getElementDimensions("iframe", urlData);
					if (urlData.queryFields)
					{
						urlData.queryFields.width = undefined;
						urlData.queryFields.height = undefined;
                    }
                    imageData = {
                        "element": $("<iframe>", {
                            src: Yox.urlDataToPath(urlData),
                            className: "yoxview_element"
                        }),
                        title: thumbImg.attr("title"),
                        frameborder: "0"
                    }
                    $.extend(imageData, iframeSize);
                }
            }
            
            return imageData;
        }
    };
    function getImageDataFromThumbnail(thumbnail, options)
    {
        var imageData = {};
        var thumbnailHref = thumbnail.attr("href");
        var thumbImg = thumbnail.children("img:first");

        var imageData = {};
        for (supportedType in supportedTypes)
        {
            var media = supportedTypes[supportedType](thumbnail, thumbnailHref, thumbImg, options);
            if (media)
            {
                $.extend(media, {
                    contentType: supportedType,
                    elementId: elementCount++
                });

                imageData.media = media;
                break;
            }
        }
        
        if (!imageData.media)
            return null;

        imageData.thumbnailImg = thumbImg;
        return imageData;
    }
    
    function setView(view, viewIndex, _options)
    {
        var view = $(view);
        view.data("yoxview", {viewIndex : viewIndex, cachedImagesCount: 0, cacheDirectionForward: true, cacheBufferLastIndex: null });
        
        _options.allowedImageUrls = [Yox.Regex.image];
        if (_options.allowedUrls)
            _options.allowedImageUrls = _options.allowedImageUrls.concat(_options.allowedUrls);

        if (!Yox.compare(options, _options))
            view.data("yoxview").options = _options;
                    
        // First, get image data from thumbnails:
		_options.isSingleLink = view[0].tagName == "A";
        var thumbnails = _options.isSingleLink ? view : view.find("a:has(img)");
        var viewImages = new Array();

        var imageIndex = 0;
        thumbnails.each(function(i, thumbnail){
            var $thumbnail = $(thumbnail);
            var imageData = getImageDataFromThumbnail($thumbnail, _options);
            if (imageData != null)
            {
                viewImages.push(imageData);
                if (_options.isSingleLink)
                    $thumbnail.data("yoxview").imageIndex = imageIndex;
                else
                    $thumbnail.data("yoxview", { "imageIndex": imageIndex });
                imageIndex++;
            }
        });

        if (_options.images)
            viewImages = viewImages.concat(_options.images);

        if (_options.dataSource)
        {
            Yox.dataSources[_options.dataSource].getImagesData(_options, function(data){
                viewImages = viewImages.concat(data.images);
                view.data("yoxview").images = viewImages;

                var thumbnailsData = data.isGroup 
                    ? [$.extend(data, {
                        media: {
                            title: data.title + " (" + data.images.length + " images)",
                            alt: data.title
                        }
                    })]
                    : data.images;
					
                createThumbnails(view, _options.isSingleLink ? null : thumbnailsData, !data.createGroups ? null :
                    function(e){
                        var viewData = $(e.currentTarget).data("yoxview");
                        var thumbnail = $(e.currentTarget);
                        var thumbnailData = thumbnail.data("yoxthumbs");
                        if (!viewData.imagesAreSet)
                        {
                            thumbnail.css("cursor", "wait");
                            var newOptions = $.extend({}, options);
                            if (!newOptions.dataSourceOptions)
                                newOptions.dataSourceOptions = thumbnailData;
                            else
                                $.extend(newOptions.dataSourceOptions, thumbnailData);

                            Yox.dataSources[options.dataSource].getImagesData(newOptions, function(data){
                                viewData.images = data.images;
                                viewData.imagesAreSet = true;
                                thumbnail.css("cursor", "");
                                yoxviewApi.openGallery(viewData.viewIndex);
                            });
                        }
                        else
                        {
                            yoxviewApi.openGallery(viewData.viewIndex);
                        }
                    }
                );
				if (data.createGroups)
				    $.each(view.yoxthumbs("thumbnails"), function(i, thumbnail){
					    thumbnail.data("yoxview", {viewIndex: views.length});
					    views.push(thumbnail);
				    });
                else
                {
                    $.each(view.yoxthumbs("thumbnails"), function(i, thumbnail){
                        var currentViewIndex = imageIndex + i;
                		viewImages[currentViewIndex].thumbnailImg = thumbnail.children("img").eq(0);
					    thumbnail.data("yoxview", {imageIndex: currentViewIndex });
					});
				}
                if (!yoxviewApi.firstViewWithImages && data.images.length > 0)
                {
                    yoxviewApi.firstViewWithImages = view;
                    
                    if (_options.cacheImagesInBackground)
                        yoxviewApi.startCache();
                }
            });
        }
        else
		{
			view.data("yoxview").images = viewImages;
            createThumbnails(view);
		}
    }

    function createThumbnails(view, additionalImages, onClick)
    {
        var clickHandler = function(e){
            var data = $(e.currentTarget).data("yoxview");
            if (!data || data.imageIndex === null)
                return true;
            else
            {
                e.preventDefault();
                yoxviewApi.openGallery($(e.liveFired || e.currentTarget).data("yoxview").viewIndex, data.imageIndex);
            }
        };
        
        if (view[0].tagName == "A")
            view.bind("click.yoxview", clickHandler);
        else if (!additionalImages)
            view.delegate("a:has(img)", "click.yoxview", clickHandler);
        else
            view.yoxthumbs({ 
                images: additionalImages,
                enableOnlyMedia: false,
                onClick: onClick || function(e){
                    e.preventDefault();
				    if (options.thumbnailsOptions && options.thumbnailsOptions.onClick)
                        options.thumbnailsOptions.onClick(
                            $(e.currentTarget).data("yoxview").imageIndex, 
                            $(e.currentTarget),
                            $(e.liveFired).data("yoxview").viewIndex);
                    else
                        yoxviewApi.openGallery($(e.liveFired || e.currentTarget).data("yoxview").viewIndex,
                            $(e.currentTarget).data("yoxview").imageIndex);

                    return false;
                }
            });
    }
    function setThumbnail(setToPopupImage)
    {
        var currentView = $(views[currentViewIndex]);
        thumbnail = currentView[0].tagName == "A"
            ? currentView
            : images[currentItemIndex].thumbnailImg;

        if (!thumbnail || thumbnail.length == 0)
            thumbnail = images[0].thumbnailImg;

        if (thumbnail)
        {
            if (setToPopupImage && image1)
                image1.attr("src", thumbnail.attr("src"));

            thumbnailPos = thumbnail.offset();
            thumbnailProperties = {
                width: thumbnail.width(), 
                height: thumbnail.height(), 
                top: thumbnailPos.top - $(window).scrollTop(), 
                left: thumbnailPos.left 
            };
        }
    }
    
//    Opens the viewer popup.
//    Arguments:
//    viewIndex: The 0-based index of the view to open, in case there are multiple instances of YoxView on the same page. Default is 0.
//    imageIndex: The 0-based index of the image to open, in the specified view. Default is 0.
//    callBack: A function to call after the gallery has opened.
    this.openGallery = function(viewIndex, initialItemIndex, callBack)
    {
        if (typeof(viewIndex) == 'function')
        {
            callBack = viewIndex;
            viewIndex = initialItemIndex = 0;
        }
        else if (typeof(initialItemIndex) == 'function')
        {
            callBack = initialItemIndex;
            initialItemIndex = 0;
        }
        viewIndex = viewIndex || 0;
        initialItemIndex = initialItemIndex || 0;

        $(document).delegate('*', 'keydown.yoxview', function(data){
            catchPress(data);
        });
        
        loadViewImages(views[viewIndex]);

        if (!popup && imagesCount != 0)
            createPopup();

        this.selectImage(initialItemIndex);
        popupWrap.stop().css({ opacity: 1 }).fadeIn("slow", function(){ popupWrap.css("opacity", "") });

        if(options.cacheImagesInBackground)
            cacheImages(initialItemIndex);
            
        if (callBack)
            onOpenCallback = callBack;

        return false;
    }

    this.selectImage = function(itemIndex)
    {
        yoxviewApi.currentImage = images[itemIndex];
        currentItemIndex = itemIndex;
        
        setThumbnail(true);
        thumbnail.blur();

        panel1.css({
            "z-index" : "1",
            "width" : thumbnailProperties.width, 
            "height" : thumbnailProperties.height
        });
        panel2.css({
            "display" : "none",
            "z-index" : "2"
        });
        
        firstImage = true;

        popup.css({
            "width" : thumbnailProperties.width,
            "height" : thumbnailProperties.height,
            "top" : thumbnailProperties.top,
            "left" : thumbnailProperties.left
        });
        this.select(itemIndex);
    }
    this.refresh = function()
    {
        resumePlay = isPlaying;

        if (isPlaying)
            stopPlay();

        setImage(currentItemIndex);
        
        if (resumePlay)
            startPlay();
    }
    
//    Displays the specified image and shows the specified button, if specified. Use when the viewer is open.
//    Arguments:
//    imageIndex: The 0-based index of the image to display.
//    pressedBtn: a jQuery element of a button to display momentarily in the viewer. 
//                For example, if the image has been selected by pressing the Next button 
//                on the keyboard, specify the Next button. If no button should be display, leave blank.
    this.select = function(itemIndex, pressedBtn, viewIndex)
    {
        if (typeof pressedBtn === "number")
        {
            viewIndex = pressedBtn;
            pressedBtn = undefined;
        }
        viewIndex = viewIndex || 0;
        
        if (!isResizing)
        {
            if (itemIndex < 0)
                itemIndex = imagesCount - 1;
            else if (itemIndex == imagesCount)
                itemIndex = 0;

            if (!isPlaying && pressedBtn)
                flicker(pressedBtn);
                
            yoxviewApi.currentImage = images[itemIndex];
            currentItemIndex = itemIndex;
            setImage(currentItemIndex);
            
            // Set the cache buffer, if required:
            calculateCacheBuffer();
        
            // Handle event onSelect:
            if (options.onSelect)
                options.onSelect(itemIndex, images[itemIndex]);
        }
    }
    this.prev = function(continuePlaying)
    {
        cacheDirectionForward = false;
        this.select(currentItemIndex - 1, prevBtn);
        if (isPlaying && continuePlaying !== true)
            stopPlay();
    }
    this.next = function(continuePlaying)
    {
        cacheDirectionForward = true;
        this.select(currentItemIndex + 1, nextBtn);
        if (isPlaying && continuePlaying !== true)
            stopPlay();
    }
    this.first = function()
    {
        if (!options.disableNotifications)
            longFlicker("first");
            
        this.select(0);
        if (isPlaying)
            stopPlay();
    }
    this.last = function()
    {
        if (!options.disableNotifications)
            longFlicker("last");
            
        this.select(imagesCount - 1);
        if (isPlaying)
            stopPlay();
    }
    this.play = function()
    {
        if (imagesCount == 1)
            return;
            
        cacheDirectionForward = true;
        
        if (!isPlaying)
        {
            if (!options.disableNotifications)
                longFlicker("play");
                
            startPlay();
        }
        else
        {
            if (!options.disableNotifications)
                longFlicker("pause");
                
            stopPlay();
        }
    }
    function flicker(button)
    {
        if (button.css("opacity") == 0)
            button.stop().animate({ opacity : 0 }, options.buttonsFadeTime, fadeOut(button));
    }
    function longFlicker(notificationName)
    {
        notification.css("background-position", sprites.getBackgroundPosition("notifications", notificationName));
        notification.stop().fadeIn(options.buttonsFadeTime, function(){ 
            $(this).delay(500)
            .fadeOut(options.buttonsFadeTime);
        });
    }
    function fadeIn(button)
    {
        $(button).stop().animate({ opacity : 0 }, options.buttonsFadeTime);
    }
    function fadeOut(button)
    {
        $(button).stop().animate({ opacity : 0.5 }, options.buttonsFadeTime);
    }

    this.close = function()
    {
        this.closeHelp();
        setThumbnail(false);
        resizePopup(thumbnailProperties.width, thumbnailProperties.height, thumbnailProperties.top, thumbnailProperties.left, function(){
            yoxviewApi.isOpen = false;
        });
        hideMenuPanel();
        
        if (infoPanel)
            hideInfoPanel(function(){
                infoText.html("");
            });

        newPanel.animate({
            width: thumbnailProperties.width,
            height: thumbnailProperties.height
        }, options.popupResizeTime, function(){
            newPanel.css("opacity", 1);
        });
		
		popupWrap.stop().fadeOut(1000);
		 
		if (isPlaying)
			stopPlay();
			
		swipePanels();
        if (options.onClose)
            options.onClose();

        $(document).undelegate("*", "keydown.yoxview");
        isResizing = false;
    }
    this.help = function()
    {
        if (this.isOpen)
        {
            if (helpPanel.css("display") == "none")
                helpPanel.css("display", "block").stop().animate({ opacity : 0.8 }, options.buttonsFadeTime);
            else
                this.closeHelp();
        }
    }
    this.closeHelp = function()
    {
        if (helpPanel.css("display") != "none")
        helpPanel.stop().animate({ opacity: 0 }, options.buttonsFadeTime, function(){
                helpPanel.css("display", "none");
            });
    }
    this.clickBtn = function(fn, stopPlaying)
    {
        if (stopPlaying && isPlaying)
            stopPlay();
            
        fn.call(this);
        return false;
    }
    
    function catchPress(e)
    {
        if (yoxviewApi && yoxviewApi.isOpen)
        {
            var pK = keyCodes[e.keyCode];
            var calledFunction = yoxviewApi[keyMappings[pK]];
            if (calledFunction)
            {
                e.preventDefault();
                calledFunction.apply(yoxviewApi);
                return false;
            }
            return true;
        }
        return true;
    }
    
    function createMenuButton(_title, btnFunction, stopPlay)
    {
        var btn = $("<a>", {
            href : "#",
            click : function(){
                return yoxviewApi.clickBtn(yoxviewApi[btnFunction], stopPlay);
            }         
        });
        var btnSpan = $("<span>" + _title + "</span>");
        btnSpan.css("opacity", "0")
        .appendTo(btn);

        btn.append(sprites.getSprite("icons", btnFunction));
        return btn;
    }

    // Prev and next buttons:
    function createNavButton(_function, _side, singleImage)
    {      
        var navBtnImg = new Image();
        navBtnImg.src = options.imagesFolder + _side + ".png";
        var navBtn = $("<a>", {
            css : {
                "background" : "url(" + navBtnImg.src + ") no-repeat " + _side + " center",
                "opacity" : "0",
                "outline" : "0"
            },
            className : "yoxview_ctlBtn",
            href : "#"
        });
        
        navBtn.css(_side, "0");
        if (!singleImage)
        {
            navBtn.click(function(){
                this.blur();
                return yoxviewApi.clickBtn(_function, true);
            });
            
            if (options.buttonsFadeTime != 0)
            {
                navBtn.hover(
                    function(){
                        if (yoxviewApi.isOpen)
                            $(this).stop().animate({ opacity : 0.6 }, options.buttonsFadeTime);
                    },
                    function(){
                        $(this).stop().animate({ opacity : 0 }, options.buttonsFadeTime);
                    }
                );
            }
        }
        else
            navBtn.css("cursor", "default");
            
        return navBtn;
    }
    
    // INIT:

    this.AddViews(_views, options);

    $(window).bind("resize.yoxview", function()
    {
        windowDimensions = getWindowDimensions();
        if (yoxviewApi.isOpen)
            yoxviewApi.resize();
    });
    
    function calculateMargins()
    {
        var margins = typeof(options.popupMargin) == "number" ? [String(options.popupMargin)] : options.popupMargin.split(" ", 4);
        popupMargins.top = parseInt(margins[0]);
        if (margins.length == 1)
        {
            popupMargins.bottom = popupMargins.right = popupMargins.left = popupMargins.top;
        }
        else if (margins.length == 2)
        {
            popupMargins.bottom = popupMargins.top;
            popupMargins.right = popupMargins.left = parseInt(margins[1]);
        }
        else if (margins.length == 3)
        {
            popupMargins.bottom = parseInt(margins[2]);
            popupMargins.right = popupMargins.left = parseInt(margins[1]);
        }
        else if (margins.length == 4)
        {
            popupMargins.right = parseInt(margins[1]);
            popupMargins.bottom = parseInt(margins[2]);
            popupMargins.left = parseInt(margins[3]);
        }
        popupMargins.totalHeight = popupMargins.top + popupMargins.bottom;
        popupMargins.totalWidth = popupMargins.left + popupMargins.right;
    }

    function createPopup()
    {
        calculateMargins();
        windowDimensions = getWindowDimensions();
        
        currentLanguage = yoxviewLanguages[options.lang];
        var skin = options.skin ? yoxviewSkins[options.skin] : null;
        
        popup = $("<div>", {
            id: 'yoxview',
            click: function(e){ e.stopPropagation(); }
        });
        
        popupWrap = $("<div>", {
            id: "yoxview_popupWrap",
            click: function(e){ e.preventDefault(); yoxviewApi.clickBtn(yoxviewApi.close, true); }
        });
        
        if (options.skin)
            popupWrap.attr("className", "yoxview_" + options.skin);
            
        if (options.backgroundOpacity === 0)
            popupWrap.css("background", "none")
        else if (Yox.Support.rgba())
            popupWrap.css("background-color", Yox.hex2rgba(options.backgroundColor, options.backgroundOpacity));

        popupWrap.appendTo(options.parentElement).append(popup);
        
		panel1 = $("<div>", {
			className: "yoxview_imgPanel",
			css: {
				"z-index": "2"
			}
		});
		panel2 = $("<div>", {
			className: "yoxview_imgPanel",
			css: {
				"z-index": "1",
				"display": "none"
			}
		});
        // the first image:
        image1 = $("<img />", {
            className : "yoxview_fadeImg",
            css : {
				"display" : "block",
				"width" : "100%",
				"height" : "100%"
			}
        });

        // the second image:
        image2 = $("<img />", {
            className : "yoxview_fadeImg",
            css : {
				"display" : "block",
				"width" : "100%",
				"height" : "100%"
			}
        });
        panel1.data("yoxviewPanel", {image: image1})
		.append(image1).appendTo(popup);
		panel2.data("yoxviewPanel", {image: image2})
		panel2.append(image2).appendTo(popup);
        var singleImage = imagesCount == 1;
        if (singleImage && !images[0].media.title)
            options.renderInfo = false;
            
        // the menu:
        if (options.renderMenu !== false)
        {
            var menuPanelWrap = $("<div>", {
                className : "yoxview_popupBarPanel yoxview_top"
            });

            if (options.autoHideMenu !== false)
            {
                menuPanelWrap.hover(
                    function(){
                        if (yoxviewApi.isOpen)
                            showMenuPanel();
                    },
                    function(){
                        if (yoxviewApi.isOpen)
                            hideMenuPanel();
                    }
                );
            }
            
            menuPanel = $("<div>", {
                id : "yoxview_menuPanel"
            });
            
            if (Yox.Support.rgba() && options.menuBackgroundColor)
                menuPanel.css("background", Yox.hex2rgba(options.menuBackgroundColor, options.menuBackgroundOpacity || 0.8));

            var helpBtn = createMenuButton(currentLanguage.Help, "help", false);

            yoxviewApi.infoButtons.playBtn = createMenuButton(currentLanguage.Slideshow, "play", false);
            playBtnText = yoxviewApi.infoButtons.playBtn.children("span");
            
            menuPanel.append(
                createMenuButton(currentLanguage.Close, "close", true),
                helpBtn,
                yoxviewApi.infoButtons.playBtn
            );
            
            if (singleImage)
            {
                yoxviewApi.infoButtons.playBtn.css("display", "none");
                helpBtn.css("display", "none");
                menuPanel.css({
                    width: 58
                });
            }
            
            menuPanel.find("a:last-child").attr("class", "last");
            menuPanelWrap.append(menuPanel).appendTo(popup);
            menuPanel.delegate("a", "mouseenter", function(){
                $(this).stop().animate({ top : "8px" }, "fast").find("span").stop().animate({opacity:1}, "fast");
            })
            .delegate("a", "mouseleave", function(){
                $(this).stop().animate({ top : "0" }, "fast").find("span").stop().animate({opacity:0}, "fast");
            });
        }

        if (options.renderButtons !== false && (!singleImage || !$.support.opacity))
        {
            // prev and next buttons:            
            prevBtn = createNavButton(yoxviewApi.prev, options.isRTL ? "right" : "left", singleImage);            
            nextBtn = createNavButton(yoxviewApi.next, options.isRTL ? "left" : "right", singleImage);

            popup.append(prevBtn, nextBtn);
            
            if (singleImage && !$.support.opacity)
            {
                ctlButtons = $();
                
            }
            else
                ctlButtons = popup.find(".yoxview_ctlBtn");
        }
        else
            ctlButtons = $();        

        // add the ajax loader:
        ajaxLoader = $("<div>", {
            id: "yoxview_ajaxLoader",
            className: "yoxview_notification",
            css: { 
                "display": "none"
            }
        });
        ajaxLoader.append($("<img>", {
            src: options.imagesFolder + "popup_ajax_loader.gif",
            alt: currentLanguage.Loading,
            css: {
                width: 32,
                height: 32,
                "background-image": "url(" + options.imagesFolder + "sprites.png)",
                "background-position": sprites.getBackgroundPosition("notifications", "empty")
            }
        }))
        .appendTo(popup);
        
        // notification image
        if (!options.disableNotifications)
        {
            notification = $("<img>", {
                className: "yoxview_notification"
            });
            popup.append(notification);
        }
        
        // help:
        helpPanel = $("<div>", {
            id : "yoxview_helpPanel", 
            href : "#", 
            title : currentLanguage.CloseHelp,
            css : {
                "background" : "url(" + options.imagesFolder + "help_panel.png) no-repeat center top",
                "direction" : currentLanguage.Direction,
                "opacity" : "0"
            },
            click : function(){
                return yoxviewApi.clickBtn(yoxviewApi.help, false);
            }
        });
        
        var helpTitle = document.createElement("h1");
        helpTitle.innerHTML = currentLanguage.Help.toUpperCase();

        var helpText = document.createElement("p");
        helpText.innerHTML = currentLanguage.HelpText;
        
        var closeHelp = document.createElement("span");
        closeHelp.id = "yoxview_closeHelp";
        closeHelp.innerHTML = currentLanguage.CloseHelp;
        
        helpPanel.append(helpTitle).append(helpText).append(closeHelp).appendTo(popup);
        
        // popup info:
        if (options.renderInfo !== false)
        {
            infoPanel = $("<div>", {
                id: "yoxview_infoPanel",
                click: function(e){ e.stopPropagation(); }
            });

            if (options.infoBackOpacity === 0)
            {
                infoPanel.css("background", "none");
                infoPanelContent = infoPanel;
            }
            else
            {
                if (Yox.Support.rgba())
                {
                    infoPanelContent = infoPanel;
                    infoPanel.css("background-color", Yox.hex2rgba(options.infoBackColor, options.infoBackOpacity));
                }
                else
                {
                    infoPanel.append(
                        $("<div>", {
                            id : "yoxview_infoPanelBack",
                            css : {
                                "background" : options.infoBackColor,
                                "opacity" : options.infoBackOpacity
                            }
                        })
                    );
                    infoPanelContent = $("<div>", {
                        id: "yoxview_infoPanelContent"
                    });
                }
            }
            countDisplay = $("<span>", {
                id: "yoxview_count"
            });
            
            infoText = $("<div>", {
                id: "yoxview_infoText"
            });
            
            if (singleImage)
            {
                infoText.css("margin-left", "10px");
                countDisplay.css("display", "none");
            }
            infoPanelContent.append(countDisplay);
            
            if (options.renderInfoPin !== false)
            {
                infoPinLinkImg = sprites.getSprite("icons", options.autoHideInfo ? "pin" : "unpin");
                infoPinLink = $("<a>", {
                    className: "yoxviewInfoLink",
                    href: "#",
                    title: options.autoHideInfo ? currentLanguage.PinInfo : currentLanguage.UnpinInfo,
                    css: { display: 'inline' },
                    click: function(e){
                        e.preventDefault();
                        options.autoHideInfo = !options.autoHideInfo;
                        infoPinLinkImg.css("background-position", sprites.getBackgroundPosition("icons", options.autoHideInfo ? "pin" : "unpin"));
                        this.title = options.autoHideInfo ? currentLanguage.PinInfo : currentLanguage.UnpinInfo;
                    }
                });
                infoPinLink.append(infoPinLinkImg).appendTo(infoPanelContent);
            }   

            if (skin && skin.infoButtons)
            {
                var skinButtons = skin.infoButtons(options, currentLanguage, sprites, popupWrap, popup);
                if (options.infoButtons)
			        $.extend(options.infoButtons, skinButtons);
			    else
			        options.infoButtons = skinButtons;
			}
			if (options.infoButtons)
			{  
				$.extend(yoxviewApi.infoButtons, options.infoButtons);
				for (infoButton in options.infoButtons)
				{
					options.infoButtons[infoButton].attr("className", "yoxviewInfoLink").css("display", "block").appendTo(infoPanelContent);
				}
			}
            
            if (options.linkToOriginalContext !== false)
            {
                infoPanelLink = $("<a>", {
                    className: "yoxviewInfoLink",
                    target: "_blank",
                    title: currentLanguage.OriginalContext
                });
                infoPanelLink.append(sprites.getSprite("icons", "link")).appendTo(infoPanelContent);
            }
            
            infoPanelContent.append(infoText);
            if (!Yox.Support.rgba())
                infoPanel.append(infoPanelContent);
                
            infoPanel.appendTo(options.renderInfoExternally ? popupWrap : popup);
            
            if (!options.renderInfoExternally)
            {
                infoPanelWrap = $("<div>", {
                    className : "yoxview_popupBarPanel yoxview_bottom"
                });
                
                infoPanelWrap.hover(
                    function(){
                        if (yoxviewApi.isOpen && !disableInfo && options.autoHideInfo !== false)
                            setInfoPanelHeight();
                    },
                    function(){
                        if (yoxviewApi.isOpen && !disableInfo && options.autoHideInfo !== false)
                            hideInfoPanel();
                    }
                );
                infoPanel.wrap(infoPanelWrap);
                infoPanelWrap = infoPanel.parent();
            }
        }        
        // set the background if no RGBA support found:
        if (!Yox.Support.rgba())
        {
            popupBackground = $("<div>", {
                css : {
                    "position" : "fixed",
                    "height" : "100%",
                    "width" : "100%",
                    "top" : "0",
                    "left" : "0",
                    "background" : options.backgroundColor,
                    "z-index" : "1",
                    "opacity" : options.backgroundOpacity
                }
            }).appendTo(popupWrap);
        }
    }
    
    $(cacheImg).load(function()
    {
        $.extend(images[currentCacheImg].media, {
            width: this.width,
            height: this.height,
            loaded: true
        });
        advanceCache();
    })
    .error(function(){
        advanceCache();
	});
	
	function advanceCache()
	{
	    cachedImagesCount++;
        if (cachedImagesCount == imagesCount)
            cacheComplete = true;

        if (!cacheComplete)
            getCacheBuffer();
	}
    this.startCache = function()
    {
        loadViewImages(this.firstViewWithImages);
        calculateCacheBuffer();
        cacheImages(0);
    }
    function getCacheBuffer()
    {
        if (!options.cacheBuffer || currentCacheImg != cacheBufferLastIndex)
            cacheImages(currentCacheImg + (cacheDirectionForward ? 1 : -1));
    }
    function calculateCacheBuffer()
    {
        if (options.cacheBuffer)
        {
            cacheBufferLastIndex = cacheDirectionForward ? currentItemIndex + options.cacheBuffer : currentItemIndex - options.cacheBuffer;
            if (cacheBufferLastIndex < 0)
                cacheBufferLastIndex += imagesCount;
            else if (cacheBufferLastIndex >= imagesCount)
                cacheBufferLastIndex -= imagesCount;
        }
    }
    function cacheImages(imageIndexToCache)
    {
        if (cacheComplete)
            return;

        if (imageIndexToCache == imagesCount)
            imageIndexToCache = 0;
        else if (imageIndexToCache < 0)
            imageIndexToCache += imagesCount;
            
        var image = images[imageIndexToCache].media;
        currentCacheImg = imageIndexToCache;
        if (image && !image.loaded)
        {
            if (image.contentType === "image" || !image.contentType)
                cacheImg.src = image.src;
            else
                loadMedia(image, function(){
                    advanceCache();
                });
        }
        else
            getCacheBuffer();
    }
    
    function showLoaderIcon()
    {
        loading = true;
        clearTimeout(loaderTimeout);
        ajaxLoader.stop();
        loaderTimeout = setTimeout(
            function()
            {
                ajaxLoader.css("opacity", "0.6").fadeIn(options.buttonsFadeTime);
            },
            options.buttonsFadeTime
        );
    }

    function hideLoaderIcon()
    {
        loading = false;
        clearTimeout(loaderTimeout);
        ajaxLoader.stop().fadeOut(options.buttonsFadeTime);
    }

    function setImage(itemIndex)
    {
        if (!isPlaying)
        {
            showLoaderIcon();
        }
        loadAndDisplayMedia(yoxviewApi.currentImage.media);
    }
    
    function resizePopup(_width, _height, _top, _left, callBack)
    {
        popup.stop().animate({
            width: _width,
            height: _height,
            top: _top,
            left: _left
        }, options.popupResizeTime, callBack);
    }
    function startPlay()
    {
        if (imagesCount == 1)
            return;

        isPlaying = true;
        if(playBtnText)
            playBtnText.text(currentLanguage.Pause);
        else if (yoxviewApi.infoButtons.playBtn)
            yoxviewApi.infoButtons.playBtn.attr("title", currentLanguage.Pause);
        
        if (yoxviewApi.infoButtons.playBtn)
            yoxviewApi.infoButtons.playBtn.find("img").css("background-position", sprites.getBackgroundPosition("icons", "pause"));
            
        if (currentItemIndex < imagesCount - 1)
        {
            popupTimeout = setTimeout(
                function(){
                    yoxviewApi.next(true);
                },
                options.playDelay
            );
        }
        else
        {
            if (options.loopPlay)
                popupTimeout = setTimeout(
                    function(){
                        yoxviewApi.select(0, null);
                    },
                    options.playDelay
                );
            else
                stopPlay();
        }
    }
    function stopPlay()
    {
        clearTimeout(popupTimeout);
        isPlaying = false;
        if(playBtnText)
            playBtnText.text(currentLanguage.Play);
        else if (yoxviewApi.infoButtons.playBtn)
            yoxviewApi.infoButtons.playBtn.attr("title", currentLanguage.Play);
            
        if (yoxviewApi.infoButtons.playBtn)
            yoxviewApi.infoButtons.playBtn.find("img").css("background-position", sprites.getBackgroundPosition("icons", "play"));
    }

    function blink(_element)
    {
        _element.animate({ opacity : 0.8 }, 1000, function()
        {
            $(this).animate({opacity: 0.2}, 1000, blink($(this)));
        });
    }
    
    var newPanel = panel2;
    var oldPanel = panel1;
    
    function getWindowDimensions()
    {
        var widthVal = $(parent.window).width();
        var heightVal = $(parent.window).height();
        var returnValue = {
            height : heightVal,
            width : widthVal,
            usableHeight : heightVal - popupMargins.totalHeight,
            usableWidth : widthVal - popupMargins.totalWidth
        };
        return returnValue;
    }
    
    function getImagePosition(imageSize)
    {
        var imagePosition = (imageSize.width && imageSize.height)
            ? Yox.fitImageSize(imageSize, {width: windowDimensions.usableWidth, height: windowDimensions.usableHeight })
            : { 
                width: imageSize.width ? Math.min(imageSize.width, windowDimensions.usableWidth) : windowDimensions.usableWidth,
                height: imageSize.height ? Math.min(imageSize.height, windowDimensions.usableHeight) : windowDimensions.usableHeight
            };

        imagePosition.top = popupMargins.top + Math.round((windowDimensions.usableHeight - imagePosition.height) / 2);
        imagePosition.left = popupMargins.left + Math.round((windowDimensions.usableWidth - imagePosition.width) / 2);
        
        return imagePosition;
    }
    this.resize = function()
    {
        if (isPlaying)
        {
            resumePlay = true;
            stopPlay();
        }

        var newImagePosition = getImagePosition(currentMaxSize);
        newPanel.css({"width" : "100%", "height" : "100%"});
        
        isResizing = true;
        if (!isImageMode)
            ctlButtons.css({top: Math.round((newImagePosition.height - mediaButtonsSize.height) / 2)});

        resizePopup(
            newImagePosition.width,
            newImagePosition.height,
            newImagePosition.top,
            newImagePosition.left,
            function(){
                var newImageWidth = popup.width();
                var newImageHeight = popup.height();

                if (currentMaxSize.padding)
                {
                    newImageWidth -= currentMaxSize.padding.horizontal;
                    newImageHeight -= currentMaxSize.padding.vertical;
                }
                newPanel.css({ "width" : newImageWidth + "px", "height" : newImageHeight + "px" });
                isResizing = false;

                if (infoPanel)
                    setInfoPanelHeight();
                
                if (resumePlay)
                {
                    startPlay();
                    resumePlay = false;
                }
            }
        );
    }

    function setInfoPanelHeight(callback)
    {
        clearTimeout(hideInfoTimeout);
        var titleHeight = infoText.outerHeight();

        if (titleHeight < infoPanelMinHeight)
            titleHeight = infoPanelMinHeight;
        
        infoPanel.stop().animate({height : titleHeight}, 500, function(){ 
            if (callback)
                callback();
        });
    }
    function hideInfoPanel(callback)
    {
        clearTimeout(hideInfoTimeout);
        infoPanel.stop().animate({ height: 0 }, 500, function(){
            if (callback)
                callback();
        });
    }
    function hideMenuPanel(callback)
    {
        if (menuPanel)
        {
            clearTimeout(hideMenuTimeout);
            menuPanel.stop().animate({ top: menuHidePosition }, 500, function(){
                if (callback)
                    callback();
            });
        }
    }
    function showMenuPanel(callback)
    {
        if (menuPanel)
        {
            clearTimeout(hideMenuTimeout);
            menuPanel.stop().animate({ top: 0 }, 500, function(){
                if (callback)
                    callback();
            });
        }
    }
    
    function swipePanels()
    {
        oldPanel = newPanel;
	    newPanel = isFirstPanel ? panel2 : panel1;
	    isFirstPanel = !isFirstPanel;
    }
	function changeMedia(media)
	{
	    var mediaIsImage = media.contentType === "image" || !media.contentType;
	    
	    if (mediaIsImage && disableInfo && infoPanelWrap)
	        infoPanelWrap.css("display", "block");
	        
	    clearTimeout(hideInfoTimeout);
	    
	    swipePanels();
	    var panelData = newPanel.data("yoxviewPanel");
	    
	    currentMaxSize.width = media.width;
	    currentMaxSize.height = media.height;
	    currentMaxSize.padding = media.padding;

	    if (infoPanel)
        {
            var infoTextValue = media.title || "";
            if (media.description)
                infoTextValue += infoTextValue != ""
                    ? "<div id='yoxview_infoTextDescription'>" + media.description + "</div>"
                    : media.description;

            infoText.html(infoTextValue);
            
            if (imagesCount > 1)
                countDisplay.html(currentItemIndex + 1 + "/" + imagesCount);
            
            if (infoPanelLink)
            {
                if (yoxviewApi.currentImage.link)
                    infoPanelLink.attr("href", yoxviewApi.currentImage.link).css("display", "inline");
                else
                    infoPanelLink.css("display", "none");
            }
        }
        
        var newImagePosition = getImagePosition(media);
	    if (mediaIsImage)
	    {
	        currentImageElement = isFirstPanel ? image1 : image2;
		    currentImageElement.attr({
			    src : media.src,
			    title : media.title,
			    alt: media.alt
		    });
			
			panelData.image = currentImageElement;
			
		    // change to image mode:
		    if (!panelData.isImage && panelData.element)
		    {
		        panelData.element.css("display", "none");
                panelData.image.css("display", "block");
		        panelData.isImage = true;
		    }
		    
		    if (!isImageMode)
		    {
		        if (options.renderButtons)
		            ctlButtons.css({"height": "100%", "width": "50%", "top": "0"});
		            
		        disableInfo = false;
		        isImageMode = true;
		    }
		}
		else
        {
            if (panelData.element && panelData.elementId != media.elementId)
            {
                panelData.element.remove();
                panelData.element = undefined;
            }
            if (!panelData.element)
            {
                if (media.html)
                {
                    panelData.element = $("<div>", {
	                    className: mediaPanelClass
	                });
	                popup.append(panelData.element);
                }
                else
                {
                    popup.append(media.element);
                    panelData.element = media.element;
                }
            }

            if (media.html)
                panelData.element.html(media.html);

            newPanel = panelData.element;
            
            if (isImageMode)
            {
                if (infoPanelWrap)
		        {
		            if (options.autoHideInfo !== false)
		                hideInfoPanel();
        			    
		            infoPanelWrap.css("display", "none");
		            disableInfo = true;
		        }
		        
		        if (options.renderButtons)
		            ctlButtons.css({
			            "width": mediaButtonsSize.width,
			            "height": mediaButtonsSize.height
			        });
			    
                isImageMode = false;
            }
            
            if (options.renderButtons)
                ctlButtons.css({top: (newImagePosition.height - mediaButtonsSize.height) / 2 });

            // change to element mode:
            if (panelData.isImage === undefined || panelData.isImage)
            {
                panelData.element.css("display", "block");
                panelData.image.css("display", "none");
                panelData.isImage = false;
            }
        }
        
        if (firstImage)
            newPanel.animate({
                width: newImagePosition.width,
                height: newImagePosition.height
            }, options.popupResizeTime);
        else
            newPanel.css({
                width : newImagePosition.width,
                height : newImagePosition.height
            });
              
        if (loading)
            hideLoaderIcon();

        isResizing = true;
        resizePopup(
            newImagePosition.width,
            newImagePosition.height,
            newImagePosition.top,
            newImagePosition.left,
            function()
            {
                if (firstImage)
                {
                    yoxviewApi.isOpen = true;

                    if (options.controlsInitialDisplayTime > 0)
                    {
                        if (options.showButtonsOnOpen)
                            ctlButtons.animate({opacity: 0.5}, options.controlsInitialFadeTime, function(){ 
                                if(options.buttonsFadeTime != 0)
                                    $(this).delay(options.controlsInitialDisplayTime).animate({opacity : 0}, options.controlsInitialFadeTime);
                            });
                        
                        if (options.showBarsOnOpen)
                        {
                            showMenuPanel(function(){
                                if (options.autoHideMenu !== false)
                                    hideMenuTimeout = setTimeout(function(){ 
                                            hideMenuPanel();
                                        }, 
                                        options.controlsInitialDisplayTime
                                    );
                            });
                            if (infoPanel)
                                setInfoPanelHeight(function(){
                                    if (options.autoHideInfo !== false)
                                        hideInfoTimeout = setTimeout(function(){ hideInfoPanel(); }, options.controlsInitialDisplayTime);
                                });
                        }
                    }

                    if (options.autoPlay)
                        yoxviewApi.play();

                    if (options.onOpen)
                        options.onOpen();
                        
                    if (onOpenCallback)
                    {
                        onOpenCallback();
                        onOpenCallback = undefined;
                    }
            
                    firstImage = false;
                }
                
                if (currentMaxSize.padding)
                {
                    var newImageWidth = popup.width();
                    var newImageHeight = popup.height();

                    if (currentMaxSize.padding)
                    {
                        newImageWidth -= currentMaxSize.padding.horizontal;
                        newImageHeight -= currentMaxSize.padding.vertical;
                    }
                    newPanel.css({ "width" : newImageWidth + "px", "height" : newImageHeight + "px" });                    
                }
                isResizing = false;
            }
        );

        newPanel.css({'z-index': '2', opacity: 1});
        if (oldPanel)
            oldPanel.css('z-index', '1');
        
        newPanel.fadeIn(options.popupResizeTime, function(){
            if (oldPanel)
                oldPanel.css('display', 'none');
                
            if (infoPanel)
                setInfoPanelHeight(function(){
                    if (options.autoHideInfo !== false)
                        hideInfoTimeout = setTimeout(function(){ hideInfoPanel(); }, options.titleDisplayDuration);
                });

            if (imagesCount > 1)
            {
                if (options.cacheImagesInBackground && !cacheComplete)
                        cacheImages(currentItemIndex + (cacheDirectionForward ? 1 : -1));

                if (isPlaying)
                    startPlay();
            }
        });
	}
    $(tempImg).load(function()
    {
		if (this.width == 0)
		{
		    displayError("Image error");
            return;
        }
        changeMedia($.extend({}, yoxviewApi.currentImage.media, {
            width: this.width,
            height: this.height
        }));
    })
    .error(function(){
        displayError("Image not found:<br /><span class='errorUrl'>" + this.src + "</span>");
    });

    function loadMediaFromProvider(provider, url, availableSize, onLoad, onError)
    {
        jQuery.jsonp({
            url: (mediaProviderUrls[provider] || "http://oohembed.com/oohembed/"),
            data: jQuery.extend({
                "url" : url,
                "format": "json"
            }, availableSize),
            dataType: 'jsonp',
            callbackParameter: "callback",
            success: function(data)
            {
                var media = {
                    title: data.title,
                    width: data.width,
                    height: data.height,
                    type: data.type
                };
                
                if (data.type === "video")
                {
                    media.html = data.html
                        .replace(/<embed /, "<embed wmode=\"transparent\" ")
                        .replace(/<param/, "<param name=\"wmode\" value=\"transparent\"><param")
                        .replace(/width=\"[\d]+\"/ig, "width=\"100%\"")
                        .replace(/height=\"[\d]+\"/ig, "height=\"100%\"");
                }
                else if (data.type === "photo")
                {
                    jQuery.extend(media, {
                        src: data.url,
                        alt: data.title,
                        type: "image"
                    });
                }
                onLoad(media);
            },
            error: function(errorSender, errorMsg){
                if (onError)
                    onError(errorSender, errorMsg);
            }
        });
    };

    function loadAndDisplayMedia(media)
    {
        try
        {
            if (!media)
                throw("Error: Media is unavailable.");

            if (media.contentType === "image" || !media.contentType)
            {
                // Resets the src attribute for the image - avoids a rendering problem in Chrome.
                // $.opacity is tested so this isn't applied in IE (up to IE8), 
                // since it creates a problem with the image's fading:
                if ($.support.opacity)
                    tempImg.src = "";

                tempImg.src = media.src;
            }
            else
            {
                if (!media.loaded && media.contentType == "ooembed")
                {
                    loadMedia(
                        media, 
                        function(loadedMedia){
                            changeMedia(loadedMedia);
                        },
                        function(errorSender)
                        {
                            displayError("Error getting data from:<br /><span class='errorUrl'>" + errorSender.data.url + "</span>");
                        }
                    );
		        }
		        else
		            changeMedia(yoxviewApi.currentImage.media);
		    }
		}
		catch(error)
		{
		    displayError(error);
		}
    }
    function loadMedia(media, onLoad, onError)
    {
        if (media.contentType == "ooembed")
        {
	        loadMediaFromProvider(
	            media.provider,
	            media.url,
	            options.videoSize,
	            function(mediaData){
	                $.extend(media, mediaData, {loaded: true});
	                if (onLoad)
	                    onLoad(media);
	            },
	            onError
            );
        }
    }
    function displayError(errorMsg)
    {
        changeMedia({
            html: "<span class='yoxview_error'>" + errorMsg + "</span>",
            width: 500,
            height: 300,
            type: "error",
            title: ""
        });
    }
    this.unload = function(){
        jQuery.each(views, function(i, view){
            var $view = $(view);
            $view.undelegate("a", "click.yoxview")
            .removeData("yoxview")
            .yoxthumbs("unload", "yoxview");
        });
        
        $(document).undelegate("*", "keydown.yoxview");
        $(window).unbind(".yoxview");
        
        if (popup){
            popupWrap.remove();
            popup = undefined;
        }
    };
}