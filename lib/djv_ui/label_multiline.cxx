/*------------------------------------------------------------------------------
 lib/djv_ui/label_multiline.cxx

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
// Multiline_Label
//------------------------------------------------------------------------------

Multiline_Label::Multiline_Label(const String & in) :
   push_signal(this),
  _word_size_max(0),
  _space_size(0),
  _text_height(0),
  _font_size(0)
{
  debug_path("Multiline_Label");
  
  align(layout_item::LEFT, layout_item::CENTER);
  
  set(in);
}

Multiline_Label::~Multiline_Label()
{}

void Multiline_Label::set(const String & in)
{
  if (in == _value)
    return;

  DJV_DEBUG("Multiline_Label::set");
  DJV_DEBUG_PRINT("in = " << in);

  _value = in;
  
  _word_list.clear();
  
  List<String> tmp;
  
  if (_value.size())
    tmp = string::split(_value, '\n', true);
  
  for (list::size_type i = 0; i < tmp.size(); ++i)
  {
    if (tmp[i].size())
      _word_list += string::split(tmp[i], ' ', true);

    if (i < tmp.size() - 1)
      _word_list += "\n";
  }
  
  _word_size_list.resize(_word_list.size() + 1);
  
  text_measure();
  update();
  redraw();
}

const String & Multiline_Label::get() const
{
  return _value;
}

int Multiline_Label::handle(int in)
{
  switch (in)
  {
    case FL_PUSH:
      push_signal.emit(true);
      return 1;
  }
  
  return Widget::handle(in);
}

void Multiline_Label::draw()
{
  Widget::draw();

  const Box2i & geom = this->geom();

  fl_font(text_font(), style::global()->font_size());
  fl_color(FL_FOREGROUND_COLOR);
  
  int x = 0, y = 0;
  for (list::size_type i = 0; i < _word_list.size(); ++i)
  {
    if (_word_list[i] != "\n")
    {
      fl_draw(
        _word_list[i].c_str(),
        geom.x + offset(align()[0], geom.w, _text_hint.x) + x,
        geom.y + offset(align()[1], geom.h, _text_hint.y) + y +
          fl_height() - fl_descent()
      );
    }
    
    x += _word_size_list[i];

    if (
      "\n" == _word_list[i] ||
      ((x + _word_size_list[i + 1]) > (geom.w - 1))
    ) {
      x = 0;
      y += _text_height;
    }
  }
}

void Multiline_Label::dirty()
{
  DJV_DEBUG("Multiline_Label::dirty");
  
  Widget::dirty();
  
  const int font_size = style::global()->font_size();
  fl_font(text_font(), font_size);
  if (font_size != _font_size)
  {
    _font_size = font_size;
    text_measure();
  }
  
  _text_hint = text_hint(geom().size);
  
  _size_min = V2i(
    math::max(_word_size_max, style::global()->size_text()),
    _text_hint.y
  );
  
  DJV_DEBUG_PRINT("text = " << _text_hint);
  DJV_DEBUG_PRINT("min = " << _size_min);
  
  // XXX
  
  //if (_text_hint != size_hint())
  const int slop =
    static_cast<int>(vector::length(math::abs(_text_hint - size_hint())));
  if (slop > 20)
  {
    DJV_DEBUG_PRINT("update");
    
    size_hint(_text_hint);
    
    update();
  }
}

void Multiline_Label::geom(const Box2i & in)
{
  DJV_DEBUG("Multiline_Label::geom");
  DJV_DEBUG_PRINT("in = " << in);
  
  const Box2i geom = this->geom();
  
  Widget::geom(in);
  
  if (geom != this->geom())
  {
    const V2i size = text_hint(in.size);
    const V2i hint = size_hint();
    
    DJV_DEBUG_PRINT("size = " << size);
    DJV_DEBUG_PRINT("hint = " << hint);
    
    if (size != hint)
      dirty();
  }
}

const Box2i & Multiline_Label::geom() const
{
  return Widget::geom();
}

const V2i & Multiline_Label::size_min() const
{
  return _size_min;
}

Layout_Item * Multiline_Label::group_align()
{
  return 0;
}

Layout_Item * Multiline_Label::label_align()
{
  return this;
}

void Multiline_Label::text_measure()
{
  _word_size_max = 0;
  
  if (_word_list.size())
  {
    list::size_type i = 0;
    
    for (; i < _word_list.size(); ++i)
    {
      _word_size_list[i] =
        _word_list[i] != "\n" ?
        base::string_width(_word_list[i] + " ") :
        0;

      _word_size_max = math::max(_word_size_max, _word_size_list[i]);
    }
    
    _word_size_list[i] = 0;
  }
  
  _space_size = base::string_width(" ");
  
  _text_height = fl_height();
}

V2i Multiline_Label::text_hint(const V2i & in) const
{
  V2i out;
  
  int _x = 0;
  int _y = _text_height;
  
  for (list::size_type i = 0; i < _word_list.size(); ++i)
  {
    const bool newline = "\n" == _word_list[i];
    
    _x += _word_size_list[i];
    
    if (
      newline ||
      ((_x + _word_size_list[i + 1]) > (in.x - 1))
    ) {
      out.x = math::max(out.x, _x);
      
      _x = 0;
      _y += _text_height;
    }
  }
  
  out.y = _y;
  
  return out;
}

//------------------------------------------------------------------------------

}
}

