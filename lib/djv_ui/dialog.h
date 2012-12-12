/*------------------------------------------------------------------------------
 lib/djv_ui/dialog.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_DIALOG_H
#define DJV_UI_DIALOG_H

#include <djv_ui/dialog_def.h>

#include <djv_ui/window.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::dialog
//
//! Dialogs.
//------------------------------------------------------------------------------

namespace dialog {

//------------------------------------------------------------------------------
//!\class Base
//
//! A base class for dialogs.
//------------------------------------------------------------------------------

class _DJV_UI Base : public window::Window
{
public:
  
  Base(const String & title = String());
  
  virtual ~Base() = 0;
  
  virtual void title(const String &);
};

//------------------------------------------------------------------------------
//!\class Message_Base
//
//! A base class for message dialogs.
//------------------------------------------------------------------------------

class _DJV_UI Message_Base : public Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Message_Base(const String & title = String());
  
  virtual ~Message_Base() = 0;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual void dirty();
  
  virtual const V2i & size_min() const;
  
  //@}
  
private:

  V2i _size_min;
};

//------------------------------------------------------------------------------
//!\class Message
//
//! A message dialog.
//------------------------------------------------------------------------------

class _DJV_UI Message : public Message_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Message(const String & label = String());
  
  virtual ~Message();

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
  
  virtual void show();

  //@}
  
private:

  DJV_CALLBACK(Message, close_callback, bool);

  struct _Message;
  std::auto_ptr<_Message> _p;
};

//------------------------------------------------------------------------------
//!\class Question
//
//! A question dialog.
//------------------------------------------------------------------------------

class _DJV_UI Question : public Message_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Question(const String & label = String());
  
  virtual ~Question();

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
  
  virtual void show();
  
  //@}

private:

  DJV_CALLBACK(Question, ok_callback, bool);
  DJV_CALLBACK(Question, close_callback, bool);

  struct _Question;
  std::auto_ptr<_Question> _p;
};

//------------------------------------------------------------------------------
//!\class Input
//
//! An input dialog.
//------------------------------------------------------------------------------

class _DJV_UI Input : public Message_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Input(const String & label = String(), const String & value = String());
  
  virtual ~Input();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(const String &);
  
  String get() const;
  
  callback::Signal<const String &> signal;

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
  
  virtual void show();
  
  //@}
  
private:

  DJV_CALLBACK(Input, widget_callback, const String &);
  DJV_CALLBACK(Input, ok_callback, bool);
  DJV_CALLBACK(Input, close_callback, bool);

  struct _Input;
  std::auto_ptr<_Input> _p;
};

//------------------------------------------------------------------------------
//!\class Choice
//
//! A choice dialog.
//------------------------------------------------------------------------------

class _DJV_UI Choice : public Message_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Choice(
    const String & label = String(),
    const List<String> & list = List<String>()
  );
  
  virtual ~Choice();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void list(const List<String> &);
  
  List<String> list() const;

  callback::Signal<int> signal;

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
  
  virtual void show();

  //@}
  
private:

  DJV_CALLBACK(Choice, ok_callback, bool);
  DJV_CALLBACK(Choice, close_callback, bool);

  struct _Choice;
  std::auto_ptr<_Choice> _p;
};

//------------------------------------------------------------------------------
//!\class Multi_Choice
//
//! A multiple choice dialog.
//------------------------------------------------------------------------------

class _DJV_UI Multi_Choice : public Message_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Multi_Choice(
    const String & label = String(),
    const List<String> & list = List<String>()
  );
  
  virtual ~Multi_Choice();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void list(const List<String> &);
  
  List<String> list() const;

  callback::Signal<const List<bool> &> signal;
  callback::Signal<const List<int> &> index_signal;

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
  
  virtual void show();

  //@}
  
private:

  DJV_CALLBACK(Multi_Choice, ok_callback, bool);
  DJV_CALLBACK(Multi_Choice, close_callback, bool);

  struct _Multi_Choice;
  std::auto_ptr<_Multi_Choice> _p;
};

//------------------------------------------------------------------------------
//!\class Progress
//
//! A progress-bar dialog.
//------------------------------------------------------------------------------

class _DJV_UI Progress : public Message_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Progress();
  
  virtual ~Progress();
  
  virtual void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(int);
  
  callback::Signal<int> signal;
  callback::Signal<bool> del_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{
  
  void label(const String &);
  
  const String & label() const;

  //@}
  
private:

  DJV_CALLBACK(Progress, cancel_callback, bool);
  DJV_FL_CALLBACK(Progress, idle_callback);

  int _value, _current;
  djv_base::time::Timer _time;
  double _time_accum;
  djv_base::time::Timer _elapsed;
  
  struct _Progress;
  std::auto_ptr<_Progress> _p;
};

//------------------------------------------------------------------------------
//!\class Color
//
//! A color picker dialog.
//------------------------------------------------------------------------------

class _DJV_UI Color : public Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Color(const djv_image::Color & = djv_image::Color());
  
  virtual ~Color();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(const djv_image::Color &);
  
  const djv_image::Color & get() const;
  
  callback::Signal<const djv_image::Color &> signal;

  //@}
  
private:

  DJV_CALLBACK(Color, widget_callback, const djv_image::Color &);
  DJV_CALLBACK(Color, close_callback, bool);
  
  void widget_update();
  
  djv_image::Color _value;
  
  struct _Color;
  std::auto_ptr<_Color> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  label_message,
  label_question,
  label_input,
  label_choice,
  label_multi_choice,
  label_progress,
  label_progress_estimate,
  label_progress_elapsed,
  label_color,
  label_ok,
  label_close,
  label_cancel,
  label_yes,
  label_no,
  label_show,
  label_pin;

_DJV_UI extern const String
  tooltip_pin;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global dialogs.

class _DJV_UI Global
{
public:

  Global();
  
  ~Global();

  void message(const String &) const;

  void question(
    const String & label,
    Callback_Base *,
    callback::Signal<bool>::Callback *
  ) const;

  void input(
    const String & label,
    const String & value,
    Callback_Base *,
    callback::Signal<const String &>::Callback *
  ) const;

  void choice(
    const String & label,
    const List<String> & list,
    Callback_Base *,
    callback::Signal<int>::Callback *
  ) const;

  void multi_choice(
    const String & label,
    const List<String> & list,
    Callback_Base *,
    callback::Signal<const List<bool> &>::Callback *
  ) const;

  void multi_choice(
    const String & label,
    const List<String> & list,
    Callback_Base *,
    callback::Signal<const List<int> &>::Callback *
  ) const;

  void progress(
    const String & label,
    int value,
    Callback_Base *,
    callback::Signal<int>::Callback * callback,
    callback::Signal<bool>::Callback * del_callback
  ) const;
  
  void progress_del(Callback_Base *) const;

  void color(
    const djv_image::Color &,
    Callback_Base *,
    callback::Signal<const djv_image::Color &>::Callback *
  ) const;

private:

  widget::Auto_Ptr<Message> _message;
  widget::Auto_Ptr<Question> _question;
  widget::Auto_Ptr<Input> _input;
  widget::Auto_Ptr<Choice> _choice;
  widget::Auto_Ptr<Multi_Choice> _multi_choice;
  widget::Auto_Ptr<Progress> _progress;
  widget::Auto_Ptr<Color> _color;
};

//! Access to global dialogs.

_DJV_UI Global * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

