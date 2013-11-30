<?php

/**
 * @file
 * Functions that need to be loaded on every Drupal request.
 */

/**
 * Indicates that the item should never be removed unless explicitly told to
 * using cache_clear_all() with a cache ID.
 */
define('CACHE_PERMANENT', 0);

/**
 * Indicates that the item should be removed at the next general cache wipe.
 */
define('CACHE_TEMPORARY', -1);

/**
 * Indicates that page caching is disabled.
 */
define('CACHE_DISABLED', 0);

/**
 * Indicates that page caching is enabled, using "normal" mode.
 */
define('CACHE_NORMAL', 1);

/**
 * Indicates that page caching is using "aggressive" mode. This bypasses
 * loading any modules for additional speed, which may break functionality in
 * modules that expect to be run on each page load.
 */
define('CACHE_AGGRESSIVE', 2);

/**
 * Indicates that page caching is using "external" mode. This disables the
 * internal page cache but returns headers allowing downstream caches (such
 * as Squid, Varnish, and other reverse proxies) to cache full pages. Like
 * "aggressive" mode, this may break functionality in some modules.
 */
define('CACHE_EXTERNAL', 3);

/**
 * Log message severity -- Emergency: system is unusable.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_EMERG', 0);

/**
 * Log message severity -- Alert: action must be taken immediately.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_ALERT', 1);

/**
 * Log message severity -- Critical: critical conditions.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_CRITICAL', 2);

/**
 * Log message severity -- Error: error conditions.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_ERROR', 3);

/**
 * Log message severity -- Warning: warning conditions.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_WARNING', 4);

/**
 * Log message severity -- Notice: normal but significant condition.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_NOTICE', 5);

/**
 * Log message severity -- Informational: informational messages.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_INFO', 6);

/**
 * Log message severity -- Debug: debug-level messages.
 *
 * The WATCHDOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1: http://www.faqs.org/rfcs/rfc3164.html
 *
 * @see watchdog()
 * @see watchdog_severity_levels()
 */
define('WATCHDOG_DEBUG', 7);

/**
 * First bootstrap phase: initialize configuration.
 */
define('DRUPAL_BOOTSTRAP_CONFIGURATION', 0);

/**
 * Second bootstrap phase: try to call a non-database cache
 * fetch routine.
 */
define('DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE', 1);

/**
 * Third bootstrap phase: initialize database layer.
 */
define('DRUPAL_BOOTSTRAP_DATABASE', 2);

/**
 * Fourth bootstrap phase: identify and reject banned hosts.
 */
define('DRUPAL_BOOTSTRAP_ACCESS', 3);

/**
 * Fifth bootstrap phase: initialize session handling.
 */
define('DRUPAL_BOOTSTRAP_SESSION', 4);

/**
 * Sixth bootstrap phase: load bootstrap.inc and module.inc, start
 * the variable system and try to serve a page from the cache.
 */
define('DRUPAL_BOOTSTRAP_LATE_PAGE_CACHE', 5);

/**
 * Seventh bootstrap phase: find out language of the page.
 */
define('DRUPAL_BOOTSTRAP_LANGUAGE', 6);

/**
 * Eighth bootstrap phase: set $_GET['q'] to Drupal path of request.
 */
define('DRUPAL_BOOTSTRAP_PATH', 7);

/**
 * Final bootstrap phase: Drupal is fully loaded; validate and fix
 * input data.
 */
define('DRUPAL_BOOTSTRAP_FULL', 8);

/**
 * Role ID for anonymous users; should match what's in the "role" table.
 */
define('DRUPAL_ANONYMOUS_RID', 1);

/**
 * Role ID for authenticated users; should match what's in the "role" table.
 */
define('DRUPAL_AUTHENTICATED_RID', 2);

/**
 * No language negotiation. The default language is used.
 */
define('LANGUAGE_NEGOTIATION_NONE', 0);

/**
 * Path based negotiation with fallback to default language
 * if no defined path prefix identified.
 */
define('LANGUAGE_NEGOTIATION_PATH_DEFAULT', 1);

/**
 * Path based negotiation with fallback to user preferences
 * and browser language detection if no defined path prefix
 * identified.
 */
define('LANGUAGE_NEGOTIATION_PATH', 2);

/**
 * Domain based negotiation with fallback to default language
 * if no language identified by domain.
 */
define('LANGUAGE_NEGOTIATION_DOMAIN', 3);

/**
 * Language written left to right. Possible value of $language->direction.
 */
define('LANGUAGE_LTR', 0);

/**
 * Language written right to left. Possible value of $language->direction.
 */
define('LANGUAGE_RTL', 1);

// Hide E_DEPRECATED messages.
if (defined('E_DEPRECATED')) {
  error_reporting(error_reporting() & ~E_DEPRECATED);
}

/**
 * Start the timer with the specified name. If you start and stop
 * the same timer multiple times, the measured intervals will be
 * accumulated.
 *
 * @param name
 *   The name of the timer.
 */
function timer_start($name) {
  global $timers;

  list($usec, $sec) = explode(' ', microtime());
  $timers[$name]['start'] = (float)$usec + (float)$sec;
  $timers[$name]['count'] = isset($timers[$name]['count']) ? ++$timers[$name]['count'] : 1;
}

/**
 * Read the current timer value without stopping the timer.
 *
 * @param name
 *   The name of the timer.
 * @return
 *   The current timer value in ms.
 */
function timer_read($name) {
  global $timers;

  if (isset($timers[$name]['start'])) {
    list($usec, $sec) = explode(' ', microtime());
    $stop = (float)$usec + (float)$sec;
    $diff = round(($stop - $timers[$name]['start']) * 1000, 2);

    if (isset($timers[$name]['time'])) {
      $diff += $timers[$name]['time'];
    }
    return $diff;
  }
}

/**
 * Stop the timer with the specified name.
 *
 * @param name
 *   The name of the timer.
 * @return
 *   A timer array. The array contains the number of times the
 *   timer has been started and stopped (count) and the accumulated
 *   timer value in ms (time).
 */
function timer_stop($name) {
  global $timers;

  $timers[$name]['time'] = timer_read($name);
  unset($timers[$name]['start']);

  return $timers[$name];
}

/**
 * Find the appropriate configuration directory.
 *
 * Try finding a matching configuration directory by stripping the website's
 * hostname from left to right and pathname from right to left. The first
 * configuration file found will be used; the remaining will ignored. If no
 * configuration file is found, return a default value '$confdir/default'.
 *
 * Example for a fictitious site installed at
 * http://www.drupal.org:8080/mysite/test/ the 'settings.php' is searched in
 * the following directories:
 *
 *  1. $confdir/8080.www.drupal.org.mysite.test
 *  2. $confdir/www.drupal.org.mysite.test
 *  3. $confdir/drupal.org.mysite.test
 *  4. $confdir/org.mysite.test
 *
 *  5. $confdir/8080.www.drupal.org.mysite
 *  6. $confdir/www.drupal.org.mysite
 *  7. $confdir/drupal.org.mysite
 *  8. $confdir/org.mysite
 *
 *  9. $confdir/8080.www.drupal.org
 * 10. $confdir/www.drupal.org
 * 11. $confdir/drupal.org
 * 12. $confdir/org
 *
 * 13. $confdir/default
 *
 * @param $require_settings
 *   Only configuration directories with an existing settings.php file
 *   will be recognized. Defaults to TRUE. During initial installation,
 *   this is set to FALSE so that Drupal can detect a matching directory,
 *   then create a new settings.php file in it.
 * @param reset
 *   Force a full search for matching directories even if one had been
 *   found previously.
 * @return
 *   The path of the matching directory.
 */
