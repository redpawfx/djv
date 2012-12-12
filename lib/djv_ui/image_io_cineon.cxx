/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_cineon.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_io_cineon.h"

#include <djv_ui/group_box.h>
#include <djv_ui/float_value.h>
#include <djv_ui/int_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace image_io_cineon {
using namespace djv_image::io_cineon;

//------------------------------------------------------------------------------
// Load::_Load
//------------------------------------------------------------------------------

struct Load::_Load
{
  Radio_Button_Group * color_profile;
  Int_Edit_Slider * black;
  Int_Edit_Slider * white;
  Float_Edit_Slider * gamma;
  Int_Edit_Slider * soft_clip;
  
  Radio_Button_Group * convert;
};

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _plugin(0),
  _p(new _Load)
{
  DJV_DEBUG("Load::Load");
  
  // Create color profile widgets.
  
  group_box::Box_Large * color_profile_group =
    new group_box::Box_Large(label_color_profile_group);
  
  _p->color_profile =
    new Radio_Button_Group(label_color_profile());
  
  group_box::Box * film_print_group =
    new group_box::Box(label_film_print_group);
  
  _p->black =
    new Int_Edit_Slider(0, 1023, label_film_print_black);
  _p->white =
    new Int_Edit_Slider(0, 1023, label_film_print_white);
  _p->gamma =
    new Float_Edit_Slider(0.01, 4.0, label_film_print_gamma);
  _p->soft_clip =
    new Int_Edit_Slider(0, 50, label_film_print_soft_clip);
  
  // Create convert widgets.
  
  group_box::Box_Large * convert_group =
    new group_box::Box_Large(label_convert_group);
  
  _p->convert =
    new Radio_Button_Group(label_convert());
  
  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    _p->black <<
    _p->white <<
    _p->gamma <<
    _p->soft_clip);

  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(color_profile_group);
  color_profile_group->layout()->add(_p->color_profile);

  color_profile_group->layout()->add(film_print_group);
  layout_v = new Layout_V(film_print_group->layout());
  layout_v->margin(0);
  layout_v->add(_p->black);
  layout_v->add(_p->white);
  layout_v->add(_p->gamma);
  layout_v->add(_p->soft_clip);

  layout->add(convert_group);
  convert_group->layout()->add(_p->convert);
  
  layout->add_stretch();
  
  // Initialize.
  
  _p->black->default_value(Load_Option().film_print.black);
  _p->white->default_value(Load_Option().film_print.white);
  _p->gamma->default_value(Load_Option().film_print.gamma);
  _p->soft_clip->default_value(Load_Option().film_print.soft_clip);

  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->color_profile->signal.set(this, color_profile_callback);
  
  _p->black->signal.set(this, black_callback);
  _p->white->signal.set(this, white_callback);
  _p->gamma->signal.set(this, gamma_callback);
  _p->soft_clip->signal.set(this, soft_clip_callback);

  _p->convert->signal.set(this, convert_callback);
}

Load::~Load()
{
  DJV_DEBUG("Load::~Load");
}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return djv_image::io_cineon::name;
}

void Load::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Load::color_profile_callback(int in)
{
  _option.color_profile = static_cast<COLOR_PROFILE>(in);

  callback(true);
}

void Load::black_callback(int in)
{
  _option.film_print.black = in;

  callback(true);
}

void Load::white_callback(int in)
{
  _option.film_print.white = in;

  callback(true);
}

void Load::gamma_callback(double in)
{
  _option.film_print.gamma = in;

  callback(true);
}

void Load::soft_clip_callback(int in)
{
  _option.film_print.soft_clip = in;

  callback(true);
}

void Load::convert_callback(int in)
{
  _option.convert = static_cast<CONVERT>(in);

  callback(true);
}

