<?php
/* $Id: mimemail.inc,v 1.53 2010/09/12 15:57:20 sgabe Exp $ */

/**
 * @file
 * Common mail functions for sending e-mail.  Originally written by Gerhard.
 *
 *   Allie Micka < allie at pajunas dot com >
 */

/**
 * Attempts to RFC822-compliant headers for the mail message or its MIME parts
 * TODO could use some enhancement and stress testing
 *
 * @param $headers An array of headers
 * @return header string
 */
function mimemail_rfc_headers($headers) {
  $header = '';
  $crlf = variable_get('mimemail_crlf', "\n");
  foreach ($headers as $key => $value) {
    $key = trim($key);
    // collapse spaces and get rid of newline characters
    $value = preg_replace('/(\s+|\n|\r|^\s|\s$)/', ' ', $value);

    //fold headers if they're too long
    if (strlen($value) > 60) {
      //if there's a semicolon, use that to separate
      if (count($array = preg_split('/;\s*/', $value)) > 1) {
        $value = trim(join(";$crlf    ", $array));
      }
      else {
        $value = wordwrap($value, 50, "$crlf    ", FALSE);
      }
    }
    $header .= "$key: $value$crlf";
  }
  return trim($header);
}

/**
 * Gives useful defaults for standard email headers.
 *
 * @param $headers An array of headers
 * @return header string.
 */
function mimemail_headers($headers, $from = '') {
  // Note: This may not work. The MTA may rewrite the Return-Path, and Errors-To is deprecated.
  if (!$from) {
    $from = variable_get('site_mail', ini_get('sendmail_from'));
  }
  preg_match('/[a-z0-9\-\.]+@{1}[a-z0-9\-\.]+/i', $from, $matches);
  $from_email = $matches[0];
  // allow a mail to overwrite standard headers.
  $headers = array_merge(array('Return-Path' => "<$from_email>", 'Errors-To' => $from, 'From' => $from, 'Content-Type' => 'text/plain; charset=utf-8; format=flowed'), $headers);

  // Run all headers through mime_header_encode() to convert non-ascii
  // characters to an rfc compliant string, similar to drupal_mail().
  foreach($headers as $key => $value) {
    $headers[$key] = mime_header_encode($value);
  }

  return $headers;
}

/**
 * Extracts links to local images from html documents.
 *
 * @param $html html text
 * @param $name document name
 *
 * @return an array of arrays
 *            array(array(
 *                     'name' => document name
 *                     'content' => html text, local image urls replaced by Content-IDs,
 *                     'Content-Type' => 'text/html; charset=utf-8')
 *                  array(
 *                     'name' => file name,
 *                     'file' => reference to local file,
 *                     'Content-ID' => generated Content-ID,
 *                     'Content-Type' => derived using mime_content_type
 *                                       if available, educated guess otherwise
 *                     )
 *                  )
 */
function mimemail_extract_files($html) {
  $pattern = '/(<link[^>]+href=[\'"]?|<object[^>]+codebase=[\'"]?|@import |[\s]src=[\'"]?)([^\'>"]+)([\'"]?)/mis';
  $html = preg_replace_callback($pattern, '_mimemail_replace_files', $html);

  $document = array(array(
    'Content-Type' => "text/html; charset=utf-8",
    'Content-Transfer-Encoding' => '8bit',
    'content' => $html,
  ));

  $files = _mimemail_file();

  return array_merge($document, $files);
}

/**
 * Callback for preg_replace_callback()
 */
function _mimemail_replace_files($matches) {
  return stripslashes($matches[1]) . _mimemail_file($matches[2]) . stripslashes($matches[3]);
}

/**
 * Helper function to extract local files
 *
 * @param $url a URL to a file
 *
 * @return an absolute :
 */
