//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_color_inline.h

#include <djv_assert.h>

namespace djv
{

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------

inline Pixel::PIXEL Color::pixel() const
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

#define _COLOR_SET_WORK(TYPE) \
  \
  switch (Pixel::type(_pixel)) \
  { \
    \
    case Pixel::U8: \
      (reinterpret_cast<Pixel::U8_T *>(_data))[c] = PIXEL_##TYPE##_TO_U8(in); \
      break; \
    \
    case Pixel::U10: \
    { \
      Pixel::U10_S * p = reinterpret_cast<Pixel::U10_S *>(_data); \
      switch (c) \
      { \
        case 0: p->r = PIXEL_##TYPE##_TO_U10(in); break; \
        case 1: p->g = PIXEL_##TYPE##_TO_U10(in); break; \
        case 2: p->b = PIXEL_##TYPE##_TO_U10(in); break; \
      } \
    } \
    break; \
    \
    case Pixel::U16: \
      (reinterpret_cast<Pixel::U16_T *>(_data))[c] = \
        PIXEL_##TYPE##_TO_U16(in); \
      break; \
    \
    case Pixel::F16: \
      (reinterpret_cast<Pixel::F16_T *>(_data))[c] = \
        PIXEL_##TYPE##_TO_F16(in); \
      break; \
    \
    case Pixel::F32: \
      (reinterpret_cast<Pixel::F32_T *>(_data))[c] = \
        PIXEL_##TYPE##_TO_F32(in); \
      break; \
    \
    default: break; \
  }

#define _COLOR_SET(NAME, TYPE) \
  \
  inline void Color::set_##NAME(Pixel::TYPE##_T in, int c) \
  { \
    if (-1 == c) \
    { \
      for (c = 0; c < _channels; ++c) \
      { \
        _COLOR_SET_WORK(TYPE) \
      } \
    } \
    else \
    { \
      DJV_ASSERT(c >= 0 && c < Pixel::channels(_pixel)); \
      \
      _COLOR_SET_WORK(TYPE) \
    } \
  }

_COLOR_SET( u8,  U8)
_COLOR_SET(u10, U10)
_COLOR_SET(u16, U16)
_COLOR_SET(f16, F16)
_COLOR_SET(f32, F32)

#define _COLOR_GET(NAME, TYPE) \
  \
  inline Pixel::TYPE##_T Color::get_##NAME(int c) const \
  { \
    DJV_ASSERT(c >= 0 && c < Pixel::channels(_pixel)); \
    \
    switch (Pixel::type(_pixel)) \
    { \
      \
      case Pixel::U8: \
        return PIXEL_U8_TO_##TYPE( \
          (reinterpret_cast<const Pixel::U8_T *>(_data))[c]); \
      \
      case Pixel::U10: \
      { \
        const Pixel::U10_S * p = \
          reinterpret_cast<const Pixel::U10_S *>(_data); \
        switch (c) \
        { \
          case 0: return PIXEL_U10_TO_##TYPE(p->r); \
          case 1: return PIXEL_U10_TO_##TYPE(p->g); \
          case 2: return PIXEL_U10_TO_##TYPE(p->b); \
        } \
      } \
      break; \
      \
      case Pixel::U16: \
        return PIXEL_U16_TO_##TYPE( \
          (reinterpret_cast<const Pixel::U16_T *>(_data))[c]); \
      \
      case Pixel::F16: \
        return PIXEL_F16_TO_##TYPE( \
          (reinterpret_cast<const Pixel::F16_T *>(_data))[c]); \
      \
      case Pixel::F32: \
        return PIXEL_F32_TO_##TYPE( \
          (reinterpret_cast<const Pixel::F32_T *>(_data))[c]); \
      \
      default: break; \
    } \
    return 0; \
  }

_COLOR_GET( u8,  U8)
_COLOR_GET(u10, U10)
_COLOR_GET(u16, U16)
_COLOR_GET(f16, F16)
_COLOR_GET(f32, F32)

inline void Color::rgb_to_hsv(const double in [3], double out [3])
{
    const double & min = Math::min(in[0], Math::min(in[1], in[2]));
    const double & max = Math::max(in[0], Math::max(in[1], in[2]));
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
        {
            out[0] = (in[1] - in[2]) / v;
        }
        else
        {
            out[0] = 1.0 + (in[1] - in[2]) / v;
        }
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

inline void Color::hsv_to_rgb(const double in [3], double out [3])
{
    const double h = Math::mod(in[0] * 6.0, 6.0);
    const int i = Math::floor(h);
    const double f = h - i;
    const double p = in[2] * (1.0 - in[1]);
    const double q = in[2] * (1.0 - (in[1] * f));
    const double t = in[2] * (1.0 - (in[1] * (1.0 - f)));

    switch (i)
    {
        case 0:
            out[0] = in[2];
            out[1] = t;
            out[2] = p;
            break;

        case 1:
            out[0] = q;
            out[1] = in[2];
            out[2] = p;
            break;

        case 2:
            out[0] = p;
            out[1] = in[2];
            out[2] = t;
            break;

        case 3:
            out[0] = p;
            out[1] = q;
            out[2] = in[2];
            break;

        case 4:
            out[0] = t;
            out[1] = p;
            out[2] = in[2];
            break;

        case 5:
            out[0] = in[2];
            out[1] = p;
            out[2] = q;
            break;
    }
}

} // djv

