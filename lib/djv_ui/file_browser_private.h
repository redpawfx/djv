/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_private.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_FILE_BROWSER_PRIVATE_H
#define DJV_UI_FILE_BROWSER_PRIVATE_H

#include <djv_ui/check_button.h>
#include <djv_ui/choice.h>
#include <djv_ui/columns.h>
#include <djv_ui/file_browser.h>
#include <djv_ui/file_browser_prefs.h>
#include <djv_ui/menu.h>
#include <djv_ui/prefs.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>
#include <djv_ui/tool_button.h>
#include <djv_image/io.h>
#include <djv_base/file.h>
#include <FL/Fl_Browser_.H>
#include <list>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Item
//------------------------------------------------------------------------------

class Item
{
public:

  Item();
  Item(const File &, Item * prev, Item * next, bool odd);
  
  ~Item();
  
  const File & get() const { return _value; }
  Item * prev() const { return _prev; }
  Item * next() const { return _next; }
  bool odd() const { return _odd; }

  Fl_Image * image(int size, IMAGE) const;
  const djv_image::io::Info & image_info() const { return _image_info; }
  bool image_is_valid() const { return _image_is_valid; }
  bool image_init() const { return _image_init; }

private:
  
  File _value;
  Item * _prev;
  Item * _next;
  bool _odd;
  Fl_Image * _image;
  djv_image::io::Info _image_info;
  bool _image_is_valid;
  bool _image_init;
};

//------------------------------------------------------------------------------
// Browser
//------------------------------------------------------------------------------

class Browser : public Fl_Browser_, public Widget_Base
{
public:

  Browser();
  
  ~Browser();
  
  void del();
  
  // Value.
  
  void set(const List<File> &, IMAGE image);
  
  File get() const;
  
  callback::Signal<const File &> signal;
  callback::Signal<const File &> mouse_signal;
  callback::Signal<const File &> action_signal;
  
  // Columns.

  void column_list(const List<int> &);
  
  // Widget.
  
  int handle(int);
  void show();
  void draw();

  inline void * item_first() const;
  inline void * item_next(void *) const;
  inline void * item_prev(void *) const;
  
  int item_height(void *) const;
  int item_width(void *) const;
  void item_draw(void *, int, int, int, int) const;

  const Fl_Widget * fltk_widget() const;

  // Layout.
  
  void dirty();
  void geom(const Box2i &);
  
  const Box2i & geom() const;
  
  void resize(int, int, int, int);
  
  int visible() const;
  
private:
  
  void item_del();
  
  DJV_FL_WIDGET_CALLBACK(Browser, widget_callback);
  DJV_FL_CALLBACK(Browser, image_callback);

  List<file_browser::Item> _item_list;
  
  int _column_list [_SORT_SIZE];
  
  int _size_text;
  int _size_icon;
  int _size_thumbnail;
  int _margin;
  
  IMAGE _image;
  std::list<file_browser::Item *> _image_work_list;
  
  bool _handle_mouse;
};

inline void * Browser::item_first() const
{
  return _item_list.size() ? (void *)&_item_list.front() : 0;
}

inline void * Browser::item_next(void * in) const
{
  return in ? static_cast<file_browser::Item *>(in)->next() : 0;
}

inline void * Browser::item_prev(void * in) const
{
  return in ? static_cast<file_browser::Item *>(in)->prev() : 0;
}

//------------------------------------------------------------------------------
// Dialog::_Dialog
//------------------------------------------------------------------------------

struct Dialog::_Dialog
{
  menu::Menu * menu;
  List<int> menu_recent;
  List<int> menu_image;
  List<int> menu_seq;
  List<int> menu_type;
  List<int> menu_sort;
  List<int> menu_navigate;
  List<int> menu_bookmark;
#if defined(DJV_WINDOWS)
  List<int> menu_drive;
#endif

  Text_Edit * value;
  
  Tool_Button * up;
  Tool_Button * prev;
  Tool_Button * reload;
  Choice * seq;
  
  Browser * browser;
  columns::Columns * columns;
  
  Check_Button * pin;
  
  Frame * browser_frame;
};

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

List<int> columns_size_list();

//------------------------------------------------------------------------------

}
}

#endif