function _mimemail_file($url = NULL, $name = '', $type = '', $disposition = 'related') {
  static $files = array();
  static $filenames = array();

  if ($url) {
    $url = _mimemail_url($url, 'TRUE');

    // If the $url is absolute, we're done here.
    if (strpos($url, '://') !== FALSE || preg_match('!mailto:!', $url)) {
      return $url;
    }
    // The $url is a relative file path, continue processing.
    else {
      // Download method is private, and the $url needs conversion.
      if (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PRIVATE && strpos($url, 'system/files/') !== FALSE) {
        $file = file_create_path(drupal_substr($url, (strpos($url, 'system/files/') + drupal_strlen('system/files/'))));
      }
      // Download method is public.
      else {
        $file = $url;
      }
    }
  }

  if (isset($file) && @file_exists($file)) {
    // Prevent duplicate items.
    if (isset($filenames[$file])) return 'cid:'. $filenames[$file];

    $content_id = md5($file) .'@'. $_SERVER['HTTP_HOST'];

    if (!$name) $name = drupal_substr($file, strrpos($file, '/') + 1);

    $new_file = array(
      'name' => $name,
      'file' => $file,
      'Content-ID' => $content_id,
      'Content-Disposition' => $disposition,
    );

    $new_file['Content-Type'] = (!empty($name)) ? file_get_mimetype($name) : file_get_mimetype($file);

    $files[] = $new_file;
    $filenames[$file] = $content_id;

    return 'cid:'. $content_id;
  }
  // The $file is not exist, return the $url if possible.
  elseif ($url) {
    return $url;
  }

  $ret = $files;
  $files = array();
  $filenames = array();
  return $ret;
}

/**
 *
 * @param $parts
 *        an array of parts to be included
 *        each part is itself an array:
 *        array(
 *          'name' => $name the name of the attachement
 *          'content' => $content textual content
 *          'file' => $file a file
 *          'Content-Type' => Content type of either file or content.
 *                            Mandatory for content, optional for file.
 *                            If not present, it will be derived from
 *                            file the file if mime_content_type is available.
 *                            If not, application/octet-stream is used.
 *          'Content-Disposition' => optional, inline is assumed
 *          'Content-Transfer-Encoding' => optional,
 *                                         base64 is assumed for files
 *                                         8bit for other content.
 *          'Content-ID' => optional, for in-mail references to attachements.
 *        )
 *        name is mandatory, one of content and file is required,
 *        they are mutually exclusive.
 *
 * @param $content_type
 *        Content-Type for the combined message, optional, default: multipart/mixed
 *
 * @return
 *     an array containing the elements 'header' and 'body'.
 *     'body' is the mime encoded multipart body of a mail.
 *     'headers' is an array that includes some headers for the mail to be sent.
 */
function mimemail_multipart_body($parts, $content_type = 'multipart/mixed; charset=utf-8', $sub_part = FALSE) {
  $boundary = md5(uniqid(time()));
  $body = '';
  $headers = array(
    'Content-Type' => "$content_type; boundary=\"$boundary\"",
  );
  if (!$sub_part) {
    $headers['MIME-Version'] = '1.0';
    $body = "This is a multi-part message in MIME format.\n";
  }

  foreach ($parts as $part) {
    $part_headers = array();

    if (isset($part['Content-ID'])) {
      $part_headers['Content-ID'] = '<'. $part['Content-ID'] .'>';
    }

    if (isset($part['Content-Type'])) {
      $part_headers['Content-Type'] = $part['Content-Type'];
    }

    if (isset($part['Content-Disposition'])) {
      $part_headers['Content-Disposition'] = $part['Content-Disposition'];
    }
    else {
      $part_headers['Content-Disposition'] = 'inline';
    }

    if (isset($part['Content-Transfer-Encoding'])) {
      $part_headers['Content-Transfer-Encoding'] = $part['Content-Transfer-Encoding'];
    }

    // mail content provided as a string
    if (isset($part['content']) && $part['content']) {
      if (!isset($part['Content-Transfer-Encoding'])) {
        $part_headers['Content-Transfer-Encoding'] = '8bit';
      }
      $part_body = $part['content'];
      if (isset($part['name'])) {
        $part_headers['Content-Type'] .= '; name="'. $part['name'] .'"';
        $part_headers['Content-Disposition'] .= '; filename="'. $part['name'] .'"';
      }

    // mail content references in a filename
    }
    else {
      if (!isset($part['Content-Transfer-Encoding'])) {
        $part_headers['Content-Transfer-Encoding'] = 'base64';
      }

      if (!isset($part['Content-Type'])) {
        $part['Content-Type'] = file_get_mimetype($part['file']);
      }

      if (isset($part['name'])) {
        $part_headers['Content-Type'] .= '; name="'. $part['name'] .'"';
        $part_headers['Content-Disposition'] .= '; filename="'. $part['name'] .'"';
      }

      if (isset($part['file'])) {
        $part_body = chunk_split(base64_encode(file_get_contents($part['file'])));
      }
    }

    $body .= "\n--$boundary\n";
    $body .= mimemail_rfc_headers($part_headers) ."\n\n";
    $body .= $part_body;
  }
  $body .= "\n--$boundary--\n";

  return array('headers' => $headers, 'body' => $body);
}

