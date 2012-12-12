/*------------------------------------------------------------------------------
 lib/djv_ui/columns.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "columns.h"

#include <djv_ui/icon.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace columns {

//------------------------------------------------------------------------------
// Columns
//------------------------------------------------------------------------------

Columns::Columns() :
  signal(this),
  action_signal(this),
  _sort(-1),
  _sort_reverse(false),
  _push(-1),
  _drag(-1)
{
  //text_font(style::global()->font_bold());
}

Columns::~Columns()
{}

void Columns::name_list(const List<String> & in)
{
  if (in == name_list())
    return;
  
  DJV_DEBUG("Columns::name_list");
  DJV_DEBUG_PRINT("in = " << in);

  _column_list.resize(in.size());
  
  for (list::size_type i = 0; i < _column_list.size(); ++i)
  {
    _column_list[i].name = in[i];
    _column_list[i].column = 0;
    _column_list[i].size_hint = 0;
  }
  
  columns_update();
  redraw();

  signal.emit(size_list());
}

void Columns::size_list(const List<int> & in)
{
  if (in == size_list())
    return;
  
  DJV_DEBUG("Columns::size_list");
  DJV_DEBUG_PRINT("in = " << in);

  _column_list.resize(in.size());
  
  const List<int> list = this->size_list();
  int x = 0;
  for (list::size_type i = 0; i < in.size(); ++i)
  {
    x += in[i] ? in[i] : list[i];
    _column_list[i].column = x - 1;
  }
  
  columns_update();
  redraw();

  signal.emit(size_list());
}

List<String> Columns::name_list() const
{
  List<String> out(_column_list.size());
  for (list::size_type i = 0; i < _column_list.size(); ++i)
    out[i] = _column_list[i].name;
  return out;
}

List<int> Columns::size_list() const
{
  List<int> out(_column_list.size());
  int x = 0;
  for (list::size_type i = 0; i < _column_list.size(); ++i)
  {
    out[i] = _column_list[i].column - x + 1;
    x += out[i];
  }
  return out;
}

void Columns::sort(int in)
{
  if (in == _sort)
    return;
  
  DJV_DEBUG("Columns::sort");
  DJV_DEBUG_PRINT("in = " << in);

  _sort = in;
  
  redraw();
}

int Columns::sort() const
{
  return _sort;
}

void Columns::sort_reverse(bool in)
{
  if (in == _sort_reverse)
    return;
  
  DJV_DEBUG("Columns::sort_reverse");
  DJV_DEBUG_PRINT("in = " << in);

  _sort_reverse = in;
  
  redraw();
}

bool Columns::sort_reverse() const
{
  return _sort_reverse;
}

int Columns::handle(int in)
{
  const Box2i & geom = this->geom();
  const V2i mouse(Fl::event_x(), Fl::event_y());

  switch (in)
  {
    case FL_PUSH:
    {
      DJV_DEBUG("Columns::handle");
      
      _drag = handle_drag(mouse.x - geom.x, _column_list);
      
      if (-1 == _drag)
        _push = handle_push(mouse.x - geom.x, _column_list);
      
      DJV_DEBUG_PRINT("push = " << _push);
      DJV_DEBUG_PRINT("drag = " << _drag);
      
      redraw();
    }
    return 1;
  
    case FL_RELEASE:
    {
      const int emit = box::intersect(geom, mouse) ? _push : -1;
      _push = _drag = -1;
      
      redraw();
      
      if (emit != -1)
        action_signal.emit(emit);
    }
    return 1;
    
    case FL_DRAG:
    {
      if (-1 == _drag) break;
      
      DJV_DEBUG("Columns::handle");
      DJV_DEBUG_PRINT("drag = " << _drag);
      DJV_DEBUG_PRINT("columns = " << size_list());
      
      const list::size_type size = _column_list.size();
      
      int min = 0, max = geom.w - 1;
      for (int i = 0; i <= _drag; ++i)
        min += _column_list[i].size_hint;
      for (list::size_type i = _drag + 1; i < size; ++i)
        max -= _column_list[i].size_hint;
      
      _column_list[_drag].column = math::clamp(mouse.x - geom.x, min, max);

      // Push columns.
      
      for (int i = _drag - 1; i >= 0; --i)
        _column_list[i].column = math::min(
          _column_list[i].column,
          _column_list[i + 1].column - _column_list[i + 1].size_hint);
      for (list::size_type i = _drag + 1; i < size; ++i)
        _column_list[i].column = math::max(
          _column_list[i].column,
          _column_list[i - 1].column + _column_list[i].size_hint);

      columns_update();
      redraw();
      
      signal.emit(size_list());
      
    }
    return 1;
    
    case FL_MOVE:
    {
      if (handle_drag(mouse.x - geom.x, _column_list) != -1)
        base::cursor(FL_CURSOR_WE);
      else
        base::cursor(FL_CURSOR_DEFAULT);
    }
    return 1;
    
    case FL_LEAVE:
    {
      base::cursor(FL_CURSOR_DEFAULT);
    }
    return 1;
  }
  
  return widget::Widget::handle(in);
}

int Columns::handle_push(int in, const List<Column> & list)
{
  for (int i = 0; i < static_cast<int>(list.size()); ++i)
    if (in < list[i].column) return i;
  
  return -1;
}

int Columns::handle_drag(int in, const List<Column> & list)
{
  const int size_pick = style::global()->size_pick();
  
  for (int i = 0; i < static_cast<int>(list.size()) - 1; ++i)
    if (
      math::intersect(
        in,
        math::Range<int>(
          list[i].column - size_pick,
          list[i].column + size_pick))
    )
      return i;
  
  return -1;
}

void Columns::draw()
{
  DJV_DEBUG("Columns::draw");
  DJV_DEBUG_PRINT("sort = " << _sort);
  DJV_DEBUG_PRINT("sort reverse = " << _sort_reverse);
  
  const Box2i & geom = this->geom();
  const List<int> size_list = this->size_list();
  
  DJV_DEBUG_PRINT("columns = " << size_list);

  // Background.
  
  fl_font(text_font(), style::global()->font_size());
  fl_color(FL_BACKGROUND_COLOR);
  fl_rectf(geom.x, geom.y, geom.w, geom.h);
  
  const int margin = style::global()->margin_widget();
  int x = geom.x;
  for (list::size_type i = 0; i < size_list.size(); ++i)
  {
    fl_draw_box(
      static_cast<int>(i) == _push ?
        style::FL_BOX_THIN_DOWN :
        style::FL_BOX_THIN_UP,
      x,
      geom.y,
      size_list[i],
      geom.h,
      color()
    );

    // Label.
    
    fl_color(FL_FOREGROUND_COLOR);
    fl_draw(
      _column_list[i].name.c_str(),
      x + margin,
      geom.y + geom.h / 2 - fl_height() / 2 + fl_height() - fl_descent()
    );
    
    // Sort icon.
    
    if (static_cast<int>(i) == _sort)
    {
      Fl_Image * icon = djv_ui::icon::get(
        _sort_reverse ? "tool_dec" : "tool_inc"
      );
      icon->draw(
        x + size_list[i] - style::global()->size_tool_icon() - margin,
        geom.y + (geom.h - icon->h()) / 2
      );
    }
    
    x += size_list[i];
  }
}

void Columns::dirty()
{
  widget::Widget::dirty();

  fl_font(text_font(), style::global()->font_size());
  
  V2i size;
  for (list::size_type i = 0; i < _column_list.size(); ++i)
  {
    const V2i label_size = base::string_size(_column_list[i].name);
    
    _column_list[i].size_hint =
      label_size.x +
      style::global()->size_tool_icon() +
      style::global()->margin_widget() * 3;

    size.x += _column_list[i].size_hint;
    size.y = math::max(label_size.y, size.y);
  }
  
  size_hint(V2i(size.x, size.y + style::global()->margin_widget() * 2));
}

void Columns::geom(const Box2i & in)
{
  bool dirty = in.size != geom().size;
  
  widget::Widget::geom(in);
  
  if (! dirty)
    return;
  
  DJV_DEBUG("Columns::geom");
  DJV_DEBUG_PRINT("width = " << in.w);
  DJV_DEBUG_PRINT("columns = " << size_list());
  
  // Resize first column.
  
  if (_column_list.size())
  {
    const int total = _column_list[_column_list.size() - 1].column;
    for (list::size_type i = 0; i < _column_list.size(); ++i)
      _column_list[i].column += in.w - total - 1;
  }
  
  columns_update();

  signal.emit(size_list());
}

const Box2i & Columns::geom() const
{
  return widget::Widget::geom();
}

void Columns::columns_update()
{
  DJV_DEBUG("Columns::columns_update");
  DJV_DEBUG_PRINT("columns = " << size_list());
  DJV_DEBUG_PRINT("width = " << w());
  
  const Box2i & geom = this->geom();
  
  if (geom.w)
  {
    const list::size_type size = _column_list.size();
    List<int> size_list = this->size_list();
    
    // Check column minimum size.

    for (list::size_type i = 0; i < size; ++i)
      size_list[i] = math::max(size_list[i], _column_list[i].size_hint);
    
    DJV_DEBUG_PRINT("columns = " << size_list);
    
    // Fit columns within widget.
    
    int total = 0;
    for (list::size_type i = 0; i < size; ++i)
      total += size_list[i];
    
    DJV_DEBUG_PRINT("total = " << total);
    
    for (list::size_type i = 0; i < size && total > geom.w; ++i)
    {
      int tmp = size_list[i];
      size_list[i] = math::max(
        size_list[i] - (total - geom.w),
        _column_list[i].size_hint);
      total -= tmp - size_list[i];
    }
    
    DJV_DEBUG_PRINT("total = " << total);
    DJV_DEBUG_PRINT("columns = " << size_list);
    
    int x = 0;
    for (list::size_type i = 0; i < size; ++i)
    {
      x += size_list[i];
      _column_list[i].column = x - 1;
    }
    
    DJV_DEBUG_PRINT("total = " << x);
  }
  
  DJV_DEBUG_PRINT("columns = " << size_list());
}

//------------------------------------------------------------------------------

}
}

