/*------------------------------------------------------------------------------
 bin/djv_view/input_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_INPUT_PREFS_H
#define DJV_VIEW_INPUT_PREFS_H

#include "input_base.h"
#include <djv_ui/widget.h>

namespace djv_bin {
namespace view {
namespace input {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! Device input preferences.
//------------------------------------------------------------------------------

class Prefs : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Mouse Wheel
  //@{
  
  void mouse_wheel(MOUSE_WHEEL);
  void mouse_wheel_shift(MOUSE_WHEEL);
  void mouse_wheel_ctrl(MOUSE_WHEEL);
  
  MOUSE_WHEEL mouse_wheel() const;
  MOUSE_WHEEL mouse_wheel_shift() const;
  MOUSE_WHEEL mouse_wheel_ctrl() const;
  
  //@}
  
private:

  MOUSE_WHEEL _mouse_wheel;
  MOUSE_WHEEL _mouse_wheel_shift;
  MOUSE_WHEEL _mouse_wheel_ctrl;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! Device input preferences widget.
//------------------------------------------------------------------------------

class Prefs_Widget : public djv_ui::Widget
{
public:

  Prefs_Widget();
  
  ~Prefs_Widget();

private:

  DJV_CALLBACK(Prefs_Widget, mouse_wheel_callback, int);
  DJV_CALLBACK(Prefs_Widget, mouse_wheel_shift_callback, int);
  DJV_CALLBACK(Prefs_Widget, mouse_wheel_ctrl_callback, int);
  
  struct _Prefs_Widget;
  std::auto_ptr<_Prefs_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  prefs_title,
  prefs_mouse_wheel_group,
  prefs_mouse_wheel,
  prefs_mouse_wheel_shift,
  prefs_mouse_wheel_ctrl;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

