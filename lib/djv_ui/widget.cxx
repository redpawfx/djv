/*------------------------------------------------------------------------------
 lib/djv_ui/widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "widget.h"

#include <djv_ui/global.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <iostream>

namespace djv_ui {
namespace widget {

//------------------------------------------------------------------------------
// Widget
//------------------------------------------------------------------------------

Widget::Widget() :
  Fl_Group(0, 0, 0, 0),
  _visible(false),
  _highlight(false)
{
  debug_path("Widget");
  
  //DJV_DEBUG("Widget::Widget");

  Fl_Group::end();

  Fl_Group::box(FL_NO_BOX);
}

Widget::~Widget()
{
  //DJV_DEBUG("Widget::~Widget");
}

void Widget::tooltip(const String & in)
{
  Widget_Base::tooltip(in);
  
  const String::size_type size = in.size() + 1;
  _tooltip_cstr.size(size);
  string::cstring(in, _tooltip_cstr(), size);
  Fl_Group::tooltip(_tooltip_cstr());
}

int Widget::handle(int in)
{
  DJV_DEBUG("Widget::handle");
  
  switch (Widget_Base::handle(in))
  {
    case -1: return 0;
    case  1: return 1;
  }
  
  switch (in)
  {
    case FL_ENTER:
    case FL_LEAVE:
      if (_highlight)
      {
        DJV_DEBUG_PRINT("highlight");
        
        redraw();
      }
      break;
  }
    
  return Fl_Group::handle(in);
}

void Widget::show()
{
  Fl_Group::show();
  
  Widget_Base::show();
}

void Widget::draw()
{
  Fl_Group::draw();
  
  Widget_Base::draw();
}
  
void Widget::highlight(bool in)
{
  _highlight = in;
}

bool Widget::highlight() const
{
  return _highlight;
}

void Widget::dirty()
{
  Widget_Base::dirty();

  labelfont(text_font());
  labelsize(style::global()->font_size());
  labelcolor(text_color());
  
  _visible = Fl_Group::visible() ? true : false;
}

void Widget::geom(const Box2i & in)
{
  Fl_Widget::resize(in.x, in.y, in.w, in.h);
  
  Widget_Base::geom(in);
}

void Widget::size(layout_item::SIZE x, layout_item::SIZE y)
{
  Widget_Base::size(x, y);
}

void Widget::align(layout_item::ALIGN x, layout_item::ALIGN y)
{
  Widget_Base::align(x, y);
}

//------------------------------------------------------------------------------
// debug_children
//------------------------------------------------------------------------------

void debug_children(const Fl_Group * in, int indent)
{
  const String indent_(' ', indent);
  
  const Fl_Widget * const * children = in->array();
  const int size = in->children();
  
  std::cout << indent_ << "{" << std::endl;
  
  for (int i = 0; i < size; ++i)
  {
    if (const Widget_Base * widget = global()->widget_get(children[i]))
      std::cout << indent_ << "  " << widget->debug_path() << std::endl;
    else
      std::cout << indent_ << "  " << long(children[i]) << std::endl;
    
    if (const Fl_Group * group = dynamic_cast<const Fl_Group *>(children[i]))
      if (group->children())
        debug_children(group, indent + 2);
  }
  
  std::cout << indent_ << "}" << std::endl;
}

//------------------------------------------------------------------------------

}
}

