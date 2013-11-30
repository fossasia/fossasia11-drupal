$Id: README.txt,v 1.1.4.1 2009/06/03 22:25:34 crell Exp $

ABOUT

This module provides two additional Views display plugins, "Profile" and "Node content".
These displays do not display on a page but get "attached" to either a user
page or nodes of specified types.  These views are not stored with the user or
node, the way viewreference.module would, so they can be globally-reconfigured.

The node content display can be reordered on the "Manage fields" tab if CCK is
enabled.  If not, it will have a weight of 10 so it floats to the bottom of the
node.

For added fun, configure a view that takes a node id for a nodereference field
as an argument, then attach it to nodes.  Poof, instant contextual views!

Views_attach was originally developed by Jeff Eaton but never released.  Larry
Garfield later cleaned it up and added the CCK integration.

REQUIREMENTS

- Drupal 6
- Views 2
- Token

AUTHOR AND CREDIT

Original Development:
Jeff Eaton "eaton" (http://drupal.org/user/16496)

Maintainer:
Larry Garfield "Crell" (http://drupal.org/user/26398)
