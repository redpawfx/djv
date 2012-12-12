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

//! \file djv_iff_save.cpp

#include <djv_iff_save.h>

#include <djv_gl_image.h>

namespace djv_iff
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
    compression(COMPRESSION_RLE)
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
    return djv_iff::name;
}

List<String> Save::extensions() const
{
    return djv_iff::extensions;
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
    _info.endian = Memory::MSB;

    Pixel::FORMAT format = Pixel::format(info.pixel);

    switch (format)
    {
        case Pixel::L:
            format = Pixel::RGB;
            break;

        case Pixel::LA:
            format = Pixel::RGBA;
            break;

        default:
            break;
    }

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

    _info.pixel = Pixel::pixel(format, type);
    _info.endian = Memory::MSB;

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

    const int w = p->w(), h = p->h();
    const int channels = Pixel::channels(p->info().pixel);
    const int channel_bytes = Pixel::channel_bytes(p->info().pixel);
    const int bytes = Pixel::bytes(p->info().pixel);

    bool compress = _options.compression ? true : false;

    uint32_t length = 0;

    //DJV_DEBUG_PRINT("channels = " << channels);
    //DJV_DEBUG_PRINT("channel_bytes = " << channel_bytes);
    //DJV_DEBUG_PRINT("bytes = " << bytes);

    size_t pos = 0;
    pos = _io.position();

    //DJV_DEBUG_PRINT("position = " << static_cast<int>(pos));

    // 'FOR4' type
    _io.set_u8('F');
    _io.set_u8('O');
    _io.set_u8('R');
    _io.set_u8('4');

    // 'FOR4' length
    // NOTE: only reserved for now.
    _io.set_u32(length);

    // 'TBMP' type
    _io.set_u8('T');
    _io.set_u8('B');
    _io.set_u8('M');
    _io.set_u8('P');

    // Write tiles.

    V2i size = tile_size (w, h);

    // Y order.

    for (int y = 0; y < size.y; y++)
    {
        // X order.

        for (int x = 0; x < size.x; x++)
        {
            // Set tile coordinates.
            uint16_t xmin, xmax, ymin, ymax;

            // Set xmin and xmax.
            xmin = x * tile_width();
            xmax = Math::min(xmin + tile_width(), w) - 1;

            // Set ymin and ymax.
            ymin = y * tile_height();
            ymax = Math::min(ymin + tile_height(), h) - 1;

            // Set width and height.
            uint32_t tw = xmax - xmin + 1;
            uint32_t th = ymax - ymin + 1;

            // Set type.
            _io.set_u8('R');
            _io.set_u8('G');
            _io.set_u8('B');
            _io.set_u8('A');

            // Length.
            uint32_t length = tw * th * bytes;

            // Tile length.
            uint32_t tile_length = length;

            // Align.
            length = align_size(length, 4);

            // Append xmin, xmax, ymin and ymax.
            length += 8;

            // Tile compression.
            bool tile_compress = compress;

            // Set bytes.
            Memory_Buffer<uint8_t> tile(tile_length);
            uint8_t * out_p = tile();

            // Handle 8-bit data.
            if (p->info().pixel == Pixel::RGB_U8 ||
                p->info().pixel == Pixel::RGBA_U8)
            {
                if (tile_compress)
                {
                    uint32_t index = 0, size = 0;

                    // Set bytes.
                    // NOTE: prevent buffer overrun.
                    Memory_Buffer<uint8_t> tmp(tile_length * 2);

                    // Map: RGB(A)8 RGBA to BGRA
                    for (int c = (channels * channel_bytes) - 1; c >= 0; --c)
                    {
                        Memory_Buffer<uint8_t> in(tw * th);
                        uint8_t * in_p = in();

                        // Data.

                        for (uint16_t py = ymin; py <= ymax; py++)
                        {
                            const uint8_t * in_dy = p->data(0, py);

                            for (uint16_t px = xmin; px <= xmax; px++)
                            {
                                // Get pixel.
                                uint8_t pixel;
                                const uint8_t * in_dx = in_dy + px * bytes + c;
                                Memory::copy(in_dx, &pixel, 1);
                                // Set pixel.
                                *in_p++ = pixel;
                            }
                        }

                        // Compress

                        size = rle_save(in(), tmp() + index, tw * th);
                        index += size;
                    }

                    // If size exceeds tile length use uncompressed.

                    if (index < tile_length)
                    {
                        Memory::copy(tmp(), tile(), index);

                        // Set tile length.
                        tile_length = index;

                        // Append xmin, xmax, ymin and ymax.
                        length = index + 8;

                        // Set length.
                        uint32_t align = align_size(length, 4);

                        if (align > length)
                        {
                            out_p = tile() + index;

                            // Pad.
                            for (int i = 0;
                                i < static_cast<int>(align - length);
                                i++)
                            {
                                *out_p++ = '\0';
                                tile_length++;
                            }
                        }
                    }
                    else
                    {
                        tile_compress = false;
                    }
                }

                if (!tile_compress)
                {
                    for (uint16_t py = ymin; py <= ymax; py++)
                    {
                        const uint8_t * in_dy = p->data(0, py);

                        for (uint16_t px = xmin; px <= xmax; px++)
                        {
                            // Map: RGB(A)8 RGBA to BGRA
                            for (int c = channels - 1; c >= 0; --c)
                            {
                                // Get pixel.
                                uint8_t pixel;
                                const uint8_t * in_dx =
                                    in_dy + px * bytes + c * channel_bytes;
                                Memory::copy(in_dx, &pixel, 1);
                                // Set pixel.
                                *out_p++ = pixel;
                            }
                        }
                    }
                }
            }
            // Handle 16-bit data.
            else if (
                p->info().pixel == Pixel::RGB_U16 ||
                p->info().pixel == Pixel::RGBA_U16)
            {
                if (tile_compress)
                {
                    uint32_t index = 0, size = 0;

                    // Set bytes.
                    // NOTE: prevent buffer overrun.
                    Memory_Buffer<uint8_t> tmp(tile_length * 2);

                    // Set map.
                    int* map = NULL;

                    if (Memory::endian () == Memory::LSB)
                    {
                        int rgb16[] = { 0, 2, 4, 1, 3, 5 };
                        int rgba16[] = { 0, 2, 4, 7, 1, 3, 5, 6 };

                        if (_info.pixel == Pixel::RGB_U16)
                        {
                            map = rgb16;
                        }
                        else
                        {
                            map = rgba16;
                        }
                    }
                    else
                    {
                        int rgb16[] = { 1, 3, 5, 0, 2, 4 };
                        int rgba16[] = { 1, 3, 5, 7, 0, 2, 4, 6 };

                        if (_info.pixel == Pixel::RGB_U16)
                        {
                            map = rgb16;
                        }
                        else
                        {
                            map = rgba16;
                        }
                    }

                    // Map: RGB(A)16 RGBA to BGRA
                    for (int c = (channels * channel_bytes) - 1; c >= 0; --c)
                    {
                        int mc = map[c];
                        Memory_Buffer<uint8_t> in(tw * th);
                        uint8_t * in_p = in();

                        // Data.

                        for (uint16_t py = ymin; py <= ymax; py++)
                        {
                            const uint8_t * in_dy = p->data(0, py);

                            for (uint16_t px = xmin; px <= xmax; px++)
                            {
                                // Get pixel.
                                uint8_t pixel;
                                const uint8_t * in_dx = in_dy + px * bytes + mc;
                                Memory::copy(in_dx, &pixel, 1);
                                // Set pixel.
                                *in_p++ = pixel;
                            }
                        }

                        // Compress

                        size = rle_save(in(), tmp() + index, tw * th);
                        index += size;
                    }

                    // If size exceeds tile length use uncompressed.

                    if (index < tile_length)
                    {
                        Memory::copy(tmp(), tile(), index);

                        // Set tile length.
                        tile_length = index;

                        // Append xmin, xmax, ymin and ymax.
                        length = index + 8;

                        // Set length.
                        uint32_t align = align_size(length, 4);

                        if (align > length)
                        {
                            out_p = tile() + index;

                            // Pad.
                            for (
                                int i = 0;
                                i < static_cast<int>(align - length);
                                i++)
                            {
                                *out_p++ = '\0';
                                tile_length++;
                            }
                        }
                    }
                    else
                    {
                        tile_compress = false;
                    }
                }

                if (!tile_compress)
                {
                    for (uint16_t py = ymin; py <= ymax; py++)
                    {
                        const uint8_t * in_dy = p->data(0, py);

                        for (uint16_t px = xmin; px <= xmax; px++)
                        {
                            // Map: RGB(A)16 RGBA to BGRA
                            for (int c = channels - 1; c >= 0; --c)
                            {
                                uint16_t pixel;
                                const uint8_t * in_dx =
                                    in_dy + px * bytes + c * channel_bytes;

                                if (Memory::endian () == Memory::LSB)
                                {
                                    Memory::endian(in_dx, &pixel, 1, 2);
                                }
                                else
                                {
                                    Memory::copy(in_dx, &pixel, 2);
                                }

                                // Set pixel.
                                *out_p++ = pixel;
                                out_p++;
                            }
                        }
                    }
                }
            }

            // Set length.
            _io.set_u32(length);

            // Set xmin, xmax, ymin and ymax.
            _io.set_u16(xmin);
            _io.set_u16(ymin);
            _io.set_u16(xmax);
            _io.set_u16(ymax);

            // Write.
            _io.set(tile(), tile_length);
        }
    }

    // Set FOR4 CIMG and FOR4 TBMP size
    uint32_t p0 = _io.position() - 8;
    uint32_t p1 = p0 - pos;

    // NOTE: FOR4 <size> CIMG
    _io.position (4);
    _io.set_u32 (p0);

    // NOTE: FOR4 <size> TBMP
    _io.position (pos + 4);
    _io.set_u32 (p1);
    _io.close();
}

} // djv_iff
