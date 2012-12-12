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

//! \file djv_set.h

#ifndef DJV_SET_H
#define DJV_SET_H

#include <djv_list.h>

#include <set>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Set
//!
//! This class provides a set of items.
//------------------------------------------------------------------------------

template<typename T>
class Set : public std::set<T>
{
public:

    //! Constructor.

    inline Set();

    //! Constructor.

    inline Set(const Set<T> &);

    //! Add an item to the set.

    inline void add(const T &);

    //! Add another set to this set.

    inline void add(const Set<T> &);

    //! Add a list to this set.

    inline void add(const List<T> &);

    inline Set<T> & operator += (const T &);
    inline Set<T> & operator += (const Set<T> &);
    inline Set<T> & operator += (const List<T> &);

    inline Set<T> & operator << (const T &);
    inline Set<T> & operator << (const Set<T> &);
    inline Set<T> & operator << (const List<T> &);

    inline operator List<T> () const;
};

} // djv

#include <djv_set_inline.h>

#endif // DJV_SET_H

