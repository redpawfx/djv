/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_ppm.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_io_ppm.h"

#include <djv_ui/group_box.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace image_io_ppm {
using namespace djv_image::io_ppm;

//------------------------------------------------------------------------------
// Save::_Save
//------------------------------------------------------------------------------

struct Save::_Save
{
  Radio_Button_Group * type;
  Radio_Button_Group * data;
};

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _plugin(0),
  _p(new _Save)
{
  // Create widgets.
  
  group_box::Box_Large * type_group =
    new group_box::Box_Large(label_type_group);

  _p->type =
    new Radio_Button_Group(djv_image::io_ppm::label_type());

  group_box::Box_Large * data_group =
    new group_box::Box_Large(label_data_group);

  _p->data =
    new Radio_Button_Group(djv_image::io_ppm::label_data());
  
  // Layout.

  Layout_V * layout = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());
  
  layout->add(type_group);
  type_group->layout()->add(_p->type);

  layout->add(data_group);
  data_group->layout()->add(_p->data);
  
  layout->add_stretch();
  
  // Initialize.
  
  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->type->signal.set(this, type_callback);
  _p->data->signal.set(this, data_callback);
}

Save::~Save()
{}

plugin::Plugin * Save::copy() const
{
  return new Save;
}

String Save::name() const
{
  return djv_image::io_ppm::name;
}

void Save::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Save::type_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.type = static_cast<TYPE>(in);
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Save::data_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.data = static_cast<DATA>(in);
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Save::plugin_update()
{
  callbacks(false);
  
  if (_plugin)
  {
    String tmp;
    tmp << _option.type;
    _plugin->option(_plugin->option_list()[SAVE_TYPE], &tmp);
    tmp << _option.data;
    _plugin->option(_plugin->option_list()[SAVE_DATA], &tmp);
  }
  
  callbacks(true);
}

void Save::widget_update()
{
  callbacks(false);

  try
  {
    if (_plugin)
    {
      String tmp;
      tmp = _plugin->option(_plugin->option_list()[SAVE_TYPE]);
      tmp >> _option.type;
      tmp = _plugin->option(_plugin->option_list()[SAVE_DATA]);
      tmp >> _option.data;
    }
  }
  catch (String) {}

  _p->type->set(_option.type);
  _p->data->set(_option.data);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_type_group = "File Type",
  label_data_group = "File Data";

//------------------------------------------------------------------------------

}
}

