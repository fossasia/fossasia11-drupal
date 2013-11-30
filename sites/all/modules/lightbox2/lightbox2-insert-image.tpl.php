<?php
// $Id: lightbox2-insert-image.tpl.php,v 1.1.2.2 2010/09/22 10:16:30 snpower Exp $

/**
 * @file
 * Template file for Lightbox2 content inserted via the Insert module.
 *
 * Available variables:
 * - $item: The complete item being inserted.
 * - $url: The URL to the image.
 * - $linkurl: The URL to the image being linked to.
 * - $class: A set of classes assigned to this image (if any).
 * - $image_preset_name: The ImageCache preset being used for the image.
 * - $link_preset_name: The ImageCache preset being used for the linked to image.
 * - $download_link: The download original link.
 *
 * Note that ALT and Title fields should not be filled in here, instead they
 * should use placeholders that will be updated through JavaScript when the
 * image is inserted.
 *
 * Available placeholders:
 * - __alt__: The ALT text, intended for use in the <img> tag.
 * - __title__: The Title text, intended for use in the <img> tag.
 * - __description__: A description of the image, sometimes used as a caption.
 */
?>
<a rel="lightbox[group1][__description__<?php print $download_link ?>]" href="<?php print $linkurl ?>"><img src="<?php print $url ?>" alt="__alt__" title="__title__" class="imagecache-<?php print $image_preset_name ?><?php print $class ? ' ' . $class : '' ?>" /></a>
