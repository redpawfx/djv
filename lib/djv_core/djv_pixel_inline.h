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

//! \file djv_pixel_inline.h

#include <djv_math.h>

namespace djv
{

//------------------------------------------------------------------------------
// Pixel
//------------------------------------------------------------------------------

inline Pixel::FORMAT Pixel::format(PIXEL in)
{
    static const FORMAT data [] =
    {
        L,          L,    L,    L,
        LA,         LA,   LA,   LA,
        RGB,  RGB,  RGB,  RGB,  RGB,
        RGBA,       RGBA, RGBA, RGBA
    };

    return data[in];
}

inline Pixel::FORMAT Pixel::format(int channels)
{
    FORMAT out = static_cast<FORMAT>(0);

    switch (channels)
    {
        case 1:
            out = L;
            break;

        case 2:
            out = LA;
            break;

        case 3:
            out = RGB;
            break;

        case 4:
            out = RGBA;
            break;
    }

    return out;
}

inline Pixel::TYPE Pixel::type(PIXEL in)
{
    static const TYPE data [] =
    {
        U8,      U16, F16, F32,
        U8,      U16, F16, F32,
        U8, U10, U16, F16, F32,
        U8,      U16, F16, F32
    };

    return data[in];
}

inline Pixel::PIXEL Pixel::pixel(FORMAT format, TYPE type)
{
    PIXEL out = static_cast<PIXEL>(0);

    switch (format)
    {
        case L:
            switch (type)
            {
                case U8:
                    out = L_U8;
                    break;

                case U10:
                case U16:
                    out = L_U16;
                    break;

                case F16:
                    out = L_F16;
                    break;

                case F32:
                    out = L_F32;
                    break;

                default:
                    break;
            }

            break;

        case LA:
            switch (type)
            {
                case U8:
                    out = LA_U8;
                    break;

                case U10:
                case U16:
                    out = LA_U16;
                    break;

                case F16:
                    out = LA_F16;
                    break;

                case F32:
                    out = LA_F32;
                    break;

                default:
                    break;
            }

            break;

        case RGB:
            switch (type)
            {
                case U8:
                    out = RGB_U8;
                    break;

                case U10:
                    out = RGB_U10;
                    break;

                case U16:
                    out = RGB_U16;
                    break;

                case F16:
                    out = RGB_F16;
                    break;

                case F32:
                    out = RGB_F32;
                    break;

                default:
                    break;
            }

            break;

        case RGBA:
            switch (type)
            {
                case U8:
                    out = RGBA_U8;
                    break;

                case U10:
                case U16:
                    out = RGBA_U16;
                    break;

                case F16:
                    out = RGBA_F16;
                    break;

                case F32:
                    out = RGBA_F32;
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    return out;
}

inline Pixel::PIXEL Pixel::pixel(PIXEL in, TYPE type)
{
    return pixel(format(in), type);
}

inline bool Pixel::pixel(FORMAT format, TYPE type, PIXEL * out)
{
    switch (format)
    {
        case L:
            switch (type)
            {
                case U8:
                    *out = L_U8;
                    break;

                case U16:
                    *out = L_U16;
                    break;

                case F16:
                    *out = L_F16;
                    break;

                case F32:
                    *out = L_F32;
                    break;

                default:
                    return false;
            }

            break;

        case LA:
            switch (type)
            {
                case U8:
                    *out = LA_U8;
                    break;

                case U16:
                    *out = LA_U16;
                    break;

                case F16:
                    *out = LA_F16;
                    break;

                case F32:
                    *out = LA_F32;
                    break;

                default:
                    return false;
            }

            break;

        case RGB:
            switch (type)
            {
                case U8:
                    *out = RGB_U8;
                    break;

                case U10:
                    *out = RGB_U10;
                    break;

                case U16:
                    *out = RGB_U16;
                    break;

                case F16:
                    *out = RGB_F16;
                    break;

                case F32:
                    *out = RGB_F32;
                    break;

                default:
                    return false;
            }

            break;

        case RGBA:
            switch (type)
            {
                case U8:
                    *out = RGBA_U8;
                    break;

                case U16:
                    *out = RGBA_U16;
                    break;

                case F16:
                    *out = RGBA_F16;
                    break;

                case F32:
                    *out = RGBA_F32;
                    break;

                default:
                    return false;
            }

            break;

        default:
            return false;
    }

    return true;
}

inline bool Pixel::pixel_(int channels, int bit_depth, bool fp, PIXEL * out)
{
    switch (channels)
    {
        case 1:
            switch (bit_depth)
            {
                case 8:
                    *out = L_U8;
                    break;

                case 16:
                    *out = L_U16;
                    break;

                case 32:
                    *out = L_F32;
                    break;

                default:
                    return false;
            }

            break;

        case 2:
            switch (bit_depth)
            {
                case 8:
                    *out = LA_U8;
                    break;

                case 16:
                    *out = LA_U16;
                    break;

                case 32:
                    *out = LA_F32;
                    break;

                default:
                    return false;
            }

            break;

        case 3:
            switch (bit_depth)
            {
                case 8:
                    *out = RGB_U8;
                    break;

                case 10:
                    *out = RGB_U10;
                    break;

                case 16:
                    *out = RGB_U16;
                    break;

                case 32:
                    *out = RGB_F32;
                    break;

                default:
                    return false;
            }

            break;

        case 4:
            switch (bit_depth)
            {
                case 8:
                    *out = RGBA_U8;
                    break;

                case 16:
                    *out = RGBA_U16;
                    break;

                case 32:
                    *out = RGBA_F32;
                    break;

                default:
                    return false;
            }

            break;

        default:
            return false;
    }

    return true;
}

inline int Pixel::pixel_(int channels, int bit_depth, bool fp)
{
    PIXEL tmp = static_cast<PIXEL>(0);

    if (! pixel_(channels, bit_depth, fp, &tmp))
    {
        return -1;
    }

    return tmp;
}

inline int Pixel::channels(FORMAT in)
{
    static const int data [] = { 1, 2, 3, 4 };

    return data[in];
}

inline int Pixel::channels(PIXEL in)
{
    static const int data [] =
    {
        1, 1,    1, 1,
        2, 2,    2, 2,
        3, 3, 3, 3, 3,
        4, 4,    4, 4
    };

    return data[in];
}

inline int Pixel::channel_bytes(PIXEL in)
{
    static const int data [] =
    {
        1, 2,    2, 4,
        1, 2,    2, 4,
        1, 0, 2, 2, 4,
        1, 2,    2, 4
    };

    return data[in];
}

inline int Pixel::bytes(PIXEL in)
{
    static const int data [] =
    {
        1, 2,    2, 4,
        2, 4,    4, 8,
        3, 4, 6, 6, 12,
        4, 8,    8, 16
    };

    return data[in];
}

inline int Pixel::bit_depth(TYPE in)
{
    static const int data [] =
    {
        8, 10, 16, 16, 32
    };

    return data[in];
}

inline int Pixel::bit_depth(PIXEL in)
{
    static const int data [] =
    {
        8,     16, 16, 32,
        8,     16, 16, 32,
        8, 10, 16, 16, 32,
        8,     16, 16, 32
    };

    return data[in];
}

inline int Pixel::max(PIXEL in)
{
    static const int data [] =
    {
        u8_max,          u16_max, 1, 1,
        u8_max,          u16_max, 1, 1,
        u8_max, u10_max, u16_max, 1, 1,
        u8_max,          u16_max, 1, 1
    };

    return data[in];
}

//! \todo Use a LUT because shifting doesn't preserve max values?

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

inline Pixel::U10_T Pixel::u8_to_u10(U8_T in)
{
    //return in << 2;
    _PIXEL_LUT(U8, U10, u8_max, u10_max)
}

inline Pixel::U16_T Pixel::u8_to_u16(U8_T in)
{
    //return in << 8;
    _PIXEL_LUT(U8, U16, u8_max, u16_max)
}

inline Pixel::F16_T Pixel::u8_to_f16(U8_T in)
{
    //return in / F16(u8_max);
    _PIXEL_LUT(U8, F16, u8_max, 1.0f)
}

inline Pixel::F32_T Pixel::u8_to_f32(U8_T in)
{
    //return in / F32(u8_max);
    _PIXEL_LUT(U8, F32, u8_max, 1.0f)
}

inline Pixel::U8_T Pixel::u10_to_u8(U10_T in)
{
    return in >> 2;
}

inline Pixel::U16_T Pixel::u10_to_u16(U10_T in)
{
    //return in << 6;
    _PIXEL_LUT(U10, U16, u10_max, u16_max)
}

inline Pixel::F16_T Pixel::u10_to_f16(U10_T in)
{
    //return in / F16(u10_max);
    _PIXEL_LUT(U10, F16, u10_max, 1.0f)
}

inline Pixel::F32_T Pixel::u10_to_f32(U10_T in)
{
    //return in / F32(u10_max);
    _PIXEL_LUT(U10, F32, u10_max, 1.0f)
}

inline Pixel::U8_T Pixel::u16_to_u8(U16_T in)
{
    return in >> 8;
}

inline Pixel::U10_T Pixel::u16_to_u10(U16_T in)
{
    return in >> 6;
}

inline Pixel::F16_T Pixel::u16_to_f16(U16_T in)
{
    //return in / F16(u16_max);
    _PIXEL_LUT(U16, F16, u16_max, 1.0f);
}

inline Pixel::F32_T Pixel::u16_to_f32(U16_T in)
{
    //return in / F32(u16_max);
    _PIXEL_LUT(U16, F32, u16_max, 1.0f)
}

inline Pixel::U8_T Pixel::f16_to_u8(F16_T in)
{
    return Math::clamp(static_cast<int>(in * u8_max + 0.5), 0, u8_max);
}

inline Pixel::U10_T Pixel::f16_to_u10(F16_T in)
{
    return Math::clamp(static_cast<int>(in * u10_max + 0.5), 0, u10_max);
}

inline Pixel::U16_T Pixel::f16_to_u16(F16_T in)
{
    return Math::clamp(static_cast<int>(in * u16_max + 0.5), 0, u16_max);
}

inline Pixel::F32_T Pixel::f16_to_f32(F16_T in)
{
    return in;
}

inline Pixel::U8_T Pixel::f32_to_u8(F32_T in)
{
    //DJV_DEBUG("f32_to_u8");
    //DJV_DEBUG_PRINT("in = " << in);

    return Math::clamp(static_cast<int>(in * u8_max + 0.5), 0, u8_max);

    /*//! \todo Ill floating point conversion. Assumes IEEE floats: sign (1 bit)
    //! + exponent (8 bits) + mantissa (23 bits).
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

inline Pixel::U10_T Pixel::f32_to_u10(F32_T in)
{
    return Math::clamp(static_cast<int>(in * u10_max + 0.5), 0, u10_max);
}

inline Pixel::U16_T Pixel::f32_to_u16(F32_T in)
{
    return Math::clamp(static_cast<int>(in * u16_max + 0.5), 0, u16_max);
}

inline Pixel::F16_T Pixel::f32_to_f16(F32_T in)
{
    return in;
}

} // djv

