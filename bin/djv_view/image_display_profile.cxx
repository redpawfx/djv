/*------------------------------------------------------------------------------
 bin/djv_view/image_display_profile.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "image_display_profile.h"

#include <djv_ui/color.h>
#include <djv_ui/file.h>
#include <djv_ui/float_value.h>
#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/push_button.h>
#include <djv_image/image.h>

namespace djv_bin {
namespace view {
namespace image_display_profile {

//------------------------------------------------------------------------------
// Dialog::_Dialog
//------------------------------------------------------------------------------

struct Dialog::_Dialog
{
  djv_ui::File_Edit * lut;
  djv_ui::Float_Edit_Slider * brightness;
  djv_ui::Float_Edit_Slider * contrast;
  djv_ui::Float_Edit_Slider * saturation;
  djv_ui::Float_Edit_Slider * levels_in [2];
  djv_ui::Float_Edit_Slider * gamma;
  djv_ui::Float_Edit_Slider * levels_out [2];
  djv_ui::Float_Edit_Slider * soft_clip;
  djv_ui::Push_Button * close;
};

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

Dialog::Dialog() :
  Base(dialog_title),
  signal(this),
  _obj(0),
  _p(new _Dialog)
{
  using namespace djv_ui;
  
  // Create LUT widgets.
  
  group_box::Box * lut_group =
    new group_box::Box(dialog_lut);
  
  _p->lut =
    new File_Edit;
  
  // Create color widgets.
  
  group_box::Box * color_group =
    new group_box::Box(dialog_color);
  
  _p->brightness =
    new Float_Edit_Slider;
  _p->brightness->default_value(_value.color.brightness);
  _p->brightness->range(0.0, 4.0);
  _p->brightness->label(new Label(dialog_brightness));
  
  _p->contrast =
    new Float_Edit_Slider;
  _p->contrast->default_value(_value.color.contrast);
  _p->contrast->range(0.0, 4.0);
  _p->contrast->label(new Label(dialog_contrast));
  
  _p->saturation =
    new Float_Edit_Slider;
  _p->saturation->default_value(_value.color.saturation);
  _p->saturation->range(0.0, 4.0);
  _p->saturation->label(new Label(dialog_saturation));
  
  // Create levels widgets.
  
  group_box::Box * levels_group =
    new group_box::Box(dialog_levels);
  
  _p->levels_in[0] =
    new Float_Edit_Slider;
  _p->levels_in[0]->default_value(_value.levels.in_low);
  _p->levels_in[0]->range(0.0, 1.0);
  _p->levels_in[0]->label(new Label(dialog_levels_in));
  
  _p->levels_in[1] =
    new Float_Edit_Slider;
  _p->levels_in[1]->default_value(_value.levels.in_high);
  _p->levels_in[1]->range(0.0, 1.0);
  _p->levels_in[1]->label(new Label);
  
  _p->gamma =
    new Float_Edit_Slider;
  _p->gamma->default_value(_value.levels.gamma);
  _p->gamma->range(0.0, 4.0);
  _p->gamma->label(new Label(dialog_gamma));
  
  _p->levels_out[0] =
    new Float_Edit_Slider;
  _p->levels_out[0]->default_value(_value.levels.out_low);
  _p->levels_out[0]->range(0.0, 1.0);
  _p->levels_out[0]->label(new Label(dialog_levels_out));
  
  _p->levels_out[1] =
    new Float_Edit_Slider;
  _p->levels_out[1]->default_value(_value.levels.out_high);
  _p->levels_out[1]->range(0.0, 1.0);
  _p->levels_out[1]->label(new Label);
  
  _p->soft_clip =
    new Float_Edit_Slider;
  _p->soft_clip->default_value(_value.soft_clip);
  _p->soft_clip->range(0.0, 1.0);
  _p->soft_clip->label(new Label(dialog_soft_clip));

  // Create dialog widgets.
  
  _p->close =
    new Push_Button(djv_ui::dialog::label_close);
  
  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    _p->brightness <<
    _p->contrast <<
    _p->saturation <<
    _p->levels_in[0] <<
    _p->levels_in[1] <<
    _p->gamma <<
    _p->levels_out[0] <<
    _p->levels_out[1] <<
    _p->soft_clip);
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  Layout_H * layout_h = 0;
  
  layout = new Layout_V(this);
  
  layout->add(lut_group);
  lut_group->layout()->add(_p->lut);
  
  layout->add(color_group);
  layout_v = new Layout_V(color_group->layout());
  layout_v->margin(0);
  layout_v->add(_p->brightness);
  layout_v->add(_p->contrast);
  layout_v->add(_p->saturation);
  
  layout->add(levels_group);
  layout_v = new Layout_V(levels_group->layout());
  layout_v->margin(0);
  layout_v->add(_p->levels_in[0]);
  layout_v->add(_p->levels_in[1]);
  layout_v->add(_p->gamma);
  layout_v->add(_p->levels_out[0]);
  layout_v->add(_p->levels_out[1]);
  layout_v->add(_p->soft_clip);
  
  layout->add_spacer(-1, true);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  widget_update();
  
  size(size_hint());
  
  // Callbacks.
  
  _p->lut->signal.set(this, lut_callback);
  
  _p->brightness->signal.set(this, brightness_callback);
  _p->contrast->signal.set(this, contrast_callback);
  _p->saturation->signal.set(this, saturation_callback);

  _p->levels_in[0]->signal.set(this, levels_in0_callback);
  _p->levels_in[1]->signal.set(this, levels_in1_callback);
  _p->gamma->signal.set(this, gamma_callback);
  _p->levels_out[0]->signal.set(this, levels_out0_callback);
  _p->levels_out[1]->signal.set(this, levels_out1_callback);
  _p->soft_clip->signal.set(this, soft_clip_callback);
  
  _p->close->signal.set(this, close_callback);
}

Dialog::~Dialog()
{}

void Dialog::set(const void * in, const image::Display_Profile & value)
{
  _obj = in;
  _value = value;
  
  widget_update();
}

void Dialog::pick(const void * in, const image::Display_Profile & value)
{
  if (in == _obj || ! shown() || ! visible())
    return;
  
  set(in, value);
}

void Dialog::update(const void * in, const image::Display_Profile & value)
{
  if (in != _obj)
    return;
  
  _value = value;
  
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

void Dialog::widget_callback(const image::Display_Profile & in)
{
  _value = in;
  
  widget_update();
}

void Dialog::lut_callback(const File & in)
{
  _value.lut_file = in;
  const djv_image::Image * image = image::lut_load(_value.lut_file);
  _value.lut = image ? *image : djv_image::Image();
  
  signal.emit(_value);
}

void Dialog::brightness_callback(double in)
{
  _value.color.brightness = in;
  
  signal.emit(_value);
}

void Dialog::contrast_callback(double in)
{
  _value.color.contrast = in;
  
  signal.emit(_value);
}

void Dialog::saturation_callback(double in)
{
  _value.color.saturation = in;
  
  signal.emit(_value);
}

void Dialog::levels_in0_callback(double in)
{
  _value.levels.in_low = in;
  
  signal.emit(_value);
}

void Dialog::levels_in1_callback(double in)
{
  _value.levels.in_high = in;
  
  signal.emit(_value);
}

void Dialog::gamma_callback(double in)
{
  _value.levels.gamma = in;
  
  signal.emit(_value);
}

void Dialog::levels_out0_callback(double in)
{
  _value.levels.out_low = in;
  
  signal.emit(_value);
}

void Dialog::levels_out1_callback(double in)
{
  _value.levels.out_high = in;
  
  signal.emit(_value);
}

void Dialog::soft_clip_callback(double in)
{
  _value.soft_clip = in;
  
  signal.emit(_value);
}

void Dialog::close_callback(bool)
{
  hide();
}

void Dialog::widget_update()
{
  callbacks(false);
  
  _p->lut->set(_value.lut_file);
  
  _p->brightness->set(_value.color.brightness);
  _p->contrast->set(_value.color.contrast);
  _p->saturation->set(_value.color.saturation);

  _p->levels_in[0]->set(_value.levels.in_low);
  _p->levels_in[1]->set(_value.levels.in_high);
  _p->gamma->set(_value.levels.gamma);
  _p->levels_out[0]->set(_value.levels.out_low);
  _p->levels_out[1]->set(_value.levels.out_high);
  _p->soft_clip->set(_value.soft_clip);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  dialog_title = "Display Profile",
  dialog_lut = "Lookup Table",
  dialog_color = "Color",
  dialog_brightness = "Brightness:",
  dialog_contrast = "Contrast:",
  dialog_saturation = "Saturation:",
  dialog_levels = "Levels",
  dialog_levels_in = "Input:",
  dialog_gamma = "Gamma:",
  dialog_levels_out = "Output:",
  dialog_soft_clip = "Soft clip:"; 

//------------------------------------------------------------------------------

}
}
}

