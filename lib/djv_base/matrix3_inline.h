/*------------------------------------------------------------------------------
 include/djv_base/matrix3_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace matrix {

//------------------------------------------------------------------------------
// M3
//------------------------------------------------------------------------------

template<typename T>
const int M<T, 3>::dimension = 3;
template<typename T>
const int M<T, 3>::dimension_x2 = 3 * 3;

template<typename T>
inline M<T, 3>::M()
{
  identity();
}

template<typename T>
inline M<T, 3>::M(const M<T, 3> & in)
{
  set(in);
}

template<typename T>
inline M<T, 3>::M(
  T _0, T _1, T _2,
  T _3, T _4, T _5,
  T _6, T _7, T _8
) {
  set(
    _0, _1, _2,
    _3, _4, _5,
    _6, _7, _8
  );
}

template<typename T>
inline M<T, 3>::M(const T * in)
{
  set(in);
}

template<typename T>
inline M<T, 3> & M<T, 3>::operator = (const M<T, 3> & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}

template<typename T>
inline void M<T, 3>::set(const M<T, 3> & in)
{
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = in.e[i];
}

template<typename T>
inline void M<T, 3>::set(
  T _0, T _1, T _2,
  T _3, T _4, T _5,
  T _6, T _7, T _8
) {
  T * p = M<T, 3>::e;
  p[0] = _0; p[1] = _1; p[2] = _2;
  p[3] = _3; p[4] = _4; p[5] = _5;
  p[6] = _6; p[7] = _7; p[8] = _8;
}

template<typename T>
inline void M<T, 3>::set(const T * in)
{
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = in[i];
}

template<typename T>
inline void M<T, 3>::identity()
{
  static const T zero = static_cast<T>(0.0);
  static const T one = static_cast<T>(1.0);
  
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      e[i * 3 + j] = (i == j) ? one : zero;
}

template<typename T>
inline void M<T, 3>::zero()
{
  static const T zero = static_cast<T>(0.0);
  
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = zero;
}

//------------------------------------------------------------------------------

}
}

