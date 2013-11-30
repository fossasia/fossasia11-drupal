<?php // $Id: mimemail_compress.inc,v 1.11 2010/09/12 17:37:23 sgabe Exp $

/**
 * @file
 * Converts CSS styles into inline style attributes.
 *
 * Code based on Emogrifier by Pelago Design (http://www.pelagodesign.com).
 */

/**
 * Separate CSS from HTML for processing
 */
function mimemail_compress_clean_message($message) {
  $parts = array();
  preg_match('|(<style[^>]+)>(.*)</style>|mis', $message, $matches);
  $css = str_replace('<!--', '', $matches[2]);
  $css = str_replace('-->', '', $css);
  $css = preg_replace('|\{|', "\n{\n", $css);
  $css = preg_replace('|\}|', "\n}\n", $css);
  $html = str_replace($matches[0], '', $message);
  $parts = array('html' => $html, 'css' => $css);
  return $parts;
}

/**
 * Compress HTML and CSS into combined message
 */
class mimemail_compress {
  private $html = '';
  private $css = '';
  private $unprocessable_tags = array('wbr');

  public function mimemail_compress($html = '', $css = '') {
    $this->html = $html;
    $this->css  = $css;
  }

  // There are some HTML tags that DOMDocument cannot process,
  // and will throw an error if it encounters them.
  // These functions allow you to add/remove them if necessary.
  // It only strips them from the code (does not remove actual nodes).
  public function add_unprocessable_tag($tag) {
    $this->unprocessable_tags[] = $tag;
  }

  public function remove_unprocessable_tag($tag) {
    if (($key = array_search($tag, $this->unprocessable_tags)) !== FALSE) {
      unset($this->unprocessableHTMLTags[$key]);
    }
  }

  public function compress() {
    if (!class_exists('DOMDocument', FALSE)) {
      return $this->html;
    }

    $body = $this->html;
    // Process the CSS here, turning the CSS style blocks into inline CSS.
    if (count($this->unprocessable_tags)) {
      $unprocessable_tags = implode('|', $this->unprocessable_tags);
      $body = preg_replace("/<($unprocessable_tags)[^>]*>/i", '', $body);
    }

    $encoding = mb_detect_encoding($body);
    $body = mb_convert_encoding($body, 'HTML-ENTITIES', $encoding);

    $err = error_reporting(0);
    $doc = new DOMDocument;
    $doc->encoding = $encoding;
    $doc->strictErrorChecking = FALSE;
    $doc->formatOutput = TRUE;
    $doc->loadHTML($body);
    $doc->normalizeDocument();

    $xpath = new DOMXPath($doc);

    // Before be begin processing the CSS file, parse the document and normalize
    // all existing CSS attributes (changes 'DISPLAY: none' to 'display: none');
    // we wouldn't have to do this if DOMXPath supported XPath 2.0.
    $nodes = @$xpath->query('//*[@style]');
    if ($nodes->length > 0) {
      foreach ($nodes as $node) {
        $node->setAttribute('style', preg_replace('/[A-z\-]+(?=\:)/Se', "strtolower('\\0')", $node->getAttribute('style')));
      }
    }

    // Get rid of comments.
    $css = preg_replace('/\/\*.*\*\//sU', '', $this->css);

    // Process the CSS file for selectors and definitions.
    preg_match_all('/^\s*([^{]+){([^}]+)}/mis', $css, $matches);

    $all_selectors = array();
    foreach ($matches[1] as $key => $selector_string) {
      // If there is a blank definition, skip.
      if (!strlen(trim($matches[2][$key]))) continue;
      // Else split by commas and duplicate attributes so we can sort by selector precedence.
      $selectors = explode(',', $selector_string);
      foreach ($selectors as $selector) {
        // Don't process pseudo-classes.
        if (strpos($selector, ':') !== FALSE) continue;
        $all_selectors[] = array(
          'selector' => $selector,
          'attributes' => $matches[2][$key],
          'index' => $key, // Keep track of where it appears in the file, since order is important.
        );
      }
    }

    // Now sort the selectors by precedence.
    usort($all_selectors, array('self', 'sort_selector_precedence'));

    foreach ($all_selectors as $value) {
      // Query the body for the xpath selector.
      $nodes = $xpath->query($this->css_to_xpath(trim($value['selector'])));

      foreach ($nodes as $node) {
        // If it has a style attribute, get it, process it, and append (overwrite) new stuff.
        if ($node->hasAttribute('style')) {
          // Break it up into an associative array.
          $old_style = $this->css_style_to_array($node->getAttribute('style'));
          $new_style = $this->css_style_to_array($value['attributes']);
          // New styles overwrite the old styles (not technically accurate, but close enough).
          $compressed = array_merge($old_style, $new_style);
          $style = '';
          foreach ($compressed as $k => $v) $style .= (strtolower($k) . ':' . $v . ';');
        }
        else {
          // Otherwise create a new style.
          $style = trim($value['attributes']);
        }
        $node->setAttribute('style', $style);
      }
    }

    // This removes styles from your email that contain display:none. You could comment these out if you want.
    $nodes = $xpath->query('//*[contains(translate(@style," ",""), "display:none")]');
    foreach ($nodes as $node) $node->parentNode->removeChild($node);

    error_reporting($err);

    return $doc->saveHTML();
  }

