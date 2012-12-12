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

//! \file djv_vector.h

#ifndef DJV_VECTOR_H
#define DJV_VECTOR_H

#include <djv_debug.h>
#include <djv_string.h>

namespace djv
{

template<typename T, int D>
struct V;
template<typename T>
struct V<T, 2>;
template<typename T>
struct V<T, 3>;

typedef V<bool, 2>   V2b;
typedef V<int, 2>    V2i;
typedef V<double, 2> V2f;
typedef V<bool, 3>   V3b;
typedef V<int, 3>    V3i;
typedef V<double, 3> V3f;

//------------------------------------------------------------------------------
//! \class V
//!
//! This class provides the base functionality for vectors.
//------------------------------------------------------------------------------

template<typename T, int D>
struct V
{
    //! Constructor.

    inline V();

    //! Constructor.

    inline V(const V<T, D> &);

    //! Constructor.

    inline V(T);

    V<T, D> & operator = (const V<T, D> &);

    //! Dimension.

    static const int dimension;

    //! Components.

    T e [D];

    //! Copy from another vector.

    inline void set(const V<T, D> &);

    //! Set the components.

    inline void set(T);

    //! Zero the components.

    inline void zero();

    inline V<T, D> & operator += (const V<T, D> &);
    inline V<T, D> & operator -= (const V<T, D> &);
    inline V<T, D> & operator *= (const V<T, D> &);
    inline V<T, D> & operator /= (const V<T, D> &);

    inline V<T, D> & operator += (T);
    inline V<T, D> & operator -= (T);
    inline V<T, D> & operator *= (T);
    inline V<T, D> & operator /= (T);
};

//------------------------------------------------------------------------------
//! \class V<T, 2>
//!
//! This class provides a two-dimensional vector.
//------------------------------------------------------------------------------

template<typename T>
struct V<T, 2>
{
    //! Constructor.

    inline V();

    //! Constructor.

    inline V(const V<T, 2> &);

    //! Constructor.

    inline V(T, T);

    //! Constructor.

    inline V(T);

    inline V<T, 2> & operator = (const V<T, 2> &);

    //! Dimension.

    static const int dimension;

    //! Components.

    T e [2];

    //! Components.

    T & x, & y;

    //! Copy from another vector.

    inline void set(const V<T, 2> &);

    //! Set the components.

    inline void set(T);

    //! Zero the components.

    inline void zero();

    inline V<T, 2> & operator += (const V<T, 2> &);
    inline V<T, 2> & operator -= (const V<T, 2> &);
    inline V<T, 2> & operator *= (const V<T, 2> &);
    inline V<T, 2> & operator /= (const V<T, 2> &);

    inline V<T, 2> & operator += (T);
    inline V<T, 2> & operator -= (T);
    inline V<T, 2> & operator *= (T);
    inline V<T, 2> & operator /= (T);

    inline operator V<int, 2> () const;
    inline operator V<double, 2> () const;
};

//------------------------------------------------------------------------------
//! \class V<T, 3>
//!
//! This class provides a three-dimensional vector.
//------------------------------------------------------------------------------

template<typename T>
struct V<T, 3>
{
    //! Constructor.

    inline V();

    //! Constructor.

    inline V(const V<T, 3> &);

    //! Constructor.

    inline V(T, T, T);

    //! Constructor.

    inline V(T);

    inline V<T, 3> & operator = (const V<T, 3> &);

    //! Dimension.

    static const int dimension;

    //! Components.

    T e [3];

    //! Components.

    T & x, & y, & z;

    //! Copy from another vector.

    inline void set(const V<T, 3> &);

    //! Set the components.

    inline void set(T);

    //! Zero the components.

    inline void zero();

    inline V<T, 3> & operator += (const V<T, 3> &);
    inline V<T, 3> & operator -= (const V<T, 3> &);
    inline V<T, 3> & operator *= (const V<T, 3> &);
    inline V<T, 3> & operator /= (const V<T, 3> &);

    inline V<T, 3> & operator += (T);
    inline V<T, 3> & operator -= (T);
    inline V<T, 3> & operator *= (T);
    inline V<T, 3> & operator /= (T);

