/*------------------------------------------------------------------------------
 lib/djv_image/gl.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "gl.h"

#include <djv_base/math.h>

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// ortho()
//------------------------------------------------------------------------------

bool opengl_2_0()
{
  return 
    (GLEW_VERSION_1_3 || GLEW_ARB_multitexture) &&
    GLEW_ARB_shader_objects &&
    GLEW_ARB_texture_rectangle;
}

//------------------------------------------------------------------------------
// ortho()
//------------------------------------------------------------------------------

void ortho(const V2i & size, const V2f & min_max)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, size.x, 0, size.y, min_max.x, min_max.y);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

//------------------------------------------------------------------------------
// format()
//------------------------------------------------------------------------------

GLenum format(PIXEL in, bool bgr)
{
  switch (in)
  {
    case pixel::L_U8:
    case pixel::L_U16:
#if defined (DJV_OPENEXR)
    case pixel::L_F16:
#endif
    case pixel::L_F32:
      return GL_LUMINANCE;
    
    case pixel::LA_U8:
    case pixel::LA_U16:
#if defined (DJV_OPENEXR)
    case pixel::LA_F16:
#endif
    case pixel::LA_F32:
      return GL_LUMINANCE_ALPHA;
    
    case pixel::RGB_U8:
    case pixel::RGB_U16:
#if defined (DJV_OPENEXR)
    case pixel::RGB_F16:
#endif
    case pixel::RGB_F32:
      return bgr ? GL_BGR : GL_RGB;
    
    case pixel::RGB_U10:
      return bgr ? GL_BGRA : GL_RGBA;
    
    case pixel::RGBA_U8:
    case pixel::RGBA_U16:
#if defined (DJV_OPENEXR)
    case pixel::RGBA_F16: 
#endif
    case pixel::RGBA_F32:
      return bgr ? GL_BGRA : GL_RGBA;

    default: break;
  }
  return 0;
}

//------------------------------------------------------------------------------
// type()
//------------------------------------------------------------------------------

GLenum type(PIXEL in)
{
  switch (in)
  {
    case pixel::L_U8:
    case pixel::LA_U8:
    case pixel::RGB_U8:
    case pixel::RGBA_U8:
      return GL_UNSIGNED_BYTE;
    
    case pixel::RGB_U10:
      return GL_UNSIGNED_INT_10_10_10_2;
    
    case pixel::L_U16:
    case pixel::LA_U16:
    case pixel::RGB_U16:
    case pixel::RGBA_U16:
      return GL_UNSIGNED_SHORT;
    
#if defined (DJV_OPENEXR)
    case pixel::L_F16:
    case pixel::LA_F16:
    case pixel::RGB_F16:
    case pixel::RGBA_F16:
      return GLEW_ARB_half_float_pixel ? GL_HALF_FLOAT_ARB : 0;
#endif

    case pixel::L_F32:
    case pixel::LA_F32:
    case pixel::RGB_F32:
    case pixel::RGBA_F32:
      return GL_FLOAT;

    default: break;
  }
  return 0;
}

//------------------------------------------------------------------------------
// texture()
//------------------------------------------------------------------------------

GLenum texture()
{
  return GLEW_ARB_texture_rectangle ?
    GL_TEXTURE_RECTANGLE_ARB :
    GL_TEXTURE_2D;
}

//------------------------------------------------------------------------------
// Convesion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(PIPELINE, label_pipeline())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_pipeline()
{
  static const List<String> data = List<String>() <<
    "OpenGL 1.2" <<
    "OpenGL 2.0";
  
  DJV_ASSERT(data.size() == _PIPELINE_SIZE);

  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, PIPELINE in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Device::Device()
{
  DJV_DEBUG("Device::Device");
}

Device::~Device()
{
  DJV_DEBUG("Device::~Device");
}

const String & Device::vendor() const
{
  return _vendor;
}

const String & Device::renderer() const
{
  return _renderer;
}

const String & Device::version() const
{
  return _version;
}

Device * Global::device() const
{
  return _device.get();
}

void Global::pipeline(PIPELINE in)
{
  DJV_DEBUG("Global::pipeline");
  DJV_DEBUG_PRINT("in = " << in);

  _pipeline = in;
}

PIPELINE Global::pipeline() const
{
  return _pipeline;
}

//------------------------------------------------------------------------------

}
}

