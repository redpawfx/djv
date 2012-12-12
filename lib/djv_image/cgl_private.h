/*------------------------------------------------------------------------------
 lib/djv_image/cgl_private.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_CGL_PRIVATE_H
#define DJV_IMAGE_CGL_PRIVATE_H

#include <djv_image/gl.h>

#include <OpenGL/OpenGL.h>

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// _CGL_Device
//
// Apple CGL.
//------------------------------------------------------------------------------

class _CGL_Device : public Device
{
public:

  _CGL_Device();
  
  ~_CGL_Device();

  void init() throw (Error);

private:

  CGLContextObj _context;
};

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

static const String
  error_glew = "GLEW",
  error_glew_init = "Cannot initialize (#%%)",
  error_cgl = "CGL",
  error_cgl_pixel_format = "Cannot get pixel format",
  error_cgl_context_create = "Cannot create context (#%%)",
  error_cgl_context_bind = "Cannot bind context (#%%)";

//------------------------------------------------------------------------------

}
}

#endif
