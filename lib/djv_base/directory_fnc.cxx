/*------------------------------------------------------------------------------
 lib/djv_base/directory_fnc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "directory.h"

#include <djv_base/file.h>
#include <FL/filename.H>
#if defined(DJV_WINDOWS)
#include <shlobj.h>
#include <windows.h>
#endif // DJV_WINDOWS

namespace djv_base {
namespace directory {

//------------------------------------------------------------------------------
// home()
//------------------------------------------------------------------------------

String home()
{
  String out;
  
#if ! defined(DJV_WINDOWS)

  char tmp [string::cstring_size] = "";
  fl_filename_expand(tmp, string::cstring_size, "~");
  out = tmp;

#endif // DJV_WINDOWS

  return file::path_fix(out);
}

//------------------------------------------------------------------------------
// desktop()
//------------------------------------------------------------------------------

String desktop()
{
  String out;
  
#if defined(DJV_WINDOWS)

  // XXX TCHAR conversion?
  
  TCHAR tmp [MAX_PATH] = "";
  ::SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, tmp);
  out = tmp;

#else // DJV_WINDOWS

  // XXX

  out = home() + "/Desktop";

#endif // DJV_WINDOWS

  return file::path_fix(out);
}


//------------------------------------------------------------------------------
// drive_list()
//------------------------------------------------------------------------------

List<String> drive_list()
{
  DJV_DEBUG("drive_list");

  List<String> out;
  
#if defined(DJV_WINDOWS)

  // XXX TCHAR conversion?

  TCHAR path [MAX_PATH] = "";
  const DWORD size = ::GetLogicalDriveStrings(MAX_PATH - 1, path);
  for (DWORD i = 0, j = 0; i < size; ++i)
    if (! path[i])
    {
      out += file::path_fix(String(path + j, i - j));
      
      j = i + 1;
    }

#endif // DJV_WINDOWS

  DJV_DEBUG_PRINT("out = " << out);
  
  return out;
}

//------------------------------------------------------------------------------
// root()
//------------------------------------------------------------------------------

String root()
{
  String out;
  
#if ! defined(DJV_WINDOWS)

  out = String(file::path_separator, 1);

#endif // ! DJV_WINDOWS

  return file::path_fix(out);
}

//------------------------------------------------------------------------------

}
}

