/*------------------------------------------------------------------------------
 lib/djv_ui/menu.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "menu.h"

#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace menu {

//------------------------------------------------------------------------------
// Item
//------------------------------------------------------------------------------

Item::Item() :
  shortcut(0),
  callback(0),
  data(0),
  flags(0)
{}

Item::Item(
  const String & label,
  int shortcut,
  Fl_Callback * callback,
  void * data,
  int flags
) :
  label(label),
  shortcut(shortcut),
  callback(callback),
  data(data),
  flags(flags)
{
  // Convert label to a c-string, chopping it if necessary.
  
  const int label_size = static_cast<int>(label.size());
  
  const int size = math::min(label_size, item_label_max_size);
  cstr.size(size + 1);
  memory::copy(label.c_str(), cstr(), size);
  cstr()[size] = 0;

  if (label_size > item_label_max_size)
  {
    DJV_ASSERT(item_label_max_size >= 3);
    
    cstr()[size - 1] = '.';
    cstr()[size - 2] = '.';
    cstr()[size - 3] = '.';
  }
}

//------------------------------------------------------------------------------
// Menu::Widget
//------------------------------------------------------------------------------

class Menu::Widget : public Fl_Menu_Bar
{
public:

  Widget() :
    Fl_Menu_Bar(0, 0, 0, 0)
  {
    box(style::FL_BOX_MENU);
  }
  
  void draw()
  {
    // XXX
    
    const Fl_Boxtype tmp = box();
    box(FL_NO_BOX);
    
    Fl_Menu_Bar::draw();
    
    box(tmp);
  }
};

//------------------------------------------------------------------------------
// Group
//------------------------------------------------------------------------------

Group::Group()
{}

Group::Group(const Group & in)
{
  set(in);
}

Group::~Group()
{
  del();
}

Group & Group::operator = (const Group & in)
{
  if (&in != this)
    set(in);
  
  return *this;
}
  
void Group::set(const Group & in)
{
  del();
  
  _list.resize(in._list.size());
  
  for (list::size_type i = 0; i < in._list.size(); ++i)
    _list[i] = new Item(*in._list[i]);
}

int Group::add(
  const String & label,
  int shortcut,
  Fl_Callback * callback,
  void * data,
  int flags,
  bool value
) {
  _list += new Item(
    label,
    shortcut,
    callback,
    data,
    flags | (value ? VALUE : 0)
  );

  return static_cast<int>(_list.size()) - 1;
}

List<int> Group::add(
  const List<String> & label,
  const List<int> & shortcut,
  Fl_Callback * callback,
  void * data,
  int flags,
  int value
) {
  List<int> out;
  
  for (list::size_type i = 0; i < label.size(); ++i)
    out += add(
      label_list(label[i], static_cast<int>(i)),
      i < shortcut.size() ? shortcut[i] : 0,
      callback,
      data,
      flags,
      value != -1 ? (static_cast<int>(i) == value) : false
    );

  return out;
}

const List<Item *> & Group::get() const
{
  return _list;
}

void Group::end()
{
  _list += new Item;
}

void Group::del()
{
  for (list::size_type i = 0; i < _list.size(); ++i)
    delete _list[i];
  _list.clear();
}

void Group::fl(memory::Buffer<Fl_Menu_Item> * out) const
{
  const list::size_type size = _list.size();
  out->size(size + 1);
  out->zero();
    
  Fl_Menu_Item * p = (*out)();
  for (list::size_type i = 0; i < size; ++i)
  {
    p[i].text = _list[i]->cstr();
    p[i].shortcut_ = _list[i]->shortcut;
    p[i].callback_ = _list[i]->callback;
    p[i].user_data_ = _list[i]->data;
    p[i].flags =
      (_list[i]->flags & INACTIVE ? FL_MENU_INACTIVE : 0) |
      (_list[i]->flags & TOGGLE ? FL_MENU_TOGGLE : 0) |
      (_list[i]->flags & RADIO ? FL_MENU_RADIO : 0) |
      (_list[i]->flags & VALUE ? FL_MENU_VALUE : 0) |
      (_list[i]->flags & SUB_MENU ? FL_SUBMENU : 0) |
      (_list[i]->flags & DIVIDER ? FL_MENU_DIVIDER : 0);
    //p[i].labelfont_ = (_list[i]->flags & SUB_MENU) ?
    //  style::global()->font_bold() : 0;
  }
}

//------------------------------------------------------------------------------
// Menu
//------------------------------------------------------------------------------

Menu::Menu() :
  _test_shortcut(false)
{
  Frame::add(_widget = new Widget);
  
  style(frame::STYLE_THIN);

  size(layout_item::STRETCH, layout_item::FIXED);
}

void Menu::set(const menu::Group & in)
{
  _group = in;
  
  memory::Buffer<Fl_Menu_Item> tmp;
  _group.fl(&tmp);
  _widget->clear();
  _widget->copy(tmp());
}

const menu::Group & Menu::get() const
{
  return _group;
}

int Menu::item() const
{
  return _widget->value();
}

bool Menu::value() const
{
  DJV_DEBUG("Menu::value");
  
  //return _widget->mvalue()->value();

  const int item = _widget->value();
  bool value = (_widget->menu()[item].value() & FL_MENU_VALUE) != 0;
  if (_test_shortcut)
    value = ! value;
  
  DJV_DEBUG_PRINT("item = " << item);
  DJV_DEBUG_PRINT("value = " << value);
  
  return value;
}

int Menu::test_shortcut()
{
  DJV_DEBUG("Menu::test_shortcut");
  
  const List<menu::Item *> & list = _group.get();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    if (Fl::test_shortcut(list[i]->shortcut))
    {
      DJV_DEBUG_PRINT("item = " << i);
      
      _widget->value(static_cast<int>(i));
      
      const Fl_Menu_Item & item = _widget->menu()[i];
      
      if (item.callback())
      {
        _test_shortcut = true;
        item.do_callback(_widget);
        _test_shortcut = false;
      }
      
      return 1;
    }
  
  return 0;
}

void Menu::dirty()
{
  Frame::dirty();
  
  _widget->textfont(text_font());
  _widget->textsize(style::global()->font_size());
  _widget->textcolor(text_color());
  
  fl_font(text_font(), style::global()->font_size());
  
  size_hint(
    V2i(0, fl_height()) +
    style::global()->margin_widget() * 2
  );
}

void Menu::geom(const Box2i & in)
{
  Frame::geom(in);
  
  _widget->resize(in.x, in.y, in.w, in.h);
}

const Box2i & Menu::geom() const
{
  return Frame::geom();
}

//------------------------------------------------------------------------------

}
}
