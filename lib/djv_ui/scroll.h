/*------------------------------------------------------------------------------
 lib/djv_ui/scroll.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_SCROLL_H
#define DJV_UI_SCROLL_H

#include <djv_ui/scroll_def.h>

#include <djv_ui/widget.h>
#include <FL/Fl_Scroll.H>

class Fl_Box;

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::scroll
//
//! Scroll widgets.
//------------------------------------------------------------------------------

namespace scroll {

//------------------------------------------------------------------------------
//!\class Scroll
//
//! A scroll widget.
//------------------------------------------------------------------------------

class _DJV_UI Scroll : public Fl_Scroll, public Widget_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Scroll();
  
  virtual ~Scroll();

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

  void resizable(bool);
  
  bool resizable() const;

  virtual void dirty();
  virtual void geom(const Box2i &);
  virtual void resize(int, int, int, int);
  
  virtual const Box2i & geom() const;
  virtual const V2i & size_min() const;
  virtual int visible() const;

  //@}
  
private:

  bool _resizable;
  Fl_Box * _dummy_widget;
  V2i _size_min;
};

//------------------------------------------------------------------------------

}
}

#endif

