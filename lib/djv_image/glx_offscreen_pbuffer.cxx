/*------------------------------------------------------------------------------
 lib/djv_image/glx_offscreen_pbuffer.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "glx_offscreen_private.h"

#include "glx_private.h"
#include <FL/x.H>
#include <algorithm>

namespace djv_image {
namespace gl_offscreen {

//------------------------------------------------------------------------------
// _Pbuffer
//------------------------------------------------------------------------------

_Pbuffer::_Pbuffer() :
  _context(0),
  _pbuffer(0)
{
  DJV_DEBUG("_Pbuffer::_Pbuffer");
}

_Pbuffer::~_Pbuffer()
{
  DJV_DEBUG("_Pbuffer::~_Pbuffer");

  if (_pbuffer)
  {
    DJV_DEBUG_PRINT("pbuffer");
    
    glXDestroyPbuffer(fl_display, _pbuffer);
  }

  if (_context)
  {
    DJV_DEBUG_PRINT("context");
    
    glXDestroyContext(fl_display, _context);
  }
}

void _Pbuffer::init(const Info & in, GLXFBConfig config) throw (Error)
{
  DJV_DEBUG("_Pbuffer::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  _info = in;
  
  // Create context.
  
  _context = glXCreateNewContext(fl_display, config, GLX_RGBA_TYPE,
    (GLXContext)0, True);
  if (! _context)
    throw Error(gl::error_glx, gl::error_glx_context_create);
  
  // Create pbuffer.
  
  int attrib [] =
  {
    GLX_PBUFFER_WIDTH, _info.size.x,
    GLX_PBUFFER_HEIGHT, _info.size.y,
    GLX_PRESERVED_CONTENTS, True,
    //GLX_LARGEST_PBUFFER, True,
    0
  };
  _pbuffer = glXCreatePbuffer(fl_display, config, attrib);
  if (! _pbuffer)
    throw Error(gl::error_glx, error_glx_pbuffer_create);
  
  GLuint w = 0, h = 0;
  glXQueryDrawable(fl_display, _pbuffer, GLX_WIDTH_SGIX, &w);
  glXQueryDrawable(fl_display, _pbuffer, GLX_HEIGHT_SGIX, &h);
  _info.size = V2i(w, h);
  
  DJV_DEBUG_PRINT("size = " << _info.size);
}

void _Pbuffer::current() throw (Error)
{
  DJV_DEBUG("_Pbuffer::current");
  DJV_DEBUG_PRINT("context = " << _context);
  
  if (! glXMakeCurrent(fl_display, _pbuffer, _context))
    throw Error(gl::error_glx, gl::error_glx_context_bind);
}

//------------------------------------------------------------------------------
// _GLX_Pbuffer_Factory
//------------------------------------------------------------------------------

namespace {

struct Fb_Config
{
  GLXFBConfig p;
  int id;
  int visual, visual_type;
  int caveat;
  int r, g, b, a;
};

inline int weight_visual_type(int in)
{
  switch (in)
  {
    case GLX_NONE: return 7;
    case GLX_TRUE_COLOR: return 6;
    case GLX_DIRECT_COLOR: return 5;
    case GLX_PSEUDO_COLOR: return 4;
    case GLX_STATIC_COLOR: return 3;
    case GLX_GRAY_SCALE: return 2;
    case GLX_STATIC_GRAY: return 1;
  }
  return 0;
}

inline int weight_caveat(int in)
{
  switch (in)
  {
    case GLX_NONE: return 3;
    case GLX_SLOW_CONFIG: return 2;
    case GLX_NON_CONFORMANT_CONFIG: return 1;
  }
  return 0;
}

String label_visual(int in)
{
  char out [string::cstring_size] = "";
  SNPRINTF(out, string::cstring_size, "%x", in);
  return out;
}

String label_visual_type(int in)
{
  switch (in)
  {
    case GLX_TRUE_COLOR: return "true color";
    case GLX_DIRECT_COLOR: return "direct color";
    case GLX_PSEUDO_COLOR: return "pseudo color";
    case GLX_STATIC_COLOR: return "static color";
    case GLX_GRAY_SCALE: return "gray scale";
    case GLX_STATIC_GRAY: return "static gray";
  }
  return "none";
}

String label_caveat(int in)
{
  switch (in)
  {
    case GLX_SLOW_CONFIG: return "slow";
    case GLX_NON_CONFORMANT_CONFIG: return "nonconformant";
  }
  return "none";
}

inline bool compare(const Fb_Config & a, const Fb_Config & b)
{
  return
    (b.r + b.g + b.b + b.a + weight_visual_type(b.visual_type) +
      weight_caveat(b.caveat)) <
    (a.r + a.g + a.b + a.a + weight_visual_type(a.visual_type) +
      weight_caveat(a.caveat));
}

GLXFBConfig _fb_config(Display * display, int screen)
{
  DJV_DEBUG("fb_config");

  int attrib [] =
  {
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT_SGIX,
    GLX_DOUBLEBUFFER, False,
    GLX_RED_SIZE, 1,
    GLX_GREEN_SIZE, 1,
    GLX_BLUE_SIZE, 1,
    GLX_ALPHA_SIZE, 0,
    None
  };
  int size = 0;
  GLXFBConfig * config = glXChooseFBConfigSGIX(display, screen, attrib, &size);
  
  List<Fb_Config> list;
  for (int i = 0; i < size; ++i)
  {
    Fb_Config tmp;
    tmp.p = config[i];
    glXGetFBConfigAttrib(display, config[i], GLX_FBCONFIG_ID, &tmp.id);
    glXGetFBConfigAttrib(display, config[i], GLX_VISUAL_ID, &tmp.visual);
    glXGetFBConfigAttrib(display, config[i], GLX_X_VISUAL_TYPE,
      &tmp.visual_type);
    glXGetFBConfigAttrib(display, config[i], GLX_CONFIG_CAVEAT, &tmp.caveat);
    glXGetFBConfigAttrib(display, config[i], GLX_RED_SIZE, &tmp.r);
    glXGetFBConfigAttrib(display, config[i], GLX_GREEN_SIZE, &tmp.g);
    glXGetFBConfigAttrib(display, config[i], GLX_BLUE_SIZE, &tmp.b);
    glXGetFBConfigAttrib(display, config[i], GLX_ALPHA_SIZE, &tmp.a);

    if (GLEW_ARB_half_float_pixel)
      if (tmp.r >= 16 || tmp.g >= 16 || tmp.b >= 16 || tmp.a >= 16) continue;

    list += tmp;
  }
  if (config) XFree(config);
  
  std::sort(list.begin(), list.end(), compare);
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    DJV_DEBUG_PRINT("id " << list[i].id <<
      " visual = " << label_visual(list[i].visual) <<
      " visual type = " << label_visual_type(list[i].visual_type) <<
      " caveat " << label_caveat(list[i].caveat) <<
      " depth = " << list[i].r << " " << list[i].g << " " << list[i].b <<
        " " << list[i].a);

    GLXContext context = glXCreateNewContext(
      display, list[i].p, GLX_RGBA_TYPE, (GLXContext)0, True);
    if (context)
    {
      glXDestroyContext(display, context);
      return list[i].p;
    }
  }

  return 0;
}

}

_GLX_Pbuffer_Factory::_GLX_Pbuffer_Factory() :
  _config(0)
{
  DJV_DEBUG("_GLX_Pbuffer_Factory::_GLX_Pbuffer_Factory");
}

_GLX_Pbuffer_Factory::~_GLX_Pbuffer_Factory()
{
  DJV_DEBUG("_GLX_Pbuffer_Factory::~_GLX_Pbuffer_Factory");

  for (list::size_type i = 0; i < _list.size(); ++i)
    delete _list[i];
}

void _GLX_Pbuffer_Factory::init() throw (Error)
{
  DJV_DEBUG("_GLX_Pbuffer_Factory::init");

  // Frame buffer configuration.
  
  _config = _fb_config(fl_display, fl_screen);
}

void _GLX_Pbuffer_Factory::add(_Pbuffer * in)
{
  DJV_DEBUG("_GLX_Pbuffer_Factory::add");

  _list += in;
  if (_list.size() > _list_max)
    delete _list.pop_front();
}

_Pbuffer * _GLX_Pbuffer_Factory::get(const Info & in) throw (Error)
{
  DJV_DEBUG("_GLX_Pbuffer_Factory::get");
  DJV_DEBUG_PRINT("in = " << in);
  
  _Pbuffer * out = 0;

  if (! _config)
    throw Error(gl::error_glx, error_glx_fbconfig);

  for (List<_Pbuffer *>::iterator i = _list.begin(); i != _list.end(); ++i)
    if (in == (*i)->info())
    {
      DJV_DEBUG_PRINT("cached");
      out = *i;
      _list.erase(i);
      break;
    }
  
  if (! out)
  {
    out = new _Pbuffer;
    out->init(in, _config);
  }
  return out;
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

class Global::_Pbuffer::Context_Restore
{
public:

  Context_Restore() :
    _context(glXGetCurrentContext()), 
    _drawable(glXGetCurrentDrawable())
  {}
  
  ~Context_Restore()
  {
    glXMakeCurrent(fl_display, _drawable, _context);
  }

private:

  GLXContext _context;
  GLXDrawable _drawable;
};

Global::_Pbuffer::_Pbuffer(_Pbuffer_Factory * factory) :
  _factory(factory),
  _p(0)
{
  DJV_DEBUG("Global::_Pbuffer::_Pbuffer");
}

Global::_Pbuffer::~_Pbuffer()
{
  DJV_DEBUG("Global::_Pbuffer::~_Pbuffer");
  
  if (_p)
    _factory->add(_p);
}

void Global::_Pbuffer::init(const Info & in) throw (Error)
{
  DJV_DEBUG("Global::_Pbuffer::init");
  DJV_DEBUG_PRINT("in = " << in);

  Context_Restore context_restore;

  if (_p)
    if (in != _p->info())
    {
      _factory->add(_p);
      _p = 0;
    }

  if (! _p)
    _p = _factory->get(in);
}

const Info & Global::_Pbuffer::info() const
{
  return _p->info();
}

void Global::_Pbuffer::bind()
{
  DJV_DEBUG("Global::_Pbuffer::bind");
  
  _context_restore = std::auto_ptr<Context_Restore>(new Context_Restore);
  
  _p->current();
}

void Global::_Pbuffer::unbind()
{
  DJV_DEBUG("Global::_Pbuffer::unbind");
  
  _context_restore.reset();
}

//------------------------------------------------------------------------------

}
}