void Load::callback(bool)
{
  if (! _plugin)
    return;
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Load::plugin_update()
{
  callbacks(false);

  if (_plugin)
  {
    String tmp;
    tmp << _option.color_profile;
    _plugin->option(_plugin->option_list()[LOAD_COLOR_PROFILE], &tmp);
    tmp << _option.film_print;
    _plugin->option(_plugin->option_list()[LOAD_FILM_PRINT], &tmp);
    tmp << _option.convert;
    _plugin->option(_plugin->option_list()[LOAD_CONVERT], &tmp);
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
      tmp = _plugin->option(_plugin->option_list()[LOAD_COLOR_PROFILE]);
      tmp >> _option.color_profile;
      tmp = _plugin->option(_plugin->option_list()[LOAD_FILM_PRINT]);
      tmp >> _option.film_print;
      tmp = _plugin->option(_plugin->option_list()[LOAD_CONVERT]);
      tmp >> _option.convert;
    }
  }
  catch (String) {}

  _p->color_profile->set(_option.color_profile);

  _p->black->set(_option.film_print.black);
  _p->white->set(_option.film_print.white);
  _p->gamma->set(_option.film_print.gamma);
  _p->soft_clip->set(_option.film_print.soft_clip);

  _p->convert->set(_option.convert);

  callbacks(true);
}

//------------------------------------------------------------------------------
// Save::_Save
//------------------------------------------------------------------------------

struct Save::_Save
{
  Radio_Button_Group * color_profile;
  Int_Edit_Slider * black;
  Int_Edit_Slider * white;
  Float_Edit_Slider * gamma;
};

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _plugin(0),
  _p(new _Save)
{
  // Create color profile widgets.
  
  group_box::Box_Large * color_profile_group =
    new group_box::Box_Large(label_color_profile_group);

  _p->color_profile =
    new Radio_Button_Group(label_color_profile());
  
  group_box::Box * film_print_group =
    new group_box::Box(label_film_print_group);

  _p->black =
    new Int_Edit_Slider(0, 1023, label_film_print_black);
  _p->white =
    new Int_Edit_Slider(0, 1023, label_film_print_white);
  _p->gamma =
    new Float_Edit_Slider(0.01, 4.0, label_film_print_gamma);
  
  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    _p->black <<
    _p->white <<
    _p->gamma);
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(color_profile_group);
  color_profile_group->layout()->add(_p->color_profile);
  
  color_profile_group->layout()->add(film_print_group);
  layout_v = new Layout_V(film_print_group->layout());
  layout_v->margin(0);
  layout_v->add(_p->black);
  layout_v->add(_p->white);
  layout_v->add(_p->gamma);

  layout->add_stretch();
  
  // Initialize.
  
  _p->black->default_value(Save_Option().film_print.black);
  _p->white->default_value(Save_Option().film_print.white);
  _p->gamma->default_value(Save_Option().film_print.gamma);

  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->color_profile->signal.set(this, color_profile_callback);
  _p->black->signal.set(this, black_callback);
  _p->white->signal.set(this, white_callback);
  _p->gamma->signal.set(this, gamma_callback);
}

Save::~Save()
{}

plugin::Plugin * Save::copy() const
{
  return new Save;
}

String Save::name() const
{
  return djv_image::io_cineon::name;
}

void Save::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Save::color_profile_callback(int in)
{
  _option.color_profile = static_cast<COLOR_PROFILE>(in);

  callback(true);
}

void Save::black_callback(int in)
{
  _option.film_print.black = in;

  callback(true);
}

void Save::white_callback(int in)
{
  _option.film_print.white = in;

  callback(true);
}

void Save::gamma_callback(double in)
{
  _option.film_print.gamma = in;

  callback(true);
}

void Save::callback(bool)
{
  if (! _plugin)
    return;
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Save::plugin_update()
{
  callbacks(false);

  if (_plugin)
  {
    String tmp;
    tmp << _option.color_profile;
    _plugin->option(_plugin->option_list()[SAVE_COLOR_PROFILE], &tmp);
    tmp << _option.film_print;
    _plugin->option(_plugin->option_list()[SAVE_FILM_PRINT], &tmp);
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
      tmp = _plugin->option(_plugin->option_list()[SAVE_COLOR_PROFILE]);
      tmp >> _option.color_profile;
      tmp = _plugin->option(_plugin->option_list()[SAVE_FILM_PRINT]);
      tmp >> _option.film_print;
    }
  }
  catch (String) {}

  _p->color_profile->set(_option.color_profile);
  _p->black->set(_option.film_print.black);
  _p->white->set(_option.film_print.white);
  _p->gamma->set(_option.film_print.gamma);

  callbacks(true);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_color_profile_group = "Color Profile",
  label_film_print_group = "Film Print",
  label_film_print_black = "Black:",
  label_film_print_white = "White:",
  label_film_print_gamma = "Gamma:",
  label_film_print_soft_clip = "Soft clip:",
  label_convert_group = "Convert";

//------------------------------------------------------------------------------

}
}

