/*------------------------------------------------------------------------------
 plugin/djv_jpeg/base.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_JPEG_BASE_H
#define DJV_JPEG_BASE_H

#include <djv_image/base.h>
#include <stdio.h>
namespace libjpeg { // XXX Windows conflicts?
extern "C" {
#include <jpeglib.h>
}
}
#include <setjmp.h>

namespace djv_plugin {

//------------------------------------------------------------------------------
//!\namespace djv_plugin::jpeg
//
//! JPEG plugin - Joint Photographic Experts Group image file format.
//!
//! Requires:
//!
//! - libjpeg - http://www.ijg.org
//!
//! Support:
//!
//! - Image data: L, RGB, 8-bit
//! - File compression
//------------------------------------------------------------------------------

namespace jpeg {
using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "JPEG";

static const List<String> extension_list = List<String>() <<
  ".jpeg" <<
  ".jpg" <<
  ".jfif";

//@}
//------------------------------------------------------------------------------
//!\name Errors
//------------------------------------------------------------------------------
//@{

//! libjpeg error.

struct Jpeg_Error
{
  struct libjpeg::jpeg_error_mgr pub;
  
  char msg [JMSG_LENGTH_MAX];
  
  jmp_buf jump;
};

extern "C" {

void djv_jpeg_error(libjpeg::j_common_ptr);
void djv_jpeg_warning(libjpeg::j_common_ptr, int);

}

//@}
//------------------------------------------------------------------------------

}
}

#endif

