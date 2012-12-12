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

//! \file djv_color.h

#ifndef DJV_COLOR_H
#define DJV_COLOR_H

#include <djv_pixel.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Color
//!
//! This class provides a color.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Color
{
public:

    //! Constructor.

    Color();

    //! Constructor.

    Color(const Color &);

    //! Constructor.

    Color(Pixel::PIXEL);

    //! Constructor.

    Color(Pixel::F32_T l);

    //! Constructor.

    Color(Pixel::F32_T l, Pixel::F32_T a);

    //! Constructor.

    Color(Pixel::F32_T r, Pixel::F32_T g, Pixel::F32_T b);

    //! Constructor.

    Color(Pixel::F32_T r, Pixel::F32_T g, Pixel::F32_T b, Pixel::F32_T a);

    Color & operator = (const Color &);

    //! Set the pixel.

    void pixel(Pixel::PIXEL);

    //! Get the pixel.

    inline Pixel::PIXEL pixel() const;

    //! Get the channel couunt.

    inline int channels() const;

    //! Set a channel.

    inline void set_u8(Pixel::U8_T, int);

    //! Set a channel.

    inline void set_u10(Pixel::U10_T, int);

    //! Set a channel.

    inline void set_u16(Pixel::U16_T, int);

    //! Set a channel.

    inline void set_f16(Pixel::F16_T, int);

    //! Set a channel.

    inline void set_f32(Pixel::F32_T, int);

    //! Get a channel.

    inline Pixel::U8_T get_u8(int) const;

    //! Get a channel.

    inline Pixel::U10_T get_u10(int) const;

    //! Get a channel.

    inline Pixel::U16_T get_u16(int) const;

    //! Get a channel.

    inline Pixel::F16_T get_f16(int) const;

    //! Get a channel.

    inline Pixel::F32_T get_f32(int) const;

    //! Zero the channels.

    void zero();

    //! Get a pointer to the color data.

    inline uint8_t * data();

    //! Get a pointer to the color data.

    inline const uint8_t * data() const;

    //! Scale a color.

    static void  scale(double, const Color &, Color &);

    //! Scale a color.

    static Color scale(double, const Color &);

    //! Interpolate a color.

    static void lerp(double, const Color &, const Color &, Color &);

    //! Interpolate a color.

    static Color lerp(double, const Color &, const Color &);

    //! Convert a color.

    static void convert(const Color &, Color &);

    //! Convert RGB to HSV.

    static inline void rgb_to_hsv(const double [3], double [3]);

    //! Convert HSV to RGB.

    static inline void hsv_to_rgb(const double [3], double [3]);

private:

    void init();

    Pixel::PIXEL _pixel;

    int _channels;

    uint8_t _data [Pixel::channels_max * Pixel::bytes_max];
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT bool operator == (const Color &, const Color &);

DJV_CORE_EXPORT bool operator != (const Color &, const Color &);

DJV_CORE_EXPORT String & operator >> (String &, Color &) throw (String);

DJV_CORE_EXPORT String & operator << (String &, const Color &);

DJV_CORE_EXPORT Debug & operator << (Debug &, const Color &);

} // djv

#include <djv_color_inline.h>

#endif // DJV_COLOR_H

