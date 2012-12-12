/*------------------------------------------------------------------------------
 include/djv_image/gl_offscreen.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GL_OFFSCREEN_H
#define DJV_IMAGE_GL_OFFSCREEN_H

#include <djv_image/gl_offscreen_def.h>

#include <djv_image/base.h>
#include <djv_image/data_def.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::gl_offscreen
//
//! OpenGL offscreen rendering.
//------------------------------------------------------------------------------

namespace gl_offscreen {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! OpenGL offscreen rendering.

enum BUFFER
{
  PBUFFER,  //!< Pixel buffer
  FBO,      //!< Frame buffer object
  
  _BUFFER_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Buffer
//
//! OpenGL offscreen rendering buffer.
//------------------------------------------------------------------------------

class _DJV_IMAGE Buffer
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Buffer() {}
  
  virtual ~Buffer() = 0;

  //@}
  //----------------------------------------------------------------------------
  //\name Functions
  //@{
  
  virtual void init(const Info &) throw (Error) = 0;
  virtual void bind() = 0;
  virtual void unbind() = 0;
  virtual void texture_bind() {}
  
  virtual const Info & info() const = 0;
  
  //@}
  
private:

  Buffer(const Buffer &);
  
  Buffer & operator = (const Buffer &);
};

//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, BUFFER &) throw (String);

_DJV_IMAGE String & operator << (String &, BUFFER);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_buffer();

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, BUFFER);

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class _DJV_IMAGE Global
{
public:
  
  Global() throw (Error);
  
  ~Global();

  void buffer(BUFFER);
  
  BUFFER buffer() const;
  
  Buffer * buffer_create() const;

private:

  BUFFER _buffer;
  
  class _Pbuffer;
  class _FBO;
  class _Global;
  std::auto_ptr<_Global> _global;
};

//! Access to global options.

_DJV_IMAGE Global * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

