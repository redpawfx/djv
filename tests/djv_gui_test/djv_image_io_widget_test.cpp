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

//! \file djv_image_io_widget_test.cpp

#include <djv_application.h>
#include <djv_image_io_widget.h>
#include <djv_row_layout.h>
#include <djv_tabs.h>
#include <djv_window.h>

#include <djv_image_io.h>

namespace djv_image_io_widget_test
{
using namespace djv;

class Application : public djv::Application
{
public:

    Application(int & argc, char ** argv);

    virtual ~Application();

private:

    DJV_CALLBACK(Application, exit_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }

    djv::Window * _window;
};

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_image_io_widget_test", argc, argv),
    _window(0)
{
    is_valid(true);

    // Widgets.

    Tabs * tabs_widget = new Tabs;

    Tabs * base_tabs_widget = new Tabs;

    tabs_widget->tab_add("Base", base_tabs_widget);

    const List<Plugin *> & base_plugins =
        Image_Io_Base_Factory::global()->plugins();

    for (size_t i = 0; i < base_plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin =
            dynamic_cast<Image_Io_Base *>(base_plugins[i]))
        {
            Image_Io_Base_Widget * widget =
                Image_Io_Base_Widget_Factory::global()->get(plugin);

            if (! widget)
            {
                continue;
            }

            widget->set(plugin);

            base_tabs_widget->tab_add(plugin->name(), widget);
        }
    }

    Tabs * load_tabs_widget = new Tabs;

    tabs_widget->tab_add("Load", load_tabs_widget);

    const List<Plugin *> & load_plugins =
        Image_Load_Factory::global()->plugins();

    for (size_t i = 0; i < load_plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(load_plugins[i]))
        {
            Image_Load_Widget * widget =
                Image_Load_Widget_Factory::global()->get(plugin);

            if (! widget)
            {
                continue;
            }

            widget->set(plugin);

            load_tabs_widget->tab_add(plugin->name(), widget);
        }
    }

    Tabs * save_tabs_widget = new Tabs;

    tabs_widget->tab_add("Save", save_tabs_widget);

    const List<Plugin *> & save_plugins =
        Image_Save_Factory::global()->plugins();

    for (size_t i = 0; i < save_plugins.size(); ++i)
    {
        if (Image_Save * plugin = dynamic_cast<Image_Save *>(save_plugins[i]))
        {
            Image_Save_Widget * widget =
                Image_Save_Widget_Factory::global()->get(plugin);

            if (! widget)
            {
                continue;
            }

            widget->set(plugin);

            save_tabs_widget->tab_add(plugin->name(), widget);
        }
    }

    _window = new djv::Window(name());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(_window);
    layout->margin(0);
    layout->add(tabs_widget);

    // Initialize.

    _window->size(V2i(500, 400));
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

} // djv_image_io_widget_test

int main(int argc, char ** argv)
{
    return djv_image_io_widget_test::Application(argc, argv).exec();
}