/**
 * Callback for preg_replace_callback()
 */
function _mimemail_expand_links($matches) {
  return $matches[1] . _mimemail_url($matches[2]);
}

/**
 * Generate a multipart message body with a text alternative for some html text
 * @param $body An HTML message body
 * @param $subject The message subject
 * @param $plaintext Whether the recipient prefers plaintext-only messages (default false)
 *
 * @return
 *     an array containing the elements 'header' and 'body'.
 *     'body' is the mime encoded multipart body of a mail.
 *     'headers' is an array that includes some headers for the mail to be sent.
 *
 * The first mime part is a multipart/alternative containing mime-encoded
 * sub-parts for HTML and plaintext.  Each subsequent part is the required
 * image/attachment
 */
function mimemail_html_body($body, $subject, $plaintext = FALSE, $text = NULL, $attachments = array()) {
  if (is_null($text)) {
    // todo: remove this preg_replace once filter_xss() is properly handling
    // direct descendant css selectors '>' in inline CSS. For now this cleans
    // up our plain text part. See mimemail #364198, drupal #370903
    $text = preg_replace('|<style.*</style>|mis', '', $body);
    $text = drupal_html_to_text($text);
  }
  if ($plaintext) {
    //Plain mail without attachment
    if (empty($attachments)) {
      $content_type = 'text/plain';
      return array(
        'body' => $text,
        'headers' => array('Content-Type' => 'text/plain; charset=utf-8'),
      );
    }
    //Plain mail has attachement
    else {
      $content_type = 'multipart/mixed';
      $parts = array(array(
      'content' => $text,
        'Content-Type' => 'text/plain; charset=utf-8',
      ));
    }
  }
  else {
    $content_type = 'multipart/related; type="multipart/alternative"';

    $text_part = array('Content-Type' => 'text/plain; charset=utf-8', 'content' => $text);

    //expand all local links
    $pattern = '/(<a[^>]+href=")([^"]*)/mi';
    $body = preg_replace_callback($pattern, '_mimemail_expand_links', $body);

    $mime_parts = mimemail_extract_files($body);

    $content = array($text_part, array_shift($mime_parts));
    $content = mimemail_multipart_body($content, 'multipart/alternative', TRUE);
    $parts = array(array('Content-Type' => $content['headers']['Content-Type'], 'content' => $content['body']));

    if ($mime_parts) {
      $parts = array_merge($parts, $mime_parts);
    }
  }

  foreach ($attachments as $a) {
    $a = (object) $a;
    // Check the list parameter if its set or ignore it (Upload module support).
    if(!isset($a->list) || $a->list) {
      _mimemail_file($a->filepath, $a->filename, $a->filemime, 'attachment');
      $parts = array_merge($parts, _mimemail_file());
    }
  }

  return mimemail_multipart_body($parts, $content_type);
}

