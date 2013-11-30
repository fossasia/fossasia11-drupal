<?php

/**
 * @file
 * User session handling functions.
 */

function sess_open($save_path, $session_name) {
  return TRUE;
}

function sess_close() {
  return TRUE;
}

/**
 * Reads an entire session from the database (internal use only).
 *
 * Also initializes the $user object for the user associated with the session.
 * This function is registered with session_set_save_handler() to support
 * database-backed sessions. It is called on every page load when PHP sets
 * up the $_SESSION superglobal.
 *
 * This function is an internal function and must not be called directly.
 * Doing so may result in logging out the current user, corrupting session data
 * or other unexpected behavior. Session data must always be accessed via the
 * $_SESSION superglobal.
 *
 * @param $key
 *   The session ID of the session to retrieve.
 *
 * @return
 *   The user's session, or an empty string if no session exists.
 */
function sess_read($key) {
  global $user;

  // Write and Close handlers are called after destructing objects
  // since PHP 5.0.5.
  // Thus destructors can use sessions but session handler can't use objects.
  // So we are moving session closure before destructing objects.
  register_shutdown_function('session_write_close');

  // Handle the case of first time visitors and clients that don't store cookies (eg. web crawlers).
  if (!isset($_COOKIE[session_name()])) {
    $user = drupal_anonymous_user();
    return '';
  }

  // Otherwise, if the session is still active, we have a record of the client's session in the database.
  $user = db_fetch_object(db_query("SELECT u.*, s.* FROM {users} u INNER JOIN {sessions} s ON u.uid = s.uid WHERE s.sid = '%s'", $key));

  // We found the client's session record and they are an authenticated,
  // active user.
  if ($user && $user->uid > 0 && $user->status == 1) {
    // This is done to unserialize the data member of $user
    $user = drupal_unpack($user);

    // Add roles element to $user
    $user->roles = array();
    $user->roles[DRUPAL_AUTHENTICATED_RID] = 'authenticated user';
    $result = db_query("SELECT r.rid, r.name FROM {role} r INNER JOIN {users_roles} ur ON ur.rid = r.rid WHERE ur.uid = %d", $user->uid);
    while ($role = db_fetch_object($result)) {
      $user->roles[$role->rid] = $role->name;
    }
  }
  // We didn't find the client's record (session has expired), or they are
  // blocked, or they are an anonymous user.
  else {
    $session = isset($user->session) ? $user->session : '';
    $user = drupal_anonymous_user($session);
  }

  return $user->session;
}

/**
 * Writes an entire session to the database (internal use only).
 *
 * This function is registered with session_set_save_handler() to support
 * database-backed sessions.
 *
 * This function is an internal function and must not be called directly.
 * Doing so may result in corrupted session data or other unexpected behavior.
 * Session data must always be accessed via the $_SESSION superglobal.
 *
 * @param $key
 *   The session ID of the session to write to.
 * @param $value
 *   Session data to write as a serialized string.
 *
 * @return
 *   Always returns TRUE.
 */
function sess_write($key, $value) {
  global $user;

  // If saving of session data is disabled, or if a new empty anonymous session
  // has been started, do nothing. This keeps anonymous users, including
  // crawlers, out of the session table, unless they actually have something
  // stored in $_SESSION.
  if (!session_save_session() || ($user->uid == 0 && empty($_COOKIE[session_name()]) && empty($value))) {
    return TRUE;
  }

  db_query("UPDATE {sessions} SET uid = %d, cache = %d, hostname = '%s', session = '%s', timestamp = %d WHERE sid = '%s'", $user->uid, isset($user->cache) ? $user->cache : '', ip_address(), $value, time(), $key);
  if (db_affected_rows()) {
    // Last access time is updated no more frequently than once every 180 seconds.
    // This reduces contention in the users table.
    if ($user->uid && time() - $user->access > variable_get('session_write_interval', 180)) {
      db_query("UPDATE {users} SET access = %d WHERE uid = %d", time(), $user->uid);
    }
  }
  else {
    // If this query fails, another parallel request probably got here first.
    // In that case, any session data generated in this request is discarded.
    @db_query("INSERT INTO {sessions} (sid, uid, cache, hostname, session, timestamp) VALUES ('%s', %d, %d, '%s', '%s', %d)", $key, $user->uid, isset($user->cache) ? $user->cache : '', ip_address(), $value, time());
  }

  return TRUE;
}

/**
 * Called when an anonymous user becomes authenticated or vice-versa.
 *
 * This backport obsoletes this function, but a call to the new function
 * is kept here to maximize API consistency.
 */
function sess_regenerate() {
  drupal_session_regenerate();
}

/**
 * Counts how many users have sessions. Can count either anonymous sessions or authenticated sessions.
 *
 * @param int $timestamp
 *   A Unix timestamp representing a point of time in the past.
 *   The default is 0, which counts all existing sessions.
 * @param boolean $anonymous
 *   TRUE counts only anonymous users.
 *   FALSE counts only authenticated users.
 * @return  int
 *   The number of users with sessions.
 */
function sess_count($timestamp = 0, $anonymous = true) {
  $query = $anonymous ? ' AND uid = 0' : ' AND uid > 0';
  return db_result(db_query('SELECT COUNT(sid) AS count FROM {sessions} WHERE timestamp >= %d'. $query, $timestamp));
}

/**
 * Called by PHP session handling with the PHP session ID to end a user's session.
 *
 * @param  string $sid
 *   the session id
 */
function sess_destroy_sid($sid) {
  // Delete session data.
  db_query("DELETE FROM {sessions} WHERE sid = '%s'", $sid);

  // If the session ID being destroyed is the one of the current user,
  // clean-up his/her session data and cookie.
  if ($sid == session_id()) {
    global $user;

    // Reset $_SESSION and $user to prevent a new session from being started
    // in drupal_session_commit()
    $_SESSION = array();
    $user = drupal_anonymous_user();

    // Unset the session cookie.
    if (isset($_COOKIE[session_name()])) {
      $params = session_get_cookie_params();

      if (version_compare(PHP_VERSION, '5.2.0') === 1) {
        setcookie(session_name(), '', $_SERVER['REQUEST_TIME'] - 3600, $params['path'], $params['domain'], $params['secure'], $params['httponly']);
      }
      else {
        setcookie(session_name(), '', $_SERVER['REQUEST_TIME'] - 3600, $params['path'], $params['domain'], $params['secure']);      
      }
      unset($_COOKIE[session_name()]);
    }
  }
}

/**
 * End a specific user's session
 *
 * @param  string $uid
 *   the user id
 */
function sess_destroy_uid($uid) {
  db_query('DELETE FROM {sessions} WHERE uid = %d', $uid);
}

function sess_gc($lifetime) {
  // Be sure to adjust 'php_value session.gc_maxlifetime' to a large enough
  // value. For example, if you want user sessions to stay in your database
  // for three weeks before deleting them, you need to set gc_maxlifetime
  // to '1814400'. At that value, only after a user doesn't log in after
  // three weeks (1814400 seconds) will his/her session be removed.
  db_query("DELETE FROM {sessions} WHERE timestamp < %d", time() - $lifetime);

  return TRUE;
}

/**
 * Renamed to drupal_save_session(). Stub kept here for API compatibility.
 */
function session_save_session($status = NULL) {
  return drupal_save_session($status);
}
