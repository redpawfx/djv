/*------------------------------------------------------------------------------
 lib/djv_ui/image.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_H
#define DJV_UI_IMAGE_H

#include <djv_ui/base.h>
#include <djv_image/image.h>
#include <FL/Fl_Image.H>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::image
//
//! Image widgets.
//------------------------------------------------------------------------------

namespace image {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_UI Fl_Image * image_to_fltk(
  const djv_image::Data &,
  const V2i & resize = V2i(),
  const djv_image::Color_Profile & = djv_image::Color_Profile()
);

//@}
//------------------------------------------------------------------------------

}
}

#endif

