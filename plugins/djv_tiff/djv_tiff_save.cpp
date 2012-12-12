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

//! \file djv_tiff_save.cpp

#include <djv_tiff_save.h>

#include <djv_gl_image.h>

namespace djv_tiff
{

//------------------------------------------------------------------------------
// djv_image_save
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_save()
{
    tiff_init();

    return new Save;
}

}

//------------------------------------------------------------------------------
// Save::Options
//------------------------------------------------------------------------------

Save::Options::Options() :
    compression(_COMPRESSION_NONE)
{}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Compression";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Save::Save() :
    _f(0)
{}

Save::~Save()
{
    _close();
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_options = _options;
    return out;
}

String Save::name() const
{
    return djv_tiff::name;
}

List<String> Save::extensions() const
{
    return djv_tiff::extensions;
}

bool Save::option(const String & in, String * data)
{
    try
    {
        if (String_Util::compare_no_case(in, options()[COMPRESSION_OPTION]))
        {
            *data >> _options.compression;
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

    if (String_Util::compare_no_case(in, options()[COMPRESSION_OPTION]))
    {
        out << _options.compression;
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
    _info.size = info.size;
    _info.mirror.y = true;

    Pixel::TYPE type = Pixel::type(info.pixel);

    switch (type)
    {
        case Pixel::U10:
            type = Pixel::U16;
            break;

        case Pixel::F16:
            type = Pixel::F32;
            break;

        default:
            break;
    }

    _info.pixel = Pixel::pixel(Pixel::format(info.pixel), type);

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

void Save::_open(const String & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    // Open the file.

    _f = TIFFOpen(in.c_str(), "w");

    if (! _f)
    {
        throw_error_open(name(), in);
    }

    // Write the header.

    uint16 photometric        = 0;
    uint16 samples            = 0;
    uint16 sample_depth       = 0;
    uint16 sample_format      = 0;
    uint16 extra_samples []   = { EXTRASAMPLE_ASSOCALPHA };
    uint16 extra_samples_size = 0;
    uint16 compression        = 0;

    switch (Pixel::format(_image.pixel()))
    {
        case Pixel::L:
            photometric = PHOTOMETRIC_MINISBLACK;
            samples     = 1;
            break;

        case Pixel::LA:
            photometric        = PHOTOMETRIC_MINISBLACK;
            samples            = 2;
            extra_samples_size = 1;
            break;

        case Pixel::RGB:
            photometric = PHOTOMETRIC_RGB;
            samples     = 3;
            break;

        case Pixel::RGBA:
            photometric        = PHOTOMETRIC_RGB;
            samples            = 4;
            extra_samples_size = 1;
            break;

        default:
            break;
    }

    switch (Pixel::type(_image.pixel()))
    {
        case Pixel::U8:
            sample_depth  = 8;
            sample_format = SAMPLEFORMAT_UINT;
            break;

        case Pixel::U16:
            sample_depth  = 16;
            sample_format = SAMPLEFORMAT_UINT;
            break;

        case Pixel::F16:
        case Pixel::F32:
            sample_depth  = 32;
            sample_format = SAMPLEFORMAT_IEEEFP;
            break;

        default:
            break;
    }

    switch (_options.compression)
    {
        case _COMPRESSION_NONE:
            compression = COMPRESSION_NONE;
            break;

        case _COMPRESSION_RLE:
            compression = COMPRESSION_PACKBITS;
            break;

        case _COMPRESSION_LZW:
            compression = COMPRESSION_LZW;
            break;

        default:
            break;
    }

    TIFFSetField(_f, TIFFTAG_IMAGEWIDTH, _image.w());
    TIFFSetField(_f, TIFFTAG_IMAGELENGTH, _image.h());
    TIFFSetField(_f, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(_f, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(_f, TIFFTAG_BITSPERSAMPLE, sample_depth);
    TIFFSetField(_f, TIFFTAG_SAMPLEFORMAT, sample_format);
    TIFFSetField(_f, TIFFTAG_EXTRASAMPLES, extra_samples_size, extra_samples);
    TIFFSetField(_f, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(_f, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(_f, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Set image tags.

    const List<String> & label_tag = Image_Tag::label_tag();
    
    String tag = info.tag[label_tag[Image_Tag::CREATOR]];

    if (tag.size())
    {
        TIFFSetField(_f, TIFFTAG_ARTIST, tag.c_str());
    }

    tag = info.tag[label_tag[Image_Tag::COPYRIGHT]];

    if (tag.size())
    {
        TIFFSetField(_f, TIFFTAG_COPYRIGHT, tag.c_str());
    }

    tag = info.tag[label_tag[Image_Tag::TIME]];

    if (tag.size())
    {
        TIFFSetField(_f, TIFFTAG_DATETIME, tag.c_str());
    }

    tag = info.tag[label_tag[Image_Tag::DESCRIPTION]];

    if (tag.size())
    {
        TIFFSetField(_f, TIFFTAG_IMAGEDESCRIPTION, tag.c_str());
    }
}

void Save::_close()
{
    if (_f)
    {
        TIFFClose(_f);
        
        _f = 0;
    }
}

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
    
    //! Write the file.

    const int h = p->h();

    for (int y = 0; y < h; ++y)
    {
        if (TIFFWriteScanline(_f, (tdata_t *)p->data(0, y), y) == -1)
        {
            throw_error_write(name(), file_name);
        }
    }

    _close();
}

} // djv_tiff

