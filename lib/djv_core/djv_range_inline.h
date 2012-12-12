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

//! \file djv_range_inline.h

#include <djv_math.h>

namespace djv
{

//------------------------------------------------------------------------------
// Range
//------------------------------------------------------------------------------

template<typename T>
inline Range<T>::Range(T in) :
    min(in),
    max(in)
{}

template<typename T>
inline Range<T>::Range(T min, T max) :
    min(min),
    max(max)
{}

template<typename T>
inline bool Range<T>::operator == (const Range<T> & other) const
{
    return min == other.min && max == other.max;
}

template<typename T>
inline bool Range<T>::operator != (const Range<T> & other) const
{
    return ! (*this == other);
}

//------------------------------------------------------------------------------
// Range_Util
//------------------------------------------------------------------------------

template<typename T>
inline bool Range_Util::intersect(T in, const Range<T> & range)
{
    return in >= range.min && in < range.max;
}

inline bool Range_Util::intersect(int in, const Range<int> & range)
{
    return in >= range.min && in < range.max + 1;
}

template<typename T>
inline bool Range_Util::intersect(
    const Range<T> & a,
    const Range<T> & b,
    Range<T> * out)
{
    out->min = Math::max(a.min, b.min);
    out->max = Math::min(a.max, b.max);
    return out->max > out->min;
}

inline bool Range_Util::intersect(const Range<int> & a, const Range<int> & b,
                                  Range<int> * out)
{
    out->min = Math::max(a.min, b.min);
    out->max = Math::min(a.max, b.max);
    return (out->max + 1) > out->min;
}

template<typename T>
inline void Range_Util::bound(
    const Range<T> & a,
    const Range<T> & b,
    Range<T> * out)
{
    out->min = Math::min(a.min, b.min);
    out->max = Math::max(a.max, b.max);
}

inline List<Range<int64_t> > Range_Util::range(const List<int64_t> & in)
{
    List<Range<int64_t> > out;

    if (in.size())
    {
        out += Range<int64_t>(in[0], in[0]);
    }

    for (size_t i = 1; i < in.size(); ++i)
        if (in[i] - 1 != out[out.size() - 1].max)
        {
            out += Range<int64_t>(in[i], in[i]);
        }
        else
        {
            out[out.size() - 1].max = in[i];
        }

    return out;
}

//------------------------------------------------------------------------------

template<class T>
inline Debug & operator << (Debug & debug, const Range<T> & in)
{
    return debug << String_Format("%%-%%").arg(in.min).arg(in.max);
}

} // djv

