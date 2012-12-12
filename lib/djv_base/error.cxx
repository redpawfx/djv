/*------------------------------------------------------------------------------
 lib/djv_base/error.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "error.h"

#include <djv_base/string.h>

namespace djv_base {
namespace error {

//------------------------------------------------------------------------------
// Error
//------------------------------------------------------------------------------

Error::Error()
{}

Error::Error(const String & in) :
  _value(in)
{}

Error::Error(const String & prefix, const String & in) :
  _value(prefix + " - " + in)
{}

const String & Error::string() const
{
  return _value;
}
  
Error::operator String () const
{
  return _value;
}

//------------------------------------------------------------------------------
// format()
//------------------------------------------------------------------------------

String format(const Error & in)
{
  return string::Format(error_format).arg(in);
}

//------------------------------------------------------------------------------
// print()
//------------------------------------------------------------------------------

void print(const Error & in)
{
  ::printf("%s\n", format(in).c_str());
}

//------------------------------------------------------------------------------
// last_error()
//------------------------------------------------------------------------------

_DJV_BASE String last_error()
{
  String out;
  
#if defined(DJV_WINDOWS)

  struct Buffer
  {
    Buffer() : p(0) {}
    ~Buffer() { LocalFree(p); }
    LPTSTR * p;
  } buffer;
  
  ::FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    0,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&buffer.p,
    0,
    0
  );
  
  out = String((LPCTSTR)buffer.p);

#endif // DJV_WINDOWS

  return out;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  error_format = "[ERROR] %%";

//------------------------------------------------------------------------------

}
}

