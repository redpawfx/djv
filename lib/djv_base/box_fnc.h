/*------------------------------------------------------------------------------
 include/djv_base/box_fnc.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace box {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Create a box with the given corners.

template<typename T, int D>
inline Box<T, D> corners(
  const vector::V<T, D> & upper_left,
  const vector::V<T, D> & lower_right
);

//! Swap the box position and size components.

template<typename T, int D>
inline Box<T, D> swap(const Box<T, D> &);

//! Intersection of boxes.

template<typename T, int D>
inline Box<T, D> intersect(const Box<T, D> &, const Box<T, D> &);

template<typename T, int D>
inline bool intersect(const Box<T, D> &, const vector::V<T, D> &);

//! Union of boxes.

template<typename T, int D>
inline Box<T, D> bound(const Box<T, D> &, const Box<T, D> &);

//! Expand a box to contain the given point.

template<typename T, int D>
inline Box<T, D> expand(const Box<T, D> &, const vector::V<T, D> &);

//! Add a border to a box.

template<typename T, int D>
inline Box<T, D> border(const Box<T, D> &, const vector::V<T, D> &);

//! Is the box's size greater than zero?

template<typename T, int D>
inline bool is_size_valid(const Box<T, D> &);

//@}
//------------------------------------------------------------------------------
//!\name Operators
//------------------------------------------------------------------------------
//@{

template <typename T, int D>
inline Box<T, D> operator + (const Box<T, D> &, T);
template <typename T, int D>
inline Box<T, D> operator - (const Box<T, D> &, T);
template <typename T, int D>
inline Box<T, D> operator * (const Box<T, D> &, T);
template <typename T, int D>
inline Box<T, D> operator / (const Box<T, D> &, T);

template <typename T, int D>
inline Box<T, D> operator + (const Box<T, D> &, const vector::V<T, D> &);
template <typename T, int D>
inline Box<T, D> operator - (const Box<T, D> &, const vector::V<T, D> &);
template <typename T, int D>
inline Box<T, D> operator * (const Box<T, D> &, const vector::V<T, D> &);
template <typename T, int D>
inline Box<T, D> operator / (const Box<T, D> &, const vector::V<T, D> &);

//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

template <typename T, int D>
inline bool operator == (const Box<T, D> &, const Box<T, D> &);

template <typename T, int D>
inline bool operator != (const Box<T, D> &, const Box<T, D> &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

template <typename T, typename T2, int D>
inline Box<T2, D> convert(const Box<T, D> &);

template<typename T, int D>
inline String & operator >> (String &, Box<T, D> &) throw (String);

template<typename T, int D>
inline String & operator << (String &, const Box<T, D> &);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

template <typename T, int D>
inline Debug & operator << (Debug &, const Box<T, D> &);

//@}
//------------------------------------------------------------------------------

}
}