  private static function sort_selector_precedence($a, $b) {
    $precedenceA = self::get_selector_precedence($a['selector']);
    $precedenceB = self::get_selector_precedence($b['selector']);

    // We want these sorted ascendingly so selectors with lesser precedence get processed first and selectors with greater precedence get sorted last.
    return ($precedenceA == $precedenceB) ? ($a['index'] < $b['index'] ? -1 : 1) : ($precedenceA < $precedenceB ? -1 : 1);
  }

  private static function get_selector_precedence($selector) {
    $precedence = 0;
    $value = 100;
    $search = array('\#', '\.', ''); // ids: worth 100, classes: worth 10, elements: worth 1

    foreach ($search as $s) {
      if (trim($selector == '')) break;
      $num = 0;
      $selector = preg_replace('/'. $s .'\w+/', '', $selector, -1, $num);
      $precedence += ($value * $num);
      $value /= 10;
    }

    return $precedence;
  }

  /**
   * Right now we only support CSS 1 selectors, but include CSS2/3 selectors are fully possible.
   *
   * @see http://plasmasturm.org/log/444/
   */
  private function css_to_xpath($selector) {
    // Returns an Xpath selector.
    $search = array(
      '/\s+>\s+/', // Matches any F element that is a child of an element E.
      '/(\w+)\s+\+\s+(\w+)/', // Matches any F element that is a child of an element E.
      '/\s+/', // Matches any F element that is a descendant of an E element.
      '/(\w)\[(\w+)\]/', // Matches element with attribute.
      '/(\w)\[(\w+)\=[\'"]?(\w+)[\'"]?\]/', // Matches element with EXACT attribute.
      '/(\w+)?\#([\w\-]+)/e', // Matches id attributes.
      '/(\w+|\*)?((\.[\w\-]+)+)/e', // Matches class attributes.
    );
    $replace = array(
      '/',
      '\\1/following-sibling::*[1]/self::\\2',
      '//',
      '\\1[@\\2]',
      '\\1[@\\2="\\3"]',
      "(strlen('\\1') ? '\\1' : '*').'[@id=\"\\2\"]'",
      "(strlen('\\1') ? '\\1' : '*').'[contains(concat(\" \",normalize-space(@class),\" \"),concat(\" \",\"'.implode('\",\" \"))][contains(concat(\" \",normalize-space(@class),\" \"),concat(\" \",\"',explode('.',substr('\\2',1))).'\",\" \"))]'",
    );
    return '//'. preg_replace($search, $replace, trim($selector));
  }

  private function css_style_to_array($style) {
    $definitions = explode(';', $style);
    $css_styles = array();
    foreach ($definitions as $def) {
      if (empty($def) || strpos($def, ':') === FALSE) continue;
      list($key, $value) = explode(':', $def, 2);
      if (empty($key) || empty($value)) continue;
      $css_styles[trim($key)] = trim($value);
    }
    return $css_styles;
  }
}