function conf_path($require_settings = TRUE, $reset = FALSE) {
  static $conf = '';

  if ($conf && !$reset) {
    return $conf;
  }

  $confdir = 'sites';
  $uri = explode('/', $_SERVER['SCRIPT_NAME'] ? $_SERVER['SCRIPT_NAME'] : $_SERVER['SCRIPT_FILENAME']);
  $server = explode('.', implode('.', array_reverse(explode(':', rtrim($_SERVER['HTTP_HOST'], '.')))));
  for ($i = count($uri) - 1; $i > 0; $i--) {
    for ($j = count($server); $j > 0; $j--) {
      $dir = implode('.', array_slice($server, -$j)) . implode('.', array_slice($uri, 0, $i));
      if (file_exists("$confdir/$dir/settings.php") || (!$require_settings && file_exists("$confdir/$dir"))) {
        $conf = "$confdir/$dir";
        return $conf;
      }
    }
  }
  $conf = "$confdir/default";
  return $conf;
}

/**
 * Unsets all disallowed global variables. See $allowed for what's allowed.
 */
function drupal_unset_globals() {
  if (ini_get('register_globals')) {
    $allowed = array('_ENV' => 1, '_GET' => 1, '_POST' => 1, '_COOKIE' => 1, '_FILES' => 1, '_SERVER' => 1, '_REQUEST' => 1, 'GLOBALS' => 1);
    foreach ($GLOBALS as $key => $value) {
      if (!isset($allowed[$key])) {
        unset($GLOBALS[$key]);
      }
    }
  }
}

/**
 * Validate that a hostname (for example $_SERVER['HTTP_HOST']) is safe.
 *
 * As $_SERVER['HTTP_HOST'] is user input, ensure it only contains characters
 * allowed in hostnames.  See RFC 952 (and RFC 2181). $_SERVER['HTTP_HOST'] is
 * lowercased.
 *
 * @return
 *  TRUE if only containing valid characters, or FALSE otherwise.
 */
function drupal_valid_http_host($host) {
  return preg_match('/^\[?(?:[a-z0-9-:\]_]+\.?)+$/', $host);
}

/**
 * Loads the configuration and sets the base URL, cookie domain, and
 * session name correctly.
 */
function conf_init() {
  global $base_url, $base_path, $base_root;

  // Export the following settings.php variables to the global namespace
  global $db_url, $db_slave_url, $db_prefix, $db_collation, $cookie_domain, $conf, $installed_profile, $update_free_access;
  $conf = array();

  if (!isset($_SERVER['SERVER_PROTOCOL']) || ($_SERVER['SERVER_PROTOCOL'] != 'HTTP/1.0' && $_SERVER['SERVER_PROTOCOL'] != 'HTTP/1.1')) {
    $_SERVER['SERVER_PROTOCOL'] = 'HTTP/1.0';
  }

  if (isset($_SERVER['HTTP_HOST'])) {
    // As HTTP_HOST is user input, ensure it only contains characters allowed
    // in hostnames. See RFC 952 (and RFC 2181).
    // $_SERVER['HTTP_HOST'] is lowercased here per specifications.
    $_SERVER['HTTP_HOST'] = strtolower($_SERVER['HTTP_HOST']);
    if (!drupal_valid_http_host($_SERVER['HTTP_HOST'])) {
      // HTTP_HOST is invalid, e.g. if containing slashes it may be an attack.
      header($_SERVER['SERVER_PROTOCOL'] .' 400 Bad Request');
      exit;
    }
  }
  else {
    // Some pre-HTTP/1.1 clients will not send a Host header. Ensure the key is
    // defined for E_ALL compliance.
    $_SERVER['HTTP_HOST'] = '';
  }

  if (file_exists('./'. conf_path() .'/settings.php')) {
    include_once './'. conf_path() .'/settings.php';
  }

  // Ignore the placeholder URL from default.settings.php.
  if (isset($db_url) && $db_url == 'mysql://username:password@localhost/databasename') {
    $db_url = '';
  }

  if (isset($base_url)) {
    // Parse fixed base URL from settings.php.
    $parts = parse_url($base_url);
    if (!isset($parts['path'])) {
      $parts['path'] = '';
    }
    $base_path = $parts['path'] .'/';
    // Build $base_root (everything until first slash after "scheme://").
    $base_root = substr($base_url, 0, strlen($base_url) - strlen($parts['path']));
  }
  else {
    // Create base URL
    $base_root = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';

    $base_url = $base_root .= '://'. $_SERVER['HTTP_HOST'];

    // $_SERVER['SCRIPT_NAME'] can, in contrast to $_SERVER['PHP_SELF'], not
    // be modified by a visitor.
    if ($dir = trim(dirname($_SERVER['SCRIPT_NAME']), '\,/')) {
      $base_path = "/$dir";
      $base_url .= $base_path;
      $base_path .= '/';
    }
    else {
      $base_path = '/';
    }
  }

  if ($cookie_domain) {
    // If the user specifies the cookie domain, also use it for session name.
    $session_name = $cookie_domain;
  }
  else {
    // Otherwise use $base_url as session name, without the protocol
    // to use the same session identifiers across HTTP and HTTPS.
    list( , $session_name) = explode('://', $base_url, 2);
    // We escape the hostname because it can be modified by a visitor.
    if (!empty($_SERVER['HTTP_HOST'])) {
      $cookie_domain = check_plain($_SERVER['HTTP_HOST']);
      // Strip leading periods, www., and port numbers from cookie domain.
      $cookie_domain = ltrim($cookie_domain, '.');
      if (strpos($cookie_domain, 'www.') === 0) {
        $cookie_domain = substr($cookie_domain, 4);
      }
      $cookie_domain = explode(':', $cookie_domain);
      $cookie_domain = '.'. $cookie_domain[0];
    }
  }
  // To prevent session cookies from being hijacked, a user can configure the
  // SSL version of their website to only transfer session cookies via SSL by
  // using PHP's session.cookie_secure setting. The browser will then use two
  // separate session cookies for the HTTPS and HTTP versions of the site. So we
  // must use different session identifiers for HTTPS and HTTP to prevent a
  // cookie collision.
  if (ini_get('session.cookie_secure')) {
    $session_name .= 'SSL';
  }
  // Per RFC 2109, cookie domains must contain at least one dot other than the
  // first. For hosts such as 'localhost' or IP Addresses we don't set a cookie domain.
  if (count(explode('.', $cookie_domain)) > 2 && !is_numeric(str_replace('.', '', $cookie_domain))) {
    ini_set('session.cookie_domain', $cookie_domain);
  }

  // Use httponly session cookies.
  ini_set('session.cookie_httponly', '1');

  session_name('SESS'. md5($session_name));
}

/**
 * Returns and optionally sets the filename for a system item (module,
 * theme, etc.). The filename, whether provided, cached, or retrieved
 * from the database, is only returned if the file exists.
 *
 * This function plays a key role in allowing Drupal's resources (modules
 * and themes) to be located in different places depending on a site's
 * configuration. For example, a module 'foo' may legally be be located
 * in any of these three places:
 *
 * modules/foo/foo.module
 * sites/all/modules/foo/foo.module
 * sites/example.com/modules/foo/foo.module
 *
 * Calling drupal_get_filename('module', 'foo') will give you one of
 * the above, depending on where the module is located.
 *
 * @param $type
 *   The type of the item (i.e. theme, theme_engine, module, profile).
 * @param $name
 *   The name of the item for which the filename is requested.
 * @param $filename
 *   The filename of the item if it is to be set explicitly rather
 *   than by consulting the database.
 *
 * @return
 *   The filename of the requested item.
 */
