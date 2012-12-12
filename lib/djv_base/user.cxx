/*------------------------------------------------------------------------------
 lib/djv_base/user.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "user.h"

#if defined(DJV_WINDOWS)
#include <windows.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

namespace djv_base {
namespace user {

//------------------------------------------------------------------------------
// current()
//------------------------------------------------------------------------------

String current()
{
  String out;

#if defined(DJV_WINDOWS)

  // XXX TCHAR conversion?
  
  TCHAR tmp [string::cstring_size] = { 0 };
  DWORD size = string::cstring_size;
  ::GetUserName(tmp, &size);
  out = String(tmp);

#else // DJV_WINDOWS

  out = uid_to_string(::getuid());

#endif // DJV_WINDOWS

  return out;
}

//------------------------------------------------------------------------------
// uid_to_string()
//------------------------------------------------------------------------------

String uid_to_string(uid_t in)
{
  String out;

#if ! defined(DJV_WINDOWS)

  struct passwd * buf = ::getpwuid(in);
  if (buf)
    out = buf->pw_name;

#endif // ! DJV_WINDOWS

  return out;
}

//------------------------------------------------------------------------------

}
}

