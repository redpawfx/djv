/*------------------------------------------------------------------------------
 lib/djv_image/glx.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "glx_private.h"

#include <djv_base/math.h>
#include <FL/x.H>

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// _GLX_Device
//------------------------------------------------------------------------------

_GLX_Device::_GLX_Device() :
  _window(0),
  _context(0)
{
  DJV_DEBUG("_GLX_Device::_GLX_Device");
}

_GLX_Device::~_GLX_Device()
{
  DJV_DEBUG("_GLX_Device::~_GLX_Device");

  if (_context)
  {
    DJV_DEBUG_PRINT("context");
    
    glXDestroyContext(fl_display, _context);
  }
  
  if (_window)
  {
    DJV_DEBUG_PRINT("window");
    
    XDestroyWindow(fl_display, _window);
  }
}

void _GLX_Device::init() throw (Error)
{
  DJV_DEBUG("_GLX_Device::init");
  
  // Open X display.
  
  fl_open_display();
  
  // Check for GLX support.
  
  if (! glXQueryExtension(fl_display, 0, 0))
    throw Error(error_glx, error_glx_init);

  // XXX Create a dummy window and rendering context for glewInit.
  // According to the docs, glewInit can be called just once per-process?
  
  XSetWindowAttributes win_attrib;
  win_attrib.colormap = fl_colormap;
  win_attrib.border_pixel = 0;
  
  _window = XCreateWindow(
    fl_display,
    RootWindow(fl_display, fl_screen),
    0, 0, 1, 1, 0,
    fl_visual->depth,
    InputOutput,
    fl_visual->visual,
    CWColormap | CWBorderPixel,
    &win_attrib
  );
  
  if (! _window)
    throw Error(error_x11, error_x11_window);

  // Create context.
  
  DJV_DEBUG_PRINT("x visual id = " << fl_visual->visualid);
  DJV_DEBUG_PRINT("x visual screen = " << fl_visual->screen);
  DJV_DEBUG_PRINT("x visual depth = " << fl_visual->depth);
  //DJV_DEBUG_PRINT("x visual class = " << fl_visual->class);
  DJV_DEBUG_PRINT("x visual red mask = " << fl_visual->red_mask);
  DJV_DEBUG_PRINT("x visual green mask = " << fl_visual->green_mask);
  DJV_DEBUG_PRINT("x visual blue mask = " << fl_visual->blue_mask);
  DJV_DEBUG_PRINT("x visual colormap size = " << fl_visual->colormap_size);
  DJV_DEBUG_PRINT("x visual bits per rgb = " << fl_visual->bits_per_rgb);
  DJV_DEBUG_PRINT("");
  
  _context = glXCreateContext(fl_display, fl_visual, 0, True);
  
  if (! _context)
    throw Error(error_glx, error_glx_context_create);
  
  if (! glXMakeCurrent(fl_display, _window, _context))
    throw Error(error_glx, error_glx_context_bind);
  
  // Initialize GLEW.
  
  GLint error = glewInit();
  
  if (error != GLEW_OK)
    throw Error(error_glew, string::Format(error_glew_init).
      arg((char *)glewGetErrorString(error)));

  _vendor = String((const char *)glGetString(GL_VENDOR));
  _renderer = String((const char *)glGetString(GL_RENDERER));
  _version = String((const char *)glGetString(GL_VERSION));
  
  DJV_DEBUG_PRINT("OpenGL vendor = " << _vendor);
  DJV_DEBUG_PRINT("OpenGL renderer = " << _renderer);
  DJV_DEBUG_PRINT("OpenGL version = " << _version);
  DJV_DEBUG_PRINT("");
  
  DJV_DEBUG_PRINT(
    "server glx vendor string = " <<
    glXQueryServerString(fl_display, fl_screen, GLX_VENDOR));
  DJV_DEBUG_PRINT(
    "server glx version string = " <<
    glXQueryServerString(fl_display, fl_screen, GLX_VERSION));
  DJV_DEBUG_PRINT(
    "server glx extensions = " <<
    glXQueryServerString(fl_display, fl_screen, GLX_EXTENSIONS));
  DJV_DEBUG_PRINT(
   "client glx vendor string = " <<
    glXGetClientString(fl_display, GLX_VENDOR));
  DJV_DEBUG_PRINT(
    "client glx version string = " <<
    glXGetClientString(fl_display, GLX_VERSION));
  DJV_DEBUG_PRINT(
    "client glx extensions = " <<
    glXGetClientString(fl_display, GLX_EXTENSIONS));
  DJV_DEBUG_PRINT(
    "GLX extensions = " <<
    glXQueryExtensionsString(fl_display, fl_screen));
  DJV_DEBUG_PRINT(
    "OpenGL extensions = " <<
    (const char *)glGetString(GL_EXTENSIONS));
  DJV_DEBUG_PRINT(
    "glu version = " <<
    (const char *)gluGetString(GLU_VERSION));
  DJV_DEBUG_PRINT(
    "glu extensions = " <<
    (const char *)gluGetString(GLU_EXTENSIONS));
  DJV_DEBUG_PRINT("");
  
  GLint red_bits = 0;
  GLint green_bits = 0;
  GLint blue_bits = 0;
  GLboolean double_buffer = false;
  GLint max_texture_size = 0;
  glGetIntegerv(GL_RED_BITS, &red_bits);
  glGetIntegerv(GL_GREEN_BITS, &green_bits);
  glGetIntegerv(GL_BLUE_BITS, &blue_bits);
  glGetBooleanv(GL_DOUBLEBUFFER, &double_buffer);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
  
  DJV_DEBUG_PRINT(
    "direct rendering = " <<
    glXIsDirect(fl_display, _context));
  DJV_DEBUG_PRINT("red bits = " << red_bits);
  DJV_DEBUG_PRINT("green bits = " << green_bits);
  DJV_DEBUG_PRINT("blue bits = " << blue_bits);
  DJV_DEBUG_PRINT("double buffer = " << double_buffer);
  DJV_DEBUG_PRINT("maximum texture size = " << max_texture_size);
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() throw (Error) :
  _device(new _GLX_Device),
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

