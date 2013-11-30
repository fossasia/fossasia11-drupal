/* $Id: lightbox_lite.js,v 1.1.2.2.2.19 2010/06/07 14:54:30 snpower Exp $ */

/**
 * Lightbox JS: Fullsize Image Overlays
 * by Lokesh Dhakar - http://www.huddletogether.com
 *
 * For more information on this script, visit:
 * http://huddletogether.com/projects/lightbox/
 *
 * This script is distributed via Drupal.org with permission from Lokesh Dhakar.
 * Under GPL license.
 *    Mailto: bugzie@gmail.com
 */

//
// getPageScroll()
// Returns array with x,y page scroll values.
// Core code from - quirksmode.org
//
function getPageScroll() {

  var xScroll, yScroll;

  if (self.pageYOffset) {
    yScroll = self.pageYOffset;
    xScroll = self.pageXOffset;

  // Explorer 6 Strict
  }
  else if (document.documentElement && document.documentElement.scrollTop) {
    yScroll = document.documentElement.scrollTop;
    xScroll = document.documentElement.scrollLeft;
  }
  else if (document.body) {// all other Explorers
    yScroll = document.body.scrollTop;
    xScroll = document.body.scrollLeft;
  }

  arrayPageScroll = [xScroll, yScroll];
  return arrayPageScroll;
}



// getPageSize()
// Returns array with page width, height and window width, height
// Core code from - quirksmode.org
// Edit for Firefox by pHaez
function getPageSize() {

  var xScroll, yScroll;

  if (window.innerHeight && window.scrollMaxY) {
    xScroll = window.innerWidth + window.scrollMaxX;
    yScroll = window.innerHeight + window.scrollMaxY;
  // all but Explorer Mac
  }
  else if (document.body.scrollHeight > document.body.offsetHeight) {
    xScroll = document.body.scrollWidth;
    yScroll = document.body.scrollHeight;
  // Explorer Mac...would also work in Explorer 6 Strict, Mozilla and Safari
  }
  else {
    xScroll = document.body.offsetWidth;
    yScroll = document.body.offsetHeight;
  }

  var windowWidth, windowHeight;
  if (self.innerHeight) { // all except Explorer
    if (document.documentElement.clientHeight) {
      windowWidth = document.documentElement.clientWidth;
    }
    else {
      windowWidth = self.innerWidth;
    }
    windowHeight = self.innerHeight;
  }
  else if (document.documentElement && document.documentElement.clientHeight) { // Explorer 6 Strict Mode
    windowWidth = document.documentElement.clientWidth;
    windowHeight = document.documentElement.clientHeight;
  }
  else if (document.body) { // other Explorers
    windowWidth = document.body.clientWidth;
    windowHeight = document.body.clientHeight;
  }

  // for small pages with total height less then height of the viewport
  if (yScroll < windowHeight) {
    pageHeight = windowHeight;
  }
  else {
    pageHeight = yScroll;
  }

  // for small pages with total width less then width of the viewport
  if (xScroll < windowWidth) {
    pageWidth = xScroll;
  }
  else {
    pageWidth = windowWidth;
  }


  arrayPageSize = [pageWidth, pageHeight, windowWidth, windowHeight];
  return arrayPageSize;
}


// pause(numberMillis)
function pause(ms) {
  var date = new Date();
  var curDate = null;
  do { curDate = new Date(); }
  while (curDate - date < ms);
}

// hideLightbox()
function hideLightbox() {
  // get objects
  objOverlay = document.getElementById('lightbox2-overlay');
  objLightbox = document.getElementById('lightbox');

  // hide lightbox and overlay
  objOverlay.style.display = 'none';
  objLightbox.style.display = 'none';

  // make select boxes visible
  selects = document.getElementsByTagName("select");
  for (i = 0; i != selects.length; i++) {
    if (selects[i].style.display != "none") {
      selects[i].style.visibility = "visible";
    }
  }

  // make flash objects visible
  embed = document.getElementsByTagName("embed");
  for (i = 0; i != embed.length; i++) {
    if (embed[i].style.display != "none") {
      embed[i].style.visibility = "visible";
    }
  }
  objects = document.getElementsByTagName("object");
  for (i = 0; i != objects.length; i++) {
    if (objects[i].style.display != "none") {
      objects[i].style.visibility = "visible";
    }
  }

  // disable keydown listener
  document.onkeydown = '';
}


