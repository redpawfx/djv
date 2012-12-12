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

//! \file djv_view_win_main.cpp

#include <djv_view_application.h>

#if defined(DJV_WINDOWS)
#include <windows.h>
#include <tchar.h>
#endif // DJV_WINDOWS

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    int r = 1;

    try
    {
        r = djv_view::Application(argc, argv).exec();
    }
    catch (djv::Error in)
    {
        djv::Error_Util::print(in);
    }

    return r;
}

//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------

#if defined(DJV_WINDOWS)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    //! \todo Verify that this properly creates argc/argv.

    TCHAR   cmdline[4096];
    TCHAR * argv   [4096];
    int     argc = 0;

    _tcscpy(cmdline, GetCommandLine());

    argv[argc] = _tcstok(cmdline, TEXT(" \t"));

    while (argv[argc] != 0)
    {
        argc++;
        argv[argc] = _tcstok(0, TEXT(" \t"));
    }

    return main(argc, argv);
}

#endif // DJV_WINDOWS
