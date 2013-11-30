/*!
 * jquery.yoxthumbs v0.9
 * jQuery thumbnails plugin
 * http://yoxigen.com/
 *
 * Copyright (c) 2010 Yossi Kolesnicov
 *
 * Date: 13th July, 2010
 * Version : 0.91
 */
(function($){
    $.fn.yoxthumbs = function(opt) {
		if (this.length == 0)
            return this;
			
        if (typeof(opt) != 'string')
        {
            var defaults = {
                target: null, // an yoxview instance
                selectedThumbnailClassName : "selected",
                thumbsOpacityFadeTime: 300,
                thumbsOpacity : undefined,
                prevBtn: undefined,
                nextBtn: undefined,
                onClick: undefined,
                images: undefined,
                enableOnlyMedia: false // If set to true, YoxThumbs is enabled only for links whose HREF is to an image or other supported media.
            };
            
            var options = $.extend(defaults, opt);
            var $this = $(this);
            $this.data("yoxthumbs", new YoxThumbs($this, options));
            return this;
        }
        else
        {
            var instance = $(this).data("yoxthumbs");
            if (instance)
            {
                if ($.isFunction(instance[opt]))
                    instance[opt].apply(instance, Array.prototype.slice.call(arguments, 1));
                else
                    return instance[opt];
            }
            return this;
        }
    };
    function YoxThumbs(container, options)
    {
        var self = this;
        var prevBtn = options.prevBtn;
        var nextBtn = options.nextBtn;
        var viewIndex = container.data("yoxview") ? container.data("yoxview").viewIndex : undefined;
        var $ = jQuery;
		var containerIsAnchor = container[0].tagName == "A";
			
        // If images data has been specified, create the thumbnails:
        if (options.images)
            jQuery.each(options.images, function(i, imageData){
                container.append(createThumbnail(imageData));
            });

        this.thumbnails = [];
        var foundThumbnails = containerIsAnchor ? container : container.find("a:has(img)");
        var currentImageIndex = 0;

        $.each(foundThumbnails, function(i, thumbnail)
        {
            var $thumbnail = $(thumbnail);
            var addThumb = true;
            if (options.enableOnlyMedia)
            {
                if (!thumbnail.href.match(Yox.Regex.image))
                {
                    var isData = false;
                    for(dataProvider in Yox.Regex.data)
                    {
                        if (thumbnail.href.match(Yox.Regex.data[dataProvider]))
                        {
                            isData = true;
                            break;
                        }
                    }
                    if (!isData)
                    {
                        var isVideo = false;
                        for(videoProvider in Yox.Regex.video)
                        {
                            if (thumbnail.href.match(Yox.Regex.video[videoProvider]))
                            {
                                isVideo = true;
                                break;
                            }
                        }
                        if (!isVideo)
                            addThumb = false;
                    }
                }
            }
            
            if (addThumb)
            {
                $thumbnail.data("yoxthumbs", $.extend({imageIndex: currentImageIndex++}, $thumbnail.data("yoxthumbs")));
                self.thumbnails.push($thumbnail);
            }
        });

        if (options.thumbsOpacity)
        {
            this.thumbnails.css("opacity", options.thumbsOpacity);
            container.delegate("a:has(img)", "mouseenter.yoxthumbs", function(e){
                if (self.currentSelectedIndex === undefined || 
                    $(e.currentTarget).data("yoxthumbs").imageIndex != self.currentSelectedIndex){
                    $(e.currentTarget).stop().animate({opacity: 1}, options.thumbsOpacityFadeTime);
                }
            })
            .delegate("a:has(img)", "mouseout.yoxthumbs", function(e){
                if (self.currentSelectedIndex === undefined || 
                    $(e.currentTarget).data("yoxthumbs").imageIndex != self.currentSelectedIndex)
                    $(e.currentTarget).stop().animate({opacity: options.thumbsOpacity}, options.thumbsOpacityFadeTime);
            });
        }
        if (options.onClick)
        {
			if (containerIsAnchor)
				container.bind("click.yoxthumbs", function(e){
					options.onClick(e);
					return false;
				});
			else
				container.delegate("a:has(img)", "click.yoxthumbs", function(e){
				    if (!$(e.currentTarget).data("yoxthumbs"))
				        return true;
				        
					options.onClick(e);
					return false;
				});
        }
        
        function createThumbnail(imageData, viewIndex)
        {
            var thumbnail = jQuery("<a>", {
                href: imageData.link
            });
            
            var thumbImage = jQuery("<img>", {
                src : imageData.thumbnailSrc,
                alt : imageData.media.alt,
                title : imageData.media.title
            });
            
            if (imageData.data)
                thumbnail.data("yoxthumbs", imageData.data);
                
            if (imageData.thumbnailDimensions)
                thumbImage.css({
                    "width": imageData.thumbnailDimensions.width,
                    "height" : imageData.thumbnailDimensions.height
                });
            thumbImage.appendTo(thumbnail);
            
            return thumbnail;
        }
        
        // Selects a thumbnail
        this.select = function(thumbIndex)
        {
            if (this.currentSelectedIndex === undefined || this.currentSelectedIndex != thumbIndex)
            {
                var currentThumbnail = this.thumbnails.eq(thumbIndex);
                var yoxslider = container.data("yoxslide");
                if (yoxslider)
                    yoxslider.show(currentThumbnail);

                // Remove selection from previous thumbnail:
                if (this.currentSelectedIndex !== undefined)
                {
                    var previousSelectedThumbnail = this.thumbnails.eq(this.currentSelectedIndex);
                    previousSelectedThumbnail.removeClass(options.selectedThumbnailClassName);
                    if (options.thumbsOpacity)
                        previousSelectedThumbnail.animate({opacity: options.thumbsOpacity}, options.thumbsOpacityFadeTime);
                        
                }
                
                currentThumbnail.addClass(options.selectedThumbnailClassName);
                if (options.thumbsOpacity)
                    currentThumbnail.animate({opacity: 1}, options.thumbsOpacityFadeTime);
                                        
                this.currentSelectedIndex = thumbIndex;
            }
        }
        this.unload = function(dataKey)
        {
            $.each(this.thumbnails, function(i, thumbnail)
            {
                $(thumbnail).removeData("yoxthumbs");
                if (dataKey)
                    $(thumbnail).removeData(dataKey);
            });
            container.undelegate("a:has(img)", "click.yoxthumbs");
            if (containerIsAnchor)
                container.unbind(".yoxthumbs");
        }
    }
})(jQuery);