// getKey(key)
// Gets keycode. If 'x' is pressed then it hides the lightbox.
function getKey(e) {
  if (e === null) { // ie
    keycode = event.keyCode;
    escapeKey = 27;
  }
  else { // mozilla
    keycode = e.keyCode;
    escapeKey = e.DOM_VK_ESCAPE;
  }
  key = String.fromCharCode(keycode).toLowerCase();

  if (key == 'x' || key == 'c' || keycode == escapeKey) { hideLightbox(); }
}


// listenKey()
function listenKey () { document.onkeydown = getKey; }


function imgLoadingError(image, objImage, objLink) {
  var settings = Drupal.settings.lightbox2;
  image.src = settings.default_image;
  objImage.src = settings.default_image;
  objLink.href = settings.default_image;
}


// showLightbox()
// Preloads images. Pleaces new image in lightbox then centers and displays.
function showLightbox(objLink) {
  var settings = Drupal.settings.lightbox2;
  // prep objects
  var objOverlay = document.getElementById('lightbox2-overlay');
  var objLightbox = document.getElementById('lightbox');
  var objCaption = document.getElementById('lightboxCaption');
  var objImage = document.getElementById('lightboxImage');
  var objLoadingImage = document.getElementById('loadingImage');
  var objLightboxDetails = document.getElementById('lightboxDetails');

  var arrayPageSize = getPageSize();
  var arrayPageScroll = getPageScroll();

  // set height of Overlay to take up whole page and show
  objOverlay.style.height = (arrayPageSize[1] + 'px');
  objOverlay.style.display = 'block';
  objOverlay.style.opacity = settings.overlay_opacity;
  objOverlay.style.backgroundColor = '#' + settings.overlay_color;

  // preload image
  imgPreload = new Image();
  imgPreload.onerror = function() { imgLoadingError(this, objImage, objLink); };

  imgPreload.onload = function() {
    objImage.src = objLink.href;

    // center lightbox and make sure that the top and left values are not
    // negative and the image placed outside the viewport
    var lightboxTop = arrayPageScroll[1] + ((arrayPageSize[3] - 35 - imgPreload.height) / 2);
    var lightboxLeft = ((arrayPageSize[0] - 20 - imgPreload.width) / 2);

    objLightbox.style.top = (lightboxTop < 0) ? "0px" : lightboxTop + "px";
    objLightbox.style.left = (lightboxLeft < 0) ? "0px" : lightboxLeft + "px";


    //objLightboxDetails.style.width = imgPreload.width + 'px';
    objLightbox.style.width = imgPreload.width + 'px';

    if (objLink.getAttribute('title')) {
      objCaption.style.display = 'block';
      //objCaption.style.width = imgPreload.width + 'px';
      objCaption.innerHTML = objLink.getAttribute('title');
    }
    else {
      objCaption.style.display = 'none';
    }

    // A small pause between the image loading and displaying is required with
    // IE,  this prevents the previous image displaying for a short burst
    // causing flicker.
    if (navigator.appVersion.indexOf("MSIE") != -1) {
      pause(250);
    }

    if (objLoadingImage) { objLoadingImage.style.display = 'none'; }

    // Hide select boxes as they will 'peek' through the image in IE
    selects = document.getElementsByTagName("select");
    for (i = 0; i != selects.length; i++) {
      if (selects[i].style.display != "none") {
        selects[i].style.visibility = "hidden";
      }
    }

    // Hide flash objects as they will 'peek' through the image in IE
    embed = document.getElementsByTagName("embed");
    for (i = 0; i != embed.length; i++) {
      if (embed[i].style.display != "none") {
        embed[i].style.visibility = "hidden";
      }
    }
    objects = document.getElementsByTagName("object");
    for (i = 0; i != objects.length; i++) {
      if (objects[i].style.display != "none") {
        objects[i].style.visibility = "hidden";
      }
    }

    objLightbox.style.display = 'block';

    // After image is loaded, update the overlay height as the new image might
    // have increased the overall page height.
    arrayPageSize = getPageSize();
    objOverlay.style.height = (arrayPageSize[1] + 'px');

    // Check for 'x' keydown
    listenKey();

    return false;
  };

  imgPreload.src = objLink.href;

}



