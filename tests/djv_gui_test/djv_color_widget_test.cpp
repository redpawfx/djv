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

//! \file djv_color_widget_test.cpp

#include <djv_application.h>
#include <djv_color_swatch.h>
#include <djv_color_widget.h>
#include <djv_row_layout.h>
#include <djv_window.h>

namespace djv_color_widget_test
{
using namespace djv;

class Application : public djv::Application
{
public:

    Application(int & argc, char ** argv);

    virtual ~Application();

private:

    DJV_CALLBACK(Application, color_widget_callback, const Color &);
    DJV_CALLBACK(Application, exit_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }

    Color_Swatch * _swatch;
    djv::Window * _window;
};

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_color_widget_test", argc, argv),
    _window(0)
{
    is_valid(true);

    _swatch = new Color_Swatch;
    _swatch->size(Layout_Item::STRETCH, Layout_Item::STRETCH);

    Color_Widget * color_widget = new Color_Widget;
    color_widget->align(Layout_Item::LEFT, Layout_Item::LEFT);

    _window = new djv::Window(name());

    Horizontal_Layout * layout = new Horizontal_Layout(_window);
    layout->add(_swatch);
    layout->add(color_widget);
    layout->stretch(color_widget);

    _window->size(_window->size_hint());
    _window->show();

    color_widget->signal.set(this, color_widget_callback);
    _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
    if (_window)
    {
        _window->del();
    }
}

void Application::color_widget_callback(const Color & in)
{
    _swatch->set(in);

    print(String_Format("value = %%").arg(String_Util::label(in)));
}

} // djv_color_widget_test

int main(int argc, char ** argv)
{
    return djv_color_widget_test::Application(argc, argv).exec();
}

