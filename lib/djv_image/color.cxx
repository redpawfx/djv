/*------------------------------------------------------------------------------
 lib/djv_image/color.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "color.h"

namespace djv_image {
namespace color {

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------

void Color::init()
{
  _channels = pixel::channels(_pixel);
}

Color::Color() :
  _pixel(PIXEL(0))
{
  init();
  
  zero();
}

Color::Color(const Color & in) :
  _pixel(PIXEL(0))
{
  init();
  
  *this = in;
}

Color::Color(PIXEL pixel) :
  _pixel(pixel)
{
  init();
  
  zero();
}

Color::Color(pixel::F32_T l) :
  _pixel(pixel::L_F32)
{
  init();
  
  set_f32(l, 0);
}

Color::Color(pixel::F32_T l, pixel::F32_T a) :
  _pixel(pixel::LA_F32)
{
  init();
  
  set_f32(l, 0);
  set_f32(a, 1);
}

Color::Color(pixel::F32_T r, pixel::F32_T g, pixel::F32_T b) :
  _pixel(pixel::RGB_F32)
{
  init();
  
  set_f32(r, 0);
  set_f32(g, 1);
  set_f32(b, 2);
}

Color::Color(pixel::F32_T r, pixel::F32_T g, pixel::F32_T b, pixel::F32_T a) :
  _pixel(pixel::RGBA_F32)
{
  init();
  
  set_f32(r, 0);
  set_f32(g, 1);
  set_f32(b, 2);
  set_f32(a, 3);
}

Color & Color::operator = (const Color & in)
{
  if (&in != this)
  {
    _pixel = in._pixel;
    _channels = in._channels;
    memory::copy(in._data, _data, pixel::bytes(_pixel));
  }
  return *this;
}

void Color::pixel(PIXEL in)
{
  if (in == _pixel)
    return;
  
  Color tmp(in);
  color::convert(*this, tmp);
  *this = tmp;
}

void Color::zero()
{
  memory::set<uint8_t>(0, _data, pixel::bytes(_pixel));
}

//------------------------------------------------------------------------------

}
}

