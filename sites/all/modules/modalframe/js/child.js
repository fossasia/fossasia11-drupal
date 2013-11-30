// $Id: child.js,v 1.1.2.13 2010/01/01 20:31:01 markuspetrux Exp $

(function ($) {

/**
 * Modal Frame object for child windows.
 */
Drupal.modalFrameChild = Drupal.modalFrameChild || {
  processed: false,
  behaviors: {}
};

/**
 * Child dialog behavior.
 */
Drupal.modalFrameChild.attach = function(context) {
  var self = Drupal.modalFrameChild;
  var settings = Drupal.settings.modalFrameChild || {};

  // If we cannot reach the parent window, then we have nothing else todo here.
  if (!self.isObject(parent.Drupal) || !self.isObject(parent.Drupal.modalFrame)) {
    return;
  }

  // Shortcuts to parent objects.
  self.$parentWindow = parent.jQuery(parent);
  self.parentModalFrame = parent.Drupal.modalFrame;

  // Make sure this behavior is not processed more than once.
  if (!self.processed) {
    self.processed = true;

    // If a form has been submitted successfully, then the server side script
    // may have decided to tell us the parent window to close the popup dialog.
    if (settings.closeModal) {
      self.triggerParentEvent('childLoad', [window, true]);
      // Close the child window from a separate thread because the current
      // one is busy processing Drupal behaviors.
      setTimeout(function() {
        self.triggerParentEvent('childClose', [settings.args, settings.statusMessages]);
      }, 1);
      return;
    }

    // Ok, now we can tell the parent window we're ready.
    self.triggerParentEvent('childLoad', [window]);

    // Install onBeforeUnload callback, if module is present.
    if (self.isObject(Drupal.onBeforeUnload) && !Drupal.onBeforeUnload.callbackExists('modalFrameChild')) {
      Drupal.onBeforeUnload.addCallback('modalFrameChild', function() {
        // Tell the parent window we're unloading.
        self.triggerParentEvent('childUnload', [window]);
      });
    }

    // Save the current window size to evaluate changes when new Drupal
    // behaviors are attached. For example, after an AJAX request.
    self.currentWindowSize = {width: $(window).width(), height: $('body').height() + 25};
  }
  else {
    // Tell the parent window to resize the Modal Frame if the child window
    // size has changed more than a few pixels tall or wide.
    var newWindowSize = {width: $(window).width(), height: $('body').height() + 25};
    if (Math.abs(self.currentWindowSize.width - newWindowSize.width) > 5 || Math.abs(self.currentWindowSize.height - newWindowSize.height) > 5) {
      self.currentWindowSize = newWindowSize;
      self.triggerParentEvent('childResize');
    }
  }

  // Attach child related behaviors to the current context.
  self.attachBehaviors(context);
};

/**
 * Trigger a custom event on the parent window.
 */
Drupal.modalFrameChild.triggerParentEvent = function(eventName, eventArguments) {
  this.$parentWindow.trigger(this.parentModalFrame.eventHandlerName(eventName), eventArguments);
};

/**
 * Check if the given variable is an object.
 */
Drupal.modalFrameChild.isObject = function(something) {
  return (something !== null && typeof something === 'object');
};

/**
 * Attach child related behaviors to the iframed document.
 */
Drupal.modalFrameChild.attachBehaviors = function(context) {
  $.each(this.behaviors, function() {
    this(context);
  });
};

/**
 * Add target="_new" to all external URLs.
 */
Drupal.modalFrameChild.behaviors.parseLinks = function(context) {
  $('a:not(.modalframe-processed)', context).addClass('modalframe-processed').each(function() {
    // Do not process links that have the class "modalframe-exclude".
    if ($(this).hasClass('modalframe-exclude')) {
      return;
    }
    // Obtain the href attribute of the link.
    var href = $(this).attr('href');
    // Do not process links with an empty href, or that only have the fragment.
    if (!href || href.length <= 0 || href.charAt(0) == '#') {
      return;
    }
    $(this).attr('target', '_new');
  });
};

/**
 * Attach our own behavior on top of the list of existing behaviors.
 *
 * We need to execute before everything else because the child frame is not
 * visible until we bind the child window to the parent, and this may cause
 * problems with other behaviors that need the document visible in order to
 * do its own job.
 */
Drupal.behaviors = $.extend({modalFrameChild: Drupal.modalFrameChild.attach}, Drupal.behaviors);

})(jQuery);
