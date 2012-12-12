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

//! \file djv_cineon_save.cpp

#include <djv_cineon_save.h>

#include <djv_cineon_header.h>

#include <djv_gl_image.h>

namespace djv_cineon
{

//------------------------------------------------------------------------------
// djv_image_save
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_save()
{
    return new Save;
}

}

//------------------------------------------------------------------------------
// Save::Options
//------------------------------------------------------------------------------

Save::Options::Options() :
    color_profile(COLOR_PROFILE_FILM_PRINT)
{}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Color Profile" <<
        "Film Print";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_options = _options;
    return out;
}

String Save::name() const
{
    return djv_cineon::name;
}

List<String> Save::extensions() const
{
    return djv_cineon::extensions;
}

bool Save::option(const String & in, String * data)
{
    try
    {
        if (String_Util::compare_no_case(in, options()[COLOR_PROFILE_OPTION]))
        {
            *data >> _options.color_profile;
        }
        else if (String_Util::compare_no_case(in, options()[FILM_PRINT_OPTION]))
        {
            *data >> _options.film_print;
        }
    }
    catch (String)
    {
        return false;
    }

    return true;
}

String Save::option(const String & in) const
{
    String out;

    if (String_Util::compare_no_case(in, options()[COLOR_PROFILE_OPTION]))
    {
        out << _options.color_profile;
    }
    else if (String_Util::compare_no_case(in, options()[FILM_PRINT_OPTION]))
    {
        out << _options.film_print;
    }

    return out;
}

List<String> Save::options() const
{
    return label_options();
}

void Save::open(const File & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("info = " << info);

    _file = in;

    if (info.seq.list.size() > 1)
    {
        _file.type(File::SEQ);
    }

    _info = Pixel_Data_Info();
    _info.size = info.size;
    _info.pixel = Pixel::RGB_U10;
    _info.mirror.y = true;
    _info.endian = Memory::MSB;
    _info.align = 4;

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

void Save::_open(const String & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _io.open(in, File_Io::WRITE);

    Header::info_save(_io, info, _options.color_profile);
}

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("frame = " << frame);

    // Set the color profile.

    Color_Profile color_profile;

    if (
        COLOR_PROFILE_FILM_PRINT == _options.color_profile ||
        COLOR_PROFILE_AUTO == _options.color_profile)
    {
        //DJV_DEBUG_PRINT("color profile");

        color_profile.type = Color_Profile::LUT;
        color_profile.lut = film_print_lut(_options.film_print);
    }

    // Open the file.

    const String file_name = _file.get(frame.frame);

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info(_info);
    info.file_name = file_name;
    info.tag = in.tag;

    _open(file_name, info);

    // Convert.

    const Pixel_Data * p = &in;

    if (
        in.info() != _info ||
        in.color_profile.type != Color_Profile::RAW ||
        color_profile.type != Color_Profile::RAW)
    {
        //DJV_DEBUG_PRINT("convert = " << _image);

        _image.zero();

        Gl_Image_Options options;
        options.color_profile = color_profile;
        Gl_Image::copy(*p, _image, options);

        p = &_image;
    }

    // Write the file.

    _io.set(p->data(), p->bytes_data());

    Header::info_update(_io);

    _io.close();
}

} // djv_cineon

