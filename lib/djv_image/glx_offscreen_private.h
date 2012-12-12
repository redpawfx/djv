/*------------------------------------------------------------------------------
 lib/djv_image/glx_offscreen_private.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GLX_OFFSCREEN_PRIVATE_H
#define DJV_IMAGE_GLX_OFFSCREEN_PRIVATE_H

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
  
  void init(const Info &, GLXFBConfig) throw (Error);
  
  void current() throw (Error);
  
  const Info & info() const { return _info; }
  
private:

  Info _info;
  GLXContext _context;
  GLXPbuffer _pbuffer;
};

//------------------------------------------------------------------------------
// _GLX_Pbuffer_Factory
//------------------------------------------------------------------------------

class _GLX_Pbuffer_Factory : public _Pbuffer_Factory
{
public:

  _GLX_Pbuffer_Factory();
  
  ~_GLX_Pbuffer_Factory();
  
  void init() throw (Error);
  
  void add(_Pbuffer *);
  
  _Pbuffer * get(const Info &) throw (Error);
  
  GLXFBConfig config() const { return _config; }
  
private:

  List<_Pbuffer *> _list;
  static const int _list_max = 10;
  GLXFBConfig _config;
};

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

static const String
  error_glx = "GLX",
  error_glx_fbconfig = "Cannot get frame buffer configuration",
  error_glx_pbuffer_create = "Cannot create PBuffer";

//------------------------------------------------------------------------------

}
}

#endif
