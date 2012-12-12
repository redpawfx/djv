/*------------------------------------------------------------------------------
 lib/djv_base/system.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "system.h"

#include <djv_base/application.h>
#include <djv_base/file.h>
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

namespace djv_base {
namespace system {

//------------------------------------------------------------------------------
// info()
//------------------------------------------------------------------------------

#if defined(DJV_WINDOWS)

namespace {

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
  DJV_DEBUG("windows_version");

  WINDOWS out = WINDOWS_UNKNOWN;

  // Get OS version information.

  ::OSVERSIONINFOEX info;
  memory::zero(&info, sizeof(::OSVERSIONINFOEX));
  info.dwOSVersionInfoSize = sizeof(::OSVERSIONINFOEX);

  if (! ::GetVersionEx((::OSVERSIONINFO *)&info))
    return out;

  DJV_DEBUG_PRINT("version major = " <<
    static_cast<int>(info.dwMajorVersion));
  DJV_DEBUG_PRINT("version minor = " <<
    static_cast<int>(info.dwMinorVersion));
  DJV_DEBUG_PRINT("NT Workstation = " <<
    (info.wProductType == VER_NT_WORKSTATION ? 1 : 0));

  // Get system informaion.

  ::SYSTEM_INFO system_info;
  memory::zero(&system_info, sizeof(::SYSTEM_INFO));

  if (
    PGNSI pGNSI = (PGNSI)::GetProcAddress(
      ::GetModuleHandle(TEXT("kernel32.dll")),
      "GetNativeSystemInfo"
    )
  ) {
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
        case 0: out = WINDOWS_2000; break;
        case 1: out = WINDOWS_XP; break;

        case 2:

          if (
            VER_NT_WORKSTATION == info.wProductType &&
            PROCESSOR_ARCHITECTURE_AMD64 == system_info.wProcessorArchitecture
          ) {
            out = WINDOWS_XP_PRO_64;
            break;
          }

          if (GetSystemMetrics(SM_SERVERR2) == 0)
            out = WINDOWS_SERVER_2003;
          else
            out = WINDOWS_SERVER_2003_R2;

          break;
      }

      break;

    case 6:

      switch (info.dwMinorVersion)
      {
        case 0:
          if (info.wProductType == VER_NT_WORKSTATION)
            out = WINDOWS_VISTA;
          else
            out = WINDOWS_SERVER_2008;
          break;
      }

      break;
  }

  DJV_DEBUG_PRINT("out = " << out);

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

}

#endif // DJV_WINDOWS

String info()
{
  String out;
  
#if defined(DJV_WINDOWS)

  out = label_windows()[windows_version()];

#else // DJV_WINDOWS

  ::utsname info;
  uname(&info);
  
  out = string::Format(label_info).
    arg(info.sysname).
    arg(info.release).
    arg(info.machine);

#endif // DJV_WINDOWS

  return out;
}

//------------------------------------------------------------------------------
// search_path()
//------------------------------------------------------------------------------

List<String> search_path()
{
  DJV_DEBUG("search_path");
  
  List<String> out;

  // Environment variables.

  String path = system::env("DJV_PATH");
  
  if (! path.empty())
  {
    DJV_DEBUG_PRINT("DJV_PATH = " << path);
    out += string::split(path, file::list_separator_list);
  }

  path = system::env("LD_LIBRARY_PATH");
  
  if (! path.empty())
  {
    DJV_DEBUG_PRINT("LD_LIBRARY_PATH = " << path);
    
    out += string::split(path, file::list_separator_list);
  }

  // Application path.
  
  const String application_path = application::global()->path();
  
  DJV_DEBUG_PRINT("application path = " << application_path);
  
  out += application_path;
  
  // XXX Relative library path:
  //
  //   bin/<application>
  //   lib/

  out += file::path_fix(application_path + "../lib");

#if defined(DJV_WINDOWS)
  out += file::path_fix(application_path + "../../lib/Debug");
  out += file::path_fix(application_path + "../../lib/Release");
#endif

  // Current directory.

  out += ".";

  DJV_DEBUG_PRINT("out = " << out);
  
  return list::unique(out);
}

//------------------------------------------------------------------------------
// terminal_width()
//------------------------------------------------------------------------------

int terminal_width()
{
  int out = 80;

#if defined(DJV_WINDOWS)

  HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
  if (h != INVALID_HANDLE_VALUE)
  {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (::GetConsoleScreenBufferInfo(h, &info))
      out = info.dwSize.X;
  }

#else // DJV_WINDOWS

  struct winsize ws;
  ws.ws_col = 0;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    ws.ws_col = 80;

  out = ws.ws_col;

#endif // DJV_WINDOWS

  return out;
}

//------------------------------------------------------------------------------
// print()
//------------------------------------------------------------------------------

void print(const String & in, bool newline)
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

//------------------------------------------------------------------------------
// exec()
//------------------------------------------------------------------------------

int exec(const String & in)
{
  return ::system(in.c_str());
}

//------------------------------------------------------------------------------
// open_url()
//------------------------------------------------------------------------------

bool open_url(const String & in)
{
  DJV_DEBUG("open_url");
  DJV_DEBUG_PRINT("in = " << in);

  bool out = false;

#if defined(DJV_WINDOWS)

  int err = reinterpret_cast<int>(
    ::ShellExecute(0, "open", in.c_str(), 0, 0, SW_SHOWNORMAL)
  );
  
  DJV_DEBUG_PRINT("err = " << err);

  out = err > 32;

#elif defined(DJV_MACOSX)
  
  out = exec(string::Format("%% %%").arg("/usr/bin/open").arg(in)) != -1;

#else // DJV_WINDOWS

  out = exec(string::Format("%% %% &").arg("firefox").arg(in)) != -1;

#endif // DJV_WINDOWS

  DJV_DEBUG_PRINT("out = " << out);

  return out;
}

//------------------------------------------------------------------------------
// env()
//------------------------------------------------------------------------------

bool env(const String & in, const String & value)
{
#if defined(DJV_WINDOWS)

  return ::_putenv_s(in.c_str(), value.c_str()) == 0;

#else // DJV_WINDOWS

  return ::setenv(in.c_str(), value.c_str(), 1) == 0;

#endif // DJV_WINDOWS
}

String env(const String & in)
{
#if defined(DJV_WINDOWS)

  // XXX Error checking?

  char tmp [string::cstring_size] = "";
  size_t size = 0;
  ::getenv_s(&size, tmp, string::cstring_size, in.c_str());
  return tmp;

#else // DJV_WINDOWS

  const char * p = ::getenv(in.c_str());
  return String(p ? p : "");

#endif // DJV_WINDOWS
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_info = "%% %% %%";

//------------------------------------------------------------------------------

}
}

