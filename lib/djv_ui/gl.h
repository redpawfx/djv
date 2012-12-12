/*------------------------------------------------------------------------------
 lib/djv_ui/gl.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_GL_H
#define DJV_UI_GL_H

#include <djv_ui/gl_def.h>

#include <djv_image/gl.h>
#include <djv_ui/widget.h>
#include <FL/Fl_Gl_Window.H>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::gl
//
//! OpenGL widgets.
//------------------------------------------------------------------------------

namespace gl {

//------------------------------------------------------------------------------
//!\class Gl
//
//! An OpenGL widget.
//------------------------------------------------------------------------------

class _DJV_UI Gl : public Fl_Gl_Window, public Widget_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Gl();
  
  virtual ~Gl();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual int handle(int);
  
  virtual void show();
  virtual void draw();
  
  virtual const Fl_Widget * fltk_widget() const { return this; }

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual void geom(const Box2i &);
  virtual void resize(int, int, int, int);
  
  virtual const Box2i & geom() const;
  virtual int visible() const;
  
  //@}
};

//------------------------------------------------------------------------------

}
}

#endif

