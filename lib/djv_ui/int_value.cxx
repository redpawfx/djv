/*------------------------------------------------------------------------------
 lib/djv_ui/int_value.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "int_value.h"

#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/slider.h>
#include <djv_ui/text.h>
#include <djv_ui/tool_button.h>
#include <FL/Fl.H>

namespace djv_ui {
namespace int_value {

//------------------------------------------------------------------------------
// Int_Edit::_Int_Edit
//------------------------------------------------------------------------------

struct Int_Edit::_Int_Edit
{
  Text_Edit * widget;
};

//------------------------------------------------------------------------------
// Int_Edit
//------------------------------------------------------------------------------

Int_Edit::Int_Edit() :
  signal(this),
  _p(new _Int_Edit)
{
  debug_path("Int_Edit");
  
  // Create widgets.
  
  _p->widget =
    new Text_Edit;
  _p->widget->size_string(format_int);
  
  value::Inc_Tool * inc_widget =
    new value::Inc_Tool;
  
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

Int_Edit::~Int_Edit()
{}

int Int_Edit::handle(int in)
{
  if (Int_Input_Base::handle(in, visible_focus()))
    return 1;
  
  return Widget::handle(in);
}

Layout_Item * Int_Edit::group_align()
{
  return _p->widget->group_align();
}

void Int_Edit::value_callback()
{
  value_update();
  
  signal.emit(get());
}

void Int_Edit::widget_callback(const String & in)
{
  Int_Input_Base::set(string::string_to_int(in));
  
  value_callback();
}

void Int_Edit::inc_callback(bool)
{
  Int_Input_Base::inc_small_action();
}

void Int_Edit::dec_callback(bool)
{
  Int_Input_Base::dec_small_action();
}

void Int_Edit::value_update()
{
  callbacks(false);
  
  _p->widget->set(string::int_to_string(get()));
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Int_Edit_Slider::_Int_Edit_Slider
//------------------------------------------------------------------------------

struct Int_Edit_Slider::_Int_Edit_Slider
{
  _Int_Edit_Slider() :
    label(0)
  {}

  Text_Edit * widget;
  
  Int_Slider * slider;
  
  Widget * label;
  
  Tool_Button * _default;
  
  Layout_H * layout;
};

//------------------------------------------------------------------------------
// Int_Edit_Slider
//------------------------------------------------------------------------------

Int_Edit_Slider::Int_Edit_Slider(int min, int max, const String & label) :
  Int_Input_Base(min, max),
  signal(this),
  _p(new _Int_Edit_Slider)
{
  debug_path("Int_Edit_Slider");
  
  // Create widgets.
  
  _p->widget =
    new Text_Edit;
  _p->widget->size_string(format_int);
  
  value::Inc_Tool * inc_widget =
    new value::Inc_Tool;
  
  _p->slider =
    new Int_Slider;
  
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
  
  //widget::debug_children(this);

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

Int_Edit_Slider::~Int_Edit_Slider()
{}

void Int_Edit_Slider::default_is_valid(bool in)
{
  Int_Input_Base::default_is_valid(in);
  
  if (in)
    _p->_default->show();
  else
    _p->_default->hide();
  
  update();
}
  
bool Int_Edit_Slider::default_is_valid() const
{
  return Int_Input_Base::default_is_valid();
}

void Int_Edit_Slider::label(Widget * in)
{
  if (_p->label)
    _p->layout->del(_p->label);
  
  _p->label = in;
  
  if (_p->label)
    _p->layout->insert(_p->label, 0);
  
  _p->layout->stretch(_p->slider);
  
  update();
}

Widget * Int_Edit_Slider::label() const
{
  return _p->label;
}

Layout_Item * Int_Edit_Slider::label_align()
{
  return _p->label;
}

void Int_Edit_Slider::text_font(Fl_Font in)
{
  Widget::text_font(in);
  
  if (_p->label)
    _p->label->text_font(in);
}

Fl_Font Int_Edit_Slider::text_font() const
{
  return Widget::text_font();
}

void Int_Edit_Slider::text_color(Fl_Color in)
{
  Widget::text_color(in);
  
  if (_p->label)
    _p->label->text_color(in);
}

Fl_Color Int_Edit_Slider::text_color() const
{
  return Widget::text_color();
}

int Int_Edit_Slider::handle(int in)
{
  if (Int_Input_Base::handle(in, visible_focus()))
    return 1;
  
  return Widget::handle(in);
}

Layout_Item * Int_Edit_Slider::group_align()
{
  return _p->widget->group_align();
}

void Int_Edit_Slider::value_callback()
{
  value_update();
  
  signal.emit(get());
}

void Int_Edit_Slider::range_callback()
{
  range_update();
}

void Int_Edit_Slider::widget_callback(const String & in)
{
  Int_Input_Base::set(string::string_to_int(in));
  
  value_callback();
}

void Int_Edit_Slider::inc_callback(bool)
{
  inc_small_action();
}

void Int_Edit_Slider::dec_callback(bool)
{
  dec_small_action();
}

void Int_Edit_Slider::slider_callback(int in)
{
  Int_Input_Base::set(in);
}

void Int_Edit_Slider::default_callback(bool)
{
  Int_Input_Base::set(default_value());
}

void Int_Edit_Slider::value_update()
{
  callbacks(false);
  
  const int value = get();
  
  _p->widget->set(string::int_to_string(value));
  _p->slider->set(value);
  
  if (default_is_valid() && value != default_value())
    _p->_default->activate();
  else
    _p->_default->deactivate();

  callbacks(true);
}

void Int_Edit_Slider::range_update()
{
  callbacks(false);
  
  _p->slider->range(min(), max());
  _p->slider->inc(inc_small(), inc_large());
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Int_Display::_Int_Display
//------------------------------------------------------------------------------

struct Int_Display::_Int_Display
{
  Text_Display * widget;
};

//------------------------------------------------------------------------------
// Int_Display
//------------------------------------------------------------------------------

Int_Display::Int_Display() :
  _p(new _Int_Display),
  _value(0)
{
  debug_path("Int_Display");
  
  // Create widgets.
  
  _p->widget = new Text_Display;
  _p->widget->size_string(format_int);
  
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

Int_Display::~Int_Display()
{}

void Int_Display::set(int in)
{
  _value = in;
  
  widget_update();
}

int Int_Display::get() const
{
  return _value;
}

void Int_Display::widget_update()
{
  _p->widget->set(string::int_to_string(_value));
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String format_int = "00000";

//------------------------------------------------------------------------------

}
}

