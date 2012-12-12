/*------------------------------------------------------------------------------
 lib/djv_ui/push_button.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "push_button.h"

#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace push_button {

//------------------------------------------------------------------------------
// Push_Button::Push_Button
//------------------------------------------------------------------------------

Push_Button::Push_Button(const String & label) :
  signal(this),
  _shortcut(0),
  _pressed(false),
  _pressed_inside(false),
  _draw_index(-1)
{
  debug_path("Push_Button");
  
  Fl_Group::set_visible_focus();
  
  highlight(true);
  
  size(layout_item::FIXED, layout_item::FIXED);
  
  this->label(label);
}

Push_Button::~Push_Button()
{}

void Push_Button::label(const String & in)
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

const String & Push_Button::label() const
{
  return _label;
}

int Push_Button::handle(int in)
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
          
            signal.emit(true);
            
            redraw();
            
            return 1;
        }
      break;
      
    case FL_PUSH:
    {
      take_focus();
      
      _pressed = true;
      _pressed_inside = true;
      
      redraw();
    }
    return 1;
      
    case FL_RELEASE:
    {
      if (intersect(geom, mouse))
        signal.emit(true);
      
      _pressed = false;
      _pressed_inside = false;
    
      redraw();
    }
    return 1;
    
    case FL_DRAG:
      
      if (intersect(geom, mouse))
      {
        if (! _pressed_inside)
        {
          _pressed_inside = true;
          
          redraw();
        }
      }
      else
      {
        if (_pressed_inside)
        {
          _pressed_inside = false;
          
          redraw();
        }
      }
      return 1;
    
    case FL_SHORTCUT:
      if (Fl::event_key() == _shortcut)
      {
        signal.emit(true);
        
        redraw();
        
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

void Push_Button::draw()
{
  const Box2i & geom = this->geom();
  
  const bool inside = Widget_Base::inside();
  const bool select = this == Fl::focus() && active();
  const bool highlight = inside && active();
  
  const style::Color & color = style::global()->color_value();
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  
  const bool down = _pressed && _pressed_inside;

  fl_draw_box(
    down ? style::FL_BOX_DOWN : style::FL_BOX_UP,
    geom.x, geom.y, geom.w, geom.h,
    select ?
      FL_SELECTION_COLOR :
      (highlight ? style::FL_COLOR_HIGHLIGHT : style::FL_COLOR_BUTTON)
  );
  
  fl_font(text_font(), style::global()->font_size());
  fl_color(text_color());
  
  int x = geom.x + (geom.w - base::string_width(_draw_label)) / 2;
  int y = geom.y + (geom.h - fl_height()) / 2;
 
  fl_draw(_draw_label.c_str(), x, y + fl_height() - fl_descent());
  
  if (_draw_index != -1)
  {
    x += base::string_width(String(_draw_label, 0, _draw_index));
    y += fl_height();
    fl_line(x, y, x + base::char_width(_draw_label[_draw_index]) - 1, y);
  }
}

void Push_Button::dirty()
{
  DJV_DEBUG("Push_Button::dirty");
  
  widget::Widget::dirty();
  
  fl_font(text_font(), style::global()->font_size());
  
  const V2i label_size = base::string_size(_label);

  size_hint(
    V2i(
      math::max(label_size.x, style::global()->size_button()),
      label_size.y
    ) +
    style::global()->margin_widget() * 2 +
    Fl::box_dx(style::FL_BOX_UP) * 2
  );
}

//------------------------------------------------------------------------------

}
}

