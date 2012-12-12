/*------------------------------------------------------------------------------
 plugin/djv_vlut/etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "base.h"

#include <djv_image/io.h>

namespace djv_plugin {
namespace vlut {

//------------------------------------------------------------------------------
// Handle
//------------------------------------------------------------------------------

#if defined(DJV_WINDOWS)

Handle::Handle() :
  _display(0)
{}

Handle::~Handle()
{
  close();
}

void Handle::open() throw (Error)
{
  DJV_DEBUG("Handle::open");

  close();
  
  _display = ::GetDC(0);
  if (! _display)
    djv_image::io::throw_error_open(name, "GetDC(0)");
}

void Handle::close()
{
  if (_display)
  {
    ::ReleaseDC(0, _display);
    _display = 0;
  }
}

HDC Handle::operator () () const
{
  return _display;
}

#endif // DJV_WINDOWS

//------------------------------------------------------------------------------

}
}

