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

//! \file djv_sgi_save.cpp

#include <djv_sgi_save.h>

#include <djv_gl_image.h>

namespace djv_sgi
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
    _io.endian(Memory::endian() != Memory::MSB);
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_options = _options;
    return out;
}

String Save::name() const
{
    return djv_sgi::name;
}

List<String> Save::extensions() const
{
    return djv_sgi::extensions;
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
    _info.endian = Memory::MSB;

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

void Save::_open(const String & in) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open the file.
    
    _io.open(in, File_Io::WRITE);

    info_save(_io, _info, _options.compression != COMPRESSION_NONE);

    // Setup the scanline tables.

    if (_options.compression)
    {
        const int table_size = _info.size.y * Pixel::channels(_info.pixel);
        
        //DJV_DEBUG_PRINT("rle table size = " << table_size);

        _rle_offset.size(table_size);
        _rle_size.size  (table_size);

        _io.seek(table_size * 2 * 4);
    }
}

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("compression = " << _options.compression);

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

    _tmp.set(p->info());

    const int w = _tmp.w(), h = _tmp.h();
    const int channels = Pixel::channels(_tmp.pixel());
    const int bytes = Pixel::channel_bytes(_tmp.pixel());

    // Deinterleave the image channels.

    Pixel_Data::planar_deinterleave(*p, &_tmp);

    // Write the file.

    if (! _options.compression)
    {
        _io.set(_tmp.data(), _tmp.bytes_data() / bytes, bytes);
    }
    else
    {
        Memory_Buffer<uint8_t> scanline(w * bytes * 2);

        for (int c = 0; c < channels; ++c)
        {
            for (int y = 0; y < h; ++y)
            {
                const size_t size = rle_save(
                    _tmp.data() + (c * h + y) * w * bytes,
                    scanline(),
                    w,
                    bytes,
                    _io.endian());

                _rle_offset()[y + c * h] = uint32_t(_io.position());
                _rle_size  ()[y + c * h] = uint32_t(size);

                _io.set(scanline.data(), size / bytes, bytes);
            }
        }
        
        _io.position(512);
        _io.set_u32(_rle_offset(), h * channels);
        _io.set_u32(_rle_size(), h * channels);
    }

    _io.close();
}

} // djv_sgi
