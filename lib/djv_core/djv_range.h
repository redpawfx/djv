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

//! \file djv_range.h

#ifndef DJV_RANGE_H
#define DJV_RANGE_H

#include <djv_debug.h>
#include <djv_list.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Range
//!
//! This class provides a number range.
//------------------------------------------------------------------------------

template<typename T>
struct Range
{
    //! Constructor.

    inline Range(T = T(0));

    //! Constructor.

    inline Range(T min, T max);

    //! Minimum value.

    T min;

    //! Maximum value.

    T max;

    inline bool operator == (const Range<T> &) const;

    inline bool operator != (const Range<T> &) const;
};

//------------------------------------------------------------------------------
//! \struct Range_Util
//!
//! This struct provides number range utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Range_Util
{
    //! Get whether a value is inside a range.

    template<typename T>
    static inline bool intersect(T, const Range<T> &);

    //! Get whether a value is inside a range.

    static inline bool intersect(int, const Range<int> &);

    //! Get the intersection of two ranges.

    template<typename T>
    static inline bool intersect(
        const Range<T> &,
        const Range<T> &,
        Range<T> *);

    //! Get the intersection of two ranges.

    static inline bool intersect(
        const Range<int> &,
        const Range<int> &,
        Range<int> *);

    //! Get the bound of two ranges.

    template<typename T>
    static inline void bound(const Range<T> &, const Range<T> &, Range<T> *);

    //! Convert a sorted list of positive integers into ranges.

    static inline List<Range<int64_t> > range(const List<int64_t> &);
};

//------------------------------------------------------------------------------

template<typename T>
inline Debug & operator << (Debug &, const Range<T> &);

} // djv

#include <djv_range_inline.h>

#endif // DJV_RANGE_H

