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

//! \file djv_image_tag.cpp

#include <djv_image_tag.h>

#include <djv_assert.h>

namespace djv
{

//------------------------------------------------------------------------------
// Image_Tag
//------------------------------------------------------------------------------

void Image_Tag::add(const Image_Tag & in)
{
    for (size_t i = 0; i < in._list.size(); ++i)
    {
        add(in._list[i].first, in._list[i].second);
    }
}

void Image_Tag::add(const String & key, const String & value)
{
    for (size_t i = 0; i < _list.size(); ++i)
        if (key == _list[i].first)
        {
            _list[i].second = value;
            return;
        }

    _list += Pair(key, value);
}

String Image_Tag::get(const String & key) const
{
    for (size_t i = 0; i < _list.size(); ++i)
        if (key == _list[i].first)
        {
            return _list[i].second;
        }

    return String();
}

List<String> Image_Tag::keys() const
{
    List<String> out(_list.size());

    for (size_t i = 0; i < _list.size(); ++i)
    {
        out[i] = _list[i].first;
    }

    return out;
}

List<String> Image_Tag::values() const
{
    List<String> out(_list.size());

    for (size_t i = 0; i < _list.size(); ++i)
    {
        out[i] = _list[i].second;
    }

    return out;
}

int Image_Tag::size() const
{
    return static_cast<int>(_list.size());
}

bool Image_Tag::is_valid(const String & key)
{
    bool r = false;

    for (size_t i = 0; i < _list.size(); ++i)
        if (key == _list[i].first)
        {
            r = true;
            break;
        }

    return r;
}

void Image_Tag::clear()
{
    _list.clear();
}

String & Image_Tag::operator [] (const String & key)
{
    for (size_t i = 0; i < _list.size(); ++i)
        if (key == _list[i].first)
        {
            return _list[i].second;
        }

    _list += Pair(key, String());
    return _list[_list.size() - 1].second;
}

String Image_Tag::operator [] (const String & key) const
{
    return get(key);
}

const List<String> & Image_Tag::label_tag()
{
    static const List<String> data = List<String>() <<
        "Project" <<
        "Creator" <<
        "Description" <<
        "Copyright" <<
        "Time" <<
        "UTC Offset" <<
        "Keycode" <<
        "Timecode";

    DJV_ASSERT(data.size() == _TAG_SIZE);

    return data;
}

//------------------------------------------------------------------------------

bool operator == (const Image_Tag & a, const Image_Tag & b)
{
    if (a.keys() != b.keys())
    {
        return false;
    }

    if (a.values() != b.values())
    {
        return false;
    }

    return true;
}

bool operator != (const Image_Tag & a, const Image_Tag & b)
{
    return ! (a == b);
}

} // djv
