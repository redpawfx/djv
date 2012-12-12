/*------------------------------------------------------------------------------
 bin/djv_view/input_prefs_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "input_prefs.h"

#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_bin {
namespace view {
namespace input {

//------------------------------------------------------------------------------
// Prefs_Widget::_Prefs_Widget
//------------------------------------------------------------------------------

struct Prefs_Widget::_Prefs_Widget
{
};

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget() :
  _p(new _Prefs_Widget)
{
  using namespace djv_ui;
  
  // Create widgets.

  group_box::Box_Large * mouse_wheel_group =
    new group_box::Box_Large(prefs_mouse_wheel_group);
  
  Radio_Button_Group * mouse_wheel_widget =
    new Radio_Button_Group(prefs_mouse_wheel, label_mouse_wheel());
  
  Radio_Button_Group * mouse_wheel_shift_widget =
    new Radio_Button_Group(prefs_mouse_wheel_shift, label_mouse_wheel());
  
  Radio_Button_Group * mouse_wheel_ctrl_widget =
    new Radio_Button_Group(prefs_mouse_wheel_ctrl, label_mouse_wheel());

  // Layout.
  
  Layout_V * layout = 0;
  Layout_H * layout_h = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(mouse_wheel_group);
  layout_h = new Layout_H(mouse_wheel_group->layout());
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(mouse_wheel_widget);
  layout_h->add(mouse_wheel_shift_widget);
  layout_h->add(mouse_wheel_ctrl_widget);
  layout_h->add_stretch();
  
  layout->add_stretch();
  
  // Initialize.
  
  mouse_wheel_widget->set(prefs()->mouse_wheel());
  mouse_wheel_shift_widget->set(prefs()->mouse_wheel_shift());
  mouse_wheel_ctrl_widget->set(prefs()->mouse_wheel_ctrl());
  
  // Callbacks.
  
  mouse_wheel_widget->signal.set(this, mouse_wheel_callback);
  mouse_wheel_shift_widget->signal.set(this, mouse_wheel_shift_callback);
  mouse_wheel_ctrl_widget->signal.set(this, mouse_wheel_ctrl_callback);
}

Prefs_Widget::~Prefs_Widget()
{}

void Prefs_Widget::mouse_wheel_callback(int in)
{
  prefs()->mouse_wheel(static_cast<MOUSE_WHEEL>(in));
}

void Prefs_Widget::mouse_wheel_shift_callback(int in)
{
  prefs()->mouse_wheel_shift(static_cast<MOUSE_WHEEL>(in));
}

void Prefs_Widget::mouse_wheel_ctrl_callback(int in)
{
  prefs()->mouse_wheel_ctrl(static_cast<MOUSE_WHEEL>(in));
}

//------------------------------------------------------------------------------

}
}
}

