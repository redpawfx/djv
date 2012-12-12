/*------------------------------------------------------------------------------
 lib/djv_image/gl_offscreen_fbo.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "gl_offscreen_private.h"

#include <djv_image/gl_data.h>

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// error()
//------------------------------------------------------------------------------

namespace {

String error(GLenum in)
{
  switch (in)
  {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      return "GL_FRAMEBUFFER_UNSUPPORTED_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
  }
  return "";
}

}

//------------------------------------------------------------------------------
// _FBO
//------------------------------------------------------------------------------

_FBO::_FBO(const Info & in) :
  _info(in),
  _id(0),
  _restore(0),
  _texture(0)
{
  if (! GLEW_EXT_framebuffer_object)
    return;
  
  DJV_DEBUG("_FBO::_FBO");
  DJV_DEBUG_PRINT("in = " << in);

  DJV_DEBUG_GL(glGenTextures(1, &_texture));

  const GLenum target = gl::texture();
  DJV_DEBUG_GL(glBindTexture(target, _texture));
  if (! _texture)
    return;
  
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

  // XXX
  
  GLenum format = GL_RGBA;
  if (GLEW_NV_float_buffer)
    format = GL_FLOAT_RGBA_NV;
  else if(GLEW_ARB_texture_float)
    format = GL_RGBA32F_ARB;
  
  DJV_DEBUG_GL(glTexImage2D(
    target, 0, format,
    _info.size.x, _info.size.y, 0,
    gl::format(_info.pixel, _info.bgr),
    gl::type(_info.pixel),
    0));
  
  DJV_DEBUG_GL(glGenFramebuffersEXT(1, &_id));
  if (! _id)
    return;
  
  DJV_DEBUG_GL(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _id));
  DJV_DEBUG_GL(glFramebufferTexture2DEXT(
    GL_FRAMEBUFFER_EXT,
    GL_COLOR_ATTACHMENT0_EXT,
    target, _texture, 0));
  DJV_DEBUG_PRINT("error = " <<
    error(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)));
}

_FBO::~_FBO()
{
  if (! GLEW_EXT_framebuffer_object)
    return;
  
  DJV_DEBUG("_FBO::~_FBO");
  
  if (_id)
  {
    DJV_DEBUG_GL(glDeleteFramebuffersEXT(1, &_id));
  }
  if (_texture)
  {
    DJV_DEBUG_GL(glDeleteTextures(1, &_texture));
  }
}

namespace {

GLuint _current = 0; // XXX

}

void _FBO::bind()
{
  if (! GLEW_EXT_framebuffer_object || ! _id)
    return;
  
  DJV_DEBUG("_FBO::bind");
  DJV_DEBUG_PRINT("current = " << static_cast<int>(_current));
  
  DJV_DEBUG_GL(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _id));
  _restore = _current;
  _current = _id;
}

void _FBO::unbind()
{
  if (! GLEW_EXT_framebuffer_object || ! _id)
    return;
  
  DJV_DEBUG("_FBO::unbind");
  
  DJV_DEBUG_GL(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _restore));
  _current = _restore;
  DJV_DEBUG_PRINT("current = " << static_cast<int>(_current));
}

void _FBO::texture_bind()
{
  if (! _texture)
    return;
  
  DJV_DEBUG("_FBO::texture_bind");

  DJV_DEBUG_GL(glBindTexture(gl::texture(), _texture));
}

//------------------------------------------------------------------------------
// _FBO_Factory
//------------------------------------------------------------------------------

_FBO_Factory::_FBO_Factory()
{
  DJV_DEBUG("_FBO_Factory::_FBO_Factory");
}

_FBO_Factory::~_FBO_Factory()
{
  DJV_DEBUG("_FBO_Factory::~_FBO_Factory");
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    delete _list[i];
}

void _FBO_Factory::init() throw (Error)
{
  DJV_DEBUG("_FBO_Factory::init");
  DJV_DEBUG_PRINT("valid = " << GLEW_EXT_framebuffer_object);
}

void _FBO_Factory::add(_FBO * in)
{
  DJV_DEBUG("_FBO_Factory::add");
  
  if (0)
  {
    delete in;
  }
  else
  {
    _list += in;
    if (_list.size() > _list_max)
      delete _list.pop_front();
  }
}

_FBO * _FBO_Factory::get(const Info & in) throw (Error)
{
  DJV_DEBUG("_FBO_Factory::get");
  DJV_DEBUG_PRINT("in = " << in);
  
  _FBO * out = 0;

  for (List<_FBO *>::iterator i = _list.begin(); i != _list.end(); ++i)
    if (in == (*i)->info())
    {
      DJV_DEBUG_PRINT("cached = " << (*i)->info().size);
      out = *i;
      _list.erase(i);
      break;
    }
  
  if (! out)
    out = new _FBO(in);

  return out;
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::_FBO::_FBO(_FBO_Factory * factory) :
  _factory(factory),
  _p(0)
{
  DJV_DEBUG("Global::_FBO::_FBO");
}

Global::_FBO::~_FBO()
{
  DJV_DEBUG("Global::_FBO::~_FBO");
  
  if (_p)
    _factory->add(_p);
}

void Global::_FBO::init(const Info & in) throw (Error)
{
  DJV_DEBUG("Global::_FBO::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  if (_p)
    if (in != _p->info())
    {
      _factory->add(_p);
      _p = 0;
    }

  if (! _p)
    _p = _factory->get(in);
}

void Global::_FBO::bind()
{
  DJV_DEBUG("Global::_FBO::bind");
  
  _p->bind();
}

void Global::_FBO::texture_bind()
{
  DJV_DEBUG("Global::_FBO::texture_bind");
  
  _p->texture_bind();
}

void Global::_FBO::unbind()
{
  DJV_DEBUG("Global::_FBO::unbind");
  
  _p->unbind();
}

const Info & Global::_FBO::info() const
{
  return _p->info();
}

//------------------------------------------------------------------------------

}
}

