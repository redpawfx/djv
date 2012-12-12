/*------------------------------------------------------------------------------
 include/djv_base/vector_fnc.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace vector {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Get the absolute value of the vector components.

template<typename T, int D>
inline V<T, D> abs(const V<T, D> &);

//! Swap the vector components.

template<typename T, int D>
inline V<T, D> swap(const V<T, D> &);

//! Get the minimum of the vector components.

template<typename T, int D>
inline V<T, D> min(const V<T, D> &, const V<T, D> &);

template<typename T, int D>
inline void min(const V<T, D> &, const V<T, D> &, V<T, D> *);

//! Get the maximum of the vector components.

template<typename T, int D>
inline V<T, D> max(const V<T, D> &, const V<T, D> &);

template<typename T, int D>
inline void max(const V<T, D> &, const V<T, D> &, V<T, D> *);

//! Clamp the vector components.

template<typename T, int D>
inline V<T, D> clamp(const V<T, D> &, const V<T, D> &, const V<T, D> &);

//! Is the vector's size greater than zero?

template<typename T, int D>
inline bool is_size_valid(const V<T, D> &);

//! Get the length of a vector.

template<typename T, int D>
inline double length(const V<T, D> &);

//! Get the area of a vector.

template<typename T, int D>
inline double area(const V<T, D> &);

//! Normalize a vector.

template<typename T, int D>
inline V<T, D> normalize(const V<T, D> &);

//! Vector dot product.

template<typename T, int D>
inline double dot(const V<T, D> &, const V<T, D> &);

//! Aspect ratio of a vector.

template<typename T, int D>
inline double aspect(const V<T, D> &);

//@}
//------------------------------------------------------------------------------
//!\name Arithmetic
//------------------------------------------------------------------------------
//@{

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

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

template<typename T, typename T2, int D>
inline V<T2, D> convert(const V<T, D> &);

template<typename T, typename T2, int D>
inline V<T2, D> ceil(const V<T, D> &);

template<typename T, typename T2, int D>
inline V<T2, D> floor(const V<T, D> &);

void spherical_to_cartesian(const V3f &, V3f *);

void cartesian_to_spherical(const V3f &, V3f *);

template<typename T, int D>
inline String & operator >> (String &, V<T, D> &) throw (String);

template<typename T, int D>
inline String & operator << (String &, const V<T, D> &);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

template<typename T>
inline bool operator == (const T &, const T &);

template<typename T>
inline bool operator != (const T &, const T &);

template<typename T>
inline bool operator <  (const T &, const T &);
template<typename T>
inline bool operator <= (const T &, const T &);

template<typename T>
inline bool operator >  (const T &, const T &);
template<typename T>
inline bool operator >= (const T &, const T &);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

template<typename T>
inline Debug & operator << (Debug &, const T &);

//@}
//------------------------------------------------------------------------------

}
}

