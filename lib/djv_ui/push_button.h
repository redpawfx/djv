/*------------------------------------------------------------------------------
 lib/djv_ui/push_button.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_PUSH_BUTTON_H
#define DJV_UI_PUSH_BUTTON_H

#include <djv_ui/push_button_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::push_button
//
//! Push-button widgets.
//------------------------------------------------------------------------------

namespace push_button {

//------------------------------------------------------------------------------
//!\class Push_Button
//
//! A push-button widget.
//------------------------------------------------------------------------------

class _DJV_UI Push_Button : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Push_Button(const String & label);
  
  virtual ~Push_Button();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  callback::Signal<bool> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{

  void label(const String &);
  
  const String & label() const;

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
  
  String _label;
  
  int _shortcut;
  
  bool _pressed;
  bool _pressed_inside;

  String _draw_label;
  int _draw_index;  
};

//------------------------------------------------------------------------------

}
}

#endif

