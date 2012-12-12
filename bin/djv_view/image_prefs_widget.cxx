/*------------------------------------------------------------------------------
 bin/djv_view/image_prefs_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "image_prefs.h"

#include <djv_ui/check_button.h>
#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_bin {
namespace view {
namespace image {

//------------------------------------------------------------------------------
// Prefs_Widget::_Prefs_Widget
//------------------------------------------------------------------------------

struct Prefs_Widget::_Prefs_Widget
{  
  djv_ui::Radio_Button_Group * display_profile;
};

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget() :
  _p(new _Prefs_Widget)
{
  using namespace djv_ui;
  
  // Create widgets.

  group_box::Box_Large * general_group =
    new group_box::Box_Large(prefs_group);

  Check_Button * frame_store_file_reload =
    new Check_Button(prefs_frame_store_file_reload);
  
  // Create transform widgets.
  
  group_box::Box_Large * transform_group =
    new group_box::Box_Large(prefs_xform_group);

  group_box::Box_Small * mirror_group =
    new group_box::Box_Small(prefs_xform_mirror);
  Check_Button * mirror_h =
    new Check_Button(prefs_xform_mirror_h);
  Check_Button * mirror_v =
    new Check_Button(prefs_xform_mirror_v);

  Radio_Button_Group * scale =
    new Radio_Button_Group(prefs_xform_scale, label_scale());

  Radio_Button_Group * rotate =
    new Radio_Button_Group(prefs_xform_rotate, label_rotate());

  // Create color profile widgets.

  group_box::Box_Large * color_profile_group =
    new group_box::Box_Large(prefs_color_profile_group);

  Check_Button * color_profile =
    new Check_Button(prefs_color_profile);
  
  // Create display profile widgets.

  group_box::Box_Large * display_profile_group =
    new group_box::Box_Large(prefs_display_profile_group);

  _p->display_profile =
    new Radio_Button_Group(List<String>());

  // Create image channel widgets.

  group_box::Box_Large * channel_group =
    new group_box::Box_Large(prefs_channel_group);
  
  Radio_Button_Group * channel =
    new Radio_Button_Group(label_channel());

  // Layout.
  
  Layout_V * layout = 0, * layout_v = 0;
  Layout_H * layout_h = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(general_group);
  general_group->layout()->add(frame_store_file_reload);

  layout->add(transform_group);
  layout_h = new Layout_H(transform_group->layout());
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(mirror_group);
  layout_v = new Layout_V(mirror_group->layout());
  layout_v->margin(0);
  layout_v->add(mirror_h);
  layout_v->add(mirror_v);
  layout_h->add(scale);
  layout_h->add(rotate);
  layout_h->add_stretch();
  
  layout->add(color_profile_group);
  color_profile_group->layout()->add(color_profile);
  
  layout->add(display_profile_group);
  display_profile_group->layout()->add(_p->display_profile);
  
  layout->add(channel_group);
  channel_group->layout()->add(channel);
  
  layout->add_stretch();
 
  // Initialize.
  
  frame_store_file_reload->set(prefs()->frame_store_file_reload());
  mirror_h->set(prefs()->mirror().x);
  mirror_v->set(prefs()->mirror().y);
  scale->set(prefs()->scale());
  rotate->set(prefs()->rotate());
  color_profile->set(prefs()->color_profile());
  channel->set(prefs()->channel());
  
  widget_update();
  
  // Callbacks.

  frame_store_file_reload->signal.set(this, frame_store_file_reload_callback);
  mirror_h->signal.set(this, mirror_h_callback);
  mirror_v->signal.set(this, mirror_v_callback);
  scale->signal.set(this, scale_callback);
  rotate->signal.set(this, rotate_callback);
  color_profile->signal.set(this, color_profile_callback);
  _p->display_profile->signal.set(this, display_profile_callback);
  prefs()->display_profile_list_signal.set(this, display_profile_list_callback);
  channel->signal.set(this, channel_callback);
}

Prefs_Widget::~Prefs_Widget()
{}

void Prefs_Widget::frame_store_file_reload_callback(bool in)
{
  prefs()->frame_store_file_reload(in);
}

void Prefs_Widget::mirror_h_callback(bool in)
{
  V2b tmp(prefs()->mirror());
  tmp.x = in;
  prefs()->mirror(tmp);
}

void Prefs_Widget::mirror_v_callback(bool in)
{
  V2b tmp(prefs()->mirror());
  tmp.y = in;
  prefs()->mirror(tmp);
}

void Prefs_Widget::scale_callback(int in)
{
  prefs()->scale(static_cast<image::SCALE>(in));
}

void Prefs_Widget::rotate_callback(int in)
{
  prefs()->rotate(static_cast<image::ROTATE>(in));
}

void Prefs_Widget::color_profile_callback(bool in)
{
  prefs()->color_profile(in);
}

void Prefs_Widget::display_profile_callback(int in)
{
  prefs()->display_profile(in - 1);
}

void Prefs_Widget::display_profile_list_callback(bool)
{
  widget_update();
}

void Prefs_Widget::channel_callback(int in)
{
  prefs()->channel(static_cast<image::CHANNEL>(in));
}

void Prefs_Widget::widget_update()
{
  callbacks(false);
  
  _p->display_profile->label(List<String>() <<
    prefs_display_profile_none <<
    prefs()->display_profile_name_list());

  _p->display_profile->set(prefs()->display_profile() + 1);

  callbacks(true);
}

//------------------------------------------------------------------------------

}
}
}

