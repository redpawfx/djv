/*------------------------------------------------------------------------------
 bin/djv_view/input_base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_INPUT_BASE_H
#define DJV_VIEW_INPUT_BASE_H

#include "base.h"
#include <djv_base/base.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::input
//
//! Device input.
//------------------------------------------------------------------------------

namespace input {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Mouse wheel bindings.

enum MOUSE_WHEEL
{
  MOUSE_WHEEL_VIEW_ZOOM,
  MOUSE_WHEEL_PLAYBACK_SHUTTLE,
  MOUSE_WHEEL_PLAYBACK_SPEED,
  
  _MOUSE_WHEEL_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

String & operator >> (String &, MOUSE_WHEEL &) throw (String);

String & operator << (String &, MOUSE_WHEEL);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_mouse_wheel();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

