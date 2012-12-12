/*------------------------------------------------------------------------------
 lib/djv_ui/check_button.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "check_button.h"

#include <djv_ui/icon.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace check_button {

//------------------------------------------------------------------------------
// Check_Button
//------------------------------------------------------------------------------

Check_Button::Check_Button(const String & label) :
  signal(this),
  _value(false),
  _shortcut(0),
  _icon(icon::get("tool_check")),
  _draw_index(-1)
{
  debug_path("Check_Button");
  
  Fl_Group::set_visible_focus();
  
  highlight(true);
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  this->label(label);
}

Check_Button::~Check_Button()
{}

void Check_Button::set(bool in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  signal.emit(_value);
  
  redraw();
}

void Check_Button::toggle()
{
  _value = ! _value;
  
  signal.emit(_value);
  
  redraw();
}

bool Check_Button::get() const
{
  return _value;
}

void Check_Button::label(const String & in)
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

const String & Check_Button::label() const
{
  return _label;
}

int Check_Button::handle(int in)
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
          
            toggle();
            
            return 1;
        }
      break;
      
    case FL_PUSH:
    {
      take_focus();
          
      toggle();
    }
    return 1;
    
    case FL_SHORTCUT:
      if (Fl::event_key() == _shortcut)
      {
        toggle();
        
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

void Check_Button::draw()
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

void Check_Button::dirty()
{
  widget::Widget::dirty();
  
  fl_font(text_font(), style::global()->font_size());
  
  const int margin = style::global()->margin();
  
  size_hint(
    V2i(fl_height() + margin + base::string_width(_draw_label), fl_height())
  );
}

//------------------------------------------------------------------------------

}
}

