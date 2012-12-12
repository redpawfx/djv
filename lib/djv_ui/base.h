/*------------------------------------------------------------------------------
 lib/djv_ui/base.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_BASE_H
#define DJV_UI_BASE_H

#include <djv_ui/export.h>

#include <djv_ui/base_def.h>

#include <djv_image/color.h>
#include <djv_base/vector.h>
#include <FL/Enumerations.H>

class Fl_Group;

//------------------------------------------------------------------------------
//!\namespace djv_ui
//
//! User-interface library.
//------------------------------------------------------------------------------

namespace djv_ui {
using namespace djv_base;

//------------------------------------------------------------------------------
//!\namespace djv_ui::base
//
//! Base user-interface functionality.
//------------------------------------------------------------------------------

namespace base {

//------------------------------------------------------------------------------
//!\class Cursor
//
//! Temporarily change the cursor.
//------------------------------------------------------------------------------

class _DJV_UI Cursor
{
public:

  Cursor(const Fl_Cursor &);
  
  ~Cursor();
};
  
//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Set the cursor.

_DJV_UI void cursor(const Fl_Cursor &);

//! Get the size of a character or string. You should first call fl_font() to
//! set the font appropriately.

_DJV_UI V2i char_size();
_DJV_UI int char_width();
_DJV_UI int char_width(char);

_DJV_UI V2i string_size(const String &);
_DJV_UI int string_width(const String &);
_DJV_UI int string_width(int char_size);

//@}
//------------------------------------------------------------------------------
//!\class Preferences
//
//! Base preferences.
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
  //!\name Tooltips
  //@{

  void tooltips(bool);
  
  bool tooltips() const;
  
  //@}
  
private:

  bool _tooltips;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  tooltip_reset;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class _DJV_UI Global
{
public:

  Global();
  
  ~Global();
  
  //! Tooltips.
  
  void tooltips(bool);
  
  bool tooltips() const;
  
private:

  void tooltips_update();
  
  bool _tooltips;
};

//! Access to global options.

_DJV_UI Global * global();

//! Access to preferences.

_DJV_UI Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}

#endif

