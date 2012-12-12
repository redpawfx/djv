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

//! \file djv_lut.cpp

#include <djv_lut.h>

#include <djv_color.h>

#include <stdio.h>

namespace djv_lut
{

const List<String> & label_format()
{
    static const List<String> data = List<String>() <<
        "Auto" <<
        "Inferno" <<
        "Kodak";

    DJV_ASSERT(data.size() == _FORMAT_SIZE);

    return data;
}

const List<String> & label_type()
{
    static const List<String> data = List<String>() <<
        "Auto" <<
        "U8" <<
        "U10" <<
        "U16";

    DJV_ASSERT(data.size() == _TYPE_SIZE);

    return data;
}

namespace
{

int _bit_depth(File_Io & io) throw (Error)
{
    int out = 0;

    const size_t position = io.position();

    char tmp [cstring_size] = "";

    while (1)
    {
        try
        {
            File_Io::word(io, tmp, cstring_size);
        }
        catch (Error)
        {
            break;
        }

        out = Math::max(
            String_Util::string_to_int<int>(tmp, cstring_size), out);
    }

    io.position(position);

    if (out <= Pixel::u8_max)
    {
        return 8;
    }
    else if (out <= Pixel::u10_max)
    {
        return 10;
    }

    return 16;
}

} // namespace

void inferno_open(File_Io & io, Pixel_Data_Info & info, TYPE type) throw (Error)
{
    //DJV_DEBUG("inferno_open");

    // Header.

    char tmp [cstring_size] = "";
    File_Io::word(io, tmp, cstring_size);

    //DJV_DEBUG_PRINT("magic = " << tmp);

    if (String(tmp) != "LUT:")
    {
        Image_Io_Base::throw_error_unrecognized(name, io.file_name());
    }

    File_Io::word(io, tmp, cstring_size);
    const int channels = String_Util::string_to_int<int>(tmp, cstring_size);

    File_Io::word(io, tmp, cstring_size);
    const int size = String_Util::string_to_int<int>(tmp, cstring_size);

    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("channels = " << channels);

    // Information.

    info.size = V2i(size, 1);

    int bit_depth = 0;

    switch (type)
    {
        case TYPE_AUTO:
            bit_depth = _bit_depth(io);
            break;

        case TYPE_U8:
            bit_depth = 8;
            break;

        case TYPE_U10:
            bit_depth = 10;
            break;

        case TYPE_U16:
            bit_depth = 16;
            break;

        default:
            break;
    }

    //DJV_DEBUG_PRINT("bit depth = " << bit_depth);

    if (! Pixel::pixel_(channels, bit_depth, false, &info.pixel))
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }
}

void inferno_open(File_Io & io, const Pixel_Data_Info & info) throw (Error)
{
    char tmp [cstring_size] = "";
    int size = SNPRINTF(tmp, cstring_size, "LUT: %d %d\n\n",
        Pixel::channels(info.pixel), info.size.x);

    io.set(tmp, size);
}

void kodak_open(File_Io & io, Pixel_Data_Info & info, TYPE type) throw (Error)
{
    //DJV_DEBUG("kodak_open");

    // Header.

    const size_t position = io.position();
    String header;
    int8_t c = 0;
    bool comment = false;

    while (1)
    {
        try
        {
            io.get_8(&c);
        }
        catch (Error)
        {
            break;
        }

        if ('#' == c)
        {
            comment = true;
        }

        else if ('\n' == c)
        {
            if (comment)
            {
                comment = false;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (! comment)
            {
                header += c;
            }
        }
    }

    //DJV_DEBUG_PRINT("header = " << header);

    int channels = static_cast<int>(
        String_Util::split(header, List<char>() << ' ' << '\t').size());
    int size = 1;
    comment = false;

    while (1)
    {
        try
        {
            io.get_8(&c);
        }
        catch (Error)
        {
            break;
        }

        if ('\n' == c)
        {
            ++size;
        }
    }

    io.position(position);

    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("channels = " << channels);

    // Information.

    info.size = V2i(size, 1);

    int bit_depth = 0;

    switch (type)
    {
        case TYPE_AUTO:
            bit_depth = _bit_depth(io);
            break;

        case TYPE_U8:
            bit_depth = 8;
            break;

        case TYPE_U10:
            bit_depth = 10;
            break;

        case TYPE_U16:
            bit_depth = 16;
            break;

        default:
            break;
    }

    //DJV_DEBUG_PRINT("bit depth = " << bit_depth);

    if (! Pixel::pixel_(channels, bit_depth, true, &info.pixel))
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }
}

