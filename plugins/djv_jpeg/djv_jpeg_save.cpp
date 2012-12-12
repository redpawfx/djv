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

//! \file djv_jpeg_save.cpp

#include <djv_jpeg_save.h>

#include <djv_gl_image.h>

namespace djv_jpeg
{
using libjpeg::jpeg_compress_struct;

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
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Quality";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Save::Save() :
    _f(0),
    _jpeg_init(false),
    _quality(100)
{}

Save::~Save()
{
    _close();
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_quality = _quality;
    return out;
}

String Save::name() const
{
    return djv_jpeg::name;
}

List<String> Save::extensions() const
{
    return djv_jpeg::extensions;
}

bool Save::option(const String & in, String * data)
{
    try
    {
        if (String_Util::compare_no_case(in, label_options()[QUALITY]))
        {
            *data >> _quality;
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

    if (String_Util::compare_no_case(in, label_options()[QUALITY]))
    {
        out << _quality;
    }

    return out;
}

List<String> Save::options() const
{
    return List<String>() << label_options()[QUALITY];
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

    _info = Image_Io_Info();
    _info.size = info.size;

    Pixel::FORMAT format = Pixel::format(info.pixel);

    switch (format)
    {
        case Pixel::LA:
            format = Pixel::L;
            break;

        case Pixel::RGBA:
            format = Pixel::RGB;
            break;

        default:
            break;
    }

    _info.pixel = Pixel::pixel(format, Pixel::U8);

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

namespace
{

bool jpeg_init(jpeg_compress_struct * jpeg, Jpeg_Error * error)
{
    if (setjmp(error->jump))
    {
        return false;
    }

    libjpeg::jpeg_create_compress(jpeg);

    return true;
}

bool jpeg_open(
    FILE *                 f,
    jpeg_compress_struct * jpeg,
    const Image_Io_Info &  info,
    int                    quality,
    Jpeg_Error *           error)
{
    if (setjmp(error->jump))
    {
        return false;
    }

    libjpeg::jpeg_stdio_dest(jpeg, f);

    jpeg->image_width = info.size.x;
    jpeg->image_height = info.size.y;

    if (Pixel::L_U8 == info.pixel)
    {
        jpeg->input_components = 1;
        jpeg->in_color_space = libjpeg::JCS_GRAYSCALE;
    }
    else if (Pixel::RGB_U8 == info.pixel)
    {
        jpeg->input_components = 3;
        jpeg->in_color_space = libjpeg::JCS_RGB;
    }

    libjpeg::jpeg_set_defaults(jpeg);
    libjpeg::jpeg_set_quality(jpeg, quality, TRUE);
    libjpeg::jpeg_start_compress(jpeg, TRUE);

    String _tag = info.tag[Image_Tag::label_tag()[Image_Tag::DESCRIPTION]];

    if (_tag.size())
    {
        libjpeg::jpeg_write_marker(
            jpeg,
            JPEG_COM,
            (libjpeg::JOCTET *)_tag.c_str(),
            static_cast<uint>(_tag.size()));
    }

    return true;
}

} // namespace

void Save::_open(const String & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    // Initialize libjpeg.

    _jpeg.err = libjpeg::jpeg_std_error(&_jpeg_error.pub);
    _jpeg_error.pub.error_exit = djv_jpeg_error;
    _jpeg_error.pub.emit_message = djv_jpeg_warning;
    _jpeg_error.msg[0] = 0;

    if (! jpeg_init(&_jpeg, &_jpeg_error))
    {
        throw Error(name(), _jpeg_error.msg);
    }

    _jpeg_init = true;

    // Open the file.

#   if defined(DJV_WINDOWS)

    //! \todo MinGW does not define fopen_s()?
    //::fopen_s(&_f, in.c_str(), "wb");
    _f = ::fopen(in.c_str(), "wb");

#   else

    _f = ::fopen(in.c_str(), "wb");

#   endif

    if (! _f)
    {
        throw_error_open(name(), in);
    }

    if (! jpeg_open(_f, &_jpeg, info, _quality, &_jpeg_error))
    {
        throw Error(name(), _jpeg_error.msg);
    }
}

void Save::_close()
{
    if (_jpeg_init)
    {
        libjpeg::jpeg_destroy_compress(&_jpeg);
        
        _jpeg_init = false;
    }

    if (_f)
    {
        ::fclose(_f);
        
        _f = 0;
    }
}

namespace
{

bool jpeg_scanline(
    libjpeg::jpeg_compress_struct * jpeg,
    const uint8_t *                 in,
    Jpeg_Error *                    error)
{
    if (::setjmp(error->jump))
    {
        return false;
    }

    libjpeg::JSAMPROW p [] = { (libjpeg::JSAMPLE *)(in) };

    if (! libjpeg::jpeg_write_scanlines(jpeg, p, 1))
    {
        return false;
    }

    return true;
}

bool jpeg_end(libjpeg::jpeg_compress_struct * jpeg, Jpeg_Error * error)
{
    if (::setjmp(error->jump))
    {
        return false;
    }

    libjpeg::jpeg_finish_compress(jpeg);

    return true;
}

} // namespace

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open the file.

    const String file_name = _file.get(frame.frame);

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info(_info);
    info.tag = in.tag;

    _open(file_name, info);

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

    const int h = p->h();

    for (int y = 0; y < h; ++y)
        if (! jpeg_scanline(&_jpeg, p->data(0, h - 1 - y), &_jpeg_error))
        {
            throw Error(name(), _jpeg_error.msg);
        }

    if (! jpeg_end(&_jpeg, &_jpeg_error))
    {
        throw Error(name(), _jpeg_error.msg);
    }

    _close();
}

} // djv_jpeg

