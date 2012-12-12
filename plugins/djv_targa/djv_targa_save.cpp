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

//! \file djv_targa_save.cpp

#include <djv_targa_save.h>

#include <djv_gl_image.h>

namespace djv_targa
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
    compression(COMPRESSION_NONE)
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

Save::Save()
{
    _io.endian(Memory::endian() != Memory::LSB);
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_options = _options;
    return out;
}

String Save::name() const
{
    return djv_targa::name;
}

List<String> Save::extensions() const
{
    return djv_targa::extensions;
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
    _info.pixel = Pixel::pixel(Pixel::format(info.pixel), Pixel::U8);

    switch (Pixel::format(_info.pixel))
    {
        case Pixel::RGB:
        case Pixel::RGBA:
            _info.bgr = true;
            break;

        default:
            break;
    }

    _info.endian = Memory::LSB;

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

void Save::_open(const String & in) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _io.open(in, File_Io::WRITE);

    info_save(_io, _info, _options.compression != COMPRESSION_NONE);
}

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);

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

    if (! _options.compression)
    {
        _io.set(p->data(), p->bytes_data());
    }
    else
    {
        const int w = p->w(), h = p->h();
        const int channels = Pixel::channels(p->info().pixel);

        Memory_Buffer<uint8_t> scanline(w * channels * 2);

        for (int y = 0; y < h; ++y)
        {
            const size_t size =
                rle_save(p->data(0, y), scanline(), w, channels);
            
            _io.set(scanline.data(), size);
        }
    }

    _io.close();
}

} // djv_targa
