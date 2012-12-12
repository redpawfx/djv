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

//! \file djv_jpeg_load.cpp

#include <djv_jpeg_load.h>

namespace djv_jpeg
{
using libjpeg::jpeg_decompress_struct;

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
    _f(0),
    _jpeg_init(false)
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
    return djv_jpeg::name;
}

List<String> Load::extensions() const
{
    return djv_jpeg::extensions;
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

bool jpeg_init(libjpeg::jpeg_decompress_struct * jpeg, Jpeg_Error * error)
{
    if (::setjmp(error->jump))
    {
        return false;
    }

    libjpeg::jpeg_create_decompress(jpeg);

    return true;
}

bool jpeg_open(
    FILE *                            f,
    libjpeg::jpeg_decompress_struct * jpeg,
    Jpeg_Error *                      error)
{
    if (::setjmp(error->jump))
    {
        return false;
    }

    libjpeg::jpeg_stdio_src(jpeg, f);

    libjpeg::jpeg_save_markers(jpeg, JPEG_COM, 0xFFFF);

    if (! libjpeg::jpeg_read_header(jpeg, TRUE))
    {
        return false;
    }

    if (! libjpeg::jpeg_start_decompress(jpeg))
    {
        return false;
    }

    return true;
}

} // namespace

void Load::_open(const String & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
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

    // Open.

# if defined(DJV_WINDOWS)
    //! \todo MinGW does not define fopen_s()?
    //::fopen_s(&_f, in.c_str(), "rb");
    _f = ::fopen(in.c_str(), "rb");
# else
    _f = ::fopen(in.c_str(), "rb");
# endif

    if (! _f)
    {
        throw_error_open(name(), in);
    }

    if (! jpeg_open(_f, &_jpeg, &_jpeg_error))
    {
        throw Error(name(), _jpeg_error.msg);
    }

    // Information.

    info.file_name = in;

    info.size = V2i(_jpeg.output_width, _jpeg.output_height);

    if (! Pixel::pixel_(_jpeg.out_color_components, 8, false, &info.pixel))
    {
        throw_error_unsupported(name(), in);
    }

    // Image tags.

    const libjpeg::jpeg_saved_marker_ptr marker = _jpeg.marker_list;

    if (marker)
        info.tag[Image_Tag::label_tag()[Image_Tag::DESCRIPTION]] =
            String((const char *)marker->data, marker->data_length);

    //DJV_DEBUG_PRINT("info = " << info);
}

void Load::_close()
{
    if (_jpeg_init)
    {
        libjpeg::jpeg_destroy_decompress(&_jpeg);
        
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
    libjpeg::jpeg_decompress_struct * jpeg,
    uint8_t *                         out,
    Jpeg_Error *                      error)
{
    if (::setjmp(error->jump))
    {
        return false;
    }

    libjpeg::JSAMPROW p [] = { (libjpeg::JSAMPLE *)(out) };

    if (! libjpeg::jpeg_read_scanlines(jpeg, p, 1))
    {
        return false;
    }

    return true;
}

bool jpeg_end(
    libjpeg::jpeg_decompress_struct * jpeg,
    Jpeg_Error *                      error)
{
    if (::setjmp(error->jump))
    {
        return false;
    }

    libjpeg::jpeg_finish_decompress(jpeg);

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
    
    image.tag = info.tag;

    // Read the file.

    Pixel_Data * data = frame.proxy ? &_tmp : &image;
    
    data->set(info);

    for (int y = 0; y < info.size.y; ++y)
    {
        if (! jpeg_scanline(
            &_jpeg,
            data->data(0, data->h() - 1 - y),
            &_jpeg_error))
        {
            throw Error(name(), _jpeg_error.msg);
        }
    }

    if (! jpeg_end(&_jpeg, &_jpeg_error))
    {
        throw Error(name(), _jpeg_error.msg);
    }

    if (frame.proxy)
    {
        info.size = Pixel_Data::proxy_scale(info.size, frame.proxy);
        info.proxy = frame.proxy;
        image.set(info);

        Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_jpeg

