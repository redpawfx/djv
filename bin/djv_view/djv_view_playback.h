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

//! \file djv_view_playback.h

#ifndef DJV_VIEW_PLAYBACK_H
#define DJV_VIEW_PLAYBACK_H

#include <djv_callback.h>

#include <djv_time.h>

namespace djv_view
{
using namespace djv;

class Playback_Group;

//------------------------------------------------------------------------------
//! \class Playback
//!
//! This class provides basic playback functionality.
//------------------------------------------------------------------------------

class Playback : public Callback
{
public:

    //! Constructor.

    Playback();

    //! Destructor.

    virtual ~Playback();

    //! Playback.

    enum PLAYBACK
    {
        STOP,
        FORWARD,
        REVERSE,

        _PLAYBACK_SIZE
    };

    //! Get the playback labels.

    static const List<String> & label_playback();

    //! Loop mode.

    enum LOOP
    {
        LOOP_ONCE,
        LOOP_REPEAT,
        LOOP_PING_PONG,

        _LOOP_SIZE
    };

    //! Get the loop mode labels.

    static const List<String> & label_loop();

    //! Timer type.

    enum TIMER
    {
        TIMER_SLEEP,
        TIMER_TIMEOUT,

        _TIMER_SIZE
    };

    //! Get the internal timer type labels.

    static const List<String> & label_timer();

    //! Set the timer type.

    void timer(TIMER);

    //! Get the timer type.

    TIMER timer() const;

    //! Set the timer resolution.

    void timer_resolution(double);

    //! Get the timer resolution.

    double timer_resolution() const;

    //! Add a timer.

    void timer_add(Playback_Group *);

    //! Remove a timer.

    void timer_del(Playback_Group *);

    //! User-interface layout.

    enum LAYOUT
    {
        LAYOUT_DEFAULT,
        LAYOUT_LEFT,
        LAYOUT_CENTER,
        LAYOUT_MINIMAL,

        _LAYOUT_SIZE
    };

    //! Get the user interface layout labels.

    static const List<String> & label_layout();

    //! Get the global playback options.

    static Playback * global();

private:

    DJV_FL_CALLBACK(Playback, timer_callback);

    TIMER                  _timer;
    List<Playback_Group *> _timers;
    double                 _timer_resolution;
    Timer                  _timer_time;
};

//------------------------------------------------------------------------------

String & operator >> (String &, Playback::PLAYBACK &) throw (String);
String & operator >> (String &, Playback::LOOP &) throw (String);
String & operator >> (String &, Playback::TIMER &) throw (String);
String & operator >> (String &, Playback::LAYOUT &) throw (String);

String & operator << (String &, Playback::PLAYBACK);
String & operator << (String &, Playback::LOOP);
String & operator << (String &, Playback::TIMER);
String & operator << (String &, Playback::LAYOUT);

} // djv_view

#endif // DJV_VIEW_PLAYBACK_H

