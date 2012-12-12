/*------------------------------------------------------------------------------
 lib/djv_ui/layout.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#undef DJV_DEBUG
//#define DJV_DEBUG

#include "layout.h"

#include <djv_ui/style.h>
#include <djv_ui/widget.h>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
//#include <algorithm>
#include <stdio.h>

namespace djv_ui {
namespace layout {

//------------------------------------------------------------------------------
// Layout
//------------------------------------------------------------------------------

Layout::Layout(Layout_Item * parent) :
  Layout_Item(layout_item::LAYOUT),
  _spacing(-1),
  _margin(-1)
{
  debug_path("Layout");
  
  //DJV_DEBUG("Layout::Layout");
  
  align(layout_item::CENTER, layout_item::CENTER);
  
  // XXX Parent.

  if (parent)
  {
    if (layout_item::WIDGET == parent->item())
      static_cast<Widget_Base *>(parent)->layout(this);
    else if (layout_item::LAYOUT == parent->item())
      static_cast<Layout *>(parent)->add(this);
    this->parent(parent);
  }
}

Layout::~Layout()
{
  //DJV_DEBUG("Layout::~Layout");
  //DJV_DEBUG_PRINT("item = " << item());
  
  del();
}

void Layout::add(Layout_Item * in)
{
  if (! in)
    return;
  
  insert(in, static_cast<int>(_list.size()));
}

void Layout::insert(Layout_Item * in, int index)
{
  if (! in)
    return;
  
  DJV_DEBUG("Layout::insert");
  DJV_DEBUG_PRINT(layout_item::debug_path(this) << "/" <<
    layout_item::debug_path(in));
  DJV_DEBUG_PRINT("index = " << index);
  //DJV_DEBUG_PRINT("item = " << in->item());
  
  List<Layout_Item *>::iterator i = _list.begin();
  int j = 0;
  for (; j < index; ++j, ++i)
    ;
  _list.insert(i, in);
  
  // XXX Reparent item.

  if (in->parent())
  {
    if (layout_item::WIDGET == in->parent()->item())
    {
      if (layout_item::LAYOUT == in->item())
        static_cast<Widget_Base *>(in->parent())->layout(0);
    }
    else if (layout_item::LAYOUT == in->parent()->item())
    {
      static_cast<Layout *>(in->parent())->remove(in);
    }
  }
  
  if (Fl_Widget * widget = dynamic_cast<Fl_Widget *>(in))
  {
    if (widget->parent())
    {
      DJV_DEBUG_PRINT("fltk remove");
      
      widget->parent()->remove(widget);
      widget->parent(0);
    }
    
    Layout_Item * parent = this;
    
    while (layout_item::WIDGET != parent->item())
    {
      parent = parent->parent();
      
      if (! parent)
        break;
    }
    
    if (Fl_Group * group = dynamic_cast<Fl_Group *>(parent))
    {
      DJV_DEBUG_PRINT("fltk add = " << layout_item::debug_path(parent));
      
      // XXX
      
      if (0 == j)
        group->insert(*widget, j);
      else
        group->add(*widget);
    }
  }
  
  in->parent(this);

#if defined(DJV_DEBUG)
  //debug();
#endif
}

Layout_Item * Layout::remove(int in)
{
  DJV_DEBUG("Layout::remove");
  DJV_DEBUG_PRINT("in = " << in);
  
  List<Layout_Item *>::iterator i = _list.begin();
  for (int j = 0; j < in; ++j, ++i)
    ;
  Layout_Item * item = *i;
  _list.erase(i);

  DJV_DEBUG_PRINT("item = " << layout_item::debug_path(item));
  
  // XXX

  if (Fl_Widget * widget = dynamic_cast<Fl_Widget *>(item))
    if (widget->parent())
    {
      DJV_DEBUG_PRINT("fltk remove");
      
      widget->parent()->remove(widget);
      widget->parent(0);
    }
  
  item->parent(0);
  
  return item;
}

Layout_Item * Layout::remove(Layout_Item * in)
{
  Layout_Item * out = 0;
  
  List<Layout_Item *>::iterator i = _list.begin();
  int j = 0;
  for (
    const List<Layout_Item *>::const_iterator end = _list.end();
    i != end;
    ++i, ++j
  )
    if (in == *i)
    {
      out = remove(j);
      break;
    }
  
  return out;
}

Layout_Item * Layout::remove_child(Layout_Item * in)
{
  Layout_Item * out = remove(in);
  
  if (! out)
  {
    List<Layout_Item *>::iterator i = _list.begin();
    
    for (
      List<Layout_Item *>::const_iterator end = _list.end();
      i != end;
      ++i
    )
      if (Layout * layout = dynamic_cast<Layout *>(*i))
        if (out = layout->remove_child(in))
          break;
  }
  
  return out;
}

void Layout::del(int in)
{
  DJV_DEBUG("Layout::del");
  DJV_DEBUG_PRINT("in = " << in);
  
  Layout_Item * item = remove(in);
  
  // XXX

  if (layout_item::WIDGET == item->item())
    static_cast<Widget_Base *>(item)->del();
  else
    delete item;
}

void Layout::del(Layout_Item * in)
{
  for (list::size_type i = 0; i < _list.size(); ++i)
    if (in == _list[i])
    {
      del(static_cast<int>(i));
      break;
    }
}

void Layout::del()
{
  while (_list.size())
    del(0);
}

void Layout::spacing(int in)
{
  _spacing = in;
}
  
int Layout::spacing() const
{
  return _spacing != -1 ? _spacing : style::global()->spacing();
}

void Layout::margin(int in)
{
  _margin = in;
}

int Layout::margin() const
{
  return _margin != -1 ? _margin : style::global()->margin();
}

void Layout::dirty()
{
  List<Layout_Item *>::iterator i = _list.begin();
  
  for (
    List<Layout_Item *>::const_iterator end = _list.end();
    i != end;
    ++i
  )
    (*i)->dirty();
}

int Layout::visible() const
{
  for (
    List<Layout_Item *>::const_iterator i = _list.begin(),
      end = _list.end();
    i != end;
    ++i
  )
    if (layout_item::SPACER != (*i)->item())
      if ((*i)->visible()) return 1;
  
  return 0;
}

namespace {

void _debug(const Layout_Item * in, int indent)
{
  const Box2i & geom = in->geom();
  
  const String tmp = string::Format("%%%% %%,%%,%%x%%\n").
    arg(String(indent, ' ')).
    arg(layout_item::debug_path(in)).
    arg(geom.x).arg(geom.y).arg(geom.w).arg(geom.h);
  
  printf(tmp.c_str());
}

}

void Layout::debug(int indent)
{
//#if defined(DJV_DEBUG)
  _debug(this, indent);
  
  for (list::size_type i = 0; i < _list.size(); ++i)
  {
    if (Widget_Base * widget = dynamic_cast<Widget_Base *>(_list[i]))
    {
      _debug(widget, indent + 2);
      if (widget->layout()) widget->layout()->debug(indent + 2);
    }
    if (Layout * layout = dynamic_cast<Layout *>(_list[i]))
      layout->debug(indent + 2);
  }
//#endif
}

//------------------------------------------------------------------------------

}
}

