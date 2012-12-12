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

//! \file djv_sgi.cpp

#include <djv_sgi.h>

#include <djv_assert.h>

namespace djv_sgi
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
        uint16_t magic;
        uint8_t  storage;
        uint8_t  bytes;
        uint16_t dimension;
        uint16_t width;
        uint16_t height;
        uint16_t channels;
        uint32_t pixel_min;
        uint32_t pixel_max;
    } _data;
};

Header::Header()
{
    _data.magic = 474;
    _data.storage = 0;
    _data.bytes = 0;
    _data.dimension = 0;
    _data.width = 0;
    _data.height = 0;
    _data.channels = 0;
    _data.pixel_min = 0;
    _data.pixel_max = 0;
}

void Header::load(File_Io & io, Image_Io_Info & info, bool * compression)
    throw (Error)
{
    //DJV_DEBUG("Header::load");

    // Read.

    io.get_u16(&_data.magic);

    if (_data.magic != 474)
    {
        Image_Io_Base::throw_error_unrecognized(name, io.file_name());
    }

    io.get_u8(&_data.storage);
    io.get_u8(&_data.bytes);
    io.get_u16(&_data.dimension);
    io.get_u16(&_data.width);
    io.get_u16(&_data.height);
    io.get_u16(&_data.channels);
    io.get_u32(&_data.pixel_min);
    io.get_u32(&_data.pixel_max);

    io.position(512);

    // Information.

    info.size = V2i(_data.width, _data.height);

    if (! Pixel::pixel_(
        _data.channels,
        1 == _data.bytes ? 8 : 16,
        false,
        &info.pixel))
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    info.endian = Memory::MSB;

    *compression = _data.storage ? true : false;
}

void Header::save(File_Io & io, const Image_Io_Info & info, bool compression)
    throw (Error)
{
    //DJV_DEBUG("Header::save");

    // Information.

    const int channels = Pixel::channels(info.pixel);
    const int bytes = Pixel::channel_bytes(info.pixel);

    _data.width = info.size.x;
    _data.height = info.size.y;

    _data.channels = channels;
    _data.bytes = bytes;

    _data.dimension = 1 == channels ? (1 == info.size.y ? 1 : 2) : 3;

    _data.pixel_min = 0;
    _data.pixel_max = 1 == bytes ? 255 : 65535;

    _data.storage = compression;

    // Write.

    io.set_u16(_data.magic);
    io.set_u8(_data.storage);
    io.set_u8(_data.bytes);
    io.set_u16(_data.dimension);
    io.set_u16(_data.width);
    io.set_u16(_data.height);
    io.set_u16(_data.channels);
    io.set_u32(_data.pixel_min);
    io.set_u32(_data.pixel_max);

    io.position(512);
}

void Header::debug() const
{
    //DJV_DEBUG("Header::debug");
    //DJV_DEBUG_PRINT("magic = " << _data.magic);
    //DJV_DEBUG_PRINT("storage = " << _data.storage);
    //DJV_DEBUG_PRINT("bytes = " << _data.bytes);
    //DJV_DEBUG_PRINT("dimension = " << _data.dimension);
    //DJV_DEBUG_PRINT("width = " << _data.width);
    //DJV_DEBUG_PRINT("height = " << _data.height);
    //DJV_DEBUG_PRINT("pixel range = " << _data.pixel_min << " " <<
    //    _data.pixel_max);
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

namespace
{

template<typename T>
bool load(
    const void * in,
    const void * end,
    void * out,
    int size,
    bool endian)
{
    //DJV_DEBUG("load");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("endian = " << endian);

    const size_t bytes = sizeof(T);
    //DJV_DEBUG_PRINT("bytes = " << bytes);

    const T * in_p = reinterpret_cast<const T *>(in);
    T * out_p = reinterpret_cast<T *>(out);
    const T * const out_end = out_p + size;

    while (out_p < out_end)
    {
        // Information.

        if (in_p > end)
        {
            return false;
        }

        const int count = *in_p & 0x7f;
        const bool run = ! (*in_p & 0x80);
        const int length = run ? 1 : count;
        //DJV_DEBUG_PRINT("count = " << count);
        //DJV_DEBUG_PRINT("  run = " << run);
        //DJV_DEBUG_PRINT("  length = " << length);

        ++in_p;

        // Unpack.

        if (in_p + length > end)
        {
            return false;
        }

        if (run)
        {
            if (! endian)
            {
                for (int j = 0; j < count; ++j, ++out_p)
                {
                    *out_p = *in_p;
                }
            }
            else
            {
                Memory::endian(in_p, out_p, 1, bytes);

                if (count > 1)
                {
                    Memory::set<T>(*out_p, out_p + 1, count - 1);
                }

                out_p += count;
            }

            ++in_p;
        }
        else
        {
            if (! endian)
            {
                for (int j = 0; j < length; ++j, ++in_p, ++out_p)
                {
                    *out_p = *in_p;
                }
            }
            else
            {
                Memory::endian(in_p, out_p, length, bytes);
                in_p += length;
                out_p += length;
            }
        }
    }

    return true;
}

} // namespace

bool rle_load(
    const void * in,
    const void * end,
    void * out,
    int size,
    int bytes,
    bool endian)
{
    switch (bytes)
    {
        case 1:
            return load<uint8_t>(in, end, out, size, false);

        case 2:
            return load<uint16_t>(in, end, out, size, endian);
    }

    return false;
}

namespace
{

template<typename T>
size_t save(
    const void * in,
    void * out,
    int size,
    bool endian)
{
    //DJV_DEBUG("save");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("endian = " << endian);

    const size_t bytes = sizeof(T);
    //DJV_DEBUG_PRINT("bytes = " << bytes);

    const T * in_p = reinterpret_cast<const T *>(in);
    T * out_p = reinterpret_cast<T *>(out);
    const T * const end = in_p + size;

    while (in_p < end)
    {
        // Pixel runs.

        const int min = 3;
        const int max = Math::min(0x7f, static_cast<int>(end - in_p));
        int count = 1, match = 1;

        for (; count < max; ++count)
        {
            if (in_p[count] == in_p[count - 1])
            {
                ++match;

                if (min == match && count >= min)
                {
                    count -= min - 1;
                    match = 1;
                    break;
                }
            }
            else
            {
                if (match >= min)
                {
                    break;
                }

                match = 1;
            }
        }

        const bool run = match > min;
        const int length = run ? 1 : count;
        //DJV_DEBUG_PRINT("count = " << count);
        //DJV_DEBUG_PRINT("  run = " << run);
        //DJV_DEBUG_PRINT("  length = " << length);

        // Information.

        *out_p++ = (count & 0x7f) | ((! run) << 7);

        // Pack.

        if (! endian)
        {
            for (int i = 0; i < length; ++i)
            {
                out_p[i] = in_p[i];
            }
        }
        else
        {
            Memory::endian(in_p, out_p, length, bytes);
        }

        out_p += length;
        in_p += count;
    }

    // Cap the end.

    *out_p++ = 0;

    const size_t r = (out_p - reinterpret_cast<T *>(out)) * bytes;
    //DJV_DEBUG_PRINT("r = " << r);
    return r;
}

} // namespace

size_t rle_save(
    const void * in,
    void * out,
    int size,
    int bytes,
    bool endian)
{
    switch (bytes)
    {
        case 1:
            return save<uint8_t>(in, out, size, false);

        case 2:
            return save<uint16_t>(in, out, size, endian);
    }

    return 0;
}

_DJV_STRING_OPERATOR_LABEL(COMPRESSION, label_compression())

} // djv_sgi

