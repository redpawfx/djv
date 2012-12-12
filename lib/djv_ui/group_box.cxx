/*------------------------------------------------------------------------------
 lib/djv_ui/group_box.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "group_box.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>
#include <FL/Fl_Group.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace group_box {

//------------------------------------------------------------------------------
// Box
//------------------------------------------------------------------------------

Box::Box(const String & label) :
  _label(label)
{
  debug_path("group:_box:Box");
  
  Layout_V * layout = new Layout_V(this);
}

Box::~Box()
{}

void Box::del()
{
  widget::Widget::del();
}

void Box::label(const String & in)
{
  if (in == _label)
    return;
  
  _label = in;
  
  redraw();
}

const String & Box::label() const
{
  return _label;
}

void Box::draw()
{
  const Box2i & geom = this->geom();
  const int margin = style::global()->margin_widget();
  
  const style::Color & color_value = style::global()->color_value();
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  
  djv_image::color::scale(0.6, color_value.background, tmp);
  const Fl_Color color =
    fl_rgb_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  if (_label.empty())
  {
    // Draw box.
  
    fl_draw_box(
      FL_BORDER_FRAME,
      geom.x,
      geom.y,
      geom.w,
      geom.h,
      color
    );
  }
  else
  {
    // Draw box.
  
    fl_draw_box(
      FL_BORDER_FRAME,
      geom.x,
      geom.y + _size_label.y / 2,
      geom.w,
      geom.h - _size_label.y / 2,
      color
    );
  
    // Draw label.

    fl_font(text_font(), style::global()->font_size());
  
    fl_color(FL_BACKGROUND_COLOR);
    //fl_color(FL_BLUE);
    fl_rectf(
      geom.x + 2 + margin,
      geom.y,
      _size_label.x + margin * 2,
      _size_label.y
    );
  
    fl_color(FL_FOREGROUND_COLOR);
    fl_draw(
      _label.c_str(),
      geom.x + 2 + margin * 2,
      geom.y + fl_height() - fl_descent()
    );
  }

  widget::Widget::draw();
}

void Box::dirty()
{
  DJV_DEBUG("Box::dirty");
  
  widget::Widget::dirty();

  fl_font(text_font(), style::global()->font_size());

  const V2i size = layout() ? layout()->size_hint() : V2i();
  
  DJV_DEBUG_PRINT("size = " << size);

  _size_label = _label.empty() ?
    V2i(0, 2) :
    V2i(base::string_width(_label),
    fl_height()
  );
  
  DJV_DEBUG_PRINT("label size = " << _size_label);
  
  const int margin = style::global()->margin_widget();
  const int spacing = style::global()->spacing();
  
  size_hint(
    V2i(
      math::max(_size_label.x + margin * 4, size.x) + 2 * 2,
      _size_label.y + spacing + size.y + 2
    )
  );
}

void Box::geom(const Box2i & in)
{
  DJV_DEBUG("Box::geom");
  DJV_DEBUG_PRINT("in = " << in);
  
  //Fl_Group::resize(in.x, in.y, in.w, in.h);
  Fl_Widget::resize(in.x, in.y, in.w, in.h);
  Layout_Item::geom(in);

  const int spacing = style::global()->spacing();
  
  if (layout())
    layout()->geom(Box2i(
      in.x + 2,
      in.y + _size_label.y + spacing,
      in.w - 2 * 2,
      in.h - _size_label.y - spacing - 2)
    );
}

const Box2i & Box::geom() const
{
  return widget::Widget::geom();
}

//------------------------------------------------------------------------------
// Box_Small
//------------------------------------------------------------------------------

Box_Small::Box_Small(const String & label) :
  Box(label)
{
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::LEFT);
}

//------------------------------------------------------------------------------
// Box_Large
//------------------------------------------------------------------------------

Box_Large::Box_Large(const String & label) :
  Box(label)
{
  text_font(style::global()->font_bold());
  
  Layout_V * layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());
}

//------------------------------------------------------------------------------

}
}
