/*------------------------------------------------------------------------------
 lib/djv_image/wgl_private.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_WGL_PRIVATE_H
#define DJV_IMAGE_WGL_PRIVATE_H

#include <djv_image/gl.h>

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// _WGL_Device
//------------------------------------------------------------------------------

class _WGL_Device : public Device
{
public:

  _WGL_Device();

  ~_WGL_Device();

  void init() throw (Error);

  HDC device() const { return _device; }
  
private:

  HWND _id;
  HDC _device;
  HGLRC _context;
};

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

static const String
  error_glew = "GLEW",
  error_glew_init = "Camnnot initialize (#%%)",
  error_wgl = "WGL",
  error_wgl_context_create = "Cannot create context (#%%)",
  error_wgl_context_bind = "Cannot bind context (#%%)",
  error_wgl_pixel_choose = "Cannot get pixel format (#%%)",
  error_win32 = "Win32",
  error_win32_module = "GetModuleHandle (#%%)",
  error_win32_register = "RegisterClass (#%%)",
  error_win32_window = "CreateWindow (#%%)",
  error_win32_device = "GetDC (#%%)",
  error_win32_pixel_choose = "ChoosePixelFormat (#%%)",
  error_win32_pixel_set = "SetPixelFormat (#%%)";

//------------------------------------------------------------------------------

}
}

#endif
