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

//! \file djv_list.h

#ifndef DJV_LIST_H
#define DJV_LIST_H

#include <djv_config.h>
#include <djv_core_export.h>
#include <djv_type.h>
#include <vector>

namespace djv
{

//------------------------------------------------------------------------------
//! \class List
//!
//! This class provides a list of items.
//------------------------------------------------------------------------------

template<typename T>
class List : public std::vector<T>
{
public:

    //! Constructor.

    inline List();

    //! Constructor.

    inline List(const List<T> &);

    //! Constructor.

    inline List(const List<T> &, size_t position, size_t size);

    //! Constructor.

    inline List(const T &, size_t size);

    //! Constructor.

    inline List(size_t size);

    //! Push an item onto the front of the list.

    inline void push_front(const T &);

    //! Pop an item from the front of the list.

    inline T pop_front();

    //! Pop an item from the back of the list.

    inline T pop_back();

    //! Add an item to the list.

    inline void add(const T &);

    //! Add an item to the list.

    inline void add(const List<T> &);

    inline List<T> & operator += (const T &);
    inline List<T> & operator += (const List<T> &);

    inline List<T> & operator << (const T &);
    inline List<T> & operator << (const List<T> &);
};

//------------------------------------------------------------------------------
//! \struct List_Util
//!
//! This struct provides utilities for lists.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT List_Util
{
    //! Find the index of an item within a list. If no match is found -1 is
    //! returned.

    template<typename T>
    static inline int find(const T &, const List<T> &);

    //! Return the unique items in a list.

    template<typename T>
    static inline List<T> unique(const List<T> &);

    //! Convert list types.

    template<typename A, typename B>
    static inline void convert(const List<A> &, List<B> &);

    //! Convert and append list types.

    template<typename A, typename B>
    static inline void convert_append(const List<A> &, List<B> &);
};

} // djv

#include <djv_list_inline.h>

#endif // DJV_LIST_H

