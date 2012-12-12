/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/ui_save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "ui_save.h"

#include <djv_ui/group_box.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_plugin {
namespace libquicktime {

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
  Radio_Button_Group * codec;
};

//------------------------------------------------------------------------------
// Save_Widget
//------------------------------------------------------------------------------

Save_Widget::Save_Widget() :
  _plugin(0),
  _p(new _Save_Widget)
{
  // Create widgets.
  
  group_box::Box_Large * codec_group =
    new group_box::Box_Large(label_codec_group);

  _p->codec =
    new Radio_Button_Group(libquicktime::label_codec_text());

  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    _p->codec);
  
  Layout_V * layout = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(codec_group);
  codec_group->layout()->add(_p->codec);
  
  layout->add_stretch();
  
  // Initialize.
  
  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->codec->signal.set(this, codec_callback);
}

Save_Widget::~Save_Widget()
{}

plugin::Plugin * Save_Widget::copy() const
{
  return new Save_Widget;
}

String Save_Widget::name() const
{
  return libquicktime::name;
}

void Save_Widget::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Save_Widget::codec_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.codec = libquicktime::label_codec()[in];
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Save_Widget::plugin_update()
{
  callbacks(false);

  if (_plugin)
  {
    String tmp;
    tmp << _option.codec;
    _plugin->option(_plugin->option_list()[SAVE_CODEC], &tmp);
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
      tmp = _plugin->option(_plugin->option_list()[SAVE_CODEC]);
      tmp >> _option.codec;
    }
  }
  catch (String)
  {}

  _p->codec->set(list::find(_option.codec, libquicktime::label_codec()));
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

