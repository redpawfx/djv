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

//! \file djv_box_inline.h

#include <djv_range.h>

namespace djv
{

//------------------------------------------------------------------------------
// Box<T, D>
//------------------------------------------------------------------------------

template<typename T, int D>
const int Box<T, D>::dimension = D;

template<typename T, int D>
inline Box<T, D>::Box() {}

template<typename T, int D>
inline Box<T, D>::Box(const Box<T, D> & in)
{
    set(in);
}

template<typename T, int D>
inline Box<T, D>::Box(const V<T, D> & position, const V<T, D> & size) :
    position(position),
    size(size)
{}

template<typename T, int D>
inline Box<T, D>::Box(const V<T, D> & size) : size(size) {}

template<typename T, int D>
inline void Box<T, D>::set(const Box<T, D> & in)
{
    position = in.position;
    size = in.size;
}

template<typename T, int D>
inline void Box<T, D>::zero()
{
    position.zero();
    size.zero();
}

template<typename T, int D>
inline void Box<T, D>::lower_right(const V<T, D> & in)
{
    size = in - position;
}

template<typename T, int D>
inline V<T, D> Box<T, D>::lower_right() const
{
    return position + size;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator *= (const V<T, D> & in)
{
    position *= in;
    size *= in;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator /= (const V<T, D> & in)
{
    position /= in;
    size /= in;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator *= (T in)
{
    position *= in;
    size *= in;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator /= (T in)
{
    position /= in;
    size /= in;
}

//------------------------------------------------------------------------------
// Box_Util
//------------------------------------------------------------------------------

template<typename T, int D>
inline Box<T, D> Box_Util::corners(
    const V<T, D> & upper_left,
    const V<T, D> & lower_right)
{
    const V<T, D> & a = Vector_Util::min(upper_left, lower_right);
    const V<T, D> & b = Vector_Util::max(upper_left, lower_right);

    Box<T, D> out;

    out.position = a;
    out.lower_right(b);

    return out;
}

template<typename T, int D>
inline Box<T, D> Box_Util::swap(const Box<T, D> & in)
{
    return Box<T, D>(
        Vector_Util::swap(in.position), Vector_Util::swap(in.size));
}

template<typename T, int D>
inline bool Box_Util::intersect(const Box<T, D> & a, const V<T, D> & b)
{
    const V<T, D> a1 = a.lower_right();

    for (int i = 0; i < a.dimension; ++i)
    {
        if (! Range_Util::intersect(b.e[i], Range<T>(a.position.e[i], a1.e[i])))
        {
            return false;
        }
    }

    return true;
}

template<typename T, int D>
inline Box<T, D> Box_Util::intersect(const Box<T, D> & a, const Box<T, D> & b)
{
    const V<T, D> a1 = a.lower_right();
    const V<T, D> b1 = b.lower_right();

    Box<T, D> out;
    V<T, D> lower_right;

    for (int i = 0; i < a.dimension; ++i)
    {
        Range<T> tmp;
        Range_Util::intersect(
            Range<T>(a.position.e[i], a1.e[i]),
            Range<T>(b.position.e[i], b1.e[i]),
            &tmp);
        out.position.e[i] = tmp.min;
        lower_right.e[i] = tmp.max;
    }

    out.lower_right(lower_right);

    return out;
}

template<typename T, int D>
inline Box<T, D> Box_Util::bound(const Box<T, D> & a, const Box<T, D> & b)
{
    const V<T, D> a1 = a.lower_right();
    const V<T, D> b1 = b.lower_right();

    Box<T, D> out;
    V<T, D> lower_right;

    for (int i = 0; i < a.dimension; ++i)
    {
        Range<T> tmp;
        Range_Util::bound(
            Range<T>(a.position.e[i], a1.e[i]),
            Range<T>(b.position.e[i], b1.e[i]),
            &tmp);
        out.position.e[i] = tmp.min;
        lower_right.e[i] = tmp.max;
    }

    out.lower_right(lower_right);

    return out;
}

template<typename T, int D>
inline Box<T, D> Box_Util::expand(const Box<T, D> & in, const V<T, D> & expand)
{
    return corners(
        Vector_Util::min(in.position, expand),
        Vector_Util::max(in.lower_right(), expand));
}

template<typename T, int D>
inline Box<T, D> Box_Util::border(const Box<T, D> & in, const V<T, D> & border)
{
    return Box<T, D>(in.position - border, in.size + border * T(2));
}

template<typename T, int D>
inline bool Box_Util::is_size_valid(const Box<T, D> & in)
{
    return Vector_Util::is_size_valid(in.size);
}

template<typename T, typename T2, int D>
inline Box<T2, D> Box_Util::convert(const Box<T, D> & in)
{
    Box<T2, D> out;

    out.position = Vector_Util::convert<T, T2, D>(in.position);
    out.size     = Vector_Util::convert<T, T2, D>(in.size);

    return out;
}

//------------------------------------------------------------------------------

#define _BOX_FNC_OP(IN) \
    \
    template<typename T, int D> \
    inline Box<T, D> operator IN (const Box<T, D> & a, T b) \
    { \
        return Box<T, D>(a.position IN b, a.size IN b); \
    }

_BOX_FNC_OP(+)
_BOX_FNC_OP(-)
_BOX_FNC_OP(*)
_BOX_FNC_OP(/)

#define _BOX_FNC_OP2(IN) \
    \
    template<typename T, int D> \
    inline Box<T, D> operator IN (const Box<T, D> & a, const V<T, D> & b) \
    { \
        return Box<T, D>(a.position IN b, a.size IN b); \
    }

_BOX_FNC_OP2(+)
_BOX_FNC_OP2(-)
_BOX_FNC_OP2(*)
_BOX_FNC_OP2(/)

template<typename T, int D>
inline String & operator << (String & out, const Box<T, D> & in)
{
    return out << in.position << in.size;
}

template<typename T, int D>
inline String & operator >> (String & in, Box<T, D> & out) throw (String)
{
    return in >> out.position >> out.size;
}

template<typename T, int D>
inline bool operator == (const Box<T, D> & a, const Box<T, D> & b)
{
    return a.position == b.position && a.size == b.size;
}

template<typename T, int D>
inline bool operator != (const Box<T, D> & a, const Box<T, D> & b)
{
    return ! (a == b);
}

template<typename T, int D>
inline Debug & operator << (Debug & debug, const Box<T, D> & in)
{
    return debug << String_Util::label(in);
}

} // djv

