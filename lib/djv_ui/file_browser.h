/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_FILE_BROWSER_H
#define DJV_UI_FILE_BROWSER_H

#include <djv_ui/file_browser_def.h>

#include <djv_ui/dialog.h>
#include <djv_base/file.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::file_browser
//
//! File browser.
//------------------------------------------------------------------------------

namespace file_browser {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Image thumbnails.

enum IMAGE
{
  IMAGE_OFF,
  IMAGE_LOW,
  IMAGE_HIGH,
  
  _IMAGE_SIZE
};

//! Sorting.

enum SORT
{
  SORT_NAME,
  SORT_TYPE,
  SORT_SIZE,
# if ! defined(DJV_WINDOWS)
  SORT_USER,
# endif
  SORT_TIME,
  
  _SORT_SIZE
};

//! Navigation.

enum NAVIGATE
{
  NAVIGATE_SINGLE_CLICK,
  NAVIGATE_DOUBLE_CLICK,
  
  _NAVIGATE_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Dialog
//
//! A file browser dialog.
//------------------------------------------------------------------------------

class _DJV_UI Dialog : public dialog::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Dialog(Prefs_Instance * = 0);
  
  ~Dialog();
  
  void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const File &);
  
  const File & get() const;
  
  callback::Signal<const File &> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{

  void image(IMAGE);
  void seq(file_seq::COMPRESS);
  void type(int);
  void hidden(bool);
  void sort(SORT);
  void sort_reverse(bool);
  void sort_directory(bool);

  IMAGE image() const;
  file_seq::COMPRESS seq() const;
  int type() const;
  bool hidden() const;  
  SORT sort() const;
  bool sort_reverse() const;
  bool sort_directory() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Dialog
  //@{
  
  void pin(bool);
  
  bool pin() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void show();

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void dirty();

  //@}
  
private:

  // Callbacks.

  DJV_CALLBACK(Dialog, value_callback, const String &);
  DJV_CALLBACK(Dialog, value_action_callback, const String &);
  DJV_CALLBACK(Dialog, value_change_callback, const String &);
  DJV_CALLBACK(Dialog, browser_callback, const File &);
  DJV_CALLBACK(Dialog, browser_mouse_callback, const File &);
  DJV_CALLBACK(Dialog, browser_action_callback, const File &);
  DJV_CALLBACK(Dialog, columns_callback, const List<int> &);
  DJV_CALLBACK(Dialog, columns_action_callback, int);
  
  DJV_CALLBACK(Dialog, up_callback, bool);
  DJV_CALLBACK(Dialog, prev_callback, bool);
  DJV_CALLBACK(Dialog, current_callback, bool);
#if ! defined(DJV_WINDOWS)
  DJV_CALLBACK(Dialog, home_callback, bool);
#endif
  DJV_CALLBACK(Dialog, desktop_callback, bool);
#if defined(DJV_WINDOWS)
  DJV_CALLBACK(Dialog, drive_callback, bool);
#else
  DJV_CALLBACK(Dialog, root_callback, bool);
#endif
  DJV_CALLBACK(Dialog, reload_callback, bool);
  DJV_CALLBACK(Dialog, seq_callback, int);
  
  DJV_FL_WIDGET_CALLBACK(Dialog, _up_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _prev_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _recent_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _current_callback);
#if ! defined(DJV_WINDOWS)
  DJV_FL_WIDGET_CALLBACK(Dialog, _home_callback);
#endif
  DJV_FL_WIDGET_CALLBACK(Dialog, _desktop_callback);
#if defined(DJV_WINDOWS)
  DJV_FL_WIDGET_CALLBACK(Dialog, _drive_callback);
#else
  DJV_FL_WIDGET_CALLBACK(Dialog, _root_callback);
#endif
  DJV_FL_WIDGET_CALLBACK(Dialog, _reload_callback);

  DJV_FL_WIDGET_CALLBACK(Dialog, _image_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _seq_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _type_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _hidden_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _sort_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _sort_reverse_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _sort_directory_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _navigate_callback);
  
  DJV_CALLBACK(Dialog, bookmark_del_callback, const List<int> &);
  DJV_CALLBACK(Dialog, bookmark_del_all_callback, bool);
  
  DJV_FL_WIDGET_CALLBACK(Dialog, _bookmark_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _bookmark_add_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _bookmark_del_callback);
  DJV_FL_WIDGET_CALLBACK(Dialog, _bookmark_del_all_callback);
  
  DJV_CALLBACK(Dialog, pin_callback, bool);
  DJV_CALLBACK(Dialog, ok_callback, bool);
  DJV_CALLBACK(Dialog, close_callback, bool);
  
  // Update.

  void directory_update();
  void widget_update();
  void type_update();
  void menu_update();
  
  // Variables.

  std::auto_ptr<Prefs_Instance> _prefs;
  
  File _value;
  String _directory_prev;
  
  IMAGE _image;
  file_seq::COMPRESS _seq;
  bool _hidden;
  SORT _sort;
  bool _sort_reverse;
  bool _sort_directory;
  int _type;
  List<String> _type_glob;
  List<String> _type_label;
  bool _pin;
  
  struct _Dialog;
  std::auto_ptr<_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_UI String & operator >> (String &, IMAGE &) throw (String);
_DJV_UI String & operator >> (String &, SORT &) throw (String);
_DJV_UI String & operator >> (String &, NAVIGATE &) throw (String);

_DJV_UI String & operator << (String &, IMAGE);
_DJV_UI String & operator << (String &, SORT);
_DJV_UI String & operator << (String &, NAVIGATE);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI const List<String> & label_image();
_DJV_UI const List<String> & label_sort();
_DJV_UI const List<String> & label_navigate();

_DJV_UI extern const String
  label_seq,
  label_type_all,
  label_type_all_image;

_DJV_UI extern const String
  dialog_title,
  dialog_bookmark_del,
  dialog_bookmark_del_all;

_DJV_UI extern const String
  menu_directory,
  menu_directory_up,
  menu_directory_prev,
  menu_directory_recent,
  menu_directory_current,
  menu_directory_home,
  menu_directory_desktop,
  menu_directory_drive,
  menu_directory_root,
  menu_directory_reload,
  menu_option,
  menu_option_image,
  menu_option_seq,
  menu_option_type,
  menu_option_hidden,
  menu_option_sort,
  menu_option_sort_reverse,
  menu_option_sort_directory,
  menu_option_navigate,
  menu_bookmark,
  menu_bookmark_add,
  menu_bookmark_del,
  menu_bookmark_del_all;

_DJV_UI extern const String
  tooltip_value,
  tooltip_up,
  tooltip_prev,
  tooltip_reload;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

_DJV_UI void set(
  Callback_Base *,
  callback::Signal<const File &>::Callback *
);

_DJV_UI void del(Callback_Base *);

//! Access to global file browser.

_DJV_UI Dialog * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

