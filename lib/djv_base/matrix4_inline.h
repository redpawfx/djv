/*------------------------------------------------------------------------------
 include/djv_base/matrix3_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace matrix {

//------------------------------------------------------------------------------
// M4
//------------------------------------------------------------------------------

template<typename T>
const int M<T, 4>::dimension = 4;
template<typename T>
const int M<T, 4>::dimension_x2 = 4 * 4;

template<typename T>
inline M<T, 4>::M()
{
  identity();
}

template<typename T>
inline M<T, 4>::M(const M<T, 4> & in)
{
  set(in);
}

template<typename T>
inline M<T, 4>::M(
  T _0,  T _1,  T _2,  T _3,
  T _4,  T _5,  T _6,  T _7,
  T _8,  T _9,  T _10, T _11,
  T _12, T _13, T _14, T _15
) {
  set(
    _0,  _1,  _2,  _3,
    _4,  _5,  _6,  _7,
    _8,  _9,  _10, _11,
    _12, _13, _14, _15
  );
}

template<typename T>
inline M<T, 4>::M(const T * in)
{
  set(in);
}

template<typename T>
inline M<T, 4> & M<T, 4>::operator = (const M<T, 4> & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}

template<typename T>
inline void M<T, 4>::set(const M<T, 4> & in)
{
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = in.e[i];
}

template<typename T>
inline void M<T, 4>::set(
  T _0,  T _1,  T _2,  T _3,
  T _4,  T _5,  T _6,  T _7,
  T _8,  T _9,  T _10, T _11,
  T _12, T _13, T _14, T _15
) {
  T * p = M<T, 4>::e;
  p[0 ] = _0;  p[1 ] = _1;  p[2 ] = _2;  p[3 ] = _3;
  p[4 ] = _4;  p[5 ] = _5;  p[6 ] = _6;  p[7 ] = _7;
  p[8 ] = _8;  p[9 ] = _9;  p[10] = _10; p[11] = _11;
  p[12] = _12; p[13] = _13; p[14] = _14; p[15] = _15;
}

template<typename T>
inline void M<T, 4>::set(const T * in)
{
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = in[i];
}

template<typename T>
inline void M<T, 4>::identity()
{
  static const T zero = static_cast<T>(0.0);
  static const T one = static_cast<T>(1.0);
  
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      e[i * 4 + j] = (i == j) ? one : zero;
}

template<typename T>
inline void M<T, 4>::zero()
{
  static const T zero = static_cast<T>(0.0);
  
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = zero;
}

//------------------------------------------------------------------------------

}
}

