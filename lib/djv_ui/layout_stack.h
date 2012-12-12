/*------------------------------------------------------------------------------
 lib/djv_ui/layout_stack.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_LAYOUT_STACK_H
#define DJV_UI_LAYOUT_STACK_H

#include <djv_ui/layout_stack_def.h>

#include <djv_ui/layout.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::layout_stack
//
//! Stack layout.
//------------------------------------------------------------------------------

namespace layout_stack {

//------------------------------------------------------------------------------
//!\class Layout_Stack
//
//! This layout arranges it's items in a stack.
//------------------------------------------------------------------------------

class _DJV_UI Layout_Stack : public layout::Layout
{
public:

  Layout_Stack(Layout_Item * parent = 0);
  
  virtual ~Layout_Stack();
  
  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;
};

//------------------------------------------------------------------------------

}
}

#endif

