/*------------------------------------------------------------------------------
 lib/djv_image/cgl.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "cgl_private.h"

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// _Pixel_Format
//------------------------------------------------------------------------------

namespace {

class _Pixel_Format
{
public:

  _Pixel_Format() :
    _format(0)
  {
    DJV_DEBUG("_Pixel_Format::_Pixel_Format");
  }

  ~_Pixel_Format()
  {
    if (_format) CGLDestroyPixelFormat(_format);
  }

  void init() throw (Error)
  {
    DJV_DEBUG("_Pixel_Format::init");

    CGLPixelFormatAttribute attribs [] =
    {
      //kCGLPFAColorSize, (CGLPixelFormatAttribute)32,
      (CGLPixelFormatAttribute)0
    };

    GLint npix = 0;
    CGLError error = CGLChoosePixelFormat(attribs, &_format, &npix);
    
    if (error != kCGLNoError)
      throw Error(error_cgl, string::Format(error_cgl_pixel_format).arg(error));
  }

  CGLPixelFormatObj format() const { return _format; }

private:

  CGLPixelFormatObj _format;
};

}

//------------------------------------------------------------------------------
// _CGL_Device
//------------------------------------------------------------------------------

_CGL_Device::_CGL_Device() :
  _context(0)
{
  DJV_DEBUG("_CGL_Device::_CGL_Device");
}

_CGL_Device::~_CGL_Device()
{
  DJV_DEBUG("_CGL_Device::~_CGL_Device");

  if (_context)
  {
    DJV_DEBUG_PRINT("context");
    CGLDestroyContext(_context);
  }
}

void _CGL_Device::init() throw (Error)
{
  DJV_DEBUG("_CGL_Device::init");

  // XXX Create a dummy rendering context for glewInit.
  // According to the docs, glewInit can be called just once per-process?

  CGLError error = kCGLNoError;

  _Pixel_Format pixel_format;
  pixel_format.init();

  error = CGLCreateContext(pixel_format.format(), 0, &_context);
  
  if (error != kCGLNoError || ! _context)
    throw Error(error_cgl, string::Format(error_cgl_context_create).arg(error));

  error = CGLSetCurrentContext(_context);
  
  if (error != kCGLNoError)
    throw Error(error_cgl, string::Format(error_cgl_context_bind).arg(error));

  // Initialize GLEW.

  GLint gl_error = glewInit();
  
  if (gl_error != GLEW_OK)
    throw Error(error_glew, string::Format(error_glew_init).
      arg((char *)glewGetErrorString(gl_error)));

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
  _device(new _CGL_Device),
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

