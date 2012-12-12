/*------------------------------------------------------------------------------
 lib/djv_ui/color_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "color.h"

#include <djv_ui/choice.h>
#include <djv_ui/float_value.h>
#include <djv_ui/global.h>
#include <djv_ui/int_value.h>
#include <djv_ui/layout_row.h>

namespace djv_ui {
namespace color {
using namespace djv_image::pixel;
using djv_image::Color;

//------------------------------------------------------------------------------
// Color_Widget::_Color_Widget
//------------------------------------------------------------------------------

struct Color_Widget::_Color_Widget
{
  Int_Edit_Slider * _int [djv_image::pixel::channels_max];
  Float_Edit_Slider * _float [djv_image::pixel::channels_max];
  
  Choice * format;
  Choice * type;
  
  Layout_H * bottom_layout;
};

//------------------------------------------------------------------------------
// Color_Widget
//------------------------------------------------------------------------------

Color_Widget::Color_Widget() :
  signal(this),
  _value(RGB_U8),
  _float_min(0.0),
  _float_max(1.0),
  _p(new _Color_Widget)
{
  debug_path("Color_Widget");
  
  DJV_DEBUG("Color_Widget::Color_Widget");
  
  // Create widgets.

  for (int i = 0; i < channels_max; ++i)
  {
    _p->_int[i] = new Int_Edit_Slider;
    _p->_float[i] = new Float_Edit_Slider;
  }
  
  _p->format =
    new Choice(djv_image::pixel::label_format());
  _p->format->tooltip(tooltip_pixel_format);
  
  _p->type =
    new Choice(djv_image::pixel::label_type());
  _p->type->tooltip(tooltip_pixel_type);
  
  // Layout.

  size(layout_item::STRETCH, layout_item::FIXED);
  
  List<Layout_Item *> group;
  for (int i = 0; i < channels_max; ++i)
  {
    group += _p->_int[i];
    group += _p->_float[i];
  }
  layout_item::group(group);
  
  Layout_V * layout = new Layout_V(this);
  layout->margin(0);
  for (int i = 0; i < channels_max; ++i)
  {
    layout->add(_p->_int[i]);
    layout->add(_p->_float[i]);
  }
  
  _p->bottom_layout = new Layout_H(layout);
  _p->bottom_layout->margin(0);
  _p->bottom_layout->add_stretch();
  _p->bottom_layout->add(new Frame_Group_H(List<Frame *>() <<
    _p->format <<
    _p->type));
  
  // Initialize.
  
  widget_update();
  value_update();
  
  // Callbacks.

  for (int i = 0; i < channels_max; ++i)
  {
    _p->_int[i]->signal.set(this, int_callback);
    _p->_float[i]->signal.set(this, float_callback);
  }
  _p->format->signal.set(this, format_callback);
  _p->type->signal.set(this, type_callback);
}

Color_Widget::~Color_Widget()
{}

void Color_Widget::set(const Color & in)
{
  if (in == _value)
    return;
  
  DJV_DEBUG("Color_Widget::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  const Color prev = _value;
  _value = in;
  
  if (_value.pixel() != prev.pixel())
    widget_update();
  
  value_update();
 
  signal.emit(_value);
}

const djv_image::Color & Color_Widget::get() const
{
  return _value;
}

void Color_Widget::float_range(double min, double max)
{
  if (min == _float_min && max == _float_max)
    return;
  
  _float_min = min;
  _float_max = max;
  
  set(_value);
  
  widget_update();
}

double Color_Widget::float_min() const
{
  return _float_min;
}

double Color_Widget::float_max() const
{
  return _float_max;
}

Layout_H * Color_Widget::bottom_layout()
{
  return _p->bottom_layout;
}

void Color_Widget::int_callback(int)
{
  for (int i = 0; i < djv_image::pixel::channels(_value.pixel()); ++i)
    switch (djv_image::pixel::type(_value.pixel()))
    {
      case U8: _value.set_u8(_p->_int[i]->get(), i); break;
      case U10: _value.set_u10(_p->_int[i]->get(), i); break;
      case U16: _value.set_u16(_p->_int[i]->get(), i); break;
      
#if defined(DJV_OPENEXR)
      case F16:
        _value.set_f16(
          static_cast<djv_image::pixel::F16_T>(_p->_float[i]->get()),
          i
        );
        break;
#endif

      case F32:
        _value.set_f32(
          static_cast<djv_image::pixel::F32_T>(_p->_float[i]->get()),
          i
        );
        break;
      
      default: break;
    }

  value_update();
  
  signal.emit(_value);
}

void Color_Widget::float_callback(double)
{
  int_callback(0);
}

void Color_Widget::format_callback(int in)
{
  Color tmp(djv_image::pixel::pixel(
    static_cast<FORMAT>(in),
    djv_image::pixel::type(_value.pixel())));
  
  djv_image::color::convert(_value, tmp);
  
  set(tmp);
  
  widget_update();
}

void Color_Widget::type_callback(int in)
{
  Color tmp(djv_image::pixel::pixel(
    format(_value.pixel()),
    static_cast<TYPE>(in)));
  
  djv_image::color::convert(_value, tmp);
  
  set(tmp);
  
  widget_update();
}

void Color_Widget::widget_update()
{
  DJV_DEBUG("Color_Widget::widget_update");
  DJV_DEBUG_PRINT("value = " << _value);
  
  callbacks(false);
  
  const int channels = djv_image::pixel::channels(_value.pixel());
  
  const String tooltip0 [] =
  {
    tooltip_luminance,
    tooltip_alpha
  };
  
  const String tooltip1 [] =
  {
    tooltip_red,
    tooltip_green,
    tooltip_blue,
    tooltip_alpha
  };
  
  const String * tooltip = 0;
  switch (djv_image::pixel::format(_value.pixel()))
  {
    case L:
    case LA: tooltip = tooltip0; break;
    case RGB:
    case RGBA: tooltip = tooltip1; break;
    default: break;
  }

  for (int i = 0; i < channels; ++i)
  {
    _p->_int[i]->range(0, djv_image::pixel::max(_value.pixel()));
    _p->_int[i]->tooltip(tooltip[i]);
    _p->_float[i]->range(_float_min, _float_max);
    _p->_float[i]->tooltip(tooltip[i]);
  }
 
  for (int i = 0; i < channels_max; ++i)
    switch (djv_image::pixel::type(_value.pixel()))
    {
      case U8:
      case U10:
      case U16:
        if (i < channels)
          _p->_int[i]->show();
        else
          _p->_int[i]->hide();
        _p->_float[i]->hide();
        break;
      
#if defined(DJV_OPENEXR)
      case F16:
#endif
      case F32:
        _p->_int[i]->hide();
        if (i < channels)
          _p->_float[i]->show();
        else
          _p->_float[i]->hide();
        break;
      
      default: break;
    }
  
  _p->format->set(djv_image::pixel::format(_value.pixel()));
  _p->type->set(djv_image::pixel::type(_value.pixel()));

  update();

  callbacks(true);
}

void Color_Widget::value_update()
{
  DJV_DEBUG("Color_Widget::value_update");
  
  callbacks(false);
  
  for (int i = 0; i < djv_image::pixel::channels(_value.pixel()); ++i)
    switch (djv_image::pixel::type(_value.pixel()))
    {
      case U8: _p->_int[i]->set(_value.get_u8(i)); break;
      case U10: _p->_int[i]->set(_value.get_u10(i)); break;
      case U16: _p->_int[i]->set(_value.get_u16(i)); break;
#if defined(DJV_OPENEXR)
      case F16: _p->_float[i]->set(_value.get_f16(i)); break;
#endif
      case F32: _p->_float[i]->set(_value.get_f32(i)); break;
      default: break;
    }
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}
