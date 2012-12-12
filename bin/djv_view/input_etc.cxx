/*------------------------------------------------------------------------------
 bin/djv_view/input_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "input_base.h"

#include "input_prefs.h"

namespace djv_bin {
namespace view {
namespace input {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(MOUSE_WHEEL, label_mouse_wheel())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_mouse_wheel()
{
  static const List<String> data = List<String>() <<
    "View zoom" <<
    "Playback shuttle" <<
    "Playback speed";
  
  DJV_ASSERT(data.size() == _MOUSE_WHEEL_SIZE);
  
  return data;
}

extern const String
  prefs_title = "Input",
  prefs_mouse_wheel_group = "Mouse Wheel",
  prefs_mouse_wheel = "Wheel",
  prefs_mouse_wheel_shift = "Shift + Wheel",
  prefs_mouse_wheel_ctrl = "Ctrl + Wheel";

//------------------------------------------------------------------------------

}
}
}

