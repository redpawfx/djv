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

//! \file djv_color.cpp

#include <djv_color.h>

#include <djv_memory.h>

namespace djv
{

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------

void Color::init()
{
    _channels = Pixel::channels(_pixel);
}

Color::Color() :
    _pixel(Pixel::PIXEL(0))
{
    init();

    zero();
}

Color::Color(const Color & in) :
    _pixel(Pixel::PIXEL(0))
{
    init();

    *this = in;
}

Color::Color(Pixel::PIXEL pixel) :
    _pixel(pixel)
{
    init();

    zero();
}

Color::Color(Pixel::F32_T l) :
    _pixel(Pixel::L_F32)
{
    init();

    set_f32(l, 0);
}

Color::Color(Pixel::F32_T l, Pixel::F32_T a) :
    _pixel(Pixel::LA_F32)
{
    init();

    set_f32(l, 0);
    set_f32(a, 1);
}

Color::Color(Pixel::F32_T r, Pixel::F32_T g, Pixel::F32_T b) :
    _pixel(Pixel::RGB_F32)
{
    init();

    set_f32(r, 0);
    set_f32(g, 1);
    set_f32(b, 2);
}

Color::Color(Pixel::F32_T r, Pixel::F32_T g, Pixel::F32_T b, Pixel::F32_T a) :
    _pixel(Pixel::RGBA_F32)
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
        Memory::copy(in._data, _data, Pixel::bytes(_pixel));
    }

    return *this;
}

void Color::pixel(Pixel::PIXEL in)
{
    if (in == _pixel)
    {
        return;
    }

    Color tmp(in);
    Color::convert(*this, tmp);
    *this = tmp;
}

void Color::zero()
{
    Memory::set<uint8_t>(0, _data, Pixel::bytes(_pixel));
}

void Color::scale(double value, const Color & in, Color & out)
{
    Color in_(Pixel::pixel(Pixel::format(out.pixel()), Pixel::F32));
    convert(in, in_);

    const int channels = Pixel::channels(out.pixel());

    for (int c = 0; c < channels; ++c)
    {
        in_.set_f32(in_.get_f32(c) * static_cast<float>(value), c);
    }

    convert(in_, out);
}

Color Color::scale(double value, const Color & in)
{
    Color out(in.pixel());
    scale(value, in, out);
    return out;
}

void Color::lerp(
    double value,
    const Color & min,
    const Color & max,
    Color & out)
{
    Color min_(Pixel::pixel(Pixel::format(out.pixel()), Pixel::F32));
    Color max_(Pixel::pixel(Pixel::format(out.pixel()), Pixel::F32));
    convert(min, min_);
    convert(max, max_);

    const int channels = Pixel::channels(out.pixel());

    for (int c = 0; c < channels; ++c)
    {
        min_.set_f32(
            Math::lerp(value, min_.get_f32(c), max_.get_f32(c)),
            c);
    }

    convert(min_, out);
}

Color Color::lerp(double value, const Color & min, const Color & max)
{
    Color out(Math::max(min.pixel(), max.pixel()));
    lerp(value, min, max, out);
    return out;
}

void Color::convert(const Color & in, Color & out)
{
    Pixel::convert(in.data(), in.pixel(), out.data(), out.pixel());
}

//------------------------------------------------------------------------------

bool operator == (const Color & a, const Color & b)
{
    if (a.pixel() != b.pixel())
    {
        return false;
    }

    return Memory::compare(a.data(), b.data(), Pixel::bytes(a.pixel())) == 0;
}

bool operator != (const Color & a, const Color & b)
{
    return ! (a == b);
}

String & operator >> (String & in, Color & out) throw (String)
{
    Pixel::PIXEL pixel(Pixel::PIXEL(0));
    in >> pixel;
    out.pixel(pixel);

    const int channels = Pixel::channels(pixel);

    for (int c = 0; c < channels; ++c)
        switch (Pixel::type(pixel))
        {
            case Pixel::U8:
            {
                int value = 0;
                in >> value;
                out.set_u8(Math::clamp(value, 0, Pixel::u8_max), c);
            }
            break;

            case Pixel::U10:
            {
                int value = 0;
                in >> value;
                out.set_u10(Math::clamp(value, 0, Pixel::u10_max), c);
            }
            break;

            case Pixel::U16:
            {
                int value = 0;
                in >> value;
                out.set_u16(Math::clamp(value, 0, Pixel::u16_max), c);
            }
            break;

            case Pixel::F16:
            {
                double value = 0.0;
                in >> value;
                out.set_f16(static_cast<Pixel::F16_T>(value), c);
            }
            break;

            case Pixel::F32:
            {
                double value = 0.0;
                in >> value;
                out.set_f32(static_cast<Pixel::F32_T>(value), c);
            }
            break;

            default:
                break;
        }

    return in;
}

String & operator << (String & out, const Color & in)
{
    out << in.pixel();

    const int channels = Pixel::channels(in.pixel());

    for (int c = 0; c < channels; ++c)
        switch (Pixel::type(in.pixel()))
        {
            case Pixel::U8:
                out << in.get_u8(c);
                break;

            case Pixel::U10:
                out << in.get_u10(c);
                break;

            case Pixel::U16:
                out << in.get_u16(c);
                break;

            case Pixel::F16:
                out << in.get_f16(c);
                break;

            case Pixel::F32:
                out << in.get_f32(c);
                break;

            default:
                break;
        }

    return out;
}

Debug & operator << (Debug & debug, const Color & in)
{
    return debug << String_Util::label(in);
}

} // djv

