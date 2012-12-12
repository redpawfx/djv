/*------------------------------------------------------------------------------
 lib/djv_image/time.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "time.h"

#include <djv_base/file.h>
#include <djv_base/math.h>

namespace djv_image {
namespace time {

//------------------------------------------------------------------------------
// Speed
//------------------------------------------------------------------------------

Speed::Speed()
{
  set(global()->speed());
}

Speed::Speed(int scale, int duration) :
  scale(scale),
  duration(duration)
{}

Speed::Speed(FPS in)
{
  set(in);
}

void Speed::set(FPS in)
{
  switch (in)
  {
    case FPS_1:     scale = 1;     duration = 1;    break;
    case FPS_3:     scale = 3;     duration = 1;    break;
    case FPS_6:     scale = 6;     duration = 1;    break;
    case FPS_12:    scale = 12;    duration = 1;    break;
    case FPS_15:    scale = 15;    duration = 1;    break;
    case FPS_16:    scale = 16;    duration = 1;    break;
    case FPS_18:    scale = 18;    duration = 1;    break;
    case FPS_23_98: scale = 24000; duration = 1001; break;
    case FPS_24:    scale = 24;    duration = 1;    break;
    case FPS_25:    scale = 25;    duration = 1;    break;
    case FPS_29_97: scale = 30000; duration = 1001; break;
    case FPS_30:    scale = 30;    duration = 1;    break;
    case FPS_50:    scale = 50;    duration = 1;    break;
    case FPS_59_94: scale = 60000; duration = 1001; break;
    case FPS_60:    scale = 60;    duration = 1;    break;
    case FPS_120:   scale = 120;   duration = 1;    break;
  }
}
  
bool Speed::valid() const
{
  return scale != 0 && duration != 0;
}

//------------------------------------------------------------------------------
// Time
//------------------------------------------------------------------------------

Time::Time()
{}

Time::Time(const List<int64_t> & in, const Speed & speed) :
  Seq(in),
  speed(speed)
{}

Time::Time(int64_t start, int64_t end, const Speed & speed) :
  speed(speed)
{
  set(start, end);
}

//------------------------------------------------------------------------------
// timecode_to_time(), time_to_timecode()
//------------------------------------------------------------------------------

// XXX All timecode/keycode stuff is probably wrong.

void timecode_to_time(
  uint32_t in,
  int & hour,
  int & minute,
  int & second,
  int & frame
) {
  hour   = (in >> 28 & 0x0f) * 10 + (in >> 24 & 0x0f);
  minute = (in >> 20 & 0x0f) * 10 + (in >> 16 & 0x0f);
  second = (in >> 12 & 0x0f) * 10 + (in >> 8  & 0x0f);
  frame  = (in >> 4  & 0x0f) * 10 + (in >> 0  & 0x0f);
}

uint32_t time_to_timecode(int hour, int minute, int second, int frame)
{
  return
    (hour   / 10 & 0x0f) << 28 | (hour   % 10 & 0x0f) << 24 |
    (minute / 10 & 0x0f) << 20 | (minute % 10 & 0x0f) << 16 |
    (second / 10 & 0x0f) << 12 | (second % 10 & 0x0f) << 8  |
    (frame  / 10 & 0x0f) << 4  | (frame  % 10 & 0x0f) << 0;
}

//------------------------------------------------------------------------------
// timecode_to_frame(), frame_to_timecode()
//------------------------------------------------------------------------------

int64_t timecode_to_frame(uint32_t in, const time::Speed & speed)
{
  if (! speed.valid())
    return 0;

  int hour = 0, minute = 0, second = 0, frame = 0;
  timecode_to_time(in, hour, minute, second, frame);
  
  return
    static_cast<int64_t>(
      (hour * 60 * 60 + minute * 60 + second) * speed_to_float(speed)) +
    frame;
}

uint32_t frame_to_timecode(int64_t frame, const time::Speed & speed)
{
  DJV_DEBUG("frame_to_timecode");
  DJV_DEBUG_PRINT("frame = " << frame);
  DJV_DEBUG_PRINT("speed = " << speed);
  
  if (! speed.valid())
    return 0;
  
  const double speed_float = time::speed_to_float(speed);
  
  const int hour = static_cast<int>(frame / (speed_float * 60 * 60));
  frame -= static_cast<int>(hour * speed_float * 60 * 60);
  
  const int minute = static_cast<int>(frame / (speed_float * 60));
  frame -= static_cast<int>(minute * speed_float * 60);
  
  const int second = static_cast<int>(frame / speed_float);
  frame -= static_cast<int>(second * speed_float);
  
  DJV_DEBUG_PRINT("hour = " << hour);
  DJV_DEBUG_PRINT("minute = " << minute);
  DJV_DEBUG_PRINT("second = " << second);
  DJV_DEBUG_PRINT("frame = " << frame);
  
  const uint32_t out = time_to_timecode(hour, minute, second, frame);
  
  DJV_DEBUG_PRINT("out = " << timecode_to_string(out));
  
  return out;
}

//------------------------------------------------------------------------------
// timecode_to_string(), string_to_timecode()
//------------------------------------------------------------------------------

String timecode_to_string(uint32_t in)
{
  int hour = 0, minute = 0, second = 0, frame = 0;
  timecode_to_time(in, hour, minute, second, frame);
  
  return string::Format("%%:%%:%%:%%").
    arg(hour, 2, '0').
    arg(minute, 2, '0').
    arg(second, 2, '0').
    arg(frame, 2, '0');
}

uint32_t string_to_timecode(const String & in)
{
  int hour = 0, minute = 0, second = 0, frame = 0;

  List<String> tmp = string::split(in, ':');
  list::size_type i = 0;
  switch (tmp.size())
  {
    case 4: hour = string::string_to_int(tmp[i++]);
    case 3: minute = string::string_to_int(tmp[i++]);
    case 2: second = string::string_to_int(tmp[i++]);
    case 1: frame = string::string_to_int(tmp[i++]);
  }

  return time_to_timecode(hour, minute, second, frame);
}

//------------------------------------------------------------------------------
// keycode_to_string(), string_to_keycode()
//------------------------------------------------------------------------------

String keycode_to_string(int id, int type, int prefix, int count, int offset)
{
  return string::Format("%%:%%:%%:%%:%%").
    arg(id).
    arg(type).
    arg(prefix).
    arg(count).
    arg(offset);
}

bool string_to_keycode(
  const String & in,
  int & id,
  int & type,
  int & prefix,
  int & count,
  int & offset
) {
  const List<String> tmp = string::split(in, ':');
  if (tmp.size() != 5)
    return false;
  
  id = string::string_to_int(tmp[0]);
  type = string::string_to_int(tmp[1]);
  prefix = string::string_to_int(tmp[2]);
  count = string::string_to_int(tmp[3]);
  offset = string::string_to_int(tmp[4]);
  
  return true;
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Speed & a, const Speed & b)
{
  return a.scale == b.scale && a.duration == b.duration;
}

bool operator != (const Speed & a, const Speed & b)
{
  return ! (a == b);
}

bool operator == (const Time & a, const Time & b)
{
  return
    static_cast<const file_seq::Seq &>(a) ==
      static_cast<const file_seq::Seq &>(b) &&
    a.speed == b.speed;
}

bool operator != (const Time & a, const Time & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

double speed_to_float(const Speed & in)
{
  return in.scale / static_cast<double>(in.duration);
}

Speed float_to_speed(double in)
{
  // XXX
  
  if (math::abs(in - speed_to_float(FPS_1)) < 0.001)
    return FPS_1;
  else if (math::abs(in - speed_to_float(FPS_3)) < 0.001)
    return FPS_3;
  else if (math::abs(in - speed_to_float(FPS_6)) < 0.001)
    return FPS_6;
  else if (math::abs(in - speed_to_float(FPS_12)) < 0.001)
    return FPS_12;
  else if (math::abs(in - speed_to_float(FPS_15)) < 0.001)
    return FPS_15;
  else if (math::abs(in - speed_to_float(FPS_16)) < 0.001)
    return FPS_16;
  else if (math::abs(in - speed_to_float(FPS_18)) < 0.001)
    return FPS_18;
  else if (math::abs(in - speed_to_float(FPS_23_98)) < 0.001)
    return FPS_23_98;
  else if (math::abs(in - speed_to_float(FPS_24)) < 0.001)
    return FPS_24;
  else if (math::abs(in - speed_to_float(FPS_25)) < 0.001)
    return FPS_25;
  else if (math::abs(in - speed_to_float(FPS_29_97)) < 0.001)
    return FPS_29_97;
  else if (math::abs(in - speed_to_float(FPS_30)) < 0.001)
    return FPS_30;
  else if (math::abs(in - speed_to_float(FPS_50)) < 0.001)
    return FPS_50;
  else if (math::abs(in - speed_to_float(FPS_59_94)) < 0.001)
    return FPS_59_94;
  else if (math::abs(in - speed_to_float(FPS_60)) < 0.001)
    return FPS_60;
  else if (math::abs(in - speed_to_float(FPS_120)) < 0.001)
    return FPS_120;
  
  return Speed(math::round(in));
}

String units_to_string(int64_t frame, const Speed & speed)
{
  String out;
  
  switch (global()->units())
  {
    case UNITS_TIMECODE:
      out = timecode_to_string(frame_to_timecode(frame, speed));
      break;

    case UNITS_FRAMES:
      out = string::int_to_string(frame);
      break;
  }
  
  return out;
}

int64_t string_to_units(const String & in, const Speed & speed)
{
  int64_t out = 0;
  
  switch (global()->units())
  {
    case UNITS_TIMECODE:
      out = timecode_to_frame(string_to_timecode(in), speed);
      break;

    case UNITS_FRAMES:
      out = string::string_to_int64(in);
      break;
  }
  
  return out;
}

String & operator >> (String & in, Speed & out) throw (String)
{
  return in >> out.scale >> out.duration;
}

String & operator << (String & out, const Speed & in)
{
  return out << in.scale << in.duration;
}

String & operator >> (String & in, Time & out) throw (String)
{
  file_seq::operator >> (in, out);
  
  return in >> out.speed;
}

String & operator << (String & out, const Time & in)
{
  file_seq::operator << (out, in);
  
  return out << in.speed;
}

_DJV_STRING_OPERATOR_LABEL(UNITS, label_units())
_DJV_STRING_OPERATOR_LABEL(FPS, label_fps())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_units()
{
  static const List<String> data = List<String>() <<
    "Timecode" <<
    "Frames";
  
  DJV_ASSERT(data.size() == _UNITS_SIZE);

  return data;
}

const List<String> & label_fps()
{
  static const List<String> data = List<String>() <<
    "1" <<
    "3" <<
    "6" <<
    "12" <<
    "15" <<
    "16" <<
    "18" <<
    "23.98" <<
    "24" <<
    "25" <<
    "29.97" <<
    "30" <<
    "50" <<
    "59.94" <<
    "60" <<
    "120";
  
  DJV_ASSERT(data.size() == _FPS_SIZE);

  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const Speed & in)
{
  return debug << speed_to_float(in);
}

Debug & operator << (Debug & debug, const Time & in)
{
  return debug << string::label(in) << "@" << speed_to_float(in.speed);
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  _speed(FPS_24),
  _units(UNITS_TIMECODE)
{
  DJV_DEBUG("Global::Global");
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");
}

void Global::speed(FPS in)
{
  _speed = in;
}

FPS Global::speed() const
{
  return _speed;
}

void Global::units(UNITS in)
{
  _units = in;
}

UNITS Global::units() const
{
  return _units;
}

//------------------------------------------------------------------------------

}
}
