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

//! \file djv_pixel.h

#ifndef DJV_IMAGE_PIXEL_H
#define DJV_IMAGE_PIXEL_H

#include <djv_debug.h>
#include <djv_string.h>

#include <half.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Pixel
//!
//! This struct provides support for image pixels.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Pixel
{
    //! Pixel format.

    enum FORMAT
    {
        L,     //!< Luminance
        LA,    //!< Luminance, alpha
        RGB,   //!< Red, green, blue
        RGBA,  //!< Red, green, blue, alpha

        _FORMAT_SIZE
    };

    //! Get the pixel format labels.

    static const List<String> & label_format();

    //! Pixel type.

    enum TYPE
    {
        U8,   //!< 8-bit unsigned
        U10,  //!< 10-bit unsigned
        U16,  //!< 16-bit unsigned
        F16,  //!< 16-bit floating-point
        F32,  //!< 32-bit floating-point

        _TYPE_SIZE
    };

    //! Get the pixel type labels.

    static const List<String> & label_type();

    typedef uint8_t   U8_T;
    typedef uint16_t U10_T;
    typedef uint16_t U16_T;
    typedef half     F16_T;
    typedef float    F32_T;

    //! 10-bit MSB data.

    struct U10_S_MSB
    {
        uint r: 10, g: 10, b: 10, pad: 2;
    };

    //! 10-bit LSB data.

    struct U10_S_LSB
    {
        uint pad: 2, b: 10, g: 10, r: 10;
    };

# if defined(DJV_MSB)
    typedef U10_S_MSB U10_S;
# else
    typedef U10_S_LSB U10_S;
# endif

    static const int channels_max = 4;
    static const int bytes_max    = 4;

    static const int u8_max  = 255;
    static const int u10_max = 1023;
    static const int u16_max = 65535;

    //! Pixel.

    enum PIXEL
    {
        L_U8,
        L_U16,
        L_F16,
        L_F32,
        LA_U8,
        LA_U16,
        LA_F16,
        LA_F32,
        RGB_U8,
        RGB_U10,
        RGB_U16,
        RGB_F16,
        RGB_F32,
        RGBA_U8,
        RGBA_U16,
        RGBA_F16,
        RGBA_F32,

        _PIXEL_SIZE
    };

    //! Get the pixel labels.

    static const List<String> & label_pixel();

    //! Get the pixel format.

    static inline FORMAT format(PIXEL);

    //! Get the format for the given number of channels.

    static inline FORMAT format(int channels);

    //! Get the pixel type.

    static inline TYPE type(PIXEL);

    //! Get the number of channels in a format.

    static inline int channels(FORMAT);

    //! Get the number of channels in a pixel.

    static inline int channels(PIXEL);

    //! Get the number of bytes in a channel.

    static inline int channel_bytes(PIXEL);

    //! Get the number of bytes in a pixel.

    static inline int bytes(PIXEL);

    //! Get the bit depth of a type.

    static inline int bit_depth(TYPE);

    //! Get the bit depth of a pixel.

    static inline int bit_depth(PIXEL);

    //! Get the maximum value a pixel can hold (1 is returned for floating point
    //! formats.

    static inline int max(PIXEL);

    //! Get a pixel from the format and type.

    static inline PIXEL pixel(FORMAT, TYPE);

    //! Convert a pixel to the given type.

    static inline PIXEL pixel(PIXEL, TYPE);

    //! Convert a pixel to the given type.

    static inline bool pixel(FORMAT, TYPE, PIXEL *);

    //! Get a pixel from the given information.
    //!
    //! \todo Pointer/bool casting confusion?

    static inline bool pixel_(
        int channels,
        int bit_depth,
        bool floating_point,
        PIXEL *);

    //! Get a pixel from the given information. If no pixel matches then -1
    //! is returned.

    static inline int pixel_(int channels, int bit_depth, bool floating_point);

    //! Convert type data.

    static inline U10_T u8_to_u10(U8_T);

    //! Convert type data.

    static inline U16_T u8_to_u16(U8_T);

    //! Convert type data.

    static inline F16_T u8_to_f16(U8_T);

    //! Convert type data.

    static inline F32_T u8_to_f32(U8_T);

    //! Convert type data.

    static inline U8_T  u10_to_u8 (U10_T);

    //! Convert type data.

    static inline U16_T u10_to_u16(U10_T);

    //! Convert type data.

    static inline F16_T u10_to_f16(U10_T);

    //! Convert type data.

    static inline F32_T u10_to_f32(U10_T);

    //! Convert type data.

    static inline U8_T  u16_to_u8 (U16_T);

    //! Convert type data.

    static inline U10_T u16_to_u10(U16_T);

    //! Convert type data.

    static inline F16_T u16_to_f16(U16_T);

    //! Convert type data.

    static inline F32_T u16_to_f32(U16_T);

    //! Convert type data.

    static inline U8_T  f16_to_u8 (F16_T);

    //! Convert type data.

    static inline U10_T f16_to_u10(F16_T);

    //! Convert type data.

    static inline U16_T f16_to_u16(F16_T);

    //! Convert type data.

    static inline F32_T f16_to_f32(F16_T);

    //! Convert type data.

    static inline U8_T  f32_to_u8 (F32_T);

    //! Convert type data.

    static inline U10_T f32_to_u10(F32_T);

    //! Convert type data.

    static inline U16_T f32_to_u16(F32_T);

    //! Convert type data.

    static inline F16_T f32_to_f16(F32_T);

    //! Convert pixel data.

    static void convert(
        const void * in, PIXEL,
        void * out, PIXEL,
        int size = 1,
        int stride = 1,
        bool bgr = false);
};

