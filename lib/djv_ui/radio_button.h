/*------------------------------------------------------------------------------
 lib/djv_ui/radio_button.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_RADIO_BUTTON_H
#define DJV_UI_RADIO_BUTTON_H

#include <djv_ui/radio_button_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::radio_button
//
//! Radio-button widgets.
//------------------------------------------------------------------------------

namespace radio_button {

//------------------------------------------------------------------------------
//!\class Radio_Button
//
//! A radio button widget.
//------------------------------------------------------------------------------

class _DJV_UI Radio_Button : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Radio_Button(const String &);
  
  virtual ~Radio_Button();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(bool = true);
  
  bool get() const;
  
  callback::Signal<bool> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Group
  //@{
  
  void group_set(int);
  void group(const List<Radio_Button *> &);
  
  int group_get() const;
  const List<Radio_Button *> & group() const;
  
  callback::Signal<int> group_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{

  virtual void label(const String &);
  
  virtual const String & label() const;

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

  void widget_update();

  bool _value;
  List<Radio_Button *> _group;
  
  String _label;
  
  int _shortcut;
  
  Fl_Image * _icon;

  String _draw_label;
  int _draw_index;
};

//------------------------------------------------------------------------------
//!\class Radio_Button_Group
//
//! A group of radio-buttons.
//------------------------------------------------------------------------------

class _DJV_UI Radio_Button_Group : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Radio_Button_Group(
    const List<String> & label = List<String>()
  );
  
  Radio_Button_Group(
    const String & title,
    const List<String> & label = List<String>()
  );

  virtual ~Radio_Button_Group();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(int);
  
  int get() const;
  
  callback::Signal<int> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Title
  //@{
  
  void title(const String &);
  
  const String & title() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{
  
  void label(const List<String> &);
  
  const List<String> & label() const;

  //@}
  
private:

  void init();

  DJV_CALLBACK(Radio_Button_Group, widget_callback, int);

  void value_update();
  void widget_update();

  int _value;
  String _title;
  List<String> _label;
  
  struct _Radio_Button_Group;
  std::auto_ptr<_Radio_Button_Group> _p;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_UI void group(const List<Radio_Button *> &);

//@}
//------------------------------------------------------------------------------

}
}

#endif

