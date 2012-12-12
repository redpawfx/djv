/*------------------------------------------------------------------------------
 plugin/djv_vlut/base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VLUT_BASE_H
#define DJV_VLUT_BASE_H

#include <djv_image/base.h>
#if defined(DJV_WINDOWS)
#include <windows.h>
#elif defined(DJV_IRIX)
#include <X11/Xlib.h>
#include <X11/extensions/XSGIvc.h>
#else
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#endif

namespace djv_plugin {

//------------------------------------------------------------------------------
//!\namespace djv_plugin::vlut
//
//! VLUT plugin - Video lookup table I/O.
//!
//! Video lookup tables control how colors are shown on a display device.
//!
//! Requires:
//!
//! - Linux - X11 Video Mode Extension, http://www.x.org
//! - IRIX - X11 Video Control Extension, http://www.sgi.org/irix
//!
//! Support:
//!
//! - Image data: RGB, 8-bit, 16-bit (platform-dependent)
//! - Not supported on Mac OS or Solaris SPARC
//! - Not all lookup tables work on Windows (reffered to as "extreme" lookup
//!   tables).
//------------------------------------------------------------------------------

namespace vlut {
using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "VLUT";

static const List<String> extension_list = List<String>() <<
  ".vlut";

//@}
//------------------------------------------------------------------------------
//!\name Video LUT Handle
//------------------------------------------------------------------------------
//@{

#if defined(DJV_WINDOWS)

struct Handle
{
  Handle();
  
  ~Handle();

  void open() throw (Error);
  void close();
  
  HDC operator () () const;

private:

  HDC _display;
};

#endif // DJV_WINDOWS

//@}
//------------------------------------------------------------------------------

}
}

#endif