function mimemail_parse($message) {

  // Provides a "headers", "content-type" and "body" element.
  $mail = mimemail_parse_headers($message);

  // Get an address-only version of "From" (useful for user_load() and such).
  $mail['from'] = preg_replace('/.*\b([a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,4})\b.*/i', '\1', strtolower($mail['headers']['From']));

  // Get a subject line, which may be cleaned up/modified later.
  $mail['subject'] = $mail['headers']['Subject'];

  // Make an array to hold any non-content attachments.
  $mail['attachments'] = array();

  // We're dealing with a multi-part message.
  $mail['parts'] = mimemail_parse_boundary($mail);

  foreach ($mail['parts'] as $i => $part_body) {

    $part = mimemail_parse_headers($part_body);

    $sub_parts = mimemail_parse_boundary($part);

    // Content is encoded in a multipart/alternative section
    if (count($sub_parts) > 1) {
      foreach ($sub_parts as $j => $sub_part_body) {
        $sub_part = mimemail_parse_headers($sub_part_body);

        if ($sub_part['content-type'] == 'text/plain') {
          $mail['text'] = mimemail_parse_content($sub_part);
        }
        if ($sub_part['content-type'] == 'text/html') {
          $mail['html'] = mimemail_parse_content($sub_part);
        }
        else {
          $mail['attachments'][] = mimemail_parse_attachment($sub_part);
        }
      }
    }

    if (($part['content-type'] == 'text/plain') && !isset($mail['text'])) {
      $mail['text'] = mimemail_parse_content($part);
    }
    elseif (($part['content-type'] == 'text/html') && !isset($mail['html'])) {
      $mail['html'] = mimemail_parse_content($part);
    }
    else {
      $mail['attachments'][] = mimemail_parse_attachment($part);
    }
  }

  // Make sure our text and html parts are accounted for
  if (isset($mail['html']) && !isset($mail['text'])) {
    $mail['text'] = preg_replace('|<style.*</style>|mis', '', $mail['html']);
    $mail['text'] = drupal_html_to_text($mail['text']);
  }
  elseif (isset($mail['text']) && !isset($mail['html'])) {
    $format = variable_get('mimemail_format', FILTER_FORMAT_DEFAULT);
    $mail['html'] = check_markup($mail['text'], $format, FALSE);
  }

  // Last ditch attempt - use the body as-is
  if (!isset($mail['text'])) {
    $mail['text'] = mimemail_parse_content($mail);
    $format = variable_get('mimemail_format', FILTER_FORMAT_DEFAULT);
    $mail['html'] = check_markup($mail['text'], $format, FALSE);
  }

  return $mail;
}

/*
 * Split a multi-part message using mime boundaries
 */
function mimemail_parse_boundary($part) {
  $m = array();
  if (preg_match('/.*boundary="?([^";]+)"?.*/', $part['headers']['Content-Type'], $m)) {
    $boundary = "\n--". $m[1];
    $body     = str_replace("$boundary--", '', $part['body']);
    return array_slice(explode($boundary, $body), 1);
  }
  return array($part['body']);
}

/*
 * Split a message (or message part) into its headers and body section
 */
function mimemail_parse_headers($message) {
  // Split out body and headers
  if (preg_match("/^(.*?)\r?\n\r?\n(.*)/s", $message, $match)) {
    list($hdr, $body) = array($match[1], $match[2]);
  }

  // Un-fold the headers.
  $hdr = preg_replace(array("/\r/", "/\n(\t| )+/"), array('', ' '), $hdr);

  $headers = array();
  foreach (explode("\n", trim($hdr)) as $row) {
    $split = strpos( $row, ':' );
    $name = trim(substr($row, 0, $split));
    $val  = trim(substr($row, $split+1));
    $headers[$name] = $val;
  }

  $type = (preg_replace('/\s*([^;]+).*/', '\1', $headers['Content-Type']));

  return array('headers' => $headers, 'body' => $body, 'content-type' => $type);
}

/*
 * Return a decoded mime part in UTF8
 */
