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

//! \file djv_font.cpp

#include <djv_font.h>

#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Font
//------------------------------------------------------------------------------

namespace
{

//! \todo Is this still necessary?

/*int char_average_size()
{
  double out = 0.0;

  int count = 0;
  for (int i = 32; i <= 126; ++i, ++count)
    out += fl_width(static_cast<uchar>(i));

  return Math::ceil(out / static_cast<double>(count));
}*/

int char_average_size()
{
    return Math::ceil(fl_width('0') * 0.9);
}

} // namespace

V2i Font::char_size()
{
    return V2i(char_average_size(), fl_height());
}

int Font::char_width()
{
    return char_average_size();
}

int Font::char_width(char in)
{
    return static_cast<int>(fl_width(in));
}

V2i Font::string_size(const String & in)
{
    //DJV_DEBUG("Font::string_size");
    //DJV_DEBUG_PRINT("in = " << in);

    const List<String> lines = String_Util::split(in, '\n', true);

    //DJV_DEBUG_PRINT("lines = " << lines.size());

    int width = 0;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        width = Math::max(string_width(lines[i]), width);
    }

    const V2i out(width, static_cast<int>(lines.size()) * fl_height());

    //DJV_DEBUG_PRINT("out = " << out);

    return out;
}

int Font::string_width(const String & in)
{
    return Math::ceil(fl_width(in.c_str()));
}

int Font::string_width(int char_size)
{
    return char_width() * char_size;
}

} // djv
