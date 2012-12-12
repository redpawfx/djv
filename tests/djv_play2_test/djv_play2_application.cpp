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

//! \file djv_play2_application.cpp

#include <djv_play2_application.h>

#include <djv_row_layout.h>

#include <FL/Fl.H>

namespace djv_play2_test
{

void update_callback(void *);

Application::Application(int argc, char ** argv) :
    djv::Application("djv_play2_test", argc, argv),
    _frame(0),
    _image0(0),
    _image1(0),
    _window(0)
{
    File::seq_extensions.add(List<String>() << ".ppm" << ".PPM");

    // Command line.

    if (argc != 2)
    {
        print("usage: test_image_play2 (input)");
        exit(EXIT_ERROR);
        return;
    }

    _file.set(argv[1]);

    if (_file.is_seq_valid())
    {
        _file.type(File::SEQ);
    }

    is_valid(true);

    // Widgets.

    _window = new djv::Window(name());

    _widget = new Widget;

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(_window);
    layout->margin(0);
    layout->add(_widget);
    layout->stretch(_widget);

    // Initialize.

    _image_buf.resize(20);
    List<Image_Ptr>::size_type i = 0;

    for (; i < _image_buf.size(); ++i)
    {
        _image_buf[i].p = new Image_Ppm;
    }

    i = 0;

    for (; i < _image_buf.size() - 1; ++i)
    {
        _image_buf[i].next = &_image_buf[i + 1];
    }

    _image_buf[i].next = &_image_buf[0];

    _image0 = &_image_buf[0];
    _image0->p->load(_file.get(_file.seq().list[_frame]));
    _image0->frame = _frame;

    _image1 = &_image_buf[0];

    //update();
    Fl::add_idle(update_callback, this);

    _window->size(_image0->p->info().size);
    _window->show();

    // Callbacks.

    _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
    if (_window)
    {
        _window->del();
    }

    for (size_t i = 0; i < _image_buf.size(); ++i)
        if (_image_buf[i].p)
        {
            delete _image_buf[i].p;
        }
}

void Application::update()
{
    //DJV_DEBUG("Application::update");

    static Timer t;
    static double average = 0.0;
    static int accum = 0;

    t.check();
    const double fps = t.fps();
    t.start();

    if (fps < 1000.0)
    {
        average += fps;
        ++accum;
    }

    print(String_Format("FPS = %% (%%)").
          arg(fps).
          arg(average / static_cast<double>(accum)));

    try
    {
        //_image->load(_file.get(_file.seq().list[_frame]));

        _widget->set(_image0->p);
        _widget->redraw();

        int frame = _image1->frame + 1;

        if (_image_buf.size() > 1)
        {
            while (_image1->next != _image0 &&
                    frame < static_cast<int>(_file.seq().list.size()))
            {
                //DJV_DEBUG_PRINT("advance = " << frame);

                _image1 = _image1->next;
                _image1->p->load(_file.get(_file.seq().list[frame]));
                _image1->frame = frame;

                ++frame;
            }
        }
        else
        {
            if (frame < static_cast<int>(_file.seq().list.size()))
            {
                _image1 = _image1->next;
                _image1->p->load(_file.get(_file.seq().list[frame]));
                _image1->frame = frame;
            }
        }

        _image0 = _image0->next;
    }
    catch (Error error)
    {
        djv::Application::error(error);
    }

    ++_frame;

    if (_frame >= static_cast<int>(_file.seq().list.size()))
        //_frame = 0;
    {
        exit(EXIT_DEFAULT);
    }
}

void update_callback(void * in)
{
    reinterpret_cast<Application *>(in)->update();
}

} // djv_play2_test

int main(int argc, char ** argv)
{
    int r = 1;

    try
    {
        r = djv_play2_test::Application(argc, argv).exec();
    }
    catch (djv::Error error)
    {
        djv::Error_Util::print(error);
    }

    return r;
}

