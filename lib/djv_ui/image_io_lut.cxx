/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_lut.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_io_lut.h"

#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace image_io_lut {
using namespace djv_image::io_lut;

//------------------------------------------------------------------------------
// Load::_Load
//------------------------------------------------------------------------------

struct Load::_Load
{
  Radio_Button_Group * format;
  Radio_Button_Group * type;
};

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _plugin(0),
  _p(new _Load)
{
  // Create widgets.
  
  group_box::Box_Large * format_group =
    new group_box::Box_Large(label_format_group);

  _p->format =
    new Radio_Button_Group(djv_image::io_lut::label_format());

  group_box::Box_Large * type_group =
    new group_box::Box_Large(label_type_group);

  _p->type =
    new Radio_Button_Group(djv_image::io_lut::label_type());
  
  // Layout.
  
  Layout_V * layout = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(format_group);
  format_group->layout()->add(_p->format);

  layout->add(type_group);
  type_group->layout()->add(_p->type);
  
  layout->add_stretch();
  
  // Initialize.
  
  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->format->signal.set(this, format_callback);
  _p->type->signal.set(this, type_callback);
}

Load::~Load()
{}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return djv_image::io_lut::name;
}

void Load::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Load::format_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.format = static_cast<FORMAT>(in);
  
  plugin_update();

  signal.emit(_plugin);
}

void Load::type_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.type = static_cast<TYPE>(in);
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Load::plugin_update()
{
  callbacks(false);
  
  if (_plugin)
  {
    String tmp;
    tmp << _option.format;
    _plugin->option(_plugin->option_list()[LOAD_FORMAT], &tmp);
    tmp << _option.type;
    _plugin->option(_plugin->option_list()[LOAD_TYPE], &tmp);
  }
  
  callbacks(true);
}

void Load::widget_update()
{
  callbacks(false);

  try
  {  
    if (_plugin)
    {
      String tmp;
      tmp = _plugin->option(_plugin->option_list()[LOAD_FORMAT]);
      tmp >> _option.format;
      tmp = _plugin->option(_plugin->option_list()[LOAD_TYPE]);
      tmp >> _option.type;
    }
  }
  catch (String) {}

  _p->format->set(_option.format);
  _p->type->set(_option.type);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Save::_Save
//------------------------------------------------------------------------------

struct Save::_Save
{
  Radio_Button_Group * format;
};

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _plugin(0),
  _p(new _Save)
{
  // Widgets.
  
  group_box::Box_Large * format_group =
    new group_box::Box_Large(label_format_group);

  _p->format =
    new Radio_Button_Group(djv_image::io_lut::label_format());
  
  // Layout.

  Layout_V * layout = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(format_group);
  format_group->layout()->add(_p->format);

  layout->add_stretch();
  
  // Initialize.
  
  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->format->signal.set(this, format_callback);
}

Save::~Save()
{}

plugin::Plugin * Save::copy() const
{
  return new Save;
}

String Save::name() const
{
  return djv_image::io_lut::name;
}

void Save::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Save::format_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.format = static_cast<FORMAT>(in);
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Save::plugin_update()
{
  callbacks(false);
  
  if (_plugin)
  {
    String tmp;
    tmp << _option.format;
    _plugin->option(_plugin->option_list()[SAVE_FORMAT], &tmp);
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
      tmp = _plugin->option(_plugin->option_list()[SAVE_FORMAT]);
      tmp >> _option.format;
    }
  }
  catch (String) {}

  _p->format->set(_option.format);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_format_group = "File Format",
  label_type_group = "File Type";

//------------------------------------------------------------------------------

}
}

