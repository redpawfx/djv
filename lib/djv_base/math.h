/*------------------------------------------------------------------------------
 include/djv_base/math.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_MATH_H
#define DJV_MATH_H

#include <djv_base/math_def.h>

#include <djv_base/base.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::math
//
//! Math.
//------------------------------------------------------------------------------

namespace math {

//------------------------------------------------------------------------------
//!\name Constants
//------------------------------------------------------------------------------
//@{

static const double pi    = 3.1415927;  //!< Pi
static const double pi_x2 = 6.2831853;  //!< Pi times two
static const double pi_2  = 1.5707963;  //!< Pi divided by two
static const double pi_4  = 0.7853981;  //!< Pi divided by four

//@}
//------------------------------------------------------------------------------
//!\name Functions
//!
//! References:
//!
//! - Andrew Glassner, "Graphics Gems"
//! - David S. Ebert, F. Kenton Musgrave, Darwyn Peachey, Ken Perlin,
//!   Steven Worley, "Texturing & Modeling, A Procedural Approach"
//------------------------------------------------------------------------------
//@{

//! Get the absolute value.

template<typename T>
inline T abs(T);

//! Raise a value to a power.

_DJV_BASE int    pow(int in, int value);
_DJV_BASE double pow(double in, double value);

//! Get the square root of a value.

_DJV_BASE double sqrt(double);

//! Exponential.

_DJV_BASE double exp(double);

_DJV_BASE double log(double);
_DJV_BASE double log_10(double);
_DJV_BASE double anti_log_10(double);

//! Modulus.

template<typename T>
inline T mod(T, T mod);

template<typename T>
inline T wrap(T in, T min, T max);

template<typename T>
inline T step(T in, T value);

template<typename T>
inline T pulse(T in, T min, T max);

_DJV_BASE double bias(double in, double value);

_DJV_BASE double gain(double in, double value);

template<typename T>
inline T min(const T &, const T &);
template<typename T>
inline void min(const T &, const T &, T *);

template<typename T>
inline T max(const T &, const T &);
template<typename T>
inline void max(const T &, const T &, T *);

template<typename T>
inline T clamp(T, T min, T max);

//! Linear interpolation.

template<typename T, typename U>
inline T lerp(U in, T min, T max);

inline double smooth_step(double in, double min, double max);

//@}
//------------------------------------------------------------------------------
//!\name Soft Clip
//!
//! References:
//!
//! - Ken McGaugh, "Re: [Openexr-devel] Cineon -> half conversion?"
//!   OpenEXR Mailing List 3/2/05
//------------------------------------------------------------------------------
//@{

_DJV_BASE double soft_clip(double in, double value);

//@}
//------------------------------------------------------------------------------
//!\name Trigonometry
//------------------------------------------------------------------------------
//@{

_DJV_BASE double sin(double);
_DJV_BASE double cos(double);
_DJV_BASE double sec(double);
_DJV_BASE double tan(double);

_DJV_BASE double arc_sin(double);
_DJV_BASE double arc_cos(double);
_DJV_BASE double arc_tan(double);
_DJV_BASE double arc_tan(double, double);

_DJV_BASE double co_sec(double);
_DJV_BASE double co_tan(double);

//@}
//------------------------------------------------------------------------------
//!\name Number Ranges
//------------------------------------------------------------------------------
//@{

//! Number range.

template<typename T>
struct Range
{
  inline Range(T = T(0));
  inline Range(T min, T max);
  
  T min, max;
};

template<typename T>
inline bool intersect(T, const Range<T> &);
template<typename T>
inline bool intersect(const Range<T> &, const Range<T> &, Range<T> *);

template<typename T>
inline void bound(const Range<T> &, const Range<T> &, Range<T> *);

//! Convert a sorted list of positive integers into ranges.

_DJV_BASE List<Range<int64_t> > range(const List<int64_t> &);

//@}
//------------------------------------------------------------------------------
//!\name Random Numbers
//------------------------------------------------------------------------------
//@{

_DJV_BASE double rand();
_DJV_BASE double rand(double);
_DJV_BASE double rand(double min, double max);

//! Seed the random value generator. A value of zero uses a random seed.

_DJV_BASE void rand_seed(uint = 0);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

template<typename T>
inline bool operator == (const Range<T> &, const Range<T> &);

template<typename T>
inline bool operator != (const Range<T> &, const Range<T> &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

//inline int ceil(double);
//inline int floor(double);
_DJV_BASE int ceil(double);
_DJV_BASE int floor(double);
_DJV_BASE int round(double);
inline double fraction(double);

//! Find the closest power of two.

inline int to_pow2(int);

inline double degrees_to_radians(double);
inline double radians_to_degrees(double);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

template<typename T>
inline Debug & operator << (Debug &, const Range<T> &);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_base/math_inline.h>
#endif

