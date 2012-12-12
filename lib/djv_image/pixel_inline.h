/*------------------------------------------------------------------------------
 include/djv_image/pixel_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include <djv_base/math.h>

namespace djv_image {
namespace pixel {

//------------------------------------------------------------------------------
// format()
//------------------------------------------------------------------------------

inline FORMAT format(PIXEL in)
{
#if defined(DJV_OPENEXR)

  static const FORMAT data [] =
  {
    L,          L,    L,    L,
    LA,         LA,   LA,   LA,
    RGB,  RGB,  RGB,  RGB,  RGB,
    RGBA,       RGBA, RGBA, RGBA
  };

#else

  static const FORMAT data [] =
  {
    L,          L,    L,
    LA,         LA,   LA,
    RGB,  RGB,  RGB,  RGB,
    RGBA,       RGBA, RGBA
  };

#endif
  
  return data[in];
}

inline FORMAT format(int channels)
{
  FORMAT out = static_cast<FORMAT>(0);
  switch (channels)
  {
    case 1: out = L; break;
    case 2: out = LA; break;
    case 3: out = RGB; break;
    case 4: out = RGBA; break;
  }
  
  return out;
}

//------------------------------------------------------------------------------
// type()
//------------------------------------------------------------------------------

inline TYPE type(PIXEL in)
{
#if defined(DJV_OPENEXR)

  static const TYPE data [] =
  {
    U8,      U16, F16, F32,
    U8,      U16, F16, F32,
    U8, U10, U16, F16, F32,
    U8,      U16, F16, F32
  };

#else

  static const TYPE data [] =
  {
    U8,      U16, F32,
    U8,      U16, F32,
    U8, U10, U16, F32,
    U8,      U16, F32
  };

#endif

  return data[in];
}

//------------------------------------------------------------------------------
// pixel()
//------------------------------------------------------------------------------

inline PIXEL pixel(FORMAT format, TYPE type)
{
  PIXEL out = static_cast<PIXEL>(0);
  
  switch (format)
  {
    case L:
      switch (type)
      {
        case U8:  out = L_U8; break;
        case U10:
        case U16: out = L_U16; break;
#if defined(DJV_OPENEXR)
        case F16: out = L_F16; break;
#endif
        case F32: out = L_F32; break;
        default: break;
      }
      break;

    case LA:
      switch (type)
      {
        case U8:  out = LA_U8; break;
        case U10:
        case U16: out = LA_U16; break;
#if defined(DJV_OPENEXR)
        case F16: out = LA_F16; break;
#endif
        case F32: out = LA_F32; break;
        default: break;
      }
      break;

    case RGB:
      switch (type)
      {
        case U8:  out = RGB_U8; break;
        case U10: out = RGB_U10; break;
        case U16: out = RGB_U16; break;
#if defined(DJV_OPENEXR)
        case F16: out = RGB_F16; break;
#endif
        case F32: out = RGB_F32; break;
        default: break;
      }
      break;

    case RGBA:
      switch (type)
      {
        case U8:  out = RGBA_U8; break;
        case U10:
        case U16: out = RGBA_U16; break;
#if defined(DJV_OPENEXR)
        case F16: out = RGBA_F16; break;
#endif
        case F32: out = RGBA_F32; break;
        default: break;
      }
      break;

    default: break;
  }
  
  return out;
}

inline PIXEL pixel(PIXEL in, TYPE type)
{
  return pixel(format(in), type);
}

inline bool pixel(FORMAT format, TYPE type, PIXEL * out)
{
  switch (format)
  {
    case L:
      switch (type)
      {
        case U8:  *out = L_U8; break;
        case U16: *out = L_U16; break;
#if defined(DJV_OPENEXR)
        case F16: *out = L_F16; break;
#endif
        case F32: *out = L_F32; break;
        default: return false;
      }
      break;

    case LA:
      switch (type)
      {
        case U8:  *out = LA_U8; break;
        case U16: *out = LA_U16; break;
#if defined(DJV_OPENEXR)
        case F16: *out = LA_F16; break;
#endif
        case F32: *out = LA_F32; break;
        default: return false;
      }
      break;

    case RGB:
      switch (type)
      {
        case U8:  *out = RGB_U8; break;
        case U10: *out = RGB_U10; break;
        case U16: *out = RGB_U16; break;
#if defined(DJV_OPENEXR)
        case F16: *out = RGB_F16; break;
#endif
        case F32: *out = RGB_F32; break;
        default: return false;
      }
      break;

    case RGBA:
      switch (type)
      {
        case U8:  *out = RGBA_U8; break;
        case U16: *out = RGBA_U16; break;
#if defined(DJV_OPENEXR)
        case F16: *out = RGBA_F16; break;
#endif
        case F32: *out = RGBA_F32; break;
        default: return false;
      }
      break;

    default: return false;
  }
  return true;
}

//------------------------------------------------------------------------------
// pixel_()
//------------------------------------------------------------------------------

inline bool pixel_(int channels, int bit_depth, bool fp, PIXEL * out)
{
  switch (channels)
  {
    case 1:
      switch (bit_depth)
      {
        case 8:  *out = L_U8; break;
        case 16: *out = L_U16; break;
        case 32: *out = L_F32; break;
        default: return false;
      }
      break;

    case 2:
      switch (bit_depth)
      {
        case 8:  *out = LA_U8; break;
        case 16: *out = LA_U16; break;
        case 32: *out = LA_F32; break;
        default: return false;
      }
      break;

    case 3:
      switch (bit_depth)
      {
        case 8:  *out = RGB_U8; break;
        case 10: *out = RGB_U10; break;
        case 16: *out = RGB_U16; break;
        case 32: *out = RGB_F32; break;
        default: return false;
      }
      break;

    case 4:
      switch (bit_depth)
      {
        case 8:  *out = RGBA_U8; break;
        case 16: *out = RGBA_U16; break;
        case 32: *out = RGBA_F32; break;
        default: return false;
      }
      break;

    default: return false;
  }
  return true;
}

inline int pixel_(int channels, int bit_depth, bool fp)
{
  PIXEL tmp = static_cast<PIXEL>(0);

  if (! pixel_(channels, bit_depth, fp, &tmp))
    return -1;

    return tmp;
}

//------------------------------------------------------------------------------
// channels(), channel_bytes(), bytes(), bit_depth(), max()
//------------------------------------------------------------------------------

inline int channels(FORMAT in)
{
  static const int data [] = { 1, 2, 3, 4 };
  
  return data[in];
}

inline int channels(PIXEL in)
{
#if defined(DJV_OPENEXR)

  static const int data [] =
  {
    1, 1,    1, 1,
    2, 2,    2, 2,
    3, 3, 3, 3, 3,
    4, 4,    4, 4
  };

#else

  static const int data [] =
  {
    1, 1,    1,
    2, 2,    2,
    3, 3, 3, 3,
    4, 4,    4
  };

#endif

  return data[in];
}

inline int channel_bytes(PIXEL in)
{
#if defined(DJV_OPENEXR)

  static const int data [] =
  {
    1, 2,    2, 4,
    1, 2,    2, 4,
    1, 0, 2, 2, 4,
    1, 2,    2, 4
  };

#else

  static const int data [] =
  {
    1, 2,    4,
    1, 2,    4,
    1, 0, 2, 4,
    1, 2,    4
  };

#endif

  return data[in];
}

inline int bytes(PIXEL in)
{
#if defined(DJV_OPENEXR)

  static const int data [] =
  {
    1, 2,    2, 4,
    2, 4,    4, 8,
    3, 4, 6, 6, 12,
    4, 8,    8, 16
  };

#else

  static const int data [] =
  {
    1, 2,    4,
    2, 4,    8,
    3, 4, 6, 12,
    4, 8,    16
  };

#endif

  return data[in];
}

inline int bit_depth(TYPE in)
{
#if defined(DJV_OPENEXR)

  static const int data [] =
  {
    8, 10, 16, 16, 32
  };

#else

  static const int data [] =
  {
    8, 10, 16, 32
  };

#endif

  return data[in];
}

inline int bit_depth(PIXEL in)
{
#if defined(DJV_OPENEXR)

  static const int data [] =
  {
    8,     16, 16, 32,
    8,     16, 16, 32,
    8, 10, 16, 16, 32,
    8,     16, 16, 32
  };
  
#else

  static const int data [] =
  {
    8,     16, 32,
    8,     16, 32,
    8, 10, 16, 32,
    8,     16, 32
  };
  
#endif

  return data[in];
}

inline int max(PIXEL in)
{
#if defined(DJV_OPENEXR)

  static const int data [] =
  {
    u8_max,          u16_max, 1, 1,
    u8_max,          u16_max, 1, 1,
    u8_max, u10_max, u16_max, 1, 1,
    u8_max,          u16_max, 1, 1
  };
  
#else

  static const int data [] =
  {
    u8_max,          u16_max, 1,
    u8_max,          u16_max, 1,
    u8_max, u10_max, u16_max, 1,
    u8_max,          u16_max, 1
  };
  
#endif

  return data[in];
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

// XXX Use a LUT because shifting doesn't preserve max values?

#define _PIXEL_U8_MAX  u8_max
#define _PIXEL_U10_MAX u10_max
#define _PIXEL_U16_MAX u16_max

#define _PIXEL_LUT(IN, OUT, IN_MAX, OUT_MAX) \
  \
  static OUT##_T lut [_PIXEL_##IN##_MAX + 1]; \
  static bool init = true; \
  if (init) \
  { \
    for (int i = 0; i <= IN_MAX; ++i) \
      lut[i] = OUT##_T(i / static_cast<float>(IN_MAX) * OUT_MAX); \
    init = false; \
  } \
  return lut[in];

inline U10_T u8_to_u10(U8_T in)
{
  //return in << 2;
  _PIXEL_LUT(U8, U10, u8_max, u10_max)
}

inline U16_T u8_to_u16(U8_T in)
{
  //return in << 8;
  _PIXEL_LUT(U8, U16, u8_max, u16_max)
}

#if defined(DJV_OPENEXR)

inline F16_T u8_to_f16(U8_T in)
{
  //return in / F16(u8_max);
  _PIXEL_LUT(U8, F16, u8_max, 1.0f)
}

#endif

inline F32_T u8_to_f32(U8_T in)
{
  //return in / F32(u8_max);
  _PIXEL_LUT(U8, F32, u8_max, 1.0f)
}

inline U8_T u10_to_u8(U10_T in)
{
  return in >> 2;
}

inline U16_T u10_to_u16(U10_T in)
{
  //return in << 6;
  _PIXEL_LUT(U10, U16, u10_max, u16_max)
}

#if defined(DJV_OPENEXR)

inline F16_T u10_to_f16(U10_T in)
{
  //return in / F16(u10_max);
  _PIXEL_LUT(U10, F16, u10_max, 1.0f)
}

#endif

inline F32_T u10_to_f32(U10_T in)
{
  //return in / F32(u10_max);
  _PIXEL_LUT(U10, F32, u10_max, 1.0f)
}

inline U8_T u16_to_u8(U16_T in)
{
  return in >> 8;
}

inline U10_T u16_to_u10(U16_T in)
{
  return in >> 6;
}

#if defined(DJV_OPENEXR)

inline F16_T u16_to_f16(U16_T in)
{
  //return in / F16(u16_max);
  _PIXEL_LUT(U16, F16, u16_max, 1.0f);
}

#endif

inline F32_T u16_to_f32(U16_T in)
{
  //return in / F32(u16_max);
  _PIXEL_LUT(U16, F32, u16_max, 1.0f)
}

#if defined(DJV_OPENEXR)

inline U8_T f16_to_u8(F16_T in)
{
  return math::clamp(static_cast<int>(in * u8_max + 0.5), 0, u8_max);
}

inline U10_T f16_to_u10(F16_T in)
{
  return math::clamp(static_cast<int>(in * u10_max + 0.5), 0, u10_max);
}

inline U16_T f16_to_u16(F16_T in)
{
  return math::clamp(static_cast<int>(in * u16_max + 0.5), 0, u16_max);
}

inline F32_T f16_to_f32(F16_T in)
{
  return in;
}

#endif

inline U8_T f32_to_u8(F32_T in)
{
  //DJV_DEBUG("f32_to_u8");
  //DJV_DEBUG_PRINT("in = " << in);
  
  return math::clamp(static_cast<int>(in * u8_max + 0.5), 0, u8_max);

  /*// XXX Ill floating point conversion. Assumes IEEE floats: sign (1 bit)
  // + exponent (8 bits) + mantissa (23 bits).
  //
  // References:
  //
  // * http://www.stereopsis.com/FPU.html

  // Transform to get a constant exponent of 127 for 0.0 to 1.0 range.

  const F32_T tmp = in + 1.0;
  const uint32_t bits = *(const uint32_t *)&tmp;
  //DJV_DEBUG_PRINT(_bits(tmp));
  //DJV_DEBUG_PRINT(_bits(bits));

  // Bail on negative numbers.

  if (bits >> 31) return 0;

  // Exponent:
  //
  // * < 127 - Input <= 0.0.
  // * 127 - Input between 0.0 and 1.0. Mantissa becomes 23 bit integer.
  // * > 127 - Input >= 1.0.

  const uint8_t exp = (bits >> 23) & 0xff;
  //DJV_DEBUG_PRINT("exp = " << exp);
  if (127 == exp) return (bits >> 15) & 0xff;
  else if (exp > 127) return u8_max;
  else return 0;*/
}

inline U10_T f32_to_u10(F32_T in)
{
  return math::clamp(static_cast<int>(in * u10_max + 0.5), 0, u10_max);
}

inline U16_T f32_to_u16(F32_T in)
{
  return math::clamp(static_cast<int>(in * u16_max + 0.5), 0, u16_max);
}

#if defined(DJV_OPENEXR)

inline F16_T f32_to_f16(F32_T in)
{
  return in;
}

#endif

//------------------------------------------------------------------------------

}
}

