/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_widget.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_browser_private.h"

#include <djv_image/time.h>
#include <djv_base/user.h>
#include <djv_ui/icon.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Browser
//------------------------------------------------------------------------------

Browser::Browser() :
  Fl_Browser_(0, 0, 0, 0),
  signal(this),
  mouse_signal(this),
  action_signal(this),
  _size_text(0),
  _size_icon(0),
  _size_thumbnail(0),
  _margin(0),
  _image(IMAGE(0)),
  _handle_mouse(false)
{
  debug_path("file_browser::Browser");
  
  DJV_DEBUG("Browser::Browser");
  
  Fl_Browser_::end();
  
  Fl_Browser_::type(FL_HOLD_BROWSER);
  Fl_Browser_::has_scrollbar(VERTICAL_ALWAYS);

  Fl_Browser_::box(FL_FLAT_BOX);
  Fl_Browser_::scrollbar.slider(style::FL_BOX_THIN_UP);
  Fl_Browser_::hscrollbar.slider(style::FL_BOX_THIN_UP);

  for (uint i = 0; i < _SORT_SIZE; ++i)
    _column_list[i] = 0;

  callback(widget_callback, this);
}

Browser::~Browser()
{}

void Browser::del()
{
  DJV_DEBUG("Browser::del");
  
  Widget_Base::del();

  Fl::remove_idle(image_callback, this);
  
  item_del();
}

void Browser::set(const List<File> & in, IMAGE image)
{
  using file_browser::Item;

  DJV_DEBUG("Browser::set");
  DJV_DEBUG_PRINT("in = " << in.size());
  DJV_DEBUG_PRINT("image = " << image);
    
  item_del();
  
  const list::size_type size = in.size();
  
  _item_list.resize(size);
  
  for (list::size_type i = 0; i < size; ++i)
    _item_list[i] =
      Item(
        in[i],
        i > 0 ? &_item_list[i - 1] : 0,
        i < size - 1 ? &_item_list[i + 1] : 0,
        i % 2 != 0
      );
  
  _image = image;
  
  Fl_Browser_::deselect();
  Fl_Browser_::new_list();
}

File Browser::get() const
{
  const file_browser::Item * item = reinterpret_cast<file_browser::Item *>(
    Fl_Browser_::selection());
  
  return item ? item->get() : File();
}

void Browser::item_del()
{
  DJV_DEBUG("Browser::item_del");
  
  _item_list.clear();
  _image_work_list.clear();
}

void Browser::column_list(const List<int> & in)
{
  DJV_DEBUG("Browser::column_list");
  DJV_DEBUG_PRINT("in = " << in);
  
  for (uint i = 0; i < _SORT_SIZE; ++i)
    _column_list[i] = i < in.size() ? in[i] : 0;
  
  redraw();
}

int Browser::handle(int in)
{
  switch (in)
  {
    case FL_PUSH:
    case FL_RELEASE:
    case FL_DRAG:
    
      _handle_mouse = true;
      
      break;
    
    case FL_KEYBOARD:
    
      _handle_mouse = false;
      
      if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
        switch (Fl::event_key())
        {
          case FL_Enter:
          case FL_KP_Enter:
          {
            file_browser::Item * item =
              reinterpret_cast<file_browser::Item *>(Fl_Browser_::selection());
            
            if (item)
              action_signal.emit(item->get());
            
            return 1;
          }
        }
      
      break;
  }
  
  return Fl_Browser_::handle(in);
}

void Browser::show()
{
  Fl_Browser_::show();
  
  Widget_Base::show();
}

void Browser::draw()
{
  Fl_Browser_::draw();
  
  Widget_Base::draw();
}

int Browser::item_height(void * in) const
{
  file_browser::Item * item = static_cast<file_browser::Item *>(in);
  
  int height = _size_icon;
  if (_image && item)
    if (item->image_is_valid())
      height = math::max(math::max(_size_thumbnail, _size_text * 3), height);

  return math::max(_size_text, height) + _margin * 2;
}

