// $Id: modalframe_example.js,v 1.1.2.7 2009/12/28 02:21:20 markuspetrux Exp $

(function ($) {

Drupal.behaviors.modalFrameExample = function() {
  $('.modalframe-example-child:not(.modalframe-example-processed)').addClass('modalframe-example-processed').click(function() {
    var element = this;

    // This is our onSubmit callback that will be called from the child window
    // when it is requested by a call to modalframe_close_dialog() performed
    // from server-side submit handlers.
    function onSubmitCallbackExample(args, statusMessages) {
      // Display status messages generated during submit processing.
      if (statusMessages) {
        $('.modalframe-example-messages').hide().html(statusMessages).show('slow');
      }

      if (args && args.message) {
        // Provide a simple feedback alert deferred a little.
        setTimeout(function() { alert(args.message); }, 500);
      }
    }

    // Hide the messages are before opening a new dialog.
    $('.modalframe-example-messages').hide('fast');

    // Build modal frame options.
    var modalOptions = {
      url: $(element).attr('href'),
      autoFit: true,
      onSubmit: onSubmitCallbackExample
    };

    // Try to obtain the dialog size from the className of the element.
    var regExp = /^.*modalframe-example-size\[\s*([0-9]*\s*,\s*[0-9]*)\s*\].*$/;
    if (typeof element.className == 'string' && regExp.test(element.className)) {
      var size = element.className.replace(regExp, '$1').split(',');
      modalOptions.width = parseInt(size[0].replace(/ /g, ''));
      modalOptions.height = parseInt(size[1].replace(/ /g, ''));
    }

    // Open the modal frame dialog.
    Drupal.modalFrame.open(modalOptions);

    // Prevent default action of the link click event.
    return false;
  });
};

})(jQuery);
