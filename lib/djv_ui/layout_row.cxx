/*------------------------------------------------------------------------------
 lib/djv_ui/layout_row.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "layout_row.h"

#include <djv_ui/style.h>

namespace djv_ui {
namespace layout_row {

//------------------------------------------------------------------------------
// _Stretch
//------------------------------------------------------------------------------

namespace {

class _Stretch : public Layout_Spacer
{};

}

//------------------------------------------------------------------------------
// Layout_Row_Base
//------------------------------------------------------------------------------

Layout_Row_Base::Layout_Row_Base(Layout_Item * parent) :
  Layout(parent),
  _stretch(-1)
{}

Layout_Row_Base::~Layout_Row_Base()
{}

Layout_Item * Layout_Row_Base::add_spacer(const V2i & in, bool stretch)
{
  Layout_Item * item = new layout_item::Layout_Spacer(in);
  
  add(item);
  
  if (stretch)
    this->stretch(item);

  return item;
}

Layout_Item * Layout_Row_Base::add_stretch(bool stretch)
{
  Layout_Item * item = new _Stretch;
  
  add(item);
  
  if (stretch)
    this->stretch(item);

  return item;
}

void Layout_Row_Base::stretch(int in)
{
  _stretch = in;
}

void Layout_Row_Base::stretch(const Layout_Item * in)
{
  const List<Layout_Item *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    if (in == list[i])
    {
      stretch(static_cast<int>(i));
      break;
    }
}

int Layout_Row_Base::stretch() const
{
  return _stretch;
}

void Layout_Row_Base::layout(
  const List<Layout_Item *> & in,
  List<Box2i> * out,
  const V2i & size,
  DIRECTION direction,
  int spacing,
  int stretch
) {
  DJV_DEBUG("Layout_Row_Base::layout");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("direction = " << direction);
  DJV_DEBUG_PRINT("stretch = " << stretch);
  
  const list::size_type list_size = in.size();
  out->resize(list_size);
  
  // Calculate minimum sizes.

  int total_length = 0;
  int stretch_count = 0;
  const Layout_Item * prev = 0;
  
  List<Layout_Item *>::const_iterator i, end = in.end();
  List<Box2i>::iterator j;
  int k;

  for (i = in.begin(), j = out->begin(), k = 0; i != end; ++i, ++j, ++k)
  {
    const Layout_Item * item = *i;
    
    if (item->visible())
    {
      j->position.zero();
      j->size = item->size_min();
      
      const List<Layout_Item *> & item_group = item->group();
      List<Layout_Item *>::const_iterator _i = item_group.begin();
      for (
        const List<Layout_Item *>::const_iterator _end = item_group.end();
        _i != _end;
        ++_i
      )
        if ((*_i) != item && (*_i)->visible())
          vector::max((*_i)->size_min(), j->size, &j->size);

      total_length += j->size.e[direction];

      if (item->item() != layout_item::SPACER && prev)
        if (prev->item() != layout_item::SPACER)
          total_length += spacing;

      if (layout_item::STRETCH == item->size()[direction] || k == stretch)
        ++stretch_count;

      prev = item;
    }
    else
    {
      j->zero();
    }

    DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j <<
      " group = " << item->group().size());
  }
  
  DJV_DEBUG_PRINT("total length = " << total_length);
  
  // Expand to size hints.
  
  for (i = in.begin(), j = out->begin();
    i != end && (size.e[direction] - total_length) > 0; ++i, ++j)
  {
    const Layout_Item * item = *i;
    if (item->visible())
    {
      const int tmp = j->size.e[direction];
      
      j->size.e[direction] = math::min(
        math::max(item->size_hint().e[direction], j->size.e[direction]),
        tmp + size.e[direction] - total_length);

      total_length += j->size.e[direction] - tmp;

      DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);
    }
  }
  DJV_DEBUG_PRINT("total size = " << total_length);
  
  const int stretch_size = math::max(size.e[direction] - total_length, 0);
  
  DJV_DEBUG_PRINT("stretch count = " << stretch_count);
  DJV_DEBUG_PRINT("stretch size = " << stretch_size);
  
  // Layout items.

  prev = 0;
  int position = 0;
  for (i = in.begin(), j = out->begin(), k = 0; i != end; ++i, ++j, ++k)
  {
    const Layout_Item * item = *i;
    
    if (item->visible())
    {
      if (item->item() != layout_item::SPACER && prev)
        if (prev->item() != layout_item::SPACER)
          position += spacing;
      
      if (layout_item::STRETCH == item->size()[direction] || k == stretch)
      {
        int tmp = 0;
        if (k == stretch)
          tmp = stretch_size;
        else if (-1 == stretch && stretch_count)
          tmp = stretch_size / stretch_count;
        j->size.e[direction] += tmp;
      }

      prev = item;
    }
    
    j->position.e[direction] = position;
    position += j->size.e[direction];
    
    DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);
  }
}

//------------------------------------------------------------------------------
// Layout_Row_Horizontal
//------------------------------------------------------------------------------

Layout_Row_Horizontal::Layout_Row_Horizontal(Layout_Item * parent) :
  Layout_Row_Base(parent)
{
  debug_path("Layout_Row_Horizontal");
  
  //size(STRETCH, FIXED);
}

void Layout_Row_Horizontal::add_spacer(int in, bool stretch)
{
  Layout_Row_Base::add_spacer(V2i(in, 0), stretch);
}

void Layout_Row_Horizontal::dirty()
{
  DJV_DEBUG("Layout_Row_Horizontal::dirty");
  
  Layout_Row_Base::dirty();

  layout(list(), &_list, V2i(), HORIZONTAL, spacing(), stretch());
  
  int w = 0, h = 0;
  if (_list.size())
    w = _list[_list.size() - 1].lower_right().x + 1 + margin() * 2;
  for (list::size_type i = 0; i < _list.size(); ++i)
    h = math::max(h, _list[i].h);

  _geom_prev.zero();
  
  size_hint(V2i(w, h + margin() * 2));
}

void Layout_Row_Horizontal::geom(const Box2i & in)
{
  if (in == _geom_prev)
    return;
  
  _geom_prev = in;

  DJV_DEBUG("Layout_Row_Horizontal::geom");
  DJV_DEBUG_PRINT("in = " << in);

  Layout_Row_Base::geom(in);

  const List<Layout_Item *> & list = this->list();
  const int margin = this->margin();
  const int margin2 = margin * 2;
  
  layout(list, &_list, in.size - margin2, HORIZONTAL, spacing(), stretch());
    
  Box2i geom;
  List<Layout_Item *>::const_iterator i = list.begin();
  List<Box2i>::const_iterator j = _list.begin();
  for (
    const List<Layout_Item *>::const_iterator end = list.end();
    i != end;
    ++i, ++j
  ) {
    Layout_Item * item = *i;
    
    DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);
    
    int _y = j->y, _h = j->h;
    if (layout_item::STRETCH == item->size()[VERTICAL])
      _h = math::max(in.h - margin2, _h);
    else
      switch (item->align()[VERTICAL])
      {
        case layout_item::LEFT: break;
        case layout_item::CENTER: _y += ((in.h - margin2) - _h) / 2; break;
        case layout_item::RIGHT: break;
      }
    geom.x = in.x + margin + j->x;
    geom.y = in.y + margin + _y;
    geom.w = j->w;
    geom.h = _h;
    item->geom(geom);
  }
}

const Box2i & Layout_Row_Horizontal::geom() const
{
  return Layout_Row_Base::geom();
}

//------------------------------------------------------------------------------
// Layout_Row_Vertical
//------------------------------------------------------------------------------

Layout_Row_Vertical::Layout_Row_Vertical(Layout_Item * parent) :
  Layout_Row_Base(parent)
{
  debug_path("Layout_Row_Vertical");
  
  //size(FIXED, STRETCH);
}

void Layout_Row_Vertical::add_spacer(int in, bool stretch)
{
  Layout_Row_Base::add_spacer(V2i(0, in), stretch);
}

void Layout_Row_Vertical::dirty()
{
  DJV_DEBUG("Layout_Row_Vertical::dirty");
  
  Layout_Row_Base::dirty();

  const List<Layout_Item *> & list = this->list();
  layout(list, &_list, V2i(), VERTICAL, spacing(), stretch());
  
  int w = 0, h = 0;
  for (list::size_type i = 0; i < _list.size(); ++i)
    w = math::max(w, _list[i].w);
  if (_list.size())
    h = _list[_list.size() - 1].lower_right().y + 1 + margin() * 2;
  
  _geom_prev.zero();
  
  size_hint(V2i(w + margin() * 2, h));
}

void Layout_Row_Vertical::geom(const Box2i & in)
{
  if (in == _geom_prev)
    return;
  
  _geom_prev = in;
  
  DJV_DEBUG("Layout_Row_Vertical::geom");
  DJV_DEBUG_PRINT("in = " << in);

  Layout_Row_Base::geom(in);
  
  const List<Layout_Item *> & list = this->list();
  const int margin = this->margin();
  const int margin2 = margin * 2;
  
  layout(list, &_list, in.size - margin2, VERTICAL, spacing(), stretch());
  
  Box2i geom;
  List<Layout_Item *>::const_iterator i = list.begin();
  List<Box2i>::const_iterator j = _list.begin();
  for (
    const List<Layout_Item *>::const_iterator end = list.end();
    i != end;
    ++i, ++j
  ) {
    Layout_Item * item = *i;
    
    DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);
    
    int _x = j->x, _w = j->w;
    if (layout_item::STRETCH == item->size()[HORIZONTAL])
      _w = math::max(in.w - margin2, _w);
    else
      switch (item->align()[HORIZONTAL])
      {
        case layout_item::LEFT: break;
        case layout_item::CENTER: _x += ((in.w - margin2) - _w) / 2; break;
        case layout_item::RIGHT: break;
      }
    geom.x = in.x + margin + _x;
    geom.y = in.y + margin + j->y;
    geom.w = _w;
    geom.h = j->h;
    item->geom(geom);
  }
}

const Box2i & Layout_Row_Vertical::geom() const
{
  return Layout_Row_Base::geom();
}

//------------------------------------------------------------------------------

}
}

