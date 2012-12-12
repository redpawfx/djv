/*------------------------------------------------------------------------------
 plugin/djv_png/base.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_PNG_BASE_H
#define DJV_PNG_BASE_H

#include <djv_image/base.h>
#include <png.h>

namespace djv_plugin {

//------------------------------------------------------------------------------
//!\namespace djv_plugin::png
//
//! PNG plugin - Portable Network Graphics image file format.
//!
//! Requires:
//!
//! - libpng - http://www.libpng.org
//!
//! Support:
//!
//! - Image data: L, RGB, RGBA, 8-bit, 16-bit
//! - File compression
//------------------------------------------------------------------------------

namespace png {
using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "PNG";

static const List<String> extension_list = List<String>() <<
  ".png";

//@}
//------------------------------------------------------------------------------
//!\name Errors
//------------------------------------------------------------------------------
//@{

//! libpng error.

struct Png_Error
{
  char msg [string::cstring_size];
};

extern "C" {

void djv_png_error(png_structp, png_const_charp);
void djv_png_warning(png_structp, png_const_charp);

}

//@}
//------------------------------------------------------------------------------

}
}

#endif