function drupal_get_filename($type, $name, $filename = NULL) {
  static $files = array();

  if (!isset($files[$type])) {
    $files[$type] = array();
  }

  if (!empty($filename) && file_exists($filename)) {
    $files[$type][$name] = $filename;
  }
  elseif (isset($files[$type][$name])) {
    // nothing
  }
  // Verify that we have an active database connection, before querying
  // the database.  This is required because this function is called both
  // before we have a database connection (i.e. during installation) and
  // when a database connection fails.
  elseif (db_is_active() && (($file = db_result(db_query("SELECT filename FROM {system} WHERE name = '%s' AND type = '%s'", $name, $type))) && file_exists($file))) {
    $files[$type][$name] = $file;
  }
  else {
    // Fallback to searching the filesystem if the database connection is
    // not established or the requested file is not found.
    $config = conf_path();
    $dir = (($type == 'theme_engine') ? 'themes/engines' : "${type}s");
    $file = (($type == 'theme_engine') ? "$name.engine" : "$name.$type");

    foreach (array("$config/$dir/$file", "$config/$dir/$name/$file", "$dir/$file", "$dir/$name/$file") as $file) {
      if (file_exists($file)) {
        $files[$type][$name] = $file;
        break;
      }
    }
  }

  if (isset($files[$type][$name])) {
    return $files[$type][$name];
  }
}

/**
 * Load the persistent variable table.
 *
 * The variable table is composed of values that have been saved in the table
 * with variable_set() as well as those explicitly specified in the configuration
 * file.
 */
function variable_init($conf = array(), $regenerate = FALSE, $recursion_depth = 0) {
  // NOTE: caching the variables improves performance by 20% when serving cached pages.
  if (!$regenerate && $cached = cache_get('variables', 'cache')) {
    $variables = $cached->data;
  }
  else {
    if (defined('MAINTENANCE_MODE') || lock_acquire('variable_cache_regenerate')) {
      $result = db_query('SELECT * FROM {variable}');
      while ($variable = db_fetch_object($result)) {
        $variables[$variable->name] = unserialize($variable->value);
      }
      cache_set('variables', $variables);
      if (!defined('MAINTENANCE_MODE')) {
        lock_release('variable_cache_regenerate');
      }
    }
    else {
      // Wait for another request that is already doing this work.
      lock_wait('variable_cache_regenerate');

      // Run the function again. Try a limited number of times to avoid 
      // infinite recursion if the database connection is invalid for  
      // some reason, e.g., mysqld restart, loss of network, etc.
      $recursion_depth++;
      if ($recursion_depth < 50) {
        return variable_init($conf, $regenerate, $recursion_depth);
      }

      $variables = array();
    }
  }

  foreach ($conf as $name => $value) {
    $variables[$name] = $value;
  }

  return $variables;
}

/**
 * Returns a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to return.
 * @param $default
 *   The default value to use if this variable has never been set.
 * @return
 *   The value of the variable.
 *
 * @see variable_del(), variable_set()
 */
function variable_get($name, $default) {
  global $conf;

  return isset($conf[$name]) ? $conf[$name] : $default;
}

/**
 * Sets a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set. This can be any PHP data type; these functions take care
 *   of serialization as necessary.
 *
 * @see variable_del(), variable_get()
 */
function variable_set($name, $value) {
  global $conf, $db_prefix;

  $serialized_value = serialize($value);
  db_query("UPDATE {variable} SET value = '%s' WHERE name = '%s'", $serialized_value, $name);
  if (!db_affected_rows()) {
    @db_query("INSERT INTO {variable} (name, value) VALUES ('%s', '%s')", $name, $serialized_value);
  }

  $conf[$name] = $value;

  // The write-through rebuild optimization isn't compatible with SimpleTest.
  // Because array-based prefixes don't work with SimpleTest, we can assume
  // that a non-string prefix indicates lack of SimpleTest operations.
  if (is_string($db_prefix) && strpos($db_prefix, 'simpletest') === 0) {
    cache_clear_all('variables', 'cache');
  }
  
  variable_cache_rebuild();
}

/**
 * Unsets a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see variable_get(), variable_set()
 */
function variable_del($name) {
  global $conf, $db_prefix;

  db_query("DELETE FROM {variable} WHERE name = '%s'", $name);

  unset($conf[$name]);

  // The write-through rebuild optimization isn't compatible with SimpleTest.
  // Because array-based prefixes don't work with SimpleTest, we can assume
  // that a non-string prefix indicates lack of SimpleTest operations.
  if (is_string($db_prefix) && strpos($db_prefix, 'simpletest') === 0) {
    cache_clear_all('variables', 'cache');
  }
  
  variable_cache_rebuild();
}

/**
 * Schedules a rebuild of the variable cache on shutdown.
 */
function variable_cache_rebuild() {
  static $shutdown_registered = FALSE;
  if (!$shutdown_registered) {
    register_shutdown_function('variable_init', array(), TRUE);
    $shutdown_registered = TRUE;
  }
}

/**
 * Retrieve the current page from the cache.
 *
 * Note: we do not serve cached pages to authenticated users, or to anonymous
 * users when $_SESSION is non-empty. $_SESSION may contain status messages
 * from a form submission, the contents of a shopping cart, or other user-
 * specific content that should not be cached and displayed to other users.
 *
 * @return
 *   The cache object, if the page was found in the cache.
 */
function page_get_cache() {
  global $user, $base_root;

  if (drupal_page_is_cacheable()) {
    $cache = cache_get($base_root . request_uri(), 'cache_page');

    // Unserialize the cached page headers
    if (isset($cache->headers)) {
      $cache->headers = unserialize($cache->headers);
    }

    if ($cache) {
      return $cache;
    }
  }
}

function drupal_page_is_cacheable($force = NULL) {
  static $forced_cache = TRUE;
  if (isset($force)) {
    $forced_cache = $force;
  }

  $result = $forced_cache
    && !drupal_session_started()
    && ($_SERVER['REQUEST_METHOD'] == 'GET' || $_SERVER['REQUEST_METHOD'] == 'HEAD')
    && !count(drupal_get_messages(NULL, FALSE))
    && !drupal_is_cli();

  return $result;
}

/**
 * Call all init or exit hooks without including all modules.
 *
 * @param $hook
 *   The name of the bootstrap hook we wish to invoke.
 */
function bootstrap_invoke_all($hook) {
  foreach (module_list(TRUE, TRUE) as $module) {
    drupal_load('module', $module);
    module_invoke($module, $hook);
  }
}

/**
 * Includes a file with the provided type and name. This prevents
 * including a theme, engine, module, etc., more than once.
 *
 * @param $type
 *   The type of item to load (i.e. theme, theme_engine, module, profile).
 * @param $name
 *   The name of the item to load.
 *
 * @return
 *   TRUE if the item is loaded or has already been loaded.
 */
function drupal_load($type, $name) {
  static $files = array();

  if (isset($files[$type][$name])) {
    return TRUE;
  }

  $filename = drupal_get_filename($type, $name);

  if ($filename) {
    include_once "./$filename";
    $files[$type][$name] = TRUE;

    return TRUE;
  }

  return FALSE;
}

/**
 * Set an HTTP response header for the current page.
 *
 * Note: When sending a Content-Type header, always include a 'charset' type,
 * too. This is necessary to avoid security bugs (e.g. UTF-7 XSS).
 *
 * @param $name
 *   The HTTP header name, or a status code followed by a reason phrase, e.g.
 *   "404 Not Found".
 * @param $value
 *   The HTTP header value; if omitted, the specified header is unset.
 * @param $append
 *   Whether to append the value to an existing header or to replace it.
 */