int Browser::item_width(void *) const
{
  return 0;
}

void Browser::item_draw(void * in, int x, int y, int w, int h) const
{
  file_browser::Item * item = static_cast<file_browser::Item *>(in);
  
  if (! item)
    return;

  //DJV_DEBUG("Browser::item_draw");
  //DJV_DEBUG_PRINT("in = " << item->get());
  
  fl_font(style::global()->font(), style::global()->font_size());
  
  const File & file = item->get();
  const int margin = style::global()->margin_widget();
  const int text_off = fl_height() - fl_descent();
  int _x, _y;
  
  // Background.
  
  if (item != selection())
  {
    const Fl_Color color_list [] =
    {
      FL_BACKGROUND_COLOR,
      FL_BACKGROUND2_COLOR
    };
    
    fl_color(color_list[item->odd()]);
    fl_rectf(x, y, w, h);
  }
  
  // Name, icon, and thumbnail image.
  
  fl_color(in == selection() ? FL_BLACK : FL_FOREGROUND_COLOR);
  
  fl_push_clip(x, y, _column_list[0], h);

  _x = x + margin;
  _y = y + margin;
  
  Fl_Image * image = 0;
  
  if (_image)
  {
    if (! item->image_init())
    {
      //DJV_DEBUG_PRINT("image init");
      
      Browser * that = const_cast<Browser *>(this);
      
      that->_image_work_list.push_back(item);
      
      Fl::remove_idle(image_callback, that);
      Fl::add_idle(image_callback, that);
    }
    else
    {
      //DJV_DEBUG_PRINT("image draw");

      image = item->image(_size_thumbnail, _image);
      
      if (image)
        image->draw(_x, y + h / 2 - image->h() / 2);
    }
  }
  
  if (! image)
  {
    Fl_Image * icon = 0;
    
    switch (file.type())
    {
      case djv_base::file::SEQ:
        icon = djv_ui::icon::get("file_seq");
        break;
      
      case djv_base::file::DIRECTORY:
        icon = djv_ui::icon::get("directory");
        break;
      
      default:
        icon = djv_ui::icon::get("file");
        break;
    }
    
    if (icon)
      icon->draw(_x, y + h / 2 - icon->h() / 2);
  }
  
  _x += (
    _image ?
      math::max(
        style::global()->size_thumbnail(),
        style::global()->size_icon()) :
      style::global()->size_icon()
    ) + margin;
  
  if (image)
  {
    const djv_image::io::Info & info = item->image_info();

    _y = y + (h / 2 - fl_height() * 3 / 2);
    
    String tmp = file.get(-1, false);
    fl_draw(tmp.c_str(), _x, _y + text_off);
    
    tmp = string::Format("%%x%%:%% %%").
      arg(info.size.x).
      arg(info.size.y).
      arg(aspect(info.size), 0, -2).
      arg(string::label(info.pixel));
    
    fl_draw(tmp.c_str(), _x, _y + fl_height() + text_off);

    tmp = string::Format("%%@%%").
      arg(djv_image::time::units_to_string(
        static_cast<int>(info.time.list.size()), info.time.speed)).
      arg(djv_image::time::speed_to_float(info.time.speed));
    
    fl_draw(tmp.c_str(), _x, _y + fl_height() * 2 + text_off);
  }
  else
  {
    _y = y + (h / 2 - fl_height() / 2);
    
    fl_draw(file.get(-1, false).c_str(), _x, _y + text_off);
  }
  
  fl_pop_clip();

  // Information.
  
  int column = 0;

  _x = x + _column_list[column];
  _y = y + (h / 2 - fl_height() / 2);
  ++column;
  
  fl_push_clip(_x, y, _column_list[column], h);
  String tmp = djv_base::file::label_type()[file.type()];
  fl_draw(
    tmp.c_str(),
    _x + _column_list[column] - margin - base::string_width(tmp),
    _y + text_off
  );
  fl_pop_clip();
  _x += _column_list[column];
  ++column;
  
  fl_push_clip(_x, y, _column_list[column], h);
  tmp = djv_base::file::label_size(file.size());
  fl_draw(
    tmp.c_str(),
    _x + _column_list[column] - margin - base::string_width(tmp),
    _y + text_off
  );
  fl_pop_clip();
  _x += _column_list[column];
  ++column;
  
#if ! defined(DJV_WINDOWS)

  fl_push_clip(_x, y, _column_list[column], h);
  tmp = user::uid_to_string(file.user());
  fl_draw(
    tmp.c_str(),
    _x + _column_list[column] - margin - base::string_width(tmp),
    _y + text_off
  );
  fl_pop_clip();
  _x += _column_list[column];
  ++column;

#endif // DJV_WINDOWS

  fl_push_clip(_x, y, _column_list[column], h);
  tmp = djv_base::time::time_to_string(file.time());
  fl_draw(
    tmp.c_str(),
    _x +
      _column_list[column] - margin -
      (base::string_width(tmp) + Fl_Browser_::scrollbar.w()),
    _y + text_off
  );
  fl_pop_clip();
  ++column;
  
  // Layout.
  
  _x = x + _column_list[0];
  
  for (uint i = 1; i < _SORT_SIZE; ++i)
  {
    fl_color(style::FL_COLOR_BORDER);
    fl_line(_x - 1, y, _x - 1, y + h - 1);
  
    _x += _column_list[i];
  }
}

