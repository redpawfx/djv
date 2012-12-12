/*------------------------------------------------------------------------------
 lib/djv_image/cgl_offscreen.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "cgl_offscreen_private.h"

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::_Global::_Global() throw (Error)
{
  DJV_DEBUG("Global::_Global::_Global");
  
  _pbuffer = std::auto_ptr<_Pbuffer_Factory>(new _CGL_Pbuffer_Factory);
  _pbuffer->init();
  
  _fbo = std::auto_ptr<_FBO_Factory>(new _FBO_Factory);
  _fbo->init();
}

Global::_Global::~_Global()
{
  DJV_DEBUG("Global::_Global::~_Global");
}

//------------------------------------------------------------------------------

}
}

