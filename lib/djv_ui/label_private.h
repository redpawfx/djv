/*------------------------------------------------------------------------------
 lib/djv_ui/_label.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef _DJV_UI_LABEL_H
#define _DJV_UI_LABEL_H

#include <djv_ui/label.h>

namespace djv_ui {
namespace label {

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

inline int offset(layout_item::ALIGN in, int size, int size_hint)
{
  int out = 0;
  switch (in)
  {
    case layout_item::LEFT: break;
    case layout_item::CENTER: out += size / 2 - size_hint / 2; break;
    case layout_item::RIGHT: out += size - size_hint; break;
  }
  return out;
}

//------------------------------------------------------------------------------

}
}

#endif

