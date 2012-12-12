/*------------------------------------------------------------------------------
 bin/djv_view/tool_info.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "tool_info.h"

#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/prefs.h>
#include <djv_ui/push_button.h>
#include <djv_ui/text.h>

namespace djv_bin {
namespace view {
namespace tool_info {

//------------------------------------------------------------------------------
// Dialog::_Dialog
//------------------------------------------------------------------------------

struct Dialog::_Dialog
{
  djv_ui::Text_Display * file_name;
  djv_ui::Text_Display * layer_name;
  djv_ui::Text_Display * size;
  djv_ui::Text_Display * pixel;
  djv_ui::Text_Display * time;
  djv_ui::Text_Multiline_Display * tag;
  djv_ui::Push_Button * close;
};

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

Dialog::Dialog() :
  Base(dialog_title),
  _obj(0),
  _p(new _Dialog)
{
  using namespace djv_ui;
  
  // Create widgets.

  _p->file_name =
    new Text_Display;
  Label * file_name_label =
    new Label(dialog_file_name);

  _p->layer_name =
    new Text_Display;
  Label * layer_name_label =
    new Label(dialog_layer_name);

  _p->size =
    new Text_Display;
  Label * size_label =
    new Label(dialog_size);

  _p->pixel =
    new Text_Display;
  Label * pixel_label =
    new Label(dialog_pixel);

  _p->time =
    new Text_Display;
  Label * time_label =
    new Label(dialog_time);

  _p->tag =
    new Text_Multiline_Display;
  Label * tag_label =
    new Label(dialog_tag);

  _p->close =
    new Push_Button(djv_ui::dialog::label_close);
  
  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    file_name_label <<
    layer_name_label <<
    size_label <<
    pixel_label <<
    time_label);
  
  Layout_V * layout = new Layout_V(this);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(file_name_label);
  layout_h->add(_p->file_name);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(layer_name_label);
  layout_h->add(_p->layer_name);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(size_label);
  layout_h->add(_p->size);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(pixel_label);
  layout_h->add(_p->pixel);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(time_label);
  layout_h->add(_p->time);
  
  layout->add(tag_label);
  layout->add(_p->tag);
  layout->stretch(_p->tag);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.
  
  size(V2i(500, 300));
  
  // Callbacks.
 
  _p->close->signal.set(this, close_callback);
}

Dialog::~Dialog()
{}

void Dialog::set(const void * in, const djv_image::io::Info & info)
{
  _obj = in;
  _info = info;
  
  widget_update();
}

void Dialog::pick(const void * in, const djv_image::io::Info & info)
{
  if (in == _obj || ! shown() || ! visible())
    return;
  
  set(in, info);
}

void Dialog::update(const void * in, const djv_image::io::Info & info)
{
  if (in != _obj || ! shown() || ! visible())
    return;
  
  _info = info;
  
  widget_update();
}

void Dialog::del(const void * in)
{
  if (in != _obj)
    return;
  
  _obj = 0;
}

void Dialog::show()
{
  _p->close->take_focus();
  
  djv_ui::dialog::Base::show();
}

void Dialog::close_callback(bool)
{
  hide();
}

void Dialog::widget_update()
{
  callbacks(false);
  
  _p->file_name->set(_info.file_name);

  _p->layer_name->set(_info.layer_name);

  _p->size->set(string::Format("%%x%%:%%").
    arg(_info.size.x).
    arg(_info.size.y).
    arg(vector::aspect(_info.size)));
  
  _p->pixel->set(string::Format("%%").
    arg(string::label(_info.pixel)));
  
  _p->time->set(string::Format("%%@%%").
    arg(djv_image::time::units_to_string(
      static_cast<int>(_info.time.list.size()), _info.time.speed)).
    arg(djv_image::time::speed_to_float(_info.time.speed), 0, -2));

  String tmp;
  const List<String> key_list = _info.tag.key_list();
  for (list::size_type i = 0; i < key_list.size(); ++i)
    tmp += string::Format("%% = %%\n").
      arg(key_list[i]).arg(_info.tag[key_list[i]]);
  _p->tag->set(tmp);

  callbacks(true);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  dialog_title = "Information",
  dialog_file_name = "File Name:",
  dialog_layer_name = "Layer:",
  dialog_size = "Size:",
  dialog_pixel = "Pixel:",
  dialog_time = "Time:",
  dialog_tag = "Tags:";

//------------------------------------------------------------------------------

}
}
}

