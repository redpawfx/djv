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

//! \file djv_file_sort.cpp

#include <djv_file.h>

#include <algorithm>

namespace djv
{

//------------------------------------------------------------------------------
// File_Util::sort
//------------------------------------------------------------------------------

namespace
{

typedef bool (Compare)(const File &, const File &);

inline bool name(const File & a, const File & b)
{
    return a.get() < b.get();
}
inline bool type(const File & a, const File & b)
{
    return a.type() < b.type();
}
inline bool size(const File & a, const File & b)
{
    return a.size() < b.size();
}
inline bool user(const File & a, const File & b)
{
    return a.user() < b.user();
}
inline bool perm(const File & a, const File & b)
{
    return a.perm() < b.perm();
}
inline bool time(const File & a, const File & b)
{
    return a.time() < b.time();
}

inline bool name_r(const File & a, const File & b)
{
    return name(b, a);
}
inline bool type_r(const File & a, const File & b)
{
    return type(b, a);
}
inline bool size_r(const File & a, const File & b)
{
    return size(b, a);
}
inline bool user_r(const File & a, const File & b)
{
    return user(b, a);
}
inline bool perm_r(const File & a, const File & b)
{
    return perm(b, a);
}
inline bool time_r(const File & a, const File & b)
{
    return time(b, a);
}

} // namespace

void File_Util::sort(List<File> & in, SORT sort, bool reverse)
{
    //DJV_DEBUG("File_Util::sort");
    //DJV_DEBUG_PRINT("in = " << in.size());
    //DJV_DEBUG_PRINT("sort = " << sort);
    //DJV_DEBUG_PRINT("reverse = " << reverse);

    Compare * compare = 0;

    switch (sort)
    {
        case SORT_NAME:
            compare = reverse ? name_r : name;
            break;

        case SORT_TYPE:
            compare = reverse ? type_r : type;
            break;

        case SORT_SIZE:
            compare = reverse ? size_r : size;
            break;

        case SORT_USER:
            compare = reverse ? user_r : user;
            break;

        case SORT_PERM:
            compare = reverse ? perm_r : perm;
            break;

        case SORT_TIME:
            compare = reverse ? time_r : time;
            break;

        default:
            break;
    }

    std::sort(in.begin(), in.end(), compare);
}

namespace
{

inline bool directories_first(const File & a, const File & b)
{
    return File::DIRECTORY == a.type() && File::DIRECTORY != b.type();
}

} // namespace

void File_Util::sort_directories_first(List<File> & in)
{
    //DJV_DEBUG("File_Util::sort_directories_first");

    std::stable_sort(in.begin(), in.end(), directories_first);
}

} // djv