const Fl_Widget * Browser::fltk_widget() const
{
  return this;
}

void Browser::dirty()
{
  Widget_Base::dirty();
  
  labelfont(style::global()->font());
  labelsize(style::global()->font_size());

  fl_font(style::global()->font(), style::global()->font_size());
  _size_text = fl_height();
  _size_icon = style::global()->size_icon();
  _size_thumbnail = style::global()->size_thumbnail();
  _margin = style::global()->margin_widget();
}

void Browser::geom(const Box2i & in)
{
  Fl_Browser_::resize(in.x, in.y, in.w, in.h);
  
  Widget_Base::geom(in);
}

const Box2i & Browser::geom() const
{
  return Widget_Base::geom();
}

void Browser::resize(int x, int y, int w, int h)
{
  geom(Box2i(x, y, w, h));
}

int Browser::visible() const
{
  return Fl_Browser_::visible();
}

void Browser::widget_callback()
{
  DJV_DEBUG("Browser::widget_callback");
  
  using file_browser::Item;
  
  if (const Item * item = reinterpret_cast<const Item *>(selection()))
  {
    const File & value = item->get();

    DJV_DEBUG_PRINT("value = " << value);
      
    if (Fl::event_clicks())
    {
      Fl::event_clicks(0);
      
      action_signal.emit(value);
    }
    else
    {
      if (_handle_mouse)
        mouse_signal.emit(value);
      else
        signal.emit(value);
    }
  }
}

void Browser::image_callback()
{
  using file_browser::Item;
  
  //DJV_DEBUG("Browser::image_callback");
  //DJV_DEBUG_PRINT("image = " << _image_work_list.size());
  //DJV_DEBUG_PRINT("visible = " << visible_r());
  
  if (_image_work_list.size())
  {
    Item * item = _image_work_list.front();
    _image_work_list.pop_front();
    if (displayed(item))
    {
      item->image(_size_thumbnail, _image);
      redraw_line(item);
    }
  }
  
  if (! _image_work_list.size())
    Fl::remove_idle(image_callback, this);
  
  if (! visible_r())
  {
    Fl::remove_idle(image_callback, this);
    _image_work_list.clear();
  }
}

//------------------------------------------------------------------------------

}
}

