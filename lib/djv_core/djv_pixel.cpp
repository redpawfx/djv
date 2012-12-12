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

//! \file djv_pixel.cpp

#include <djv_pixel.h>

#include <djv_assert.h>

namespace djv
{

//------------------------------------------------------------------------------
// Pixel
//------------------------------------------------------------------------------

const List<String> & Pixel::label_format()
{
    static const List<String> data = List<String>() <<
        "L" <<
        "LA" <<
        "RGB" <<
        "RGBA";

    DJV_ASSERT(data.size() == _FORMAT_SIZE);

    return data;
}

const List<String> & Pixel::label_type()
{
    static const List<String> data = List<String>() <<
        "U8" <<
        "U10" <<
        "U16" <<
        "F16" <<
        "F32";

    DJV_ASSERT(data.size() == _TYPE_SIZE);

    return data;
}

const List<String> & Pixel::label_pixel()
{
    static const List<String> data = List<String>() <<
        "L U8" <<
        "L U16" <<
        "L F16" <<
        "L F32" <<
        "LA U8" <<
        "LA U16" <<
        "LA F16" <<
        "LA F32" <<
        "RGB U8" <<
        "RGB U10" <<
        "RGB U16" <<
        "RGB F16" <<
        "RGB F32" <<
        "RGBA U8" <<
        "RGBA U16" <<
        "RGBA F16" <<
        "RGBA F32";

    DJV_ASSERT(data.size() == _PIXEL_SIZE);

    return data;
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Pixel::FORMAT, Pixel::label_format())
_DJV_STRING_OPERATOR_LABEL(Pixel::TYPE, Pixel::label_type())
_DJV_STRING_OPERATOR_LABEL(Pixel::PIXEL, Pixel::label_pixel())

Debug & operator << (Debug & debug, Pixel::FORMAT in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, Pixel::TYPE in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, Pixel::PIXEL in)
{
    return debug << String_Util::label(in);
}

} // djv
