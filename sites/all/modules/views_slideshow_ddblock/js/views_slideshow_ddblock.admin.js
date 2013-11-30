
/**
 *  @file
 *  Views Slideshow DDblock admin page functionality 
 */


function initPagerPositionOptions(key, value) {
  this.key = key;
  this.value = value;
}

function setPagerPositionOptions(pagerPositionOptions) {
  var oldPagerPosition = $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-position').val();
  $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-position').find('option').remove();
  var options = '' ;
  for (var i = 0; i < pagerPositionOptions.length; i++) {
    if (i==0) { 
      options += '<option selected value="' + pagerPositionOptions[i].key + '">' + pagerPositionOptions[i].value + '</option>'; 
    }
    else {
      options += '<option value="' + pagerPositionOptions[i].key + '">' + pagerPositionOptions[i].value + '</option>';
    }
  }
  // populate select box with array
  $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-position').html(options);  
  $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-position').val(oldPagerPosition);  
}

/**
 * Show/hide pager settings sections on the views_slideshow_ddblock configuration page.
 * don't change
 */
Drupal.behaviors.ddblockShowHidePagerOptions = function(context) {
  // Show/hide pager options depending on the checkbox.
  $('#edit-style-options-views-slideshow-ddblock-settings-pager-toggle:not(.ddblock-show-hide-pager-options-processed)', context)
  .addClass('ddblock-show-hide-pager-options-processed')
  .bind("click change", function() {
    if (this.checked){
      $('#ddblock-pager-settings-wrapper').show();
    }
    else {
      $('#ddblock-pager-settings-wrapper').hide();
    }
    return true;
  }).trigger('change').trigger('change')
};

/**
 * Show/hide prev/next pager settings sections on the views_slideshow_ddblock configuration page.
 * don't change
 */
Drupal.behaviors.ddblockShowHidePager2Options = function(context) {
  // Show/hide pev/next pager options depending on the checkbox.
  $('#edit-style-options-views-slideshow-ddblock-settings-pager2:not(.ddblock-show-hide-pager2-options-processed)', context)
  .addClass('ddblock-show-hide-pager2-options-processed')
  .bind("click change", function() {
    if (this.checked){
      $('#ddblock-pager2-settings-wrapper').show();
    }
    else {
      $('#ddblock-pager2-settings-wrapper').hide();
    }
    return true;
  }).trigger('change').trigger('change')
};

/**
 * Show/hide prev/next pager settings sections on the views_slideshow_ddblock configuration page.
 * don't change
 */
Drupal.behaviors.ddblockShowHidePager2PagerOptions = function(context) {
  // Show/hide pager options depending on the checkbox.
  $('#edit-style-options-views-slideshow-ddblock-settings-pager2-settings-pager2-position-pager:not(.ddblock-show-hide-pager2-pager-options-processed)', context)
  .addClass('ddblock-show-hide-pager2-pager-options-processed')
  .bind("click change", function() {
    if (this.checked){
      $('#ddblock-pager2-pager-settings-wrapper').show();
    }
    else {
      $('#ddblock-pager2-pager-settings-wrapper').hide();
    }
    return true;
  }).trigger('change').trigger('change')
};

/**
 * Show/hide prev/next pager settings sections on the views_slideshow_ddblock configuration page.
 * don't change
 */
Drupal.behaviors.ddblockShowHidePager2SlideOptions = function(context) {
  // Show/hide pager options depending on the checkbox.
  $('#edit-style-options-views-slideshow-ddblock-settings-pager2-settings-pager2-position-slide:not(.ddblock-show-hide-pager2-slide-options-processed)', context)
  .addClass('ddblock-show-hide-pager2-slide-options-processed')
  .bind("click change", function() {
    if (this.checked){
      $('#ddblock-pager2-slide-settings-wrapper').show();
    }
    else {
      $('#ddblock-pager2-slide-settings-wrapper').hide();
    }
    return true;
  }).trigger('change').trigger('change')
};

/**
 * Show/hide slide text settings sections on the views_slideshow_ddblock configuration page.
 * don't change
 */
Drupal.behaviors.ddblockShowHideSlideTextOptions = function(context) {
  // Show/hide slide text options depending on the checkbox.
  $('#edit-style-options-views-slideshow-ddblock-settings-slide-text:not(.ddblock-show-hide-text-options-processed)', context)
  .addClass('ddblock-show-hide-text-options-processed')
  .bind("click change", function() {
    if (this.checked) {
      $('#ddblock-slide-text-settings-wrapper').show();
    }
    else {
      $('#ddblock-slide-text-settings-wrapper').hide();
    }
    return true;
  }).trigger('change').trigger('change')
};

/**
 * Show/hide jquery slide text settings sections on the views_slideshow_ddblock configuration page.
 * don't change
 */
Drupal.behaviors.ddblockShowHideSlideJqueryTextOptions = function(context) {
  // Show/hide slide text options depending on the checkbox.
  $('#edit-style-options-views-slideshow-ddblock-settings-slide-text-settings-slide-text-jquery:not(.ddblock-show-hide-text-jquery-options-processed)', context)
  .addClass('ddblock-show-hide-text-jquery-options-processed')
  .bind("click change", function() {
    if (this.checked) {
      $('#ddblock-slide-jquery-settings-wrapper').show();
    }
    else {
      $('#ddblock-slide-jquery-settings-wrapper').hide();
    }
    return true;
  }).trigger('change').trigger('change')
};

/**
 * Change pager container depending on the pager.
 */
