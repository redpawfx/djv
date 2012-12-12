/*------------------------------------------------------------------------------
 include/djv_base/math_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace math {

//------------------------------------------------------------------------------
// abs(), mod(), wrap(), step(), pulse()
//------------------------------------------------------------------------------

template<class T>
inline T abs(T in)
{
  return in < T(0) ? -in : in;
}

template<class T>
inline T mod(T in, T mod)
{
  T tmp = in;
  if (mod != T(0))
  {
    tmp = in - (static_cast<int>(in / mod) * mod);
    if (tmp < T(0)) tmp += mod;
  }
  return tmp;
}

template<class T>
inline T wrap(T in, T min, T max)
{
  return max > min ?
    (min + mod(in - min, max - min + static_cast<T>(1))) :
    min;
}

template<class T>
inline T step(T in, T v)
{
  return in <= v;
}

template<class T>
inline T pulse(T in, T min, T max)
{
  return step(min, in) - step(max, in);
}

//------------------------------------------------------------------------------
// min(), max(), clamp()
//------------------------------------------------------------------------------

template<class T>
inline T min(const T & a, const T & b)
{
  return a < b ? a : b;
}

template<class T>
inline void min(const T & a, const T & b, T * out)
{
  *out = a < b ? a : b;
}

template<class T>
inline T max(const T & a, const T & b)
{
  return a > b ? a : b;
}

template<class T>
inline void max(const T & a, const T & b, T * out)
{
  *out = a > b ? a : b;
}

template<class T>
inline T clamp(T in, T min, T max)
{
  return in < min ? min : (in > max ? max : in);
}

//------------------------------------------------------------------------------
// lerp(), smooth_step()
//------------------------------------------------------------------------------

template<class T, class U>
inline T lerp(U in, T min, T max)
{
  return min + T(in * (max - min));
}

inline double smooth_step(double in, double min, double max)
{
  return lerp(in * in * (3.0 - (2.0 * in)), min, max);
}

//------------------------------------------------------------------------------
// Range
//------------------------------------------------------------------------------

template<typename T>
inline Range<T>::Range(T in) :
  min(in),
  max(in)
{}

template<typename T>
inline Range<T>::Range(T min, T max) :
  min(min),
  max(max)
{}

//------------------------------------------------------------------------------
// intersect()
//------------------------------------------------------------------------------

template<class T>
inline bool intersect(T in, const Range<T> & range)
{
  return in >= range.min && in < range.max;
}

inline bool intersect(int in, const Range<int> & range)
{
  return in >= range.min && in < range.max + 1;
}

template<class T>
inline bool intersect(const Range<T> & a, const Range<T> & b, Range<T> * out)
{
  out->min = math::max(a.min, b.min);
  out->max = math::min(a.max, b.max);
  return out->max > out->min;
}

inline bool intersect(const Range<int> & a, const Range<int> & b,
  Range<int> * out)
{
  out->min = math::max(a.min, b.min);
  out->max = math::min(a.max, b.max);
  return (out->max + 1) > out->min;
}

//------------------------------------------------------------------------------
// bound()
//------------------------------------------------------------------------------

template<class T>
inline void bound(const Range<T> & a, const Range<T> & b, Range<T> * out)
{
  out->min = math::min(a.min, b.min);
  out->max = math::max(a.max, b.max);
}

template<typename T>
inline bool operator == (const Range<T> & a, const Range<T> & b)
{
  return a.min == b.min && a.max == b.max;
}

template<typename T>
inline bool operator != (const Range<T> & a, const Range<T> & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// ceil(), floor()
//------------------------------------------------------------------------------

/*inline int ceil(double in)
{
  const int tmp = static_cast<int>(in);
  return tmp + (in > 0 && in != tmp);
}

inline int floor(double in)
{
  const int tmp = static_cast<int>(in);
  return tmp - (in < 0 && ! in != tmp);
}*/

//------------------------------------------------------------------------------
// fraction()
//------------------------------------------------------------------------------

inline double fraction(double in)
{
  return in - static_cast<int>(in);
}

//------------------------------------------------------------------------------
// to_pow2()
//------------------------------------------------------------------------------

inline int to_pow2(int in)
{
  int i = 0;
  for (; in > (1 << i); ++i)
    ;
  return 1 << i;
}

//------------------------------------------------------------------------------
// degress_to_radians(), radians_to_degrees()
//------------------------------------------------------------------------------

inline double degrees_to_radians(double in)
{
  return in * 0.017453;
}

inline double radians_to_degrees(double in)
{
  return in * 57.29578;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

template<class T>
inline Debug & operator << (Debug & debug, const Range<T> & in)
{
  return debug << string::Format("%%-%%").arg(in.min).arg(in.max);
}

//------------------------------------------------------------------------------

}
}

