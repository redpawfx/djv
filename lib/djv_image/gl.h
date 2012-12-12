/*------------------------------------------------------------------------------
 include/djv_image/gl.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GL_H
#define DJV_IMAGE_GL_H

#include <djv_image/gl_def.h>

#include <djv_image/color_def.h>
#include <djv_image/pixel.h>
#include <djv_base/box_def.h>
#include <djv_base/vector.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::gl
//
//! OpenGL.
//------------------------------------------------------------------------------

namespace gl {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! OpenGL rendering pipeline

enum PIPELINE
{
  OPENGL_1_2,
  OPENGL_2_0,
  
  _PIPELINE_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool opengl_2_0();

_DJV_IMAGE void ortho(
  const V2i & size,
  const V2f & min_max = V2f(-1.0, 1.0)
);

_DJV_IMAGE GLenum format(PIXEL, bool bgr = false);
_DJV_IMAGE GLenum type(PIXEL);
_DJV_IMAGE GLenum texture();

inline void color(const Color &);
  
inline void draw_box(const Box2i &);
inline void draw_box(const Box2f &);
inline void draw_box(const Box2i &, const V2f uv [4]);
inline void draw_box(const Box2f &, const V2f uv [4]);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, PIPELINE &) throw (String);

_DJV_IMAGE String & operator << (String &, PIPELINE);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_pipeline();

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//!
//! References:
//!
//! - Dave Shreiner, "Performance OpenGL: Platform Independent Techniques"
//!   SIGGRAPH 2001
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, PIPELINE);

#if defined(DJV_DEBUG)

#define DJV_DEBUG_GL(cmd) \
  \
  cmd; \
  \
  { \
    ::GLenum error = GL_NO_ERROR; \
    \
    if ((error = ::glGetError()) != GL_NO_ERROR) \
    { \
      DJV_DEBUG_PRINT(string::Format("%% = %% (%%, line %%)"). \
        arg(#cmd). \
        arg((char *)::gluErrorString(error)). \
        arg(__FILE__). \
        arg(__LINE__)); \
    } \
  }

#else

#define DJV_DEBUG_GL(cmd) \
  cmd;

#endif

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! OpenGL device.

class _DJV_IMAGE Device
{
public:

  Device();
  
  virtual ~Device() = 0;

  virtual void init() throw (Error) = 0;

  const String & vendor() const;
  const String & renderer() const;
  const String & version() const;

protected:

  String _vendor;
  String _renderer;
  String _version;
};

//! Global options.

class _DJV_IMAGE Global
{
public:
  
  Global() throw (Error);
  
  ~Global();
  
  Device * device() const;

  void pipeline(PIPELINE);
  
  PIPELINE pipeline() const;
  
private:

  std::auto_ptr<Device> _device;
  PIPELINE _pipeline;
};

//! Access to global options.

_DJV_IMAGE Global * global();

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_image/gl_inline.h>
#endif

