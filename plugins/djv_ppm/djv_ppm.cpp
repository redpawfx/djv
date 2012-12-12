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

//! \file djv_ppm.cpp

#include <djv_ppm.h>

#include <djv_assert.h>

namespace djv_ppm
{

const List<String> & label_type()
{
    static const List<String> data = List<String>() <<
        "Auto" <<
        "U1";

    DJV_ASSERT(data.size() == _TYPE_SIZE);

    return data;
}

const List<String> & label_data()
{
    static const List<String> data = List<String>() <<
        "ASCII" <<
        "Binary";

    DJV_ASSERT(data.size() == _DATA_SIZE);

    return data;
}

size_t bytes_scanline(int width, int channels, int bit_depth, DATA data)
{
    //DJV_DEBUG("bytes_scanline");
    //DJV_DEBUG_PRINT("width = " << width);
    //DJV_DEBUG_PRINT("channels = " << channels);
    //DJV_DEBUG_PRINT("bit depth = " << bit_depth);
    //DJV_DEBUG_PRINT("data = " << data);

    size_t out = 0;

    switch (data)
    {
        case DATA_ASCII:
        {
            int chars = 0;

            switch (bit_depth)
            {
                case  1:
                    chars = 1;
                    break;

                case  8:
                    chars = 3;
                    break;

                case 16:
                    chars = 5;
                    break;

                default:
                    break;
            }

            out = (chars + 1) * width * channels + 1;
        }
        break;

        case DATA_BINARY:
            switch (bit_depth)
            {
                case  1:
                    out = Math::ceil(width / 8.0);
                    break;

                case  8:
                case 16:
                    out = width * channels;
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    //DJV_DEBUG_PRINT("out = " << static_cast<int>(out));

    return out;
}

void ascii_load(File_Io & io, void * out, int size, int bit_depth) throw (Error)
{
    //DJV_DEBUG("ascii_load");

    char tmp [cstring_size] = "";
    int i = 0;

    switch (bit_depth)
    {
        case 1:
        {
            uint8_t * out_p = reinterpret_cast<uint8_t *>(out);

            for (; i < size; ++i)
            {
                File_Io::word(io, tmp, cstring_size);
                out_p[i] = String_Util::string_to_int(tmp) ? 0 : 255;
            }
        }
        break;

#define _LOAD(TYPE) \
  \
  TYPE * out_p = reinterpret_cast<TYPE *>(out); \
  for (; i < size; ++i) \
  { \
File_Io::word(io, tmp, cstring_size); \
out_p[i] = String_Util::string_to_int(tmp); \
  }

        case 8:
        {
            _LOAD(uint8_t)
        }
        break;

        case 16:
        {
            _LOAD(uint16_t)
        }
        break;

        default:
            break;
    }
}

size_t ascii_save(const void * in, void * out, int size, int bit_depth)
{
    char * out_p = reinterpret_cast<char *>(out);

    switch (bit_depth)
    {
        case 1:
        {
            const uint8_t * in_p = reinterpret_cast<const uint8_t *>(in);

            for (int i = 0; i < size; ++i)
            {
                out_p[0] = '0' + (! in_p[i]);
                out_p[1] = ' ';
                out_p += 2;
            }
        }
        break;

#define _SAVE(TYPE) \
  \
  const TYPE * in_p = reinterpret_cast<const TYPE *>(in); \
  for (int i = 0; i < size; ++i) \
  { \
const size_t j = String_Util::int_to_string(in_p[i], out_p); \
out_p[j] = ' '; \
out_p += j + 1; \
  }

        case 8:
        {
            _SAVE(uint8_t)
        }
        break;

        case 16:
        {
            _SAVE(uint16_t)
        }
        break;

        default:
            break;
    }

    *out_p++ = '\n';

    return out_p - reinterpret_cast<char *>(out);
}

_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())
_DJV_STRING_OPERATOR_LABEL(DATA, label_data())

} // namespace djv
