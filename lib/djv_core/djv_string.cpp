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

//! \file djv_string.cpp

#include <djv_string.h>

#include <djv_assert.h>
#include <djv_debug.h>
#include <djv_math.h>
#include <djv_memory.h>

#include <stdio.h>
#include <stdlib.h>

namespace djv
{

//------------------------------------------------------------------------------
// String_Util
//------------------------------------------------------------------------------

List<String> String_Util::split(
    const String & in,
    const String & separator,
    bool keep_empty)
{
    return split(in, List<String>() << separator, keep_empty);
}

List<String> String_Util::split(
    const String & in, const List<String> & separator, bool keep_empty)
{
    List<String> out;

    const size_t size = in.size();
    size_t i = 0, j = 0;

    for (; i < size; ++i)
        for (size_t k = 0; k < separator.size(); ++k)
        {
            const size_t separator_size = separator[k].size();

            if (0 == in.compare(i, separator_size, separator[k]))
            {
                if (i != j)
                {
                    out += String(in, j, i - j);
                }
                else if (keep_empty)
                {
                    out += String();
                }

                i += separator_size - 1;
                j = i + 1;
                break;
            }
        }

    if (j < size)
    {
        out += String(in, j, i - j);
    }
    else if (! size)
    {
        out += String();
    }

    return out;
}

List<String> String_Util::split(
    const String & in,
    char separator,
    bool keep_empty)
{
    return split(in, List<char>() << separator, keep_empty);
}

List<String> String_Util::split(
    const String & in,
    const List<char> & separator,
    bool keep_empty)
{
    List<String> out;

    const size_t size = in.size();
    size_t i = 0, j = 0;

    for (; i < size; ++i)
        for (size_t k = 0; k < separator.size(); ++k)
            if (in[i] == separator[k])
            {
                if (i != j)
                {
                    out += String(in, j, i - j);
                }
                else if (keep_empty)
                {
                    out += String();
                }

                j = i + 1;
                break;
            }

    if (j < size)
    {
        out += String(in, j, i - j);
    }
    else if (! size)
    {
        out += String();
    }

    return out;
}

size_t String_Util::cstring(
    const String & in,
    char * out,
    size_t max_len,
    bool terminate)
{
    DJV_ASSERT(max_len > 0);

    const size_t size =
        max_len ?
        Math::min(in.size(), max_len - static_cast<size_t>(terminate)) :
        (in.size() + static_cast<size_t>(terminate));

    size_t i = 0;

    for (; i < size; ++i)
    {
        out[i] = in[i];
    }

    if (terminate)
    {
        out[i++] = 0;
    }

    return i;
}

double String_Util::string_to_float(const String & in)
{
    return atof(in.c_str());
}

String String_Util::float_to_string(double in, int precision)
{
    char tmp [cstring_size] = "";
    int size = 0;

    if (precision)
    {
        char format [] = "%0. f";
        format[3] = '0' + abs(precision);
        size = SNPRINTF(tmp, cstring_size, format, in);
    }
    else
    {
        size = SNPRINTF(tmp, cstring_size, "%0.3f", in);
    }

    if (precision <= 0 && size)
    {
        int i = size - 1;

        for (; i >= 0 && '0' == tmp[i]; --i, --size) ;

        if (i)
            if ('.' == tmp[i])
            {
                --size;
            }
    }

    return String(tmp, (size > 0) ? size : 0);
}

const List<String> & String_Util::label_bool()
{
    static const List<String> data = List<String>() <<
        "False" <<
        "True";

    return data;
}

//------------------------------------------------------------------------------

namespace
{

const String serialize_null = "null";

const char serialize_separator = '|';

} // namespace

bool serialize(String * in, String * out)
{
    const size_t size = in->size();

    if (! size)
    {
        return false;
    }

    // Find word.

    size_t start = 0;

    for (; ' ' == (*in)[start] && start < size; ++start)
        ;

    size_t end = start;

    for (; ' ' != (*in)[end] && end < size; ++end)
        ;

    // Extract word.

    const String tmp = String(*in, start, end - start);

    if (tmp != serialize_null)
        //*out = tmp;
    {
        *out = String_Util::replace(tmp, serialize_separator, ' ');
    }

    *in = String(*in, end, size - end + 1);
    return true;
}

bool serialize(String * in, uint * out, const List<String> & label)
{
    const size_t size = in->size();

    if (! size)
    {
        return false;
    }

    //! \todo Case-insensitive?

    //const String _in = lower(*in);

    //! \todo Extra space prevents partial matches.

    const String _in = String_Util::lower(*in) + " ";

    // Find word.

    size_t start = 0;

    for (; ' ' == _in[start] && start < size; ++start)
        ;

    // Extract word.

    const size_t label_size = label.size();

    for (size_t i = 0; i < label_size; ++i)
    {
        //if (0 == _in.compare(start, label[i].size(), lower(label[i])))
        const String tmp = String_Util::replace(
                               String(_in, start, label[i].size() + 1),
                               serialize_separator,
                               ' ');

        if (String_Util::compare_no_case(tmp, label[i] + " "))
        {
            *out = static_cast<uint>(i);
            const size_t end = start + label[i].size();
            *in = String(*in, end, size - end + 1);
            return true;
        }
    }

    return false;
}

bool serialize(String * in, String * out, const List<String> & label)
{
    uint i = 0;

    if (! serialize(in, &i, label))
    {
        return false;
    }

    *out = label[i];
    return true;
}

String serialize(const String & in)
{
    //return in.size() ? in : serialize_null;
    const String tmp = String_Util::replace(in, ' ', serialize_separator);
    return tmp.size() ? tmp : serialize_null;
}

String & operator >> (String & in, String & out) throw (String)
{
    if (! serialize(&in, &out))
    {
        throw in;
    }

    return in;
}

String & operator << (String & out, const String & in)
{
    if (out.size())
    {
        out += ' ';
    }

    out += serialize(in);
    return out;
}

String & operator >> (String & in, bool & out) throw (String)
{
    uint i = 0;

    if (! serialize(&in, &i, String_Util::label_bool()))
    {
        throw in;
    }

    out = i != 0;
    return in;
}

String & operator << (String & out, bool in)
{
    if (out.size())
    {
        out += ' ';
    }

    out += String_Util::label_bool()[in];
    return out;
}

String & operator >> (String & in, int & out) throw (String)
{
    String tmp;
    in >> tmp;
    out = String_Util::string_to_int(tmp);
    return in;
}

String & operator << (String & out, int in)
{
    if (out.size())
    {
        out += ' ';
    }

    out += String_Util::int_to_string(in);
    return out;
}

String & operator >> (String & in, uint & out) throw (String)
{
    String tmp;
    in >> tmp;
    out = String_Util::string_to_uint(tmp);
    return in;
}

String & operator << (String & out, uint in)
{
    if (out.size())
    {
        out += ' ';
    }

    out += String_Util::uint_to_string(in);
    return out;
}

String & operator >> (String & in, double & out) throw (String)
{
    String tmp;
    in >> tmp;
    out = String_Util::string_to_float(tmp);
    return in;
}

String & operator << (String & out, double in)
{
    if (out.size())
    {
        out += ' ';
    }

    out += String_Util::float_to_string(in);
    return out;
}

} // djv