function drupal_set_header($name = NULL, $value = NULL, $append = FALSE) {
  // The headers as name/value pairs.
  static $headers = array();

  if (!isset($name)) {
    return $headers;
  }
  
  // Support the Drupal 6 header API
  if (!isset($value)) {
    if (strpos($name, ':') !== FALSE) {
      $parts = explode(':', $name, 2);
      $name = trim($parts[0]);
      $value = trim($parts[1]);
    }
  }
  if (substr($name, 0, 7) == 'HTTP/1.') {
    $name = substr($name, 9);
  }

  // Save status codes using the special key ":status".
  if (preg_match('/^\d{3} /', $name)) {
    $value = $name;
    $name = $name_lower = ':status';
  }
  else {
    $name_lower = strtolower($name);
  }
  _drupal_set_preferred_header_name($name);

  if (!isset($value)) {
    $headers[$name_lower] = FALSE;
  }
  elseif (isset($headers[$name_lower]) && $append) {
    // Multiple headers with identical names may be combined using comma (RFC
    // 2616, section 4.2).
    $headers[$name_lower] .= ',' . $value;
  }
  else {
    $headers[$name_lower] = $value;
  }
  drupal_send_headers(array($name => $headers[$name_lower]), TRUE);
}

/**
 * Get the HTTP response headers for the current page.
 *
 * @param $name
 *   An HTTP header name. If omitted, all headers are returned as name/value
 *   pairs. If an array value is FALSE, the header has been unset.
 * @return
 *   A string containing the header value, or FALSE if the header has been set,
 *   or NULL if the header has not been set.
 */
function drupal_get_header($name = NULL) {
  $headers = drupal_set_header();
  if (isset($name)) {
    $name = strtolower($name);
    return isset($headers[$name]) ? $headers[$name] : NULL;
  }
  else {
    return $headers;
  }
}

/**
 * Header names are case-insensitive, but for maximum compatibility they should
 * follow "common form" (see RFC 2617, section 4.2).
 */
function _drupal_set_preferred_header_name($name = NULL) {
  static $header_names = array();

  if (!isset($name)) {
    return $header_names;
  }
  $header_names[strtolower($name)] = $name;
}

/**
 * Send the HTTP response headers previously set using drupal_set_header().
 * Add default headers, unless they have been replaced or unset using
 * drupal_set_header().
 *
 * @param $default_headers
 *   An array of headers as name/value pairs.
 * @param $single
 *   If TRUE and headers have already be sent, send only the specified header.
 */
function drupal_send_headers($default_headers = array(), $only_default = FALSE) {
  static $headers_sent = FALSE;
  $headers = drupal_get_header();
  if ($only_default && $headers_sent) {
    $headers = array();
  }
  $headers_sent = TRUE;

  $header_names = _drupal_set_preferred_header_name();
  foreach ($default_headers as $name => $value) {
    $name_lower = strtolower($name);
    if (!isset($headers[$name_lower])) {
      $headers[$name_lower] = $value;
      $header_names[$name_lower] = $name;
    }
  }
  foreach ($headers as $name_lower => $value) {
    if ($name_lower == ':status') {
      header($_SERVER['SERVER_PROTOCOL'] . ' ' . $value);
    }
    // Skip headers that have been unset.
    elseif ($value) {
      header($header_names[$name_lower] . ': ' . $value);
    }
  }
}

/**
 * Set HTTP headers in preparation for a page response.
 *
 * Authenticated users are always given a 'no-cache' header, and will fetch a
 * fresh page on every request. This prevents authenticated users from seeing
 * locally cached pages.
 *
 * Also give each page a unique ETag. This will force clients to include both
 * an If-Modified-Since header and an If-None-Match header when doing
 * conditional requests for the page (required by RFC 2616, section 13.3.4),
 * making the validation more robust. This is a workaround for a bug in Mozilla
 * Firefox that is triggered when Drupal's caching is enabled and the user
 * accesses Drupal via an HTTP proxy (see
 * https://bugzilla.mozilla.org/show_bug.cgi?id=269303): When an authenticated
 * user requests a page, and then logs out and requests the same page again,
 * Firefox may send a conditional request based on the page that was cached
 * locally when the user was logged in. If this page did not have an ETag
 * header, the request only contains an If-Modified-Since header. The date will
 * be recent, because with authenticated users the Last-Modified header always
 * refers to the time of the request. If the user accesses Drupal via a proxy
 * server, and the proxy already has a cached copy of the anonymous page with an
 * older Last-Modified date, the proxy may respond with 304 Not Modified, making
 * the client think that the anonymous and authenticated pageviews are
 * identical.
 *
 * @see page_set_cache()
 */
function drupal_page_header() {
  static $headers_sent = FALSE;
  if ($headers_sent) {
    return TRUE;
  }
  $headers_sent = TRUE;

  $default_headers = array(
    'Last-Modified' => gmdate(DATE_RFC1123, $_SERVER['REQUEST_TIME']),
    'Cache-Control' => 'no-cache, must-revalidate, post-check=0, pre-check=0',
    'ETag' => '"' . $_SERVER['REQUEST_TIME'] . '"',
  );
  drupal_send_headers($default_headers);
}

/**
 * Set HTTP headers in for downstream caching.
 *
 * The headers allow as much as possible in proxies and browsers without any
 * particular knowledge about the pages. Used for the external caching mode.
 */
function drupal_page_cache_header_external() {
  // Get headers set in hook_boot(). Keys are lower-case.
  $hook_boot_headers = drupal_get_header();

  $max_age = variable_get('page_cache_max_age', 0);
  drupal_set_header('Cache-Control', 'public, max-age=' . $max_age);
  drupal_set_header('Last-Modified', gmdate(DATE_RFC1123, $_SERVER['REQUEST_TIME']));

  // HTTP/1.0 proxies do not support the Vary header, so prevent any caching
  // by sending an Expires date in the past. HTTP/1.1 clients ignores the
  // Expires header if a Cache-Control: max-age= directive is specified (see RFC
  // 2616, section 14.9.3).
  drupal_set_header('Expires', 'Sun, 11 Mar 1984 12:00:00 GMT');

  // Allow HTTP proxies to cache pages for anonymous users without a session
  // cookie. The Vary header is used to indicates the set of request-header
  // fields that fully determines whether a cache is permitted to use the
  // response to reply to a subsequent request for a given URL without
  // revalidation. If a Vary header has been set in hook_boot(), it is assumed
  // that the module knows how to cache the page.
  if (!isset($hook_boot_headers['vary']) && !variable_get('omit_vary_cookie', FALSE)) {
    drupal_set_header('Vary', 'Cookie');
  }
}


/**
 * Set HTTP headers in preparation for a cached page response.
 *
 * The headers allow as much as possible in proxies and browsers without any
 * particular knowledge about the pages. Modules can override these headers
 * using drupal_set_header().
 *
 * If the request is conditional (using If-Modified-Since and If-None-Match),
 * and the conditions match those currently in the cache, a 304 Not Modified
 * response is sent.
 */
