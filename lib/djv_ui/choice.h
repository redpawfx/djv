/*------------------------------------------------------------------------------
 lib/djv_ui/choice.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_CHOICE_H
#define DJV_UI_CHOICE_H

#include <djv_ui/choice_def.h>

#include <djv_ui/frame.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::choice
//
//! Choice widgets.
//------------------------------------------------------------------------------

namespace choice {

//------------------------------------------------------------------------------
//!\class Choice
//
//! A basic widget for selecting an item from a list.
//------------------------------------------------------------------------------

class _DJV_UI Choice : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Choice();
  
  Choice(
    const List<String> & list
  );
  
  Choice(
    const List<String> & list,
    const List<String> & text
  );
  
  virtual ~Choice();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(int);
  void list(const List<String> &);
  
  int get() const;
  const List<String> & list() const;
  
  callback::Signal<int> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{
  
  //! List labels.

  void text(const List<String> &);

  const List<String> & text() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void draw();

  virtual const Fl_Widget * fltk_widget() const;

  //@}
  
private:

  virtual void down(bool);
  virtual void select(bool);

  void init(
    const List<String> & list,
    const List<String> & text
  );

  DJV_CALLBACK(Choice, widget_callback, int);
  
  List<String> _list;
  List<String> _text;
  
  bool _down;
  bool _select;
  
  class _Menu_Button;
  friend class _Menu_Button;
  
  struct _Choice;
  std::auto_ptr<_Choice> _p;
};

//------------------------------------------------------------------------------
//!\class Choice_Edit
//
//! A widget for editing text with a list of preset values.
//------------------------------------------------------------------------------

class _DJV_UI Choice_Edit : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Choice_Edit(const List<String> & list = List<String>());
  
  virtual ~Choice_Edit();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const String &);
  
  String get() const;
  
  callback::Signal<const String &> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name List
  //@{
  
  void list(const List<String> &);
  
  const List<String> & list() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void size_string(const String &);

  virtual void dirty();
  
  //@}
  
private:

  DJV_CALLBACK(Choice_Edit, widget_callback, const String &);
  DJV_CALLBACK(Choice_Edit, menu_callback, int);

  void list_update();

  String _value;
  List<String> _list;
  
  struct _Choice_Edit;
  std::auto_ptr<_Choice_Edit> _p;
};

//------------------------------------------------------------------------------

}
}

#endif
