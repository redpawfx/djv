/*------------------------------------------------------------------------------
 include/djv_base/vector3_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace vector {

//------------------------------------------------------------------------------
// V3
//------------------------------------------------------------------------------

template<typename T>
const int V<T, 3>::dimension = 3;

#define _VEC3_INIT \
  \
  x(e[0]), \
  y(e[1]), \
  z(e[2])

template<typename T>
inline V<T, 3>::V() :
  _VEC3_INIT
{
  x = y = 0;
}

template<typename T>
inline V<T, 3>::V(const V<T, 3> & in) :
  _VEC3_INIT
{
  x = in.x;
  y = in.y;
  z = in.z;
}

template<typename T>
inline V<T, 3>::V(T _x, T _y, T _z) :
  _VEC3_INIT
{
  x = _x;
  y = _y;
  z = _z;
}

template<typename T>
inline V<T, 3>::V(T in) :
  _VEC3_INIT
{
  x = y = z = in;
}
  
template<typename T>
inline V<T, 3> & V<T, 3>::operator = (const V<T, 3> & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}

template<typename T>
inline void V<T, 3>::set(const V<T, 3> & in)
{
  x = in.x;
  y = in.y;
  z = in.z;
}

template<typename T>
inline void V<T, 3>::set(T in)
{
  x = y = z = in;
}

template<typename T>
inline void V<T, 3>::zero()
{
  x = y = z = T(0);
}

#define _VEC3_OP(IN) \
  \
  template<typename T> \
  inline V<T, 3> & V<T, 3>::operator IN (const V<T, 3> & in) \
  { \
    x IN in.x; \
    y IN in.y; \
    z IN in.z; \
    return *this; \
  }

_VEC3_OP(+=)
_VEC3_OP(-=)
_VEC3_OP(*=)
_VEC3_OP(/=)

#define _VEC3_OP2(IN) \
  \
  template<typename T> \
  inline V<T, 3> & V<T, 3>::operator IN (T in) \
  { \
    x IN in; \
    y IN in; \
    z IN in; \
    return *this; \
  }

_VEC3_OP2(+=)
_VEC3_OP2(-=)
_VEC3_OP2(*=)
_VEC3_OP2(/=)

template<typename T>
inline V<T, 3>::operator V<int, 3> () const
{
  return V<int, 3>(
    static_cast<int>(x),
    static_cast<int>(y),
    static_cast<int>(z)
  );
}

template<typename T>
inline V<T, 3>::operator V<double, 3> () const
{
  return V<double, 3>(
    static_cast<double>(x),
    static_cast<double>(y),
    static_cast<double>(z)
  );
}

//------------------------------------------------------------------------------

}
}

