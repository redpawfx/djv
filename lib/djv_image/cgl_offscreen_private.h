/*------------------------------------------------------------------------------
 lib/djv_image/cgl_offscreen_private.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_CGL_OFFSCREEN_PRIVATE_H
#define DJV_IMAGE_CGL_OFFSCREEN_PRIVATE_H

#include <djv_image/gl_offscreen_private.h>

#include <OpenGL/OpenGL.h>

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// _Pbuffer
//------------------------------------------------------------------------------

class _Pbuffer
{
public:

  _Pbuffer();
  
  ~_Pbuffer();
  
  void init(const Info &, const CGLPixelFormatObj) throw (Error);
  void current() throw (Error);
  
  const Info & info() const { return _info; }
  
private:

  Info _info;
  CGLContextObj _context;
  CGLPBufferObj _pbuffer;
};

//------------------------------------------------------------------------------
// _CGL_Pbuffer_Factory
//------------------------------------------------------------------------------

class _CGL_Pbuffer_Factory : public _Pbuffer_Factory
{
public:

  _CGL_Pbuffer_Factory();
  
  ~_CGL_Pbuffer_Factory();
  
  void init() throw (Error);
  void add(_Pbuffer *);
  
  _Pbuffer * get(const Info &) throw (Error);

  CGLPixelFormatObj format() const { return _format; }
  
private:

  List<_Pbuffer *> _list;
  static const int _list_max = 10;
  CGLPixelFormatObj _format;
};

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const String
  error_cgl = "CGL",
  error_cgl_pbuffer_create = "Cannot create PBuffer (#%%)",
  error_cgl_pbuffer_set = "Cannot set PBuffer (#%%)",
  error_cgl_pbuffer_describe = "Cannot describe PBuffer (#%%)";

//------------------------------------------------------------------------------

}
}

#endif
