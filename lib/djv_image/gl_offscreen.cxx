/*------------------------------------------------------------------------------
 lib/djv_image/gl_offscreen.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "gl_offscreen_private.h"

#include <djv_image/gl.h>

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// _Offscreen
//------------------------------------------------------------------------------

_Offscreen::~_Offscreen()
{}

//------------------------------------------------------------------------------
// _Pbuffer_Factory
//------------------------------------------------------------------------------

_Pbuffer_Factory::~_Pbuffer_Factory()
{}

//------------------------------------------------------------------------------
// Buffer
//------------------------------------------------------------------------------

Buffer::~Buffer()
{}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() throw (Error) :
  _buffer(PBUFFER)
{
  DJV_DEBUG("Global::Global");
  DJV_DEBUG_PRINT("buffer = " << _buffer);
  
  if (gl::OPENGL_1_2 == gl::global()->pipeline())
    _buffer = PBUFFER;
  else if (GLEW_EXT_framebuffer_object)
    _buffer = FBO;
  
  _global = std::auto_ptr<_Global>(new _Global);
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");
}

void Global::buffer(BUFFER in)
{
  DJV_DEBUG("Global::buffer");
  DJV_DEBUG_PRINT("in = " << in);

  _buffer = in;
}

BUFFER Global::buffer() const
{
  return _buffer;
}

Buffer * Global::buffer_create() const
{
  DJV_DEBUG("Global::buffer_create");
  
  const BUFFER buffer = this->buffer();
  DJV_DEBUG_PRINT("buffer = " << buffer);
  
  switch (buffer)
  {
    case PBUFFER: return new _Pbuffer(_global->pbuffer());
    case FBO: return new _FBO(_global->fbo());
  }
  
  return 0;
}

//------------------------------------------------------------------------------

}
}

