/*------------------------------------------------------------------------------
 lib/djv_ui/frame.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "frame.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace frame {

//------------------------------------------------------------------------------
// Frame
//------------------------------------------------------------------------------

Frame::Frame(STYLE style, bool down) :
  select_signal(this),
  select_off_signal(this),
  highlight_signal(this),
  highlight_off_signal(this),
  _style(style),
  _down(down),
  _margin(0)
{
  debug_path("frame::Frame");
  
  Fl_Group::box(FL_NO_BOX);
}

Frame::~Frame()
{}

void Frame::style(STYLE in)
{
  if (in == _style)
    return;
  
  _style = in;
  
  dirty();
  update();
}

STYLE Frame::style() const
{
  return _style;
}

void Frame::down(bool in)
{
  if (in == _down)
    return;
  
  _down = in;
  
  dirty();
  update();
}

bool Frame::down() const
{
  return _down;
}

void Frame::margin(int in)
{
  if (in == _margin)
    return;
  
  _margin = in;
  
  dirty();
  update();
}

int Frame::margin() const
{
  return _margin;
}

int Frame::handle(int in)
{
  DJV_DEBUG("Frame::handle");
  
  switch (in)
  {
    case FL_ENTER:
      if (highlight() && active())
        highlight_signal.emit(this);
      break;

    case FL_LEAVE:
      if (highlight() && active())
        highlight_off_signal.emit(this);
      break;
  }
  
  return Widget::handle(in);
}

void Frame::draw()
{
  DJV_DEBUG("Frame::draw");
  
  const Box2i & geom = this->geom();
  const int margin = frame_size();
  
  const bool inside = Widget_Base::inside();
  
  DJV_DEBUG_PRINT("geom = " << geom);
  DJV_DEBUG_PRINT("margin = " << margin);
  
  const Box2i box(
    geom.x + _margin,
    geom.y + _margin,
    geom.w - _margin * 2,
    geom.h - _margin * 2
  );
  
  DJV_DEBUG_PRINT("box = " << box);
  
  fl_draw_box(
    box_type(_style, _down),
    box.x, box.y, box.w, box.h,
    color()
  );
  
  Fl_Group::draw();
  Widget_Base::draw();
}

int Frame::frame_size() const
{
  return Fl::box_dx(box_type(_style, _down)) + _margin;
}

Box2i Frame::frame_geom() const
{
  const Box2i & geom = this->geom();
  const int margin = frame_size();
  
  return Box2i(
    geom.x + margin,
    geom.y + margin,
    geom.w - margin * 2,
    geom.h - margin * 2
  );
}

void Frame::dirty()
{
  widget::Widget::dirty();

  const int margin = frame_size();
  
  size_hint((layout() ? layout()->size_hint() : 0) + margin * 2);
}

void Frame::geom(const Box2i & in)
{
  Fl_Widget::resize(in.x, in.y, in.w, in.h);
  
  Layout_Item::geom(in);
  
  if (layout())
    layout()->geom(frame_geom());
}

const Box2i & Frame::geom() const
{
  return widget::Widget::geom();
}

Fl_Boxtype Frame::box_type(STYLE style, bool down)
{
  Fl_Boxtype out = static_cast<Fl_Boxtype>(0);
  
  switch (style)
  {
    case STYLE_NORMAL:
      out = down ? style::FL_BOX_DOWN : style::FL_BOX_UP;
      break;
    
    case STYLE_H1:
      out = down ? style::FL_BOX_DOWN_H1 : style::FL_BOX_UP_H1;
      break;
    case STYLE_H2:
      out = down ? style::FL_BOX_DOWN_H2 : style::FL_BOX_UP_H2;
      break;
    case STYLE_H3:
      out = down ? style::FL_BOX_DOWN_H3 : style::FL_BOX_UP_H3;
      break;

    case STYLE_V1:
      out = down ? style::FL_BOX_DOWN_V1 : style::FL_BOX_UP_V1;
      break;
    case STYLE_V2:
      out = down ? style::FL_BOX_DOWN_V2 : style::FL_BOX_UP_V2;
      break;
    case STYLE_V3:
      out = down ? style::FL_BOX_DOWN_V3 : style::FL_BOX_UP_V3;
      break;
    
    case STYLE_THIN:
      out = down ? style::FL_BOX_THIN_DOWN : style::FL_BOX_THIN_UP;
      break;

    case STYLE_BORDER:
      out = style::FL_BOX_BORDER;
      break;
    case STYLE_FLAT:
      out = FL_FLAT_BOX;
      break;
    case STYLE_EMPTY:
      out = FL_NO_BOX;
      break;
  }
  
  return out;
}

//------------------------------------------------------------------------------
// Frame_Group_Base::_Frame_Group_Base
//------------------------------------------------------------------------------

struct Frame_Group_Base::_Frame_Group_Base
{
  layout_row::Layout_Row_Base * layout;
};

//------------------------------------------------------------------------------
// Frame_Group_Base
//------------------------------------------------------------------------------

Frame_Group_Base::Frame_Group_Base(
  bool horizontal,
  const List<Frame *> & group
) :
  _horizontal(horizontal),
  _draw_select(0),
  _draw_highlight(0),
  _p(new _Frame_Group_Base)
{
  debug_path("Frame_Group_Base");
  
  size(layout_item::FIXED, layout_item::FIXED);

  if (_horizontal)
    _p->layout = new Layout_H(this);
  else
    _p->layout = new Layout_V(this);
  _p->layout->spacing(1);
  _p->layout->margin(0);
  
  this->group(group);
  
  widget_update();
}

Frame_Group_Base::~Frame_Group_Base()
{}
  
void Frame_Group_Base::group(const List<Frame *> & in)
{
  DJV_DEBUG("Frame_Group_Base::group");
  
  _p->layout->del();
  
  list::size_type size = _group.size();
  
  for (list::size_type i = 0; i < size; ++i)
  {
    _group[i]->select_signal.del(this);
    _group[i]->select_off_signal.del(this);
    _group[i]->highlight_signal.del(this);
    _group[i]->highlight_off_signal.del(this);
  }
  
  _group = in;
  
  size = _group.size();
  
  for (list::size_type i = 0; i < size; ++i)
  {
    _group[i]->select_signal.set(this, draw_select_callback);
    _group[i]->select_off_signal.set(this, draw_select_off_callback);
    _group[i]->highlight_signal.set(this, draw_highlight_callback);
    _group[i]->highlight_off_signal.set(this, draw_highlight_off_callback);
  }

  widget_update();
}

const List<Frame *> & Frame_Group_Base::group() const
{
  return _group;
}

namespace {

void draw_edge0(const Box2i & in, bool horizontal)
{
  if (horizontal)
    fl_line(
      in.x - 1,
      in.y,
      in.x - 1,
      in.y + in.h - 1
    );
  else
    fl_line(
      in.x,
      in.y - 1,
      in.x + in.w - 1,
      in.y - 1
    );
}

void draw_edge1(const Box2i & in, bool horizontal)
{
  if (horizontal)
    fl_line(
      in.x + in.w,
      in.y,
      in.x + in.w,
      in.y + in.h - 1
    );
  else
    fl_line(
      in.x,
      in.y + in.h,
      in.x + in.w - 1,
      in.y + in.h
    );
}

}

void Frame_Group_Base::draw()
{
  DJV_DEBUG("Frame_Group_Base::draw");
  DJV_DEBUG_PRINT("draw select = " << long(_draw_select));
  DJV_DEBUG_PRINT("draw highlight = " << long(_draw_highlight));
  
  const Box2i & geom = this->geom();
  
  const style::Color & color_value = style::global()->color_value();
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  
  djv_image::color::scale(0.6, color_value.background, tmp);
  
  const list::size_type size = _group.size();
  
  for (list::size_type i = 0; i < size - 1; ++i)
  {
    const Box2i & child_geom = _group[i]->geom();
    
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
    
    draw_edge1(child_geom, _horizontal);
  }
  
  if (_draw_select && style::prefs()->focus())
  {
    const Box2i & child_geom = _draw_select->geom();
    
    fl_color(FL_SELECTION_COLOR);
    
    if (_draw_select != _group[0])
      draw_edge0(child_geom, _horizontal);
    if (_draw_select != _group[size - 1])
      draw_edge1(child_geom, _horizontal);
  }
  else if (_draw_highlight && style::prefs()->highlight())
  {
    const Box2i & child_geom = _draw_highlight->geom();
    
    fl_color(style::FL_COLOR_HIGHLIGHT);
    
    if (_draw_highlight != _group[0])
      draw_edge0(child_geom, _horizontal);
    if (_draw_highlight != _group[size - 1])
      draw_edge1(child_geom, _horizontal);
  }

  Widget::draw();
}

void Frame_Group_Base::draw_select_callback(const Frame * in)
{
  DJV_DEBUG("Frame_Group_Base::draw_select_callback");
  DJV_DEBUG_PRINT("in = " << long(in));
  
  _draw_select = in;

  redraw();
}

void Frame_Group_Base::draw_select_off_callback(const Frame * in)
{
  DJV_DEBUG("Frame_Group_Base::draw_select_off_callback");
  DJV_DEBUG_PRINT("in = " << long(in));
  
  if (in == _draw_select)
    _draw_select = 0;

  redraw();
}

void Frame_Group_Base::draw_highlight_callback(const Frame * in)
{
  DJV_DEBUG("Frame_Group_Base::draw_highlight_callback");
  DJV_DEBUG_PRINT("in = " << long(in));
  
  _draw_highlight = in;

  redraw();
}

void Frame_Group_Base::draw_highlight_off_callback(const Frame * in)
{
  DJV_DEBUG("Frame_Group_Base::draw_highlight_off_callback");
  DJV_DEBUG_PRINT("in = " << long(in));
  
  if (in == _draw_highlight)
    _draw_highlight = 0;

  redraw();
}

void Frame_Group_Base::widget_update()
{
  const list::size_type size = _group.size();

  for (list::size_type i = 0; i < size; ++i)
  {
    if (0 == i)
      _group[i]->style(_horizontal ? STYLE_H1 : STYLE_V1);
    else if ((size - 1) == i)
      _group[i]->style(_horizontal ? STYLE_H3 : STYLE_V3);
    else
      _group[i]->style(_horizontal ? STYLE_H2 : STYLE_V2);
    
    _p->layout->add(_group[i]);
  }
}

//------------------------------------------------------------------------------
// Frame_Group_Horizontal
//------------------------------------------------------------------------------

Frame_Group_Horizontal::Frame_Group_Horizontal(const List<Frame *> & group) :
  Frame_Group_Base(true, group)
{}

//------------------------------------------------------------------------------
// Frame_Group_Vertical
//------------------------------------------------------------------------------

Frame_Group_Vertical::Frame_Group_Vertical(const List<Frame *> & group) :
  Frame_Group_Base(false, group)
{}

//------------------------------------------------------------------------------

}
}
