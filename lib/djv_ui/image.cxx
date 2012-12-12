/*------------------------------------------------------------------------------
 lib/djv_ui/image.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "image.h"

#include <djv_image/gl_data.h>

namespace djv_ui {
namespace image {

//------------------------------------------------------------------------------
// image_to_fltk()
//------------------------------------------------------------------------------

Fl_Image * image_to_fltk(
  const djv_image::Data & in,
  const V2i & resize,
  const djv_image::Color_Profile & color_profile
) {
  using namespace djv_image;
  
  //DJV_DEBUG("fl_image");
  //DJV_DEBUG_PRINT("in = " << in);
  
  // Temporary buffer.
  
  Data tmp(
    data::Info(
      vector::is_size_valid(resize) ? resize : in.size(),
      pixel::RGB_U8
    )
  );
  
  //DJV_DEBUG_PRINT("tmp = " << tmp);

  // Transform.
  
  gl_data::Option option;
  
  option.xform.scale =
    V2f(tmp.size()) /
    (V2f(in.size() * data::proxy_scale(in.info().proxy)));
  
  option.xform.mirror.y = true;
  
  option.color_profile = color_profile;
  
  gl_data::copy(in, &tmp, option);

  // Convert to FLTK.
  
  return Fl_RGB_Image(tmp.data(), tmp.w(), tmp.h()).copy();
}

//------------------------------------------------------------------------------

}
}