void kodak_open(File_Io &, const Pixel_Data_Info &) throw (Error)
{}

void inferno_load(File_Io & io, Image * out) throw (Error)
{
    //DJV_DEBUG("inferno_load");

    List<Color> color(out->w());

    for (int x = 0; x < out->w(); ++x)
    {
        color[x].pixel(out->pixel());
    }

    for (int c = 0; c < Pixel::channels(out->pixel()); ++c)
        for (int x = 0; x < out->w(); ++x)
        {
            char tmp [cstring_size] = "";
            File_Io::word(io, tmp, cstring_size);
            const int v = String_Util::string_to_int<int>(tmp, cstring_size);

            switch (Pixel::type(out->pixel()))
            {
                case Pixel::U8:
                    color[x].set_u8(v, c);
                    break;

                case Pixel::U10:
                    color[x].set_u10(v, c);
                    break;

                case Pixel::U16:
                    color[x].set_u16(v, c);
                    break;

                default:
                    break;
            }
        }

    for (int x = 0; x < out->w(); ++x)
    {
        //DJV_DEBUG_PRINT(x << " = " << color[x]);
        Memory::copy(
            color[x].data(),
            out->data(x, 0),
            Pixel::bytes(out->pixel()));
    }
}

void kodak_load(File_Io & io, Image * out) throw (Error)
{
    //DJV_DEBUG("kodak_load");

    for (int x = 0; x < out->w(); ++x)
    {
        Color color(out->pixel());

        for (int c = 0; c < Pixel::channels(out->pixel()); ++c)
        {
            char tmp [cstring_size] = "";
            File_Io::word(io, tmp, cstring_size);
            const int v = String_Util::string_to_int<int>(tmp, cstring_size);

            switch (Pixel::type(out->pixel()))
            {
                case Pixel::U8:
                    color.set_u8(v, c);
                    break;

                case Pixel::U10:
                    color.set_u10(v, c);
                    break;

                case Pixel::U16:
                    color.set_u16(v, c);
                    break;

                default:
                    break;
            }
        }

        //DJV_DEBUG_PRINT(x << " = " << color);

        Memory::copy(
            color.data(),
            out->data(x, 0),
            Pixel::bytes(out->pixel()));
    }
}

void inferno_save(File_Io & io, const Pixel_Data * out) throw (Error)
{
    List<Color> color(out->w());

    for (int x = 0; x < out->w(); ++x)
    {
        color[x].pixel(out->pixel());
        
        Memory::copy(
            out->data(x, 0),
            color[x].data(),
            Pixel::bytes(out->pixel()));
    }

    for (int c = 0; c < Pixel::channels(out->pixel()); ++c)
        for (int x = 0; x < out->w(); ++x)
        {
            int v = 0;

            switch (Pixel::type(out->pixel()))
            {
                case Pixel::U8:
                    v = color[x].get_u8(c);
                    break;

                case Pixel::U10:
                    v = color[x].get_u10(c);
                    break;

                case Pixel::U16:
                    v = color[x].get_u16(c);
                    break;

                default:
                    break;
            }

            char tmp [cstring_size] = "";
            int size = SNPRINTF(tmp, cstring_size, "%9d\n", v);

            io.set(tmp, size);
        }
}

void kodak_save(File_Io & io, const Pixel_Data * out) throw (Error)
{
    for (int x = 0; x < out->w(); ++x)
    {
        Color color(out->pixel());
        
        Memory::copy(out->data(x, 0), color.data(), Pixel::bytes(out->pixel()));

        for (int c = 0; c < Pixel::channels(out->pixel()); ++c)
        {
            int v = 0;

            switch (Pixel::type(out->pixel()))
            {
                case Pixel::U8:
                    v = color.get_u8(c);
                    break;

                case Pixel::U10:
                    v = color.get_u10(c);
                    break;

                case Pixel::U16:
                    v = color.get_u16(c);
                    break;

                default:
                    break;
            }

            char tmp [cstring_size] = "";
            int size = SNPRINTF(tmp, cstring_size, "%6d", v);
            io.set(tmp, size);
        }

        io.set_8('\n');
    }
}

_DJV_STRING_OPERATOR_LABEL(FORMAT, label_format())
_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())

} // djv_lut
