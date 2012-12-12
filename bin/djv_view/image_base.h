/*------------------------------------------------------------------------------
 bin/djv_view/image_base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_IMAGE_BASE_H
#define DJV_VIEW_IMAGE_BASE_H

#include "base.h"
#include <djv_image/gl_data.h>
#include <djv_image/image_def.h>
#include <djv_base/file.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::image
//
//! Images.
//------------------------------------------------------------------------------

namespace image {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Image scale.

enum SCALE
{
  SCALE_NONE,
  SCALE_16_9,
  SCALE_1_85,
  SCALE_2_35,
  
  _SCALE_SIZE
};

//! Image rotate.

enum ROTATE
{
  ROTATE_0,
  ROTATE_90,
  ROTATE_180,
  ROTATE_270,
  
  _ROTATE_SIZE
};

//! Display profile.

struct Display_Profile : public djv_image::gl_data::Display_Profile
{
  String name;
  
  File lut_file;
};

//! Channel.

using djv_image::gl_data::CHANNEL;

//@}
//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

V2f scale(SCALE, const V2i &);

double rotate(ROTATE);

const djv_image::Image * lut_load(const File &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

String & operator >> (String &, SCALE &) throw (String);
String & operator >> (String &, ROTATE &) throw (String);
String & operator >> (String &, Display_Profile &) throw (String);

String & operator << (String &, SCALE);
String & operator << (String &, ROTATE);
String & operator << (String &, const Display_Profile &);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_scale();
const List<String> & label_rotate();
using djv_image::gl_data::label_channel;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

