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

//! \file djv_png_save.cpp

#include <djv_png_save.h>

#include <djv_gl_image.h>

namespace djv_png
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
// Save
//------------------------------------------------------------------------------

Save::Save() :
    _f       (0),
    _png     (0),
    _png_info(0)
{}

Save::~Save()
{
    _close();
}

Plugin * Save::copy() const
{
    return new Save;
}

String Save::name() const
{
    return djv_png::name;
}

List<String> Save::extensions() const
{
    return djv_png::extensions;
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
    _info.size = info.size;

    Pixel::TYPE type = Pixel::type(info.pixel);

    switch (type)
    {
        case Pixel::U10:
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

namespace
{

bool png_open(
    FILE *                f,
    png_structp           png,
    png_infop *           png_info,
    const Image_Io_Info & info)
{
    if (setjmp(png_jmpbuf(png)))
    {
        return false;
    }

    *png_info = png_create_info_struct(png);

    if (! *png_info)
    {
        return false;
    }

    png_init_io(png, f);

    int color_type = 0;

    switch (info.pixel)
    {
        case Pixel::L_U8:
        case Pixel::L_U16:
            color_type = PNG_COLOR_TYPE_GRAY;
            break;

        case Pixel::LA_U8:
        case Pixel::LA_U16:
            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
            break;

        case Pixel::RGB_U8:
        case Pixel::RGB_U16:
            color_type = PNG_COLOR_TYPE_RGB;
            break;

        case Pixel::RGBA_U8:
        case Pixel::RGBA_U16:
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
            break;

        default:
            break;
    }

    png_set_IHDR(
        png,
        *png_info,
        info.size.x,
        info.size.y,
        Pixel::bit_depth(info.pixel),
        color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, *png_info);

    return true;
}

const String error_open = "Error opening: %%";

} // namespace

void Save::_open(const String & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    // Initialize libpng.

    _png = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        &_png_error,
        djv_png_error,
        djv_png_warning);

    if (! _png)
    {
        throw Error(name(), _png_error.msg);
    }

    SNPRINTF(
        _png_error.msg,
        cstring_size,
        String(String_Format(error_open).arg(in)).c_str());

    // Open.

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

    if (! png_open(_f, _png, &_png_info, info))
    {
        throw Error(name(), _png_error.msg);
    }
}

void Save::_close()
{
    if (_png || _png_info)
    {
        png_destroy_write_struct(
            _png ? &_png : 0,
            _png_info ? &_png_info : 0);
        
        _png      = 0;
        _png_info = 0;
    }

    if (_f)
    {
        ::fclose(_f);
        
        _f = 0;
    }
}

namespace
{

bool png_scanline(png_structp png, const uint8_t * in)
{
    if (setjmp(png_jmpbuf(png)))
    {
        return false;
    }

    png_write_row(png, (png_byte *)in);

    return true;
}

bool png_end(png_structp png, png_infop png_info)
{
    if (setjmp(png_jmpbuf(png)))
    {
        return false;
    }

    png_write_end(png, png_info);

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

    if (in.info() != _image.info())
    {
        _image.zero();
        
        Gl_Image::copy(in, _image);
        
        p = &_image;
    }

    // Write the file.

    const int h = p->h();

    for (int y = 0; y < h; ++y)
    {
        if (! png_scanline(_png, p->data(0, h - 1 - y)))
        {
            throw Error(name(), _png_error.msg);
        }
    }

    if (! png_end(_png, _png_info))
    {
        throw Error(name(), _png_error.msg);
    }

    _close();
}

} // djv_png

