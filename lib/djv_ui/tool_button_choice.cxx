/*------------------------------------------------------------------------------
 lib/djv_ui/tool_button_choice.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "tool_button.h"

#include <djv_ui/icon.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace tool_button {

//------------------------------------------------------------------------------
// Tool_Choice_Button
//------------------------------------------------------------------------------

Tool_Choice_Button::Tool_Choice_Button(
  const List<String> & icon,
  int margin
) :
  signal(this),
  _value(-1),
  _shortcut(0),
  _margin(margin),
  _pressed(false),
  _pressed_inside(false)
{
  debug_path("Tool_Choice_Button");
  
  highlight(true);
  
  size(layout_item::FIXED, layout_item::FIXED);

  _icon.resize(icon.size());
  for (list::size_type i = 0; i < _icon.size(); ++i)
    _icon[i] = djv_ui::icon::get(icon[i]);
}

Tool_Choice_Button::~Tool_Choice_Button()
{}

void Tool_Choice_Button::set(int in)
{
  if (in == _value)
    return;
  
  DJV_DEBUG("Tool_Choice_Button::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  _value = in;
  
  dirty();
  redraw();
  
  signal.emit(_value);
}

int Tool_Choice_Button::get() const
{
  return _value;
}

void Tool_Choice_Button::shortcut(int in)
{
  _shortcut = in;
}

int Tool_Choice_Button::shortcut() const
{
  return _shortcut;
}

int Tool_Choice_Button::handle(int in)
{
  const Box2i & geom = this->geom();
  const V2i mouse(Fl::event_x(), Fl::event_y());
  
  switch (in)
  {
    case FL_PUSH:
    {
      _pressed = true;
      _pressed_inside = true;
      
      redraw();
    }
    return 1;
      
    case FL_RELEASE:
    {
      if (intersect(geom, mouse))
        action();
      
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
        action();
        
        return 1;
      }
      break;
  }
  
  return Frame::handle(in);
}

void Tool_Choice_Button::draw()
{
  const Box2i & geom = this->geom();
  
  const bool inside = Widget_Base::inside();
  
  const bool down = _pressed && _pressed_inside;
  
  fl_draw_box(
    box_type(style(), down),
    geom.x, geom.y, geom.w, geom.h,
    inside ? style::FL_COLOR_HIGHLIGHT : style::FL_COLOR_BUTTON
  );
  
  if (_value >= 0 && _value < static_cast<int>(_icon.size()))
  {
    const int x = geom.x + (geom.w - _icon[_value]->w()) / 2;
    const int y = geom.y + (geom.h - _icon[_value]->h()) / 2;
  
    _icon[_value]->draw(x, y);
  }
}

void Tool_Choice_Button::margin(int in)
{
  _margin = in;
}

int Tool_Choice_Button::margin() const
{
  return _margin;
}

void Tool_Choice_Button::dirty()
{
  Frame::dirty();

  size_hint(
    (
      (_value >= 0 && _value < static_cast<int>(_icon.size())) ?
        V2i(_icon[0]->w(), _icon[0]->h()) :
        V2i(style::global()->size_icon())
    ) +
    _margin * 2
  );
}

void Tool_Choice_Button::action()
{
  DJV_DEBUG("Tool_Choice_Button::action");
  
  const int value =
    math::wrap(_value + 1, 0, static_cast<int>(_icon.size()) - 1);
  
  DJV_DEBUG_PRINT("value = " << _value);
  DJV_DEBUG_PRINT("value = " << value);
  
  set(value);
}

//------------------------------------------------------------------------------

}
}

