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

//! \file djv_view_application.h

#ifndef DJV_VIEW_APPLICATION_H
#define DJV_VIEW_APPLICATION_H

#include <djv_view_playback.h>

#include <djv_application.h>

#include <djv_pixel_data.h>

//! \namespace djv_view
//!
//! This program provides movie playback and image viewing.

namespace djv_view
{
using namespace djv;

class Window;

//------------------------------------------------------------------------------
//! \class Application
//!
//! This class provides the application.
//------------------------------------------------------------------------------

class Application : public djv::Application
{
public:

    //! Constructor.

    Application(int & argc, char ** argv);

    //! Destructor.

    virtual ~Application();

    //! Parse the command line.

    void command_line(String &) throw (Error);

    virtual String command_line_help() const;

private:

    Window * window() const;

    struct Command_Line
    {
        std::auto_ptr<bool> file_seq_auto;
        std::auto_ptr<int> file_layer;
        std::auto_ptr<Pixel_Data_Info::PROXY> file_proxy;
        std::auto_ptr<bool> file_cache;
        std::auto_ptr<bool> window_fullscreen;
        std::auto_ptr<Playback::PLAYBACK> playback;
        std::auto_ptr<int> playback_frame;
        std::auto_ptr<Speed> playback_speed;
    } _command_line;

    List<String> _input;
};

} // djv_view

#endif // DJV_VIEW_APPLICATION_H

