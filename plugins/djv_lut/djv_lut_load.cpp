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

//! \file djv_lut_load.cpp

#include <djv_lut_load.h>

#include <djv_assert.h>

namespace djv_lut
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
    format(FORMAT_AUTO),
    type  (TYPE_AUTO)
{}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

const List<String> & Load::label_options()
{
    static const List<String> data = List<String>() <<
        "Format" <<
        "Type";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Plugin * Load::copy() const
{
    Load * out = new Load;
    out->_options = _options;
    return out;
}

String Load::name() const
{
    return djv_lut::name;
}

List<String> Load::extensions() const
{
    return djv_lut::extensions;
}

bool Load::option(const String & in, String * data)
{
    try
    {
        if (String_Util::compare_no_case(in, options()[FORMAT_OPTION]))
        {
            *data >> _options.format;
        }
        else if (String_Util::compare_no_case(in, options()[TYPE_OPTION]))
        {
            *data >> _options.type;
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

    if (String_Util::compare_no_case(in, options()[FORMAT_OPTION]))
    {
        out << _options.format;
    }
    else if (String_Util::compare_no_case(in, options()[TYPE_OPTION]))
    {
        out << _options.type;
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
    _open(_file.get(_file.seq().start()), info);

    if (File::SEQ == _file.type())
    {
        info.seq.list = _file.seq().list;
    }
}

void Load::_open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _io.open(in, File_Io::READ);

    info.file_name = in;

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
            inferno_open(_io, info, _options.type);
            break;

        case FORMAT_KODAK:
            kodak_open(_io, info, _options.type);
            break;

        default:
            break;
    }
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::load");
    //DJV_DEBUG_PRINT("frame = " << frame);

    image.color_profile = Color_Profile();
    image.tag = Image_Tag();

    // Open the file.

    const String file_name =
        _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());

    Image_Io_Info info;
    
    _open(file_name, info);

    // Read the file.

    image.set(info);

    switch (_format)
    {
        case FORMAT_INFERNO:
            inferno_load(_io, &image);
            break;

        case FORMAT_KODAK:
            kodak_load(_io, &image);
            break;

        default:
            break;
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_lut
