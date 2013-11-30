/* $Id $ */

Summary
-------
As the name suggests, Custom Breadcrumbs allows you to create and modify your
own breadcrumbs based on node type. After enabling the module, click on
Administer > Site building > Custom breadcrumbs. On this page you'll see
the option to add a new custom breadcrumb. 

Clicking on that link, you have the option to select the node type the breadcrumb 
will apply to. There are two text fields below-- "Titles" and "Paths." When creating 
a breadcrumb, you're simply creating a link. In the custom breadcrumbs interface 
"Titles" describes the text of the breadcrumb while "Paths" describes the Drupal 
path the breadcrumb links to. Each Title must have a corresponding Path. 

To give a very simple example of how to use this module, let's say I have a blog on 
my web site called "Deep Thoughts." To create this, I use the Views module to create 
a page at /blog that displays all the node types "blog post." Whenever a user views 
a blog post I want the breadcrumb to show Home > Deep Thoughts instead of simply Home. 
To do this I would simply type "Deep Thoughts" in the "Titles" field and and "blog" 
in the "Paths" field and save my breadcrumb.

Using the Tokens module, the Custom breadcrumbs module becomes much more flexible 
because breadcrumbs can become dynamic. You can create a breadcrumb like 
Home > Deep Thoughts > [Month of Blog Post] [Year of Blog Post], where "Deep Thoughts" 
links to my main blog page and "[Month of Blog Post] [Year of Blog Post]" links to 
a view that shows only blog posts from the month and year the blog post was created 
(e.g. June 2007). For this, you would do the following:

Node Type:
Blog Post

Titles:
Deep Thoughts
[month] [yyyy]

Paths:
blog
blog/[mm]_[yyyy]

(where of course, blog/[mm]_[yyyy] is the path to the view of blog posts from that month 
and year). So if you created a blog pos on June 13, 2007 your breadcrumb would show 
Home > Deep Thoughts > June 2007 and "June 2007" links to "blog/06_2007" which is a view 
of all blog posts from June 2007.

Also, note that Custom Breadcrumbs doesn't actually check to be sure that a particular path 
exists, so you'll have to check yourself to avoid 404 errors.

Only users with 'administer custom breadcrumbs' permission will be allowed to 
create or modify custom breadcrumbs.

Breadcrumb Visibility
---------------------
Users given 'use php in custom breadcrumbs' permission can include php code snippet that 
returns TRUE or FALSE to control whether or not the breadcrumb is displayed. Note that this 
code has access to the $node variable, and can check its type or any other property.

Special Identifiers
-------------------
The following identifiers can be used to achieve a special behavior:
<pathauto> - will clean any path using the current pathauto module settings, if that module is installed.
<none>     - can be used as a path to have a breadcrumb element that is not hyperlinked.

Identifiers should be added to the paths area in the following format: identifier|path.
To be recognized, the idenfier must be enclosed in angular brackets, and proceed any 
part of the path:
 
For example: <pathauto>|[ogname-raw]

Authors
-------
bennybobw, dbabbage, MGN
