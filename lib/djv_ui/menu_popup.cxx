/*------------------------------------------------------------------------------
 lib/djv_ui/menu_popup.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "menu.h"

#include <djv_ui/icon.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace menu {

//------------------------------------------------------------------------------
// Popup::Widget
//------------------------------------------------------------------------------

class Popup::Widget : public Fl_Menu_Button
{
public:

  Widget(Popup * parent) :
    Fl_Menu_Button(0, 0, 0, 0),
    _parent(parent)
  {
    box(style::FL_BOX_MENU);
  }

  int handle(int in)
  {
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
  
  void draw()
  {}

private:

  Popup * _parent;
};

//------------------------------------------------------------------------------
// Popup
//------------------------------------------------------------------------------

Popup::Popup(const List<String> & list) :
  signal(this),
  _icon(icon::get("tool_dec")),
  _down(false),
  _select(false)
{
  // Create widgets.
  
  Frame::add(_widget = new Widget(this));

  // Layout.
  
  size(layout_item::FIXED, layout_item::FIXED);
  
  // Initialize.
  
  highlight(true);

  set(list);
  
  // Callbacks.
  
  _widget->callback(widget_callback, this);
}

void Popup::set(const List<String> & in)
{
  _group.del();
  for (list::size_type i = 0; i < in.size(); ++i)
    _group.add(in[i]);

  memory::Buffer<Fl_Menu_Item> tmp;
  _group.fl(&tmp);
  _widget->clear();
  _widget->copy(tmp());
}

void Popup::set(const menu::Group & in)
{
  _group = in;
  
  memory::Buffer<Fl_Menu_Item> tmp;
  _group.fl(&tmp);
  _widget->clear();
  _widget->copy(tmp());
}

const menu::Group & Popup::get() const
{
  return _group;
}

int Popup::value() const
{
  return _widget->value();
}

void Popup::draw()
{
  /*const bool inside = Widget_Base::inside();
  
  color(
    _select && active() ?
      FL_SELECTION_COLOR :
      (inside && active() ? style::FL_COLOR_HIGHLIGHT : style::FL_COLOR_BUTTON)
  );
  
  Frame::draw();*/
  
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
  
  if (_icon)
    _icon->draw(
      x() + w() / 2 - _icon->w() / 2,
      y() + h() / 2 - _icon->h() / 2
    );
}

void Popup::down(bool in)
{
  if (in == _down)
    return;
  
  _down = in;
  
  redraw();
}

void Popup::select(bool in)
{
  if (in == _select)
    return;
  
  _select = in;
  
  redraw();
}

void Popup::dirty()
{
  Frame::dirty();

  _widget->textfont(text_font());
  _widget->textsize(style::global()->font_size());
  _widget->textcolor(text_color());
  
  size_hint(style::global()->size_tool_icon() + style::global()->margin() * 2);
}

void Popup::geom(const Box2i & in)
{
  Frame::geom(in);
  
  _widget->resize(in.x, in.y, in.w, in.h);
}

const Box2i & Popup::geom() const
{
  return Frame::geom();
}

void Popup::widget_callback()
{
  signal.emit(_widget->value());
}

//------------------------------------------------------------------------------

}
}

