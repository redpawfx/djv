/*------------------------------------------------------------------------------
 lib/djv_ui/color_swatch.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "color.h"

#include <djv_ui/dialog.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace color {

//------------------------------------------------------------------------------
// Swatch
//------------------------------------------------------------------------------

Swatch::Swatch() :
  signal(this),
  action_signal(this)
{
  debug_path("color::Swatch");
  
  Fl_Group::clear_visible_focus();
  
  style(frame::STYLE_BORDER);
  
  // Layout.
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::LEFT);
}

Swatch::~Swatch()
{}

void Swatch::set(const djv_image::Color & in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  redraw();
  
  signal.emit(_value);
}

const djv_image::Color & Swatch::get() const
{
  return _value;
}

int Swatch::handle(int in)
{
  DJV_DEBUG("Swatch::handle");
  
  switch (in)
  {
    case FL_FOCUS:
    case FL_UNFOCUS:
      if (visible_focus())
      {
        redraw();
        
        return 1;
      }
      break;
    
    case FL_KEYBOARD:
      if (this == Fl::focus())
        switch (Fl::event_key())
        {
          case ' ':
          
            action_signal.emit(true);
            
            redraw();
            
            return 1;
        }
      break;
      
    case FL_PUSH:
    {
      if (visible_focus())
        take_focus();
      
      action_signal.emit(true);
      
      redraw();
      
    } return 1;
  }
  
  return Frame::handle(in);
}

void Swatch::draw()
{
  DJV_DEBUG("Swatch::draw");
  
  const Box2i & geom = frame_geom();
  
  const bool inside = Widget_Base::inside();
  const bool select = visible_focus() && this == Fl::focus() && active();
  const bool highlight = this->highlight() && inside && active();
  
  DJV_DEBUG_PRINT("inside = " << inside);
  DJV_DEBUG_PRINT("select = " << select);
  DJV_DEBUG_PRINT("highlight = " << highlight);

  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  djv_image::color::convert(_value, tmp);
  fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  fl_rectf(geom.x, geom.y, geom.w, geom.h);
  
  fl_draw_box(
    box_type(style(), false),
    geom.x, geom.y, geom.w, geom.h,
    select ?
      FL_SELECTION_COLOR :
      (highlight ? style::FL_COLOR_HIGHLIGHT : FL_BACKGROUND_COLOR)
  );
  
  Widget::draw();
}

void Swatch::dirty()
{
  Frame::dirty();

  size_hint(V2i(style::global()->size_swatch()) + frame_size() * 2);
}

//------------------------------------------------------------------------------
// Swatch_Widget::_Swatch_Widget
//------------------------------------------------------------------------------

struct Swatch_Widget::_Swatch_Widget
{ 
  Swatch * widget;
  
  Label * label;
  
  Layout_V * layout;
};

//------------------------------------------------------------------------------
// Swatch_Widget
//------------------------------------------------------------------------------

Swatch_Widget::Swatch_Widget(const String & label) :
  signal(this),
  _p(new _Swatch_Widget)
{
  _p->widget =
    new Swatch;
  _p->widget->Fl_Group::set_visible_focus();
  _p->widget->highlight(true);
  
  _p->label =
    new Label(label);

  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::LEFT);

  _p->layout = new Layout_V(this);
  _p->layout->margin(0);
  if (! label.empty())
    _p->layout->add(_p->label);
  _p->layout->add(_p->widget);
  
  _p->widget->signal.set(this, widget_callback);
  _p->widget->action_signal.set(this, dialog_callback);
}

Swatch_Widget::~Swatch_Widget()
{}

void Swatch_Widget::set(const djv_image::Color & in)
{
  _p->widget->set(in);
}

const djv_image::Color & Swatch_Widget::get() const
{
  return _p->widget->get();
}

void Swatch_Widget::label(const String & in)
{
  if (! in.empty())
    _p->layout->insert(_p->label, 0);
  else
    _p->layout->remove(_p->label);
  
  _p->label->set(in);
}

const String & Swatch_Widget::label() const
{
  return _p->label->get();
}

void Swatch_Widget::widget_callback(const djv_image::Color & in)
{
  _p->widget->set(in);
  
  signal.emit(in);
}

void Swatch_Widget::dialog_callback(bool)
{
  dialog::global()->color(get(), this, widget_callback);
}

//------------------------------------------------------------------------------

}
}
