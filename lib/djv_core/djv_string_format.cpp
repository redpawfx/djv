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

//! \file djv_string_format.cpp

#include <djv_string.h>

#include <djv_debug.h>
#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
// String_Format
//------------------------------------------------------------------------------

String_Format::String_Format(const String & in) :
    _value(in)
{}

String_Format String_Format::arg(const String & in, int width, char fill) const
{
    //DJV_DEBUG("String_Format::arg");
    //DJV_DEBUG_PRINT("value = " << _value);
    //DJV_DEBUG_PRINT("in = " << in << " (" << in.size() << ")");
    //DJV_DEBUG_PRINT("width = " << width);
    //DJV_DEBUG_PRINT("fill = " << fill);

    // Find argument string "%%".

    int size = static_cast<int>(_value.size()), index = -1;

    for (int i = 0; i < size - 1; ++i)
        if ('%' == _value[i] && '%' == _value[i + 1])
        {
            index = i;
            break;
        }

    if (-1 == index)
    {
        return _value;
    }

    //DJV_DEBUG_PRINT("index = " << index);

    // Format input.

    String tmp = in;

    if (width)
    {
        int w = (width < 0 ? -width : width) - static_cast<int>(in.size());

        if (w < 0)
        {
            w = 0;
        }

        if (width < 0)
        {
            tmp += String(w, fill);
        }
        else
        {
            tmp.insert(tmp.begin(), w, fill);
        }
    }

    // Replace.

    const String out = String(_value, 0, index) + tmp +
                       String(_value, index + 2, _value.size() - (index + 2));
    //DJV_DEBUG_PRINT("out = " << out);
    return out;
}

String_Format String_Format::arg(
    const List<String> & in,
    const String & separator,
    int width,
    char fill) const
{
    return arg(String_Util::join(in, separator), width, fill);
}

String_Format String_Format::arg(int in, int width, char fill) const
{
    return arg(String_Util::int_to_string(in), width, fill);
}

String_Format String_Format::arg(int64_t in, int width, char fill) const
{
    return arg(String_Util::int_to_string(in), width, fill);
}

String_Format String_Format::arg(uint in, int width, char fill) const
{
    return arg(String_Util::int_to_string(in), width, fill);
}

String_Format String_Format::arg(uint64_t in, int width, char fill) const
{
    return arg(String_Util::int_to_string(in), width, fill);
}

String_Format String_Format::arg(
    double in,
    int width,
    int precision,
    char fill) const
{
    return arg(String_Util::float_to_string(in, precision), width, fill);
}

String_Format::operator String () const
{
    return _value;
}

} // djv
