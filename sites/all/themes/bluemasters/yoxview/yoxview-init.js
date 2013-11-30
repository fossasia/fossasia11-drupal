var _yoxviewPath = getYoxviewPath();

var cssLink = parent.document.createElement("link");
cssLink.setAttribute("rel", "Stylesheet");
cssLink.setAttribute("type", "text/css");
cssLink.setAttribute("href", _yoxviewPath + "yoxview.css");
parent.document.getElementsByTagName("head")[0].appendChild(cssLink);

function LoadScript( url )
{
	document.write( '<scr' + 'ipt type="text/javascript" src="' + url + '"><\/scr' + 'ipt>' ) ;
}

var jQueryIsLoaded = typeof jQuery != "undefined";

if (!jQueryIsLoaded)
    LoadScript("http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js");
    
if (!jQueryIsLoaded || !jQuery().jsonp)
    LoadScript(_yoxviewPath + "jquery.jsonp-1.0.4.min.js");

// If no Flash is required (for local video or Flash files), you can disable the following two lines:
if (!jQueryIsLoaded || !jQuery().flash)
    LoadScript(_yoxviewPath + "jquery.swfobject.1-1-1.min.js");
    
if (typeof Yox == "undefined")
    LoadScript(_yoxviewPath + "yox.min.js");

if (!jQueryIsLoaded || !jQuery().yoxthumbs)
    LoadScript(_yoxviewPath + "jquery.yoxthumbs.min.js");
    
LoadScript(_yoxviewPath + "jquery.yoxview-2.05.min.js");

function getYoxviewPath()
{
    var scripts = document.getElementsByTagName("script");
    var regex = /(.*\/)yoxview-init\.js/i;
    for(var i=0; i<scripts.length; i++)
    {
        var currentScriptSrc = scripts[i].src;
        if (currentScriptSrc.match(regex))
            return currentScriptSrc.match(regex)[1];
    }
    
    return null;
}
// Remove the next line's comment to apply yoxview without knowing jQuery to all containers with class 'yoxview':
//LoadScript(_yoxviewPath + "yoxview-nojquery.js"); 