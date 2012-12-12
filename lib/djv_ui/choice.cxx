/*------------------------------------------------------------------------------
 lib/djv_ui/choice.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "choice.h"

#include <djv_ui/icon.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace choice {

//------------------------------------------------------------------------------
// Choice::_Menu_Button
//------------------------------------------------------------------------------

class Choice::_Menu_Button : public Fl_Menu_Button, public Widget_Base
{
public:

  _Menu_Button(Choice * parent);

  // Value.
  
  void set(int);
  void list(const List<String> &);
  
  int get() const { return _value; }
  
  callback::Signal<int> signal;
  
  // Label.
  
  void text(const List<String> &);

  // Widget.
  
  int handle(int);
  void show();
  void draw();

  const Fl_Widget * fltk_widget() const { return this; }
  
  // Layout.
  
  void dirty();
  void geom(const Box2i &);
  
  const Box2i & geom() const { return Widget_Base::geom(); }
  
  void resize(int x, int y, int w, int h) { geom(Box2i(x, y, w, h)); }
  
  int visible() const { return Fl_Menu_Button::visible(); }
  
private:

  DJV_FL_WIDGET_CALLBACK(_Menu_Button, menu_callback) { set(value()); }

  void widget_update();
  
  int _value;
  
  List<memory::Buffer<char> > _list;
  
  List<memory::Buffer<char> > _text;
  
  Fl_Image * _icon;
  
  Choice * _parent;
};

Choice::_Menu_Button::_Menu_Button(Choice * parent) :
  Fl_Menu_Button(0, 0, 0, 0),
  signal(this),
  _value(0),
  _icon(icon::get("tool_dec")),
  _parent(parent)
{
  debug_path("choice::_Menu_Button");
  
  box(style::FL_BOX_MENU);
  
  widget_update();
  
  callback(menu_callback, this);
}

void Choice::_Menu_Button::set(int in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  redraw();
  
  signal.emit(_value);
}

namespace {

String pack(const String & in)
{
  String out = in;
  
  String::size_type pos = 0;
  while ((pos = out.find('&', pos)) != String::npos)
    out = out.replace(pos, 1, 0, ' ');
  
  return out;
}

List<memory::Buffer<char> > cstr_list(const List<String> & in)
{
  List<memory::Buffer<char> > out(in.size());

  for (list::size_type i = 0; i < in.size(); ++i)
  {
    const String tmp = pack(in[i]);
    
    out[i].size(tmp.size() + 1);
    
    char * p = out[i]();
    memory::copy(tmp.c_str(), p, tmp.size());
    p[tmp.size()] = 0;
  }
  
  return out;
}

}

void Choice::_Menu_Button::list(const List<String> & in)
{
  _list = cstr_list(in);
  
  _value = 0;
  
  widget_update();
  
  signal.emit(_value);
}

void Choice::_Menu_Button::text(const List<String> & in)
{
  _text = cstr_list(in);
  
  _value = 0;
  
  widget_update();
  
  signal.emit(_value);
}

int Choice::_Menu_Button::handle(int in)
{
  switch (Widget_Base::handle(in))
  {
    case -1: return 0;
    case 1: return 1;
  }
  
  switch (in)
  {
    case FL_PUSH:
      _parent->down(true);
      _parent->select(true);
      break;
    case FL_RELEASE:
      _parent->down(false);
      break;
      
    case FL_FOCUS:
      _parent->select(true);
      break;
    case FL_UNFOCUS:
      _parent->down(false);
      _parent->select(false);
      break;
  }
  
  return Fl_Menu_Button::handle(in);
}

void Choice::_Menu_Button::show()
{
  Fl_Menu_Button::show();
  
  Widget_Base::show();
}

void Choice::_Menu_Button::draw()
{
  DJV_DEBUG("_Menu_Button::draw");
  
  const Box2i & geom = this->geom();

  DJV_DEBUG_PRINT("geom = " << geom);

  // Label.
  
  const int margin = Fl::box_dx(box()) + 3;
  
  DJV_DEBUG_PRINT("margin = " << margin);
  
  Box2i tmp = box::border(geom, -V2i(margin));

  if (_value >= 0 && _value < static_cast<int>(_list.size()))
  {
    DJV_DEBUG_PRINT("value = " << _list[_value]());
  
    fl_font(textfont(), textsize());
    fl_color(text_color());
    fl_draw(
      _list[_value](),
      tmp.x,
      tmp.y + fl_height() - fl_descent()
    );
  }
  
  // Icon.
  
  if (_icon)
    _icon->draw(
      tmp.x + tmp.w - style::global()->size_tool_icon(),
      tmp.y + tmp.h / 2 - _icon->h() / 2
    );
}
  
void Choice::_Menu_Button::dirty()
{
  DJV_DEBUG("_Menu_Button::dirty");
  
  Widget_Base::dirty();

  textfont(text_font());
  textsize(style::global()->font_size());
  textcolor(text_color());
  
  fl_font(text_font(), style::global()->font_size());
  V2i size;
  for (list::size_type i = 0; i < _list.size(); ++i)
    size = max(base::string_size(_list[i]()), size);
  
  size_hint(
    V2i(
      size.x +
        style::global()->size_tool_icon() +
        style::global()->margin_widget(),
      size.y
    ) +
    style::global()->margin_widget() * 2
  );
}

void Choice::_Menu_Button::geom(const Box2i & in)
{
  Fl_Menu_Button::resize(in.x, in.y, in.w, in.h);
  
  Widget_Base::geom(in);
}

void Choice::_Menu_Button::widget_update()
{
  callbacks(false);
  
  clear();
  
  const List<memory::Buffer<char> > & list = _text.size() ? _text : _list;
  
  const list::size_type size = list.size();
  memory::Buffer<Fl_Menu_Item> tmp(size + 1);
  memory::set<uint8_t>(0, tmp(), (size + 1) * sizeof(Fl_Menu_Item));
  
  Fl_Menu_Item * p = tmp();
  for (list::size_type i = 0; i < size; ++i)
  {
    p[i].text = list[i]();
    p[i].callback_ = menu_callback;
    p[i].user_data_ = this;
  }
  copy(tmp());
  
  update();

  callbacks(true);
}

//------------------------------------------------------------------------------
// Choice::_Choice
//------------------------------------------------------------------------------

struct Choice::_Choice
{
  _Menu_Button * widget;
  
  Layout_H * layout;
};

//------------------------------------------------------------------------------
// Choice
//------------------------------------------------------------------------------

void Choice::init(
  const List<String> & list,
  const List<String> & text
) {
  DJV_DEBUG("Choice::init");
  
  debug_path("Choice");
  
  _down = false;
  _select = false;
  
  highlight(true);
  
  _p->widget =
    new _Menu_Button(this);
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  _p->layout = new Layout_H(this);
  _p->layout->margin(0);
  _p->layout->add(_p->widget);

  _p->widget->list(list);
  _p->widget->text(text);
  
  _p->widget->signal.set(this, widget_callback);
}

Choice::Choice() :
  signal(this),
  _p(new _Choice)
{
  init(List<String>(), List<String>());
}

Choice::Choice(const List<String> & list) :
  signal(this),
  _p(new _Choice)
{
  init(list, List<String>());
}

Choice::Choice(
  const List<String> & list,
  const List<String> & text
) :
  signal(this),
  _p(new _Choice)
{
  init(list, text);
}

Choice::~Choice()
{}

void Choice::set(int in)
{
  _p->widget->set(in);
}

int Choice::get() const
{
  return _p->widget->get();
}

void Choice::list(const List<String> & in)
{
  if (in == _list)
    return;
  
  _list = in;
  
  _p->widget->list(_list);
}

const List<String> & Choice::list() const
{
  return _list;
}

void Choice::text(const List<String> & in)
{
  if (in == _text)
    return;
  
  _text = in;
  
  _p->widget->text(_text);
}

const List<String> & Choice::text() const
{
  return _text;
}

void Choice::draw()
{
  const Box2i & geom = this->geom();
  
  const bool inside = Widget_Base::inside();
  const bool select = _select && active();
  const bool highlight = inside && active();
  
  fl_draw_box(
    box_type(style(), _down),
    geom.x, geom.y, geom.w, geom.h,
    select ?
      FL_SELECTION_COLOR :
      (highlight ? style::FL_COLOR_HIGHLIGHT : style::FL_COLOR_BUTTON)
  );
  
  Widget::draw();
}

void Choice::down(bool in)
{
  if (in == _down)
    return;
  
  _down = in;
  
  redraw();
}

void Choice::select(bool in)
{
  if (in == _select)
    return;
  
  _select = in;
  
  redraw();
  
  if (in)
    select_signal.emit(this);
  else
    select_off_signal.emit(this);
}

const Fl_Widget * Choice::fltk_widget() const
{
  return _p->widget;
}

void Choice::widget_callback(int in)
{
  signal.emit(in);
}

//------------------------------------------------------------------------------

}
}

