
    // $Id: countdowntimer.module,v 1.8.2.22.2.4.2.30 2009/06/22 17:07:23 jvandervort Exp $
    Drupal.behaviors.countdowntimer = function (context) {
      $(document).ready(countdown_auto_attach);
    }
    //countdowntimer namespace
    Drupal.countdowntimer = {};
    Drupal.countdowntimer.formats = ['<em>(%dow% %moy%%day%)</em><br/>%days% days + %hours%:%mins%:%secs%','Only %days% days, %hours% hours, %mins% minutes and %secs% seconds left', '%days% shopping days left', '<em>(%dow% %moy%%day%)</em><br/>%days% days + %hours%:%mins%:%secs%'];

    Drupal.countdowntimer.jstimer = function(timer_element) {
      /* defaults */
      var d = {dir:"down",format_num:0, format_txt:"", timer_complete:new String('<em>Timer Completed</em>'), highlight:new String('style="color:red"').split(/=/), threshold:new Number('5')};
      /* jstimer.properties */
      this.element = timer_element;
      this.d = d;
      /* jstimer.methods */
      this.parse_microformat = Drupal.countdowntimer.parse_microformat;
      this.update = Drupal.countdowntimer.update_timer;

      /* bootstrap, parse microformat, load object */
      try {
        this.parse_microformat();
      }
      catch(e) {
        alert(e.message);
        alert($(timer_element).html());
        this.parse_microformat_success = 0;
        return;
      }

      if ( d.format_txt != "" ) {
          this.outformat = d.format_txt;
      } else {
        this.outformat = Drupal.countdowntimer.formats[d.format_num];
      }

      // replace the static stuff in the format string
      // this only needs to be done once, so here is a good spot.
      this.outformat = this.outformat.replace(/%day%/,   this.to_date.getDate());
      this.outformat = this.outformat.replace(/%month%/, this.to_date.getMonth() + 1);
      this.outformat = this.outformat.replace(/%year%/,  this.to_date.getFullYear());
      this.outformat = this.outformat.replace(/%moy%/,   this.to_date.countdowntimer_get_moy());
      this.outformat = this.outformat.replace(/%dow%/,   this.to_date.countdowntimer_get_dow());
    }

    Drupal.countdowntimer.parse_microformat = function() {

      var timer_span = $(this.element);
      if ( timer_span.hasClass("countdowntimer") ) {
        timer_span.removeClass("countdowntimer")
      }

      var cdt_class = timer_span.children("span[class=cdt_class]").html();
      if ( cdt_class == 'simple-timer' ) {
        this.d.cdt_class = cdt_class;
        var interval = timer_span.children("span[class=cdt_interval]").html();
        var date = new Date();
        this.to_date = date;
        this.to_date.setTime(date.getTime() + (interval*1000));
      } else {
        this.d.cdt_class = 'date-timer';
        var strdate = timer_span.children("span[class=datetime]").html();
        var str_current_server_time = timer_span.children("span[class=current_server_time]").html();
        if ( String(strdate) == 'null' ) {
          this.parse_microformat_success = 0;
          throw new Object({name:"NoDate",message:"CountdownTimer: Span with class=datetime not found within the timer span."});
        }
        var date = new Date();
        try {
          date.countdowntimer_set_iso8601_date(strdate);
        }
        catch(e) {
          throw(e);
        }
        this.to_date = date;
        if ( String(str_current_server_time) != 'null' ) {
          // this is a feedback time from the server to correct for small server-client time differences.
          // not used for normal block and node timers.
          var date_server = new Date();
          date_server.countdowntimer_set_iso8601_date(str_current_server_time);
          var date_client = new Date();
          var adj = date_client.getTime() - date_server.getTime();
          // adjust target date to clients domain
          this.to_date.setTime(this.to_date.getTime() + adj);
        }
      }

      // common attributes
      this.d.dir = timer_span.children("span[class=dir]").html() || this.d.dir;
      this.d.format_num = timer_span.children("span[class=format_num]").html() || this.d.format_num;
      this.d.format_txt = timer_span.children("span[class=format_txt]").html() || "";
      if ( String(this.d.format_txt).match("'") ) {
        this.d.format_txt = "<span style=\"color:red;\">Format may not contain single quotes(').</span>";
      }
      this.d.threshold = timer_span.children("span[class=threshold]").html() || this.d.threshold;
      this.d.timer_complete = timer_span.children("span[class=complete]").html() || this.d.timer_complete;
      this.d.tc_redir = timer_span.children("span[class=tc_redir]").html() || '';
      this.d.tc_msg = timer_span.children("span[class=tc_msg]").html() || '';

      this.parse_microformat_success = 1;
    }

    // update_timer: returns false if the timer is done.
    Drupal.countdowntimer.update_timer = function() {
      var timer_span = $(this.element);
      var now_date = new Date();
      var diff_secs;
      if ( this.d.dir == "down" ) {
        diff_secs = Math.floor((this.to_date.getTime() - now_date.getTime()) / 1000);
      } else {
        diff_secs = Math.floor((now_date.getTime() - this.to_date.getTime()) / 1000);
      }

      if ( this.d.dir == "down" && diff_secs < 0 ) {
        /* timer complete */
        timer_span.html(this.d.timer_complete.valueOf());

        if ( this.d.tc_msg != '' && diff_secs > -3 ) {
          alert(this.d.tc_msg);
          if ( this.d.tc_redir != '' ) {
            window.location = this.d.tc_redir;
          }
        } else if ( this.d.tc_redir != '' && diff_secs > -3) {
          window.location = this.d.tc_redir;
        }

        return false;
      } else {
        /* timer still counting */
        var years = Math.floor(diff_secs / 60 / 60 / 24 / 365.25);
        var days = Math.floor(diff_secs / 60 / 60 / 24);
        var ydays = Math.ceil(days - (years * 365.25));
        var hours = Math.floor(diff_secs / 60 / 60) - (days * 24);
        var minutes = Math.floor(diff_secs / 60) - (hours * 60) - (days * 24 * 60);
        var seconds = diff_secs - (minutes * 60) - (hours * 60 * 60) - (days * 24 * 60 * 60);

        var outhtml = new String(this.outformat);

        //handle all counts with units first
        var year_str = Drupal.formatPlural(years, "1 year", "@count years");
        outhtml = outhtml.replace(/%years% years/, year_str);
        var ydays_str = Drupal.formatPlural(ydays, "1 day", "@count days");
        outhtml = outhtml.replace(/%ydays% days/, ydays_str);
        var days_str = Drupal.formatPlural(days, "1 day", "@count days");
        outhtml = outhtml.replace(/%days% days/, days_str);
        var hours_str = Drupal.formatPlural(hours, "1 hour", "@count hours");
        outhtml = outhtml.replace(/%hours% hours/, hours_str);
        var mins_str = Drupal.formatPlural(minutes, "1 minute", "@count minutes");
        outhtml = outhtml.replace(/%mins% minutes/, mins_str);
        var secs_str = Drupal.formatPlural(seconds, "1 second", "@count seconds");
        outhtml = outhtml.replace(/%secs% seconds/, secs_str);

        //handle counts without units
        outhtml = outhtml.replace(/%years%/, years);
        outhtml = outhtml.replace(/%ydays%/, ydays);
        outhtml = outhtml.replace(/%days%/, days);
        outhtml = outhtml.replace(/%hours%/, LZ(hours));
        outhtml = outhtml.replace(/%mins%/, LZ(minutes));
        outhtml = outhtml.replace(/%secs%/, LZ(seconds));
        outhtml = outhtml.replace(/%hours_nopad%/, hours);
        outhtml = outhtml.replace(/%mins_nopad%/, minutes);
        outhtml = outhtml.replace(/%secs_nopad%/, seconds);

        if ( this.d.dir == "down" && (diff_secs <= (this.d.threshold * 60)) ) {
          timer_span.html('<span ' + this.d.highlight[0] + '=' + this.d.highlight[1] + '>' +  outhtml + '</span>');
        } else {
          timer_span.html(outhtml);
        }

        return true;
      }
    }

    // clock functions
    Drupal.countdowntimer.js_clock = function(_element) {
      this.element = _element;
      this.update = Drupal.countdowntimer.update_clock;
    }
    Drupal.countdowntimer.update_clock = function() {
      var timenow = new Date();
      var h = timenow.getHours();
      var m = timenow.getMinutes();
      var s = timenow.getSeconds();
      if ( '0' == '0' ) {
        var am_pm = ""
        if ( h <= 12 ) {
          am_pm = "am";
        } else {
          am_pm = "pm";
          h = h - 12;
        }
        $(this.element).html(h + ":" + LZ(m) + ":" + LZ(s) + am_pm);
      } else if ( '0' == '1' ) {
        $(this.element).html(h + ":" + LZ(m) + ":" + LZ(s));
      }
      return true;
    }


    // bootstrap and timing functions
    Drupal.countdowntimer.running = 0;
    Drupal.countdowntimer.timer_stack = new Array();

    function countdown_auto_attach() {
      $(".countdowntimer").each(
        function(i) {  // i is the position in the each fieldset
          var t = new Drupal.countdowntimer.jstimer(this,1);
          if ( t.parse_microformat_success == 1 ) {
            Drupal.countdowntimer.timer_stack[Drupal.countdowntimer.timer_stack.length] = t;
          }
          if ( Drupal.countdowntimer.running == 0 ) {
            Drupal.countdowntimer.running = 1;
            timer_loop();
          }
        }
      );
      $(".js-clock").each(
        function(i) {
          var t = new Drupal.countdowntimer.js_clock(this,1);
          Drupal.countdowntimer.timer_stack[Drupal.countdowntimer.timer_stack.length] = t;
          if ( Drupal.countdowntimer.running == 0 ) {
            Drupal.countdowntimer.running = 1;
            timer_loop();
          }
        }
      );
    }
    function timer_loop() {
      for (var i = Drupal.countdowntimer.timer_stack.length - 1; i >= 0; i--) {
        if ( Drupal.countdowntimer.timer_stack[i].update() == false ) {
          Drupal.countdowntimer.timer_stack.splice(i, 1);
        }
      }
      setTimeout('timer_loop()',999);
    }
    function LZ(x) {
      return (x >= 10 || x < 0 ? "" : "0") + x;
    }

    Date.prototype.countdowntimer_set_iso8601_date = function (string) {
      var iso8601_re = /^(?:(\d{4})(?:-(\d{2})(?:-(\d{2}))?)?)?(?:T(\d{2}):(\d{2})(?::(\d{2})(.\d+)?)?((?:[+-](\d{2}):(\d{2}))|Z)?)?$/;
      var date_bits = iso8601_re.exec(string);
      var date_obj = null;
      if ( date_bits ) {
        date_bits.shift();
        date_bits[1] && date_bits[1]--; // normalize month
        date_bits[6] && (date_bits[6] *= 1000); // convert mils
        date_obj = new Date(date_bits[0]||1970, date_bits[1]||0, date_bits[2]||0, date_bits[3]||0, date_bits[4]||0, date_bits[5]||0, date_bits[6]||0);

        //timezone handling
        var zone_offset = 0;  // in minutes
        var zone_plus_minus = date_bits[7] && date_bits[7].charAt(0);
        // get offset from isostring time to Z time
        if ( zone_plus_minus != 'Z' ) {
          zone_offset = ((date_bits[8] || 0) * 60) + (Number(date_bits[9]) || 0);
          if ( zone_plus_minus != '-' ) {
            zone_offset *= -1;
          }
        }
        // convert offset to localtime offset, will include daylight savings
        if ( zone_plus_minus ) {
          zone_offset -= date_obj.getTimezoneOffset();
        }
        if ( zone_offset ) {
          date_obj.setTime(date_obj.getTime() + zone_offset * 60000);
        }
      }

      // set this object to current localtime representation
      try {
        this.setTime(date_obj.getTime());
      }
      catch(e) {
        throw new Object({name:"DatePatternFail",message:"CountdownTimer: Date does not have proper format (ISO8601, see readme.txt)."});
      }
    }
    Date.prototype.countdowntimer_get_moy = function () {
      var myMonths=new Array("Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec");
      return myMonths[this.getMonth()];
    }
    Date.prototype.countdowntimer_get_dow = function () {
      var myDays=["Sun","Mon","Tue","Wed","Thu","Fri","Sat","Sun"];
      return myDays[this.getDay()];
    }

