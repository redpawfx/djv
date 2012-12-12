/*------------------------------------------------------------------------------
 lib/djv_ui/radio_button.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "radio_button.h"

#include <djv_ui/icon.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace radio_button {

//------------------------------------------------------------------------------
// Radio_Button
//------------------------------------------------------------------------------

Radio_Button::Radio_Button(const String & label) :
  signal(this),
  group_signal(this),
  _value(false),
  _shortcut(0),
  _icon(icon::get("tool_radio")),
  _draw_index(-1)
{
  debug_path("Radio_Button");
  
  Fl_Group::set_visible_focus();
  
  highlight(true);
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::LEFT);
  
  this->label(label);
}

Radio_Button::~Radio_Button()
{}
  
void Radio_Button::set(bool in)
{
  if (in == _value)
    return;

  DJV_DEBUG("Radio_Button::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  _value = in;
  
  widget_update();
  
  signal.emit(_value);
  
  if (_value)
  {
    const int group_value = group_get();
    
    for (list::size_type i = 0; i < _group.size(); ++i)
      _group[i]->group_signal.emit(group_value);
  }
}

bool Radio_Button::get() const
{
  return _value;
}

void Radio_Button::group_set(int in)
{
  DJV_DEBUG("Radio_Button::group_set");
  DJV_DEBUG_PRINT("in = " << in);
  
  if (in >= 0 && in < static_cast<int>(_group.size()))
    _group[in]->set(true);
}

void Radio_Button::group(const List<Radio_Button *> & in)
{
  _group = in;
}

int Radio_Button::group_get() const
{
  int out = -1;
  
  for (list::size_type i = 0; i < _group.size(); ++i)
    if (_group[i]->get())
    {
      out = static_cast<int>(i);
      break;
    }
  
  return out;
}

const List<Radio_Button *> & Radio_Button::group() const
{
  return _group;
}

void Radio_Button::label(const String & in)
{
  if (in == _label)
    return;

  _label = in;
  
  _draw_label = _label;
  String::size_type i = _draw_label.find('&');
  if (i != String::npos)
  {
    _shortcut = string::lower(_draw_label[i + 1]);
    
    _draw_index = static_cast<int>(i);
    _draw_label.erase(i, 1);
  }
  
  redraw();
}

const String & Radio_Button::label() const
{
  return _label;
}

int Radio_Button::handle(int in)
{
  const Box2i & geom = this->geom();
  const V2i mouse(Fl::event_x(), Fl::event_y());
  
  switch (in)
  {
    case FL_KEYBOARD:
      if (this == Fl::focus())
        switch (Fl::event_key())
        {
          case ' ':
          
            set(true);
            
            return 1;
        }
      break;
      
    case FL_PUSH:
    {
      take_focus();
          
      set(true);
    }
    return 1;
    
    case FL_SHORTCUT:
      if (Fl::event_key() == _shortcut)
      {
        set(true);
        
        return 1;
      }
      break;
    
    case FL_FOCUS:
    case FL_UNFOCUS:
      redraw();
      return 1;
  }

  return Widget::handle(in);
}

void Radio_Button::draw()
{
  const Box2i & geom = this->geom();
  
  const bool inside = Widget_Base::inside();
  
  const style::Color & color = style::global()->color_value();
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  
  fl_font(text_font(), style::global()->font_size());

  int x = geom.x;
  int y = geom.y;
  
  fl_rectf(x, y, geom.w, geom.h, FL_BACKGROUND_COLOR);
  
  fl_rectf(x, y, fl_height(), fl_height(), FL_BACKGROUND2_COLOR);
  fl_draw_box(
    style::FL_BOX_BORDER,
    x, y, fl_height(), fl_height(),
    this == Fl::focus() && active() ?
      FL_SELECTION_COLOR :
      (inside && active() ? style::FL_COLOR_HIGHLIGHT : FL_BACKGROUND_COLOR)
  );
  
  if (_value)
    _icon->draw(
      x + (fl_height() - _icon->w()) / 2,
      y + (fl_height() - _icon->h()) / 2
    );
  
  x += fl_height() + style::global()->margin();
  
  fl_color(text_color());
  
  fl_draw(_draw_label.c_str(), x, y + fl_height() - fl_descent());
  
  if (_draw_index != -1)
  {
    x += base::string_width(String(_draw_label, 0, _draw_index));
    y += fl_height();
    fl_line(x, y, x + base::char_width(_draw_label[_draw_index]) - 1, y);
  }
}

void Radio_Button::dirty()
{
  widget::Widget::dirty();
  
  fl_font(text_font(), style::global()->font_size());
  
  const int margin = style::global()->margin();
  
  size_hint(
    V2i(fl_height() + margin + base::string_width(_draw_label), fl_height())
  );
}

void Radio_Button::widget_update()
{
  callbacks(false);
  
  if (_value)
    for (list::size_type i = 0; i < _group.size(); ++i)
      if (_group[i] != this)
        _group[i]->set(false);
  
  redraw();
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// group()
//------------------------------------------------------------------------------

void group(const List<Radio_Button *> & in)
{
  for (list::size_type i = 0; i < in.size(); ++i)
    in[i]->group(in);
}

//------------------------------------------------------------------------------

}
}

