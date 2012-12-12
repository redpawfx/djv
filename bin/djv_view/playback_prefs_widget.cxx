/*------------------------------------------------------------------------------
 bin/djv_view/playback_prefs_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "playback_prefs.h"

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
namespace playback {

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget()
{
  using namespace djv_ui;
  
  // Create widgets.

  group_box::Box_Large * general_group =
    new group_box::Box_Large(prefs_group);
  
  Check_Button * start_widget =
    new Check_Button(prefs_start);
  
  Radio_Button_Group * loop_widget =
    new Radio_Button_Group(prefs_loop, label_loop());
  
  Check_Button * every_frame_widget =
    new Check_Button(prefs_every_frame);

  group_box::Box_Large * ui_group =
    new group_box::Box_Large(prefs_ui_group);
  
  Radio_Button_Group * layout_widget =
    new Radio_Button_Group(prefs_layout, label_layout());
  
  // Layout.
  
  Layout_V * layout = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());
  
  layout->add(general_group);
  general_group->layout()->add(start_widget);
  general_group->layout()->add(loop_widget);
  general_group->layout()->add(every_frame_widget);

  layout->add(ui_group);
  ui_group->layout()->add(layout_widget);

  layout->add_stretch();

  // Initialize.
  
  start_widget->set(prefs()->start());
  loop_widget->set(prefs()->loop());
  every_frame_widget->set(prefs()->every_frame());
  layout_widget->set(prefs()->layout());

  // Callbacks.

  start_widget->signal.set(this, start_callback);
  loop_widget->signal.set(this, loop_callback);
  every_frame_widget->signal.set(this, every_frame_callback);
  layout_widget->signal.set(this, layout_callback);
}

void Prefs_Widget::start_callback(bool in)
{
  prefs()->start(in);
}

void Prefs_Widget::loop_callback(int in)
{
  prefs()->loop(static_cast<LOOP>(in));
}

void Prefs_Widget::every_frame_callback(bool in)
{
  prefs()->every_frame(in);
}

void Prefs_Widget::layout_callback(int in)
{
  prefs()->layout(static_cast<LAYOUT>(in));
}

//------------------------------------------------------------------------------

}
}
}

