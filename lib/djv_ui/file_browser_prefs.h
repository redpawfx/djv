/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_FILE_BROWSER_PREFS_H
#define DJV_UI_FILE_BROWSER_PREFS_H

#include <djv_ui/file_browser_def.h>

#include <djv_ui/file_browser.h>
#include <djv_ui/prefs_def.h>
#include <djv_ui/shortcut.h>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! File browser preferences.
//------------------------------------------------------------------------------

class _DJV_UI Prefs
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();

  //@}
  //----------------------------------------------------------------------------
  //!\name Recent Directories
  //@{

  void recent(const List<String> &);
  void recent_add(const String &);
  
  const List<String> & recent() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Bookmarks
  //@{

  void bookmark(const List<String> &);
  void bookmark_add(const String &);
  
  const List<String> & bookmark() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Navigation
  //@{
  
  void navigate(NAVIGATE);
  
  NAVIGATE navigate() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Keyboard Shortcuts
  //@{
  
  enum SHORTCUT
  {
    UP,
    PREV,
    CURRENT,
    HOME,
    DESKTOP,
    ROOT,
    RELOAD,
    SEQ_OFF,
    SEQ_SPARSE,
    SEQ_RANGE,
    HIDDEN,
    SORT_NAME,
    SORT_SIZE,
#if ! defined(DJV_WINDOWS)
    SORT_USER,
#endif
    SORT_TIME,
    SORT_REVERSE,
    SORT_DIRECTORY,
    BOOKMARK_ADD,
    BOOKMARK_1,
    BOOKMARK_2,
    BOOKMARK_3,
    BOOKMARK_4,
    BOOKMARK_5,
    BOOKMARK_6,
    BOOKMARK_7,
    BOOKMARK_8,
    BOOKMARK_9,
    BOOKMARK_10,
    BOOKMARK_11,
    BOOKMARK_12,
    
    _SHORTCUT_SIZE
  };

  void shortcut(const List<shortcut::Shortcut> &);
  
  const List<shortcut::Shortcut> & shortcut() const;

  //@}
  
private:

  List<String> _recent;
  List<String> _bookmark;
  NAVIGATE _navigate;
  List<shortcut::Shortcut> _shortcut;
};

//------------------------------------------------------------------------------
//!\class Prefs_Instance
//
//! File browser per-instance preferences.
//------------------------------------------------------------------------------

class _DJV_UI Prefs_Instance
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs_Instance(prefs::Prefs *, const String & prefs_name);
  
  ~Prefs_Instance();

  //@}
  //----------------------------------------------------------------------------
  //!\name View Options
  //@{

  void image(IMAGE);
  void seq(file_seq::COMPRESS);
  void hidden(bool);
  
  IMAGE image() const;
  file_seq::COMPRESS seq() const;
  bool hidden() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Sorting Options
  //@{
  
  void sort(SORT);
  void sort_reverse(bool);
  void sort_directory(bool);
  
  SORT sort() const;
  bool sort_reverse() const;
  bool sort_directory() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Type Options
  //@{
  
  void type(int);
  
  int type() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Dialog
  //@{
  
  void pin(bool);
  
  bool pin() const;

  //@}
  
private:

  prefs::Prefs * _prefs;
  String _prefs_name;
  IMAGE _image;
  file_seq::COMPRESS _seq;
  bool _hidden;
  SORT _sort;
  bool _sort_reverse, _sort_directory;
  int _type;
  bool _pin;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! File browser preferences widget.
//------------------------------------------------------------------------------

class _DJV_UI Prefs_Widget : public widget::Widget
{
public:

  Prefs_Widget();

private:

  DJV_CALLBACK(Prefs_Widget, shortcut_callback,
    const List<shortcut::Shortcut> &);
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI const List<String> & label_shortcut();

_DJV_UI extern const String
  prefs_title;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

_DJV_UI Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}

#endif

