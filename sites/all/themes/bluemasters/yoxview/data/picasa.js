/*!
 * YoxView Picasa plugin
 * http://yoxigen.com/yoxview/
 *
 * Copyright (c) 2010 Yossi Kolesnicov
 *
 * Licensed under the MIT license.
 * http://www.opensource.org/licenses/mit-license.php
 *
 * Date: 20th May, 2010
 * Version : 1.9
 */ 
function yox_picasa(){function k(b){var f=b.url;if(b.user&&b.user!="lh"){f+="user/"+b.user;if(b.album)f+="/album/"+b.album}else f+="all";return f}function l(b,f,d){b=parseInt(b);for(var c=f.length-1;c>=0;c--){var a=f[c];if(b>=a)return d?c<f.length-1?f[c+1]:a:a}return b}var h=jQuery,m=/http:\/\/picasaweb\.google\.\w+\/([^\/#\?]+)\/?([^\/#\?]+)?(\?([^#]*))?/,n=this;this.getImagesData=function(b,f){var d={};if(b.dataUrl){var c=b.dataUrl.match(m);if(c&&c.length>1){d.user=c[1];if(c[2])d.album=c[2];c[4]&& h.extend(d,Yox.queryToJson(c[4]))}}var a=jQuery.extend({},{url:"http://picasaweb.google.com/data/feed/api/",setThumbnail:true,setSingleAlbumThumbnails:true,setTitle:true,alt:"json",thumbsize:64},d,b.dataSourceOptions);if(a.user&&!a.album&&!a.q)a.thumbsize=104;if(a.tags)a.tag=a.tags;if(a.album=="")a.album=null;d=screen.width>screen.height?screen.width:screen.height;c=a.imgmax&&h.inArray(a.imgmax,i)==-1?a.imgmax:null;if(!a.imgmax||c||d<a.imgmax)a.imgmax=l(c||d,i,a.roundSizeUp);d=k(a);var g={};b.onLoadBegin&& b.onLoadBegin();h.jsonp({url:d,async:false,dataType:"jsonp",data:a,callbackParameter:"callback",success:function(e){if(!e.feed.entry||e.feed.entry.length==0)b.onNoData&&b.onNoData();else{var j=e.feed.entry[0].category[0].term.match(/.*#(.*)/)[1];j=="album"&&h.extend(g,{title:e.feed.title.$t,createGroups:true});g.images=n.getImagesDataFromJson(e,a,j);g.images.length>0&&a.setThumbnail&&!a.setSingleAlbumThumbnails&&h.extend(g,{isGroup:true,link:e.feed.link[1].href,thumbnailSrc:e.feed.icon.$t,title:e.feed.title.$t}); f&&f(g);b.onLoadComplete&&b.onLoadComplete()}},error:function(){b.onLoadError&&b.onLoadError("Picasa plugin encountered an error while retrieving data")}})};var i=[94,110,128,200,220,288,320,400,512,576,640,720,800,912,1024,1152,1280,1440,1600];this.getImagesDataFromJson=function(b,f,d){var c=[];jQuery.each(b.feed.entry,function(a,g){var e=d=="album"?g.title.$t+" ("+g.gphoto$numphotos.$t+" images)":g.summary.$t;e={thumbnailSrc:g.media$group.media$thumbnail[0].url,link:g.link[1].href,media:{src:g.media$group.media$content[0].url, title:e,alt:e}};if(d=="album")e.data={album:g.gphoto$name.$t};c.push(e)});return c}};