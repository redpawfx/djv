/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_callback.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_browser_private.h"

#include <djv_ui/file_browser_prefs.h>
#include <djv_base/directory.h>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

void Dialog::value_callback(const String & in)
{
  DJV_DEBUG("Dialog::value_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  set(
    File(in).type() == djv_base::file::DIRECTORY ?
      djv_base::file::path_fix(in) :
      in
  );
}

void Dialog::value_action_callback(const String & in)
{
  value_callback(in);
  
  if (_value.type() != djv_base::file::DIRECTORY)
    ok_callback(true);
}

void Dialog::value_change_callback(const String & in)
{
  File tmp(in, false);
  if (tmp.stat() && tmp.type() == djv_base::file::DIRECTORY)
    set(tmp);
}

void Dialog::browser_callback(const File & in)
{
  DJV_DEBUG("Dialog::browser_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  set(in);
}

void Dialog::browser_mouse_callback(const File & in)
{
  DJV_DEBUG("Dialog::browser_mouse_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  set(
    (
      NAVIGATE_SINGLE_CLICK == prefs()->navigate() &&
      in.type() == djv_base::file::DIRECTORY
    ) ?
      File(djv_base::file::path_fix(in)) :
      in
  );
}

void Dialog::browser_action_callback(const File & in)
{
  DJV_DEBUG("Dialog::browser_action_callback");
  DJV_DEBUG_PRINT("in = " << in);

  set(
    in.type() == djv_base::file::DIRECTORY ?
      File(djv_base::file::path_fix(in)) :
      in
  );
  
  if (_value.type() != djv_base::file::DIRECTORY)
    ok_callback(true);
}

void Dialog::columns_callback(const List<int> & in)
{
  _p->browser->column_list(in);
}

void Dialog::columns_action_callback(int in)
{
  if (in == _sort)
    sort_reverse(! _sort_reverse);
  else
    sort(static_cast<SORT>(in));
}

void Dialog::up_callback(bool)
{
  Directory tmp(_value);
  tmp.up();
  set(tmp.get());
}

void Dialog::_up_callback()
{
  up_callback(true);
}

void Dialog::prev_callback(bool)
{
  set(_directory_prev);
}

void Dialog::_prev_callback()
{
  prev_callback(true);
}

void Dialog::_recent_callback()
{
  const List<String> & recent = prefs()->recent();
  
  const int i = list::find(_p->menu->item(), _p->menu_recent);
  if (i >= 0 && i < static_cast<int>(recent.size()))
    set(recent[i]);
}

void Dialog::current_callback(bool)
{
  set(Directory(".").get());
}

void Dialog::_current_callback()
{
  current_callback(true);
}

#if ! defined(DJV_WINDOWS)

void Dialog::home_callback(bool)
{
  set(djv_base::directory::home());
}

void Dialog::_home_callback()
{
  home_callback(true);
}

#endif // ! DJV_WINDOWS

void Dialog::desktop_callback(bool)
{
  set(djv_base::directory::desktop());
}

void Dialog::_desktop_callback()
{
  desktop_callback(true);
}

#if defined(DJV_WINDOWS)

void Dialog::drive_callback(bool)
{
  const List<String> drive_list = djv_base::directory::drive_list();
  
  const int i = list::find(_p->menu->item(), _p->menu_drive);
  if (i >= 0 && i < static_cast<int>(drive_list.size()))
    set(drive_list[i]);
}

void Dialog::_drive_callback()
{
  drive_callback(true);
}

#else // DJV_WINDOWS

void Dialog::root_callback(bool)
{
  set(djv_base::directory::root());
}

void Dialog::_root_callback()
{
  root_callback(true);
}

#endif // DJV_WINDOWS

void Dialog::reload_callback(bool)
{
  directory_update();
}

void Dialog::_reload_callback()
{
  reload_callback(true);
}

void Dialog::_image_callback()
{
  image(static_cast<IMAGE>(list::find(_p->menu->item(), _p->menu_image)));
}

void Dialog::seq_callback(int in)
{
  seq(static_cast<file_seq::COMPRESS>(in));
}

void Dialog::_seq_callback()
{
  seq_callback(list::find(_p->menu->item(), _p->menu_seq));
}

void Dialog::_type_callback()
{
  type(list::find(_p->menu->item(), _p->menu_type) - 1);
}

void Dialog::_hidden_callback()
{
  hidden(! _hidden);
}

void Dialog::_sort_callback()
{
  sort(static_cast<SORT>(list::find(_p->menu->item(), _p->menu_sort)));
}

void Dialog::_sort_reverse_callback()
{
  sort_reverse(! _sort_reverse);
}

void Dialog::_sort_directory_callback()
{
  sort_directory(! _sort_directory);
}

void Dialog::_navigate_callback()
{
  prefs()->navigate(
    static_cast<NAVIGATE>(list::find(_p->menu->item(), _p->menu_navigate))
  );
}

void Dialog::_bookmark_callback()
{
  const List<String> & bookmark = prefs()->bookmark();
  
  const int i = list::find(_p->menu->item(), _p->menu_bookmark);
  if (i >= 0 && i < static_cast<int>(bookmark.size()))
    set(bookmark[i]);
}

void Dialog::_bookmark_add_callback()
{
  prefs()->bookmark_add(_value.path());

  menu_update();
}

void Dialog::bookmark_del_callback(const List<int> & in)
{
  const List<String> & bookmark = prefs()->bookmark();
  
  List<String> tmp;
  
  for (list::size_type i = 0; i < bookmark.size(); ++i)
  {
    list::size_type j = 0;
    for (; j < in.size(); ++j)
      if (in[j] == i)
        break;
    if (j < in.size())
      continue;
    
    tmp += bookmark[i];
  }

  prefs()->bookmark(tmp);

  menu_update();
}

void Dialog::_bookmark_del_callback()
{
  const List<String> & bookmark = prefs()->bookmark();
  
  if (! bookmark.size())
    return;

  dialog::global()->multi_choice(
    dialog_bookmark_del,
    bookmark,
    this,
    bookmark_del_callback
  );
}

void Dialog::bookmark_del_all_callback(bool)
{
  prefs()->bookmark(List<String>());

  menu_update();
}

void Dialog::_bookmark_del_all_callback()
{
  if (! prefs()->bookmark().size())
    return;
  
  dialog::global()->question(
    dialog_bookmark_del_all,
    this,
    bookmark_del_all_callback
  );
}

void Dialog::pin_callback(bool)
{
  pin(_p->pin->get());
}

void Dialog::ok_callback(bool)
{
  DJV_DEBUG("Dialog::ok_callback");
  DJV_DEBUG_PRINT("value = " << _value);
  DJV_DEBUG_PRINT("value type = " << _value.type());
  DJV_DEBUG_PRINT("value seq = " << _value.seq());
  
  prefs()->recent_add(_value.path());

  if (! _pin)
    hide(); 
  
  menu_update();

  signal.emit(_value);
}

void Dialog::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------

}
}

