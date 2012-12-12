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

//! \file djv_dpx_save.cpp

#include <djv_dpx_save.h>

#include <djv_dpx_header.h>

#include <djv_gl_image.h>

namespace djv_dpx
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
    color_profile(djv_cineon::COLOR_PROFILE_FILM_PRINT),
    version      (VERSION_2_0),
    type         (TYPE_U10),
    endian       (ENDIAN_MSB)
{}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Color Profile" <<
        "Film Print" <<
        "Version" <<
        "Type" <<
        "Endian";

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
    return djv_dpx::name;
}

List<String> Save::extensions() const
{
    return djv_dpx::extensions;
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
        else if (String_Util::compare_no_case(in, options()[VERSION_OPTION]))
        {
            *data >> _options.version;
        }
        else if (String_Util::compare_no_case(in, options()[TYPE_OPTION]))
        {
            *data >> _options.type;
        }
        else if (String_Util::compare_no_case(in, options()[ENDIAN_OPTION]))
        {
            *data >> _options.endian;
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
    else if (String_Util::compare_no_case(in, options()[VERSION_OPTION]))
    {
        out << _options.version;
    }
    else if (String_Util::compare_no_case(in, options()[TYPE_OPTION]))
    {
        out << _options.type;
    }
    else if (String_Util::compare_no_case(in, options()[ENDIAN_OPTION]))
    {
        out << _options.endian;
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
    _info.mirror.y = true;

    switch (_options.endian)
    {
        case ENDIAN_AUTO:
            break;

        case ENDIAN_MSB:
            _info.endian = Memory::MSB;
            break;

        case ENDIAN_LSB:
            _info.endian = Memory::LSB;
            break;

        default:
            break;
    }

    switch (_options.type)
    {
        case TYPE_U10:
            _info.pixel = Pixel::RGB_U10;
            break;

        default:
        {
            Pixel::TYPE type = Pixel::type(info.pixel);

            switch (type)
            {
                case Pixel::F16:
                case Pixel::F32:
                    type = Pixel::U16;
                    break;

                default:
                    break;
            }

            _info.pixel = Pixel::pixel(Pixel::format(info.pixel), type);
        }
        break;
    }

    switch (Pixel::bit_depth(_info.pixel))
    {
        case 8:
        case 10:
            _info.align = 4;
            break;

        default:
            break;
    }

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

void Save::_open(const String & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _io.open(in, File_Io::WRITE);

    Header::info_save(
        _io,
        _info,
        _options.endian,
        _options.color_profile,
        _options.version);
}

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);

    // Set the color profile.

    Color_Profile color_profile;

    if (djv_cineon::COLOR_PROFILE_FILM_PRINT == _options.color_profile ||
        djv_cineon::COLOR_PROFILE_AUTO == _options.color_profile)
    {
        //DJV_DEBUG_PRINT("color profile");

        color_profile.type = Color_Profile::LUT;
        color_profile.lut  = film_print_lut(_options.film_print);
    }

    // Open the file.

    const String file_name = _file.get(frame.frame);

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info(_info);
    info.file_name = file_name;
    info.tag = in.tag;

    _open(file_name, info);

    // Convert the image.

    const Pixel_Data * p = &in;

    if (in.info() != _info ||
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

} // djv_dpx

