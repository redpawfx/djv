/*------------------------------------------------------------------------------
 lib/djv_ui/widget_base.cxx

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
#include <FL/fl_draw.H>

namespace djv_ui {
namespace widget {

//------------------------------------------------------------------------------
// Widget_Base
//------------------------------------------------------------------------------

Widget_Base::Widget_Base() :
  Layout_Item(layout_item::WIDGET),
  _text_font(style::global()->font()),
  _text_color(FL_FOREGROUND_COLOR),
  _inside(false),
  _arrow_navkeys(true),
  _del(false)
{
  debug_path("Widget_Base");
  
  //DJV_DEBUG("Widget_Base::Widget_Base");
  
  djv_ui::global()->widget_add(this);
}

Widget_Base::~Widget_Base()
{
  //DJV_DEBUG("Widget_Base::~Widget_Base");
}

void Widget_Base::del()
{
  //if (_del)
  //  return;
  //_del = true;
  
  DJV_DEBUG(string::Format("Widget_Base::del(%%)").
    arg(layout_item::debug_path(this)));

  Callback_Base::del();

  _layout.reset();

  Fl_Widget * widget = dynamic_cast<Fl_Widget *>(this);
  
  DJV_ASSERT(widget);
  
  if (widget->parent())
  {
    DJV_DEBUG_PRINT("fltk remove");
    
    widget->parent()->remove(widget);
    widget->parent(0);
  }
  
  Fl::delete_widget(widget);

  djv_ui::global()->widget_del(this);
}

void Widget_Base::text_font(Fl_Font in)
{
  _text_font = in;
}

void Widget_Base::text_color(Fl_Color in)
{
  _text_color = in;
}

void Widget_Base::tooltip(const String & in)
{
  _tooltip = in;
}

int Widget_Base::handle(int in)
{
  switch (in)
  {
    case FL_KEYBOARD:
      if (! _arrow_navkeys)
        switch (Fl::event_key())
        {
          case FL_Left:
          case FL_Right:
          case FL_Up:
          case FL_Down:
            return -1;
        }
      break;
    
    case FL_ENTER:
      _inside = true;
      break;      
    case FL_LEAVE:
    case FL_HIDE:
      _inside = false;
      break;
  }
  
  return 0;
}

void Widget_Base::arrow_navkeys(bool in)
{
  _arrow_navkeys = in;
}

bool Widget_Base::arrow_navkeys() const
{
  return _arrow_navkeys;
}

void Widget_Base::show()
{}

namespace {

void draw_item(Layout_Item * in)
{
  const Box2i & geom = in->geom();
  
  const int x = geom.x, y = geom.y;
  const int w = geom.w, h = geom.h;
  
  switch (in->item())
  {
    case layout_item::WIDGET:
      fl_color(FL_RED);
      fl_rect(x + 0, y + 0, w - 0, h - 0);
      break;

    case layout_item::LAYOUT:
      fl_color(FL_BLUE);
      fl_rect(x + 0, y + 0, w - 0, h - 0);
      break;

    case layout_item::SPACER:
      fl_color(FL_CYAN);
      fl_rectf(x + 0, y + 0, w - 0, h - 0);
      break;
  }
}

void draw_layout(layout::Layout * in)
{
  const List<Layout_Item *> & list = in->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    if (layout_item::LAYOUT == list[i]->item())
      draw_layout(static_cast<layout::Layout *>(list[i]));
    else
      draw_item(list[i]);
  }
  
  draw_item(in);
}

}

void Widget_Base::draw()
{
  //if (_layout.get())
  //  draw_layout(_layout.get());
  
  //draw_item(this);
}

void Widget_Base::layout(layout::Layout * in)
{
  _layout.reset();
  _layout = std::auto_ptr<layout::Layout>(in);
  
  if (_layout.get())
    _layout->parent(this);
}

void Widget_Base::dirty()
{
  Layout_Item::dirty();
  
  if (_layout.get())
  {
    _layout->dirty();
    
    size_hint(_layout->size_hint());
  }
}

void Widget_Base::geom(const Box2i & in)
{
  //DJV_DEBUG(string::Format("Widget_Base::geom(%%)").
  //  arg(layout_item::debug_path(this)));
  //DJV_DEBUG_PRINT("in = " << in);

  Layout_Item::geom(in);
  
  if (_layout.get())
    _layout->geom(in);
}

//------------------------------------------------------------------------------

}
}

