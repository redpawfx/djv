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

//! \file djv_tabs_test.cpp

#include <djv_application.h>
#include <djv_color_widget.h>
#include <djv_row_layout.h>
#include <djv_tabs.h>
#include <djv_tool_button.h>
#include <djv_window.h>

namespace djv_tabs_test
{
using namespace djv;

class Application : public djv::Application
{
public:

    Application(int & argc, char ** argv);

    virtual ~Application();

private:

    void add();
    void del();

    DJV_CALLBACK(Application, widget_callback, int)
    {
        print(String_Format("tab = %%").arg(in));
    }
    DJV_CALLBACK(Application, add_callback, bool)
    {
        add();
    }
    DJV_CALLBACK(Application, del_callback, bool)
    {
        del();
    }
    DJV_CALLBACK(Application, exit_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }

    Tabs * _widget;
    djv::Window * _window;
};

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_tabs_test", argc, argv),
    _window(0)
{
    is_valid(true);

    // Widgets.

    _widget = new Tabs;

    Tool_Button * add_button = new Tool_Button("tool_add");
    add_button->type(Tool_Button::REPEAT);
    Tool_Button * del_button = new Tool_Button("tool_x");
    del_button->type(Tool_Button::REPEAT);

    _window = new djv::Window(name());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(_window);
    layout->spacing(0);
    layout->margin(0);
    layout->add(_widget);
    layout->stretch(_widget);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->add_stretch();
    layout_h->add(add_button);
    layout_h->add(del_button);

    // Initialize.

    add();

    _window->size(V2i(300, 300));
    _window->show();

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    add_button->signal.set(this, add_callback);
    del_button->signal.set(this, del_callback);
    _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
    if (_window)
    {
        _window->del();
    }
}

void Application::add()
{
    _widget->tab_add(
        String_Format("tab %%").arg(_widget->tab_size()),
        new Color_Widget);
}

void Application::del()
{
    if (! _widget->tab_size())
    {
        return;
    }

    _widget->tab_del(_widget->get());
}

} // namespace djv_tabs_test

int main(int argc, char ** argv)
{
    return djv_tabs_test::Application(argc, argv).exec();
}

