/*------------------------------------------------------------------------------
 lib/djv_ui/layout_row.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_LAYOUT_ROW_H
#define DJV_UI_LAYOUT_ROW_H

#include <djv_ui/layout_row_def.h>

#include <djv_ui/layout.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::layout_row
//
//! Row layouts.
//------------------------------------------------------------------------------

namespace layout_row {

//------------------------------------------------------------------------------
//!\class Layout_Row_Base
//
//! A base class for row layouts.
//------------------------------------------------------------------------------

class _DJV_UI Layout_Row_Base : public layout::Layout
{
public:

  Layout_Row_Base(Layout_Item * parent = 0);
  
  virtual ~Layout_Row_Base() = 0;

  Layout_Item * add_spacer(
    const V2i & = V2i(-1),
    bool stretch = false
  );
  
  Layout_Item * add_stretch(bool stretch = true);
  
  void stretch(int);
  void stretch(const Layout_Item *);
  
  int stretch() const;

protected:

  enum DIRECTION
  {
    HORIZONTAL,
    VERTICAL
  };

  static void layout(
    const List<Layout_Item *> &,
    List<Box2i> *,
    const V2i & size,
    DIRECTION,
    int spacing,
    int stretch
  );

private:

  int _stretch;
};

//------------------------------------------------------------------------------
//!\class Layout_Row_Horizontal
//
//! This layout arranges it's items in a horizontal row.
//------------------------------------------------------------------------------

class _DJV_UI Layout_Row_Horizontal : public Layout_Row_Base
{
public:

  Layout_Row_Horizontal(Layout_Item * parent = 0);
  
  void add_spacer(int = -1, bool stretch = false);
  
  void dirty();
  void geom(const Box2i &);
  
  const Box2i & geom() const;
  
private:

  Box2i _geom_prev;
  List<Box2i> _list;
};

//------------------------------------------------------------------------------
//!\class Layout_Row_Vertical
//
//! This layout arranges it's items in a vertical row.
//------------------------------------------------------------------------------

class _DJV_UI Layout_Row_Vertical : public Layout_Row_Base
{
public:

  Layout_Row_Vertical(Layout_Item * parent = 0);
  
  void add_spacer(int = -1, bool stretch = false);
  
  void dirty();
  void geom(const Box2i &);
  
  const Box2i & geom() const;
  
private:

  Box2i _geom_prev;
  List<Box2i> _list;
};

}

//------------------------------------------------------------------------------

}

#endif
