/*------------------------------------------------------------------------------
 include/djv_base/matrix_fnc.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace matrix {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

template<typename T, int D>
inline M<T, D> transpose(const M<T, D> &);

//! Create a translation matrix.

template<typename T, int D, int D2>
inline M<T, D> translate(const vector::V<T, D2> &);

inline M3f translate3f(const V2f &);

//! Create a scale matrix.

template<typename T, int D, int D2>
inline M<T, D> scale(const vector::V<T, D2> &);

inline M3f scale3f(const V2f &);

//! Create a rotation matrix.

inline M3f rotate3f(double);

//@}
//------------------------------------------------------------------------------
//!\name Arithmetic
//------------------------------------------------------------------------------
//@{

template<typename T, int D>
inline M<T, D> operator * (const M<T, D> &, const M<T, D> &);

//@}
//------------------------------------------------------------------------------
//!\name Operators
//------------------------------------------------------------------------------
//@{

//! Transform a point by a matrix.

template<typename T, int D, int D2>
inline vector::V<T, D2> operator * (const M<T, D> &, const vector::V<T, D2> &);

//! Transform a box by a matrix.

template<typename T>
inline box::Box<T, 2> operator * (const M<T, 3> &, const box::Box<T, 2> &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

template <typename T, typename T2, int D>
inline M<T2, D> convert(const M<T, D> &);

template <typename T>
M<T, 4> matrix4(const M<T, 3> &);

template<typename T, int D>
inline String & operator >> (String &, M<T, D> &) throw (String);

template<typename T, int D>
inline String & operator << (String &, const M<T, D> &);

//@}
//------------------------------------------------------------------------------

}
}