// initLightbox()
// Function runs on window load, going through link tags looking for
// rel="lightbox".  These links receive onclick events that enable the lightbox
// display for their targets.  The function also inserts html markup at the top
// of the page which will be used as a container for the overlay pattern and
// the inline image.
function initLightbox() {

  if (!document.getElementsByTagName) { return; }
  var anchors = document.getElementsByTagName("a");

  // loop through all anchor tags
  for (var i = 0; i < anchors.length; i++) {
    var anchor = anchors[i];
    var relAttribute = String(anchor.getAttribute("rel"));

    if (anchor.getAttribute("href") && relAttribute.toLowerCase().match("lightbox")) {
      $(anchor).click(function(e) { showLightbox(this); if (e.preventDefault) { e.preventDefault(); } return false; });
    }
  }

  // the rest of this code inserts html at the top of the page that looks like
  // this:
  // <div id="lightbox2-overlay">
  //  <a href="#" onclick="hideLightbox(); return false;"><img id="loadingImage" /></a>
  // </div>
  // <div id="lightbox">
  //  <a href="#" onclick="hideLightbox(); return false;" title="Click anywhere to close image">
  //   <img id="closeButton" />
  //   <img id="lightboxImage" />
  //  </a>
  //  <div id="lightboxDetails">
  //   <div id="lightboxCaption"></div>
  //   <div id="keyboardMsg"></div>
  //  </div>
  // </div>

  var objBody = document.getElementsByTagName("body").item(0);

  // create overlay div and hardcode some functional styles (aesthetic styles are in CSS file)
  var objOverlay = document.createElement("div");
  objOverlay.setAttribute('id', 'lightbox2-overlay');
  objOverlay.onclick = function () { hideLightbox(); return false; };
  objOverlay.style.display = 'none';
  objOverlay.style.position = 'absolute';
  objOverlay.style.top = '0';
  objOverlay.style.left = '0';
  objOverlay.style.zIndex = '90';
  objOverlay.style.width = '100%';
  objBody.insertBefore(objOverlay, objBody.firstChild);

  var arrayPageSize = getPageSize();
  var arrayPageScroll = getPageScroll();

  // create loader image
  var objLoadingImage = document.createElement("span");
  objLoadingImage.setAttribute('id', 'loadingImage');
  objOverlay.appendChild(objLoadingImage);

  // create lightbox div, same note about styles as above
  var objLightbox = document.createElement("div");
  objLightbox.setAttribute('id', 'lightbox');
  objLightbox.style.display = 'none';
  objLightbox.style.position = 'absolute';
  objLightbox.style.zIndex = '100';
  objBody.insertBefore(objLightbox, objOverlay.nextSibling);

  // create link
  var objLink = document.createElement("a");
  objLink.setAttribute('href', '#');
  objLink.setAttribute('title', 'Click to close');
  objLink.onclick = function () { hideLightbox(); return false; };
  objLightbox.appendChild(objLink);

  // create close button image
  var objCloseButton = document.createElement("span");
  objCloseButton.setAttribute('id', 'closeButton');
  objLink.appendChild(objCloseButton);

  // create image
  var objImage = document.createElement("img");
  objImage.setAttribute('id', 'lightboxImage');
  objLink.appendChild(objImage);

  // create details div, a container for the caption and keyboard message
  var objLightboxDetails = document.createElement("div");
  objLightboxDetails.setAttribute('id', 'lightboxDetails');
  objLightbox.appendChild(objLightboxDetails);

  // create caption
  var objCaption = document.createElement("div");
  objCaption.setAttribute('id', 'lightboxCaption');
  objCaption.style.display = 'none';
  objLightboxDetails.appendChild(objCaption);

  // create keyboard message
  var settings = Drupal.settings.lightbox2;
  var objKeyboardMsg = document.createElement("div");
  objKeyboardMsg.setAttribute('id', 'keyboardMsg');
  objKeyboardMsg.innerHTML = settings.lite_press_x_close;
  objLightboxDetails.appendChild(objKeyboardMsg);


}

Drupal.behaviors.initLightbox2 = function (context) {
  initLightbox();
};
