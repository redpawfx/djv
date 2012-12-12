/*------------------------------------------------------------------------------
 lib/djv_base/time.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "time.h"

#include <djv_base/string.h>
#if defined(DJV_WINDOWS)
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <float.h>
#include <time.h>

namespace djv_base {
namespace time {

//------------------------------------------------------------------------------
// Timer
//------------------------------------------------------------------------------

Timer::Timer()
{
#if defined(DJV_WINDOWS)

  //_t0 = _t1 = 0;
  _t0.QuadPart = 0;
  _t1.QuadPart = 0;
  ::QueryPerformanceFrequency(&_frequency);

#else // DJV_WINDOWS

  _t0.tv_sec = _t0.tv_usec = 0;
  _t1.tv_sec = _t1.tv_usec = 0;

#endif // DJV_WINDOWS

  start();
}

void Timer::start()
{
#if defined(DJV_WINDOWS)

  //_t0 = ::GetTickCount();
  ::QueryPerformanceCounter(&_t0);

#else // DJV_WINDOWS

  ::gettimeofday(&_t0, 0);

#endif // DJV_WINDOWS

  _t1 = _t0;
}

void Timer::check()
{
#if defined(DJV_WINDOWS)

  //_t1 = ::GetTickCount();
  ::QueryPerformanceCounter(&_t1);

#else // DJV_WINDOWS

  ::gettimeofday(&_t1, 0);

#endif // DJV_WINDOWS
}

#if ! defined(DJV_WINDOWS)

namespace {

inline void sub(const timeval & a, const timeval & b, timeval * out)
{
  out->tv_sec = a.tv_sec - b.tv_sec;
  out->tv_usec = a.tv_usec - b.tv_usec;
  
  if (out->tv_usec < 0)
  {
    --out->tv_sec;
    out->tv_usec += 1000000;
  }
}

}

#endif // ! DJV_WINDOWS

double Timer::seconds() const
{
  double out = 0.0;

#if defined(DJV_WINDOWS)

  //out = (_t1 - _t0) / 1000.0;
  if (_frequency.QuadPart)
    out = (_t1.QuadPart - _t0.QuadPart) /
      static_cast<double>(_frequency.QuadPart);

#else // DJV_WINDOWS

  timeval t;
  sub(_t1, _t0, &t);
  out = t.tv_sec + t.tv_usec / 1000000.0;

#endif // DJV_WINDOWS
  
  return out;
}

double Timer::fps() const
{
  const double seconds = this->seconds();
  return (seconds != 0.0) ? (1.0 / seconds) : 0.0;
}

//------------------------------------------------------------------------------
// current()
//------------------------------------------------------------------------------

::time_t current()
{
  return ::time(0);
}

//------------------------------------------------------------------------------
// sleep()
//------------------------------------------------------------------------------

void sleep(double seconds)
{
  // XXX double/int

#if defined(DJV_WINDOWS)

  ::Sleep(seconds * 1000);

#else // DJV_WINDOWS

  ::sleep(seconds);

#endif // DJV_WINDOWS
}

//------------------------------------------------------------------------------
// seconds_to_time()
//------------------------------------------------------------------------------

void seconds_to_time(double in, int * hour, int * minute, double * second)
{
  *hour = static_cast<int>(in) / (60 * 60);
  in -= *hour * 60 * 60;

  *minute = static_cast<int>(in) / 60;
  in -= *minute * 60;

  *second = in;
}

//------------------------------------------------------------------------------
// time_to_string()
//------------------------------------------------------------------------------

String time_to_string(::time_t in)
{
  String out;

#if defined(DJV_WINDOWS)

  char tmp [string::cstring_size] = "";
  ::ctime_s(tmp, string::cstring_size, &in);
  out = tmp;

#else // DJV_WINDOWS

  out = ::ctime(&in);

#endif // DJV_WINDOWS

  // XXX Remove newline.

  if (out.size())
    out.erase(out.size() - 1);

  return out;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

String label_time(double in)
{
  int hour = 0, minute = 0;
  double second = 0.0;
  djv_base::time::seconds_to_time(in, &hour, &minute, &second);

  return string::Format("%%:%%:%%").
    arg(hour, 2, '0').
    arg(minute, 2, '0').
    arg(second, 5, 2, '0');
}

//------------------------------------------------------------------------------

}
}

