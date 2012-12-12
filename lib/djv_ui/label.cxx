/*------------------------------------------------------------------------------
 lib/djv_ui/label.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "label_private.h"

#include <djv_ui/style.h>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace label {

//------------------------------------------------------------------------------
// Label
//------------------------------------------------------------------------------

Label::Label(const String & in) :
  push_signal(this),
  _lines_max_width(0)
{
  debug_path("Label");
  
  box(FL_FLAT_BOX);

  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  set(in);
}

Label::~Label()
{}

void Label::set(const String & in)
{
  if (in == _value)
    return;

  DJV_DEBUG("Label::set");
  DJV_DEBUG_PRINT("in = " << in);

  _value = in;
  
  // Split the value into lines.
  
  _lines = string::split(_value, '\n', true);
  
  // Update.
  
  const V2i size = size_hint();
  dirty();
  if (size != size_hint())
    update();
  redraw();
}

const String & Label::get() const
{
  return _value;
}

int Label::handle(int in)
{
  switch (in)
  {
    case FL_PUSH:
      push_signal.emit(true);
      return 1;
  }
  
  return Widget::handle(in);
}

void Label::draw()
{
  Widget::draw();

  const Box2i & geom = this->geom();
  
  fl_font(text_font(), style::global()->font_size());
  fl_color(text_color());
  
  // Draw each line of text.
  
  int x = geom.x + offset(align()[0], geom.w, _lines_max_width);
  int y = geom.y +
    offset(align()[1], geom.h, fl_height() * static_cast<int>(_lines.size()));
  
  for (list::size_type i = 0; i < _lines.size(); ++i, y += fl_height())
    fl_draw(_lines[i].c_str(), x, y + fl_height() - fl_descent());
}

void Label::size_string(const String & in)
{
  _size_string = in;
}

const String & Label::size_string() const
{
  return _size_string;
}

void Label::dirty()
{
  Widget::dirty();

  fl_font(text_font(), style::global()->font_size());
  
  // Find the maximum width of the lines.
  
  _lines_max_width = 0;
  
  for (list::size_type i = 0; i < _lines.size(); ++i)
    _lines_max_width =
      math::max(base::string_width(_lines[i]), _lines_max_width);
  
  // Size hint is either the maximum line width or user-specified "size_string".
  
  size_hint(
    V2i(
      math::max(_lines_max_width, base::string_width(_size_string)),
      static_cast<int>(_lines.size()) * fl_height()
    )
  );
}

Layout_Item * Label::group_align()
{
  return 0;
}

Layout_Item * Label::label_align()
{
  return this;
}

//------------------------------------------------------------------------------

}
}

