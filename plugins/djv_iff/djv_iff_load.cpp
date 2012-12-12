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

//! \file djv_iff_load.cpp

#include <djv_iff_load.h>

namespace djv_iff
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
    _tiles(0),
    _compression(false)
{
    _io.endian(Memory::endian() != Memory::MSB);
}

Plugin * Load::copy() const
{
    return new Load;
}

String Load::name() const
{
    return djv_iff::name;
}

List<String> Load::extensions() const
{
    return djv_iff::extensions;
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

void Load::_open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _io.open(in, File_Io::READ);

    info.file_name = in;
    
    info_load(_io, info, &_tiles, &_compression);
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::get");
    //DJV_DEBUG_PRINT("frame = " << frame);

    image.color_profile = Color_Profile();
    image.tag = Image_Tag();

    uint8_t type[4];
    uint8_t pixels[32];

    uint32_t size;
    uint32_t chunk_size;
    uint32_t tiles_rgba;

    // Open the file.

    const String file_name =
        _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info;
    
    _open(file_name, info);
    
    image.tag = info.tag;

    // Read the file.

    const int channel_bytes = Pixel::channel_bytes(info.pixel);
    const uint bytes = Pixel::bytes(info.pixel);

    //DJV_DEBUG_PRINT("channels = " << channels);
    //DJV_DEBUG_PRINT("channel_bytes = " << channel_bytes);
    //DJV_DEBUG_PRINT("bytes = " << bytes);

    _io.read_ahead();

    Pixel_Data * data = frame.proxy ? &_tmp : &image;

    data->set(info);

    tiles_rgba = _tiles;

    // Read FOR4 <size> TBMP block
    for (;;)
    {
        // Get type.
        _io.get(&type, 4);

        // Get length.
        _io.get_u32(&size, 1);
        chunk_size = align_size(size, 4);

        if (type[0] == 'A' &&
                type[1] == 'U' &&
                type[2] == 'T' &&
                type[3] == 'H')
        {
            //DJV_DEBUG_PRINT("type = AUTH");
            const uint8_t * p = _io.mmap_p();

            // Get tag.
            info.tag[Image_Tag::label_tag()[Image_Tag::CREATOR]] =
                String((const char *)p, chunk_size);

            // Skip to the next block.
            _io.seek(chunk_size);

            // Set tag.
            image.tag = info.tag;
        }
        else if (type[0] == 'F' &&
                 type[1] == 'O' &&
                 type[2] == 'R' &&
                 type[3] == '4')
        {
            //DJV_DEBUG_PRINT("type = FOR4");

            // Get type.
            _io.get(&type, 4);

            // Check if TBMP.
            if (type[0] == 'T' &&
                    type[1] == 'B' &&
                    type[2] == 'M' &&
                    type[3] == 'P')
            {
                //DJV_DEBUG_PRINT("type = TBMP");

                // Read RGBA and ZBUF block.
                for (;;)
                {
                    // Get type.
                    _io.get(&type, 4);

                    // Get length.
                    _io.get_u32(&size, 1);
                    chunk_size = align_size(size, 4);

                    // Tiles and RGBA.
                    if (tiles_rgba &&
                            type[0] == 'R' &&
                            type[1] == 'G' &&
                            type[2] == 'B' &&
                            type[3] == 'A')
                    {
                        // Set image size.
                        uint32_t image_size = size;

                        // Get tile coordinates.
                        uint16_t xmin, xmax, ymin, ymax;
                        _io.get_u16 (&xmin, 1);
                        _io.get_u16 (&ymin, 1);
                        _io.get_u16 (&xmax, 1);
                        _io.get_u16 (&ymax, 1);

                        if (xmin > xmax ||
                                ymin > ymax ||
                                xmax >= info.size.x ||
                                ymax >= info.size.y)
                        {
                            throw_error_unsupported(name(), file_name);
                        }

                        // NOTE: tile w = xmax - xmin + 1
                        //       tile h = ymax - ymin + 1

                        uint32_t tw = xmax - xmin + 1;
                        uint32_t th = ymax - ymin + 1;

                        if (!tw || !th)
                        {
                            throw_error_unsupported(name(), file_name);
                        }

                        bool tile_compress = false;

                        // If tile compression fails to be less than
                        // image data stored uncompressed, the tile
                        // is written uncompressed.

                        // Set channels.
                        uint8_t channels = Pixel::channels(info.pixel);

                        // Set tile pixels.

                        // Append xmin, xmax, ymin and ymax.
                        uint32_t tile_size =
                            tw * th * channels *
                            Pixel::channel_bytes(info.pixel) + 8;

                        // Test compressed.
                        if (tile_size > image_size)
                        {
                            tile_compress = true;
                        }

                        // Handle 8-bit data.
                        if (info.pixel == Pixel::RGB_U8 ||
                            info.pixel == Pixel::RGBA_U8)
                        {

                            // Tile compress.

                            if (tile_compress)
                            {
                                // Set bytes.
                                const uint8_t * p = _io.mmap_p();

                                // Map: RGB(A)8 BGRA to RGBA
                                for (int c = (channels * channel_bytes) - 1;
                                    c >= 0;
                                    --c)
                                {
                                    Memory_Buffer<uint8_t> in(tw * th);
                                    uint8_t * in_p = in ();

                                    // Uncompress.

                                    p += rle_load (p, in(), tw * th);

                                    for (uint16_t py = ymin; py <= ymax; py++)
                                    {
                                        uint8_t * out_dy = data->data(0, py);

                                        for (
                                            uint16_t px = xmin;
                                            px <= xmax;
                                            px++)
                                        {
                                            uint8_t * out_p =
                                                out_dy + px * bytes + c;
                                            *out_p++ = *in_p++;
                                        }
                                    }
                                }

                                // Seek

                                _io.seek(image_size - 8);

                                // Test.

                                if (p != _io.mmap_p())
                                {
                                    throw_error_unsupported(name(), file_name);
                                }
                            }
                            else
                            {
                                for (uint16_t py = ymin; py <= ymax; py++)
                                {
                                    uint8_t * out_dy = data->data(xmin, py);

                                    // Tile scanline.

                                    Memory_Buffer<uint8_t> scanline(tw * bytes);
                                    uint8_t * out_p = scanline();

                                    // Set bytes.

                                    for (uint16_t px = xmin; px <= xmax; px++)
                                    {
                                        // Get pixels.
                                        _io.get(&pixels, bytes);

                                        if (size < static_cast<uint32_t>(bytes))
                                        {
                                            throw_error_read(name(), file_name);
                                        }

                                        size -= bytes;

                                        // Map: RGB(A)8 ABGR to ARGB
                                        for (int c = channels - 1; c >= 0; --c)
                                        {
                                            uint8_t pixel;
                                            uint8_t * in_p =
                                                pixels + c * channel_bytes;
                                            Memory::copy(in_p, &pixel, 1);
                                            *out_p++ = pixel;
                                        }
                                    }

                                    // Copy data.
                                    Memory::copy(
                                        scanline(), out_dy, tw * bytes);
                                }
                            }
                        }
                        // Handle 16-bit data.
                        else if (
                            info.pixel == Pixel::RGB_U16 ||
                            info.pixel == Pixel::RGBA_U16)
                        {
                            if (tile_compress)
                            {

                                // Set bytes.
                                const uint8_t * p = _io.mmap_p();

                                // Set map.
                                int* map = NULL;

                                if (Memory::endian () == Memory::LSB)
                                {
                                    int rgb16[] = { 0, 2, 4, 1, 3, 5 };
                                    int rgba16[] = { 0, 2, 4, 7, 1, 3, 5, 6 };

                                    if (info.pixel == Pixel::RGB_U16)
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

                                    if (info.pixel == Pixel::RGB_U16)
                                    {
                                        map = rgb16;
                                    }
                                    else
                                    {
                                        map = rgba16;
                                    }
                                }

                                // Map: RGB(A)8 BGRA to RGBA
                                for (int c = (channels * channel_bytes) - 1;
                                    c >= 0;
                                    --c)
                                {
                                    int mc = map[c];

                                    Memory_Buffer<uint8_t> in(tw * th);
                                    uint8_t * in_p = in ();

                                    // Uncompress.

                                    p += rle_load (p, in(), tw * th);

                                    for (uint16_t py = ymin; py <= ymax; py++)
                                    {
                                        uint8_t * out_dy = data->data(0, py);

                                        for (
                                            uint16_t px = xmin;
                                            px <= xmax;
                                            px++)
                                        {
                                            uint8_t * out_p =
                                                out_dy + px * bytes + mc;
                                            *out_p++ = *in_p++;
                                        }
                                    }
                                }

                                // Seek

                                _io.seek(image_size - 8);

                                // Test.

                                if (p != _io.mmap_p())
                                {
                                    throw_error_unsupported(name(), file_name);
                                }
                            }
                            else
                            {
                                for (uint16_t py = ymin; py <= ymax; py++)
                                {
                                    uint8_t * out_dy = data->data(xmin, py);

                                    // Tile scanline.

                                    Memory_Buffer<uint16_t> scanline(
                                        tw * bytes);
                                    uint16_t * out_p = scanline();

                                    // Set bytes.

                                    for (uint16_t px = xmin; px <= xmax; px++)
                                    {
                                        // Get pixels.
                                        _io.get(&pixels, bytes);

                                        if (size < static_cast<uint32_t>(bytes))
                                        {
                                            throw_error_read(name(), file_name);
                                        }

                                        size -= bytes;

                                        // Map: RGB8 ABGR to ARGB
                                        for (int c = channels - 1; c >= 0; --c)
                                        {
                                            uint16_t pixel;
                                            uint8_t * in =
                                                pixels + c * channel_bytes;

                                            if (Memory::endian () ==
                                                Memory::LSB)
                                            {
                                                Memory::endian(
                                                    in, &pixel, 1, 2);
                                            }
                                            else
                                            {
                                                Memory::copy(in, &pixel, 2);
                                            }

                                            // Set pixel.
                                            *out_p++ = pixel;
                                        }
                                    }

                                    // Copy data.
                                    Memory::copy(
                                        scanline(),
                                        out_dy,
                                        tw * bytes);
                                }
                            }
                        }
                        else
                        {
                            _io.seek(chunk_size);
                        }

                        // Seek to align to chunksize.
                        size = chunk_size - image_size;

                        if (size)
                        {
                            _io.seek(size);
                        }

                        tiles_rgba--;
                    }
                    else
                    {
                        _io.seek(chunk_size);
                    }

                    if (tiles_rgba)
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }

                }

                // TBMP done, break.
                break;
            }

        }
        else
        {
            // Skip to the next block.
            _io.seek(chunk_size);
        }
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

} // djv_iff