function mimemail_parse_content($part) {
  $content = $part['body'];

  // Decode this part
  if ($encoding = strtolower($part['headers']['Content-Transfer-Encoding'])) {
    switch ($encoding) {

      case 'base64':
        $content = base64_decode($content);
        break;

      case 'quoted-printable':
        $content = quoted_printable_decode($content);
        break;

      case '7bit':  // 7bit is the RFC default
        break;
    }
  }

  // Try to convert character set to UTF-8.
  if (preg_match('/.*charset="?([^";]+)"?.*/', $part['headers']['Content-Type'], $m)) {
    $content = drupal_convert_to_utf8($content, $m[1]);
    //$content = iconv($m[1], 'utf-8', $content);
  }

  return $content;
}

/*
 * Convert a mime part into a file array
 */
function mimemail_parse_attachment($part) {
  $m = array();
  if (preg_match('/.*filename="?([^";])"?.*/', $part['headers']['Content-Disposition'], $m)) {
    $name = $m[1];
  }
  elseif (preg_match('/.*name="?([^";])"?.*/', $part['headers']['Content-Type'], $m)) {
    $name = $m[1];
  }

  return array(
    'filename' => $name,
    'filemime' => $part['content-type'],
    'content'  => mimemail_parse_content($part),
  );
}

/**
 * Helper function to format urls
 *
 * @param $url an url
 *
 * @return an absolute url, sans mailto:
 */
function _mimemail_url($url, $embed_file = NULL) {
  global $base_url;
  $url = urldecode($url);

  // If the URL is absolute or a mailto, return it as-is.
  if (strpos($url, '://') !== FALSE || preg_match('!mailto:!', $url)) {
    $url = str_replace(' ', '%20', $url);
    return $url;
  }

  $url = preg_replace( '!^'. base_path() .'!', '', $url, 1);

  // If we're processing to embed the file, we're done here so return.
  if ($embed_file) return $url;

  if (!preg_match('!^\?q=*!', $url)) {
    $strip_clean = TRUE;
  }

  $url = str_replace('?q=', '', $url);
  list($url, $fragment) = explode('#', $url, 2);
  list($path, $query) = explode('?', $url, 2);

  // If we're dealing with an intra-document reference, return it.
  if (empty($path) && !empty($fragment)) {
    return '#'. $fragment;
  }

  // If we have not yet returned, then let's clean things up and leave.
  $url = url($path, array('query' => $query, 'fragment' => $fragment, 'absolute' => TRUE));


  // If url() added a ?q= where there should not be one, remove it.
  if ($strip_clean) $url = preg_replace('!\?q=!', '', $url);

  $url = str_replace('+', '%20', $url);
  return $url;
}

/**
 * Formats an address string.
 * TODO could use some enhancement and stress testing.
 *
 * @param $address
 *   A user object, a text email address or an array containing name, mail.
 * @return
 *   A formatted address string or FALSE.
 */
function mimemail_address($address) {

  if (is_array($address)) {

    // it's an array containing 'mail' and/or 'name'
    if (isset($address['mail'])) {
      $output = '';
      if (empty($address['name'])) {
        return $address['mail'];
      }
      else {
        return '"'. addslashes(mime_header_encode($address['name'])) .'" <'. $address['mail'] .'>';
      }
    }

    // it's an array of address items
    $addresses = array();
    foreach ($address as $a) {
      $addresses[] = mimemail_address($a);
    }
    return $addresses;
  }

  // it's a user object
  if (is_object($address) && isset($address->mail)) {
    if (empty($address->name)) {
      return $address->mail;
    }
    else {
      return '"'. addslashes(mime_header_encode($address->name)) .'" <'. $address->mail .'>';
    }
  }

  // it's formatted or unformatted string
  // TODO shouldn't assume it's valid - should try to re-parse
  if (is_string($address)) {
    return $address;
  }

  // it's null.  return the site default address
  if (is_null($address)) {
    return array(
      'name' => mime_header_encode(variable_get('site_name', 'Drupal')),
      'mail' => variable_get('site_mail', ini_get('sendmail_from')),
    );
  }

  return FALSE;
}
