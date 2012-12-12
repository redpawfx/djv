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

//! \file djv_view_playback_prefs.h

#ifndef DJV_VIEW_PLAYBACK_PREFS_H
#define DJV_VIEW_PLAYBACK_PREFS_H

#include <djv_view_playback.h>

#include <djv_widget.h>

namespace djv_view
{

//------------------------------------------------------------------------------
//! \class Playback_Prefs
//!
//! This class provides playback preferences.
//------------------------------------------------------------------------------

class Playback_Prefs : public Callback
{
public:

    //! Constructor.

    Playback_Prefs();

    //! Destructor.

    virtual ~Playback_Prefs();

    //! Set whether playback is started automatically.

    void start(bool);

    //! Get whether playback is started automatically.

    bool start() const;

    //! Set the playback loop.

    void loop(Playback::LOOP);

    //! Get the playback loop.

    Playback::LOOP loop() const;

    //! This signal is emitted when the playbackl loop is changed.

    Signal<Playback::LOOP> loop_signal;

    //! Set whether every frame is played.

    void every_frame(bool);

    //! Get whether every frame is played.

    bool every_frame() const;

    //! This signal is emitted when every frame played is changed.

    Signal<bool> every_frame_signal;

    //! Set the playback layout.

    void layout(Playback::LAYOUT);

    //! Get the playback layout.

    Playback::LAYOUT layout() const;

    //! This signal is emitted when the playback layout is changed.

    Signal<Playback::LAYOUT> layout_signal;

    //! Get global preferences.

    static Playback_Prefs * global();

private:

    bool             _start;
    Playback::LOOP   _loop;
    bool             _every_frame;
    Playback::LAYOUT _layout;
};

//------------------------------------------------------------------------------
//! \class Playback_Prefs_Widget
//!
//! This class provides a playback preferences widget.
//------------------------------------------------------------------------------

class Playback_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    Playback_Prefs_Widget();

private:

    DJV_CALLBACK(Playback_Prefs_Widget, start_callback, bool);
    DJV_CALLBACK(Playback_Prefs_Widget, loop_callback, int);
    DJV_CALLBACK(Playback_Prefs_Widget, every_frame_callback, bool);
    DJV_CALLBACK(Playback_Prefs_Widget, layout_callback, int);
};

} // djv_view

#endif // DJV_VIEW_PLAYBACK_PREFS_H

