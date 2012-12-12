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

//! \file djv_file_util.cpp

#include <djv_file.h>

#include <djv_assert.h>
#include <djv_file_io.h>
#include <djv_math.h>
#include <djv_memory.h>

#include <algorithm>

namespace djv
{

//------------------------------------------------------------------------------
// File_Util
//------------------------------------------------------------------------------

bool File_Util::exists(const File & in)
{
    //DJV_DEBUG("File_Util::exists");
    //DJV_DEBUG_PRINT("in = " << in);

    List<String> list;

    if (File::SEQ == in.type())
    {
        list = seq_expand(in);
    }
    else
    {
        list += in;
    }

    //DJV_DEBUG_PRINT("list = " << list);

    for (size_t i = 0; i < list.size(); ++i)
    {
        try
        {
            File_Io().open(list[i], File_Io::READ);

            return true;
        }
        catch (Error)
        {}
    }

    return false;;
}

void File_Util::seq_sort(File & in)
{
    Seq::sort(in._seq);
}

const File & File_Util::seq_wildcard_match(
    const File & in,
    const List<File> & list)
{
    for (size_t i = 0; i < list.size(); ++i)
    {
        const File & tmp = list[i];

        if (
            in.base() == tmp.base() &&
            in.extension() == tmp.extension())
        {
            return tmp;
        }
    }

    return in;
}

void File_Util::seq_compress(List<File> & in, Seq::COMPRESS compress)
{
    if (Seq::COMPRESS_OFF == compress)
    {
        return;
    }

    //DJV_DEBUG("File_Util::seq_compress");
    //DJV_DEBUG_PRINT("size = " << in.size());

    size_t cache = 0;
    bool cache_valid = false;
    size_t i = 0;

    for (size_t j = 0; j < in.size(); ++j)
    {
        //DJV_DEBUG_PRINT("in = " << in[j]);
        //DJV_DEBUG_PRINT("in seq = " << in[j].seq());

        const bool seq = in[j].is_seq_valid();

        //DJV_DEBUG_PRINT("seq = " << seq);

        if (seq)
        {
            if (cache_valid)
            {
                if (in[cache].seq_add(in[j]))
                {
                    continue;
                }
            }

            cache_valid = false;

            for (size_t k = 0; k < i; ++k)
            {
                if (File::SEQ == in[k].type())
                {
                    if (in[k].seq_add(in[j]))
                    {
                        cache = k;
                        cache_valid = true;
                        continue;
                    }
                }
            }
        }

        if (! seq || ! cache_valid)
        {
            in[i] = in[j];

            if (seq)
            {
                in[i].type(File::SEQ);
            }

            ++i;
        }
    }

    //DJV_DEBUG_PRINT("size = " << i);

    in.resize(i);

    for (size_t i = 0; i < in.size(); ++i)
    {
        Seq::sort(in[i]._seq);
    }

    if (Seq::COMPRESS_RANGE == compress)
    {
        for (size_t i = 0; i < in.size(); ++i)
        {
            if (in[i]._seq.list.size())
            {
                in[i]._seq.set(in[i]._seq.start(), in[i]._seq.end());
            }
        }
    }
}

List<String> File_Util::seq_expand(const File & in)
{
    List<String> out;

    const Seq & seq = in.seq();
    const size_t size = seq.list.size();

    if (File::SEQ == in.type() && size)
    {
        out.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            out[i] = in.get(seq.list[i]);
        }
    }
    else
    {
        out += in;
    }

    return out;
}

void File_Util::recent(const String & in, List<String> & list, int max)
{
    const List<String>::iterator i = std::find(list.begin(), list.end(), in);

    if (i == list.end())
    {
        // Insert new item at front of list.

        list.insert(list.begin(), in);

        while (static_cast<int>(list.size()) > max)
        {
            list.erase(list.end() - 1);
        }
    }
    else
    {
        // Move existing item to front of list.

        const String tmp = *i;
        list.erase(i);
        list.insert(list.begin(), tmp);
    }
}

List<char> File_Util::path_separators = List<char>() << '/' << '\\';

#if defined(DJV_WINDOWS)
const char & File_Util::path_separator = File_Util::path_separators[1];
#else
const char & File_Util::path_separator = File_Util::path_separators[0];
#endif

List<char> File_Util::list_separators = List<char>() << ':' << ';';

const List<String> & File_Util::label_type()
{
    static const List<String> data = List<String>() <<
        "File" <<
        "Seq" <<
        "Dir";

    DJV_ASSERT(data.size() == File::_TYPE_SIZE);

    return data;
}

String File_Util::label_size(uint64_t in)
{
    if (in > Memory::terabyte)
    {
        return String_Format("%%TB").arg(in /
            static_cast<double>(Memory::terabyte), 0, -2);
    }
    else if (in > Memory::gigabyte)
    {
        return String_Format("%%GB").arg(in /
            static_cast<double>(Memory::gigabyte), 0, -2);
    }
    else if (in > Memory::megabyte)
    {
        return String_Format("%%MB").arg(in /
            static_cast<double>(Memory::megabyte), 0, -2);
    }
    else
    {
        return String_Format("%%KB").arg(in /
            static_cast<double>(Memory::kilobyte), 0, -2);
    }
}

const List<String> & File_Util::label_perm()
{
    static const List<String> data = List<String>() <<
        "r" <<
        "w" <<
        "x";

    DJV_ASSERT(static_cast<int>(data.size()) == File::_PERM_SIZE);

    return data;
}

const String & File_Util::label_perm(int in)
{
    static const String data [] =
    {
        "",
        "r",
        "w",
        "rw",
        "x",
        "rx",
        "wx",
        "rwx"
    };

    return data[in];
}

const List<String> & File_Util::label_sort()
{
    static const List<String> data = List<String>() <<
        "Name" <<
        "Type" <<
        "Size" <<
        "User" <<
        "Permissions" <<
        "Time";

    DJV_ASSERT(data.size() == _SORT_SIZE);

    return data;
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(File::TYPE, File_Util::label_type())
_DJV_STRING_OPERATOR_LABEL(File_Util::SORT, File_Util::label_sort())

Debug & operator << (Debug & debug, const File & in)
{
    return debug << in.get();
}

} // djv

