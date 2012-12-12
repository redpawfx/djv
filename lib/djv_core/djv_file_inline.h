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

//! \file djv_file_inline.h

#include <djv_math.h>

namespace djv
{

//------------------------------------------------------------------------------
// File
//------------------------------------------------------------------------------

inline const String & File::path() const
{
    return _path;
}

inline const String & File::base() const
{
    return _base;
}

inline const String & File::number() const
{
    return _number;
}

inline const String & File::extension() const
{
    return _extension;
}

inline File::TYPE File::type () const
{
    return _type;
}

inline uint64_t File::size () const
{
    return _size;
}

inline uid_t File::user () const
{
    return _user;
}

inline int File::perm () const
{
    return _perm;
}

inline time_t File::time () const
{
    return _time;
}

inline const Seq & File::seq() const
{
    return _seq;
}

inline bool File::is_seq_valid() const
{
    return
        _type != File::DIRECTORY &&
        _number.size() &&
        _seq.list.size() &&
        _has_seq_extension;
}

inline bool File::is_seq_wildcard() const
{
    return
        _type != File::DIRECTORY &&
        _number.size() &&
        '#' == _number[0];
}

inline bool File::seq_add(const File & in)
{
    // Compare.

    if (_extension != in._extension)
    {
        return false;
    }

    if (_base != in._base)
    {
        return false;
    }

    if (! in.is_seq_valid())
    {
        return false;
    }

    //if (is_seq_valid() && _seq.pad != in._seq.pad)
    //  return false;

    // Update sequence.

    _seq.list += in._seq.list;
    _seq.pad = Math::max(_seq.pad, in._seq.pad);

    // Update information.

    _size += in._size;
    _user = Math::max(_user, in._user);
    _time = Math::max(_time, in._time);

    return true;
}

inline bool File::has_seq_extension() const
{
    return _has_seq_extension;
}

inline File::operator String() const
{
    return get();
}

//------------------------------------------------------------------------------

inline bool operator == (const File & a, const File & b)
{
    return a.get() == b.get();
}

inline bool operator != (const File & a, const File & b)
{
    return ! (a == b);
}

} // djv

