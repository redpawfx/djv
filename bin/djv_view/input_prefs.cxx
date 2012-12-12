/*------------------------------------------------------------------------------
 bin/djv_view/input_prefs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "input_prefs.h"

#include <djv_ui/prefs.h>

namespace djv_bin {
namespace view {
namespace input {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  _mouse_wheel(MOUSE_WHEEL_VIEW_ZOOM),
  _mouse_wheel_shift(MOUSE_WHEEL_PLAYBACK_SHUTTLE),
  _mouse_wheel_ctrl(MOUSE_WHEEL_PLAYBACK_SPEED)
{
  DJV_DEBUG("Prefs::Prefs");
  
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "input");
  djv_ui::prefs::get(&prefs, "mouse_wheel", &_mouse_wheel);
  djv_ui::prefs::get(&prefs, "mouse_wheel_shift", &_mouse_wheel_shift);
  djv_ui::prefs::get(&prefs, "mouse_wheel_ctrl", &_mouse_wheel_ctrl);
}

Prefs::~Prefs()
{
  DJV_DEBUG("Prefs::~Prefs");

  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "input");
  djv_ui::prefs::set(&prefs, "mouse_wheel", _mouse_wheel);
  djv_ui::prefs::set(&prefs, "mouse_wheel_shift", _mouse_wheel_shift);
  djv_ui::prefs::set(&prefs, "mouse_wheel_ctrl", _mouse_wheel_ctrl);
}

void Prefs::mouse_wheel(MOUSE_WHEEL in)
{
  if (in == _mouse_wheel)
    return;
  
  _mouse_wheel = in;
}

MOUSE_WHEEL Prefs::mouse_wheel() const
{
  return _mouse_wheel;
}

void Prefs::mouse_wheel_shift(MOUSE_WHEEL in)
{
  if (in == _mouse_wheel_shift)
    return;
  
  _mouse_wheel_shift = in;
}

MOUSE_WHEEL Prefs::mouse_wheel_shift() const
{
  return _mouse_wheel_shift;
}

void Prefs::mouse_wheel_ctrl(MOUSE_WHEEL in)
{
  if (in == _mouse_wheel_ctrl)
    return;
  
  _mouse_wheel_ctrl = in;
}

MOUSE_WHEEL Prefs::mouse_wheel_ctrl() const
{
  return _mouse_wheel_ctrl;
}

//------------------------------------------------------------------------------

}
}
}

