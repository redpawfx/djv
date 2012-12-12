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

//! \file djv_view_test.cpp

#include <djv_application.h>
#include <djv_row_layout.h>
#include <djv_window.h>
#include <djv_image_view.h>

#include <djv_file.h>
#include <djv_image_io.h>

#include <FL/Fl.H>

namespace djv_view_test
{
using namespace djv;

class Widget : public Image_View
{
public:

    int handle(int in)
    {
        const V2i mouse(
            Fl::event_x(),
            geom().h - 1 - Fl::event_y());

        switch (in)
        {
            case FL_SHORTCUT:
                switch (Fl::event_key())
                {
                    case '-':
                        zoom(zoom() * 0.5, mouse);
                        redraw();
                        return 1;

                    case '=':
                        zoom(zoom() * 2.0, mouse);
                        redraw();
                        return 1;

                    case FL_BackSpace:
                        view_zero();
                        redraw();
                        return 1;
                }

                break;

            case FL_PUSH:
                _view_tmp = view();
                _mouse_push = mouse;
                return 1;

            case FL_DRAG:
                view(_view_tmp + mouse - _mouse_push);
                redraw();
                return 1;
        }

        return Image_View::handle(in);
    }

private:

    V2i _view_tmp;
    V2i _mouse_push;
};

class Application : public djv::Application
{
public:

    Application(int & argc, char ** argv);

    virtual ~Application();

private:

    void open(const File &);

    DJV_CALLBACK(Application, exit_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }

    Image_Load *  _load;
    Image         _image;
    djv::Window * _window;
    Widget *      _widget;
};

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_view_test", argc, argv),
    _window(0)
{
    // Command line.

    if (argc != 2)
    {
        print("usage: djv_view_test (input)");
        exit(EXIT_ERROR);
        return;
    }

    File file(argv[1]);

    try
    {
        _load = Image_Load_Factory::global()->get(file);
        
        _load->load(_image);
    }
    catch (Error error)
    {
        this->error(error);
        exit(EXIT_ERROR);
        return;
    }

    is_valid(true);

    // Widgets.

    _window = new djv::Window(name());

    _widget = new Widget;
    _widget->set(&_image);

    Gl_Image_Options options;
    options.color_profile = _image.color_profile;
    _widget->options(options);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(_window);
    layout->spacing(0);
    layout->margin(0);
    layout->add(_widget);
    layout->stretch(_widget);

    // Initialize.

    _window->size(_image.size());

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
}

} // djv_view_test

int main(int argc, char ** argv)
{
    return djv_view_test::Application(argc, argv).exec();
}

