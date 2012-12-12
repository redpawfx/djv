/*------------------------------------------------------------------------------
 include/djv_base/box2_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace box {

//------------------------------------------------------------------------------
// Box2
//------------------------------------------------------------------------------

template<typename T>
const int Box<T, 2>::dimension = 2;

#define _BOX2_INIT \
  \
  x(Box::position.e[0]), \
  y(Box::position.e[1]), \
  w(Box::size.e[0]), \
  h(Box::size.e[1])

template<typename T>
inline Box<T, 2>::Box() :
  _BOX2_INIT
{}

template<typename T>
inline Box<T, 2>::Box(const Box<T, 2> & in) :
  position(in.position),
  size(in.size),
  _BOX2_INIT
{}

template<typename T>
inline Box<T, 2>::Box(
  const vector::V<T, 2> & position,
  const vector::V<T, 2> & size
) :
  position(position),
  size(size),
  _BOX2_INIT
{}

template<typename T>
inline Box<T, 2>::Box(const vector::V<T, 2> & size) :
  size(size),
  _BOX2_INIT
{}

template<typename T>
inline Box<T, 2>::Box(T _x, T _y, T _w, T _h) :
  _BOX2_INIT
{
  x = _x;
  y = _y;
  w = _w;
  h = _h;
}

template<typename T>
inline Box<T, 2>::Box(T _w, T _h) :
  _BOX2_INIT
{
  w = _w;
  h = _h;
}

template<typename T>
inline Box<T, 2> & Box<T, 2>::operator = (const Box<T, 2> & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}

template<typename T>
inline void Box<T, 2>::set(const Box<T, 2> & in)
{
  position = in.position;
  size = in.size;
}

template<typename T>
inline void Box<T, 2>::zero()
{
  position.zero();
  size.zero();
}

template<typename T>
inline void Box<T, 2>::lower_right(const vector::V<T, 2> & in)
{
  size = in - position;
}

template<typename T>
inline vector::V<T, 2> Box<T, 2>::lower_right() const
{
  return position + size;
}

template<typename T>
inline Box<T, 2> & Box<T, 2>::operator *= (const vector::V<T, 2> & in)
{
  position *= in;
  size *= in;
}

template<typename T>
inline Box<T, 2> & Box<T, 2>::operator /= (const vector::V<T, 2> & in)
{
  position /= in;
  size /= in;
}

template<typename T>
inline Box<T, 2> & Box<T, 2>::operator *= (T in)
{
  position *= in;
  size *= in;
}

template<typename T>
inline Box<T, 2> & Box<T, 2>::operator /= (T in)
{
  position /= in;
  size /= in;
}

// XXX Specialization for integer types.

template<>
inline void Box<int, 2>::lower_right(const vector::V<int, 2> & in)
{
  size = in - position + 1;
}

template<>
inline vector::V<int, 2> Box<int, 2>::lower_right() const
{
  return position + size - 1;
}

template<typename T>
inline Box<T, 2>::operator Box<int, 2> () const
{
  return Box<int, 2>(
    static_cast<int>(x),
    static_cast<int>(y),
    static_cast<int>(w),
    static_cast<int>(h)
  );
}

template<typename T>
inline Box<T, 2>::operator Box<double, 2> () const
{
  return Box<double, 2>(
    static_cast<double>(x),
    static_cast<double>(y),
    static_cast<double>(w),
    static_cast<double>(h)
 );
}

//------------------------------------------------------------------------------

}
}

