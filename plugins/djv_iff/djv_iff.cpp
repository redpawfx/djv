//------------------------------------------------------------------------------
// Copyright (c) 2008-2009 Mikael Sundell
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

//! \file djv_iff.cpp

#include <djv_iff.h>

#include <djv_assert.h>

namespace djv_iff
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
    Header ();

    void load(File_Io &, Image_Io_Info &, int * tiles, bool * compression)
    throw (Error);
    void save(File_Io &, const Image_Io_Info &, bool compression)
    throw (Error);

    void debug() const;

private:

    struct Data
    {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
        uint8_t pixel_bits;
        uint8_t pixel_channels;
        uint16_t tiles;
    } _data;
};

Header::Header()
{
    _data.x = 0;
    _data.y = 0;
    _data.width = 0;
    _data.height = 0;
    _data.pixel_bits = 0;
    _data.pixel_channels = 0;
    _data.tiles = 0;
}

void Header::load(
    File_Io &       io,
    Image_Io_Info & info,
    int *           tiles,
    bool *          compression) throw (Error)
{
    //DJV_DEBUG("Header::load");

    uint8_t type[4];
    uint32_t size;
    uint32_t chunksize;
    uint32_t tbhdsize;
    uint32_t flags;
    uint32_t compressed;
    uint16_t bytes;
    uint16_t prnum;
    uint16_t prden;

    // Read FOR4 <size> CIMG.
    for (;;)
    {
        // Get type.
        io.get(&type, 4);

        // Get length.
        io.get_u32(&size, 1);
        chunksize = align_size(size, 4);

        if (type[0] == 'F' &&
                type[1] == 'O' &&
                type[2] == 'R' &&
                type[3] == '4')
        {
            //DJV_DEBUG_PRINT("type = FOR4");
            //DJV_DEBUG_PRINT("size = " << size);
            //DJV_DEBUG_PRINT("chunksize = " << chunksize);

            // Get type
            io.get(&type, 4);

            // Check if CIMG.
            if (type[0] == 'C' &&
                    type[1] == 'I' &&
                    type[2] == 'M' &&
                    type[3] == 'G')
            {
                //DJV_DEBUG_PRINT("type = CIMG");

                // Read TBHD.
                for (;;)
                {
                    // Get type
                    io.get(&type, 4);

                    // Get length
                    io.get_u32(&size, 1);
                    chunksize = align_size(size, 4);

                    if (type[0] == 'T' &&
                            type[1] == 'B' &&
                            type[2] == 'H' &&
                            type[3] == 'D')
                    {
                        tbhdsize = size;

                        // Test if size if correct.
                        if (tbhdsize != 24 && tbhdsize != 32)
                        {
                            Image_Io_Base::throw_error_read(
                                name, io.file_name());
                        }

                        // Set data.
                        Data data;

                        // Get width and height.
                        io.get_u32(&data.width, 1);
                        io.get_u32(&data.height, 1);
                        info.size = V2i(data.width, data.height);

                        // Get prnum and prdeb
                        io.get_u16(&prnum, 1);
                        io.get_u16(&prden, 1);

                        // Get flags, bytes, tiles and compressed.
                        io.get_u32(&flags, 1);
                        io.get_u16(&bytes, 1);

                        // Get tiles.
                        io.get_u16(&data.tiles, 1);
                        *tiles = data.tiles;

                        // Get compressed.
                        io.get_u32(&compressed, 1);

                        // 0 no compression
                        // 1 RLE compression
                        // 2 QRL (not supported)
                        // 3 QR4 (not supported)
                        if (compressed > 1)
                        {
                            // no compression or non-rle compression not
                            // supported
                            Image_Io_Base::throw_error_unsupported(
                                name, io.file_name());
                        }

                        // Get compressed.
                        *compression = compressed;

                        // Set XY.
                        if (tbhdsize == 32)
                        {
                            io.get_u32(&data.x, 1);
                            io.get_u32(&data.y, 1);
                        }
                        else
                        {
                            data.x = 0;
                            data.y = 0;
                        }

                        // Test format.
                        if (flags & 0x00000003)
                        {
                            // Test if grayscale is set, if throw assert.
                            DJV_ASSERT(!(flags & 0x00000010));
                            bool alpha = false;

                            // Test for RGB channels.
                            if (flags & 0x00000001)
                            {
                                data.pixel_channels = 3;
                            }

                            // Test for Alpha channel.
                            if (flags & 0x00000002)
                            {
                                data.pixel_channels++;
                                alpha = true;
                            }

                            // Test pixel bits.
                            if (flags & 0x00002000)
                            {
                                data.pixel_bits = 16; // 12bit represented as
                                                      // 16bit

                                if (!alpha)
                                {
                                    info.pixel = Pixel::RGB_U16;
                                }
                                else
                                {
                                    info.pixel = Pixel::RGBA_U16;
                                }
                            }
                            else
                            {
                                if (!bytes)
                                {
                                    data.pixel_bits = 8; // 8bit

                                    if (!alpha)
                                    {
                                        info.pixel = Pixel::RGB_U8;
                                    }
                                    else
                                    {
                                        info.pixel = Pixel::RGBA_U8;
                                    }
                                }
                                else
                                {
                                    data.pixel_bits = 16; // 16bit

                                    if (!alpha)
                                    {
                                        info.pixel = Pixel::RGB_U16;
                                    }
                                    else
                                    {
                                        info.pixel = Pixel::RGBA_U16;
                                    }
                                }
                            }

                            // Test bits.
                            const int bits =
                                data.pixel_channels * data.pixel_bits;
                            DJV_ASSERT (
                                bits == (Pixel::channels(info.pixel) *
                                    data.pixel_bits) &&
                                (bits % data.pixel_bits) == 0);
                        }

                        // Z format.
                        else if (flags & 0x00000004)
                        {
                            data.pixel_channels = 1;
                            data.pixel_bits = 32; // 32bit

                            // NOTE: Z_F32 support - not supported
                            // info->pixel = PIXEL(Pixel::Z_F32);

                            DJV_ASSERT(bytes == 0);
                        }

                        // Set data.
                        _data = data;

                        // TBHD done, break.
                        break;
                    }

                    // Skip to the next block.
                    io.seek(chunksize);
                }

                // Test if supported else skip to next block.
                if (_data.width > 0 &&
                        _data.height > 0 &&
                        _data.pixel_bits > 0 &&
                        _data.pixel_channels > 0)
                {
                    debug();
                    break;
                }

            }
        }

        // Skip to the next block.
        io.seek(chunksize);
    }
}


