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

//! \file djv_file.cpp

#include <djv_file.h>

#include <djv_math.h>
#include <djv_memory.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

namespace djv
{

//------------------------------------------------------------------------------
// File
//------------------------------------------------------------------------------

void File::init()
{
    _type = FILE;
    _size = 0;
    _user = 0;
    _perm = 0;
    _time = 0;
    _has_seq_extension = false;
}

File::File()
{
    init();
}

File::File(const String & in, bool stat)
{
    init();
    set(in, stat);
}

void File::set(const String & in, bool stat)
{
    //DJV_DEBUG("File::set");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("stat = " << stat);

    init();

    // Split file name into pieces.

    File_Util::split(in, _path, _base, _number, _extension);

    //DJV_DEBUG_PRINT("path = " << _path);
    //DJV_DEBUG_PRINT("base = " << _base);
    //DJV_DEBUG_PRINT("number = " << _number);
    //DJV_DEBUG_PRINT("extension = " << _extension);

    // Information.

    if (stat)
    {
        this->stat();
    }

    // Sequence.

    _seq = Seq::string_to_seq(_number);

    update_has_seq_extension();

    //DJV_DEBUG_PRINT("seq = " << _seq);
    //DJV_DEBUG_PRINT("has seq extension = " << _has_seq_extension);
}

String File::get(int64_t frame, bool path) const
{
    String out;

    // Reconstruct pieces.

    if (path)
    {
        out += _path;
    }

    out += _base;

    if (SEQ == _type && frame != -1)
    {
        out += Seq::frame_to_string(frame, _seq.pad);
    }
    else if (SEQ == _type && _seq.list.size())
    {
        out += Seq::seq_to_string(_seq);
    }
    else
    {
        out += _number;
    }

    out += _extension;

    return out;
}

String File::name() const
{
    return _base + _number + _extension;
}

void File::path(const String & in)
{
    _path = in;
}

void File::base(const String & in)
{
    _base = in;
}

void File::number(const String & in)
{
    _number = in;
}

void File::extension(const String & in)
{
    _extension = in;

    update_has_seq_extension();
}

void File::type(TYPE in)
{
    _type = in;
}

void File::size(uint64_t in)
{
    _size = in;
}

void File::user(uid_t in)
{
    _user = in;
}

void File::perm(int in)
{
    _perm = in;
}

void File::time(time_t in)
{
    _time = in;
}

#if defined(DJV_WINDOWS)

#define _STAT struct ::_stati64
#define _STAT_FNC ::_stati64

#elif (defined(DJV_FREEBSD) || defined(DJV_OSX))

//! \todo No stat64?

#define _STAT struct ::stat
#define _STAT_FNC ::stat

#else

#define _STAT struct ::stat64
#define _STAT_FNC ::stat64

#endif // DJV_WINDOWS

bool File::stat(const String & path)
{
    //DJV_DEBUG("File::stat");
    //DJV_DEBUG_PRINT("path = " << path);

    _STAT info;
    Memory::set<uint8_t>(0, &info, sizeof(_STAT));

    const String file =
        File_Util::path_fix(path.size() ? path : _path) + get(-1, false);
    //DJV_DEBUG_PRINT("file = " << file);

    //! \todo Windows stat.

    if (0 == _STAT_FNC(file.c_str(), &info))
        ;
    else if (
        0 == _STAT_FNC(String(file + File_Util::path_separator).c_str(), &info))
        ;
    else if (
        0 == _STAT_FNC(
            String(file, static_cast<int>(file.size()) - 1).c_str(), &info))
        ;
    else
    {
        String err;

#if defined(DJV_WINDOWS)

        //! \todo MinGW does not define strerror_s()?
        //char tmp[cstring_size] = "";
        //::strerror_s(tmp, cstring_size, errno);
        //err = tmp;
        err = ::strerror(errno);

#else // DJV_WINDOWS

        err = ::strerror(errno);

#endif // DJV_WINDOWS

        //DJV_DEBUG_PRINT("errno = " << err);

        return false;
    }

    _size = info.st_size;
    _user = info.st_uid;
    _perm = 0;
    _time = info.st_mtime;

#if defined(DJV_WINDOWS)

    if (info.st_mode & _S_IFDIR)
    {
        _type = DIRECTORY;
    }

    _perm |= (info.st_mode & _S_IREAD)  ? file::READ  : 0;
    _perm |= (info.st_mode & _S_IWRITE) ? file::WRITE : 0;
    _perm |= (info.st_mode & _S_IEXEC)  ? file::EXEC  : 0;

#else // DJV_WINDOWS

    if (S_ISDIR(info.st_mode))
    {
        _type = DIRECTORY;
    }

    _perm |= (info.st_mode & S_IRUSR) ? READ  : 0;
    _perm |= (info.st_mode & S_IWUSR) ? WRITE : 0;
    _perm |= (info.st_mode & S_IXUSR) ? EXEC  : 0;

#endif // DJV_WINDOWS

    return true;
}

void File::seq(const Seq & in)
{
    _seq = in;
}

Set<String> File::seq_extensions;

void File::update_has_seq_extension()
{
    _has_seq_extension = String_Util::find_no_case(
                             _extension,
                             seq_extensions);
}

} // djv

