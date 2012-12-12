/*------------------------------------------------------------------------------
 lib/djv_image/pixel_etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "pixel.h"

namespace djv_image {
namespace pixel {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(FORMAT, label_format())
_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())
_DJV_STRING_OPERATOR_LABEL(PIXEL, label_pixel())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_format()
{
  static const List<String> data = List<String>() <<
    "L" <<
    "LA" <<
    "RGB" <<
    "RGBA";
  
  DJV_ASSERT(data.size() == _FORMAT_SIZE);

  return data;
}

const List<String> & label_type()
{
  static const List<String> data = List<String>() <<
    "U8" <<
    "U10" <<
    "U16" <<
#if defined(DJV_OPENEXR)
    "F16" <<
#endif
    "F32";

  DJV_ASSERT(data.size() == _TYPE_SIZE);

  return data;
}

const List<String> & label_pixel()
{
  static const List<String> data = List<String>() <<
    "L U8" <<
    "L U16" <<
#if defined(DJV_OPENEXR)
    "L F16" <<
#endif
    "L F32" <<
    "LA U8" <<
    "LA U16" <<
#if defined(DJV_OPENEXR)
    "LA F16" <<
#endif
    "LA F32" <<
    "RGB U8" <<
    "RGB U10" <<
    "RGB U16" <<
#if defined(DJV_OPENEXR)
    "RGB F16" <<
#endif
    "RGB F32" <<
    "RGBA U8" <<
    "RGBA U16" <<
#if defined(DJV_OPENEXR)
    "RGBA F16" <<
#endif
    "RGBA F32";

  DJV_ASSERT(data.size() == _PIXEL_SIZE);

  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, FORMAT in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, TYPE in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, PIXEL in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------

}
}
