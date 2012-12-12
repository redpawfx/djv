/*------------------------------------------------------------------------------
 include/djv_base/box_fnc_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include <djv_base/math.h>

namespace djv_base {
namespace box {

//------------------------------------------------------------------------------
// corners()
//------------------------------------------------------------------------------

template<typename T, int D>
inline Box<T, D> corners(
  const vector::V<T, D> & upper_left,
  const vector::V<T, D> & lower_right
) {
  const vector::V<T, D> & a = vector::min(upper_left, lower_right);
  const vector::V<T, D> & b = vector::max(upper_left, lower_right);

  Box<T, D> out;

  out.position = a;
  out.lower_right(b);

  return out;
}

//------------------------------------------------------------------------------
// swap()
//------------------------------------------------------------------------------

template<typename T, int D>
inline Box<T, D> swap(const Box<T, D> & in)
{
  return Box<T, D>(vector::swap(in.position), vector::swap(in.size));
}

//------------------------------------------------------------------------------
// intersect()
//------------------------------------------------------------------------------

template<typename T, int D>
inline bool intersect(const Box<T, D> & a, const vector::V<T, D> & b)
{
  const vector::V<T, D> a1 = a.lower_right();

  for (int i = 0; i < a.dimension; ++i)
    if (! math::intersect(b.e[i], math::Range<T>(a.position.e[i], a1.e[i])))
      return false;

  return true;
}

template<typename T, int D>
inline Box<T, D> intersect(const Box<T, D> & a, const Box<T, D> & b)
{
  const vector::V<T, D> a1 = a.lower_right();
  const vector::V<T, D> b1 = b.lower_right();

  Box<T, D> out;
  vector::V<T, D> lower_right;

  for (int i = 0; i < a.dimension; ++i)
  {
    math::Range<T> tmp;
    math::intersect(
      math::Range<T>(a.position.e[i], a1.e[i]),
      math::Range<T>(b.position.e[i], b1.e[i]),
      &tmp);
    out.position.e[i] = tmp.min;
    lower_right.e[i] = tmp.max;
  }

  out.lower_right(lower_right);

  return out;
}

//------------------------------------------------------------------------------
// bound()
//------------------------------------------------------------------------------

template<typename T, int D>
inline Box<T, D> bound(const Box<T, D> & a, const Box<T, D> & b)
{
  const vector::V<T, D> a1 = a.lower_right();
  const vector::V<T, D> b1 = b.lower_right();
  
  Box<T, D> out;
  vector::V<T, D> lower_right;

  for (int i = 0; i < a.dimension; ++i)
  {
    math::Range<T> tmp;
    math::bound(
      math::Range<T>(a.position.e[i], a1.e[i]),
      math::Range<T>(b.position.e[i], b1.e[i]),
      &tmp);
    out.position.e[i] = tmp.min;
    lower_right.e[i] = tmp.max;
  }

  out.lower_right(lower_right);

  return out;
}

//------------------------------------------------------------------------------
// expand()
//------------------------------------------------------------------------------

template<typename T, int D>
inline Box<T, D> expand(const Box<T, D> & in, const vector::V<T, D> & expand)
{
  return corners(
    vector::min(in.position, expand),
    vector::max(in.lower_right(), expand)
  );
}

//------------------------------------------------------------------------------
// border()
//------------------------------------------------------------------------------

template<typename T, int D>
inline Box<T, D> border(const Box<T, D> & in, const vector::V<T, D> & border)
{
  return Box<T, D>(in.position - border, in.size + border * T(2));
}

//------------------------------------------------------------------------------
// is_size_valid()
//------------------------------------------------------------------------------

template<typename T, int D>
inline bool is_size_valid(const Box<T, D> & in)
{
  return vector::is_size_valid(in.size);
}

//------------------------------------------------------------------------------
// Arithmetic
//------------------------------------------------------------------------------

#define _BOX_FNC_OP(IN) \
  \
  template<typename T, int D> \
  inline Box<T, D> operator IN (const Box<T, D> & a, T b) \
  { \
    return Box<T, D>(a.position IN b, a.size IN b); \
  }

_BOX_FNC_OP(+)
_BOX_FNC_OP(-)
_BOX_FNC_OP(*)
_BOX_FNC_OP(/)

#define _BOX_FNC_OP2(IN) \
  \
  template<typename T, int D> \
  inline Box<T, D> operator IN ( \
    const Box<T, D> & a, \
    const vector::V<T, D> & b \
  ) { \
    return Box<T, D>(a.position IN b, a.size IN b); \
  }

_BOX_FNC_OP2(+)
_BOX_FNC_OP2(-)
_BOX_FNC_OP2(*)
_BOX_FNC_OP2(/)

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

template<typename T, typename T2, int D>
inline Box<T2, D> convert(const Box<T, D> & in)
{
  Box<T2, D> out;
  
  out.position = vector::convert<T, T2, D>(in.position);
  out.size = vector::convert<T, T2, D>(in.size);
  
  return out;
}

template<typename T, int D>
inline String & operator << (String & out, const Box<T, D> & in)
{
  return out << in.position << in.size;
}

template<typename T, int D>
inline String & operator >> (String & in, Box<T, D> & out) throw (String)
{
  return in >> out.position >> out.size;
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

template<typename T, int D>
inline bool operator == (const Box<T, D> & a, const Box<T, D> & b)
{
  return a.position == b.position && a.size == b.size;
}

template<typename T, int D>
inline bool operator != (const Box<T, D> & a, const Box<T, D> & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

template<typename T, int D>
inline Debug & operator << (Debug & debug, const Box<T, D> & in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------

}
}

