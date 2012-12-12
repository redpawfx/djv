/*------------------------------------------------------------------------------
 lib/djv_ui/tabs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "tabs.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/layout_stack.h>
#include <djv_ui/style.h>
#include <djv_ui/scroll.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace tabs {

//------------------------------------------------------------------------------
// _Tab
//------------------------------------------------------------------------------

namespace {

class _Tab : public widget::Widget
{
public:

  _Tab(Widget_Base *);
  
  void name(const String &);

  const String & name() const { return _name; }
  
private:

  String _name;
  memory::Buffer<char> _name_cstr;
};

_Tab::_Tab(Widget_Base * item)
{
  debug_path("tabs::_Tab");
  
  text_font(style::global()->font_bold());
  
  scroll::Scroll * scroll_widget = new scroll::Scroll;
  Layout_V * layout = new Layout_V(this);
  layout->add(scroll_widget);
  scroll_widget->layout()->margin(0);
  if (item)
    scroll_widget->layout()->add(item);
}

void _Tab::name(const String & in)
{
  if (in == _name) return;
  
  _name = in;
  
  const String::size_type size = _name.size() + 1;
  _name_cstr.size(size);
  string::cstring(_name, _name_cstr(), size);
  Fl_Group::label(_name_cstr());
}

}

//------------------------------------------------------------------------------
// Tabs
//------------------------------------------------------------------------------

Tabs::Tabs() :
  Fl_Tabs(0, 0, 0, 0),
  signal(this)
{
  debug_path("Tabs");
  
  Fl_Tabs::end();
  
  box(style::FL_BOX_THIN_UP);

  Layout_Stack * layout = new Layout_Stack(this);
  layout->margin(0);
  
  callback(widget_callback, this);
}

Tabs::~Tabs()
{}

void Tabs::set(int in)
{
  DJV_ASSERT(in < static_cast<int>(_list.size()));
  
  value(dynamic_cast<Fl_Widget *>(_list[in]));
}

int Tabs::get() const
{
  // XXX FLTK forgot their const.

  Tabs * widget = const_cast<Tabs *>(this);
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    if (widget->value() == dynamic_cast<Fl_Widget *>(_list[i]))
      return static_cast<int>(i);
      
  return -1;
}

void Tabs::tab_add(const String & name, Widget_Base * in)
{
  tab_insert(name, in, static_cast<int>(_list.size()));
}

void Tabs::tab_insert(
  const String & name,
  Widget_Base * in,
  int index
) {
  DJV_DEBUG("Tabs::tab_insert");
  DJV_DEBUG_PRINT("name = " << name);
  DJV_DEBUG_PRINT("index = " << index);
  
  _Tab * widget = new _Tab(in);
  widget->name(name);
  
  List<Widget_Base *>::iterator i = _list.begin();
  int j = 0;
  for (; j < index && j < static_cast<int>(_list.size()); ++j, ++i)
    ;
  _list.insert(i, widget);
  
  layout()->insert(widget, j);
  
  //dirty();
  update();
  redraw();
  
  //tab(index);
}

void Tabs::tab_name(const String & name, int tab)
{
  DJV_ASSERT(tab < static_cast<int>(_list.size()));
  
  DJV_DEBUG("Tabs::tab_name");
  DJV_DEBUG_PRINT("name = " << name);
  DJV_DEBUG_PRINT("tab = " << tab);
  
  static_cast<_Tab *>(_list[tab])->name(name);
  
  //dirty();
  update();
  redraw();
}

void Tabs::tab_del(int in)
{
  DJV_ASSERT(in < static_cast<int>(_list.size()));
  
  DJV_DEBUG("Tabs::tab_del");
  DJV_DEBUG_PRINT("in = " << in);

  List<Widget_Base *>::iterator i = _list.begin();
  for (int j = 0; j < in; ++j, ++i) ;
  _list.erase(i);
  
  layout()->del(in);
  
  //dirty();
  update();
  redraw();
}

void Tabs::tab_del()
{
  while (_list.size())
    tab_del(0);
}
  
int Tabs::tab_size() const
{
  return static_cast<int>(_list.size());
}

int Tabs::handle(int in)
{
  switch (Widget_Base::handle(in))
  {
    case -1: return 0;
    case 1: return 1;
  }
  
  return Fl_Tabs::handle(in);
}
  
void Tabs::show()
{
  Fl_Tabs::show();
  
  Widget_Base::show();
}

void Tabs::draw()
{
  Fl_Tabs::draw();

  const Box2i & geom = this->geom();

  // XXX Erase extra tab lines.
  
  fl_color(FL_BACKGROUND_COLOR);
  fl_line(geom.x, geom.y + _size_tab.y, geom.x, geom.y + geom.h - 1);
  fl_line(geom.x + geom.w - 1, geom.y + _size_tab.y, geom.x + geom.w - 1,
    geom.y + geom.h - 1);
  fl_line(geom.x, geom.y + geom.h - 1, geom.x + geom.w - 1,
    geom.y + geom.h - 1);
  
  Widget_Base::draw();
}

void Tabs::dirty()
{
  DJV_DEBUG("Tabs::dirty");
  
  Widget_Base::dirty();

  fl_font(text_font(), style::global()->font_size());
  
  int label_hint = 0;
  for (list::size_type i = 0; i < _list.size(); ++i)
    label_hint = math::max(
      base::string_width(static_cast<_Tab *>(_list[i])->name()),
      label_hint);

  _size_tab =
    V2i(0, fl_height()) +
    style::global()->margin_widget() * 2 +
    V2i(1 * 2, 1);
  
  DJV_DEBUG_PRINT("tab = " << _size_tab);

  size_hint(_size_tab + layout()->size_hint() + 1 * 2);
}

void Tabs::geom(const Box2i & in)
{
  DJV_DEBUG("Tabs::geom");
  DJV_DEBUG_PRINT("in = " << in);
  
  Fl_Widget::resize(in.x, in.y, in.w, in.h);
  
  Layout_Item::geom(in);
  
  layout()->geom(
    Box2i(
      in.x + 1,
      in.y + _size_tab.y + 1,
      in.w - 1 * 2,
      in.h - _size_tab.y - 1 * 2
    )
  );
}

void Tabs::resize(int x, int y, int w, int h)
{
  geom(Box2i(x, y, w, h));
}

const Box2i & Tabs::geom() const
{
  return Widget_Base::geom();
}

int Tabs::visible() const
{
  return Fl_Tabs::visible();
}

void Tabs::widget_callback()
{
  signal.emit(get());
}

//------------------------------------------------------------------------------

}
}

