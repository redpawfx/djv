/*------------------------------------------------------------------------------
 lib/djv_ui/check_button.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_CHECK_BUTTON_H
#define DJV_UI_CHECK_BUTTON_H

#include <djv_ui/check_button_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::check_button
//
//! Check-button widgets.
//------------------------------------------------------------------------------

namespace check_button {

//------------------------------------------------------------------------------
//!\class Check_Button
//
//! A check button widget.
//------------------------------------------------------------------------------

class _DJV_UI Check_Button : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Check_Button(const String &);
  
  virtual ~Check_Button();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(bool);
  void toggle();
  
  bool get() const;
  
  callback::Signal<bool> signal;
  
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
  
  bool _value;
  
  String _label;
  
  int _shortcut;
  
  Fl_Image * _icon;

  String _draw_label;
  int _draw_index;
};

//------------------------------------------------------------------------------
//!\class Check_Button_Group
//
//! A group of check-buttons.
//------------------------------------------------------------------------------

class _DJV_UI Check_Button_Group : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Check_Button_Group(
    const List<String> & label = List<String>()
  );
  
  Check_Button_Group(
    const String & title,
    const List<String> & label = List<String>()
  );

  virtual ~Check_Button_Group();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
    
  void set(const List<bool> &);
  void set_index(const List<int> &);
  
  const List<bool> & get() const;
  const List<int> get_index() const;
  
  callback::Signal<const List<bool> &> signal;
  callback::Signal<const List<int> &> index_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Title
  //@{
  
  void title(const String &);
  
  const String & title() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Labels
  //@{
  
  void label(const List<String> &);
  
  const List<String> & label() const;
  
  //@}
  
private:

  void init();

  DJV_CALLBACK(Check_Button_Group, widget_callback, bool);

  void value_update();
  void widget_update();

  List<bool> _value;
  String _title;
  List<String> _label;
  
  struct _Check_Button_Group;
  std::auto_ptr<_Check_Button_Group> _p;
};

//------------------------------------------------------------------------------

}
}

#endif

