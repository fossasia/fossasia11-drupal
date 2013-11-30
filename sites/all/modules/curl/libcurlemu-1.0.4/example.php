<?php

// CURL Extension Emulation Library Example
//
// Usage should be straightforward; you simply use this script exactly as you
// would normally use the PHP CURL extension functions.

// first, include libcurlemu.inc.php
require_once('libcurlemu.inc.php');

// at this point, libcurlemu has detected the best available CURL solution
// (either the CURL extension, if available, or the CURL commandline binary,
// if available, or as a last resort, HTTPRetriever, our native-PHP HTTP
// client implementation) and has implemented the curl_* functions if
// necessary, so you can use CURL normally and safely assume that all CURL
// functions are available.

// the rest of this example code is copied straight from the PHP manual's
// reference for the curl_init() function, and will work fine with libcurlemu

// create a new CURL resource
$ch = curl_init();

// set URL and other appropriate options
curl_setopt($ch, CURLOPT_URL, "http://www.example.com/");
curl_setopt($ch, CURLOPT_HEADER, false);

// grab URL and pass it to the browser
curl_exec($ch);

// close CURL resource, and free up system resources
curl_close($ch);


?>
