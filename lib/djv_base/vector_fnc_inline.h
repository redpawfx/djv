/*------------------------------------------------------------------------------
 include/djv_base/vector_etc_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include <djv_base/math.h>

namespace djv_base {
namespace vector {

//------------------------------------------------------------------------------
// abs()
//------------------------------------------------------------------------------

template<typename T, int D>
inline V<T, D> abs(const V<T, D> & in)
{
  V<T, D> out;
  
  for (int i = 0; i < in.dimension; ++i)
    out.e[i] = math::abs(in.e[i]);
  
  return out;
}

//------------------------------------------------------------------------------
// swap()
//------------------------------------------------------------------------------

template<typename T, int D>
inline V<T, D> swap(const V<T, D> & in)
{
  V<T, D> out;
  
  for (int i = 0; i < in.dimension; ++i)
    out.e[i] = in.e[in.dimension - 1 - i];
  
  return out;
}

//------------------------------------------------------------------------------
// min(), max(), clamp()
//------------------------------------------------------------------------------

template<typename T, int D>
inline V<T, D> min(const V<T, D> & a, const V<T, D> & b)
{
  V<T, D> out;
  
  min(a, b, &out);
  
  return out;
}

template<typename T, int D>
inline void min(const V<T, D> & a, const V<T, D> & b, V<T, D> * out)
{
  for (int i = 0; i < a.dimension; ++i)
    math::min(a.e[i], b.e[i], &out->e[i]);
}

template<typename T, int D>
inline V<T, D> max(const V<T, D> & a, const V<T, D> & b)
{
  V<T, D> out;
  
  max(a, b, &out);
  
  return out;
}

template<typename T, int D>
inline void max(const V<T, D> & a, const V<T, D> & b, V<T, D> * out)
{
  for (int i = 0; i < a.dimension; ++i)
    math::max(a.e[i], b.e[i], &out->e[i]);
}

template<typename T, int D>
inline V<T, D> clamp(
  const V<T, D> & in,
  const V<T, D> & min,
  const V<T, D> & max
) {
  V<T, D> out;
  
  for (int i = 0; i < in.dimension; ++i)
    out.e[i] = math::clamp(in.e[i], min.e[i], max.e[i]);
  
  return out;
}

//------------------------------------------------------------------------------
// is_size_valid()
//------------------------------------------------------------------------------

template<typename T, int D>
inline bool is_size_valid(const V<T, D> & in)
{
  return in > V<T, D>(0);
}

//------------------------------------------------------------------------------
// length()
//------------------------------------------------------------------------------

template<typename T, int D>
inline double length(const V<T, D> & in)
{
  double out = 0.0;
  
  for (int i = 0; i < in.dimension; ++i)
    out += in.e[i] * in.e[i];
  
  return math::sqrt(out);
}

//------------------------------------------------------------------------------
// area()
//------------------------------------------------------------------------------

template<typename T, int D>
inline double area(const V<T, D> & in)
{
  double out = static_cast<double>(in.e[0]);
  
  for (int i = 1; i < in.dimension; ++i)
    out *= static_cast<double>(in.e[i]);
  
  return math::abs(out);
}

//------------------------------------------------------------------------------
// normalize()
//------------------------------------------------------------------------------

template<typename T, int D>
inline V<T, D> normalize(const V<T, D> & in)
{
  V<T, D> out;
  
  const double l = length(in);
  if (l != 0.0)
    for (int i = 0; i < in.dimension; ++i)
      out.e[i] = T(in.e[i] / l);
  
  return out;
}

//------------------------------------------------------------------------------
// dot()
//------------------------------------------------------------------------------

template<typename T, int D>
inline double dot(const V<T, D> & a, const V<T, D> & b)
{
  double out = 0.0;
  
  for (int i = 0; i < a.dimension; ++i)
    out += a.e[i] * b.e[i];
  
  return out;
}

//------------------------------------------------------------------------------
// aspect()
//------------------------------------------------------------------------------

template<typename T, int D>
inline double aspect(const V<T, D> & in)
{
  return in.e[1] != T(0) ? (in.e[0] / static_cast<double>(in.e[1])) : 0.0;
}

//------------------------------------------------------------------------------
// Arithmetic
//------------------------------------------------------------------------------

#define _VEC_FNC_OP(IN) \
  \
  template<typename T, int D> \
  inline V<T, D> operator IN (const V<T, D> & a, const V<T, D> & b) \
  { \
    V<T, D> out; \
    \
    for (int i = 0; i < a.dimension; ++i) \
      out.e[i] = a.e[i] IN b.e[i]; \
    \
    return out; \
  }

_VEC_FNC_OP(+)
_VEC_FNC_OP(-)
_VEC_FNC_OP(*)
_VEC_FNC_OP(/)

#define _VEC_FNC_OP2(IN) \
  \
  template<typename T, int D> \
  inline V<T, D> operator IN (const V<T, D> & a, T b) \
  { \
    V<T, D> out; \
    \
    for (int i = 0; i < a.dimension; ++i) \
      out.e[i] = a.e[i] IN b; \
    \
    return out; \
  }

_VEC_FNC_OP2(+)
_VEC_FNC_OP2(-)
_VEC_FNC_OP2(*)
_VEC_FNC_OP2(/)

template<typename T, int D>
inline V<T, D> operator - (const V<T, D> & in)
{
  V<T, D> out;

  for (int i = 0; i < in.dimension; ++i)
    out.e[i] = -in.e[i];

  return out;
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

template<typename T, typename T2, int D>
inline V<T2, D> convert(const V<T, D> & in)
{
  V<T2, D> out;

  for (int i = 0; i < in.dimension; ++i)
    out.e[i] = T2(in.e[i]);

  return out;
}

template<typename T, typename T2, int D>
inline V<T2, D> ceil(const V<T, D> & in)
{
  V<T2, D> out;

  for (int i = 0; i < in.dimension; ++i)
    out.e[i] = math::ceil(in.e[i]);

  return out;
}

template<typename T, typename T2, int D>
inline V<T2, D> floor(const V<T, D> & in)
{
  V<T2, D> out;

  for (int i = 0; i < in.dimension; ++i)
    out.e[i] = math::floor(in.e[i]);

  return out;
}

template<typename T, int D>
inline String & operator << (String & out, const V<T, D> & in)
{
  for (int i = 0; i < in.dimension; ++i)
    out << in.e[i];

  return out;
}

template<typename T, int D>
inline String & operator >> (String & in, V<T, D> & out) throw (String)
{
  for (int i = 0; i < out.dimension; ++i)
    in >> out.e[i];

  return in;
}

//------------------------------------------------------------------------------

}
}