Drupal.behaviors.ddblockChangePagerContainerOptions = function(context) {

  // Change pager container option depending on select.
  $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager:not(.ddblock-change-pager-container-options-processed)', context)
  .addClass('ddblock-change-pager-container-options-processed')
  .bind("change", function() {
    val = $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager').val();
    switch (val) {
      case "number-pager" :
      case "prev-next-pager" :
      case "custom-pager" :
        $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-container').val('.custom-pager-item');
      break;
      case "scrollable-pager" :
        $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-container').val('.scrollable-pager-item');
      break;
    }
    return false;
  }).trigger('change').trigger('change')
};



/**
 * Show/hide custom template settings sections on the views_slideshow_ddblock configuration page.
 */
Drupal.behaviors.ddblockShowHideCustomTemplateOptions = function(context) {

  // Show/hide imagefolder/contenttype options depending on the select.
  $('#edit-style-options-views-slideshow-ddblock-template:not(.ddblock-show-hide-custom-template-options-processed)', context)
  .addClass('ddblock-show-hide-custom-template-options-processed')
  .bind("change", function() {
    val = $('#edit-style-options-views-slideshow-ddblock-template').val();
    if (val.match('default') == "default" ) {
      $('#ddblock-pager-settings-wrapper').hide();
      $('#edit-style-options-views-slideshow-ddblock-settings-pager-toggle-wrapper').hide();
      $('#edit-style-options-views-slideshow-ddblock-settings-pager2-settings-pager2-position-pager-wrapper').hide();
      $('ddblock-pager2-pager-settings-wrapper').hide();
      $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager').find('option').remove().end().append('<option value="custom-pager">Custom pager</option>').val('custom-pager');
      $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-container').val('.custom-pager-item');
      $('#ddblock-nr-of-pager-items-wrapper').hide();
    }  
    else {
      $('#ddblock-pager-settings-wrapper').show();
      $('#edit-style-options-views-slideshow-ddblock-settings-pager-toggle-wrapper').show();
      $('#edit-style-options-views-slideshow-ddblock-settings-pager2-settings-pager2-position-pager-wrapper').show();
      $('#ddblock-pager2-pager-settings-wrapper').show();
    }      
    if (val.match("-10p") == "-10p" || val.match("-10l") == "-10l") {
      $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-container').val('.custom-pager-item');
      $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager').find('option').remove().end().append('<option value="number-pager">Number pager</option>').val('number-pager');
      $('#ddblock-nr-of-pager-items-wrapper').hide();
    }
    else {
      if (val.match("-20p") == "-20p" || val.match("-20l") == "-20l") {
        $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-container').val('.custom-pager-item');
        $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager').find('option').remove().end().append('<option value="prev-next-pager">Prev/Next pager</option>').val('prev-next-pager');
        $('#ddblock-nr-of-pager-items-wrapper').hide();
    }
      else {
        if (val.match("-30p") == "-30p" || val.match("-30l") == "-30l" ||
            val.match("-40p") == "-40p" || val.match("-40l") == "-40l" ||
            val.match("-50p") == "-50p" || val.match("-50l") == "-50l") {
          $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-container').val('.custom-pager-item');
          $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager').find('option').remove().end().append('<option value="custom-pager">Custom pager</option>').val('custom-pager');
          $('#ddblock-nr-of-pager-items-wrapper').hide();
        }
        else {
          if (val.match("-60p") == "-60p" || val.match("-60l") == "-60l") {
            $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager').val('scrollable-pager');
            $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager-container').val('.scrollable-pager-item');
          $('#edit-style-options-views-slideshow-ddblock-settings-pager-settings-pager').find('option').remove().end().append('<option value="scrollable-pager">Scrollable pager</option>').val('scrollable-pager');
          $('#ddblock-nr-of-pager-items-wrapper').show();
          }
        }  
      }  
    }    
    // portrait themes
    if (val.match("-10p") == "-10p" || val.match("-20p") == "-20p" ||
        val.match("-30p") == "-30p" || val.match("-40p") == "-40p" ||
        val.match("-50p") == "-50p" || val.match("-60p") == "-60p") {
      var pagerPositionOptions = new Array();  
      pagerPositionOptions[0] = new initPagerPositionOptions('top', 'Top');
      pagerPositionOptions[1] = new initPagerPositionOptions('bottom', 'Bottom');
      
      setPagerPositionOptions(pagerPositionOptions);
    }
    // landscape themes
    else {
      var pagerPositionOptions = new Array();  
      pagerPositionOptions[0] = new initPagerPositionOptions('left', 'Left');
      pagerPositionOptions[1] = new initPagerPositionOptions('right', 'Right');
 
      setPagerPositionOptions(pagerPositionOptions);
    }           
    if (val.match("custom") == "custom") {
      $('#ddblock-custom-template-settings-wrapper').show();
      $('#ddblock-nr-of-pager-items-wrapper').show();
    }
    else {
      $('#ddblock-custom-template-settings-wrapper').hide();
    }     
    return false;
  }).trigger('change').trigger('change')
};

/**
 * Show/hide custom template settings sections on the views_slideshow_ddblock configuration page.
 */
Drupal.behaviors.ddblockShowHideCustomTemplateSizeOptions = function(context) {

  // Show/hide imagefolder/contenttype options depending on the select.
  $('#edit-style-options-views-slideshow-ddblock-template-size-wrapper-template-size:not(.ddblock-show-hide-custom-template-size-options-processed)', context)
  .addClass('ddblock-show-hide-custom-template-size-options-processed')
  .bind("change", function() {
    val = $('#edit-style-options-views-slideshow-ddblock-template-size-wrapper-template-size').val();
    switch (val) {
    case "custom" :
      $('#ddblock-custom-template-size-settings-wrapper').show();
    break; 
    default:
      $('#ddblock-custom-template-size-settings-wrapper').hide();
    }
    return false;
  }).trigger('change').trigger('change')
};
