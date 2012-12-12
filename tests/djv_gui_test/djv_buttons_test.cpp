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

//! \file djv_buttons_test.cpp

#include <djv_application.h>
#include <djv_check_button.h>
#include <djv_frame_group.h>
#include <djv_push_button.h>
#include <djv_radio_button.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>
#include <djv_tool_button.h>
#include <djv_window.h>

namespace djv_buttons_test
{
using namespace djv;

class Application : public djv::Application
{
public:

    Application(int & argc, char ** argv);

    virtual ~Application();

private:

    DJV_CALLBACK(Application, push_callback, bool)
    {
        print("push");
    }
    DJV_CALLBACK(Application, check_callback, bool)
    {
        print(String_Format("check = %%").arg(in));
    }
    DJV_CALLBACK(Application, radio_callback, int)
    {
        print(String_Format("radio = %%").arg(in));
    }
    DJV_CALLBACK(Application, tool_callback0, bool)
    {
        print("tool");
    }
    DJV_CALLBACK(Application, tool_callback1, bool)
    {
        print(String_Format("tool = %%").arg(in));
    }
    DJV_CALLBACK(Application, tool_callback2, bool)
    {
        print("tool repeat");
    }

    DJV_CALLBACK(Application, exit_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }

    djv::Window * _window;
};

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_buttons_test", argc, argv),
    _window(0)
{
    is_valid(true);

    // Widgets.

    Push_Button * push_button = new Push_Button("&Push Button");

    Check_Button * check_button = new Check_Button("&Check button");

    Radio_Button_Group * radio_group = new Radio_Button_Group(
        "Radio Buttons",
        List<String>() << "&1" << "&2" << "&3");

    Tool_Button * tool_button0 = new Tool_Button("file");
    tool_button0->type(Tool_Button::PUSH);
    Tool_Button * tool_button1 = new Tool_Button("directory");
    tool_button1->type(Tool_Button::TOGGLE);
    Tool_Button * tool_button2 = new Tool_Button("magnify");
    tool_button2->type(Tool_Button::REPEAT);

    Horizontal_Frame_Group * tool_group = new Horizontal_Frame_Group(
        List<Frame *>() <<
        tool_button0 <<
        tool_button1 <<
        tool_button2);

    _window = new djv::Window(name());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(_window);
    layout->spacing(Style::global()->spacing_large());
    layout->add(push_button);
    layout->add(check_button);
    layout->add(radio_group);
    layout->add(tool_group);

    layout->add_stretch();

    // Initialize.

    radio_group->set(0);

    _window->size(_window->size_hint());
    _window->show();

    // Callbacks.

    push_button->signal.set(this, push_callback);
    check_button->signal.set(this, check_callback);
    radio_group->signal.set(this, radio_callback);
    tool_button0->signal.set(this, tool_callback0);
    tool_button1->signal.set(this, tool_callback1);
    tool_button2->signal.set(this, tool_callback2);
    _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
    if (_window)
    {
        _window->del();
    }
}

} // djv_buttons_test

int main(int argc, char ** argv)
{
    return djv_buttons_test::Application(argc, argv).exec();
}

