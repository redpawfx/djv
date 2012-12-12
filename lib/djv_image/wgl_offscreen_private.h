/*------------------------------------------------------------------------------
 lib/djv_image/wgl_offscreen_private.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_WGL_OFFSCREEN_PRIVATE_H
#define DJV_IMAGE_WGL_OFFSCREEN_PRIVATE_H

#include <djv_image/gl_offscreen_private.h>

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

  void init(const Info &, int format) throw (Error);
  void current() throw (Error);
  
  const Info & info() const { return _info; }

private:

  Info _info;
  HDC _device;
  HGLRC _context;
  HPBUFFERARB _pbuffer;
};

class _WGL_Pbuffer_Factory : public _Pbuffer_Factory
{
public:

  _WGL_Pbuffer_Factory();

  ~_WGL_Pbuffer_Factory();
  
  void init() throw (Error);
  void add(_Pbuffer *);

  _Pbuffer * get(const Info &) throw (Error);
  
  int format() const { return _format; }
  
private:

  List<_Pbuffer *> _list;
  static const int _list_max = 10;
  int _format;
};

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const String
  error_wgl_pbuffer_create = "Cannot create PBuffer (#%%)",
  error_wgl_pbuffer_device = "Cannot get PBuffer device (#%%)";

//------------------------------------------------------------------------------

}
}

#endif

