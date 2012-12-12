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

//! \file djv_glsl_application.h

#ifndef DJV_GLSL_APPLICATION_H
#define DJV_GLSL_APPLICATION_H

#include <djv_glsl_op.h>
#include <djv_glsl_widget.h>

#include <djv_application.h>
#include <djv_int_slider.h>
#include <djv_tool_button.h>
#include <djv_window.h>

#include <djv_image_io.h>

namespace djv_glsl_test
{
using namespace djv;

class Application : public djv::Application
{
public:

    Application(int argc, char ** argv);

    virtual ~Application();

private:

    void play(bool);

    DJV_CALLBACK(Application, play_callback, bool);
    DJV_CALLBACK(Application, frame_callback, int);
    DJV_CALLBACK(Application, op_callback, int);
    DJV_CALLBACK(Application, update_callback, bool);
    DJV_CALLBACK(Application, close_callback, bool);
    DJV_FL_CALLBACK(Application, idle_callback);

    void play_update();
    void frame_update();
    void update();

    std::auto_ptr<Image_Load> _load;
    Image_Io_Info             _info;
    Image                     _image;
    bool                      _play;
    int                       _frame;
    List<Op *>                _ops;
    int                       _op;
    djv::Window *             _image_window;
    Widget *                  _view;
    Tool_Button *             _play_widget;
    Int_Slider *              _slider;
    djv::Window *             _widget_window;
};

} // djv_glsl_test

#endif // DJV_GLSL_APPLICATION_H