function drupal_page_cache_header(stdClass $cache) {
  // Negotiate whether to use compression.
  $page_compression = variable_get('page_compression', TRUE) && extension_loaded('zlib');
  $return_compressed = $page_compression && isset($_SERVER['HTTP_ACCEPT_ENCODING']) && strpos($_SERVER['HTTP_ACCEPT_ENCODING'], 'gzip') !== FALSE;

  // Get headers set in hook_boot(). Keys are lower-case.
  $hook_boot_headers = drupal_get_header();

  // Headers generated in this function, that may be replaced or unset using
  // drupal_set_headers(). Keys are mixed-case.
  $default_headers = array();

  foreach ($cache->headers as $name => $value) {
    // In the case of a 304 response, certain headers must be sent, and the
    // remaining may not (see RFC 2616, section 10.3.5). Do not override
    // headers set in hook_boot().
    $name_lower = strtolower($name);
    if (in_array($name_lower, array('content-location', 'expires', 'cache-control', 'vary')) && !isset($hook_boot_headers[$name_lower])) {
      drupal_set_header($name, $value);
      unset($cache->headers[$name]);
    }
  }

  // If a cache is served from a HTTP proxy without hitting the web server,
  // the boot and exit hooks cannot be fired, so only allow caching in
  // proxies with aggressive caching. If the client send a session cookie, do
  // not bother caching the page in a public proxy, because the cached copy
  // will only be served to that particular user due to Vary: Cookie, unless
  // the Vary header has been replaced or unset in hook_boot() (see below).
  $max_age = variable_get('cache', CACHE_DISABLED) == CACHE_AGGRESSIVE && (!isset($_COOKIE[session_name()]) || isset($hook_boot_headers['vary'])) ? variable_get('page_cache_max_age', 0) : 0;
  $default_headers['Cache-Control'] = 'public, max-age=' . $max_age;

  // Entity tag should change if the output changes.
  $etag = '"' . $cache->created . '-' . intval($return_compressed) . '"';
  header('Etag: ' . $etag);

  // See if the client has provided the required HTTP headers.
  $if_modified_since = isset($_SERVER['HTTP_IF_MODIFIED_SINCE']) ? strtotime($_SERVER['HTTP_IF_MODIFIED_SINCE']) : FALSE;
  $if_none_match = isset($_SERVER['HTTP_IF_NONE_MATCH']) ? stripslashes($_SERVER['HTTP_IF_NONE_MATCH']) : FALSE;

  if ($if_modified_since && $if_none_match
      && $if_none_match == $etag // etag must match
      && $if_modified_since == $cache->created) {  // if-modified-since must match
    header($_SERVER['SERVER_PROTOCOL'] . ' 304 Not Modified');
    drupal_send_headers($default_headers);
    return;
  }

  // Send the remaining headers.
  foreach ($cache->headers as $name => $value) {
    drupal_set_header($name, $value);
  }

  $default_headers['Last-Modified'] = gmdate(DATE_RFC1123, $cache->created);

  // HTTP/1.0 proxies do not support the Vary header, so prevent any caching
  // by sending an Expires date in the past. HTTP/1.1 clients ignores the
  // Expires header if a Cache-Control: max-age= directive is specified (see RFC
  // 2616, section 14.9.3).
  $default_headers['Expires'] = 'Sun, 11 Mar 1984 12:00:00 GMT';

  drupal_send_headers($default_headers);

  // Allow HTTP proxies to cache pages for anonymous users without a session
  // cookie. The Vary header is used to indicates the set of request-header
  // fields that fully determines whether a cache is permitted to use the
  // response to reply to a subsequent request for a given URL without
  // revalidation. If a Vary header has been set in hook_boot(), it is assumed
  // that the module knows how to cache the page.
  if (!isset($hook_boot_headers['vary']) && !variable_get('omit_vary_cookie', FALSE)) {
    header('Vary: Cookie');
  }

  if ($page_compression) {
    header('Vary: Accept-Encoding', FALSE);
    // If page_compression is enabled, the cache contains gzipped data.
    if ($return_compressed) {
      ini_set('zlib.output_compression', '0');
      header('Content-Encoding: gzip');
    }
    else {
      // The client does not support compression, so unzip the data in the
      // cache. Strip the gzip header and run uncompress.
      $cache->data = gzinflate(substr(substr($cache->data, 10), 0, -8));
      header('X-PF-Uncompressing: 1');
    }
  }

  print $cache->data;
}

/**
 * Define the critical hooks that force modules to always be loaded.
 */
function bootstrap_hooks() {
  return array('boot', 'exit');
}

/**
 * Unserializes and appends elements from a serialized string.
 *
 * @param $obj
 *   The object to which the elements are appended.
 * @param $field
 *   The attribute of $obj whose value should be unserialized.
 */
function drupal_unpack($obj, $field = 'data') {
  if ($obj->$field && $data = unserialize($obj->$field)) {
    foreach ($data as $key => $value) {
      if (!empty($key) && !isset($obj->$key)) {
        $obj->$key = $value;
      }
    }
  }
  return $obj;
}

/**
 * Return the URI of the referring page.
 */
function referer_uri() {
  if (isset($_SERVER['HTTP_REFERER'])) {
    return $_SERVER['HTTP_REFERER'];
  }
}

/**
 * Encode special characters in a plain-text string for display as HTML.
 *
 * Also validates strings as UTF-8 to prevent cross site scripting attacks on
 * Internet Explorer 6.
 *
 * @param $text
 *   The text to be checked or processed.
 * @return
 *   An HTML safe version of $text, or an empty string if $text is not
 *   valid UTF-8.
 *
 * @see drupal_validate_utf8().
 */
function check_plain($text) {
  static $php525;

  if (!isset($php525)) {
    $php525 = version_compare(PHP_VERSION, '5.2.5', '>=');
  }
  // We duplicate the preg_match() to validate strings as UTF-8 from
  // drupal_validate_utf8() here. This avoids the overhead of an additional
  // function call, since check_plain() may be called hundreds of times during
  // a request. For PHP 5.2.5+, this check for valid UTF-8 should be handled
  // internally by PHP in htmlspecialchars().
  // @see http://www.php.net/releases/5_2_5.php
  // @todo remove this when support for either IE6 or PHP < 5.2.5 is dropped.

  if ($php525) {
    return htmlspecialchars($text, ENT_QUOTES, 'UTF-8');
  }
  return (preg_match('/^./us', $text) == 1) ? htmlspecialchars($text, ENT_QUOTES, 'UTF-8') : '';
}

/**
 * Checks whether a string is valid UTF-8.
 *
 * All functions designed to filter input should use drupal_validate_utf8
 * to ensure they operate on valid UTF-8 strings to prevent bypass of the
 * filter.
 *
 * When text containing an invalid UTF-8 lead byte (0xC0 - 0xFF) is presented
 * as UTF-8 to Internet Explorer 6, the program may misinterpret subsequent
 * bytes. When these subsequent bytes are HTML control characters such as
 * quotes or angle brackets, parts of the text that were deemed safe by filters
 * end up in locations that are potentially unsafe; An onerror attribute that
 * is outside of a tag, and thus deemed safe by a filter, can be interpreted
 * by the browser as if it were inside the tag.
 *
 * This function exploits preg_match behaviour (since PHP 4.3.5) when used
 * with the u modifier, as a fast way to find invalid UTF-8. When the matched
 * string contains an invalid byte sequence, it will fail silently.
 *
 * preg_match may not fail on 4 and 5 octet sequences, even though they
 * are not supported by the specification.
 *
 * The specific preg_match behaviour is present since PHP 4.3.5.
 *
 * @param $text
 *   The text to check.
 * @return
 *   TRUE if the text is valid UTF-8, FALSE if not.
 */
function drupal_validate_utf8($text) {
  if (strlen($text) == 0) {
    return TRUE;
  }
  // For performance reasons this logic is duplicated in check_plain().
  return (preg_match('/^./us', $text) == 1);
}

/**
 * Since $_SERVER['REQUEST_URI'] is only available on Apache, we
 * generate an equivalent using other environment variables.
 */
function request_uri() {

  if (isset($_SERVER['REQUEST_URI'])) {
    $uri = $_SERVER['REQUEST_URI'];
  }
  else {
    if (isset($_SERVER['argv'])) {
      $uri = $_SERVER['SCRIPT_NAME'] .'?'. $_SERVER['argv'][0];
    }
    elseif (isset($_SERVER['QUERY_STRING'])) {
      $uri = $_SERVER['SCRIPT_NAME'] .'?'. $_SERVER['QUERY_STRING'];
    }
    else {
      $uri = $_SERVER['SCRIPT_NAME'];
    }
  }
  // Prevent multiple slashes to avoid cross site requests via the FAPI.
  $uri = '/'. ltrim($uri, '/');

  return $uri;
}

