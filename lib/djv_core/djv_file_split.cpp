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

//! \file djv_file_split.cpp

#include <djv_file.h>

namespace djv
{

//------------------------------------------------------------------------------
// File_Util::split
//------------------------------------------------------------------------------

namespace
{

//! \todo Is this still necessary?

inline bool _path_separator(char in)
{
    return in == File_Util::path_separators[0] ||
           in == File_Util::path_separators[1];
}

inline bool _seq_valid(char in)
{
    return String_Util::is_digit(in) || '#' == in;
}

inline bool _seq_separator(char in)
{
    return '-' == in || ',' == in;
}

inline bool _pad_match(const String & a, const String & b)
{
    //DJV_DEBUG("_pad_match");
    //DJV_DEBUG_PRINT("a = " << a);
    //DJV_DEBUG_PRINT("b = " << b);

    return ((a.size() > 1 && '0' == a[0]) || (b.size() > 1 && '0' == b[0])) ?
           (a.size() == b.size()) :
           true;
}

} // namespace

void File_Util::split(
    const String & in,
    String &       path,
    String &       base,
    String &       number,
    String &       extension)
{
    //DJV_DEBUG("File_Util::split");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("size = " << in.size());

    path.resize(0);
    base.resize(0);
    number.resize(0);
    extension.resize(0);

    const int size = static_cast<int>(in.size());

    if (! size)
    {
        return;
    }

    // Extension.

    int i = size - 1;
    int tmp = i;

    for (; in[i] != '.' && ! _path_separator(in[i]) && i > 0; --i) ;

    if (i > 0 && '.' == in[i])
    {
        extension = String(in, i, tmp - i + 1);
        --i;
    }
    else
    {
        i = size - 1;
    }

    //DJV_DEBUG_PRINT("extension = " << extension);

    // Number.

    if (i >= 0 && _seq_valid(in[i]))
    {
        tmp = i;
        int separator = -1;
        String word;

        for (; i > 0; --i)
        {
            if (! _seq_valid(in[i - 1]) || _seq_separator(in[i - 1]))
            {
                if (separator != -1 &&
                    ! _pad_match(String(in, i, separator - i), word))
                {
                    i = separator + 1;
                    break;
                }
                else
                {
                    word = String(
                        in,
                        i,
                        -1 == separator ? (tmp - i + 1) : (separator - i));

                    separator = i - 1;
                }
            }

            if (! (_seq_valid(in[i - 1]) || _seq_separator(in[i - 1])))
            {
                break;
            }
        }

        number = String(in, i, tmp - i + 1);
        --i;
    }

    //DJV_DEBUG_PRINT("number = " << number);

    // Base.

    if (i >= 0 && ! _path_separator(in[i]))
    {
        tmp = i;

        for (; i > 0 && ! _path_separator(in[i - 1]); --i) ;

        base = String(in, i, tmp - i + 1);
        --i;
    }

    //DJV_DEBUG_PRINT("base = " << base);

    // Path.

    if (i >= 0)
    {
        path = String(in, 0, i + 1);
    }

    //DJV_DEBUG_PRINT("path = " << path);
}

} // djv
