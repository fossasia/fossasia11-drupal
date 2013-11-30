// $Id: admin.erd.js,v 1.1.2.3 2009/09/20 17:59:03 markuspetrux Exp $

(function ($) {

/**
 * Drupal behavior to apply rounded corner effects to certain elements in the
 * Entity Relationships Diagram.
 *
 * Note that we use inline-block elements so that we can render them centered
 * on a "fluid" container. Please, let me know if there's another method to
 * achieve the same effect without inline-block elements.
 *
 * Compatibility issues with IE6 and IE7:
 *
 * a) Rounded corners in IE6/7 do not work, probably because we're using
 *    inline-block elements, and IE6/7 is buggy here.
 *    Rounded corners worked well for me in Firefox 3 and Opera 9.x, so let's
 *    assume it works for all non-IE browsers.
 *
 * b) We are using inline-block attribute for elements that default to block
 *    display, so we force hasLayout and set them as inline to workaround
 *    another IE6/7 bug. See http://reference.sitepoint.com/css/haslayout
 *
 *    Ideally, we would do this using a separate stylesheet ie-fixes.css, that
 *    would be included using conditional comments in the HTML header. BUT, ...
 *    Drupal does not support this feature in drupal_add_css() API. AFAIK, the
 *    only way is using drupal_set_html_head(), BUT, ... tags included here are
 *    rendered on top of all stylesheets in the page, so this becomes a problem
 *    by itself because CSS properties defined in this ie-fixes.css file would
 *    be redefined by the default stylesheet, so we would have to user !important
 *    in the CSS properties defined in this ie-fixes.css file, which may create
 *    conflicts for custom theming of the site, and that's simply too much! :P
 *
 *    Since we already need to check for IE here, because the rounded corners
 *    issue (a), we better do all specific stuff for IE6/7 in the same place and
 *    keep the rest of the module as standard as possible.
 *
 *    Caveat: If you disable javascript in IE, then you'll see all ERD entities
 *    expanded to the whole width of the container. In other words: a mess.
 *    Let's pray IE8 (or IE9 :P) fixes this issue, at least. ;-)
 */
Drupal.behaviors.nodeRelationshipsRoundedCorners = function(context) {
  $('.noderelationships-erd:not(.noderelationships-processed)', context).addClass('noderelationships-processed').each(function() {
    if ($.browser.msie) {
      // Fix hasLayout issues in IE6/7.
      $('.noderelationships-erd-entity', context).css({display: 'inline', zoom: 1});
      $('.noderelationships-erd-relation-box', context).css({display: 'inline', zoom: 1});
      return;
    }

    // Apply rounded corners effect.
    $('.noderelationships-erd-entity-caption', this).corner('top');
    $('.noderelationships-erd-fields', this).corner('bottom');
    $('.noderelationships-erd-relation-caption', this).corner('top bottom 5px');
  });
};

})(jQuery);
