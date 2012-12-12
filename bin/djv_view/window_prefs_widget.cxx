/*------------------------------------------------------------------------------
 bin/djv_view/window_prefs_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "window_prefs.h"

#include <djv_ui/check_button.h>
#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>

namespace djv_bin {
namespace view {
namespace window {

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget()
{
  using namespace djv_ui;
  
  // Create resize widgets.
  
  group_box::Box_Large * resize_group =
    new group_box::Box_Large(prefs_resize_group);
  
  Check_Button * resize_fit_widget =
    new Check_Button(prefs_resize_fit);
  
  Radio_Button_Group * resize_max_widget =
    new Radio_Button_Group(prefs_resize_max, label_resize_max());
  
  // Create toolbar widgets.
  
  group_box::Box_Large * toolbar_group =
    new group_box::Box_Large(prefs_toolbar_group);

  Check_Button_Group * toolbar_widget =
    new Check_Button_Group(label_toolbar());

  // Layout.
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());
  
  layout->add(resize_group);
  resize_group->layout()->add(resize_fit_widget);
  resize_group->layout()->add(resize_max_widget);
  
  layout->add(toolbar_group);
  toolbar_group->layout()->add(toolbar_widget);

  layout->add_stretch();
  
  // Initialize.
  
  resize_fit_widget->set(prefs()->resize_fit());
  resize_max_widget->set(prefs()->resize_max());
  toolbar_widget->set(prefs()->toolbar());

  // Callbacks.

  resize_fit_widget->signal.set(this, resize_fit_callback);
  resize_max_widget->signal.set(this, resize_max_callback);
  toolbar_widget->signal.set(this, toolbar_callback);
}

void Prefs_Widget::resize_fit_callback(bool in)
{
  prefs()->resize_fit(in);
}

void Prefs_Widget::resize_max_callback(int in)
{
  prefs()->resize_max(static_cast<RESIZE_MAX>(in));
}

void Prefs_Widget::toolbar_callback(const List<bool> & in)
{
  prefs()->toolbar(in);
}

//------------------------------------------------------------------------------

}
}
}

