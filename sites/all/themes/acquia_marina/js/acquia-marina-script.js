// $Id: acquia-marina-script.js 7156 2010-04-24 16:48:35Z chris $

Drupal.behaviors.acquia_marinaRoundedCorners = function (context) {
  // Rounded corners - Inner background
  $(".inner .marina-rounded-corners .inner-wrapper .inner-inner").corner("bottom 7px"); 
  $(".inner .marina-title-rounded-blue h2.block-title").corner("top 5px"); 
  $(".inner .marina-title-rounded-green h2.block-title").corner("top 5px"); 
  $("#comments h2.comments-header").corner("top 5px"); 
};

Drupal.behaviors.acquia_marinaPanelsEditFix = function (context) {
  // Sets the .row class to have "overflow: visible" if editing Panel page
  $("#panels-edit-display-form").parents('.row', '.nested').css("overflow", "visible")
  $("#page-manager-edit").parents('.row', '.nested').css("overflow", "visible")
};
