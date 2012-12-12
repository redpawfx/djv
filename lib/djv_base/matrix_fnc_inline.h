/*------------------------------------------------------------------------------
 include/djv_base/matrix_fnc_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include <djv_base/math.h>

namespace djv_base {
namespace matrix {

//------------------------------------------------------------------------------
// transpose()
//------------------------------------------------------------------------------

template<typename T, int D>
inline M<T, D> transpose(const M<T, D> & in)
{
  M<T, D> out;

  for (int i = 0; i < D; ++i)
    for (int j = 0; j < D; ++j)
      out.e[j * D + i] = in.e[i * D + j];

  return out;
}

//------------------------------------------------------------------------------
// translate(), translate3f()
//------------------------------------------------------------------------------

template<typename T, int D, int D2>
inline M<T, D> translate(const vector::V<T, D2> & in)
{
  //DJV_ASSERT((D - 1) == D2);
  
  M<T, D> out;

  for (int i = 0; i < D2; ++i)
    out.e[(D - 1) * D + i] = in.e[i];

  return out;
}

inline M3f translate3f(const V2f & in)
{
  return translate<double, 3, 2>(in);
}

//------------------------------------------------------------------------------
// scale(), scale3f()
//------------------------------------------------------------------------------

template<typename T, int D, int D2>
inline M<T, D> scale(const vector::V<T, D2> & in)
{
  //DJV_ASSERT((D - 1) == D2);
  
  M<T, D> out;

  for (int i = 0; i < D2; ++i)
    out.e[i * D + i] = in.e[i];

  return out;
}

inline M3f scale3f(const V2f & in)
{
  return scale<double, 3, 2>(in);
}

//------------------------------------------------------------------------------
// rotate3f()
//------------------------------------------------------------------------------

inline M3f rotate3f(double in)
{
  const double cos = math::cos(math::degrees_to_radians(in));
  const double sin = math::sin(math::degrees_to_radians(in));

  return M3f(
    cos,  sin, 0,
    -sin, cos, 0,
    0,    0,   1
  );
}

//------------------------------------------------------------------------------
// Arithmetic
//------------------------------------------------------------------------------

template<typename T, int D>
inline M<T, D> operator * (const M<T, D> & a, const M<T, D> & b)
{
  M<T, D> out;

  out.zero();

  for (int i = 0; i < D; ++i)
    for (int j = 0; j < D; ++j)
      for (int k = 0; k < D; ++k)
        out.e[i * D + j] += a.e[i * D + k] * b.e[k * D + j];

  return out;
}

//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

template<typename T, int D, int D2>
inline vector::V<T, D2> operator * (
  const M<T, D> & a,
  const vector::V<T, D2> & b
) {
  //DJV_ASSERT((D - 1) == D2);

  vector::V<T, D2> out;

  for (int i = 0; i < D2; ++i)
  {
    int j = 0;
    for (; j < D2; ++j)
      out.e[i] += a.e[j * D + i] * b.e[j];

    out.e[i] += a.e[j * D + i];
  }

  return out;
}

template<typename T>
inline box::Box<T, 2> operator * (const M<T, 3> & a, const box::Box<T, 2> & b)
{
  const vector::V<T, 2> lower_right = b.lower_right();
  
  vector::V<T, 2> pt [] =
  {
    b.position,
    b.position,
    lower_right,
    b.position
  };
  pt[1].y = lower_right.y;
  pt[3].x = lower_right.x;
  
  box::Box<T, 2> out;
  
  for (int i = 0; i < 4; ++i)
  {
    pt[i] = a * pt[i];
    
    if (0 == i)
      out.position = pt[i];
    else
      out = box::expand(out, pt[i]);
  }
  
  return out;
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

template <typename T, typename T2, int D>
inline M<T2, D> convert(const M<T, D> & in)
{
  M<T2, D> out;
  
  for (int i = 0; i < out.dimension_x2; ++i)
    out.e[i] = T2(in.e[i]);
  
  return out;
}

// XXX Probably wrong.

template <typename T>
M<T, 4> matrix4(const M<T, 3> & in)
{
  return M<T, 4>(
    in.e[0], in.e[1], in.e[2], 0.0,
    in.e[3], in.e[4], in.e[5], 0.0,
    0.0,     0.0,     in.e[8], 0.0,
    in.e[6], in.e[7], 0.0,     1.0
  );
}

template<typename T, int D>
inline String & operator << (String & out, const M<T, D> & in)
{
  for (int i = 0; i < in.dimension_x2; ++i)
    out << in.e[i];
  
  return out;
}

template<typename T, int D>
inline String & operator >> (String & in, M<T, D> & out) throw (String)
{
  for (int i = 0; i < out.dimension_x2; ++i)
    in >> out.e[i];
  
  return in;
}

//------------------------------------------------------------------------------

}
}

