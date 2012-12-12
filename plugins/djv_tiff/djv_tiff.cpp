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

//! \file djv_tiff.cpp

#include <djv_tiff.h>

#include <djv_assert.h>

namespace djv_tiff
{

const List<String> & label_compression()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "RLE" <<
        "LZW";

    DJV_ASSERT(data.size() == _COMPRESSION_SIZE);

    return data;
}

void tiff_init()
{
    TIFFSetErrorHandler(0);
    TIFFSetWarningHandler(0);
}

void palette_load(
    uint8_t *  in,
    int        size,
    int        bytes,
    uint16_t * red,
    uint16_t * green,
    uint16_t * blue)
{
    switch (bytes)
    {
        case 1:
        {
            const uint8_t * in_p = in + size - 1;
            uint8_t * out_p = in + (size - 1) * 3;

            for (int x = 0; x < size; ++x, out_p -= 3)
            {
                const uint8_t index = *in_p--;
                out_p[0] = static_cast<uint8_t>(red[index]);
                out_p[1] = static_cast<uint8_t>(green[index]);
                out_p[2] = static_cast<uint8_t>(blue[index]);
            }
        }
        break;

        case 2:
        {
            const uint16_t * in_p =
                reinterpret_cast<const uint16_t *>(in) + size - 1;
            
            uint16_t * out_p =
                reinterpret_cast<uint16_t *>(in) + (size - 1) * 3;

            for (int x = 0; x < size; ++x, out_p -= 3)
            {
                const uint16_t index = *in_p--;
                out_p[0] = red[index];
                out_p[1] = green[index];
                out_p[2] = blue[index];
            }
        }
        break;
    }
}

_DJV_STRING_OPERATOR_LABEL(COMPRESSION, label_compression())

} // djv_tiff

