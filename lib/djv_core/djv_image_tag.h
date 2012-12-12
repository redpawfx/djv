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

//! \file djv_image_tag.h

#ifndef DJV_IMAGE_TAG_H
#define DJV_IMAGE_TAG_H

#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Image_Tag
//!
//! This class provides a collection of image tags.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Tag
{
public:

    //! Add image tags.

    void add(const Image_Tag &);

    //! Add an image tag.

    void add(const String & key, const String & value);

    //! Get an image tag.

    String get(const String & key) const;

    //! Get the list of keys.

    List<String> keys() const;

    //! Get the list of values.

    List<String> values() const;

    //! Get the number of tags.

    int size() const;

    //! Get whether the given key is valid.

    bool is_valid(const String & key);

    //! Remove all the tags.

    void clear();

    String & operator [] (const String & key);

    String operator [] (const String & key) const;

    //! Standard image tags.

    enum TAG
    {
        PROJECT,
        CREATOR,
        DESCRIPTION,
        COPYRIGHT,
        TIME,
        UTC_OFFSET,
        KEYCODE,
        TIMECODE,

        _TAG_SIZE
    };

    //! Get the image tag labels.

    static const List<String> & label_tag();

private:

    typedef std::pair<String, String> Pair;

    List<Pair> _list;
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT bool operator == (const Image_Tag &, const Image_Tag &);

DJV_CORE_EXPORT bool operator != (const Image_Tag &, const Image_Tag &);

} // djv

#endif // DJV_IMAGE_TAG_H

