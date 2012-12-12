/*------------------------------------------------------------------------------
 lib/djv_ui/prefs.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_PREFS_H
#define DJV_UI_PREFS_H

#include <djv_ui/prefs_def.h>

#include <djv_image/time.h>
#include <djv_ui/base.h>

class Fl_Preferences;

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::prefs
//
//! Preferences.
//------------------------------------------------------------------------------

namespace prefs {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! Preferences.
//------------------------------------------------------------------------------

class _DJV_UI Prefs
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs(const String & name);
  Prefs(const Prefs &, const String & group);
  Prefs(Prefs *, const String & group);
  
  ~Prefs();

  //@}
  //----------------------------------------------------------------------------
  //!\name Information
  //@{
  
  const String & name() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const String &, const String &);
  
  void del(const String &);
  void del_group(const String &);
  
  bool get(const String &, String *) const;
  
  List<String> list() const;
  List<String> group() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Location
  //@{
  
  String file_name() const;
  String file_path() const;

  static String file_base();
  
  //@}

private:

  String _name;
  
  std::auto_ptr<Fl_Preferences> _prefs;
};

//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

template<typename T>
inline void set(Prefs *, const String &, const T &);

template<typename T>
inline bool get(Prefs *, const String &, T *);

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

_DJV_UI Prefs * prefs();
_DJV_UI Prefs * prefs_global();

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_ui/prefs_inline.h>
#endif

