/*------------------------------------------------------------------------------
 bin/djv_view/window_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_WINDOW_PREFS_H
#define DJV_VIEW_WINDOW_PREFS_H

#include "window_base.h"
#include <djv_ui/widget.h>

namespace djv_bin {
namespace view {
namespace window {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! Main window preferences.
//------------------------------------------------------------------------------

class Prefs : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Resize
  //@{
  
  void resize_fit(bool);

  bool resize_fit() const;

  void resize_max(RESIZE_MAX);
  
  RESIZE_MAX resize_max() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Toolbar
  //@{
  
  void toolbar(const List<bool> &);
    
  const List<bool> & toolbar() const;
  
  djv_ui::callback::Signal<const List<bool> &> toolbar_signal;
  
  //@}
  
private:

  Box2i _geom;
  bool _resize_fit;
  RESIZE_MAX _resize_max;
  List<bool> _toolbar;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! Main window preferences widget.
//------------------------------------------------------------------------------

class Prefs_Widget : public djv_ui::Widget
{
public:

  Prefs_Widget();

private:

  DJV_CALLBACK(Prefs_Widget, resize_fit_callback, bool);
  DJV_CALLBACK(Prefs_Widget, resize_max_callback, int);
  DJV_CALLBACK(Prefs_Widget, toolbar_callback, const List<bool> &);
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  prefs_title,
  prefs_resize_group,
  prefs_resize_fit,
  prefs_resize_max,
  prefs_toolbar_group;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

