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

//! \file djv_color_test.cpp

#include <djv_color.h>

using namespace djv;

void convert()
{
    const struct Data
    {
        String in;
        Pixel::PIXEL pixel;
        String out;
    }
    data [] =
    {
        { "l u8 0", Pixel::L_U8, "l u8 0" },
        { "l u8 255", Pixel::L_U8, "l u8 255" },
        { "l u8 0", Pixel::RGB_U10, "rgb u10 0 0 0" },
        { "l u8 255", Pixel::RGB_U10, "rgb u10 1023 1023 1023" },
        { "l u8 0", Pixel::L_U16, "l u16 0" },
        { "l u8 255", Pixel::L_U16, "l u16 65535" },
        { "l u16 0", Pixel::L_U8, "l u8 0" },
        { "l u16 65535", Pixel::L_U8, "l u8 255" },
        { "l u16 0", Pixel::RGB_U10, "rgb u10 0 0 0" },
        { "l u16 65535", Pixel::RGB_U10, "rgb u10 1023 1023 1023" },
        { "l u16 0", Pixel::L_U16, "l u16 0" },
        { "l u16 65535", Pixel::L_U16, "l u16 65535" },
        { "l f32 -1.0", Pixel::L_U8, "l u8 0" },
        { "l f32 0.0", Pixel::L_U8, "l u8 0" },
        { "l f32 0.5", Pixel::L_U8, "l u8 128" },
        { "l f32 1.0", Pixel::L_U8, "l u8 255" },
        { "l f32 2.0", Pixel::L_U8, "l u8 255" },
        { "l f32 -1.0", Pixel::L_U16, "l u16 0" },
        { "l f32 0.0", Pixel::L_U16, "l u16 0" },
        { "l f32 0.5", Pixel::L_U16, "l u16 32768" },
        { "l f32 1.0", Pixel::L_U16, "l u16 65535" },
        { "l f32 2.0", Pixel::L_U16, "l u16 65535" }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        Color in;
        String tmp = data[i].in;
        tmp >> in;

        Color out(data[i].pixel);
        Color::convert(in, out);

        DJV_ASSERT(
            String_Util::compare_no_case(String_Util::label(out), data[i].out));
    }
}

template<typename T>
inline String label2(const T & in)
{
    List<String> out;

    String value;
    value << in;

    try
    {
        while (value.size())
        {
            String tmp;
            value >> tmp;
            out += tmp;
        }
    }
    catch (String) {}

    return String_Util::join(out, ' ');
}

int main(int argc, char ** argv)
{
    convert();
    
    return 0;
}

