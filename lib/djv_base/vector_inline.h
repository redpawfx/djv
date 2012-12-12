/*------------------------------------------------------------------------------
 include/djv_base/vector_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace vector {

//------------------------------------------------------------------------------
// V<T, D>
//------------------------------------------------------------------------------

template<typename T, int D>
const int V<T, D>::dimension = D;

template<typename T, int D>
inline V<T, D>::V()
{
  zero();
}

template<typename T, int D>
inline V<T, D>::V(const V<T, D> & in)
{
  set(in);
}

template<typename T, int D>
inline V<T, D>::V(T in)
{
  set(in);
}

template<typename T, int D>
inline V<T, D> & V<T, D>::operator = (const V<T, D> & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}

template<typename T, int D>
inline void V<T, D>::set(const V<T, D> & in)
{
  for (int i = 0; i < dimension; ++i)
    V<T, D>::e[i] = in.e[i];
}

template<typename T, int D>
inline void V<T, D>::set(T in)
{
  for (int i = 0; i < dimension; ++i)
    e[i] = in;
}

template<typename T, int D>
inline void V<T, D>::zero()
{
  for (int i = 0; i < dimension; ++i)
    e[i] = 0;
}

#define _VEC_OP(IN) \
  \
  template<typename T, int D> \
  inline V<T, D> & V<T, D>::operator IN (const V<T, D> & in) \
  { \
    for (int i = 0; i < dimension; ++i) \
      V<T, D>::e[i] IN in.e[i]; \
    return *this; \
  }

_VEC_OP(+=)
_VEC_OP(-=)
_VEC_OP(*=)
_VEC_OP(/=)

#define _VEC_OP2(IN) \
  \
  template<typename T, int D> \
  inline V<T, D> & V<T, D>::operator IN (T in) \
  { \
    for (int i = 0; i < dimension; ++i) \
      V<T, D>::e[i] IN in; \
    return *this; \
  }

_VEC_OP2(+=)
_VEC_OP2(-=)
_VEC_OP2(*=)
_VEC_OP2(/=)

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

#define _VEC_COMPARE(IN) \
  \
  template<typename T> \
  inline bool operator IN (const T & a, const T & b) \
  { \
    for (int i = 0; i < a.dimension; ++i) \
      if (! (a.e[i] IN b.e[i])) \
        return false; \
    \
    return true; \
  }

_VEC_COMPARE(==)
_VEC_COMPARE(<)
_VEC_COMPARE(<=)
_VEC_COMPARE(>)
_VEC_COMPARE(>=)

template<typename T>
inline bool operator != (const T & a, const T & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

template<typename T>
inline Debug & operator << (Debug & debug, const T & in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------

}
}