    inline operator V<int, 3> () const;
    inline operator V<double, 3> () const;
};

//------------------------------------------------------------------------------
//! \struct Vector_Util
//!
//! This struct provides vector utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Vector_Util
{
    //! Get the absolute value of the vector components.

    template<typename T, int D>
    static inline V<T, D> abs(const V<T, D> &);

    //! Swap the vector components.

    template<typename T, int D>
    static inline V<T, D> swap(const V<T, D> &);

    //! Get the minimum of the vector components.

    template<typename T, int D>
    static inline V<T, D> min(const V<T, D> &, const V<T, D> &);

    //! Get the minimum of the vector components.

    template<typename T, int D>
    static inline void min(const V<T, D> &, const V<T, D> &, V<T, D> *);

    //! Get the maximum of the vector components.

    template<typename T, int D>
    static inline V<T, D> max(const V<T, D> &, const V<T, D> &);

    //! Get the maximum of the vector components.

    template<typename T, int D>
    static inline void max(const V<T, D> &, const V<T, D> &, V<T, D> *);

    //! Clamp the vector components.

    template<typename T, int D>
    static inline V<T, D> clamp(
        const V<T, D> &,
        const V<T, D> &,
        const V<T, D> &);

    //! Is the vector's size greater than zero?

    template<typename T, int D>
    static inline bool is_size_valid(const V<T, D> &);

    //! Get the length of a vector.

    template<typename T, int D>
    static inline double length(const V<T, D> &);

    //! Get the area of a vector.

    template<typename T, int D>
    static inline double area(const V<T, D> &);

    //! Normalize a vector.

    template<typename T, int D>
    static inline V<T, D> normalize(const V<T, D> &);

    //! Vector dot product.

    template<typename T, int D>
    static inline double dot(const V<T, D> &, const V<T, D> &);

    //! Aspect ratio of a vector.

    template<typename T, int D>
    static inline double aspect(const V<T, D> &);

    //! Convert vector types.

    template<typename T, typename T2, int D>
    static inline V<T2, D> convert(const V<T, D> &);

    //! Round to the smallest integer value.

    template<typename T, typename T2, int D>
    static inline V<T2, D> ceil(const V<T, D> &);

    //! Round to the largest integer value.

    template<typename T, typename T2, int D>
    static inline V<T2, D> floor(const V<T, D> &);

    //! Convert spherical to cartesian coordinates.

    static void spherical_to_cartesian(const V3f &, V3f *);

    //! Convert cartesian to spherical coordinates.

    static void cartesian_to_spherical(const V3f &, V3f *);
};

//------------------------------------------------------------------------------

template<typename T, int D>
inline V<T, D> operator + (const V<T, D> &, const V<T, D> &);
template<typename T, int D>
inline V<T, D> operator - (const V<T, D> &, const V<T, D> &);
template<typename T, int D>
inline V<T, D> operator * (const V<T, D> &, const V<T, D> &);
template<typename T, int D>
inline V<T, D> operator / (const V<T, D> &, const V<T, D> &);

template<typename T, int D>
inline V<T, D> operator + (const V<T, D> &, T);
template<typename T, int D>
inline V<T, D> operator - (const V<T, D> &, T);
template<typename T, int D>
inline V<T, D> operator * (const V<T, D> &, T);
template<typename T, int D>
inline V<T, D> operator / (const V<T, D> &, T);

//! Negate.

template<typename T, int D>
inline V<T, D> operator - (const V<T, D> &);

template<typename T, int D>
inline String & operator >> (String &, V<T, D> &) throw (String);

template<typename T, int D>
inline String & operator << (String &, const V<T, D> &);

template<typename T, int D>
inline bool operator == (const V<T, D> &, const V<T, D> &);

template<typename T, int D>
inline bool operator != (const V<T, D> &, const V<T, D> &);

template<typename T, int D>
inline bool operator <  (const V<T, D> &, const V<T, D> &);
template<typename T, int D>
inline bool operator <= (const V<T, D> &, const V<T, D> &);

template<typename T, int D>
inline bool operator >  (const V<T, D> &, const V<T, D> &);
template<typename T, int D>
inline bool operator >= (const V<T, D> &, const V<T, D> &);

template<typename T>
inline Debug & operator << (Debug &, const T &);

} // djv

#include <djv_vector_inline.h>
#include <djv_vector2_inline.h>
#include <djv_vector3_inline.h>

#endif // DJV_VECTOR_H

