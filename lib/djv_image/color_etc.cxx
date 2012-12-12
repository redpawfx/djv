/*------------------------------------------------------------------------------
 lib/djv_image/color_etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "color.h"

namespace djv_image {
namespace color {

//------------------------------------------------------------------------------
// scale()
//------------------------------------------------------------------------------

void scale(double value, const Color & in, Color & out)
{
  Color in_(pixel::pixel(pixel::format(out.pixel()), pixel::F32));
  convert(in, in_);
  
  const int channels = pixel::channels(out.pixel());
  for (int c = 0; c < channels; ++c)
    in_.set_f32(in_.get_f32(c) * static_cast<float>(value), c);
  
  convert(in_, out);
}

Color scale(double value, const Color & in)
{
  Color out(in.pixel());
  scale(value, in, out);
  return out;
}

//------------------------------------------------------------------------------
// lerp()
//------------------------------------------------------------------------------

void lerp(double value, const Color & min, const Color & max, Color & out)
{
  Color min_(pixel::pixel(pixel::format(out.pixel()), pixel::F32));
  Color max_(pixel::pixel(pixel::format(out.pixel()), pixel::F32));
  convert(min, min_);
  convert(max, max_);
  
  const int channels = pixel::channels(out.pixel());
  for (int c = 0; c < channels; ++c)
    min_.set_f32(
      math::lerp(value, min_.get_f32(c), max_.get_f32(c)),
      c
    );
  
  convert(min_, out);
}

Color lerp(double value, const Color & min, const Color & max)
{
  Color out(math::max(min.pixel(), max.pixel()));
  lerp(value, min, max, out);
  return out;
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Color & a, const Color & b)
{
  if (a.pixel() != b.pixel())
    return false;
  
  return memory::compare(a.data(), b.data(), pixel::bytes(a.pixel())) == 0;
}

bool operator != (const Color & a, const Color & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

void convert(const Color & in, Color & out)
{
  pixel::convert(in.data(), in.pixel(), out.data(), out.pixel());
}

String & operator >> (String & in, Color & out) throw (String)
{
  PIXEL pixel(PIXEL(0));
  in >> pixel;
  out.pixel(pixel);
  
  const int channels = pixel::channels(pixel);
  for (int c = 0; c < channels; ++c)
    switch (pixel::type(pixel))
    {
      case pixel::U8:
      {
        int value = 0;
        in >> value;
        out.set_u8(math::clamp(value, 0, pixel::u8_max), c);
      }
      break;

      case pixel::U10:
      {
        int value = 0;
        in >> value;
        out.set_u10(math::clamp(value, 0, pixel::u10_max), c);
      }
      break;

      case pixel::U16:
      {
        int value = 0;
        in >> value;
        out.set_u16(math::clamp(value, 0, pixel::u16_max), c);
      }
      break;

#if defined(DJV_OPENEXR)

      case pixel::F16:
      {
        double value = 0.0;
        in >> value;
        out.set_f16(static_cast<pixel::F16_T>(value), c);
      }
      break;

#endif

      case pixel::F32:
      {
        double value = 0.0;
        in >> value;
        out.set_f32(static_cast<pixel::F32_T>(value), c);
      }
      break;
      
      default: break;
    }

  return in;
}

String & operator << (String & out, const Color & in)
{
  out << in.pixel();
  
  const int channels = pixel::channels(in.pixel());
  for (int c = 0; c < channels; ++c)
    switch (pixel::type(in.pixel()))
    {
      case pixel::U8: out << in.get_u8(c); break;
      case pixel::U10: out << in.get_u10(c); break;
      case pixel::U16: out << in.get_u16(c); break;
#if defined (DJV_OPENEXR)
      case pixel::F16: out << in.get_f16(c); break;
#endif
      case pixel::F32: out << in.get_f32(c); break;
      default: break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const Color & in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------

}
}

