/*------------------------------------------------------------------------------
 plugin/djv_png/etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "base.h"

namespace djv_plugin {
namespace png {

//------------------------------------------------------------------------------
// djv_png_error()
//------------------------------------------------------------------------------

extern "C" {

void djv_png_error(png_structp in, png_const_charp msg)
{
  Png_Error * error = (Png_Error *)in->error_ptr;
  
  SNPRINTF(error->msg, string::cstring_size, "%s", msg);
  
  longjmp(in->jmpbuf, 1);
}

}

//------------------------------------------------------------------------------
// djv_png_warning()
//------------------------------------------------------------------------------

extern "C" {

void djv_png_warning(png_structp in, png_const_charp msg)
{
  Png_Error * error = (Png_Error *)in->error_ptr;
  
  SNPRINTF(error->msg, string::cstring_size, "%s", msg);
  
  longjmp(in->jmpbuf, 1);
}

}

//------------------------------------------------------------------------------

}
}

