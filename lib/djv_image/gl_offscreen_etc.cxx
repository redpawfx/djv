/*------------------------------------------------------------------------------
 lib/djv_image/gl_offscreen_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "gl_offscreen_private.h"

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(BUFFER, label_buffer())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_buffer()
{
  static const List<String> data = List<String>() <<
    "PBuffer" <<
    "FBO";
  
  DJV_ASSERT(data.size() == _BUFFER_SIZE);
  
  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, BUFFER in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------

}
}

