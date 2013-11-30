README.txt for htmltidy
=======================
This module tidies up Drupal's HTML output.
It currently supports, configured via the normal interface:
 - indenting
 - word wrapping (with advanced word-wrapping options)
 - doctype specification for advanced control over validation
 - auto-enclosing blocks of text
 - optional info/warning/error information appended to pages
   including wrong HTML attributes and w3c validation info.

Additional settings can be configured using an HTMLTidy conf 
file (as documented at the tidy website)

It does this by running every web page Drupal creates through
HTML tidy (which you can download for a variety of operating
systems from http://tidy.sourceforge.net/).

Alternatively, the process can be installed as an input filter
to tidy up just user input, either as it's entered
([Validate input text] option) or as a Drupal filter.

Bugs/comments/suggestions/patches to xabbu at cox dot net

Extra enhancements (2006) by dman http://coders.co.nz
  sponsored by Gabriele Ferri - www.semioticamente.it
