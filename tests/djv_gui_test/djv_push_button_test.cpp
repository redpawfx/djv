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

//! \file djv_push_button_test.cpp

#include <djv_application.h>
#include <djv_row_layout.h>
#include <djv_push_button.h>
#include <djv_window.h>

namespace djv_push_button_test
{
using namespace djv;

class Application : public djv::Application
{
public:

    Application(int & argc, char ** argv);

    virtual ~Application();

private:

    DJV_CALLBACK(Application, callback1, bool)
    {
        print("button 1");
    }
    DJV_CALLBACK(Application, callback2, bool)
    {
        print("button 2");
    }
    DJV_CALLBACK(Application, exit_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }

    djv::Window * _window;
};

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_push_button_test", argc, argv),
    _window(0)
{
    is_valid(true);

    Push_Button * widget1 = new Push_Button("Button");
    Push_Button * widget2 = new Push_Button("Button 2");
    _window = new djv::Window(name());

    Vertical_Layout * layout = new Vertical_Layout(_window);
    layout->add(widget1);
    layout->add(widget2);

    _window->size(_window->size_hint());

    _window->show();

    widget1->signal.set(this, callback1);
    widget2->signal.set(this, callback2);
    _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
    if (_window)
    {
        _window->del();
    }
}

} // djv_push_button_test

int main(int argc, char ** argv)
{
    return djv_push_button_test::Application(argc, argv).exec();
}

