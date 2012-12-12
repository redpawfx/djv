/*------------------------------------------------------------------------------
 lib/djv_image/gl_offscreen_private.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GL_OFFSCREEN_PRIVATE_H
#define DJV_IMAGE_GL_OFFSCREEN_PRIVATE_H

#include <djv_image/gl_offscreen.h>

#include <djv_image/data.h>

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// _Offscreen
//------------------------------------------------------------------------------

class _Offscreen
{
public:

  virtual ~_Offscreen() = 0;

  virtual void init() throw (Error) = 0;
};

//------------------------------------------------------------------------------
// _Pbuffer_Factory
//------------------------------------------------------------------------------

class _Pbuffer;

class _Pbuffer_Factory
{
public:

  virtual ~_Pbuffer_Factory() = 0;
  
  virtual void init() throw (Error) = 0;
  virtual void add(_Pbuffer *) = 0;
  
  virtual _Pbuffer * get(const Info &) throw (Error) = 0;
};

//------------------------------------------------------------------------------
// Global::_Pbuffer
//------------------------------------------------------------------------------

class Global::_Pbuffer : public Buffer
{
public:

  _Pbuffer(_Pbuffer_Factory *);
  
  ~_Pbuffer();

  void init(const Info &) throw (Error);
  void bind();
  void unbind();
  
  const Info & info() const;
  
private:

  _Pbuffer(const _Pbuffer &);
  
  _Pbuffer & operator = (const _Pbuffer &);

  _Pbuffer_Factory * _factory;

  gl_offscreen::_Pbuffer * _p;
  
  class Context_Restore;
  std::auto_ptr<Context_Restore> _context_restore;
};

//------------------------------------------------------------------------------
// _FBO
//
// Frame buffer object.
//------------------------------------------------------------------------------

class _FBO
{
public:

  _FBO(const Info &);
  
  ~_FBO();
  
  void bind();
  void unbind();
  void texture_bind();
  
  const Info & info() const { return _info; }
  
private:

  Info _info;
  GLuint _id, _restore;
  GLuint _texture;
};

//------------------------------------------------------------------------------
// _FBO_Factory
//------------------------------------------------------------------------------

class _FBO_Factory
{
public:

  _FBO_Factory();
  
  ~_FBO_Factory();
  
  void init() throw (Error);
  void add(_FBO *);
  
  _FBO * get(const Info &) throw (Error);

private:

  List<_FBO *> _list;
  static const list::size_type _list_max = 10;
};

//------------------------------------------------------------------------------
// Global::_FBO
//------------------------------------------------------------------------------

class Global::_FBO : public Buffer
{
public:

  _FBO(_FBO_Factory *);
  
  ~_FBO();

  void init(const Info &) throw (Error);
  void bind();
  void unbind();
  void texture_bind();
  
  const Info & info() const;

private:

  _FBO(const _FBO &);
  
  _FBO & operator = (const _FBO &);

  _FBO_Factory * _factory;
  gl_offscreen::_FBO * _p;
};

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

class Global::_Global
{
public:

  _Global() throw (Error);
  
  ~_Global();

  _Offscreen * offscreen() const { return _offscreen.get(); }
  _Pbuffer_Factory * pbuffer() const { return _pbuffer.get(); }
  _FBO_Factory * fbo() const { return _fbo.get(); }
  
private:

  std::auto_ptr<_Offscreen> _offscreen;
  std::auto_ptr<_Pbuffer_Factory> _pbuffer;
  std::auto_ptr<_FBO_Factory> _fbo;
};

//------------------------------------------------------------------------------

}
}

#endif
