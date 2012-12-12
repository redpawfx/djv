/*------------------------------------------------------------------------------
 lib/djv_base/math.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "math.h"

#if defined(DJV_WINDOWS)
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <math.h>
#include <stdlib.h>

namespace djv_base {
namespace math {

//------------------------------------------------------------------------------
// pow(), sqrt(), exp(), log(), log_10(), anti_log_10()
//------------------------------------------------------------------------------

int pow(int in, int power)
{
  return floor(::pow(static_cast<double>(in), static_cast<double>(power)));
}

double pow(double in, double power)
{
  return ::pow(in, power);
}

double sqrt(double in)
{
  return ::sqrt(in);
}

double exp(double in)
{
  return ::exp(in);
}

double log(double in)
{
  return ::log(in);
}

double log_10(double in)
{
  return ::log10(in);
}

double anti_log_10(double in)
{
  return exp(in * log(10.0));
}

//------------------------------------------------------------------------------
// bias(), gain()
//------------------------------------------------------------------------------

double bias(double in, double bias)
{
  return pow(in, log(bias) / log(0.5));
}

// XXX

namespace {

const double almost_zero = FLT_MIN * 2.0;
const double almost_one = 1.0 - almost_zero;

}

double gain(double in, double gain)
{
  if (in < almost_zero) return 0.0;
  else if (in > almost_one) return 1.0;
  const double tmp = log(1.0 - gain) / log(0.5);
  if (in < 0.5) return pow(2.0 * in, tmp) / 2.0;
  else return 1.0 - pow(2.0 * (1.0 - in), tmp) / 2.0;
}

//------------------------------------------------------------------------------
// soft_clip()
//------------------------------------------------------------------------------

double soft_clip(double in, double value)
{
  const double tmp = 1.0 - value;
  if (in > tmp) in = tmp + (1.0 - exp(-(in - tmp) / value)) * value;
  return in;
}

//------------------------------------------------------------------------------
// sin(), cos(), tan(), arc_sin(), arc_cos(), arc_tan(), sec(), co_sec(),
// co_tan()
//------------------------------------------------------------------------------

double sin(double in) { return ::sin(in); }
double cos(double in) { return ::cos(in); }
double tan(double in) { return ::tan(in); }

double arc_sin(double in) { return ::asin(in); }
double arc_cos(double in) { return ::acos(in); }
double arc_tan(double in) { return ::atan(in); }
double arc_tan(double a, double b) { return ::atan2(a, b); }

// XXX Zero?

double sec(double in)
{
  const double tmp = cos(in);
  return tmp != 0.0 ? (1.0 / tmp) : 0.0;
}

double co_sec(double in)
{
  const double tmp = sin(in);
  return tmp != 0.0 ? (1.0 / tmp) : 0.0;
}

double co_tan(double in)
{
  const double tmp = tan(in);
  return tmp != 0.0 ? (1.0 / tmp) : 0.0;
}

//------------------------------------------------------------------------------
// ceil(), floor()
//------------------------------------------------------------------------------

int ceil(double in)
{
  return static_cast<int>(::ceil(in));
}

int floor(double in)
{
  return static_cast<int>(::floor(in));
}

//------------------------------------------------------------------------------
// round()
//------------------------------------------------------------------------------

_DJV_BASE int round(double in)
{
  //return static_cast<int>(::round(in));
  
  // XXX No Windows version of round?
  
  return static_cast<int>(in + 0.5);
}

//------------------------------------------------------------------------------
// rand(), rand_seed()
//------------------------------------------------------------------------------

namespace {

double _rand()
{
  return ::rand() / static_cast<double>(RAND_MAX);
}

}

double rand()
{
  return _rand();
}

double rand(double in)
{
  return in * _rand();
}

double rand(double min, double max)
{
  return min + (max - min) * _rand();
}

void rand_seed(uint in)
{
  if (! in)
  {
    // XXX Seed with current time.
    
#if defined(DJV_WINDOWS)

    in = ::GetTickCount();

#else // DJV_WINDOWS

    struct timeval tmp = { 0, 0 };
    gettimeofday(&tmp, 0);
    in = tmp.tv_usec;

#endif // DJV_WINDOWS
  }
  
  srand(in);
}

//------------------------------------------------------------------------------
// range()
//------------------------------------------------------------------------------

List<Range<int64_t> > range(const List<int64_t> & in)
{
  List<Range<int64_t> > out;
  
  if (in.size()) out += Range<int64_t>(in[0], in[0]);
  
  for (list::size_type i = 1; i < in.size(); ++i)
    if (in[i] - 1 != out[out.size() - 1].max)
      out += Range<int64_t>(in[i], in[i]);
    else
      out[out.size() - 1].max = in[i];

  return out;
}

//------------------------------------------------------------------------------

}
}