void Header::save(File_Io & io, const Image_Io_Info & info, bool compression)
    throw (Error)
{
    //DJV_DEBUG("Header::save");

    // FOR4 <size> CIMG    - 4 + 4 + 4 = 16 bytes
    // TBHD <size> <data>  - 4 + 4 + 32 = 40 bytes

    uint32_t length = 0;
    uint32_t flags  = 0;

    // Information.

    const int channels = Pixel::channels(info.pixel);
    int bytes = 0;

    _data.width = info.size.x;
    _data.height = info.size.y;
    _data.pixel_channels = channels;
    _data.pixel_bits = 1 == channels ? (1 == info.size.y ? 1 : 2) : 3;

    // Write.

    // 'FOR4' type
    io.set_u8('F');
    io.set_u8('O');
    io.set_u8('R');
    io.set_u8('4');

    // 'FOR4' length
    // NOTE: only reserved for now.
    io.set_u32(length);

    // 'CIMG' type
    io.set_u8('C');
    io.set_u8('I');
    io.set_u8('M');
    io.set_u8('G');

    // 'TBHD' type
    io.set_u8('T');
    io.set_u8('B');
    io.set_u8('H');
    io.set_u8('D');

    // 'TBHD' length
    length = 32;
    io.set_u32(length);

    // Set data.
    // NOTE: 32 bytes.

    // Set width and height.
    io.set_u32(_data.width);
    io.set_u32(_data.height);

    // Set prnum and prden.
    io.set_u16(1);
    io.set_u16(1);

    // Set flags.
    switch (info.pixel)
    {
        case Pixel::RGB_U8:
            // RGB 8.
            flags = 0x00000001;
            bytes = 0;
            break;

        case Pixel::RGBA_U8:
            // RGBA 8.
            flags = 0x00000003;
            bytes = 0;
            break;

        case Pixel::RGB_U16:
            // RGB 16.
            flags = 0x00000001;
            bytes = 1;
            break;

        case Pixel::RGBA_U16:
            // RGBA 16.
            flags = 0x00000003;
            bytes = 1;
            break;

        default:
            break;
    }

    io.set_u32(flags);
    io.set_u16(bytes);

    // Set tiles.
    V2i size = tile_size (_data.width, _data.height);
    io.set_u16(size.x * size.y);

    // Set compressed.
    // 0 no compression
    // 1 RLE compression
    // 2 QRL (not supported)
    // 3 QR4 (not supported)
    if (!compression)
    {
        io.set_u32(0);
    }
    else
    {
        io.set_u32(1);
    }

    // Set x and y.
    io.set_u32(0);
    io.set_u32(0);
}