/**
 * Log a system message.
 *
 * @param $type
 *   The category to which this message belongs. Can be any string, but the
 *   general practice is to use the name of the module calling watchdog().
 * @param $message
 *   The message to store in the log. See t() for documentation
 *   on how $message and $variables interact. Keep $message
 *   translatable by not concatenating dynamic values into it!
 * @param $variables
 *   Array of variables to replace in the message on display or
 *   NULL if message is already translated or not possible to
 *   translate.
 * @param $severity
 *   The severity of the message, as per RFC 3164. Possible values are
 *   WATCHDOG_ERROR, WATCHDOG_WARNING, etc.
 * @param $link
 *   A link to associate with the message.
 *
 * @see watchdog_severity_levels()
 */
function watchdog($type, $message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL) {
  global $user, $base_root;

  // Prepare the fields to be logged
  $log_message = array(
    'type'        => $type,
    'message'     => $message,
    'variables'   => $variables,
    'severity'    => $severity,
    'link'        => $link,
    'user'        => $user,
    'request_uri' => $base_root . request_uri(),
    'referer'     => referer_uri(),
    'ip'          => ip_address(),
    'timestamp'   => time(),
    );

  // Call the logging hooks to log/process the message
  foreach (module_implements('watchdog') as $module) {
    module_invoke($module, 'watchdog', $log_message);
  }
}

/**
 * Set a message which reflects the status of the performed operation.
 *
 * If the function is called with no arguments, this function returns all set
 * messages without clearing them.
 *
 * @param $message
 *   The message should begin with a capital letter and always ends with a
 *   period '.'.
 * @param $type
 *   The type of the message. One of the following values are possible:
 *   - 'status'
 *   - 'warning'
 *   - 'error'
 * @param $repeat
 *   If this is FALSE and the message is already set, then the message won't
 *   be repeated.
 */
function drupal_set_message($message = NULL, $type = 'status', $repeat = TRUE) {
  if ($message) {
    if (!isset($_SESSION['messages'])) {
      if (function_exists('drupal_set_session')) {
        drupal_set_session('messages', array());
      }
      else {
        $_SESSION['messages'] = array();
      }
    }

    if (!isset($_SESSION['messages'][$type])) {
      $_SESSION['messages'][$type] = array();
    }

    if ($repeat || !in_array($message, $_SESSION['messages'][$type])) {
      $_SESSION['messages'][$type][] = $message;
    }
  }

  // messages not set when DB connection fails
  return isset($_SESSION['messages']) ? $_SESSION['messages'] : NULL;
}

/**
 * Return all messages that have been set.
 *
 * @param $type
 *   (optional) Only return messages of this type.
 * @param $clear_queue
 *   (optional) Set to FALSE if you do not want to clear the messages queue
 * @return
 *   An associative array, the key is the message type, the value an array
 *   of messages. If the $type parameter is passed, you get only that type,
 *   or an empty array if there are no such messages. If $type is not passed,
 *   all message types are returned, or an empty array if none exist.
 */
function drupal_get_messages($type = NULL, $clear_queue = TRUE) {
  if ($messages = drupal_set_message()) {
    if ($type) {
      if ($clear_queue) {
        unset($_SESSION['messages'][$type]);
      }
      if (isset($messages[$type])) {
        return array($type => $messages[$type]);
      }
    }
    else {
      if ($clear_queue) {
        unset($_SESSION['messages']);
      }
      return $messages;
    }
  }
  return array();
}

/**
 * Perform an access check for a given mask and rule type. Rules are usually
 * created via admin/user/rules page.
 *
 * If any allow rule matches, access is allowed. Otherwise, if any deny rule
 * matches, access is denied.  If no rule matches, access is allowed.
 *
 * @param $type string
 *   Type of access to check: Allowed values are:
 *     - 'host': host name or IP address
 *     - 'mail': e-mail address
 *     - 'user': username
 * @param $mask string
 *   String or mask to test: '_' matches any character, '%' matches any
 *   number of characters.
 * @return bool
 *   TRUE if access is denied, FALSE if access is allowed.
 */
function drupal_is_denied($type, $mask) {
  if ($type == 'host') {
    // Because this function is called with $type == 'host' on every page
    // request, we first check for an array of IP addresses in settings.php
    // before querying the database. In the former case there is no wildcard
    // support.
    $blocked_ips = variable_get('blocked_ips', NULL);
    if (isset($blocked_ips) && is_array($blocked_ips)) {
      return in_array($mask, $blocked_ips);
    }
  }
  // We deny access if the only matching records in the {access} table have
  // status 0 (deny). If any have status 1 (allow), or if there are no
  // matching records, we allow access.
  $sql = "SELECT 1 FROM {access} WHERE type = '%s' AND '%s' LIKE mask AND status = %d";
  return db_result(db_query_range($sql, $type, $mask, 0, 0, 1)) && !db_result(db_query_range($sql, $type, $mask, 1, 0, 1));
}

/**
 * Generates a default anonymous $user object.
 *
 * @return Object - the user object.
 */
function drupal_anonymous_user($session = '') {
  $user = new stdClass();
  $user->uid = 0;
  $user->hostname = ip_address();
  $user->roles = array();
  $user->roles[DRUPAL_ANONYMOUS_RID] = 'anonymous user';
  $user->session = $session;
  $user->cache = 0;
  return $user;
}

/**
 * A string describing a phase of Drupal to load. Each phase adds to the
 * previous one, so invoking a later phase automatically runs the earlier
 * phases too. The most important usage is that if you want to access the
 * Drupal database from a script without loading anything else, you can
 * include bootstrap.inc, and call drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE).
 *
 * @param $phase
 *   A constant. Allowed values are:
 *     DRUPAL_BOOTSTRAP_CONFIGURATION: initialize configuration.
 *     DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE: try to call a non-database cache fetch routine.
 *     DRUPAL_BOOTSTRAP_DATABASE: initialize database layer.
 *     DRUPAL_BOOTSTRAP_ACCESS: identify and reject banned hosts.
 *     DRUPAL_BOOTSTRAP_SESSION: initialize session handling.
 *     DRUPAL_BOOTSTRAP_LATE_PAGE_CACHE: load bootstrap.inc and module.inc, start
 *       the variable system and try to serve a page from the cache.
 *     DRUPAL_BOOTSTRAP_LANGUAGE: identify the language used on the page.
 *     DRUPAL_BOOTSTRAP_PATH: set $_GET['q'] to Drupal path of request.
 *     DRUPAL_BOOTSTRAP_FULL: Drupal is fully loaded, validate and fix input data.
 */
function drupal_bootstrap($phase = NULL) {
  static $phases = array(DRUPAL_BOOTSTRAP_CONFIGURATION, DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE, DRUPAL_BOOTSTRAP_DATABASE, DRUPAL_BOOTSTRAP_ACCESS, DRUPAL_BOOTSTRAP_SESSION, DRUPAL_BOOTSTRAP_LATE_PAGE_CACHE, DRUPAL_BOOTSTRAP_LANGUAGE, DRUPAL_BOOTSTRAP_PATH, DRUPAL_BOOTSTRAP_FULL), $phase_index = 0;

  if (isset($phase)) {
    while ($phase >= $phase_index && isset($phases[$phase_index])) {
      $current_phase = $phases[$phase_index];
      unset($phases[$phase_index++]);
      _drupal_bootstrap($current_phase);
    }
  }
  
  return $phase_index;
}

/**
 * Redirect to the installer if an essential table is missing.
 */
function detect_installation_or_run_installer() {
  if (variable_get('pressflow_smart_start', FALSE) && !db_table_exists('access')) {
    include_once 'includes/install.inc';
    install_goto('install.php');
  }
}

