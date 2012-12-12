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

//! \file djv_pic.cpp

#include <djv_pic.h>

#include <djv_assert.h>
#include <djv_debug.h>
#include <djv_memory.h>

namespace djv_pic
{

const List<String> & label_compression()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "RLE";

    DJV_ASSERT(data.size() == _COMPRESSION_SIZE);

    return data;
}

const uint8_t * rle_load(
    const uint8_t * in,
    const uint8_t * end,
    uint8_t *       out,
    int             size,
    int             channels,
    int             stride,
    bool            endian)
{
    //DJV_DEBUG("rle_load");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("channels = " << channels);
    //DJV_DEBUG_PRINT("stride = " << stride);

    const uint8_t * const out_end = out + size * stride;

    while (in < end && out < out_end)
    {
        // Get RLE information.

        uint16_t count = *in++;

        //DJV_DEBUG_PRINT("count = " << count);
        if (count >= 128)
        {
            if (128 == count)
            {
                if (endian)
                {
                    Memory::endian(in, &count, 1, 2);
                }
                else
                {
                    Memory::copy(in, &count, 2);
                }

                in += 2;
            }
            else
            {
                count -= 127;
            }

            //DJV_DEBUG_PRINT("repeat = " << count);

            const uint8_t * p = in;
            in += channels;

            if (in > end)
            {
                break;
            }

            for (uint16_t i = 0; i < count; ++i, out += stride)
                for (int j = 0; j < channels; ++j)
                {
                    out[j] = p[j];
                }
        }
        else
        {
            ++count;
            //DJV_DEBUG_PRINT("raw = " << count);

            const uint8_t * p = in;
            in += count * channels;

            if (in > end)
            {
                break;
            }

            for (uint16_t i = 0; i < count; ++i, p += channels, out += stride)
                for (int j = 0; j < channels; ++j)
                {
                    out[j] = p[j];
                }
        }
    }

    return in > end ? 0 : in;
}

} // djv_pic
