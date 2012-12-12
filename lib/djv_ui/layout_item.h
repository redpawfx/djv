/*------------------------------------------------------------------------------
 lib/djv_ui/layout_item.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_LAYOUT_ITEM_H
#define DJV_UI_LAYOUT_ITEM_H

#include <djv_ui/layout_item_def.h>

#include <djv_base/box.h>
#include <djv_ui/base.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::layout_item
//
//! Layout items.
//------------------------------------------------------------------------------

namespace layout_item {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Item type.

enum ITEM
{
  WIDGET,
  LAYOUT,
  SPACER
};

//! Item size.

enum SIZE
{
  FIXED,
  STRETCH
};

//! Item alignment.

enum ALIGN
{
  LEFT,
  CENTER,
  RIGHT
};

//@}
//------------------------------------------------------------------------------
//!\class Layout_Item
//
//! An item in a layout.
//------------------------------------------------------------------------------

class _DJV_UI Layout_Item
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Layout_Item(ITEM);
  
  virtual ~Layout_Item() = 0;

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  inline ITEM item() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual void dirty();
  virtual void update();
  virtual void geom(const Box2i &);
  
  inline virtual const Box2i & geom() const;
  inline virtual const V2i & size_hint() const;
  inline virtual const V2i & size_min() const;
  inline virtual int visible() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Size
  //@{

  virtual void size(SIZE, SIZE);
  virtual void align(ALIGN, ALIGN);
  
  inline const SIZE * size() const;
  inline const ALIGN * align() const;
  
  // XXX Cheesy way to align items.

  void group(const List<Layout_Item *> &);
  
  inline const List<Layout_Item *> & group() const;
  inline virtual Layout_Item * group_align();
  inline virtual Layout_Item * label_align();

  //@}
  //----------------------------------------------------------------------------
  //!\name Parent
  //@{

  virtual void parent(Layout_Item *);
  
  inline Layout_Item * parent() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Debugging
  //@{
  
  void debug_path(const String &);

  String debug_path() const;

  //@}
  
protected:

  void size_hint(const V2i &);

private:

  ITEM _item;
  Box2i _geom;
  V2i _size_hint;
  V2i _size_min;
  SIZE _size [2];
  ALIGN _align [2];
  List<Layout_Item *> _group;
  Layout_Item * _parent;
  String _debug_path;
};

//------------------------------------------------------------------------------
//!\class Layout_Spacer
//
//! A layout spacer item.
//------------------------------------------------------------------------------

class _DJV_UI Layout_Spacer : public Layout_Item
{
public:

  Layout_Spacer(const V2i & size = V2i());

  void spacer_size(const V2i &);
  
  const V2i & spacer_size() const;

  virtual void dirty();
  
private:

  V2i _spacer_size;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_UI void group(const List<Layout_Item *> &);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

String debug_path(const Layout_Item *);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_ui/layout_item_inline.h>
#endif

