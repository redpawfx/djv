/*------------------------------------------------------------------------------
 lib/djv_image/image.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "image.h"

namespace djv_image {
namespace image {

//------------------------------------------------------------------------------
// Image
//------------------------------------------------------------------------------

Image::Image()
{}

Image::Image(const data::Info & in, const uint8_t * p) :
  Data(in, p)
{}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Image & a, const Image & b)
{
  return
    static_cast<const Data &>(a) == static_cast<const Data &>(b) &&
    a.tag == b.tag &&
    a.color_profile == b.color_profile;
}

bool operator != (const Image & a, const Image & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const Image & in)
{
  return debug << in.info() << ", color profile: " << in.color_profile.type;
}

//------------------------------------------------------------------------------

}
}

