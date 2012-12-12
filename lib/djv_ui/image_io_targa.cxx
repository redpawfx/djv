/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_targa.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_io_targa.h"

#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace image_io_targa {
using namespace djv_image::io_targa;

//------------------------------------------------------------------------------
// Save::_Save
//------------------------------------------------------------------------------

struct Save::_Save
{
  Radio_Button_Group * compression;
};

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _plugin(0),
  _p(new _Save)
{
  DJV_DEBUG("Save::Save");
  
  // Create widgets.
  
  group_box::Box_Large * group =
    new group_box::Box_Large(label_compression_group);
  
  _p->compression =
    new Radio_Button_Group(label_compression_text());
  
  // Layout.

  Layout_V * layout = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(group);
  group->layout()->add(_p->compression);
  
  layout->add_stretch();
  
  // Initialize.

  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->compression->signal.set(this, compression_callback);
}

Save::~Save()
{}

plugin::Plugin * Save::copy() const
{
  return new Save;
}

String Save::name() const
{
  return djv_image::io_targa::name;
}

void Save::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Save::compression_callback(int in)
{
  if (! _plugin)
    return;
  
  _option.compression = static_cast<COMPRESSION>(in);
  
  plugin_update();

  signal.emit(_plugin);
}

void Save::plugin_update()
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

void Save::widget_update()
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
  catch (String) {}

  _p->compression->set(_option.compression);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_compression_group = "Compression";

//------------------------------------------------------------------------------

}
}

