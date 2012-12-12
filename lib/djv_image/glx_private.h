/*------------------------------------------------------------------------------
 lib/djv_image/glx_private.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GLX_PRIVATE_H
#define DJV_IMAGE_GLX_PRIVATE_H

#include <djv_image/gl.h>

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// _GLX_Device
//------------------------------------------------------------------------------

class _GLX_Device : public Device
{
public:

  _GLX_Device();
  
  ~_GLX_Device();

  void init() throw (Error);
  
private:

  Window _window;
  GLXContext _context;
};

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

static const String
  error_glew = "GLEW",
  error_glew_init = "Cannot initialize (#%%)",
  error_glx = "GLX",
  error_glx_init = "Cannot initialize",
  error_glx_context_create = "Cannot create context",
  error_glx_context_bind = "Cannot bind context",
  error_x11 = "X11",
  error_x11_window = "Cannot create offscreen window";

//------------------------------------------------------------------------------

}
}

#endif
