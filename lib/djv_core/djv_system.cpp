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

//! \file djv_system.cpp

#include <djv_system.h>

#include <djv_core_application.h>
#include <djv_file.h>

#if defined(DJV_WINDOWS)
#include <windows.h>
#include <shellapi.h>
#else // DJV_WINDOWS
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <sys/utsname.h>
#include <unistd.h>
#endif // DJV_WINDOWS
#include <stdlib.h>
#include <stdio.h>

namespace djv
{

//------------------------------------------------------------------------------
// System
//------------------------------------------------------------------------------

#if defined(DJV_WINDOWS)

namespace
{

enum WINDOWS
{
    WINDOWS_UNKNOWN,
    WINDOWS_2000,
    WINDOWS_XP,
    WINDOWS_XP_PRO_64,
    WINDOWS_SERVER_2003,
    WINDOWS_SERVER_2003_R2,
    WINDOWS_VISTA,
    WINDOWS_SERVER_2008,

    _WINDOWS_SIZE
};

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

WINDOWS windows_version()
{
    //DJV_DEBUG("windows_version");

    WINDOWS out = WINDOWS_UNKNOWN;

    // Get OS version information.

    ::OSVERSIONINFOEX info;
    Memory::zero(&info, sizeof(::OSVERSIONINFOEX));
    info.dwOSVersionInfoSize = sizeof(::OSVERSIONINFOEX);

    if (! ::GetVersionEx((::OSVERSIONINFO *)&info))
    {
        return out;
    }

    //DJV_DEBUG_PRINT("version major = " <<
    static_cast<int>(info.dwMajorVersion));
    //DJV_DEBUG_PRINT("version minor = " <<
    static_cast<int>(info.dwMinorVersion));
    //DJV_DEBUG_PRINT("NT Workstation = " <<
    (info.wProductType == VER_NT_WORKSTATION ? 1 : 0));

    // Get system informaion.

    ::SYSTEM_INFO system_info;
    Memory::zero(&system_info, sizeof(::SYSTEM_INFO));

    if (PGNSI pGNSI = (PGNSI)::GetProcAddress(
            ::GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo"))
    {
        pGNSI(&system_info);
    }
    else
    {
        ::GetSystemInfo(&system_info);
    }

    // Use OS version and system information to determnine the version name.

    switch (info.dwMajorVersion)
    {
        case 5:

            switch (info.dwMinorVersion)
            {
                case 0:
                    out = WINDOWS_2000;
                    break;

                case 1:
                    out = WINDOWS_XP;
                    break;

                case 2:

                    if (
                        VER_NT_WORKSTATION == info.wProductType &&
                        PROCESSOR_ARCHITECTURE_AMD64 ==
                            system_info.wProcessorArchitecture)
                    {
                        out = WINDOWS_XP_PRO_64;
                        break;
                    }

                    if (GetSystemMetrics(SM_SERVERR2) == 0)
                    {
                        out = WINDOWS_SERVER_2003;
                    }
                    else
                    {
                        out = WINDOWS_SERVER_2003_R2;
                    }

                    break;
            }

            break;

        case 6:

            switch (info.dwMinorVersion)
            {
                case 0:
                    if (info.wProductType == VER_NT_WORKSTATION)
                    {
                        out = WINDOWS_VISTA;
                    }
                    else
                    {
                        out = WINDOWS_SERVER_2008;
                    }

                    break;
            }

            break;
    }

    //DJV_DEBUG_PRINT("out = " << out);

    return out;
}

const List<String> & label_windows()
{
    static const List<String> data = List<String>() <<
        "Unknown" <<
        "Windows 2000" <<
        "Windows XP" <<
        "Windows XP Professional x64 Edition" <<
        "Windows Server 2003" <<
        "Windows Server 2003 R2" <<
        "Windows Vista" <<
        "Windows Server 2008";

    DJV_ASSERT(data.size() == _WINDOWS_SIZE);

    return data;
}

} // namespace

#endif // DJV_WINDOWS

String System::info()
{
    String out;

#if defined(DJV_WINDOWS)

    out = label_windows()[windows_version()];

#else // DJV_WINDOWS

    ::utsname info;
    uname(&info);

    out = String_Format(label_info).
          arg(info.sysname).
          arg(info.release).
          arg(info.machine);

#endif // DJV_WINDOWS

    return out;
}

