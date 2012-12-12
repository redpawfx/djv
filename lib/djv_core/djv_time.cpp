//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_time.cpp

#include <djv_time.h>

#include <djv_assert.h>

#if defined(DJV_WINDOWS)
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <float.h>
#include <time.h>

namespace djv
{

//------------------------------------------------------------------------------
// Time
//------------------------------------------------------------------------------

::time_t Time::current()
{
    return ::time(0);
}

void Time::sleep(double seconds)
{
    //! \todo double/int

#if defined(DJV_WINDOWS)

    ::Sleep(seconds * 1000);

#else // DJV_WINDOWS

    ::sleep(seconds);

#endif // DJV_WINDOWS
}

void Time::seconds_to_time(double in, int * hour, int * minute, double * second)
{
    *hour = static_cast<int>(in) / (60 * 60);
    in -= *hour * 60 * 60;

    *minute = static_cast<int>(in) / 60;
    in -= *minute * 60;

    *second = in;
}

String Time::time_to_string(::time_t in)
{
    String out;

#if defined(DJV_WINDOWS)

    //! \todo MinGW does not define ctime_s()?
    //char tmp [cstring_size] = "";
    //::ctime_s(tmp, cstring_size, &in);
    //out = tmp;
    out = ::ctime(&in);

#else // DJV_WINDOWS

    out = ::ctime(&in);

#endif // DJV_WINDOWS

    //! \todo Remove newline.

    if (out.size())
    {
        out.erase(out.size() - 1);
    }

    return out;
}

String Time::label_time(double in)
{
    int hour = 0, minute = 0;
    double second = 0.0;
    seconds_to_time(in, &hour, &minute, &second);

    return String_Format("%%:%%:%%").
           arg(hour, 2, '0').
           arg(minute, 2, '0').
           arg(second, 5, 2, '0');
}

//! \todo All timecode/keycode stuff is probably wrong.

void Time::timecode_to_time(
    uint32_t in,
    int &    hour,
    int &    minute,
    int &    second,
    int &    frame)
{
    hour   = (in >> 28 & 0x0f) * 10 + (in >> 24 & 0x0f);
    minute = (in >> 20 & 0x0f) * 10 + (in >> 16 & 0x0f);
    second = (in >> 12 & 0x0f) * 10 + (in >> 8  & 0x0f);
    frame  = (in >> 4  & 0x0f) * 10 + (in >> 0  & 0x0f);
}

uint32_t Time::time_to_timecode(
    int hour,
    int minute,
    int second,
    int frame)
{
    return
        (hour   / 10 & 0x0f) << 28 | (hour   % 10 & 0x0f) << 24 |
        (minute / 10 & 0x0f) << 20 | (minute % 10 & 0x0f) << 16 |
        (second / 10 & 0x0f) << 12 | (second % 10 & 0x0f) << 8  |
        (frame  / 10 & 0x0f) << 4  | (frame  % 10 & 0x0f) << 0;
}

int64_t Time::timecode_to_frame(uint32_t in, const Speed & speed)
{
    if (! speed.is_valid())
    {
        return 0;
    }

    int hour = 0, minute = 0, second = 0, frame = 0;
    timecode_to_time(in, hour, minute, second, frame);

    return
        static_cast<int64_t>(
            (hour * 60 * 60 + minute * 60 + second) *
            Speed::speed_to_float(speed)) +
        frame;
}

uint32_t Time::frame_to_timecode(int64_t frame, const Speed & speed)
{
    //DJV_DEBUG("Time::frame_to_timecode");
    //DJV_DEBUG_PRINT("frame = " << frame);
    //DJV_DEBUG_PRINT("speed = " << speed);

    if (! speed.is_valid())
    {
        return 0;
    }

    const double speed_float = Speed::speed_to_float(speed);

    const int hour = static_cast<int>(frame / (speed_float * 60 * 60));
    frame -= static_cast<int>(hour * speed_float * 60 * 60);

    const int minute = static_cast<int>(frame / (speed_float * 60));
    frame -= static_cast<int>(minute * speed_float * 60);

    const int second = static_cast<int>(frame / speed_float);
    frame -= static_cast<int>(second * speed_float);

    //DJV_DEBUG_PRINT("hour = " << hour);
    //DJV_DEBUG_PRINT("minute = " << minute);
    //DJV_DEBUG_PRINT("second = " << second);
    //DJV_DEBUG_PRINT("frame = " << frame);

    const uint32_t out = time_to_timecode(hour, minute, second, frame);

    //DJV_DEBUG_PRINT("out = " << timecode_to_string(out));

    return out;
}

String Time::timecode_to_string(uint32_t in)
{
    int hour = 0, minute = 0, second = 0, frame = 0;
    timecode_to_time(in, hour, minute, second, frame);

    return String_Format("%%:%%:%%:%%").
           arg(hour, 2, '0').
           arg(minute, 2, '0').
           arg(second, 2, '0').
           arg(frame, 2, '0');
}

uint32_t Time::string_to_timecode(const String & in)
{
    int hour = 0, minute = 0, second = 0, frame = 0;

    List<String> tmp = String_Util::split(in, ':');
    size_t i = 0;

    switch (tmp.size())
    {
        case 4:
            hour   = String_Util::string_to_int(tmp[i++]);

        case 3:
            minute = String_Util::string_to_int(tmp[i++]);

        case 2:
            second = String_Util::string_to_int(tmp[i++]);

        case 1:
            frame  = String_Util::string_to_int(tmp[i++]);
    }

    return time_to_timecode(hour, minute, second, frame);
}

String Time::keycode_to_string(
    int id,
    int type,
    int prefix,
    int count,
    int offset)
{
    return String_Format("%%:%%:%%:%%:%%").
           arg(id).
           arg(type).
           arg(prefix).
           arg(count).
           arg(offset);
}

bool Time::string_to_keycode(
    const String & in,
    int &          id,
    int &          type,
    int &          prefix,
    int &          count,
    int & offset)
{         
    const List<String> tmp = String_Util::split(in, ':');

    if (tmp.size() != 5)
    {
        return false;
    }

    id     = String_Util::string_to_int(tmp[0]);
    type   = String_Util::string_to_int(tmp[1]);
    prefix = String_Util::string_to_int(tmp[2]);
    count  = String_Util::string_to_int(tmp[3]);
    offset = String_Util::string_to_int(tmp[4]);

    return true;
}

const List<String> & Time::label_units()
{
    static const List<String> data = List<String>() <<
        "Timecode" <<
        "Frames";

    DJV_ASSERT(data.size() == _UNITS_SIZE);

    return data;
}

String Time::units_to_string(int64_t frame, const Speed & speed)
{
    String out;

    switch (default_units)
    {
        case UNITS_TIMECODE:
            out = timecode_to_string(frame_to_timecode(frame, speed));
            break;

        case UNITS_FRAMES:
            out = String_Util::int_to_string(frame);
            break;

        default:
            break;
    }

    return out;
}

int64_t Time::string_to_units(
    const String & in,
    const Speed & speed)
{
    int64_t out = 0;

    switch (default_units)
    {
        case UNITS_TIMECODE:
            out = timecode_to_frame(string_to_timecode(in), speed);
            break;

        case UNITS_FRAMES:
            out = String_Util::string_to_int64(in);
            break;

        default:
            break;
    }

    return out;
}

Time::UNITS Time::default_units = Time::UNITS_TIMECODE;

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Time::UNITS, Time::label_units())

} // djv

