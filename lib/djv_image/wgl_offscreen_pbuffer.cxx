/*------------------------------------------------------------------------------
 lib/djv_image/wgl_offscreen_pbuffer.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "wgl_offscreen_private.h"

#include "wgl_private.h"
#include <FL/Fl.H>

//#undef WGLEW_ARB_pbuffer
//#define WGLEW_ARB_pbuffer 0

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// _Pbuffer
//------------------------------------------------------------------------------

_Pbuffer::_Pbuffer() :
  _device(0),
  _context(0),
  _pbuffer(0)
{
  DJV_DEBUG("_Pbuffer::_Pbuffer");
}

_Pbuffer::~_Pbuffer()
{
  DJV_DEBUG("_Pbuffer::~_Pbuffer");
  
  if (_context)
  {
    DJV_DEBUG_PRINT("context");
    wglDeleteContext(_context);
    _context = 0;
  }

  if (WGLEW_ARB_pbuffer && _device)
  {
    DJV_DEBUG_PRINT("device");
    wglReleasePbufferDCARB(_pbuffer, _device);
    _device = 0;
  }

  if (WGLEW_ARB_pbuffer && _pbuffer)
  {
    DJV_DEBUG_PRINT("pbuffer");
    wglDestroyPbufferARB(_pbuffer);
    _pbuffer = 0;
  }
}

void _Pbuffer::init(const Info & in, int format) throw (Error)
{
  DJV_DEBUG("_Pbuffer::init");
  DJV_DEBUG_PRINT("in = " << in);

  // Create pbuffer.
  
  int attrib [] =
  {
    WGL_PBUFFER_LARGEST_ARB, 1,
    0
  };
  if (WGLEW_ARB_pbuffer)
    _pbuffer = wglCreatePbufferARB(reinterpret_cast<gl::_WGL_Device *>(
      gl::global()->device())->device(), format, in.size.x, in.size.y,
      attrib);
  if (! _pbuffer)
    throw Error(gl::error_wgl, string::Format(error_wgl_pbuffer_create).
      arg(int(GetLastError())));
  
  if (WGLEW_ARB_pbuffer)
  {
    wglQueryPbufferARB(_pbuffer, WGL_PBUFFER_WIDTH_ARB, &_info.size.x);
    wglQueryPbufferARB(_pbuffer, WGL_PBUFFER_HEIGHT_ARB, &_info.size.y);
  }
  DJV_DEBUG_PRINT("size = " << _info.size);
  
  // Create context.
  
  if (WGLEW_ARB_pbuffer)
    _device = wglGetPbufferDCARB(_pbuffer);
  if (! _device)
    throw Error(gl::error_wgl, string::Format(error_wgl_pbuffer_device).
      arg(int(GetLastError())));
  
  _context = wglCreateContext(_device);
  if (! _context)
    throw Error(gl::error_wgl, string::Format(gl::error_wgl_context_create).
      arg(int(GetLastError())));
}
    
void _Pbuffer::current() throw (Error)
{
  DJV_DEBUG("_Pbuffer::current");
  
  if (! wglMakeCurrent(_device, _context))
    throw Error(gl::error_wgl, string::Format(gl::error_wgl_context_bind).
      arg(int(GetLastError())));
}

//------------------------------------------------------------------------------
// _WGL_Pbuffer_Factory
//------------------------------------------------------------------------------

_WGL_Pbuffer_Factory::_WGL_Pbuffer_Factory() :
  _format(-1)
{
  DJV_DEBUG("_WGL_Pbuffer_Factory::_WGL_Pbuffer_Factory");
}

_WGL_Pbuffer_Factory::~_WGL_Pbuffer_Factory()
{
  DJV_DEBUG("_WGL_Pbuffer_Factory::~_WGL_Pbuffer_Factory");
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    delete _list[i];
}

void _WGL_Pbuffer_Factory::init() throw (Error)
{
  DJV_DEBUG("_WGL_Pbuffer_Factory::init");
  
  // Pbuffer configuration.
  
  int fb_attrib [] =
  {
    WGL_SUPPORT_OPENGL_ARB, 1,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_DRAW_TO_PBUFFER_ARB, 1,
    WGL_DOUBLE_BUFFER_ARB, 0,
    WGL_RED_BITS_ARB, 1,
    WGL_GREEN_BITS_ARB, 1,
    WGL_BLUE_BITS_ARB, 1,
    WGL_ALPHA_BITS_ARB, 0,
    WGL_DEPTH_BITS_ARB, 0,
    WGL_STENCIL_BITS_ARB, 0,
    0
  };
  uint format_size = 0;
  if (WGLEW_ARB_pbuffer)
    wglChoosePixelFormatARB(reinterpret_cast<gl::_WGL_Device *>(
      gl::global()->device())->device(), fb_attrib, 0, 1, &_format,
      &format_size);
  DJV_DEBUG_PRINT("format = " << _format);
}

void _WGL_Pbuffer_Factory::add(_Pbuffer * in)
{
  DJV_DEBUG("_WGL_Pbuffer_Factory::add");

  _list += in;
  if (_list.size() > _list_max)
    delete _list.pop_front();
}

_Pbuffer * _WGL_Pbuffer_Factory::get(const Info & in) throw (Error)
{
  DJV_DEBUG("_WGL_Pbuffer_Factory::get");
  DJV_DEBUG_PRINT("in = " << in);
  
  _Pbuffer * out = 0;

  if (-1 == _format)
    throw Error(gl::error_wgl, string::Format(gl::error_wgl_pixel_choose).
      arg(int(GetLastError())));

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
    _context(wglGetCurrentContext()),
    _device(wglGetCurrentDC())
  {}
  
  ~Context_Restore()
  {
    wglMakeCurrent(_device, _context);
  }
  
private:

  HGLRC _context;
  HDC _device;
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