function _drupal_bootstrap($phase) {
  global $conf, $user, $db_prefix;

  switch ($phase) {

    case DRUPAL_BOOTSTRAP_CONFIGURATION:
      drupal_unset_globals();
      // Start a page timer:
      timer_start('page');
      // Initialize the configuration
      conf_init();
      break;

    case DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE:
      // Allow specifying special cache handlers in settings.php, like
      // using memcached or files for storing cache information.
      require_once variable_get('cache_inc', './includes/cache.inc');
      // If the page_cache_fastpath is set to TRUE in settings.php and
      // page_cache_fastpath (implemented in the special implementation of
      // cache.inc) printed the page and indicated this with a returned TRUE
      // then we are done.
      if (variable_get('page_cache_fastpath', FALSE) && page_cache_fastpath()) {
        exit;
      }
      break;

    case DRUPAL_BOOTSTRAP_DATABASE:
      // The user agent header is used to pass a database prefix in the request when
      // running tests. However, for security reasons, it is imperative that we
      // validate we ourselves made the request.
      $GLOBALS['simpletest_installed'] = TRUE;
      if (isset($_SERVER['HTTP_USER_AGENT']) && preg_match("/^(simpletest\d+);/", $_SERVER['HTTP_USER_AGENT'], $matches)) {
        if (!drupal_valid_test_ua($_SERVER['HTTP_USER_AGENT'])) {
          header($_SERVER['SERVER_PROTOCOL'] . ' 403 Forbidden');
          exit;
        }
        $db_prefix .= $matches[1];
      }

      // Initialize the default database.
      require_once './includes/database.inc';
      db_set_active();
      // Allow specifying alternate lock implementations in settings.php, like
      // those using APC or memcached.
      require_once variable_get('lock_inc', './includes/lock.inc');
      lock_init();
      
      // Detect if an installation is present.
      detect_installation_or_run_installer();

      break;

    case DRUPAL_BOOTSTRAP_ACCESS:
      // Deny access to hosts which were banned - t() is not yet available.
      if (drupal_is_denied('host', ip_address())) {
        header($_SERVER['SERVER_PROTOCOL'] .' 403 Forbidden');
        print 'Sorry, '. check_plain(ip_address()) .' has been banned.';
        exit();
      }
      break;

    case DRUPAL_BOOTSTRAP_SESSION:
      require_once variable_get('session_inc', './includes/session.inc');
      drupal_session_initialize();
      break;

    case DRUPAL_BOOTSTRAP_LATE_PAGE_CACHE:
      // Initialize configuration variables, using values from settings.php if available.
      $conf = variable_init(isset($conf) ? $conf : array());

      $cache_mode = variable_get('cache', CACHE_DISABLED);
      // Get the page from the cache, unless the cache is disabled or external.
      if ($cache_mode != CACHE_DISABLED && $cache_mode != CACHE_EXTERNAL) {
        $cache = page_get_cache();
      }
      else {
        $cache = FALSE;
      }

      // If the skipping of the bootstrap hooks is not enforced, call hook_init.
      if (!is_object($cache) || $cache_mode != CACHE_AGGRESSIVE) {
        // Load module handling.
        require_once './includes/module.inc';
        bootstrap_invoke_all('boot');
      }

      // If there is a cached page, display it.
      if (is_object($cache)) {
        header('X-Drupal-Cache: HIT');
        drupal_page_cache_header($cache);
        // If the skipping of the bootstrap hooks is not enforced, call hook_exit.
        if ($cache_mode != CACHE_AGGRESSIVE) {
          bootstrap_invoke_all('exit');
        }
        // We are done.
        exit;
      }
  
      if (!$cache && drupal_page_is_cacheable() && $cache_mode != CACHE_EXTERNAL) {
        header('X-Drupal-Cache: MISS');
      }
      
      // If using an external cache and the page is cacheable, set headers.
      if ($cache_mode == CACHE_EXTERNAL && drupal_page_is_cacheable()) {
        drupal_page_cache_header_external();
      }

      // Prepare for non-cached page workflow.
      if (!drupal_is_cli()) {
        ob_start();
        drupal_page_header();
      }
      break;

    case DRUPAL_BOOTSTRAP_LANGUAGE:
      drupal_init_language();
      break;

    case DRUPAL_BOOTSTRAP_PATH:
      require_once './includes/path.inc';
      // Initialize $_GET['q'] prior to loading modules and invoking hook_init().
      drupal_init_path();
      break;

    case DRUPAL_BOOTSTRAP_FULL:
      require_once './includes/common.inc';
      _drupal_bootstrap_full();
      break;
  }
}

/**
 * Enables use of the theme system without requiring database access.
 *
 * Loads and initializes the theme system for site installs, updates and when
 * the site is in off-line mode. This also applies when the database fails.
 *
 * @see _drupal_maintenance_theme()
 */
function drupal_maintenance_theme() {
  require_once './includes/theme.maintenance.inc';
  _drupal_maintenance_theme();
}

/**
 * Return the name of the localisation function. Use in code that needs to
 * run both during installation and normal operation.
 */
function get_t() {
  static $t;
  if (is_null($t)) {
    $t = function_exists('install_main') ? 'st' : 't';
  }
  return $t;
}

/**
 *  Choose a language for the current page, based on site and user preferences.
 */
function drupal_init_language() {
  global $language, $user;

  // Ensure the language is correctly returned, even without multilanguage support.
  // Useful for eg. XML/HTML 'lang' attributes.
  if (variable_get('language_count', 1) == 1) {
    $language = language_default();
  }
  else {
    include_once './includes/language.inc';
    $language = language_initialize();
  }
}

/**
 * Get a list of languages set up indexed by the specified key
 *
 * @param $field The field to index the list with.
 * @param $reset Boolean to request a reset of the list.
 */
function language_list($field = 'language', $reset = FALSE) {
  static $languages = NULL;

  // Reset language list
  if ($reset) {
    $languages = NULL;
  }

  // Init language list
  if (!isset($languages)) {
    if (variable_get('language_count', 1) > 1 || module_exists('locale')) {
      $result = db_query('SELECT * FROM {languages} ORDER BY weight ASC, name ASC');
      while ($row = db_fetch_object($result)) {
        $languages['language'][$row->language] = $row;
      }
    }
    else {
      // No locale module, so use the default language only.
      $default = language_default();
      $languages['language'][$default->language] = $default;
    }
  }

  // Return the array indexed by the right field
  if (!isset($languages[$field])) {
    $languages[$field] = array();
    foreach ($languages['language'] as $lang) {
      // Some values should be collected into an array
      if (in_array($field, array('enabled', 'weight'))) {
        $languages[$field][$lang->$field][$lang->language] = $lang;
      }
      else {
        $languages[$field][$lang->$field] = $lang;
      }
    }
  }
  return $languages[$field];
}

/**
 * Default language used on the site
 *
 * @param $property
 *   Optional property of the language object to return
 */
function language_default($property = NULL) {
  $language = variable_get('language_default', (object) array('language' => 'en', 'name' => 'English', 'native' => 'English', 'direction' => 0, 'enabled' => 1, 'plurals' => 0, 'formula' => '', 'domain' => '', 'prefix' => '', 'weight' => 0, 'javascript' => ''));
  return $property ? $language->$property : $language;
}

/**
 * If Drupal is behind a reverse proxy, we use the X-Forwarded-For header
 * instead of $_SERVER['REMOTE_ADDR'], which would be the IP address
 * of the proxy server, and not the client's.
 *
 * @return
 *   IP address of client machine, adjusted for reverse proxy.
 */
