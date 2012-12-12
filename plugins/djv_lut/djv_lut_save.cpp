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

//! \file djv_lut_save.cpp

#include <djv_lut_save.h>

#include <djv_gl_image.h>

namespace djv_lut
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
    format(FORMAT_AUTO)
{}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Format";

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
    return djv_lut::name;
}

List<String> Save::extensions() const
{
    return djv_lut::extensions;
}

bool Save::option(const String & in, String * data)
{
    try
    {
        if (String_Util::compare_no_case(in, options()[FORMAT_OPTION]))
        {
            *data >> _options.format;
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

    if (String_Util::compare_no_case(in, options()[FORMAT_OPTION]))
    {
        out << _options.format;
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

    _file = in;

    if (info.seq.list.size() > 1)
    {
        _file.type(File::SEQ);
    }

    _info = Pixel_Data_Info();
    _info.size = V2i(info.size.x, 1);
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

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

void Save::_open(const File & in) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _io.open(in, File_Io::WRITE);

    _format = _options.format;

    if (FORMAT_AUTO == _format)
    {
        if (in.extension() == djv_lut::extensions[0])
        {
            _format = FORMAT_INFERNO;
        }
        else
        {
            _format = FORMAT_KODAK;
        }
    }

    switch (_format)
    {
        case FORMAT_INFERNO:
            inferno_open(_io, _info);
            break;

        case FORMAT_KODAK:
            kodak_open(_io, _info);
            break;

        default:
            break;
    }
}

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("frame = " << frame);

    // Open the file.

    _open(_file.get(frame.frame));

    // Convert the image.

    const Pixel_Data * p = &in;

    if (in.info() != _info)
    {
        //DJV_DEBUG_PRINT("convert = " << _image);

        _image.zero();

        Gl_Image::copy(in, _image);

        p = &_image;
    }

    // Write the file.

    switch (_format)
    {
        case FORMAT_INFERNO:
            inferno_save(_io, p);
            break;

        case FORMAT_KODAK:
            kodak_save(_io, p);
            break;

        default:
            break;
    }

    _io.close();
}

} // djv_lut

