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

//! \file djv_targa.cpp

#include <djv_targa.h>

#include <djv_assert.h>

namespace djv_targa
{

const List<String> & label_compression()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "RLE";

    DJV_ASSERT(data.size() == _COMPRESSION_SIZE);

    return data;
}

namespace
{

struct Header
{
    Header();

    void load(File_Io &, Image_Io_Info &, bool * compression) throw (Error);
    void save(File_Io &, const Image_Io_Info &, bool compression) throw (Error);

    void debug() const;

private:

    struct Data
    {
        uint8_t  id_size;
        uint8_t  cmap_type;
        uint8_t  image_type;
        uint16_t cmap_start;
        uint16_t cmap_size;
        uint8_t  cmap_bits;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        uint8_t  pixel_bits;
        uint8_t  descriptor;
    } _data;
};

Header::Header()
{
    _data.id_size = 0;
    _data.cmap_type = 0;
    _data.image_type = 0;
    _data.cmap_start = 0;
    _data.cmap_size = 0;
    _data.cmap_bits = 0;
    _data.x = 0;
    _data.y = 0;
    _data.width = 0;
    _data.height = 0;
    _data.pixel_bits = 0;
    _data.descriptor = 0;
}

void Header::load(File_Io & io, Image_Io_Info & info, bool * compression)
    throw (Error)
{
    //DJV_DEBUG("Header::load");

    // Read.

    io.get_u8(&_data.id_size);
    io.get_u8(&_data.cmap_type);
    io.get_u8(&_data.image_type);
    io.get_u16(&_data.cmap_start);
    io.get_u16(&_data.cmap_size);
    io.get_u8(&_data.cmap_bits);
    io.get_u16(&_data.x);
    io.get_u16(&_data.y);
    io.get_u16(&_data.width);
    io.get_u16(&_data.height);
    io.get_u8(&_data.pixel_bits);
    io.get_u8(&_data.descriptor);

    io.seek(_data.id_size);

    // Information.

    info.size = V2i(_data.width, _data.height);

    info.mirror = V2b(
        (_data.descriptor >> 4) & 1,
        (_data.descriptor >> 5) & 1);

    const int alpha_bits = _data.descriptor & 15;

    //DJV_DEBUG_PRINT("alpha bits = " << alpha_bits);

    switch (alpha_bits)
    {
        case 0:
        case 8:
            break;

        default:
            Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    int pixel = -1;

    switch (_data.image_type)
    {
        case 3:
        case 11:
            switch (_data.pixel_bits)
            {
                case 8:
                    pixel = Pixel::L_U8;
                    break;

                case 16:
                    pixel = Pixel::LA_U8;
                    break;
            }

            break;

        case 2:
        case 10:
            switch (_data.pixel_bits)
            {
                case 24:
                    pixel = Pixel::RGB_U8;
                    info.bgr = true;
                    break;

                case 32:
                    pixel = Pixel::RGBA_U8;
                    info.bgr = true;
                    break;
            }

            break;
    }

    if (-1 == pixel)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    info.pixel = Pixel::PIXEL(pixel);

    //DJV_DEBUG_PRINT("pixel = " << info.pixel);

    const int bits = _data.pixel_bits + alpha_bits;

    if (bits < (Pixel::channels(info.pixel) * 8) || (bits % 8) != 0)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    *compression =
        10 == _data.image_type ||
        11 == _data.image_type;

    //DJV_DEBUG_PRINT("compression = " << compression);

    info.endian = Memory::LSB;
}

void Header::save(File_Io & io, const Image_Io_Info & info, bool compression)
    throw (Error)
{
    //DJV_DEBUG("Header::save");

    // Information.

    _data.width = info.size.x;
    _data.height = info.size.y;

    const int channels = Pixel::channels(info.pixel);
    _data.pixel_bits = channels * 8;

    switch (channels)
    {
        case 1:
        case 2:
            _data.image_type = 3;
            break;

        case 3:
        case 4:
            _data.image_type = 2;
            break;
    }

    if (compression)
    {
        _data.image_type += 8;
    }

    int alpha_bits = 0;

    switch (Pixel::format(info.pixel))
    {
        case Pixel::LA:
        case Pixel::RGBA:
            alpha_bits = 8;
            break;

        default:
            break;
    }

    const int orient = 0;

    _data.descriptor = (alpha_bits & 15) | ((orient & 3) << 4);

    // Write.

    io.set_u8(_data.id_size);
    io.set_u8(_data.cmap_type);
    io.set_u8(_data.image_type);
    io.set_u16(_data.cmap_start);
    io.set_u16(_data.cmap_size);
    io.set_u8(_data.cmap_bits);
    io.set_u16(_data.x);
    io.set_u16(_data.y);
    io.set_u16(_data.width);
    io.set_u16(_data.height);
    io.set_u8(_data.pixel_bits);
    io.set_u8(_data.descriptor);
}

void Header::debug() const
{
    //DJV_DEBUG("Header::debug");
    //DJV_DEBUG_PRINT("id size = " << _data.id_size);
    //DJV_DEBUG_PRINT("cmap type = " << _data.cmap_type);
    //DJV_DEBUG_PRINT("image type = " << _data.image_type);
    //DJV_DEBUG_PRINT("cmap start = " << _data.cmap_start);
    //DJV_DEBUG_PRINT("cmap size = " << _data.cmap_size);
    //DJV_DEBUG_PRINT("cmap bits = " << _data.cmap_bits);
    //DJV_DEBUG_PRINT("image position = " <<_data. x << " " << _data.y);
    //DJV_DEBUG_PRINT("image size = " << _data.width << " " << _data.height);
    //DJV_DEBUG_PRINT("pixel bits = " << _data.pixel_bits);
}

} // namespace

void info_load(File_Io & io, Image_Io_Info & info, bool * compression)
    throw (Error)
{
    Header header;
    header.load(io, info, compression);
}

void info_save(File_Io & io, const Image_Io_Info & info, bool compression)
    throw (Error)
{
    Header header;
    header.save(io, info, compression);
}

const uint8_t * rle_load(
    const uint8_t * in,
    const uint8_t * end,
    uint8_t *       out,
    int             size,
    int             channels)
{
    //DJV_DEBUG("rle_load");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("channels = " << channels);

    const uint8_t * const out_end = out + size * channels;

    while (out < out_end)
    {
        // Information.

        if (in > end)
        {
            return 0;
        }

        const uint8_t count = (*in & 0x7f) + 1;
        const bool run = (*in & 0x80) ? true : false;
        const int length = run ? 1 : count;
        //DJV_DEBUG_PRINT("length = " << length);

        ++in;

        // Unpack.

        if (in + length * channels > end)
        {
            return 0;
        }

        if (run)
        {
            for (int j = 0; j < count; ++j, out += channels)
            {
                switch (channels)
                {
                    case 4:
                        out[3] = in[3];

                    case 3:
                        out[2] = in[2];

                    case 2:
                        out[1] = in[1];

                    case 1:
                        out[0] = in[0];
                        break;
                }
            }

            in += channels;
        }
        else
        {
            for (int j = 0; j < count; ++j, in += channels, out += channels)
            {
                switch (channels)
                {
                    case 4:
                        out[3] = in[3];

                    case 3:
                        out[2] = in[2];

                    case 2:
                        out[1] = in[1];

                    case 1:
                        out[0] = in[0];
                        break;
                }
            }
        }
    }

    return in;
}

size_t rle_save(
    const uint8_t * in,
    uint8_t * out,
    int size,
    int channels)
{
    //DJV_DEBUG("rle_save");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("channels = " << channels);

    const uint8_t * const _out = out;
    const uint8_t * const end = in + size * channels;

    while (in < end)
    {
        // Find runs.

        const int max =
            Math::min(0x7f + 1, static_cast<int>(end - in) / channels);
        
        int count = 1;

        for (; count < max; ++count)
        {
            int c = 0;

            for (; c < channels; ++c)
            {
                if (in[count * channels + c] != in[(count - 1) * channels + c])
                {
                    break;
                }
            }

            if (c < channels)
            {
                break;
            }
        }

        const bool run = count > 1;
        const int length = run ? 1 : count;
        //DJV_DEBUG_PRINT("count = " << count);
        //DJV_DEBUG_PRINT("  run = " << run);
        //DJV_DEBUG_PRINT("  length = " << length);

        // Information.

        *out++ = ((count - 1) & 0x7f) | (run << 7);

        // Pack.

        for (int j = 0; j < length * channels; j += channels)
        {
            switch (channels)
            {
                case 4:
                    out[j + 3] = in[j + 3];

                case 3:
                    out[j + 2] = in[j + 2];

                case 2:
                    out[j + 1] = in[j + 1];

                case 1:
                    out[j    ] = in[j    ];
                    break;
            }
        }

        out += length * channels;
        in += count * channels;
    }

    const size_t r = out - _out;
    //DJV_DEBUG_PRINT("return = " << static_cast<int>(r));
    return r;
}

_DJV_STRING_OPERATOR_LABEL(COMPRESSION, label_compression())

} // djv_targa

