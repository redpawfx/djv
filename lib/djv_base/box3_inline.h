/*------------------------------------------------------------------------------
 include/djv_base/box3_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace box {

//------------------------------------------------------------------------------
// Box3
//------------------------------------------------------------------------------

template<typename T>
const int Box<T, 3>::dimension = 3;

#define _BOX3_INIT \
  \
  x(Box::position.e[0]), \
  y(Box::position.e[1]), \
  z(Box::position.e[2]), \
  w(Box::size.e[0]), \
  h(Box::size.e[1]), \
  d(Box::size.e[2])

template<typename T>
inline Box<T, 3>::Box() :
  _BOX3_INIT
{}

template<typename T>
inline Box<T, 3>::Box(const Box<T, 3> & in) :
  position(in.position),
  size(in.size),
  _BOX3_INIT
{}

template<typename T>
inline Box<T, 3>::Box(
  const vector::V<T, 3> & position,
  const vector::V<T, 3> & size
) :
  position(position),
  size(size),
  _BOX3_INIT
{}

template<typename T>
inline Box<T, 3>::Box(const vector::V<T, 3> & size) :
  size(size),
  _BOX3_INIT
{}

template<typename T>
inline Box<T, 3>::Box(T _x, T _y, T _z, T _w, T _h, T _d) :
  _BOX3_INIT
{
  x = _x;
  y = _y;
  z = _z;
  w = _w;
  h = _h;
  d = _d;
}

template<typename T>
inline Box<T, 3>::Box(T _w, T _h, T _d) :
  _BOX3_INIT
{
  w = _w;
  h = _h;
  d = _d;
}

template<typename T>
inline Box<T, 3> & Box<T, 3>::operator = (const Box<T, 3> & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}

template<typename T>
inline void Box<T, 3>::set(const Box<T, 3> & in)
{
  position = in.position;
  size = in.size;
}

template<typename T>
inline void Box<T, 3>::zero()
{
  position.zero();
  size.zero();
}

template<typename T>
inline void Box<T, 3>::lower_right(const vector::V<T, 3> & in)
{
  size = in - position;
}

template<typename T>
inline vector::V<T, 3> Box<T, 3>::lower_right() const
{
  return position + size;
}

template<typename T>
inline Box<T, 3> & Box<T, 3>::operator *= (const vector::V<T, 3> & in)
{
  position *= in;
  size *= in;
}

template<typename T>
inline Box<T, 3> & Box<T, 3>::operator /= (const vector::V<T, 3> & in)
{
  position /= in;
  size /= in;
}

template<typename T>
inline Box<T, 3> & Box<T, 3>::operator *= (T in)
{
  position *= in;
  size *= in;
}

template<typename T>
inline Box<T, 3> & Box<T, 3>::operator /= (T in)
{
  position /= in;
  size /= in;
}

// XXX Specialization for integer types.

template<>
inline void Box<int, 3>::lower_right(const vector::V<int, 3> & in)
{
  size = in - position + 1;
}

template<>
inline vector::V<int, 3> Box<int, 3>::lower_right() const
{
  return position + size - 1;
}

template<typename T>
inline Box<T, 3>::operator Box<int, 3> () const
{
  return Box<int, 3>(
    static_cast<int>(x), static_cast<int>(y), static_cast<int>(z),
    static_cast<int>(w), static_cast<int>(h), static_cast<int>(d)
  );
}

template<typename T>
inline Box<T, 3>::operator Box<double, 3> () const
{
  return Box<double, 3>(
    static_cast<double>(x), static_cast<double>(y), static_cast<double>(z),
    static_cast<double>(w), static_cast<double>(h), static_cast<double>(d)
  );
}

//------------------------------------------------------------------------------

}
}

