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

//! \file djv_vector_inline.h

#include <djv_math.h>

namespace djv
{

//------------------------------------------------------------------------------
// V<T, D>
//------------------------------------------------------------------------------

template<typename T, int D>
const int V<T, D>::dimension = D;

template<typename T, int D>
inline V<T, D>::V()
{
    zero();
}

template<typename T, int D>
inline V<T, D>::V(const V<T, D> & in)
{
    set(in);
}

template<typename T, int D>
inline V<T, D>::V(T in)
{
    set(in);
}

template<typename T, int D>
inline V<T, D> & V<T, D>::operator = (const V<T, D> & in)
{
    if (&in != this)
    {
        set(in);
    }

    return *this;
}

template<typename T, int D>
inline void V<T, D>::set(const V<T, D> & in)
{
    for (int i = 0; i < dimension; ++i)
    {
        V<T, D>::e[i] = in.e[i];
    }
}

template<typename T, int D>
inline void V<T, D>::set(T in)
{
    for (int i = 0; i < dimension; ++i)
    {
        e[i] = in;
    }
}

template<typename T, int D>
inline void V<T, D>::zero()
{
    for (int i = 0; i < dimension; ++i)
    {
        e[i] = 0;
    }
}

#define _VEC_OP(IN) \
    \
    template<typename T, int D> \
    inline V<T, D> & V<T, D>::operator IN (const V<T, D> & in) \
    { \
        for (int i = 0; i < dimension; ++i) \
        { \
            V<T, D>::e[i] IN in.e[i]; \
        } \
        \
        return *this; \
    }

_VEC_OP(+=)
_VEC_OP(-=)
_VEC_OP(*=)
_VEC_OP(/=)

#define _VEC_OP2(IN) \
    \
    template<typename T, int D> \
    inline V<T, D> & V<T, D>::operator IN (T in) \
    { \
        for (int i = 0; i < dimension; ++i) \
        { \
            V<T, D>::e[i] IN in; \
        } \
        \
        return *this; \
    }

_VEC_OP2(+=)
_VEC_OP2(-=)
_VEC_OP2(*=)
_VEC_OP2(/=)

//------------------------------------------------------------------------------
// Vector_Util
//------------------------------------------------------------------------------

template<typename T, int D>
inline V<T, D> Vector_Util::abs(const V<T, D> & in)
{
    V<T, D> out;

    for (int i = 0; i < in.dimension; ++i)
    {
        out.e[i] = Math::abs(in.e[i]);
    }

    return out;
}

template<typename T, int D>
inline V<T, D> Vector_Util::swap(const V<T, D> & in)
{
    V<T, D> out;

    for (int i = 0; i < in.dimension; ++i)
    {
        out.e[i] = in.e[in.dimension - 1 - i];
    }

    return out;
}

template<typename T, int D>
inline V<T, D> Vector_Util::min(const V<T, D> & a, const V<T, D> & b)
{
    V<T, D> out;

    min(a, b, &out);

    return out;
}

template<typename T, int D>
inline void Vector_Util::min(
    const V<T, D> & a,
    const V<T, D> & b,
    V<T, D> *       out)
{
    for (int i = 0; i < a.dimension; ++i)
    {
        Math::min(a.e[i], b.e[i], &out->e[i]);
    }
}

template<typename T, int D>
inline V<T, D> Vector_Util::max(const V<T, D> & a, const V<T, D> & b)
{
    V<T, D> out;

    max(a, b, &out);

    return out;
}

template<typename T, int D>
inline void Vector_Util::max(
    const V<T, D> & a,
    const V<T, D> & b,
    V<T, D> *       out)
{
    for (int i = 0; i < a.dimension; ++i)
    {
        Math::max(a.e[i], b.e[i], &out->e[i]);
    }
}

template<typename T, int D>
inline V<T, D> Vector_Util::clamp(
    const V<T, D> & in,
    const V<T, D> & min,
    const V<T, D> & max)
{
    V<T, D> out;

    for (int i = 0; i < in.dimension; ++i)
    {
        out.e[i] = Math::clamp(in.e[i], min.e[i], max.e[i]);
    }

    return out;
}

template<typename T, int D>
inline bool Vector_Util::is_size_valid(const V<T, D> & in)
{
    return in > V<T, D>(0);
}

template<typename T, int D>
inline double Vector_Util::length(const V<T, D> & in)
{
    double out = 0.0;

    for (int i = 0; i < in.dimension; ++i)
    {
        out += in.e[i] * in.e[i];
    }

    return Math::sqrt(out);
}

template<typename T, int D>
inline double Vector_Util::area(const V<T, D> & in)
{
    double out = static_cast<double>(in.e[0]);

    for (int i = 1; i < in.dimension; ++i)
    {
        out *= static_cast<double>(in.e[i]);
    }

    return Math::abs(out);
}

template<typename T, int D>
inline V<T, D> Vector_Util::normalize(const V<T, D> & in)
{
    V<T, D> out;

    const double l = length(in);

    if (l != 0.0)
        for (int i = 0; i < in.dimension; ++i)
        {
            out.e[i] = T(in.e[i] / l);
        }

    return out;
}

template<typename T, int D>
inline double Vector_Util::dot(const V<T, D> & a, const V<T, D> & b)
{
    double out = 0.0;

    for (int i = 0; i < a.dimension; ++i)
    {
        out += a.e[i] * b.e[i];
    }

    return out;
}

template<typename T, int D>
inline double Vector_Util::aspect(const V<T, D> & in)
{
    return in.e[1] != T(0) ? (in.e[0] / static_cast<double>(in.e[1])) : 0.0;
}

template<typename T, typename T2, int D>
inline V<T2, D> Vector_Util::convert(const V<T, D> & in)
{
    V<T2, D> out;

    for (int i = 0; i < in.dimension; ++i)
    {
        out.e[i] = T2(in.e[i]);
    }

    return out;
}

template<typename T, typename T2, int D>
inline V<T2, D> Vector_Util::ceil(const V<T, D> & in)
{
    V<T2, D> out;

    for (int i = 0; i < in.dimension; ++i)
    {
        out.e[i] = Math::ceil(in.e[i]);
    }

    return out;
}

template<typename T, typename T2, int D>
inline V<T2, D> Vector_Util::floor(const V<T, D> & in)
{
    V<T2, D> out;

    for (int i = 0; i < in.dimension; ++i)
    {
        out.e[i] = Math::floor(in.e[i]);
    }

    return out;
}

//------------------------------------------------------------------------------

#define _VEC_FNC_OP(IN) \
    \
    template<typename T, int D> \
    inline V<T, D> operator IN (const V<T, D> & a, const V<T, D> & b) \
    { \
        V<T, D> out; \
        \
        for (int i = 0; i < a.dimension; ++i) \
        { \
            out.e[i] = a.e[i] IN b.e[i]; \
        } \
        \
        return out; \
    }

_VEC_FNC_OP(+)
_VEC_FNC_OP(-)
_VEC_FNC_OP(*)
_VEC_FNC_OP(/)

#define _VEC_FNC_OP2(IN) \
    \
    template<typename T, int D> \
    inline V<T, D> operator IN (const V<T, D> & a, T b) \
    { \
        V<T, D> out; \
        \
        for (int i = 0; i < a.dimension; ++i) \
        { \
            out.e[i] = a.e[i] IN b; \
        } \
        \
        return out; \
    }

_VEC_FNC_OP2(+)
_VEC_FNC_OP2(-)
_VEC_FNC_OP2(*)
_VEC_FNC_OP2(/)

template<typename T, int D>
inline V<T, D> operator - (const V<T, D> & in)
{
    V<T, D> out;

    for (int i = 0; i < in.dimension; ++i)
    {
        out.e[i] = -in.e[i];
    }

    return out;
}

template<typename T, int D>
inline String & operator << (String & out, const V<T, D> & in)
{
    for (int i = 0; i < in.dimension; ++i)
    {
        out << in.e[i];
    }

    return out;
}

template<typename T, int D>
inline String & operator >> (String & in, V<T, D> & out) throw (String)
{
    for (int i = 0; i < out.dimension; ++i)
    {
        in >> out.e[i];
    }

    return in;
}

#define _VEC_COMPARE(IN) \
    \
    template<typename T, int D> \
    inline bool operator IN (const V<T, D> & a, const V<T, D> & b) \
    { \
        for (int i = 0; i < a.dimension; ++i) \
        { \
            if (! (a.e[i] IN b.e[i])) \
            { \
                return false; \
            } \
        } \
        \
        return true; \
    }

_VEC_COMPARE(==)
_VEC_COMPARE(< )
_VEC_COMPARE(<=)
_VEC_COMPARE(> )
_VEC_COMPARE(>=)

template<typename T, int D>
inline bool operator != (const V<T, D> & a, const V<T, D> & b)
{
    return ! (a == b);
}

template<typename T>
inline Debug & operator << (Debug & debug, const T & in)
{
    return debug << String_Util::label(in);
}

} // djv

