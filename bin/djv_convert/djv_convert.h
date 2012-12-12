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

//! \file djv_convert.h

#ifndef DJV_CONVERT_H
#define DJV_CONVERT_H

#include <djv_core_application.h>
#include <djv_file.h>
#include <djv_gl_image.h>
#include <djv_gl_offscreen_buffer.h>
#include <djv_image.h>
#include <djv_time.h>

//! \namespace djv_convert
//!
//! This program provides a command line tool for movie and image conversion.

namespace  djv_convert
{

using namespace djv;

//------------------------------------------------------------------------------
//! \struct Options
//!
//! This struct provides options.
//------------------------------------------------------------------------------

struct Options
{
    //! Constructor.

    Options();

    V2b                       mirror;
    V2f                       scale;
    Gl_Image_Options::CHANNEL channel;
    V2i                       size;
};

//------------------------------------------------------------------------------
//! \struct Input
//!
//! This struct provides input options.
//------------------------------------------------------------------------------

struct Input
{
    //! Constructor.

    Input();

    File                   file;
    int                    layer;
    Pixel_Data_Info::PROXY proxy;
    String                 start;
    String                 end;
    File                   slate;
    int                    slate_frames;
    int                    timeout;
};

//------------------------------------------------------------------------------
//! \struct Output
//!
//! This struct provides output options.
//------------------------------------------------------------------------------

struct Output
{
    //! Constructor.

    Output();

    File                            file;
    std::auto_ptr<Pixel::PIXEL>     pixel;
    Image_Tag                       tag;
    bool                            tag_auto;
    std::auto_ptr<Speed::FPS>       speed;
};

//------------------------------------------------------------------------------
//! \class Application
//!
//! The class provides the application.
//------------------------------------------------------------------------------

class Application : public Core_Application
{
public:

    //! Constructor.

    Application(int, char **);

    //! Parse the command line.

    void command_line(String &) throw (Error);

    virtual String command_line_help() const;

private:

    bool work();

    Options                            _options;
    Input                              _input;
    Output                             _output;
    std::auto_ptr<Gl_Offscreen_Buffer> _offscreen_buffer;
    Gl_Image_State                     _state;
};

} // namespace

#endif // DJV_CONVERT_H

