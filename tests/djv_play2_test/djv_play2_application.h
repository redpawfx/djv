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

//! \file djv_play2_application.h

#ifndef DJV_PLAY2_APPLICATION_H
#define DJV_PLAY2_APPLICATION_H

#include <djv_play2_widget.h>

#include <djv_application.h>
#include <djv_window.h>

#include <djv_file.h>

namespace djv_play2_test
{
using namespace djv;

struct Image_Ptr
{
    Image_Ptr() :
        p    (0),
        frame(0),
        next (0)
    {}

    Image *     p;
    int         frame;
    Image_Ptr * next;
};

class Application : public djv::Application
{
public:

    Application(int argc, char ** argv);

    virtual ~Application();

    void update();

private:

    DJV_CALLBACK(Application, exit_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }

    File _file;
    int _frame;

    Image_Ptr * _image0;
    Image_Ptr * _image1;
    List<Image_Ptr> _image_buf;

    djv::Window * _window;
    Widget *      _widget;
};

} // djv_play2_test

#endif // DJV_PLAY2_APPLICATION_H

