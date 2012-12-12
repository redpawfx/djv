/*------------------------------------------------------------------------------
 lib/djv_ui/shuttle.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_SHUTTLE_H
#define DJV_UI_SHUTTLE_H

#include <djv_ui/shuttle_def.h>

#include <djv_ui/frame.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::shuttle
//
//! Shuttle widgets.
//------------------------------------------------------------------------------

namespace shuttle {

//------------------------------------------------------------------------------
//!\class Shuttle
//
//! A shuttle widget.
//------------------------------------------------------------------------------

class _DJV_UI Shuttle : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Shuttle(const String & prefix = String());
  
  virtual ~Shuttle();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  callback::Signal<bool> signal;
  callback::Signal<int> value_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual int handle(int);
  virtual void draw();

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  
  //@}
  
private:

  double _value;

  Fl_Image * _icon [8];
  
  bool _pressed;
  V2i _mouse_td;
};

//------------------------------------------------------------------------------

}
}

#endif
