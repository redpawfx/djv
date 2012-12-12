/*------------------------------------------------------------------------------
 lib/djv_image/application.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include <djv_image/gl.h>
#include <djv_image/gl_data.h>
#include <djv_image/gl_offscreen.h>
#include <djv_image/io.h>
#include <djv_image/time.h>
#include <FL/Fl.H>

namespace djv_image {

//------------------------------------------------------------------------------
// global()
//------------------------------------------------------------------------------

namespace {

class _Global
{
public:

  void init()
  {
    _gl = std::auto_ptr<gl::Global>(new gl::Global);
    _gl_data = std::auto_ptr<gl_data::Global>(new gl_data::Global);
    _gl_offscreen =
        std::auto_ptr<gl_offscreen::Global>(new gl_offscreen::Global);
    _time = std::auto_ptr<time::Global>(new time::Global);
    _io = std::auto_ptr<io::Global>(new io::Global);
  }

  gl::Global * gl()
  {
    return _gl.get();
  }
  
  gl_data::Global * gl_data()
  {
    return _gl_data.get();
  }
  
  gl_offscreen::Global * gl_offscreen()
  {
    return _gl_offscreen.get();
  }
  
  time::Global * time()
  {
    return _time.get();
  }
  
  io::Global * io()
  {
    return _io.get();
  }
  
private:

  std::auto_ptr<gl::Global> _gl;
  std::auto_ptr<gl_data::Global> _gl_data;
  std::auto_ptr<gl_offscreen::Global> _gl_offscreen;
  std::auto_ptr<time::Global> _time;
  std::auto_ptr<io::Global> _io;
};

std::auto_ptr<_Global> _global;

}

namespace gl {

Global * global()
{
  return _global->gl();
}

}

namespace gl_data {

Global * global()
{
  return _global->gl_data();
}

}

namespace gl_offscreen {

Global * global()
{
  return _global->gl_offscreen();
}

}

namespace time {

Global * global()
{
  return _global->time();
}

}

namespace io {

Global * global()
{
  return _global->io();
}

}

//------------------------------------------------------------------------------
// Application_Base
//------------------------------------------------------------------------------

namespace application {

Application_Base::Application_Base(
  const String & name,
  int & argc,
  char ** argv
)
  throw (Error) :
  djv_base::application::Application_Base(name, argc, argv),
  _help_default(true)
{
  DJV_DEBUG("Application_Base::Application_Base");
  
  Fl::visual(FL_DOUBLE | FL_RGB);
  
  _global = std::auto_ptr<_Global>(new _Global);
  _global->init();
}

Application_Base::~Application_Base()
{
  DJV_DEBUG("Application_Base::~Application_Base");
  
  _global.reset();
}

String Application_Base::info() const
{
  return string::Format(label_info).
    arg(djv_base::application::Application_Base::info()).
    arg(string::label(gl::global()->pipeline())).
    arg(string::label(gl_offscreen::global()->buffer())).
    arg(gl::global()->device()->vendor()).
    arg(gl::global()->device()->renderer()).
    arg(gl::global()->device()->version()).
    arg(io::global()->load()->name_list(), ", ").
    arg(io::global()->save()->name_list(), ", ");
}

//------------------------------------------------------------------------------

}
}


