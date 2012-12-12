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

//! \file djv_png_load.cpp

#include <djv_png_load.h>

namespace djv_png
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
// Load
//------------------------------------------------------------------------------

Load::Load() :
    _f           (0),
    _png         (0),
    _png_info    (0),
    _png_info_end(0)
{}

Load::~Load()
{
    _close();
}

Plugin * Load::copy() const
{
    return new Load;
}

String Load::name() const
{
    return djv_png::name;
}

List<String> Load::extensions() const
{
    return djv_png::extensions;
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

namespace
{

bool png_open(
    FILE *      f,
    png_structp png,
    png_infop * png_info,
    png_infop * png_info_end)
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

    *png_info_end = png_create_info_struct(png);

    if (! *png_info_end)
    {
        return false;
    }

    uint8_t tmp [8];

    if (::fread(tmp, 8, 1, f) != 1)
    {
        return false;
    }

    if (png_sig_cmp(tmp, 0, 8))
    {
        return false;
    }

    png_init_io(png, f);
    png_set_sig_bytes(png, 8);
    png_read_info(png, *png_info);

    if (png_get_interlace_type(png, *png_info) != PNG_INTERLACE_NONE)
    {
        return false;
    }

    png_set_expand(png);
    //png_set_gray_1_2_4_to_8(png);
    png_set_palette_to_rgb(png);
    png_set_tRNS_to_alpha(png);

    return true;
}

const String error_open = "Error opening: %%";

} // namespace

void Load::_open(const String & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    // Initialize libpng.

    _png = png_create_read_struct(
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

    // Open the file.

#   if defined(DJV_WINDOWS)

    //! \todo MinGW does not define fopen_s()?
    //::fopen_s(&_f, in.c_str(), "rb");
    _f = ::fopen(in.c_str(), "rb");

#   else

    _f = ::fopen(in.c_str(), "rb");

#   endif

    if (! _f)
    {
        throw_error_open(name(), in);
    }

    if (! png_open(_f, _png, &_png_info, &_png_info_end))
    {
        throw Error(name(), _png_error.msg);
    }

    // Get file information.

    info.file_name = in;

    info.size = V2i(
        png_get_image_width(_png, _png_info),
        png_get_image_height(_png, _png_info));

    int channels = png_get_channels(_png, _png_info);

    if (png_get_color_type(_png, _png_info) == PNG_COLOR_TYPE_PALETTE)
    {
        channels = 3;
    }

    if (png_get_valid(_png, _png_info, PNG_INFO_tRNS))
    {
        ++channels;
    }

    //DJV_DEBUG_PRINT("channels = " << channels);

    int bit_depth = png_get_bit_depth(_png, _png_info);

    if (bit_depth < 8)
    {
        bit_depth = 8;
    }

    //DJV_DEBUG_PRINT("bit depth = " << bit_depth);

    if (bit_depth >= 16 && Memory::LSB == Memory::endian())
    {
        png_set_swap(_png);
    }

    if (! Pixel::pixel_(channels, bit_depth, false, &info.pixel))
    {
        throw_error_unsupported(name(), in);
    }
}

void Load::_close()
{
    if (_png || _png_info || _png_info_end)
    {
        png_destroy_read_struct(
            _png ? &_png : 0,
            _png_info ? &_png_info : 0,
            _png_info_end ? &_png_info_end : 0);
        
        _png          = 0;
        _png_info     = 0;
        _png_info_end = 0;
    }

    if (_f)
    {
        ::fclose(_f);
        
        _f = 0;
    }
}

namespace
{

bool png_scanline(png_structp png, uint8_t * out)
{
    if (setjmp(png_jmpbuf(png)))
    {
        return false;
    }

    png_read_row(png, out, 0);

    return true;
}

bool png_end(png_structp png, png_infop png_info)
{
    if (setjmp(png_jmpbuf(png)))
    {
        return false;
    }

    png_read_end(png, png_info);

    return true;
}

} // namespace

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::load");
    //DJV_DEBUG_PRINT("frame = " << frame);

    image.color_profile = Color_Profile();
    image.tag = Image_Tag();

    // Open the file.

    const String file_name =
        _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info;
    _open(file_name, info);

    // Read the file.

    Pixel_Data * data = frame.proxy ? &_tmp : &image;
    
    data->set(info);

    for (int y = 0; y < info.size.y; ++y)
    {
        if (! png_scanline(_png, data->data(0,  data->h() - 1 - y)))
        {
            throw Error(name(), _png_error.msg);
        }
    }

    png_end(_png, _png_info_end);

    // Proxy scale the image.
    
    if (frame.proxy)
    {
        info.size = Pixel_Data::proxy_scale(info.size, frame.proxy);
        info.proxy = frame.proxy;
        
        image.set(info);

        Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_png

