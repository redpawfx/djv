/*------------------------------------------------------------------------------
 include/djv_image/time.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_TIME_H
#define DJV_IMAGE_TIME_H

#include <djv_image/time_def.h>

#include <djv_image/base.h>
#include <djv_base/file_seq.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::time
//
//! Image time.
//------------------------------------------------------------------------------

namespace time {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Time units.

enum UNITS
{
  UNITS_TIMECODE,
  UNITS_FRAMES,
  
  _UNITS_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Speed
//------------------------------------------------------------------------------
//@{

//! Frames per second.

enum FPS
{
  FPS_1,
  FPS_3,
  FPS_6,
  FPS_12,
  FPS_15,
  FPS_16,
  FPS_18,
  FPS_23_98,
  FPS_24,
  FPS_25,
  FPS_29_97,
  FPS_30,
  FPS_50,
  FPS_59_94,
  FPS_60,
  FPS_120,
  
  _FPS_SIZE
};

//! Speed.

struct _DJV_IMAGE Speed
{
  Speed();
  Speed(int scale, int duration = 1);
  Speed(FPS);
  
  int scale, duration;

  void set(FPS);
  bool valid() const;
};

//@}
//------------------------------------------------------------------------------
//!\class Time
//
//! Time.
//------------------------------------------------------------------------------

struct _DJV_IMAGE Time : public file_seq::Seq
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Time();
  Time(const List<int64_t> &, const Speed & = Speed());
  Time(int64_t start, int64_t end, const Speed & = Speed());

  //@}
  
  Speed speed;
};

//------------------------------------------------------------------------------
//!\name Timecode
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE void timecode_to_time(
  uint32_t,
  int & hour,
  int & minute,
  int & second,
  int & frame
);

_DJV_IMAGE uint32_t time_to_timecode(
  int hour,
  int minute,
  int second,
  int frame
);

_DJV_IMAGE int64_t timecode_to_frame(
  uint32_t timecode,
  const time::Speed &
);

_DJV_IMAGE uint32_t frame_to_timecode(
  int64_t frame,
  const time::Speed &
);

_DJV_IMAGE String timecode_to_string(uint32_t);

_DJV_IMAGE uint32_t string_to_timecode(const String &);

//@}
//------------------------------------------------------------------------------
//!\name Keycode
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String keycode_to_string(
  int id,
  int type,
  int prefix,
  int count,
  int offset
);

_DJV_IMAGE bool string_to_keycode(
  const String &,
  int & id,
  int & type,
  int & prefix,
  int & count,
  int & offset
);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Speed &, const Speed &);
_DJV_IMAGE bool operator == (const Time &, const Time &);

_DJV_IMAGE bool operator != (const Speed &, const Speed &);
_DJV_IMAGE bool operator != (const Time &, const Time &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE double speed_to_float(const Speed &);

_DJV_IMAGE Speed float_to_speed(double);

_DJV_IMAGE String units_to_string(int64_t frame, const Speed &);

_DJV_IMAGE int64_t string_to_units(const String &, const Speed &);

_DJV_IMAGE String & operator >> (String &, Speed &) throw (String);
_DJV_IMAGE String & operator >> (String &, Time &) throw (String);
_DJV_IMAGE String & operator >> (String &, UNITS &) throw (String);
_DJV_IMAGE String & operator >> (String &, FPS &) throw (String);

_DJV_IMAGE String & operator << (String &, const Speed &);
_DJV_IMAGE String & operator << (String &, const Time &);
_DJV_IMAGE String & operator << (String &, UNITS);
_DJV_IMAGE String & operator << (String &, FPS);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_units();
_DJV_IMAGE const List<String> & label_fps();

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, const Speed &);
_DJV_IMAGE Debug & operator << (Debug &, const Time &);

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class _DJV_IMAGE Global
{
public:

  Global();

  ~Global();

  void speed(FPS);
  void units(UNITS);

  FPS speed() const;
  UNITS units() const;

private:

  FPS _speed;
  UNITS _units;
};

//! Access to global options.

_DJV_IMAGE Global * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

