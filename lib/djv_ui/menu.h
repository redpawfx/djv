/*------------------------------------------------------------------------------
 lib/djv_ui/menu.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_MENU_H
#define DJV_UI_MENU_H

#include <djv_ui/menu_def.h>

#include <djv_ui/frame.h>
#include <FL/Fl_Menu_.H>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::menu
//
//! Menus.
//------------------------------------------------------------------------------

namespace menu {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

static const int item_label_max_size = 100;

enum
{
  INACTIVE = 2 << 0,
  TOGGLE   = 2 << 1,
  RADIO    = 2 << 2,
  VALUE    = 2 << 3,
  SUB_MENU = 2 << 4,
  DIVIDER  = 2 << 5
};

//@}
//------------------------------------------------------------------------------
//!\class Item
//
//! A menu item.
//------------------------------------------------------------------------------

struct _DJV_UI Item
{
  Item();
  Item(
    const String & label,
    int shortcut,
    Fl_Callback * callback,
    void * data,
    int flags
  );
  
  String label;
  memory::Buffer<char> cstr;
  int shortcut;
  Fl_Callback * callback;
  void * data;
  int flags;
};

//------------------------------------------------------------------------------
//!\class Group
//
//! A group of menu items.
//------------------------------------------------------------------------------

class _DJV_UI Group
{
public:

  Group();
  Group(const Group &);
  
  ~Group();
  
  Group & operator = (const Group &);

  void set(const Group &);
  
  int add(
    const String & label,
    int shortcut = 0,
    Fl_Callback * = 0,
    void * = 0,
    int flags = 0,
    bool value = false
  );

  List<int> add(
    const List<String> & label,
    const List<int> & shortcut = List<int>(),
    Fl_Callback * = 0,
    void * = 0,
    int flags = 0,
    int value = -1
  );
  
  void end();
  
  void del();
  
  const List<Item *> & get() const;
  
  void fl(memory::Buffer<Fl_Menu_Item> *) const;

private:

  List<Item *> _list;
};

//------------------------------------------------------------------------------
//!\class Menu
//
//! A menu widget.
//------------------------------------------------------------------------------

class _DJV_UI Menu : public Frame
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Menu();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(const menu::Group &);
  
  const menu::Group & get() const;
  
  //! Current selection.
  
  int item() const;

  //! Current selection toggle value.
  
  bool value() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  int test_shortcut();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;

  //@}
  
private:

  class Widget;
  
  menu::Group _group;
  Widget * _widget;
  bool _test_shortcut;
};

//------------------------------------------------------------------------------
//!\class Popup
//
//! A popup-menu widget.
//------------------------------------------------------------------------------

class _DJV_UI Popup : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Popup(const List<String> & = List<String>());
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const List<String> &);
  void set(const menu::Group &);
  
  const menu::Group & get() const;
  int value() const;
  
  callback::Signal<int> signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void draw();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;
  
  //@}
  
private:

  void down(bool);
  void select(bool);
  
  DJV_FL_WIDGET_CALLBACK(Popup, widget_callback);

  menu::Group _group;
  
  class Widget;
  Widget * _widget;
  friend class Widget;
  
  Fl_Image * _icon;
  
  bool _down;
  bool _select;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI String label_list(const String &, int index);

_DJV_UI extern const String
  menu_messages,
  menu_prefs,
  menu_info,
  menu_help,
  menu_about,
  menu_exit;

//@}
//------------------------------------------------------------------------------

}
}

#endif
