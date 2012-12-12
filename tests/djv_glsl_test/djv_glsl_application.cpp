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

//! \file djv_glsl_application.cpp

#include <djv_glsl_application.h>

#include <djv_glsl_color.h>
#include <djv_glsl_levels.h>
#include <djv_glsl_exposure.h>
#include <djv_glsl_blur.h>
#include <djv_glsl_sharpen.h>
#include <djv_glsl_edge.h>
#include <djv_glsl_scale.h>

#include <djv_row_layout.h>
#include <djv_tabs.h>

#include <djv_gl_offscreen_buffer.h>

#include <FL/Fl.H>

namespace djv_glsl_test
{

Application::Application(int argc, char ** argv) :
    djv::Application("djv_glsl_test", argc, argv),
    _play         (false),
    _frame        (0),
    _op           (0),
    _image_window (0),
    _widget_window(0)
{
    //DJV_DEBUG("Application::Application");

    if (argc != 2)
    {
        print("usage: djv_glsl_test (image)");
        return;
    }

    try
    {
        File file(argv[1]);

        if (file.is_seq_valid())
        {
            file.type(File::SEQ);
        }

        _load = std::auto_ptr<Image_Load>(
            Image_Load_Factory::global()->get(file, &_info));
    }
    catch (Error error)
    {
        this->error(error);
        return;
    }

    is_valid(true);

    _ops += new Null_Op();
    _ops += new Color_Op();
    _ops += new Levels_Op();
    _ops += new Exposure_Op();
    _ops += new Blur_Op();
    _ops += new Sharpen_Op();
    _ops += new Edge_Op();
    _ops += new Scale_Op();

    _image_window = new djv::Window("djv_glsl_test");
    _view = new Widget;
    _play_widget = new Tool_Button("playback_forward");
    _play_widget->type(Tool_Button::TOGGLE);
    _slider = new Int_Slider;

    _widget_window = new djv::Window("djv_glsl_test");
    Tabs * tabs_widget = new Tabs;

    for (size_t i = 0; i < _ops.size(); ++i)
    {
        tabs_widget->tab_add(_ops[i]->name(), _ops[i]);
    }

    Vertical_Layout * layout = new Vertical_Layout(_image_window);
    layout->margin(0);
    layout->spacing(0);
    layout->add(_view);
    layout->stretch(_view);
    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(_play_widget);
    layout_h->add(_slider);

    layout = new Vertical_Layout(_widget_window);
    layout->margin(0);
    layout->spacing(0);
    layout->add(tabs_widget);

    update();

    _slider->range(0, static_cast<int>(_info.seq.list.size()) - 1);
    _widget_window->size(V2i(300, 300));
    _widget_window->show();
    _image_window->dirty();
    _image_window->size(_info.size + V2i(0, _image_window->size_min().y));
    _image_window->show();
    tabs_widget->set(_op);

    _image_window->close_signal.set(this, close_callback);
    _play_widget->signal.set(this, play_callback);
    _slider->signal.set(this, frame_callback);
    tabs_widget->signal.set(this, op_callback);

    for (size_t i = 0; i < _ops.size(); ++i)
    {
        _ops[i]->signal.set(this, update_callback);
    }

    _widget_window->close_signal.set(this, close_callback);
}

Application::~Application()
{
    //DJV_DEBUG("Application::~Application");

    if (_image_window)
    {
        _image_window->del();
    }

    if (_widget_window)
    {
        _widget_window->del();
    }
}

void Application::play_callback(bool in)
{
    _play = in;
    play_update();
}

void Application::frame_callback(int in)
{
    _play = false;
    _frame = in;
    play_update();
    frame_update();
    update();
}

void Application::op_callback(int in)
{
    _op = in;
    update();
}

void Application::update_callback(bool)
{
    update();
}

void Application::play_update()
{
    callbacks(false);

    if (_play)
    {
        Fl::add_idle(idle_callback, this);
    }
    else
    {
        Fl::remove_idle(idle_callback, this);
    }

    _play_widget->set(_play);

    callbacks(true);
}

void Application::frame_update()
{
    callbacks(false);

    _slider->set(_frame);

    callbacks(true);
}

void Application::update()
{
    callbacks(false);

    try
    {
        const int64_t frame =
            _info.seq.list.size() ? _info.seq.list[_frame] : -1;
        
        _load->load(_image, frame);
        
        _view->set(_ops[_op], &_image);
        _view->redraw();
    }
    catch (Error) {}

    callbacks(true);
}

void Application::close_callback(bool)
{
    exit(EXIT_DEFAULT);
}

void Application::idle_callback()
{
    _frame++;

    if (_frame >= static_cast<int>(_info.seq.list.size()))
    {
        _frame = 0;
    }

    frame_update();
    update();
}

} // djv_glsl_test

int main(int argc, char ** argv)
{
    return djv_glsl_test::Application(argc, argv).exec();
}