List<String> System::search_path()
{
    //DJV_DEBUG("System::search_path");

    List<String> out;

    // Environment variables.

    String path = env("DJV_PATH");

    if (! path.empty())
    {
        //DJV_DEBUG_PRINT("DJV_PATH = " << path);
        out += String_Util::split(path, File_Util::list_separators);
    }

    path = env("LD_LIBRARY_PATH");

    if (! path.empty())
    {
        //DJV_DEBUG_PRINT("LD_LIBRARY_PATH = " << path);

        out += String_Util::split(path, File_Util::list_separators);
    }

    // Application path.

    const String application_path = Core_Application::global()->path();

    //DJV_DEBUG_PRINT("application path = " << application_path);

    out += application_path;

    //! \todo Relative library paths:
    //!
    //!   bin/<application>
    //!   lib/

    out += File_Util::path_fix(application_path + "../lib");

#if defined(DJV_WINDOWS) || defined(DJV_OSX)
    out += File_Util::path_fix(application_path + "../../lib/Debug");
    out += File_Util::path_fix(application_path + "../../lib/Release");
#endif

    // Current directory.

    out += ".";

    //DJV_DEBUG_PRINT("out = " << out);

    return List_Util::unique(out);
}

int System::terminal_width()
{
    int out = 80;

#if defined(DJV_WINDOWS)

    HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);

    if (h != INVALID_HANDLE_VALUE)
    {
        CONSOLE_SCREEN_BUFFER_INFO info;

        if (::GetConsoleScreenBufferInfo(h, &info))
        {
            out = info.dwSize.X;
        }
    }

#else // DJV_WINDOWS

    struct winsize ws;
    ws.ws_col = 0;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    {
        ws.ws_col = 80;
    }

    out = ws.ws_col;

#endif // DJV_WINDOWS

    return out;
}

void System::print(const String & in, bool newline)
{
    if (newline)
    {
        ::printf("%s\n", in.c_str());
    }
    else
    {
        ::printf("%s", in.c_str());
        ::fflush(stdout);
    }
}

int System::exec(const String & in)
{
    return ::system(in.c_str());
}

bool System::open_url(const String & in)
{
    //DJV_DEBUG("System::open_url");
    //DJV_DEBUG_PRINT("in = " << in);

    bool out = false;

#if defined(DJV_WINDOWS)

    int err = reinterpret_cast<int>(
        ::ShellExecute(0, "open", in.c_str(), 0, 0, SW_SHOWNORMAL));

    //DJV_DEBUG_PRINT("err = " << err);

    out = err > 32;

#elif defined(DJV_OSX)

    out = exec(String_Format("%% %%").arg("/usr/bin/open").arg(in)) != -1;

#else // DJV_WINDOWS

    out = exec(String_Format("%% %% &").arg("firefox").arg(in)) != -1;

#endif // DJV_WINDOWS

    //DJV_DEBUG_PRINT("out = " << out);

    return out;
}

bool System::env(const String & in, const String & value)
{
#if defined(DJV_WINDOWS)

    //! \todo MinGW does not define _putenv_s()?
    //return ::_putenv_s(in.c_str(), value.c_str()) == 0;
    char tmp [cstring_size] = "";
    SNPRINTF(tmp, cstring_size, "%s=%s", in.c_str(), value.c_str());
    return ::_putenv(tmp) == 0;

#else // DJV_WINDOWS

    return ::setenv(in.c_str(), value.c_str(), 1) == 0;

#endif // DJV_WINDOWS
}

String System::env(const String & in)
{
#if defined(DJV_WINDOWS)

    //! \todo Error checking?

    //! \todo MinGW does not define getenv_s()?
    //char tmp [cstring_size] = "";
    //size_t size = 0;
    //::getenv_s(&size, tmp, cstring_size, in.c_str());
    //return tmp;
    const char * p = ::getenv(in.c_str());
    return String(p ? p : "");

#else // DJV_WINDOWS

    const char * p = ::getenv(in.c_str());
    return String(p ? p : "");

#endif // DJV_WINDOWS
}

const String
System::label_info = "%% %% %%";

} // djv

