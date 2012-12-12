/*------------------------------------------------------------------------------
 bin/djv_view/main.cxx

 Copyright (c) 2004-2012 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "application.h"

#if defined(DJV_WINDOWS)
#include <windows.h>
#include <tchar.h>
#endif // DJV_WINDOWS

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    int r = 1;

    try
    {
        r = djv_bin::view::application::Application(argc, argv).exec();
    }
    catch (djv_base::Error in)
    {
        djv_base::error::print(in);
    }

    return r;
}

//------------------------------------------------------------------------------
// WinMain()
//------------------------------------------------------------------------------

#if defined(DJV_WINDOWS)

int WINAPI WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR,
    int
)
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

//------------------------------------------------------------------------------

