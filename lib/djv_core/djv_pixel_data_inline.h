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

//! \file djv_pixel_data_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// Pixel_Data
//------------------------------------------------------------------------------

inline const Pixel_Data_Info & Pixel_Data::info() const
{
    return _info;
}

inline const V2i & Pixel_Data::size() const
{
    return _info.size;
}

inline Pixel_Data_Info::PROXY Pixel_Data::proxy() const
{
    return _info.proxy;
}

inline int Pixel_Data::w() const
{
    return _info.size.x;
}

inline int Pixel_Data::h() const
{
    return _info.size.y;
}

inline Pixel::PIXEL Pixel_Data::pixel() const
{
    return _info.pixel;
}

inline int Pixel_Data::channels() const
{
    return _channels;
}

bool Pixel_Data::is_valid() const
{
    return _p;
}

inline uint8_t * Pixel_Data::data()
{
    return _data();
}

inline const uint8_t * Pixel_Data::data() const
{
    return _p;
}

inline uint8_t * Pixel_Data::data(int x, int y)
{
    return _data() + (y * _info.size.x + x) * _bytes_pixel;
}

inline const uint8_t * Pixel_Data::data(int x, int y) const
{
    return _p + (y * _info.size.x + x) * _bytes_pixel;
}

inline size_t Pixel_Data::bytes_pixel() const
{
    return _bytes_pixel;
}

inline size_t Pixel_Data::bytes_scanline() const
{
    return _bytes_scanline;
}

inline size_t Pixel_Data::bytes_data() const
{
    return _bytes_data;
}

} // djv
