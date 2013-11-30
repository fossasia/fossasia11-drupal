(function($){

$.tablesorter.addParser({
  // set a unique id
  id: 'changefreq',
  is: function(s) {
    return false;
  },
  format: function(s) {
    switch (s) {
      case 'always':
        return 0;
      case 'hourly':
        return 1;
      case 'daily':
        return 2;
      case 'weekly':
        return 3;
      case 'monthly':
        return 4;
      case 'yearly':
        return 5;
      default:
        return 6;
    }
  },
  type: 'numeric'
});

$(document).ready(function() {
  // Set some location variales.
  $('h1').append(': ' + location);
  document.title += ': ' + location;

  $('table').tablesorter({
    sortList: [[0,0]],
    headers: {
      2: { sorter: 'changefreq' }
    },
    widgets: ['zebra']
  });
});

})(jQuery);
