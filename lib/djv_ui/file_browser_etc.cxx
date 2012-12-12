/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_fnc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "file_browser_private.h"

#include <djv_ui/file_browser_prefs.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// columns_size_list()
//------------------------------------------------------------------------------

List<int> columns_size_list()
{
  return List<int>() <<
    0 <<
    (base::string_width(4)) <<
    (base::string_width("000.0000") + style::global()->margin() * 2) <<
#if ! defined(DJV_WINDOWS)
    (base::string_width("000000") + style::global()->margin() * 2) <<
#endif
    (
      base::string_width("000 000 00 00:00:00 0000") + 16 +
      style::global()->margin() * 2
    );
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(IMAGE, label_image())
_DJV_STRING_OPERATOR_LABEL(SORT, label_sort())
_DJV_STRING_OPERATOR_LABEL(NAVIGATE, label_navigate())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_image()
{
  static const List<String> data = List<String>() <<
    "Off" <<
    "Low quality" <<
    "High quality";
  
  DJV_ASSERT(data.size() == _IMAGE_SIZE);
  
  return data;
}

const List<String> & label_sort()
{
  static const List<String> data = List<String>() <<
    djv_base::file::label_sort()[djv_base::file::SORT_NAME] <<
    djv_base::file::label_sort()[djv_base::file::SORT_TYPE] <<
    djv_base::file::label_sort()[djv_base::file::SORT_SIZE] <<
#if ! defined(DJV_WINDOWS)
    djv_base::file::label_sort()[djv_base::file::SORT_USER] <<
#endif
    djv_base::file::label_sort()[djv_base::file::SORT_TIME];
  
  DJV_ASSERT(data.size() == _SORT_SIZE);

  return data;
}

const List<String> & label_navigate()
{
  static const List<String> data = List<String>() <<
    "Single-click" <<
    "Double-click";
  
  DJV_ASSERT(data.size() == _NAVIGATE_SIZE);

  return data;
}

const List<String> & label_shortcut()
{
  static const List<String> data = List<String>() <<
    "Up" <<
    "Previous" <<
    "Current" <<
    "Home" <<
    "Desktop" <<
    "Root" <<
    "Reload" <<
    "Sequence off" <<
    "Sequence sparse" <<
    "Sequence range" <<
    "Hidden" <<
    "Sort name" <<
    "Sort size" <<
#if ! defined(DJV_WINDOWS)
    "Sort user" <<
#endif
    "Sort time" <<
    "Sort reverse" <<
    "Sort directories" <<
    "Bookmark add" <<
    "Bookmark 1" <<
    "Bookmark 2" <<
    "Bookmark 3" <<
    "Bookmark 4" <<
    "Bookmark 5" <<
    "Bookmark 6" <<
    "Bookmark 7" <<
    "Bookmark 8" <<
    "Bookmark 9" <<
    "Bookmark 10" <<
    "Bookmark 11" <<
    "Bookmark 12";
  
  DJV_ASSERT(data.size() == Prefs::_SHORTCUT_SIZE);
  
  return data;
}

extern const String
  label_seq = "Sequence:",
  label_type_all = "All",
  label_type_all_image = "All images";

extern const String
  dialog_title = "File Browser",
  dialog_bookmark_del = "Delete bookmark:",
  dialog_bookmark_del_all = "Delete all bookmarks?";

extern const String
  menu_directory = "Directory",
  menu_directory_up = "&Up",
  menu_directory_prev = "&Previous",
  menu_directory_recent = "&Recent",
  menu_directory_current = "&Current",
  menu_directory_home = "&Home",
  menu_directory_desktop = "&Desktop",
  menu_directory_drive = "Dri&ves",
  menu_directory_root = "&Root",
  menu_directory_reload = "Re&load",
  menu_option = "Options",
  menu_option_image = "Thu&mbnails",
  menu_option_seq = "Se&quence",
  menu_option_type = "&Type",
  menu_option_hidden = "&Hidden",
  menu_option_sort = "&Sort",
  menu_option_sort_reverse = "&Reverse Sort",
  menu_option_sort_directory = "&Directories First",
  menu_option_navigate = "Na&vigate",
  menu_bookmark = "Bookmarks",
  menu_bookmark_add = "&Add",
  menu_bookmark_del = "&Delete",
  menu_bookmark_del_all = "D&elete All";

extern const String
  tooltip_value = "File name",
  tooltip_up = "Go up a directory\n\nShortcut: %%",
  tooltip_prev = "Go to the previous directory\n\nShortcut: %%",
  tooltip_reload = "Reload the current directory\n\nShortcut: %%";

extern const String
  prefs_title = "File Browser";

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

void set(
  Callback_Base * in,
  callback::Signal<const File &>::Callback * callback
) {
  Dialog * dialog = global();
  dialog->signal.del();
  dialog->signal.set(in, callback);
  dialog->show();
}

void del(Callback_Base * in)
{
  Dialog * dialog = global();
  dialog->signal.del(in);
}

//------------------------------------------------------------------------------

}
}

