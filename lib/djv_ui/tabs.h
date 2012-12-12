/*------------------------------------------------------------------------------
 lib/djv_ui/tabs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_TABS_H
#define DJV_UI_TABS_H

#include <djv_ui/tabs_def.h>

#include <djv_ui/widget.h>
#include <FL/Fl_Tabs.H>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::tabs
//
//! Tab widgets.
//------------------------------------------------------------------------------

namespace tabs {

//------------------------------------------------------------------------------
//!\class Tabs
//
//! Tabs widget.
//------------------------------------------------------------------------------

class _DJV_UI Tabs : public Fl_Tabs, public Widget_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Tabs();
  
  virtual ~Tabs();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(int);
  
  int get() const;
  
  callback::Signal<int> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Tabs
  //@{
  
  void tab_add(const String & name, Widget_Base *);
  void tab_insert(const String & name, Widget_Base *, int = 0);
  void tab_name(const String & name, int tab);
  void tab_del(int tab);
  void tab_del();
  
  int tab_size() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual int handle(int);
  virtual void show();
  virtual void draw();

  virtual const Fl_Widget * fltk_widget() const { return this; }

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void geom(const Box2i &);
  virtual void resize(int, int, int, int);
  
  virtual const Box2i & geom() const;
  virtual int visible() const;

  //@}
  
private:

  DJV_FL_WIDGET_CALLBACK(Tabs, widget_callback);

  List<Widget_Base *> _list; // XXX Should be layout item.
  V2i _size_tab;
};

//------------------------------------------------------------------------------

}
}

#endif

