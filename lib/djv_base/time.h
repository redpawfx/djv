/*------------------------------------------------------------------------------
 include/djv_base/time.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_TIME_H
#define DJV_TIME_H

#include <djv_base/time_def.h>

#include <djv_base/export.h>
#include <djv_base/type.h>
#include <djv_base/string_def.h>
#if ! defined(DJV_WINDOWS)
#include <sys/time.h>
#endif

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::time
//
//! Time.
//------------------------------------------------------------------------------

namespace time {

//------------------------------------------------------------------------------
//!\class Timer
//
//! A timer.
//------------------------------------------------------------------------------

class _DJV_BASE Timer
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Timer();

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  //! Start the timer.
  
  void start();
  
  //! Check the timer.
  
  void check();
  
  //! Get elapsed time in seconds. You should call check() before calling this
  //! function.
  
  double seconds() const;
  
  //! Get elapsed time in frames per second. You should call check() before
  //! calling this function.
  
  double fps() const;

  //@}
  
private:

#if defined(DJV_WINDOWS)
  //DWORD _t0, _t1;
  LARGE_INTEGER _t0, _t1, _frequency;
#else
  ::timeval _t0, _t1;
#endif
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Get the current time.

_DJV_BASE ::time_t current();

//! Go to sleep.

_DJV_BASE void sleep(double seconds);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_BASE void seconds_to_time(
  double,
  int * hours,
  int * minutes,
  double * seconds
);

_DJV_BASE String time_to_string(::time_t);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE String label_time(double seconds);

//@}
//------------------------------------------------------------------------------

}
}

#endif

