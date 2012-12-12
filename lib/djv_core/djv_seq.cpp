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

//! \file djv_seq.cpp

#include <djv_seq.h>

#include <djv_assert.h>
#include <djv_math.h>

#include <algorithm>

#include <stdio.h>

namespace djv
{

//------------------------------------------------------------------------------
// Seq
//------------------------------------------------------------------------------

Seq::Seq() :
    pad(0)
{}

Seq::Seq(const List<int64_t> & in, int pad, const Speed & speed) :
    list (in),
    pad  (pad),
    speed(speed)
{}

Seq::Seq(int64_t start, int64_t end, int pad, const Speed & speed) :
    pad  (pad),
    speed(speed)
{
    set(start, end);
}

void Seq::set(int64_t start, int64_t end)
{
    if (start < end)
    {
        const size_t size = Math::min(end - start + 1, seq_max);

        list.resize(size);

        for (int64_t i = start, j = 0; i <= end && j < size; ++i, ++j)
        {
            list[j] = i;
        }
    }
    else
    {
        const size_t size = Math::min(start - end + 1, seq_max);

        list.resize(size);

        for (int64_t i = start, j = 0; i >= end && j < size; --i, ++j)
        {
            list[j] = i;
        }
    }
}

const int64_t Seq::seq_max = 3 * 60 * 60 * 24;

void Seq::sort(Seq & in)
{
    std::sort(in.list.begin(), in.list.end());
}

int64_t Seq::find_closest(int64_t in, const List<int64_t> & list)
{
    const size_t size = list.size();

    if (! size)
    {
        return -1;
    }

    int64_t out = 0;

    int64_t min = 0;

    for (size_t i = 0; i < size; ++i)
    {
        const int64_t tmp = Math::abs(in - list[i]);

        if (tmp < min || ! i)
        {
            out = static_cast<int64_t>(i);
            min = tmp;
        }
    }

    return out;
}

namespace
{

enum INC
{
    BREAK,
    ASCEND,
    DESCEND
};

inline INC inc(int64_t a, int64_t b)
{
    if (a + 1 == b)
    {
        return ASCEND;
    }
    else if (a - 1 == b)
    {
        return DESCEND;
    }

    return BREAK;
}

} // namespace

String Seq::seq_to_string(const Seq & in)
{
    //DJV_DEBUG("File_Seq::seq_to_string");
    //DJV_DEBUG_PRINT("in = " << in.list);

    List<String> out;

    List<int64_t> list = in.list;
    const size_t size = list.size();
    const int pad = in.pad;

    // List end marker.

    list += -2;

    // Sequence.

    int64_t tmp = list[0];
    INC _inc = BREAK;

    for (size_t i = 0; i < size; ++i)
    {
        const INC inc_ = inc(list[i], list[i + 1]);

        //DJV_DEBUG_PRINT(list[i] << " = " << inc_);

        if ((inc_ != _inc && _inc != BREAK) || BREAK == inc_)
        {
            if (tmp != list[i])
            {
                out += frame_to_string(tmp, pad) +
                    "-" +
                    frame_to_string(list[i], pad);
            }
            else
            {
                out += frame_to_string(list[i], pad);
            }

            tmp = list[i + 1];
            _inc = inc_;
        }
    }

    //DJV_DEBUG_PRINT("out = " << out);

    return String_Util::join(out, ',');
}

Seq Seq::string_to_seq(const String & in)
{
    //DJV_DEBUG("File_Seq::string_to_seq");
    //DJV_DEBUG_PRINT("in = " << in);

    Seq out;

    int pad = 0;

    const List<String> tmp = String_Util::split(in, ',');

    for (size_t i = 0; i < tmp.size(); ++i)
    {
        const List<String> tmp2 = String_Util::split(tmp[i], '-');
        const size_t size = tmp2.size();

        if (size)
        {
            int _pad = 0;
            const int64_t start = string_to_frame(tmp2[0], &_pad);
            const int64_t end = string_to_frame(tmp2[size - 1]);

            if (start != -1 && end != -1)
            {
                if (start < end)
                {
                    const size_t size = Math::min(end - start + 1, seq_max);

                    for (int64_t j = start, k = 0;
                        j <= end && k < size; ++j,
                        ++k)
                    {
                        out.list += j;
                    }
                }
                else
                {
                    const size_t size = Math::min(start - end + 1, seq_max);

                    for (int64_t j = start, k = 0;
                        j >= end && k < size; --j,
                        ++k)
                    {
                        out.list += j;
                    }
                }
            }

            pad = Math::max(_pad, pad);
        }
    }

    out.pad = pad;

    //DJV_DEBUG_PRINT("out = " << out);
    //DJV_DEBUG_PRINT("pad = " << out.pad);

    return out;
}

String Seq::frame_to_string(int64_t in, int pad)
{
    char format [] = "%00d";
    static const int format_max = 9;
    format[2] = char('0' + Math::min(pad, format_max));

    char tmp [cstring_size] = "";
    const int size = SNPRINTF(tmp, cstring_size, format, in);
    return String(tmp, size);
}

const List<String> & Seq::label_compress()
{
    static const List<String> data = List<String>() <<
        "Off" <<
        "Sparse" <<
        "Range";

    DJV_ASSERT(data.size() == _COMPRESS_SIZE);

    return data;
}

//------------------------------------------------------------------------------

String & operator << (String & out, const Seq & in)
{
    return out << Seq::seq_to_string(in);
}

String & operator >> (String & in, Seq & out) throw (String)
{
    String tmp;
    in >> tmp;
    out = Seq::string_to_seq(tmp);
    return in;
}

_DJV_STRING_OPERATOR_LABEL(Seq::COMPRESS, Seq::label_compress())

Debug & operator << (Debug & debug, const Seq & in)
{
    return debug <<
        String_Util::label(in) <<
        "@" <<
        Speed::speed_to_float(in.speed);
}

} // djv

