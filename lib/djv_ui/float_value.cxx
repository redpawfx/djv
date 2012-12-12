/*------------------------------------------------------------------------------
 lib/djv_ui/float_value.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "float_value.h"

#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/slider.h>
#include <djv_ui/text.h>
#include <djv_ui/tool_button.h>
#include <FL/Fl.H>

namespace djv_ui {
namespace float_value {

//------------------------------------------------------------------------------
// Float_Edit::_Float_Edit
//------------------------------------------------------------------------------

struct Float_Edit::_Float_Edit
{
  Text_Edit * widget;
};

//------------------------------------------------------------------------------
// Float_Edit
//------------------------------------------------------------------------------

Float_Edit::Float_Edit() :
  signal(this),
  _p(new _Float_Edit)
{
  debug_path("Float_Edit");
  
  // Create widgets.
  
  _p->widget = new Text_Edit;
  _p->widget->size_string(format_float);
  
  value::Inc_Tool * inc_widget = new value::Inc_Tool;
  
  // Layout.
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  Layout_H * layout = new Layout_H(this);
  layout->spacing(0);
  layout->margin(0);
  layout->add(_p->widget);
  layout->stretch(_p->widget);
  layout->add(inc_widget);

  // Initialize.
  
  value_update();

  // Callbacks.
  
  _p->widget->signal.set(this, widget_callback);
  inc_widget->inc_signal.set(this, inc_callback);
  inc_widget->dec_signal.set(this, dec_callback);
}

Float_Edit::~Float_Edit()
{}

int Float_Edit::handle(int in)
{
  if (Float_Input_Base::handle(in, visible_focus()))
    return 1;
  
  return Widget::handle(in);
}

Layout_Item * Float_Edit::group_align()
{
  return _p->widget->group_align();
}

void Float_Edit::value_callback()
{
  value_update();
  
  signal.emit(get());
}

void Float_Edit::widget_callback(const String & in)
{
  set(string::string_to_float(in));
  
  value_callback();
}

void Float_Edit::inc_callback(bool)
{
  inc_small_action();
}

void Float_Edit::dec_callback(bool)
{
  dec_small_action();
}

void Float_Edit::value_update()
{
  callbacks(false);
  
  _p->widget->set(string::float_to_string(get(), precision()));
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Float_Edit_Slider::_Float_Edit_Slider
//------------------------------------------------------------------------------

struct Float_Edit_Slider::_Float_Edit_Slider
{
  _Float_Edit_Slider() :
    label(0)
  {}

  Text_Edit * widget;
  
  Float_Slider * slider;
  
  Widget * label;
  
  Tool_Button * _default;
  
  Layout_H * layout;
};

//------------------------------------------------------------------------------
// Float_Edit_Slider
//------------------------------------------------------------------------------

Float_Edit_Slider::Float_Edit_Slider(double min, double max,
  const String & label) :
  Float_Input_Base(min, max),
  signal(this),
  _p(new _Float_Edit_Slider)
{
  debug_path("Float_Edit_Slider");
  
  clamp(false);
  
  // Create widgets.
  
  _p->widget =
    new Text_Edit;
  _p->widget->size_string(format_float);
  
  value::Inc_Tool * inc_widget =
    new value::Inc_Tool;

  _p->slider =
    new Float_Slider;
  
  if (! label.empty())
    _p->label =
      new Label(label);
  
  _p->_default =
    new Tool_Button("tool_x");
  _p->_default->tooltip(base::tooltip_reset);
  _p->_default->deactivate();
  _p->_default->hide();
  
  // Layout.
  
  size(layout_item::STRETCH, layout_item::FIXED);
  
  Layout_H * layout_h = 0;
  
  _p->layout = new Layout_H(this);
  _p->layout->margin(0);
  if (_p->label)
    _p->layout->add(_p->label);
  layout_h = new Layout_H(_p->layout);
  layout_h->spacing(0);
  layout_h->margin(0);
  layout_h->add(_p->widget);
  layout_h->add(inc_widget);
  _p->layout->add(_p->slider);
  _p->layout->stretch(_p->slider);
  _p->layout->add(_p->_default);

  // Initialize.
  
  range_update();
  value_update();

  // Callbacks.
  
  _p->widget->signal.set(this, widget_callback);
  inc_widget->inc_signal.set(this, inc_callback);
  inc_widget->dec_signal.set(this, dec_callback);
  _p->slider->signal.set(this, slider_callback);
  _p->_default->signal.set(this, default_callback);
}

Float_Edit_Slider::~Float_Edit_Slider()
{}

void Float_Edit_Slider::default_is_valid(bool in)
{
  Float_Input_Base::default_is_valid(in);
  
  if (in)
    _p->_default->show();
  else
    _p->_default->hide();
  
  update();
}

bool Float_Edit_Slider::default_is_valid() const
{
  return Float_Input_Base::default_is_valid();
}

void Float_Edit_Slider::range(double min, double max)
{
  Float_Input_Base::range(min, max);

  slider_range(min, max);
}

void Float_Edit_Slider::slider_range(double min, double max)
{
  _p->slider->range(min, max);
}

void Float_Edit_Slider::label(Widget * in)
{
  if (_p->label)
    _p->layout->del(_p->label);
  
  _p->label = in;
  
  if (_p->label)
    _p->layout->insert(_p->label, 0);
  
  _p->layout->stretch(_p->slider);
  
  update();
}

Widget * Float_Edit_Slider::label() const
{
  return _p->label;
}

Layout_Item * Float_Edit_Slider::label_align()
{
  return _p->label;
}

void Float_Edit_Slider::text_font(Fl_Font in)
{
  Widget::text_font(in);

  if (_p->label)
    _p->label->text_font(in);
}

Fl_Font Float_Edit_Slider::text_font() const
{
  return Widget::text_font();
}

void Float_Edit_Slider::text_color(Fl_Color in)
{
  Widget::text_color(in);

  if (_p->label)
    _p->label->text_color(in);
}

Fl_Color Float_Edit_Slider::text_color() const
{
  return Widget::text_color();
}

int Float_Edit_Slider::handle(int in)
{
  if (Float_Input_Base::handle(in, visible_focus()))
    return 1;

  return Widget::handle(in);
}

Layout_Item * Float_Edit_Slider::group_align()
{
  return _p->widget->group_align();
}

void Float_Edit_Slider::value_callback()
{
  value_update();
  
  signal.emit(get());
}

void Float_Edit_Slider::range_callback()
{
  range_update();
}

void Float_Edit_Slider::widget_callback(const String & in)
{
  Float_Input_Base::set(string::string_to_float(in));
  
  value_callback();
}

void Float_Edit_Slider::inc_callback(bool)
{
  inc_small_action();
}

void Float_Edit_Slider::dec_callback(bool)
{
  dec_small_action();
}

void Float_Edit_Slider::slider_callback(double in)
{
  Float_Input_Base::set(in);
}

void Float_Edit_Slider::default_callback(bool)
{
  Float_Input_Base::set(default_value());
}

void Float_Edit_Slider::value_update()
{
  callbacks(false);
  
  const double value = get();
  
  _p->widget->set(string::float_to_string(value));
  _p->slider->set(value);
  
  if (default_is_valid() && value != default_value())
    _p->_default->activate();
  else
    _p->_default->deactivate();

  callbacks(true);
}

void Float_Edit_Slider::range_update()
{
  callbacks(false);
  
  _p->slider->range(min(), max());
  _p->slider->inc(inc_small(), inc_large());
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Float_Display::_Float_Display
//------------------------------------------------------------------------------

struct Float_Display::_Float_Display
{
  Text_Display * widget;
};

//------------------------------------------------------------------------------
// Float_Display::Float_Display
//------------------------------------------------------------------------------

Float_Display::Float_Display() :
  _precision(0),
  _p(new _Float_Display)
{
  debug_path("Float_Display");
  
  // Create widgets.
  
  _p->widget = new Text_Display;
  _p->widget->size_string(format_float);
  
  // Layout.
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  Layout_H * layout = new Layout_H(this);
  layout->spacing(0);
  layout->margin(0);
  layout->add(_p->widget);
  
  // Initialize.
  
  widget_update();
}

Float_Display::~Float_Display()
{}

void Float_Display::set(double in)
{
  _value = in;
  
  widget_update();
}

double Float_Display::get() const
{
  return _value;
}

void Float_Display::precision(int in)
{
  _precision = in;
}

int Float_Display::precision() const
{
  return _precision;
}

void Float_Display::text_color(Fl_Color in)
{
  Widget::text_color(in);
  
  _p->widget->text_color(in);
}

Fl_Color Float_Display::text_color() const
{
  return Widget::text_color();
}

void Float_Display::draw()
{
  Widget::draw();
}

void Float_Display::dirty()
{
  Widget::dirty();
  
  _p->widget->text_color(text_color());
}

void Float_Display::widget_update()
{
  _p->widget->set(string::float_to_string(_value, _precision));
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String format_float = "000.00";

//------------------------------------------------------------------------------

}
}

