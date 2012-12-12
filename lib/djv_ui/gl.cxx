/*------------------------------------------------------------------------------
 lib/djv_ui/gl.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "gl.h"

namespace djv_ui {
namespace gl {

//------------------------------------------------------------------------------
// Gl
//------------------------------------------------------------------------------

Gl::Gl() :
  Fl_Gl_Window(0, 0, 0, 0)
{
  Fl_Gl_Window::end();
  
  Fl_Gl_Window::mode(FL_RGB | FL_DOUBLE);
}

Gl::~Gl()
{}

int Gl::handle(int in)
{
  switch (Widget_Base::handle(in))
  {
    case -1: return 0;
    case 1: return 1;
  }
  
  return Fl_Gl_Window::handle(in);
}

void Gl::show()
{
  Fl_Gl_Window::show();
  
  Widget_Base::show();
}

void Gl::draw()
{
  //Fl_Gl_Window::draw();
  
  Widget_Base::draw();
}

void Gl::geom(const Box2i & in)
{
  Fl_Gl_Window::resize(in.x, in.y, in.w, in.h);
  
  Widget_Base::geom(in);
}

const Box2i & Gl::geom() const
{
  return Widget_Base::geom();
}

void Gl::resize(int x, int y, int w, int h)
{
  geom(Box2i(x, y, w, h));
}

int Gl::visible() const
{
  return Fl_Gl_Window::visible();
}

//------------------------------------------------------------------------------

}
}