void Header::debug() const
{
    //DJV_DEBUG("Header::debug");
    //DJV_DEBUG_PRINT("x = " << _data.x);
    //DJV_DEBUG_PRINT("y = " << _data.y);
    //DJV_DEBUG_PRINT("width = " << _data.width);
    //DJV_DEBUG_PRINT("height = " << _data.height);
    //DJV_DEBUG_PRINT("pixel_bits = " << _data.pixel_bits);
    //DJV_DEBUG_PRINT("pixel_channels = " << _data.pixel_channels);
    //DJV_DEBUG_PRINT("tiles = " << _data.tiles);
}

} // namespace

void info_load(
    File_Io &       io,
    Image_Io_Info & info,
    int *           tiles,
    bool *          compression) throw (Error)
{
    Header header;
    header.load(io, info, tiles, compression);
}

void info_save(File_Io & io, const Image_Io_Info & info, bool compression)
    throw (Error)
{
    Header header;
    header.save(io, info, compression);
}

size_t rle_load(
    const uint8_t * in,
    uint8_t *       out,
    int             size)
{
    //DJV_DEBUG("rle_load");
    //DJV_DEBUG_PRINT("size = " << size);

    const uint8_t * const _in = in;
    const uint8_t * const end = out + size;

    while (out < end)
    {
        // Information.

        const uint8_t count = (*in & 0x7f) + 1;
        const bool run = (*in & 0x80) ? true : false;
        ++in;

        // Find runs.

        if (!run)
        {
            // Verbatim.

            for (int i = 0; i < count; i++)
            {
                *out++ = *in++;
            }
        }
        else
        {
            // Duplicate.

            const uint8_t p = *in++;

            for (int i = 0; i < count; i++)
            {
                *out++ = p;
            }
        }
    }

    const size_t r = in - _in;
    //DJV_DEBUG_PRINT("tile r = " << static_cast<int>(r));
    return r;
}

namespace
{

void save_verbatim (
    const uint8_t *& in,
    uint8_t *& out,
    int size)
{
    int count = 1;
    unsigned char byte = 0;

    // Two in a row or count.

    for (; count < size; ++count)
    {
        if (in[count - 1] == in[count])
        {
            if (byte == in[count - 1])
            {
                count -= 2;
                break;
            }
        }

        byte = in[count - 1];
    }

    // Information.

    *out++ = count - 1;
    Memory::copy(in, out, count);

    out += count;
    in += count;
}

void save_duplicate (
    const uint8_t *& in,
    uint8_t *& out,
    int size)
{
    int count = 1;

    for (; count < size; ++count)
    {
        if (in[count - 1] != in[count])
        {
            break;
        }
    }

    const bool run = count > 1;
    const int length = run ? 1 : count;

    // Information.

    *out++ = ((count - 1) & 0x7f) | (run << 7);
    *out = *in;

    out += length;
    in += count;
}

} // namespace

size_t rle_save (
    const uint8_t * in,
    uint8_t * out,
    int size)
{
    //DJV_DEBUG("rle_save");
    //DJV_DEBUG_PRINT("size = " << size);

    const uint8_t * const _out = out;
    const uint8_t * const end = in + size;

    while (in < end)
    {
        // Find runs.

        const int max = Math::min(0x7f + 1, static_cast<int>(end - in));

        if (max > 0)
        {
            if (in[0] != in[1])
            {
                // Verbatim.

                save_verbatim (in, out, max);
            }
            else
            {
                // Duplicate.

                save_duplicate (in, out, max);
            }
        }
    }

    const size_t r = out - _out;
    //DJV_DEBUG_PRINT("tile r = " << static_cast<int>(r));
    return r;
}

uint32_t align_size (uint32_t size, uint32_t alignment)
{
    uint32_t mod = size % alignment;

    if (mod)
    {
        mod = alignment - mod;
        size += mod;
    }

    return size;
}

V2i tile_size (uint32_t width, uint32_t height)
{
    uint32_t tw = tile_width(), th = tile_height();
    V2i size = V2i((width + tw - 1) / tw,
                   (height + th - 1) / th);
    return (size);
}

const int & tile_width()
{
    static int tile_w = 64;
    return tile_w;
}

const int & tile_height()
{
    static int tile_h = 64;
    return tile_h;
}

_DJV_STRING_OPERATOR_LABEL(COMPRESSION, label_compression())

} // djv_iff

