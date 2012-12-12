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

//! \file djv_view_playback.cpp

#include <djv_view_playback.h>

#include <djv_view_playback_group.h>

#include <djv_assert.h>
#include <djv_time.h>

#include <FL/Fl.H>

#include <algorithm>

namespace djv_view
{
using djv::Time;

//------------------------------------------------------------------------------
// Playback
//------------------------------------------------------------------------------

Playback::Playback() :
    _timer(TIMER_SLEEP),
    _timer_resolution(0.001)
{}

Playback::~Playback()
{
    Fl::remove_idle(timer_callback, this);
    Fl::remove_timeout(timer_callback, this);
}

const List<String> & Playback::label_playback()
{
    static const List<String> data = List<String>() <<
        "Stop" <<
        "Forward" <<
        "Reverse";

    DJV_ASSERT(data.size() == _PLAYBACK_SIZE);

    return data;
}

const List<String> & Playback::label_loop()
{
    static const List<String> data = List<String>() <<
        "Once" <<
        "Repeat" <<
        "Ping-Pong";

    DJV_ASSERT(data.size() == _LOOP_SIZE);

    return data;
}

const List<String> & Playback::label_timer()
{
    static const List<String> data = List<String>() <<
        "Sleep" <<
        "Timeout";

    DJV_ASSERT(data.size() == _TIMER_SIZE);

    return data;
}

void Playback::timer(TIMER in)
{
    _timer = in;
}

Playback::TIMER Playback::timer() const
{
    return _timer;
}

void Playback::timer_resolution(double in)
{
    _timer_resolution = in;
}

double Playback::timer_resolution() const
{
    return _timer_resolution;
}

void Playback::timer_add(Playback_Group * in)
{
    //DJV_DEBUG("Playback::timer_add");
    //DJV_DEBUG_PRINT("size = " << static_cast<int>(_timers.size()));

    _timers += in;

    if (1 == _timers.size())
    {
        switch (_timer)
        {
            case TIMER_SLEEP:

                _timer_time.start();

                //DJV_DEBUG_PRINT("add timer");

                Fl::add_idle(timer_callback, this);

                break;

            case TIMER_TIMEOUT:

                //DJV_DEBUG_PRINT("add timer");

                Fl::add_timeout(_timer_resolution, timer_callback, this);

                break;

            default:
                break;
        }
    }

    //DJV_DEBUG_PRINT("size = " << static_cast<int>(_timers.size()));
}

void Playback::timer_del(Playback_Group * in)
{
    //DJV_DEBUG("Playback::timer_del");
    //DJV_DEBUG_PRINT("size = " << static_cast<int>(_timers.size()));

    List<Playback_Group *>::iterator i = std::find(
            _timers.begin(),
            _timers.end(),
            in);

    if (i != _timers.end())
    {
        _timers.erase(i);
    }

    if (! _timers.size())
    {
        //DJV_DEBUG_PRINT("remove timer");

        Fl::remove_idle(timer_callback, this);

        Fl::remove_timeout(timer_callback, this);
    }

    //DJV_DEBUG_PRINT("size = " << static_cast<int>(_timers.size()));
}

const List<String> & Playback::label_layout()
{
    static const List<String> data = List<String>() <<
        "Default" <<
        "Left" <<
        "Center" <<
        "Minimal";

    DJV_ASSERT(data.size() == _LAYOUT_SIZE);

    return data;
}

Playback * Playback::global()
{
    static Playback * global = 0;

    if (! global)
    {
        global = new Playback;
    }

    return global;
}

void Playback::timer_callback()
{
    //DJV_DEBUG("Playback::timer_callback");
    //DJV_DEBUG_PRINT("size = " << static_cast<int>(_timers.size()));

    for (size_t i = 0; i < _timers.size(); ++i)
    {
        _timers[i]->idle();
    }

    switch (_timer)
    {
        case TIMER_SLEEP:
            
            while (_timer_time.seconds() < _timer_resolution)
            {
                Time::sleep(0.0);
                
                _timer_time.check();
            }

            _timer_time.start();
            
            break;

        case TIMER_TIMEOUT:
            
            Fl::repeat_timeout(_timer_resolution, timer_callback, this);
            
            break;

        default:
            break;
    }
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Playback::PLAYBACK, Playback::label_playback())
_DJV_STRING_OPERATOR_LABEL(Playback::LOOP, Playback::label_loop())
_DJV_STRING_OPERATOR_LABEL(Playback::TIMER, Playback::label_timer())
_DJV_STRING_OPERATOR_LABEL(Playback::LAYOUT, Playback::label_layout())

} // djv_view

