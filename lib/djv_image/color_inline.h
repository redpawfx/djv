/*------------------------------------------------------------------------------
 include/djv_image/color_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_image {
namespace color {

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------

inline PIXEL Color::pixel() const
{
  return _pixel;
}

inline int Color::channels() const
{
  return _channels;
}

inline uint8_t * Color::data()
{
  return _data;
}

inline const uint8_t * Color::data() const
{
  return _data;
}

//------------------------------------------------------------------------------
// Color::set()
//------------------------------------------------------------------------------

#define _COLOR_SET_WORK1(TYPE) \
  \
  case U8: \
    (reinterpret_cast<U8_T *>(_data))[c] = PIXEL_##TYPE##_TO_U8(in); \
    break; \
  \
  case U10: \
  { \
    U10_S * p = reinterpret_cast<U10_S *>(_data); \
    switch (c) \
    { \
      case 0: p->r = PIXEL_##TYPE##_TO_U10(in); break; \
      case 1: p->g = PIXEL_##TYPE##_TO_U10(in); break; \
      case 2: p->b = PIXEL_##TYPE##_TO_U10(in); break; \
    } \
  } \
  break; \
  \
  case U16: \
    (reinterpret_cast<U16_T *>(_data))[c] = PIXEL_##TYPE##_TO_U16(in); \
    break; \
  \
  case F32: \
    (reinterpret_cast<F32_T *>(_data))[c] = PIXEL_##TYPE##_TO_F32(in); \
    break;

#define _COLOR_SET_WORK2(TYPE) \
  \
  case F16: \
    (reinterpret_cast<F16_T *>(_data))[c] = PIXEL_##TYPE##_TO_F16(in); \
    break;

#if defined (DJV_OPENEXR)

#define _COLOR_SET_WORK(TYPE) \
  \
  switch (pixel::type(_pixel)) \
  { \
    _COLOR_SET_WORK1(TYPE) \
    _COLOR_SET_WORK2(TYPE) \
    default: break; \
  }

#else

#define _COLOR_SET_WORK(TYPE) \
  \
  switch (pixel::type(_pixel)) \
  { \
    _COLOR_SET_WORK1(TYPE) \
    default: break; \
  }

#endif

#define _COLOR_SET(NAME, TYPE) \
  \
  inline void Color::set_##NAME(pixel::TYPE##_T in, int c) \
  { \
    using namespace pixel; \
    \
    if (-1 == c) \
    { \
      for (c = 0; c < _channels; ++c) \
      { \
        _COLOR_SET_WORK(TYPE) \
      } \
    } \
    else \
    { \
      DJV_ASSERT(c >= 0 && c < pixel::channels(_pixel)); \
      \
      _COLOR_SET_WORK(TYPE) \
    } \
  }

_COLOR_SET( u8,  U8)
_COLOR_SET(u10, U10)
_COLOR_SET(u16, U16)
#if defined(DJV_OPENEXR)
_COLOR_SET(f16, F16)
#endif
_COLOR_SET(f32, F32)

//------------------------------------------------------------------------------
// Color::get()
//------------------------------------------------------------------------------

#define _COLOR_GET1(TYPE) \
  \
  case U8: \
    return PIXEL_U8_TO_##TYPE( \
      (reinterpret_cast<const U8_T *>(_data))[c]); \
  \
  case U10: \
  { \
    const U10_S * p = reinterpret_cast<const U10_S *>(_data); \
    switch (c) \
    { \
      case 0: return PIXEL_U10_TO_##TYPE(p->r); \
      case 1: return PIXEL_U10_TO_##TYPE(p->g); \
      case 2: return PIXEL_U10_TO_##TYPE(p->b); \
    } \
  } \
  break; \
  \
  case U16: \
    return PIXEL_U16_TO_##TYPE( \
      (reinterpret_cast<const U16_T *>(_data))[c]); \
  \
  case F32: \
    return PIXEL_F32_TO_##TYPE( \
      (reinterpret_cast<const F32_T *>(_data))[c]);

#define _COLOR_GET2(TYPE) \
  \
  case F16: \
    return PIXEL_F16_TO_##TYPE( \
      (reinterpret_cast<const F16_T *>(_data))[c]);

#if defined(DJV_OPENEXR)

#define _COLOR_GET(NAME, TYPE) \
  \
  inline pixel::TYPE##_T Color::get_##NAME(int c) const \
  { \
    using namespace pixel; \
    \
    DJV_ASSERT(c >= 0 && c < pixel::channels(_pixel)); \
    \
    switch (pixel::type(_pixel)) \
    { \
      _COLOR_GET1(TYPE) \
      _COLOR_GET2(TYPE) \
      default: break; \
    } \
    return 0; \
  }

#else

#define _COLOR_GET(NAME, TYPE) \
  \
  inline pixel::TYPE##_T Color::get_##NAME(int c) const \
  { \
    using namespace pixel; \
    \
    DJV_ASSERT(c >= 0 && c < pixel::channels(_pixel)); \
    \
    switch (pixel::type(_pixel)) \
    { \
      _COLOR_GET1(TYPE) \
      default: break; \
    } \
    return 0; \
  }

#endif

_COLOR_GET( u8,  U8)
_COLOR_GET(u10, U10)
_COLOR_GET(u16, U16)
#if defined(DJV_OPENEXR)
_COLOR_GET(f16, F16)
#endif
_COLOR_GET(f32, F32)

//------------------------------------------------------------------------------
// rgb_to_hsv(), hsv_to_rgb()
//------------------------------------------------------------------------------

inline void rgb_to_hsv(const double in [3], double out [3])
{
  const double & min = math::min(in[0], math::min(in[1], in[2]));
  const double & max = math::max(in[0], math::max(in[1], in[2]));
  out[2] = max;
  out[1] = max != 0.0 ? (1.0 - min / max) : 0.0;
  const double v = (max - min) * 6.0;
  
  if (max == min)
  {
    out[0] = 0.0;
  }
  else if (in[0] == max)
  {
    if (in[1] >= in[2])
      out[0] = (in[1] - in[2]) / v;
    else
      out[0] = 1.0 + (in[1] - in[2]) / v;
  }
  else if (in[1] == max)
  {
    out[0] = 1.0 / 3.0 + (in[2] - in[0]) / v;
  }
  else if (in[2] == max)
  {
    out[0] = 2.0 / 3.0 + (in[0] - in[1]) / v;
  }
}

inline void hsv_to_rgb(const double in [3], double out [3])
{
  const double h = math::mod(in[0] * 6.0, 6.0);
  const int i = math::floor(h);
  const double f = h - i;
  const double p = in[2] * (1.0 - in[1]);
  const double q = in[2] * (1.0 - (in[1] * f));
  const double t = in[2] * (1.0 - (in[1] * (1.0 - f)));
  
  switch (i)
  {
    case 0: out[0] = in[2]; out[1] = t; out[2] = p; break;
    case 1: out[0] = q; out[1] = in[2]; out[2] = p; break;
    case 2: out[0] = p; out[1] = in[2]; out[2] = t; break;
    case 3: out[0] = p; out[1] = q; out[2] = in[2]; break;
    case 4: out[0] = t; out[1] = p; out[2] = in[2]; break;
    case 5: out[0] = in[2]; out[1] = p; out[2] = q; break;
  }
}

//------------------------------------------------------------------------------

}
}

