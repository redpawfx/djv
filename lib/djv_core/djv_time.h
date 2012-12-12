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

//! \file djv_time.h

#ifndef DJV_TIME_H
#define DJV_TIME_H

#include <djv_speed.h>

//#if ! defined(DJV_WINDOWS)
#include <sys/time.h>
//#endif

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Time
//!
//! This struct provides time utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Time
{
    //! Get the current time.

    static ::time_t current();

    //! Go to sleep.

    static void sleep(double seconds);

    //! Convert seconds to a time.

    static void seconds_to_time(
        double,
        int * hours,
        int * minutes,
        double * seconds);

    //! Convert a time to a string.

    static String time_to_string(::time_t);

    //! Get a time label.

    static String label_time(double seconds);

    //! Convert timecode to time.

    static void timecode_to_time(
        uint32_t,
        int & hour,
        int & minute,
        int & second,
        int & frame);

    //! Convert time to timecode.

    static uint32_t time_to_timecode(
        int hour,
        int minute,
        int second,
        int frame);

    //! Convert timecode to a frame.

    static int64_t timecode_to_frame(uint32_t timecode, const Speed &);

    //! Convert a frame to timecode.

    static uint32_t frame_to_timecode(int64_t frame, const Speed &);

    //! Convert timecode to a string.

    static String timecode_to_string(uint32_t);

    //! Convert a string to timecode.

    static uint32_t string_to_timecode(const String &);

    //! Convert a keycode to a string.

    static String keycode_to_string(
        int id,
        int type,
        int prefix,
        int count,
        int offset);

    //! Convert a string to a keycode.

    static bool string_to_keycode(
        const String &,
        int & id,
        int & type,
        int & prefix,
        int & count,
        int & offset);

    //! Time units.

    enum UNITS
    {
        UNITS_TIMECODE,
        UNITS_FRAMES,

        _UNITS_SIZE
    };

    //! Get the time unit labels.

    static const List<String> & label_units();

    //! Convert a frame to a string.

    static String units_to_string(int64_t frame, const Speed &);

    //! Convert a string to a frame.

    static int64_t string_to_units(const String &, const Speed &);

    //! The default time units.

    static UNITS default_units;
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT String & operator >> (String &, Time::UNITS &) throw (String);

DJV_CORE_EXPORT String & operator << (String &, Time::UNITS);

} // djv

#endif // DJV_TIME_H

