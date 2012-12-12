/*------------------------------------------------------------------------------
 lib/djv_ui/layout.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_LAYOUT_H
#define DJV_UI_LAYOUT_H

#include <djv_ui/layout_def.h>

#include <djv_ui/layout_item.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::layout
//
//! Layouts.
//------------------------------------------------------------------------------

namespace layout {

//------------------------------------------------------------------------------
//!\class Layout
//
//! A layout.
//------------------------------------------------------------------------------

class _DJV_UI Layout : public layout_item::Layout_Item
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Layout(Layout_Item * parent = 0);
  
  virtual ~Layout() = 0;

  //@}
  //----------------------------------------------------------------------------
  //!\name Items
  //@{
  
  virtual void add(Layout_Item *);
  virtual void insert(Layout_Item *, int);
  virtual Layout_Item * remove(int);
  virtual Layout_Item * remove(Layout_Item *);
  virtual Layout_Item * remove_child(Layout_Item *);
  virtual void del(int);
  virtual void del(Layout_Item *);
  virtual void del();
  
  inline const List<Layout_Item *> & list() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  void spacing(int);
  void margin(int);
  
  int spacing() const;
  int margin() const;

  virtual void dirty();
  
  virtual int visible() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Debugging
  //@{
  
  void debug(int indent = 0);

  //@}
  
private:

  List<Layout_Item *> _list;
  
  int _spacing;
  int _margin;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_UI int window_handle_size();

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_ui/layout_inline.h>
#endif

