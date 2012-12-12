/*------------------------------------------------------------------------------
 plugin/djv_openexr/ui_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "ui_load.h"

#include <djv_ui/group_box.h>
#include <djv_ui/float_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_plugin {
namespace openexr {

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
  Radio_Button_Group * color_profile;
  Float_Edit_Slider * gamma;
  Float_Edit_Slider * exposure;
  Float_Edit_Slider * exposure_defog;
  Float_Edit_Slider * exposure_knee_low;
  Float_Edit_Slider * exposure_knee_high;
  Radio_Button_Group * channels;
};

//------------------------------------------------------------------------------
// Load_Widget
//------------------------------------------------------------------------------

Load_Widget::Load_Widget() :
  _plugin(0),
  _p(new _Load_Widget)
{
  DJV_DEBUG("Load_Widget::Load_Widget");

  // Create color profile widgets.
  
  const Load_Option default_value;
  
  group_box::Box_Large * color_profile_group =
    new group_box::Box_Large(label_color_profile_group);
  
  _p->color_profile =
    new Radio_Button_Group(label_color_profile());
  
  group_box::Box * gamma_group =
    new group_box::Box(label_gamma_group);
  
  _p->gamma =
    new Float_Edit_Slider(0.1, 4.0, label_gamma);
  
  group_box::Box * exposure_group =
    new group_box::Box(label_exposure_group);
  
  _p->exposure =
    new Float_Edit_Slider(-10.0, 10.0, label_exposure);
  _p->exposure_defog =
    new Float_Edit_Slider(0.0, 0.01, label_exposure_defog);
  _p->exposure_knee_low =
    new Float_Edit_Slider(-3.0, 3.0, label_exposure_knee_low);
  _p->exposure_knee_high =
    new Float_Edit_Slider(3.5, 7.5, label_exposure_knee_high);
  
  // Create group channel widgets.
  
  group_box::Box_Large * channels_group =
    new group_box::Box_Large(label_channels_group);
  
  _p->channels =
    new Radio_Button_Group(label_channels_text());

  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    _p->exposure <<
    _p->exposure_defog <<
    _p->exposure_knee_low <<
    _p->exposure_knee_high);
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(color_profile_group);
  color_profile_group->layout()->add(_p->color_profile);
  color_profile_group->layout()->add(gamma_group);
  layout_v = new Layout_V(gamma_group->layout());
  layout_v->margin(0);
  layout_v->add(_p->gamma);
  color_profile_group->layout()->add(exposure_group);
  layout_v = new Layout_V(exposure_group->layout());
  layout_v->margin(0);
  layout_v->add(_p->exposure);
  layout_v->add(_p->exposure_defog);
  layout_v->add(_p->exposure_knee_low);
  layout_v->add(_p->exposure_knee_high);

  layout->add(channels_group);
  layout_v = new Layout_V(channels_group->layout());
  layout_v->margin(0);
  layout_v->add(_p->channels);
  
  layout->add_stretch();
  
  // Initialize.

  _p->exposure->inc(0.1, 1.0);
  _p->exposure_defog->inc(0.0001, 0.001);
  _p->exposure_knee_low->inc(0.1, 1.0);
  _p->exposure_knee_high->inc(0.1, 1.0);

  _p->gamma->default_value(default_value.gamma);
  _p->exposure->default_value(default_value.exposure.value);
  _p->exposure_defog->default_value(default_value.exposure.defog);
  _p->exposure_knee_low->default_value(default_value.exposure.knee_low);
  _p->exposure_knee_high->default_value(default_value.exposure.knee_high);

  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->color_profile->signal.set(this, color_profile_callback);
  _p->gamma->signal.set(this, gamma_callback);
  _p->exposure->signal.set(this, exposure_callback);
  _p->exposure_defog->signal.set(this, exposure_defog_callback);
  _p->exposure_knee_low->signal.set(this, exposure_knee_low_callback);
  _p->exposure_knee_high->signal.set(this, exposure_knee_high_callback);
  _p->channels->signal.set(this, channels_callback);
}

Load_Widget::~Load_Widget()
{}

plugin::Plugin * Load_Widget::copy() const
{
  return new Load_Widget;
}

String Load_Widget::name() const
{
  return openexr::name;
}

void Load_Widget::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Load_Widget::color_profile_callback(int in)
{
  _option.color_profile = static_cast<COLOR_PROFILE>(in);
  
  callback(true);
}

void Load_Widget::gamma_callback(double in)
{
  _option.gamma = in;
  
  callback(true);
}

void Load_Widget::exposure_callback(double in)
{
  _option.exposure.value = in;
  
  callback(true);
}

void Load_Widget::exposure_defog_callback(double in)
{
  _option.exposure.defog = in;
  
  callback(true);
}

void Load_Widget::exposure_knee_low_callback(double in)
{
  _option.exposure.knee_low = in;
  
  callback(true);
}

void Load_Widget::exposure_knee_high_callback(double in)
{
  _option.exposure.knee_high = in;
  
  callback(true);
}

void Load_Widget::channels_callback(int in)
{
  _option.channels = static_cast<CHANNELS>(in);
  
  callback(true);
}

void Load_Widget::callback(bool)
{
  if (! _plugin)
    return;
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Load_Widget::plugin_update()
{
  callbacks(false);

  if (_plugin)
  {
    const List<String> & list = _plugin->option_list();
    
    String tmp;
    tmp << _option.color_profile;
    _plugin->option(list[LOAD_COLOR_PROFILE], &tmp);
    tmp << _option.gamma;
    _plugin->option(list[LOAD_GAMMA], &tmp);
    tmp << _option.exposure;
    _plugin->option(list[LOAD_EXPOSURE], &tmp);
    tmp << _option.channels;
    _plugin->option(list[LOAD_CHANNELS], &tmp);
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
      const List<String> & list = _plugin->option_list();
      
      String tmp;
      tmp = _plugin->option(list[LOAD_COLOR_PROFILE]);
      tmp >> _option.color_profile;
      tmp = _plugin->option(list[LOAD_GAMMA]);
      tmp >> _option.gamma;
      tmp = _plugin->option(list[LOAD_EXPOSURE]);
      tmp >> _option.exposure;
      tmp = _plugin->option(list[LOAD_CHANNELS]);
      tmp >> _option.channels;
    }
  }
  catch (String)
  {}
  
  _p->color_profile->set(_option.color_profile);
  _p->gamma->set(_option.gamma);
  _p->exposure->set(_option.exposure.value);
  _p->exposure_defog->set(_option.exposure.defog);
  _p->exposure_knee_low->set(_option.exposure.knee_low);
  _p->exposure_knee_high->set(_option.exposure.knee_high);
  _p->channels->set(_option.channels);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

