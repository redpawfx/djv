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

//! \file djv_tiff_load.cpp

#include <djv_tiff_load.h>

namespace djv_tiff
{

//------------------------------------------------------------------------------
// djv_image_load
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_load()
{
    tiff_init();

    return new Load;
}

}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
    _f(0)
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
    return djv_tiff::name;
}

List<String> Load::extensions() const
{
    return djv_tiff::extensions;
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

void Load::_open(const String & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    // Open the file.

    _f = TIFFOpen(in.c_str(), "r");

    if (! _f)
    {
        throw_error_open(name(), in);
    }

    // Read the Header.

    uint32   width              = 0;
    uint32   height             = 0;
    uint16   photometric        = 0;
    uint16   samples            = 0;
    uint16   sample_depth       = 0;
    uint16   sample_format      = 0;
    uint16 * extra_samples      = 0;
    uint16   extra_samples_size = 0;
    uint16   orient             = 0;
    uint16   compression        = 0;
    uint16   channels           = 0;

    TIFFGetFieldDefaulted(_f, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetFieldDefaulted(_f, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetFieldDefaulted(_f, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetFieldDefaulted(_f, TIFFTAG_SAMPLESPERPIXEL, &samples);
    TIFFGetFieldDefaulted(_f, TIFFTAG_BITSPERSAMPLE, &sample_depth);
    TIFFGetFieldDefaulted(_f, TIFFTAG_SAMPLEFORMAT, &sample_format);
    TIFFGetFieldDefaulted(_f, TIFFTAG_EXTRASAMPLES, &extra_samples_size,
        &extra_samples);
    TIFFGetFieldDefaulted(_f, TIFFTAG_ORIENTATION, &orient);
    TIFFGetFieldDefaulted(_f, TIFFTAG_COMPRESSION, &compression);
    TIFFGetFieldDefaulted(_f, TIFFTAG_PLANARCONFIG, &channels);
    TIFFGetFieldDefaulted(_f, TIFFTAG_COLORMAP,
        &_colormap[0], &_colormap[1], &_colormap[2]);

    //DJV_DEBUG_PRINT("tiff size = " << width << " " << height);
    //DJV_DEBUG_PRINT("tiff samples = " << samples);
    //DJV_DEBUG_PRINT("tiff sample depth = " << sample_depth);
    //DJV_DEBUG_PRINT("tiff channels = " << channels);

    // Get file information.

    info.file_name = in;

    info.size = V2i(width, height);

    if (samples > 1 && PLANARCONFIG_SEPARATE == channels)
    {
        throw_error_unsupported(name(), in);
    }

    int pixel = -1;

    switch (photometric)
    {
        case PHOTOMETRIC_PALETTE:
            pixel = Pixel::RGB_U8;
            break;

        case PHOTOMETRIC_MINISWHITE:
        case PHOTOMETRIC_MINISBLACK:
        case PHOTOMETRIC_RGB:
            if (32 == sample_depth && sample_format != SAMPLEFORMAT_IEEEFP)
            {
                break;
            }

            pixel = Pixel::pixel_(samples, sample_depth, true);
            break;
    }

    if (-1 == pixel)
    {
        throw_error_unsupported(name(), in);
    }

    info.pixel = Pixel::PIXEL(pixel);

    _compression = compression != COMPRESSION_NONE;
    _palette = PHOTOMETRIC_PALETTE == photometric;

    switch (orient)
    {
        case ORIENTATION_TOPLEFT:
            info.mirror.y = true;
            break;

        case ORIENTATION_TOPRIGHT:
            info.mirror.x = info.mirror.y = true;
            break;

        case ORIENTATION_BOTRIGHT:
            info.mirror.x = true;
            break;

        case ORIENTATION_BOTLEFT:
            break;
    }

    // Get image tags.

    const List<String> & label_tag = Image_Tag::label_tag();
    char * tag = 0;

    if (TIFFGetField(_f, TIFFTAG_ARTIST, &tag))
    {
        info.tag[label_tag[Image_Tag::CREATOR]] = tag;
    }

    if (TIFFGetField(_f, TIFFTAG_IMAGEDESCRIPTION, &tag))
    {
        info.tag[label_tag[Image_Tag::DESCRIPTION]] = tag;
    }

    if (TIFFGetField(_f, TIFFTAG_COPYRIGHT, &tag))
    {
        info.tag[label_tag[Image_Tag::COPYRIGHT]] = tag;
    }

    if (TIFFGetField(_f, TIFFTAG_DATETIME, &tag))
    {
        info.tag[label_tag[Image_Tag::TIME]] = tag;
    }
}

void Load::_close()
{
    if (_f)
    {
        TIFFClose(_f);
        
        _f = 0;
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

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info;
    _open(file_name, info);
    
    image.tag = info.tag;

    // Read the file.

    Pixel_Data * data = frame.proxy ? &_tmp : &image;
    data->set(info);

    for (int y = 0; y < info.size.y; ++y)
    {
        if (TIFFReadScanline(_f, (tdata_t *)data->data(0, y), y) == -1)
        {
            throw_error_read(name(), file_name);
        }

        if (_palette)
            palette_load(
                data->data(0, y),
                info.size.x,
                Pixel::channel_bytes(info.pixel),
                _colormap[0], _colormap[1], _colormap[2]);
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

} // djv_tiff

