ImageAPI 

A non writing image manipulation API for Drupal. This API is meant to be used in place of the API provided 
by image.inc. You probably do not need to install this module unless another module are you using requires
it. It provides no new features to your drupal site. It only provides an API other modules can leverage.

Changes From image.inc API:
  - Images are objects.
  - Images are not written on each image operation and must be explicitly 
    closed when processing is complete. 
  - Multiple Image ToolKits can be used simultaneously. However, only the image
    toolkit and image was opened with can be used to process it. This is hidden 
    in the imageapi layer.

API Quick Reference:
  imageapi_image_scale_and_crop($image, $width, $height) 
  imageapi_image_scale($image, $width, $height, $upscale = FALSE) 
  imageapi_image_resize($image, $width, $height) 
  imageapi_image_rotate($image, $degrees, $bgcolor = 0x000000) 
  imageapi_image_crop($image, $x, $y, $width, $height) 
  imageapi_image_desaturate($image) 
  imageapi_image_open($file, $toolkit = FALSE) 
  imageapi_image_close($image, $destination) 

  $image is an image object returned from imageapi_image_open();

Expanding ImageAPI:

  If you wish to expand on ImageAPI add a new wrapper function to 
  imageapi.module. Do any common preprocessing for all underlying layers in the 
  wrapper function, then invoke the driver. Pay heed to the function naming in
  ImageAPI and ImageAPI GD. If the toolkit changes the size of an image it must
  update the $image->info['width'] and $image->info['height'] variables. All 
  ToolKit functions should return TRUE on success and FALSE on failure.

For more detailed documentation read imageapi.module.

-dopry
