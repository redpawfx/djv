/*------------------------------------------------------------------------------
 lib/djv_ui/tool_button.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_TOOL_BUTTON_H
#define DJV_UI_TOOL_BUTTON_H

#include <djv_ui/tool_button_def.h>

#include <djv_ui/frame.h>
#include <FL/Fl_Image.H>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::tool_button
//
//! Tool-button widgets.
//------------------------------------------------------------------------------

namespace tool_button {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Type.

enum TYPE
{
  PUSH,
  TOGGLE,
  RADIO,
  REPEAT
};

//@}
//------------------------------------------------------------------------------
//!\class Tool_Button
//
//! A tool-button widget.
//------------------------------------------------------------------------------

class _DJV_UI Tool_Button : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Tool_Button(const String & icon, int margin = 3);
  
  virtual ~Tool_Button();
  
  virtual void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(bool = true);
  
  bool get() const;
  
  callback::Signal<bool> signal;
  callback::Signal<bool> push_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Type
  //@{
  
  void type(TYPE);
  
  TYPE type() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Radio Group
  //@{
  
  void radio_group(const List<Tool_Button *> &);
  
  const List<Tool_Button *> & radio_group() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Icon
  //@{

  void icon(Fl_Image *);

  //@}
  //----------------------------------------------------------------------------
  //!\name Shortcut
  //@{

  void shortcut(int);
  
  int shortcut() const;

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
  
  void margin(int);
  
  int margin() const;

  virtual void dirty();

  //@}

private:

  void action();

  DJV_FL_CALLBACK(Tool_Button, repeat_callback);

  void radio_update();

  bool _value;
  
  TYPE _type;
  
  bool _repeat_active;
  
  List<Tool_Button *> _radio_group;
  
  Fl_Image * _icon;
  std::auto_ptr<Fl_Image> _icon_inactive;
  
  int _shortcut;
  
  int _margin;
  
  bool _pressed;
  bool _pressed_inside;
};

//------------------------------------------------------------------------------
//!\class Tool_Choice_Button
//
//! A tool-button widget with multiple states.
//------------------------------------------------------------------------------

class _DJV_UI Tool_Choice_Button : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Tool_Choice_Button(
    const List<String> & icon,
    int margin = 3
    
  );
  
  virtual ~Tool_Choice_Button();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(int);
  
  int get() const;
  
  callback::Signal<int> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Shortcut
  //@{

  void shortcut(int);
  
  int shortcut() const;

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
  
  void margin(int);
  
  int margin() const;

  virtual void dirty();

  //@}
  
private:

  void action();

  int _value;
  
  List<Fl_Image *> _icon;
  
  int _shortcut;
  
  int _margin;
  
  bool _pressed;
  bool _pressed_inside;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_UI void radio_group(const List<Tool_Button *> &);

//@}
//------------------------------------------------------------------------------

}
}

#endif

