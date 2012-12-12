/*------------------------------------------------------------------------------
 lib/djv_ui/application_dialog.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_APP_DIALOG_H
#define DJV_UI_APP_DIALOG_H

#include <djv_ui/application_dialog_def.h>

#include <djv_ui/dialog.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::application_dialog
//
//! Application dialogs.
//------------------------------------------------------------------------------

namespace application_dialog {

//------------------------------------------------------------------------------
//!\class Message_Dialog
//
//! A dialog for displaying application messages.
//------------------------------------------------------------------------------

class _DJV_UI Message_Dialog : public dialog::Base
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Message_Dialog();
  
  ~Message_Dialog();
  
  virtual void del();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Messages
  //@{
  
  void set(const String &);
  void set(const Error &);
  void clear();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void show();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual void dirty();
  
  virtual const V2i & size_min() const;
  
  //@}
  
private:
  
  DJV_CALLBACK(Message_Dialog, show_callback, bool);
  DJV_CALLBACK(Message_Dialog, clear_callback, bool);
  DJV_CALLBACK(Message_Dialog, close_callback, bool);
  
  void widget_update();
  void dialog_update();
  
  List<String> _list;
  bool _show;
  V2i _size_min;
  
  struct _Message_Dialog;
  std::auto_ptr<_Message_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\class Info_Dialog
//
//! A dialog for displaying application information.
//------------------------------------------------------------------------------

class _DJV_UI Info_Dialog : public dialog::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Info_Dialog();
  
  ~Info_Dialog();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void show();
  
  //@}
  
private:
  
  DJV_CALLBACK(Info_Dialog, close_callback, bool);
  
  struct _Info_Dialog;
  std::auto_ptr<_Info_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\class About_Dialog
//
//! A dialog for displaying application about information.
//------------------------------------------------------------------------------

class _DJV_UI About_Dialog : public dialog::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  About_Dialog();
  
  ~About_Dialog();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void show();
  
  //@}
  
private:
  
  DJV_CALLBACK(About_Dialog, close_callback, bool);
  
  struct _About_Dialog;
  std::auto_ptr<_About_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  label_message,
  label_message_clear,
  label_info,
  label_about;

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

  Message_Dialog * message() const;
  
  Info_Dialog * info() const;
  
  About_Dialog * about() const;

private:

  widget::Auto_Ptr<Message_Dialog> _message;
  widget::Auto_Ptr<Info_Dialog> _info;
  widget::Auto_Ptr<About_Dialog> _about;
};

//! Access to global dialogs.

_DJV_UI Global * global();

_DJV_UI void message(const String &);
_DJV_UI void error(const Error &);

//@}
//------------------------------------------------------------------------------

}
}

#endif

