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

//! \file djv_matrix.h

#ifndef DJV_MATRIX_H
#define DJV_MATRIX_H

#include <djv_vector.h>
#include <djv_box.h>

namespace djv
{

template<typename T, int D>
struct M;
template<typename T>
struct M<T, 3>;
template<typename T>
struct M<T, 4>;

typedef M<double, 3> M3f;
typedef M<double, 4> M4f;

//------------------------------------------------------------------------------
//! \class M
//!
//! This class provides the base functionality for matrices.
//------------------------------------------------------------------------------

template<typename T, int D>
struct M
{
    //! Constructor.

    inline M();

    //! Constructor.

    inline M(const M<T, D> &);

    //! Constructor.

    inline M(const T *);

    inline M<T, D> & operator = (const M<T, D> &);

    //! Dimension.

    static const int dimension;

    //! Dimension.

    static const int dimension_x2;

    //! Components.

    T e [D * D];

    //! Copy from another matrix.

    inline void set(const M<T, D> &);

    //! Set the components.

    inline void set(const T *);

    //! Zero the components.

    inline void zero();

    //! Set to the identity matrix.

    inline void identity();
};

//------------------------------------------------------------------------------
//! \class M<T, 3>
//!
//! This class provides a 3x3 matrix.
//------------------------------------------------------------------------------

template<typename T>
struct M<T, 3>
{
    //! Constructor.

    inline M();

    //! Constructor.

    inline M(const M<T, 3> &);

    //! Constructor.

    inline M(T, T, T, T, T, T, T, T, T);

    //! Constructor.

    inline M(const T *);

    inline M<T, 3> & operator = (const M<T, 3> &);

    //! Dimension.

    static const int dimension;

    //! Dimension.

    static const int dimension_x2;

    //! Components.

    T e [3 * 3];

    //! Copy from another matrix.

    inline void set(const M<T, 3> &);

    //! Set the components.

    inline void set(const T *);

    //! Set the components.

    inline void set(T, T, T, T, T, T, T, T, T);

    //! Zero the components.

    inline void zero();

    //! Set to the identity matrix.

    inline void identity();
};

//------------------------------------------------------------------------------
//! \class M<T, 4>
//!
//! This class provides a 4x4 matrix.
//------------------------------------------------------------------------------

template<typename T>
struct M<T, 4>
{
    //! Constructor.

    inline M();

    //! Constructor.

    inline M(const M<T, 4> &);

    //! Constructor.

    inline M(T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T);

    //! Constructor.

    inline M(const T *);

    inline M<T, 4> & operator = (const M<T, 4> &);

    //! Dimension.

    static const int dimension;

    //! Dimension.

    static const int dimension_x2;

    //! Components.

    T e [4 * 4];

    //! Copy from another matrix.

    inline void set(const M<T, 4> &);

    //! Set the components.

    inline void set(const T *);

    //! Set the components.

    inline void set(T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T);

    //! Zero the components.

    inline void zero();

    //! Set to the identity matrix.

    inline void identity();
};

//------------------------------------------------------------------------------
//! \struct Matrix_Util
//!
//! This struct provides matrix utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Matrix_Util
{
    //! Transpose a matrix.

    template<typename T, int D>
    static inline M<T, D> transpose(const M<T, D> &);

    //! Create a translation matrix.

    template<typename T, int D, int D2>
    static inline M<T, D> translate(const V<T, D2> &);

    //! Create a translation matrix.

    static inline M3f translate3f(const V2f &);

    //! Create a scale matrix.

    template<typename T, int D, int D2>
    static inline M<T, D> scale(const V<T, D2> &);

    //! Create a scale matrix.

    static inline M3f scale3f(const V2f &);

    //! Create a rotation matrix.

    static inline M3f rotate3f(double);

    //! Convert matrix types.

    template <typename T, typename T2, int D>
    static inline M<T2, D> convert(const M<T, D> &);

    //! Convert a 3x3 matrix to a 4x4 matrix.

    template <typename T>
    static inline M<T, 4> matrix4(const M<T, 3> &);
};

//------------------------------------------------------------------------------

template<typename T, int D>
inline M<T, D> operator * (const M<T, D> &, const M<T, D> &);

//! Transform a point by a matrix.

template<typename T, int D, int D2>
inline V<T, D2> operator * (const M<T, D> &, const V<T, D2> &);

//! Transform a box by a matrix.

template<typename T>
inline Box<T, 2> operator * (const M<T, 3> &, const Box<T, 2> &);

template<typename T, int D>
inline String & operator >> (String &, M<T, D> &) throw (String);

template<typename T, int D>
inline String & operator << (String &, const M<T, D> &);

template<typename T, int D>
inline bool operator == (const M<T, D> &, const M<T, D> &);

template<typename T, int D>
inline bool operator != (const M<T, D> &, const M<T, D> &);

template<typename T, int D>
inline Debug & operator << (Debug &, const M<T, D> &);

} // djv

#include <djv_matrix_inline.h>
#include <djv_matrix3_inline.h>
#include <djv_matrix4_inline.h>

#endif // DJV_MATRIX_H
