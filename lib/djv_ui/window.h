/*------------------------------------------------------------------------------
 lib/djv_ui/window.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_WINDOW_H
#define DJV_UI_WINDOW_H

#include <djv_ui/window_def.h>

#include <djv_ui/widget.h>
#include <FL/Fl_Double_Window.H>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::window
//
//! Windows.
//------------------------------------------------------------------------------

namespace window {

//------------------------------------------------------------------------------
//!\class Window
//
//! A top-level window widget.
//------------------------------------------------------------------------------

class _DJV_UI Window : public Fl_Double_Window, public Widget_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Window(const String & title = String());
  
  virtual ~Window();
  
  virtual void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Title
  //@{

  virtual void title(const String &);
  
  const String & title() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{

  callback::Signal<bool> close_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  void fullscreen(bool);
  
  bool fullscreen() const;

  virtual int handle(int);
  virtual void show();
  virtual void draw();

  virtual const Fl_Widget * fltk_widget() const { return this; }

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void update();
  virtual void size(const V2i &, bool resizable = true);
  virtual void geom(const Box2i &);
  virtual void resize(int, int, int, int);
  virtual void size(layout_item::SIZE, layout_item::SIZE);
  
  virtual const Box2i & geom() const;
  virtual int visible() const;
  virtual const layout_item::SIZE * size() const;

  //@}
  
private:

  DJV_FL_WIDGET_CALLBACK(Window, close_callback);

  String _title;
  bool _fullscreen;
  Box2i _geom_tmp;
  bool _resizable;
};

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class _DJV_UI Global
{
public:

  Global();
  
  ~Global();
};

//------------------------------------------------------------------------------

}
}

#endif

