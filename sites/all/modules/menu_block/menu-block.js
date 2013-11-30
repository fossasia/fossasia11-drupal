// $Id: menu-block.js,v 1.5.2.1 2010/03/23 21:10:48 johnalbin Exp $

(function ($) {

Drupal.behaviors.menu_block = function (context) {
  // This behavior attaches by ID, so is only valid once on a page.
  if ($('#menu-block-settings.menu-block-processed').size()) {
    return;
  }
  $('#menu-block-settings', context).addClass('menu-block-processed');

  // Process the form if its in a Panel overlay.
  if ($('.menu-block-menu-tree-configure-form', context).size()) {
    // Toggle display of "title link" if "override title" is checked.
    $('.menu-block-override-title', context).change( function() {
      if ($('.menu-block-override-title:checked').length) {
        $('.menu-block-title-link').slideUp('fast');
      }
      else {
        $('.menu-block-title-link').slideDown('fast');
      }
    } );
    if ($('.menu-block-override-title:checked').length) {
      $('.menu-block-title-link').css('display', 'none');
    }
  }
  // Process the form if its on a block config page.
  else if ($('.menu-block-configure-form', context).size()) {
    // Toggle display of "title link" if "block title" has a value.
    $('input[name="title"]', context).change( function() {
      if ($('input[name="title"]').val()) {
        $('.menu-block-title-link').slideUp('fast');
      }
      else {
        $('.menu-block-title-link').slideDown('fast');
      }
    } );
    if ($('input[name="title"]', context).val()) {
      $('.menu-block-title-link').css('display', 'none');
    }

    // Split the un-wieldly "parent item" pull-down into two hierarchal pull-downs.
    $('.menu-block-parent', context)
      .html(Drupal.settings.menu_block.parent_options[Drupal.settings.menu_block.menus_default])
      .val(Drupal.settings.menu_block.parent_default)
      .before(Drupal.settings.menu_block.menus);
    $('.menu-block-parent-menu', context).change( function() {
      $('.menu-block-parent')
        .html(Drupal.settings.menu_block.parent_options[$('.menu-block-parent-menu').val()])
        .val(Drupal.settings.menu_block.parent_default);
    } );
  }

  // Toggle display of "follow parent" if "follow" has been checked.
  $('.menu-block-follow', context).change( function() {
    if ($('.menu-block-follow:checked').length) {
      $('.menu-block-follow-parent').slideDown('fast');
    }
    else {
      $('.menu-block-follow-parent').slideUp('fast');
    }
  } );
  if (!$('.menu-block-follow:checked', context).length) {
    $('.menu-block-follow-parent', context).css('display', 'none');
  }
};

})(jQuery);
