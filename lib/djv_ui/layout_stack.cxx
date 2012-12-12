/*------------------------------------------------------------------------------
 lib/djv_ui/layout_stack.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "layout_stack.h"

namespace djv_ui {
namespace layout_stack {

//------------------------------------------------------------------------------
// Layout_Stack
//------------------------------------------------------------------------------

Layout_Stack::Layout_Stack(Layout_Item * parent) :
  Layout(parent)
{}

Layout_Stack::~Layout_Stack()
{}

void Layout_Stack::dirty()
{
  DJV_DEBUG("Layout_Stack::dirty");
  
  Layout::dirty();

  V2i size;
  const List<Layout_Item *> & list = this->list();
  for (list::size_type i = 0; i < list.size(); ++i)
    size = vector::max(list[i]->size_hint(), size);
  
  size_hint(size + margin() * 2);
}

void Layout_Stack::geom(const Box2i & in)
{
  DJV_DEBUG("Layout_Stack::geom");
  DJV_DEBUG_PRINT("in = " << in);

  Layout::geom(in);
  
  const List<Layout_Item *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    list[i]->geom(Box2i(
      in.x + margin(),
      in.y + margin(),
      in.w - margin() * 2,
      in.h - margin() * 2)
    );
}

const Box2i & Layout_Stack::geom() const
{
  return Layout::geom();
}

//------------------------------------------------------------------------------

}
}

