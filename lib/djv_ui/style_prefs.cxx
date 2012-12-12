/*------------------------------------------------------------------------------
 lib/djv_ui/style_prefs.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "style.h"

#include <djv_ui/global.h>
#include <djv_ui/prefs.h>

namespace djv_ui {
namespace style {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  _color(COLOR_DARK),
  _focus(true),
  _highlight(true),
  _size(SIZE_MEDIUM)
{
  DJV_DEBUG("Prefs::Prefs");
  
  _color_value[COLOR_DARK] =
    Color(
      djv_image::Color(0.8f),
      djv_image::Color(0.3f),
      djv_image::Color(0.2f),
      djv_image::Color(0.4f),
      djv_image::Color(1.0f, 0.9f, 0.5f),
      djv_image::Color(0.6f, 0.6f, 0.8f)
    );
  _color_value[COLOR_NEUTRAL] =
    Color(
      djv_image::Color(0.9f),
      djv_image::Color(0.5f),
      djv_image::Color(0.4f),
      djv_image::Color(0.6f),
      djv_image::Color(1.0f, 0.9f, 0.5f),
      djv_image::Color(0.5f, 0.5f, 0.9f)
    );
  _color_value[COLOR_LIGHT] =
    Color(
      djv_image::Color(0.0f),
      djv_image::Color(0.8f),
      djv_image::Color(0.7f),
      djv_image::Color(0.8f),
      djv_image::Color(1.0f, 0.9f, 0.5f),
      djv_image::Color(0.4f, 0.4f, 1.0f)
    );
  _color_value[COLOR_CUSTOM] = _color_value[_color];
  
  _size_value[SIZE_SMALL] = 10;
  _size_value[SIZE_MEDIUM] = 12;
  _size_value[SIZE_LARGE] = 14;
  _size_value[SIZE_CUSTOM] = _size_value[_size];
  
  prefs::Prefs prefs(prefs::prefs_global(), "style");
  prefs::get(&prefs, "color", &_color);
  for (int i = 0; i < _COLOR_SIZE; ++i)
    prefs::get(
      &prefs,
      string::Format("color_value%%").arg(i),
      &_color_value[i]
    );
  prefs::get(&prefs, "focus", &_focus);
  prefs::get(&prefs, "highlight", &_highlight);
  prefs::get(&prefs, "size", &_size);
  for (int i = 0; i < _SIZE_SIZE; ++i)
    prefs::get(
      &prefs,
      string::Format("size_value%%").arg(i),
      &_size_value[i]
    );
}

Prefs::~Prefs()
{
  DJV_DEBUG("Prefs::~Prefs");

  prefs::Prefs prefs(prefs::prefs_global(), "style");
  prefs::set(&prefs, "color", _color);
  for (int i = 0; i < _COLOR_SIZE; ++i)
    prefs::set(
      &prefs,
      string::Format("color_value%%").arg(i),
      _color_value[i]
    );
  prefs::set(&prefs, "focus", _focus);
  prefs::set(&prefs, "highlight", _highlight);
  prefs::set(&prefs, "size", _size);
  for (int i = 0; i < _SIZE_SIZE; ++i)
    prefs::set(
      &prefs,
      string::Format("size_value%%").arg(i),
      _size_value[i]
    );
}
  
void Prefs::color(COLOR in)
{
  if (in == _color)
    return;
  
  _color = in;
  
  global()->color(_color);
}
  
COLOR Prefs::color() const
{
  return _color;
}
  
void Prefs::color_value(COLOR color, const Color & in)
{
  if (in == _color_value[color])
    return;
  
  _color_value[color] = in;
  
  global()->color_value(_color, _color_value[color]);
}

void Prefs::color_value(const Color & in)
{
  color_value(_color, in);
}
  
const Color & Prefs::color_value(COLOR color) const
{
  return _color_value[color];
}

const Color & Prefs::color_value() const
{
  return color_value(_color);
}

void Prefs::focus(bool in)
{
  if (in == _focus)
    return;
  
  _focus = in;
  
  djv_ui::global()->window_redraw();
}
  
bool Prefs::focus() const
{
  return _focus;
}

void Prefs::highlight(bool in)
{
  if (in == _highlight)
    return;
  
  _highlight = in;
  
  djv_ui::global()->window_redraw();
}
  
bool Prefs::highlight() const
{
  return _highlight;
}

void Prefs::size(SIZE in)
{
  if (in == _size)
    return;
  
  _size = in;
  
  global()->size(_size);
}
  
SIZE Prefs::size() const
{
  return _size;
}

void Prefs::size_value_(SIZE size, int in)
{
  if (in == _size_value[size])
    return;
  
  _size_value[size] = in;
  
  global()->size_value_(size, _size_value[size]);
}

void Prefs::size_value(int in)
{
  size_value_(_size, in);
}

int Prefs::size_value_(SIZE size) const
{
  return _size_value[size];
}

int Prefs::size_value() const
{
  return size_value_(_size);
}

//------------------------------------------------------------------------------

}
}