//------------------------------------------------------------------------------

#define PIXEL_U8_ZERO  0
#define PIXEL_U10_ZERO 0
#define PIXEL_U16_ZERO 0
#define PIXEL_F16_ZERO 0.0
#define PIXEL_F32_ZERO 0.0

#define PIXEL_U8_ONE   Pixel::u8_max
#define PIXEL_U10_ONE  Pixel::u10_max
#define PIXEL_U16_ONE  Pixel::u16_max
#define PIXEL_F16_ONE  1.0
#define PIXEL_F32_ONE  1.0

#define PIXEL_U8_TO_U8(IN)   (IN)
#define PIXEL_U8_TO_U10(IN)  Pixel::u8_to_u10(IN)
#define PIXEL_U8_TO_U16(IN)  Pixel::u8_to_u16(IN)
#define PIXEL_U8_TO_F16(IN)  Pixel::u8_to_f16(IN)
#define PIXEL_U8_TO_F32(IN)  Pixel::u8_to_f32(IN)

#define PIXEL_U10_TO_U8(IN)  Pixel::u10_to_u8(IN)
#define PIXEL_U10_TO_U10(IN) (IN)
#define PIXEL_U10_TO_U16(IN) Pixel::u10_to_u16(IN)
#define PIXEL_U10_TO_F16(IN) Pixel::u10_to_f16(IN)
#define PIXEL_U10_TO_F32(IN) Pixel::u10_to_f32(IN)

#define PIXEL_U16_TO_U8(IN)  Pixel::u16_to_u8(IN)
#define PIXEL_U16_TO_U10(IN) Pixel::u16_to_u10(IN)
#define PIXEL_U16_TO_U16(IN) (IN)
#define PIXEL_U16_TO_F16(IN) Pixel::u16_to_f16(IN)
#define PIXEL_U16_TO_F32(IN) Pixel::u16_to_f32(IN)

#define PIXEL_F16_TO_U8(IN)  Pixel::f16_to_u8(IN)
#define PIXEL_F16_TO_U10(IN) Pixel::f16_to_u10(IN)
#define PIXEL_F16_TO_U16(IN) Pixel::f16_to_u16(IN)
#define PIXEL_F16_TO_F16(IN) (IN)
#define PIXEL_F16_TO_F32(IN) Pixel::f16_to_f32(IN)

#define PIXEL_F32_TO_U8(IN)  Pixel::f32_to_u8(IN)
#define PIXEL_F32_TO_U10(IN) Pixel::f32_to_u10(IN)
#define PIXEL_F32_TO_U16(IN) Pixel::f32_to_u16(IN)
#define PIXEL_F32_TO_F16(IN) Pixel::f32_to_f16(IN)
#define PIXEL_F32_TO_F32(IN) (IN)

DJV_CORE_EXPORT String & operator >> (String &, Pixel::FORMAT &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Pixel::TYPE &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Pixel::PIXEL &) throw (String);

DJV_CORE_EXPORT String & operator << (String &, Pixel::FORMAT);
DJV_CORE_EXPORT String & operator << (String &, Pixel::TYPE);
DJV_CORE_EXPORT String & operator << (String &, Pixel::PIXEL);

DJV_CORE_EXPORT Debug & operator << (Debug &, Pixel::FORMAT);
DJV_CORE_EXPORT Debug & operator << (Debug &, Pixel::TYPE);
DJV_CORE_EXPORT Debug & operator << (Debug &, Pixel::PIXEL);

} // djv

#include <djv_pixel_inline.h>

#endif // DJV_PIXEL_H

