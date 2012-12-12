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

//! \file djv_dpx_load.cpp

#include <djv_dpx_load.h>

#include <djv_dpx_header.h>

#include <djv_assert.h>

namespace djv_dpx
{

//------------------------------------------------------------------------------
// djv_image_load
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_load()
{
    return new Load;
}

}

//------------------------------------------------------------------------------
// Load::Options
//------------------------------------------------------------------------------

Load::Options::Options() :
    color_profile(djv_cineon::COLOR_PROFILE_AUTO),
    convert      (djv_cineon::CONVERT_NONE)
{}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

const List<String> & Load::label_options()
{
    static const List<String> data = List<String>() <<
        "Color Profile" <<
        "Film Print" <<
        "Convert";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Load::Load() :
    _film_print(false)
{}

Plugin * Load::copy() const
{
    Load * out = new Load;
    out->_options = _options;
    return out;
}

String Load::name() const
{
    return djv_dpx::name;
}

List<String> Load::extensions() const
{
    return djv_dpx::extensions;
}

bool Load::option(const String & in, String * data)
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
        else if (String_Util::compare_no_case(in, options()[CONVERT_OPTION]))
        {
            *data >> _options.convert;
        }
    }
    catch (String)
    {
        return false;
    }

    return true;
}

String Load::option(const String & in) const
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
    else if (String_Util::compare_no_case(in, options()[CONVERT_OPTION]))
    {
        out << _options.convert;
    }

    return out;
}

List<String> Load::options() const
{
    return label_options();
}

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _file = in;
    
    File_Io io;
    
    _open(_file.get(_file.seq().start()), info, io);

    if (File::SEQ == _file.type())
    {
        info.seq.list = _file.seq().list;
    }
}

void Load::_open(const String & in, Image_Io_Info & info, File_Io & io)
    throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    io.open(in, File_Io::READ);

    info.file_name = in;

    _film_print = false;

    Header::info_load(io, info, _film_print);

    //DJV_DEBUG_PRINT("info = " << info);
    //DJV_DEBUG_PRINT("film print = " << _film_print);
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::get");
    //DJV_DEBUG_PRINT("frame = " << frame);

    // Open the file.

    const String file_name =
        _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info;
    
    std::auto_ptr<File_Io> io(new File_Io);
    
    _open(file_name, info, *io);

    image.tag = info.tag;
    
    // Set the colo profile.

    if ((djv_cineon::COLOR_PROFILE_FILM_PRINT == _options.color_profile) ||
        (djv_cineon::COLOR_PROFILE_AUTO ==
            _options.color_profile && _film_print))
    {
        //DJV_DEBUG_PRINT("color profile");

        image.color_profile.type = Color_Profile::LUT;

        if (! _film_print_lut.is_valid())
        {
            _film_print_lut = film_unprint_lut(_options.film_print);
        }

        image.color_profile.lut = _film_print_lut;
    }
    else
    {
        image.color_profile = Color_Profile();
    }

    // Read the file.

    io->read_ahead();

    const uint8_t * p = io->mmap_p();

    io->seek(Pixel_Data::bytes_data(info));

    if (! frame.proxy && djv_cineon::CONVERT_NONE == _options.convert)
    {
        image.set(info, p, io.release());
    }
    else
    {
        _tmp.set(info, p, io.release());

        info.size = Pixel_Data::proxy_scale(info.size, frame.proxy);
        info.proxy = frame.proxy;

        switch (_options.convert)
        {
            case djv_cineon::CONVERT_NONE:
                break;

            case djv_cineon::CONVERT_U8:
                info.pixel = Pixel::pixel(info.pixel, Pixel::U8);
                //info.endian = Memory::endian();
                break;

            case djv_cineon::CONVERT_U16:
                info.pixel = Pixel::pixel(info.pixel, Pixel::U16);
                //info.endian = Memory::endian();
                break;

            default:
                break;
        }

        image.set(info);

        Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_dpx

