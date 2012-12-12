/*------------------------------------------------------------------------------
 lib/djv_image/cgl_offscreen_pbuffer.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "cgl_offscreen_private.h"

#include "cgl_private.h"

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// _Pbuffer
//------------------------------------------------------------------------------

_Pbuffer::_Pbuffer() :
  _context(0),
  _pbuffer(0)
{
  DJV_DEBUG("_Pbuffer::_Pbuffer");
}

_Pbuffer::~_Pbuffer()
{
  DJV_DEBUG("_Pbuffer::~_Pbuffer");

  if (_pbuffer)
  {
    DJV_DEBUG_PRINT("pbuffer");
    CGLDestroyPBuffer(_pbuffer);
  }
  
  if (_context)
  {
    DJV_DEBUG_PRINT("context");
    CGLDestroyContext(_context);
  }
}

void _Pbuffer::init(const Info & in, const CGLPixelFormatObj format)
  throw (Error)
{
  DJV_DEBUG("_Pbuffer::init");

  // Create context.

  CGLError error = CGLCreateContext(format, 0, &_context);
  
  if (error != kCGLNoError || ! _context)
    throw Error(gl::error_cgl,
      string::Format(gl::error_cgl_context_create).arg(error));

  error = CGLSetCurrentContext(_context);
  
  if (error != kCGLNoError)
    throw Error(gl::error_cgl,
      string::Format(gl::error_cgl_context_bind).arg(error));

  // Create pbuffer.

  error = CGLCreatePBuffer(
    in.size.x,
    in.size.y,
    GL_TEXTURE_RECTANGLE_EXT,
    GL_RGBA,
    0,
    &_pbuffer
  );
  
  if (error != kCGLNoError || ! _pbuffer)
    throw Error(gl::error_cgl,
      string::Format(error_cgl_pbuffer_create).arg(error));

  error = CGLSetPBuffer(_context, _pbuffer, 0, 0, 0);
  
  if (error != kCGLNoError)
    throw Error(gl::error_cgl, string::Format(error_cgl_pbuffer_set).arg(error));

  GLsizei w = 0, h = 0;
  GLenum target = 0, internal_format = 0;
  GLint mipmap = 0;
  error = CGLDescribePBuffer(
    _pbuffer,
    &w,
    &h,
    &target,
    &internal_format,
    &mipmap
  );
  
  if (error != kCGLNoError)
    throw Error(gl::error_cgl,
      string::Format(error_cgl_pbuffer_describe).arg(error));

  _info.size = V2i(w, h);
  DJV_DEBUG_PRINT("size = " << _info.size);
}

void _Pbuffer::current() throw (Error)
{
  DJV_DEBUG("_Pbuffer::current");
  DJV_DEBUG_PRINT("context = " << static_cast<int>(_context));

  CGLError error = CGLSetCurrentContext(_context);
  
  if (error != kCGLNoError)
    throw Error(gl::error_cgl,
      string::Format(gl::error_cgl_context_bind).arg(error));
}

//------------------------------------------------------------------------------
// _CGL_Pbuffer_Factory
//------------------------------------------------------------------------------

_CGL_Pbuffer_Factory::_CGL_Pbuffer_Factory() :
  _format(0)
{
  DJV_DEBUG("_CGL_Pbuffer_Factory::_CGL_Pbuffer_Factory");
}

_CGL_Pbuffer_Factory::~_CGL_Pbuffer_Factory()
{
  DJV_DEBUG("_CGL_Pbuffer_Factory::~_CGL_Pbuffer_Factory");

  for (list::size_type i = 0; i < _list.size(); ++i)
    delete _list[i];

  if (_format)
  {
    DJV_DEBUG_PRINT("format");
    CGLDestroyPixelFormat(_format);
  }
}

void _CGL_Pbuffer_Factory::init() throw (Error)
{
  DJV_DEBUG("_CGL_Pbuffer_Factory::init");

  // Pixel format.
  
  CGLPixelFormatAttribute attribs [] =
  {
    kCGLPFAPBuffer, (CGLPixelFormatAttribute)true,
    kCGLPFAColorFloat, (CGLPixelFormatAttribute)true,
    kCGLPFAColorSize, (CGLPixelFormatAttribute)32,
    (CGLPixelFormatAttribute)0
  };
  GLint npix = 0;
  CGLChoosePixelFormat(attribs, &_format, &npix);
  
  if (! _format)
  {
    // Fallback pixel format.

    attribs[2] = (CGLPixelFormatAttribute)0;
    CGLChoosePixelFormat(attribs, &_format, &npix);
  }
}

void _CGL_Pbuffer_Factory::add(_Pbuffer * in)
{
  DJV_DEBUG("_CGL_Pbuffer_Factory::add");

  _list += in;
  if (_list.size() > _list_max)
    delete _list.pop_front();
}

_Pbuffer * _CGL_Pbuffer_Factory::get(const Info & in) throw (Error)
{
  DJV_DEBUG("_CGL_Pbuffer_Factory::get");
  DJV_DEBUG_PRINT("in = " << in);
  
  _Pbuffer * out = 0;

  if (! _format)
    throw Error(gl::error_cgl, string::Format(gl::error_cgl_pixel_format));

  for (List<_Pbuffer *>::iterator i = _list.begin(); i != _list.end(); ++i)
    if (in == (*i)->info())
    {
      DJV_DEBUG_PRINT("cached");
      out = *i;
      _list.erase(i);
      break;
    }
  
  if (! out)
  {
    out = new _Pbuffer;
    out->init(in, _format);
  }
  return out;
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

class Global::_Pbuffer::Context_Restore
{
public:

  Context_Restore() :
    _context(CGLGetCurrentContext())
  {}
  
  ~Context_Restore()
  {
    CGLSetCurrentContext(_context);
  }
  
private:

  CGLContextObj _context;
};

Global::_Pbuffer::_Pbuffer(_Pbuffer_Factory * factory) :
  _factory(factory),
  _p(0)
{
  DJV_DEBUG("Global::_Pbuffer::_Pbuffer");
}

Global::_Pbuffer::~_Pbuffer()
{
  DJV_DEBUG("Global::_Pbuffer::~_Pbuffer");
  
  if (_p)
    _factory->add(_p);
}

void Global::_Pbuffer::init(const Info & in) throw (Error)
{
  DJV_DEBUG("Global::_Pbuffer::init");
  DJV_DEBUG_PRINT("in = " << in);

  Context_Restore context_restore;
  
  if (_p)
    if (in != _p->info())
    {
      _factory->add(_p);
      _p = 0;
    }

  if (! _p)
    _p = _factory->get(in);
}

const Info & Global::_Pbuffer::info() const
{
  return _p->info();
}

void Global::_Pbuffer::bind()
{
  DJV_DEBUG("Global::_Pbuffer::bind");
  
  _context_restore = std::auto_ptr<Context_Restore>(new Context_Restore);
  
  _p->current();
}

void Global::_Pbuffer::unbind()
{
  DJV_DEBUG("Global::_Pbuffer::unbind");
  
  _context_restore.reset();
}

//------------------------------------------------------------------------------

}
}

