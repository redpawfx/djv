/*------------------------------------------------------------------------------
 lib/djv_ui/layout_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "layout.h"

namespace djv_ui {
namespace layout {

//------------------------------------------------------------------------------
// window_handle_size()
//------------------------------------------------------------------------------

int window_handle_size()
{
  int out = 0;

#if defined(DJV_MACOSX)

  // XXX
  
  out = 14;

#endif

  return out;
}

//------------------------------------------------------------------------------

}
}
