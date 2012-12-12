/*------------------------------------------------------------------------------
 plugin/djv_tiff/ui_save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "ui_save.h"

#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_plugin {
namespace tiff {

//------------------------------------------------------------------------------
// djv_ui_image_save()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_ui_image_save()
{
  return new Save_Widget;
}

}

//------------------------------------------------------------------------------
// Save_Widget::_Save_Widget
//------------------------------------------------------------------------------

struct Save_Widget::_Save_Widget
{
  Radio_Button_Group * compression;
  Radio_Button_Group * channels;
};

//------------------------------------------------------------------------------
// Save_Widget
//------------------------------------------------------------------------------

Save_Widget::Save_Widget() :
  _plugin(0),
  _p(new _Save_Widget)
{
  DJV_DEBUG("Save_Widget::Save_Widget");

  // Create widgets.
  
  group_box::Box_Large * compression_group =
    new group_box::Box_Large(label_compression_group);
  
  _p->compression =
    new Radio_Button_Group(tiff::label_compression_text());
  
  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    _p->compression);
  
  Layout_V * layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(compression_group);
  compression_group->layout()->add(_p->compression);

  layout->add_stretch();
  
  // Initialize.
  
  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->compression->signal.set(this, compression_callback);
}

Save_Widget::~Save_Widget()
{}
  
plugin::Plugin * Save_Widget::copy() const
{
  return new Save_Widget;
}

String Save_Widget::name() const
{
  return tiff::name;
}

void Save_Widget::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Save_Widget::compression_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.compression = static_cast<COMPRESSION>(in);
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Save_Widget::plugin_update()
{
  callbacks(false);

  if (_plugin)
  {
    String tmp;
    tmp << _option.compression;
    _plugin->option(_plugin->option_list()[SAVE_COMPRESSION], &tmp);
  }

  callbacks(true);
}

void Save_Widget::widget_update()
{
  callbacks(false);

  try
  {
    if (_plugin)
    {
      String tmp;
      tmp = _plugin->option(_plugin->option_list()[SAVE_COMPRESSION]);
      tmp >> _option.compression;
    }
  }
  catch (String)
  {}

  _p->compression->set(_option.compression);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

