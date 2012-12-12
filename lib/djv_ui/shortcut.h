/*------------------------------------------------------------------------------
 lib/djv_ui/shortcut.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_SHORTCUT_H
#define DJV_UI_SHORTCUT_H

#include <djv_ui/shortcut_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::shortcut
//
//! Keyboard shortcuts.
//------------------------------------------------------------------------------

namespace shortcut {

//------------------------------------------------------------------------------
//!\class Shortcut
//
//! A keyboard shortcut.
//------------------------------------------------------------------------------

struct _DJV_UI Shortcut
{
  Shortcut(const String & name = String(), int value = 0) :
    name(name),
    value(value)
  {}
  
  String name;
  int value;
};

//------------------------------------------------------------------------------
//!\class Shortcut_Widget
//
//! A widget for configuring keyboard shorcuts.
//------------------------------------------------------------------------------

class _DJV_UI Shortcut_Widget : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Shortcut_Widget();
  
  ~Shortcut_Widget();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const List<Shortcut> & in);
  
  const List<Shortcut> & get() const;
  
  callback::Signal<const List<Shortcut> &> signal;
  
  //@}
  
private:

  DJV_CALLBACK(Shortcut_Widget, widget_callback, const void *);

  void widget_update();
  void value_update();

  List<Shortcut> _list;
  
  struct _Shortcut_Widget;
  std::auto_ptr<_Shortcut_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_UI bool operator == (const Shortcut &, const Shortcut &);

_DJV_UI bool operator != (const Shortcut &, const Shortcut &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_UI bool serialize(String *, int *);

_DJV_UI String serialize(int);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI String label(int);

_DJV_UI const List<String> & label_key();
_DJV_UI const List<String> & label_mod();
_DJV_UI const List<String> & label_shortcut();

_DJV_UI extern const String
  tooltip_button;

_DJV_UI extern const String
  prefs_title;

//@}
//------------------------------------------------------------------------------

}
}

#endif
