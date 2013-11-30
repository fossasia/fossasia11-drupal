/*!
 * Yox Picasa plugin
 * http://yoxigen.com/yoxview/
 *
 * Copyright (c) 2010 Yossi Kolesnicov
 *
 * Licensed under the MIT license.
 * http://www.opensource.org/licenses/mit-license.php
 *
 * Date: 7th May, 2010
 * Version : 1.5
 */
function yox_picasa()
{
    var $ = jQuery;
    var picasaRegex = /http:\/\/picasaweb\.google\.\w+\/([^\/#\?]+)\/?([^\/#\?]+)?(\?([^#]*))?/
    var self = this;
    
    this.getImagesData = function(options, callback)
    {
        var defaults = {
            url: "http://picasaweb.google.com/data/feed/api/",
            setThumbnail: true,
            setSingleAlbumThumbnails: true,
            setTitle: true, // Whether to add a header with user and/or album name before thumbnails
			alt: 'json',
			thumbsize: 64
        };

        var fromDataUrl = {};
        if (options.dataUrl)
        {
            var urlMatch = options.dataUrl.match(picasaRegex);
            if (urlMatch && urlMatch.length > 1)
            {
                fromDataUrl.user = urlMatch[1];
                if (urlMatch[2])
                    fromDataUrl.album = urlMatch[2]
				if (urlMatch[4])
					$.extend(fromDataUrl, Yox.queryToJson(urlMatch[4]));
            }
        }

        var datasourceOptions = jQuery.extend({}, defaults, fromDataUrl, options.dataSourceOptions);
        
		if (datasourceOptions.user && !datasourceOptions.album && !datasourceOptions.q)
			datasourceOptions.thumbsize = 104;
			
        // Picasa web uses 'tags', while the API uses 'tag':
        if (datasourceOptions.tags)
            datasourceOptions.tag = datasourceOptions.tags;
            
        if (datasourceOptions.album == "")
            datasourceOptions.album = null;

        var screenSize = screen.width > screen.height ? screen.width : screen.height;

        var unknownSize = datasourceOptions.imgmax && $.inArray(datasourceOptions.imgmax, picasaImgMaxSizes) == -1 ? datasourceOptions.imgmax : null;

        // Save resources for smaller screens:
        if (!datasourceOptions.imgmax || unknownSize || screenSize < datasourceOptions.imgmax)
            datasourceOptions.imgmax = picasa_getMaxSize(unknownSize || screenSize, picasaImgMaxSizes, datasourceOptions.roundSizeUp);

        var feedUrl = getFeedUrl(datasourceOptions);
        var returnData = {};
        
        if (options.onLoadBegin)
            options.onLoadBegin();

        $.jsonp({
            url: feedUrl,
            async: false,
            dataType: 'jsonp',
			data: datasourceOptions,
            callbackParameter: "callback",
            success: function(data)
            {
                if (!data.feed.entry || data.feed.entry.length == 0)
                {
                    if (options.onNoData)
                        options.onNoData();
                        
                    return;
                }

                var kind = data.feed.entry[0].category[0].term.match(/.*#(.*)/)[1]; // album or photo
                if (kind == "album")
                    $.extend(returnData, {
                        title: data.feed.title.$t,
                        createGroups: true
                    });

                returnData.images = self.getImagesDataFromJson(data, datasourceOptions, kind);

                if (returnData.images.length > 0 && datasourceOptions.setThumbnail && !datasourceOptions.setSingleAlbumThumbnails)
                {
                    $.extend(returnData, {
                        isGroup: true,
                        link: data.feed.link[1].href,
                        thumbnailSrc: data.feed.icon.$t,
						title: data.feed.title.$t
                    });
                }
                
                if (callback)
                    callback(returnData);

                if (options.onLoadComplete)
                    options.onLoadComplete();
            },
            error : function(xOptions, textStatus){
                if (options.onLoadError)
                    options.onLoadError("Picasa plugin encountered an error while retrieving data");
            }
        });
        
        //return returnData;
    }

    var picasaThumbnailSizes = [32, 48, 64, 72, 104, 144, 150, 160];
    var picasaImgMaxSizes = [94, 110, 128, 200, 220, 288, 320, 400, 512, 576, 640, 720, 800, 912, 1024, 1152, 1280, 1440, 1600];

    function getFeedUrl(datasourceOptions)
    {
        var feedUrl = datasourceOptions.url;
		if (datasourceOptions.user && datasourceOptions.user != "lh")
		{
			feedUrl += "user/" + datasourceOptions.user;
			if (datasourceOptions.album)
				feedUrl += "/album/" + datasourceOptions.album;
        }
        else
			feedUrl += "all";

        return feedUrl;
    }
    function picasa_getMaxSize(size, sizesArray, roundSizeUp)
    {
        size = parseInt(size);
        for(var i=sizesArray.length - 1; i >= 0; i--)
        {
            var pSize = sizesArray[i];
            if (size >= pSize)
                return roundSizeUp 
                    ? i < sizesArray.length - 1 ? sizesArray[i + 1] : pSize
                    : pSize;
        }
        
        return size;
    }
    this.getImagesDataFromJson = function(data, datasourceOptions, kind)
    {
        var entry = data.feed.entry;
        
        var imagesData = [];
        jQuery.each(data.feed.entry, function(i, image){
            var imageTitle = kind == "album" ? image.title.$t + " (" + image.gphoto$numphotos.$t + " images)" : image.summary.$t;
            var imageData = {
                thumbnailSrc : image.media$group.media$thumbnail[0].url,
                link: image.link[1].href,
                media: {
                    src: image.media$group.media$content[0].url,
                    title: imageTitle,
                    alt: imageTitle
                }
            };

            if (kind == "album")
                imageData.data = { album: image.gphoto$name.$t };

            imagesData.push(imageData);
        });
        
        return imagesData;
    }
}