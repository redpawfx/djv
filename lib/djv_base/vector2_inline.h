/*------------------------------------------------------------------------------
 include/djv_base/vector2_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace vector {

//------------------------------------------------------------------------------
// V2
//------------------------------------------------------------------------------

template<typename T>
const int V<T, 2>::dimension = 2;

#define _VEC2_INIT \
  \
  x(V<T, 2>::e[0]), \
  y(V<T, 2>::e[1])

template<typename T>
inline V<T, 2>::V() :
  _VEC2_INIT
{
  x = y = 0;
}

template<typename T>
inline V<T, 2>::V(const V<T, 2> & in) :
  _VEC2_INIT
{
  x = in.x;
  y = in.y;
}

template<typename T>
inline V<T, 2>::V(T _x, T _y) :
  _VEC2_INIT
{
  x = _x;
  y = _y;
}

template<typename T>
inline V<T, 2>::V(T in) :
  _VEC2_INIT
{
  x = y = in;
}

template<typename T>
inline V<T, 2> & V<T, 2>::operator = (const V<T, 2> & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}

template<typename T>
inline void V<T, 2>::set(const V<T, 2> & in)
{
  x = in.x;
  y = in.y;
}

template<typename T>
inline void V<T, 2>::set(T in)
{
  x = y = in;
}

template<typename T>
inline void V<T, 2>::zero()
{
  x = y = T(0);
}

#define _VEC2_OP(IN) \
  \
  template<typename T> \
  inline V<T, 2> & V<T, 2>::operator IN (const V<T, 2> & in) \
  { \
    x IN in.x; \
    y IN in.y; \
    return *this; \
  }

_VEC2_OP(+=)
_VEC2_OP(-=)
_VEC2_OP(*=)
_VEC2_OP(/=)

#define _VEC2_OP2(IN) \
  \
  template<typename T> \
  inline V<T, 2> & V<T, 2>::operator IN (T in) \
  { \
    x IN in; \
    y IN in; \
    return *this; \
  }

_VEC2_OP2(+=)
_VEC2_OP2(-=)
_VEC2_OP2(*=)
_VEC2_OP2(/=)

template<typename T>
inline V<T, 2>::operator V<int, 2> () const
{
  return V<int, 2>(
    static_cast<int>(x),
    static_cast<int>(y)
  );
}

template<typename T>
inline V<T, 2>::operator V<double, 2> () const
{
  return V<double, 2>(
    static_cast<double>(x),
    static_cast<double>(y)
  );
}

//------------------------------------------------------------------------------

}
}

