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

//! \file djv_file_browser_test.cpp

#include <djv_application.h>
#include <djv_file_browser.h>

namespace djv_file_browser_test
{
using namespace djv;

class Application : public djv::Application
{
public:

    Application(int & argc, char ** argv);

    virtual ~Application();

private:

    DJV_CALLBACK(Application, widget_callback, const File &)
    {
        print(in);
    }

    DJV_CALLBACK(Application, close_callback, bool)
    {
        exit(EXIT_DEFAULT);
    }
};

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_file_browser_test", argc, argv)
{
    is_valid(true);

    File_Browser::set(this, widget_callback);
    File_Browser::global()->close_signal.set(this, close_callback);
}

Application::~Application()
{
    File_Browser::del(this);
}

} // djv_file_browser_test

int main(int argc, char ** argv)
{
    return djv_file_browser_test::Application(argc, argv).exec();
}

