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

//! \file djv_rla.cpp

#include <djv_rla.h>

#include <djv_debug.h>
#include <djv_memory.h>

namespace djv_rla
{

void rle_load(
    File_Io & io,
    uint8_t * out,
    int size,
    int channels,
    int bytes) throw (Error)
{
    //DJV_DEBUG("rle_load");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("channels = " << channels);
    //DJV_DEBUG_PRINT("bytes = " << bytes);

    int16_t _size = 0;
    io.get_16(&_size);
    //DJV_DEBUG_PRINT("io size = " << _size);

    const uint8_t * start = io.mmap_p();
    const uint8_t * p = start;
    io.seek(_size);

    for (int b = 0; b < bytes; ++b)
    {
        uint8_t * out_p =
            out + (Memory::LSB == Memory::endian() ? (bytes - 1 - b) : b);
        const int out_inc = channels * bytes;

        for (int i = 0; i < size;)
        {
            int count = *((int8_t *)p);
            ++p;
            //DJV_DEBUG_PRINT("count = " << count);

            if (count >= 0)
            {
                ++count;

                for (int j = 0; j < count; ++j, out_p += out_inc)
                {
                    *out_p = *p;
                }

                ++p;
            }
            else
            {
                count = -count;

                for (int j = 0; j < count; ++j, ++p, out_p += out_inc)
                {
                    *out_p = *p;
                }
            }

            i += count;
        }
    }

    //DJV_DEBUG_PRINT("out = " << p - start);
}

void float_load(
    File_Io & io,
    uint8_t * out,
    int size,
    int channels) throw (Error)
{

    //DJV_DEBUG("float_load");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("channels = " << channels);

    int16_t _size = 0;
    io.get_16(&_size);
    //DJV_DEBUG_PRINT("io size = " << _size);

    const uint8_t * start = io.mmap_p();
    const uint8_t * p = start;
    io.seek(_size);

    const int out_inc = channels * 4;

    if (Memory::LSB == Memory::endian())
    {
        for (int i = 0; i < size; ++i, p += 4, out += out_inc)
        {
            out[0] = p[3];
            out[1] = p[2];
            out[2] = p[1];
            out[3] = p[0];
        }
    }
    else
    {
        for (int i = 0; i < size; ++i, p += 4, out += out_inc)
        {
            out[0] = p[0];
            out[1] = p[1];
            out[2] = p[2];
            out[3] = p[3];
        }
    }

    //DJV_DEBUG_PRINT("out = " << p - start);
}

void skip(File_Io & io) throw (Error)
{
    int16_t size = 0;
    io.get_16(&size);
    io.seek(size);
}

} // djv_rla
