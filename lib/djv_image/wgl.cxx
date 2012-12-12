/*------------------------------------------------------------------------------
 lib/djv_image/wgl.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "wgl_private.h"

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// _WGL_Device
//------------------------------------------------------------------------------

_WGL_Device::_WGL_Device() :
  _id(0),
  _device(0),
  _context(0)
{
  DJV_DEBUG("_WGL_Device::_WGL_Device");
}

_WGL_Device::~_WGL_Device()
{
  DJV_DEBUG("_WGL_Device::~_WGL_Device");
  
  if (_context)
  {
    DJV_DEBUG_PRINT("context");

    wglDeleteContext(_context);
  }
  
  if (_device)
  {
    DJV_DEBUG_PRINT("device");

    ReleaseDC(_id, _device);
  }
}

void _WGL_Device::init() throw (Error)
{
  DJV_DEBUG("_WGL_Device::init");

  // XXX Create a dummy window and rendering context for glewInit.
  // According to the docs, glewInit can be called just once per-process?
  
  HINSTANCE hinstance = GetModuleHandle(0);
  
  if (! hinstance)
    throw Error(error_win32, string::Format(error_win32_module).
      arg(int(GetLastError())));
  
  static const char name [] = "djv";
  WNDCLASS wc;
  if (! GetClassInfo(hinstance, name, &wc))
  {
    wc.style = CS_OWNDC;
    //wc.lpfnWndProc = (WNDPROC)MainWndProc;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinstance;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = name;
    
    if (! RegisterClass(&wc))
      throw Error(error_win32, string::Format(error_win32_register).
        arg(int(GetLastError())));
  }
  
  _id = CreateWindow(name, 0, 0, 0, 0, 0, 0, 0, 0, hinstance, 0);
  
  if (! _id)
    throw Error(error_win32, string::Format(error_win32_window).
      arg(int(GetLastError())));
  
  _device = GetDC(_id);
  
  if (! _device)
    throw Error(error_win32, string::Format(error_win32_device).
      arg(int(GetLastError())));
  
  PIXELFORMATDESCRIPTOR pixel_format_info;
  const int pixel_format_size = DescribePixelFormat(_device, 0, 0, 0);
  DJV_DEBUG_PRINT("pixel format size = " << pixel_format_size);
  for (int i = 1; i < pixel_format_size; ++i)
  {
    DescribePixelFormat(_device, i, sizeof(PIXELFORMATDESCRIPTOR),
      &pixel_format_info);
    
    DJV_DEBUG_PRINT("  id " << i << ": " <<
      ((PFD_SUPPORT_OPENGL & pixel_format_info.dwFlags) ? "ogl " : "") <<
      ((PFD_GENERIC_FORMAT & pixel_format_info.dwFlags) ? "" : "accel ") <<
      ((PFD_TYPE_RGBA == pixel_format_info.iPixelType) ? "rgba " : "") <<
      "depth = " << pixel_format_info.cColorBits << "/" <<
        pixel_format_info.cRedBits << "/" <<
        pixel_format_info.cGreenBits << "/" <<
        pixel_format_info.cBlueBits << "/" <<
        pixel_format_info.cAlphaBits << " ");
  }
  
  PIXELFORMATDESCRIPTOR pixel_format =
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    0,
    0,
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };
  int pixel_format_id = ChoosePixelFormat(_device, &pixel_format);

  DJV_DEBUG_PRINT("pixel format = " << pixel_format_id);
  
  if (! pixel_format_id)
    throw Error(error_win32, string::Format(error_win32_pixel_choose).
      arg(int(GetLastError())));
  
  if (! SetPixelFormat(_device, pixel_format_id, &pixel_format))
    throw Error(error_win32, string::Format(error_win32_pixel_set).
      arg(int(GetLastError())));
  
  // Create context.
  
  _context = wglCreateContext(_device);
  
  if (! _context)
    throw Error(error_wgl, string::Format(error_wgl_context_create).
      arg(int(GetLastError())));
  
  if (! wglMakeCurrent(_device, _context))
    throw Error(error_wgl, string::Format(error_wgl_context_bind).
      arg(int(GetLastError())));
  
  // Initialize GLEW.
  
  GLenum err = glewInit();
  
  if (err != GLEW_OK)
    throw Error(error_glew, string::Format(error_glew_init).
      arg((char *)glewGetErrorString(err)));

  _vendor = String((const char *)glGetString(GL_VENDOR));
  _renderer = String((const char *)glGetString(GL_RENDERER));
  _version = String((const char *)glGetString(GL_VERSION));
  
  DJV_DEBUG_PRINT("OpenGL vendor string = " << _vendor);
  DJV_DEBUG_PRINT("OpenGL renderer string = " << _renderer);
  DJV_DEBUG_PRINT("OpenGL version string = " << _version);
  DJV_DEBUG_PRINT("OpenGL extensions = " <<
    (const char *)glGetString(GL_EXTENSIONS));
  DJV_DEBUG_PRINT("glu version = " <<
    (const char *)gluGetString(GLU_VERSION));
  DJV_DEBUG_PRINT("glu extensions = " <<
    (const char *)gluGetString(GLU_EXTENSIONS));
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() throw (Error) :
  _device(new _WGL_Device),
  _pipeline(OPENGL_1_2)
{
  DJV_DEBUG("Global::Global");
  
  _device->init();
  
  if (opengl_2_0())
    _pipeline = OPENGL_2_0;

  DJV_DEBUG_PRINT("pipeline = " << _pipeline);
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");
}

//------------------------------------------------------------------------------

}
}

