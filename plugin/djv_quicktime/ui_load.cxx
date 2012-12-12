/*------------------------------------------------------------------------------
 plugin/djv_quicktime/ui_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "ui_load.h"

#include <djv_ui/group_box.h>
#include <djv_ui/int_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>

namespace djv_plugin {
namespace quicktime {

//------------------------------------------------------------------------------
// djv_ui_image_load()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_ui_image_load()
{
  return new Load_Widget;
}

}

//------------------------------------------------------------------------------
// Load_Widget::_Load_Widget
//------------------------------------------------------------------------------

struct Load_Widget::_Load_Widget
{
  Int_Edit * start_frame;
};

//------------------------------------------------------------------------------
// Load_Widget
//------------------------------------------------------------------------------

Load_Widget::Load_Widget() :
  _plugin(0),
  _p(new _Load_Widget)
{
  // Create widgets.
  
  group_box::Box_Large * start_frame_group =
    new group_box::Box_Large(label_start_frame_group);

  _p->start_frame =
    new Int_Edit;
  _p->start_frame->range(0, value::int_max);

  // Layout.
  
  Layout_V * layout = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(start_frame_group);
  start_frame_group->layout()->add(_p->start_frame);
  
  layout->add_stretch();
  
  // Initialize.
  
  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->start_frame->signal.set(this, start_frame_callback);
}

Load_Widget::~Load_Widget()
{}

plugin::Plugin * Load_Widget::copy() const
{
  return new Load_Widget;
}

String Load_Widget::name() const
{
  return quicktime::name;
}

void Load_Widget::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Load_Widget::start_frame_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.start_frame = in;
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Load_Widget::plugin_update()
{
  callbacks(false);

  if (_plugin)
  {
    String tmp;
    tmp << _option.start_frame;
    _plugin->option(_plugin->option_list()[LOAD_START_FRAME], &tmp);
  }

  callbacks(true);
}

void Load_Widget::widget_update()
{
  callbacks(false);

  try
  {
    if (_plugin)
    {
      String tmp;
      tmp = _plugin->option(_plugin->option_list()[LOAD_START_FRAME]);
      tmp >> _option.start_frame;
    }
  }
  catch (String)
  {}

  _p->start_frame->set(_option.start_frame);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

