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

//! \file djv_box.h

#ifndef DJV_BOX_H
#define DJV_BOX_H

#include <djv_vector.h>

namespace djv
{

template<typename T, int D>
struct Box;
template<typename T>
struct Box<T, 2>;
template<typename T>
struct Box<T, 3>;

typedef Box<int, 2>    Box2i;
typedef Box<double, 2> Box2f;
typedef Box<int, 3>    Box3i;
typedef Box<double, 3> Box3f;

//------------------------------------------------------------------------------
//! \class Box
//!
//! This class provides the base functionality for axis-aligned boxes.
//------------------------------------------------------------------------------

template<typename T, int D>
struct Box
{
    //! Constructor.

    inline Box();

    //! Constructor.

    inline Box(const Box<T, D> &);

    //! Constructor.

    inline Box(const V<T, D> & position, const V<T, D> & size);

    //! Constructor.

    inline Box(const V<T, D> & size);

    //! Dimension.

    static const int dimension;

    //! Position.

    V<T, D> position;

    //! Size.

    V<T, D> size;

    //! Copy from another box.

    inline void set(const Box<T, D> &);

    //! Zero the box.

    inline void zero();

    //! Set the lower right hand corner.

    inline void lower_right(const V<T, D> &);

    //! Get the lower right hand corner.

    inline V<T, D> lower_right() const;

    inline Box<T, D> & operator *= (const V<T, D> &);
    inline Box<T, D> & operator /= (const V<T, D> &);

    inline Box<T, D> & operator *= (T);
    inline Box<T, D> & operator /= (T);
};

//------------------------------------------------------------------------------
//! \class Box<T, 2>
//!
//! This class provides a two-dimensional axis-aligned box.
//------------------------------------------------------------------------------

template<typename T>
struct Box<T, 2>
{
    //! Constructor.

    inline Box();

    //! Constructor.

    inline Box(const Box<T, 2> &);

    //! Constructor.

    inline Box(const V<T, 2> & position, const V<T, 2> & size);

    //! Constructor.

    inline Box(const V<T, 2> & size);

    //! Constructor.

    inline Box(T x, T y, T w, T h);

    //! Constructor.

    inline Box(T w, T h);

    //! Constructor.

    inline Box<T, 2> & operator = (const Box<T, 2> &);

    //! Dimension.

    static const int dimension;

    //! Position.

    V<T, 2> position;

    //! Size.

    V<T, 2> size;

    //! Components.

    T & x, & y, & w, & h;

    //! Copy from another box.

    inline void set(const Box<T, 2> &);

    //! Zero the box.

    inline void zero();

    //! Set the lower right hand corner.

    inline void lower_right(const V<T, 2> &);

    //! Get the lower right hand corner.

    inline V<T, 2> lower_right() const;

    inline Box<T, 2> & operator *= (const V<T, 2> &);
    inline Box<T, 2> & operator /= (const V<T, 2> &);

    inline Box<T, 2> & operator *= (T);
    inline Box<T, 2> & operator /= (T);

    inline operator Box<int, 2> () const;
    inline operator Box<double, 2> () const;
};

//------------------------------------------------------------------------------
//! \class Box<T, 3>
//!
//! This class provides a three-dimensional axis-aligned box.
//------------------------------------------------------------------------------

template<typename T>
struct Box<T, 3>
{
    //! Constructor.

    inline Box();

    //! Constructor.

    inline Box(const Box<T, 3> &);

    //! Constructor.

    inline Box(const V<T, 3> & position, const V<T, 3> & size);

    //! Constructor.

    inline Box(const V<T, 3> & size);

    //! Constructor.

    inline Box(T x, T y, T z, T w, T h, T d);

    //! Constructor.

    inline Box(T w, T h, T d);

    //! Constructor.

    inline Box<T, 3> & operator = (const Box<T, 3> &);

    //! Dimension.

    static const int dimension;

    //! Position.

    V<T, 3> position;

    //! Size.

    V<T, 3> size;

    //! Components.

    T & x, & y, & z, & w, & h, & d;

    //! Copy from another box.

    inline void set(const Box<T, 3> &);

    //! Zero the box.

    inline void zero();

    //! Set the lower right hand corner.

    inline void lower_right(const V<T, 3> &);

    //! Get the lower right hand corner.

    inline V<T, 3> lower_right() const;

    inline Box<T, 3> & operator *= (const V<T, 3> &);
    inline Box<T, 3> & operator /= (const V<T, 3> &);

    inline Box<T, 3> & operator *= (T);
    inline Box<T, 3> & operator /= (T);

    inline operator Box<int, 3> () const;
    inline operator Box<double, 3> () const;
};

//------------------------------------------------------------------------------
//! \struct Box_Util
//!
//! This struct provides box utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Box_Util
{
    //! Create a box with the given corners.

    template<typename T, int D>
    static inline Box<T, D> corners(
        const V<T, D> & upper_left,
        const V<T, D> & lower_right);

    //! Swap the box position and size components.

    template<typename T, int D>
    static inline Box<T, D> swap(const Box<T, D> &);

    //! Get the intersection of two boxes.

    template<typename T, int D>
    static inline Box<T, D> intersect(const Box<T, D> &, const Box<T, D> &);

    //! Get whether a point is inside a box.

    template<typename T, int D>
    static inline bool intersect(const Box<T, D> &, const V<T, D> &);

    //! Get the union of two boxes.

    template<typename T, int D>
    static inline Box<T, D> bound(const Box<T, D> &, const Box<T, D> &);

    //! Expand a box to contain the given point.

    template<typename T, int D>
    static inline Box<T, D> expand(const Box<T, D> &, const V<T, D> &);

    //! Add a border to a box.

    template<typename T, int D>
    static inline Box<T, D> border(const Box<T, D> &, const V<T, D> &);

    //! Is the box's size greater than zero?

    template<typename T, int D>
    static inline bool is_size_valid(const Box<T, D> &);

    //! Convert box types.

    template <typename T, typename T2, int D>
    static inline Box<T2, D> convert(const Box<T, D> &);
};

//------------------------------------------------------------------------------

template <typename T, int D>
inline Box<T, D> operator + (const Box<T, D> &, T);
template <typename T, int D>
inline Box<T, D> operator - (const Box<T, D> &, T);
template <typename T, int D>
inline Box<T, D> operator * (const Box<T, D> &, T);
template <typename T, int D>
inline Box<T, D> operator / (const Box<T, D> &, T);

template <typename T, int D>
inline Box<T, D> operator + (const Box<T, D> &, const V<T, D> &);
template <typename T, int D>
inline Box<T, D> operator - (const Box<T, D> &, const V<T, D> &);
template <typename T, int D>
inline Box<T, D> operator * (const Box<T, D> &, const V<T, D> &);
template <typename T, int D>
inline Box<T, D> operator / (const Box<T, D> &, const V<T, D> &);

template <typename T, int D>
inline bool operator == (const Box<T, D> &, const Box<T, D> &);

template <typename T, int D>
inline bool operator != (const Box<T, D> &, const Box<T, D> &);

template<typename T, int D>
inline String & operator >> (String &, Box<T, D> &) throw (String);

template<typename T, int D>
inline String & operator << (String &, const Box<T, D> &);

template <typename T, int D>
inline Debug & operator << (Debug &, const Box<T, D> &);

} // djv

#include <djv_box_inline.h>
#include <djv_box2_inline.h>
#include <djv_box3_inline.h>

#endif // DJV_BOX_H

