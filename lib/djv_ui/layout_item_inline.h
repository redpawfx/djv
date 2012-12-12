/*------------------------------------------------------------------------------
 lib/djv_ui/layout_item_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_ui {
namespace layout_item {

//------------------------------------------------------------------------------
// Layout_Item
//------------------------------------------------------------------------------

inline ITEM Layout_Item::item() const
{
  return _item;
}

inline const Box2i & Layout_Item::geom() const
{
  return _geom;
}

inline const V2i & Layout_Item::size_hint() const
{
  return _size_hint;
}

inline const V2i & Layout_Item::size_min() const
{
  return _size_hint;
}

inline int Layout_Item::visible() const
{
  return true;
}

inline const SIZE * Layout_Item::size() const
{
  return _size;
}

inline const ALIGN * Layout_Item::align() const
{
  return _align;
}
  
inline const List<Layout_Item *> & Layout_Item::group() const
{
  return _group;
}

inline Layout_Item * Layout_Item::group_align()
{
  return this;
}

inline Layout_Item * Layout_Item::label_align()
{
  return 0;
}

inline Layout_Item * Layout_Item::parent() const
{
  return _parent;
}

//------------------------------------------------------------------------------

}
}
