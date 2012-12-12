/*------------------------------------------------------------------------------
 lib/djv_ui/layout_item.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "layout.h"

#include <djv_ui/style.h>
#include <algorithm>

namespace djv_ui {
namespace layout_item {

//------------------------------------------------------------------------------
// Layout_Item
//------------------------------------------------------------------------------

Layout_Item::Layout_Item(ITEM item) :
  _item(item),
  _parent(0),
  _debug_path("Layout_Item")
{
  _size[0] = _size[1] = STRETCH;
  _align[0] = _align[1] = CENTER;
}

Layout_Item::~Layout_Item()
{}

void Layout_Item::dirty()
{}

void Layout_Item::update()
{
  if (_parent)
    _parent->update();
}

void Layout_Item::geom(const Box2i & in)
{
  _geom = in;
}

void Layout_Item::size_hint(const V2i & in)
{
  _size_hint = in;
}

void Layout_Item::size(SIZE h, SIZE v)
{
  _size[0] = h;
  _size[1] = v;
}

void Layout_Item::align(ALIGN h, ALIGN v)
{
  _align[0] = h;
  _align[1] = v;
}

void Layout_Item::group(const List<Layout_Item *> & in)
{
  _group = in;
}

void Layout_Item::parent(Layout_Item * in)
{
  _parent = in;
}

void Layout_Item::debug_path(const String & in)
{
  _debug_path = in;
}

String Layout_Item::debug_path() const
{
  return _debug_path;
}

//------------------------------------------------------------------------------
// Layout_Spacer
//------------------------------------------------------------------------------

Layout_Spacer::Layout_Spacer(const V2i & size) :
  Layout_Item(SPACER),
  _spacer_size(size)
{
  this->size(FIXED, FIXED);
}

void Layout_Spacer::spacer_size(const V2i & in)
{
  if (in == _spacer_size)
    return;
  
  _spacer_size = in;
  
  dirty();
}

const V2i & Layout_Spacer::spacer_size() const
{
  return _spacer_size;
}

void Layout_Spacer::dirty()
{
  Layout_Item::dirty();
    
  size_hint(
    V2i(
      (_spacer_size.x != -1) ? _spacer_size.x : style::global()->spacing(),
      (_spacer_size.y != -1) ? _spacer_size.y : style::global()->spacing()
    )
  );
}

//------------------------------------------------------------------------------
// group()
//------------------------------------------------------------------------------

void group(const List<Layout_Item *> & in)
{
  List<Layout_Item *> tmp, tmp2;
  
  for (list::size_type i = 0; i < in.size(); ++i)
  {
    if (in[i]->label_align())
      tmp += in[i]->label_align();
    if (in[i]->group_align())
      tmp2 += in[i]->group_align();
  }
  
  for (list::size_type i = 0; i < in.size(); ++i)
  {
    if (in[i]->label_align())
      in[i]->label_align()->group(tmp);
    if (in[i]->group_align())
      in[i]->group_align()->group(tmp2);
  }
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

String debug_path(const Layout_Item * in)
{
  List<String> out;
  
  out += in->debug_path();
  const Layout_Item * tmp = in;
  while (tmp = tmp->parent())
    out += tmp->debug_path();
  std::reverse(out.begin(), out.end());
  
  return string::join(out, '/');
}

//------------------------------------------------------------------------------

}
}