function ip_address() {
  static $ip_address = NULL;

  if (!isset($ip_address)) {
    $ip_address = $_SERVER['REMOTE_ADDR'];
    
    // Only use parts of the X-Forwarded-For (XFF) header that have followed a trusted route.
    // Specifically, identify the leftmost IP address in the XFF header that is not one of ours.
    // An XFF header is: X-Forwarded-For: client1, proxy1, proxy2
    if (isset($_SERVER['HTTP_' . variable_get('x_forwarded_for_header', 'X_FORWARDED_FOR')]) && variable_get('reverse_proxy', 0)) {
      // Load trusted reverse proxy server IPs.
      $reverse_proxy_addresses = variable_get('reverse_proxy_addresses', array());
      
      // Turn XFF header into an array.
      $forwarded = explode(',', $_SERVER['HTTP_' . variable_get('x_forwarded_for_header', 'X_FORWARDED_FOR')]);
      
      // Trim the forwarded IPs; they may have been delimited by commas and spaces.
      $forwarded = array_map('trim', $forwarded);

      // Tack direct client IP onto end of forwarded array.
      $forwarded[] = $ip_address;

      // Eliminate all trusted IPs.
      $untrusted = array_diff($forwarded, $reverse_proxy_addresses);
      
      // The right-most IP is the most specific we can trust.
      $ip_address = array_pop($untrusted);
    }
  }

  return $ip_address;
}

/**
 * Initialize the session handler, starting a session if needed.
 */
function drupal_session_initialize() {
  global $user;
   
  session_set_save_handler('sess_open', 'sess_close', 'sess_read', 'sess_write', 'sess_destroy_sid', 'sess_gc');
 
  if (isset($_COOKIE[session_name()])) {
    // If a session cookie exists, initialize the session. Otherwise the
    // session is only started on demand in drupal_session_commit(), making
    // anonymous users not use a session cookie unless something is stored in
    // $_SESSION. This allows HTTP proxies to cache anonymous pageviews.
    drupal_session_start();
    if (!empty($user->uid) || !empty($_SESSION)) {
      drupal_page_is_cacheable(FALSE);
    }
  }
  else {
    // Set a session identifier for this request. This is necessary because
    // we lazyly start sessions at the end of this request, and some
    // processes (like drupal_get_token()) needs to know the future
    // session ID in advance.
    $user = drupal_anonymous_user();
    session_id(md5(uniqid('', TRUE)));
  }
}

/**
 * Forcefully start a session, preserving already set session data.
 */
function drupal_session_start() {
  if (!drupal_session_started()) {
    // Save current session data before starting it, as PHP will destroy it.
    $session_data = isset($_SESSION) ? $_SESSION : NULL;
    session_start();
    drupal_session_started(TRUE);
    // Restore session data.
    if (!empty($session_data)) {
      $_SESSION += $session_data;
    }
  }
}

/**
 * Commit the current session, if necessary.
 *
 * If an anonymous user already has an empty session, destroy it.
 */
function drupal_session_commit() {
  global $user;

  if (empty($user->uid) && empty($_SESSION)) {
    if (drupal_session_started() && drupal_save_session()) {
      // Destroy empty anonymous sessions.
      drupal_session_destroy();
    }
  }
  else if (drupal_save_session()) {
    if (!drupal_session_started()) {
      drupal_session_start();
    }
    // Write the session data.
    session_write_close();
  }
}

/**
 * Return whether a session has been started.
 */  
function drupal_session_started($set = NULL) {
  static $session_started = FALSE;
  if (isset($set)) {
    $session_started = $set;
  }
  return $session_started && session_id();
}

/**
 * Called when an anonymous user becomes authenticated or vice-versa.
 */
function drupal_session_regenerate() {
  global $user;

  // Set the session cookie "httponly" flag to reduce the risk of session
  // stealing via XSS.
  extract(session_get_cookie_params());

  if (version_compare(PHP_VERSION, '5.2.0') === 1) {
    session_set_cookie_params($lifetime, $path, $domain, $secure, TRUE);
  }
  else {
    session_set_cookie_params($lifetime, $path, $domain, $secure);
  }

  if (drupal_session_started()) {
    $old_session_id = session_id();
    session_regenerate_id();
  }
  else {
    // Start the session when it doesn't exist yet.
    // Preserve the logged in user, as it will be reset to anonymous
    // by _sess_read.
    $account = $user;
    drupal_session_start();
    $user = $account;
  }

  if (isset($old_session_id)) {
    db_query("UPDATE {sessions} SET sid = '%s' WHERE sid = '%s'", session_id(), $old_session_id);
  }
}

/**
 * Determine whether to save session data of the current request.
 *
 * This function allows the caller to temporarily disable writing of
 * session data, should the request end while performing potentially
 * dangerous operations, such as manipulating the global $user object.
 * See http://drupal.org/node/218104 for usage.
 *
 * @param $status
 *   Disables writing of session data when FALSE, (re-)enables
 *   writing when TRUE.
 * @return
 *   FALSE if writing session data has been disabled. Otherwise, TRUE.
 */
function drupal_save_session($status = NULL) {
  static $save_session = TRUE;
  if (isset($status)) {
    $save_session = $status;
  }
  return $save_session;
}
	
/**
 * Returns the current bootstrap phase for this Drupal process.
 *
 * The current phase is the one most recently completed by drupal_bootstrap().
 *
 * @see drupal_bootstrap()
 */
function drupal_get_bootstrap_phase() {
  return drupal_bootstrap();
}

/**
 * Validate the HMAC and timestamp of a user agent header from simpletest.
 */
function drupal_valid_test_ua($user_agent) {
//  global $dbatabases;
  global $db_url;

  list($prefix, $time, $salt, $hmac) = explode(';', $user_agent);
  $check_string =  $prefix . ';' . $time . ';' . $salt;
  // We use the database credentials from settings.php to make the HMAC key, since
  // the database is not yet initialized and we can't access any Drupal variables.
  // The file properties add more entropy not easily accessible to others.
//  $filepath = DRUPAL_ROOT . '/includes/bootstrap.inc';
  $filepath = './includes/bootstrap.inc';
//  $key = sha1(serialize($databases) . filectime($filepath) . fileinode($filepath), TRUE);
  $key = sha1(serialize($db_url) . filectime($filepath) . fileinode($filepath), TRUE);
  // The HMAC must match.
  return $hmac == base64_encode(hash_hmac('sha1', $check_string, $key, TRUE));
}

/**
 * Generate a user agent string with a HMAC and timestamp for simpletest.
 */
function drupal_generate_test_ua($prefix) {
//  global $dbatabases;
  global $db_url;
  static $key;

  if (!isset($key)) {
    // We use the database credentials to make the HMAC key, since we
    // check the HMAC before the database is initialized. filectime()
    // and fileinode() are not easily determined from remote.
//    $filepath = DRUPAL_ROOT . '/includes/bootstrap.inc';
    $filepath = './includes/bootstrap.inc';                
//    $key = sha1(serialize($databases) . filectime($filepath) . fileinode($filepath), TRUE);
    $key = sha1(serialize($db_url) . filectime($filepath) . fileinode($filepath), TRUE);
  }
   // Generate a moderately secure HMAC based on the database credentials.
   $salt = uniqid('', TRUE);
   $check_string = $prefix . ';' . time() . ';' . $salt;
   return  $check_string . ';' . base64_encode(hash_hmac('sha1', $check_string, $key, TRUE));
}

/**
 * Detect whether the current script is running in a command-line environment.
 */
function drupal_is_cli() {
  return ((!isset($_SERVER['SERVER_SOFTWARE']) || $_SERVER['SERVER_SOFTWARE'] == 'PHP CLI')
    && (php_sapi_name() == 'cli' || (is_numeric($_SERVER['argc']) && $_SERVER['argc'] > 0)));
}

/**
 * Destroys all data registered to a session.
 *
 * Placed in bootstrap.inc instead of session.inc as session.inc may be
 * overridden by other session persistance layers (eg., memcache).
 *
 * @ingroup php_wrappers
 */
function drupal_session_destroy() {
  session_destroy();
  
  // Workaround PHP 5.2 fatal error "Failed to initialize storage module".
  // @see http://bugs.php.net/bug.php?id=32330
  session_set_save_handler('sess_open', 'sess_close', 'sess_read', 'sess_write', 'sess_destroy_sid', 'sess_gc');
}
