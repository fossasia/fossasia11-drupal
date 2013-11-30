// $Id: comment_notify.js,v 1.1 2009/02/13 21:43:16 greggles Exp $
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $("#edit-notify-type-1").bind("click", function() {
      if ($("#edit-notify").attr("checked", false)) {
        // Auto-notification not checked - do it for them.
        $("#edit-notify").attr("checked",true);
      }
    });
    $("#edit-notify-type-2").bind("click", function() {
      if ($("#edit-notify").attr("checked", false)) {
        // Auto-notification not checked - do it for them.
        $("#edit-notify").attr("checked",true);
      }
    });
  });
}
