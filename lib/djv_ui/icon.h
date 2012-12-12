/*------------------------------------------------------------------------------
 lib/djv_ui/icon.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_ICON_H
#define DJV_UI_ICON_H

#include <djv_ui/icon_def.h>

#include <djv_ui/base.h>
#include <FL/Fl_Image.H>
#include <map>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::icon
//
//! Icons.
//------------------------------------------------------------------------------

namespace icon {

//------------------------------------------------------------------------------
//!\class Factory
//
//! Icon factory.
//------------------------------------------------------------------------------

class _DJV_UI Factory
{
public:

  Factory();
  
  ~Factory();

  void add(const String & name, Fl_Image *);
  
  // XXX FLTK should return a const pointer.
  
  Fl_Image * get(const String & name) const;

private:

  typedef std::map<String, Fl_Image *> List;
  
  List _list;
  Fl_Image _empty;
};

//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global icon factory.

_DJV_UI Factory * global();

_DJV_UI Fl_Image * get(const String & name);

//@}
//------------------------------------------------------------------------------

}
}

#endif

