/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_menu.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_browser_private.h"

#include <djv_ui/file_browser_prefs.h>
#include <djv_ui/menu.h>
#include <djv_ui/shortcut.h>
#include <djv_base/directory.h>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Dialog::menu_update()
//------------------------------------------------------------------------------

void Dialog::menu_update()
{
  using namespace menu;
  
  DJV_DEBUG("Dialog::menu_update");
  
  menu::Group group;
  
  const List<shortcut::Shortcut> & shortcut = prefs()->shortcut();
  
  // Menu:
  //
  // Directory
  // * Up
  // * Previous
  // * Recent
  //   * ...
  //   ---
  // * Current
  // * Home
  // * Desktop
  // * Root
  //   ---
  // * Reload
  //
  // Options
  // * Image Thumbnails
  //   * ...
  // * File Sequencing
  //   * ...
  //   ---
  // * File Types
  //   * ...
  // * Hidden Files
  //   ---
  // * Sort By
  //   * ...
  // * Reverse Sort
  // * Sort Directories First
  //   ---
  // * Navigate
  //   * ...
  //
  // Bookmarks
  // * Add
  // * Delete
  // * Delete All
  //   ---
  // * ...

  group.add(menu_directory, 0, 0, 0, SUB_MENU);
  
  group.add(
    menu_directory_up,
    shortcut[Prefs::UP].value,
    _up_callback,
    this
  );

  group.add(
    menu_directory_prev,
    shortcut[Prefs::PREV].value,
    _prev_callback,
    this
  );

  group.add(
    menu_directory_recent,
    0,
    0,
    0,
    SUB_MENU | DIVIDER
  );

  _p->menu_recent = group.add(
    prefs()->recent(),
    List<int>(),
    _recent_callback,
    this
  );

  group.end();

  group.add(
    menu_directory_current,
    shortcut[Prefs::CURRENT].value,
    _current_callback,
    this
  );

#if ! defined(DJV_WINDOWS)

  group.add(
    menu_directory_home,
    shortcut[Prefs::HOME].value,
    _home_callback,
    this
  );

#endif // DJV_WINDOWS

  group.add(
    menu_directory_desktop,
    shortcut[Prefs::DESKTOP].value,
    _desktop_callback,
    this
  );

#if defined(DJV_WINDOWS)

  group.add(
    menu_directory_drive,
    0,
    0,
    0,
    SUB_MENU | DIVIDER
  );

  _p->menu_drive = group.add(
    directory::drive_list(),
    List<int>(),
    _drive_callback,
    this
  );

  group.end();

#else // DJV_WINDOWS

  group.add(
    menu_directory_root,
    shortcut[Prefs::ROOT].value,
    _root_callback,
    this,
    DIVIDER
  );

#endif // DJV_WINDOWS

  group.add(
    menu_directory_reload,
    shortcut[Prefs::RELOAD].value,
    _reload_callback,
    this
  );
  
  group.end();

  group.add(menu_option, 0, 0, 0, SUB_MENU);
  
  group.add(menu_option_image, 0, 0, 0, SUB_MENU);
  _p->menu_image = group.add(
    label_image(),
    List<int>(),
    _image_callback,
    this,
    RADIO,
    _image
  );
  group.end();
  
  group.add(menu_option_seq, 0, 0, 0, SUB_MENU);
  _p->menu_seq = group.add(
    file_seq::label_compress(),
    List<int>() <<
      shortcut[Prefs::SEQ_OFF].value <<
      shortcut[Prefs::SEQ_SPARSE].value <<
      shortcut[Prefs::SEQ_RANGE].value,
    _seq_callback,
    this,
    RADIO,
    _seq
  );
  group.end();
  
  group.add(menu_option_type, 0, 0, 0, SUB_MENU);
  _p->menu_type = group.add(
    _type_label,
    List<int>(),
    _type_callback,
    this,
    RADIO,
    _type + 1
  );
  group.end();
  
  group.add(
    menu_option_hidden,
    shortcut[Prefs::HIDDEN].value,
    _hidden_callback,
    this,
    TOGGLE | DIVIDER,
    _hidden
  );
  
  group.add(menu_option_sort, 0, 0, 0, SUB_MENU);
  _p->menu_sort = group.add(
    label_sort(),
    List<int>() <<
      shortcut[Prefs::SORT_NAME].value <<
      shortcut[Prefs::SORT_SIZE].value <<
#if ! defined(DJV_WINDOWS)
      shortcut[Prefs::SORT_USER].value <<
#endif
      shortcut[Prefs::SORT_TIME].value,
    _sort_callback,
    this,
    RADIO,
    _sort
  );
  group.end();
  
  group.add(
    menu_option_sort_reverse,
    shortcut[Prefs::SORT_REVERSE].value,
    _sort_reverse_callback,
    this,
    TOGGLE,
    _sort_reverse
  );

  group.add(
    menu_option_sort_directory,
    shortcut[Prefs::SORT_DIRECTORY].value,
    _sort_directory_callback,
    this,
    TOGGLE | DIVIDER,
    _sort_directory
  );
  
  group.add(menu_option_navigate, 0, 0, 0, SUB_MENU);
  _p->menu_navigate = group.add(
    label_navigate(),
    List<int>(),
    _navigate_callback,
    this,
    RADIO,
    prefs()->navigate()
  );
  group.end();
  
  group.end();

  group.add(menu_bookmark, 0, 0, 0, SUB_MENU);
  
  group.add(
    menu_bookmark_add,
    shortcut[Prefs::BOOKMARK_ADD].value,
    _bookmark_add_callback,
    this
  );
  
  group.add(
    menu_bookmark_del,
    0,
    _bookmark_del_callback,
    this
  );

  group.add(
    menu_bookmark_del_all,
    0,
    _bookmark_del_all_callback,
    this,
    DIVIDER
  );
  
  _p->menu_bookmark = group.add(
    prefs()->bookmark(), List<int>() <<
      shortcut[Prefs::BOOKMARK_1].value <<
      shortcut[Prefs::BOOKMARK_2].value <<
      shortcut[Prefs::BOOKMARK_3].value <<
      shortcut[Prefs::BOOKMARK_4].value <<
      shortcut[Prefs::BOOKMARK_5].value <<
      shortcut[Prefs::BOOKMARK_6].value <<
      shortcut[Prefs::BOOKMARK_7].value <<
      shortcut[Prefs::BOOKMARK_8].value <<
      shortcut[Prefs::BOOKMARK_9].value <<
      shortcut[Prefs::BOOKMARK_10].value <<
      shortcut[Prefs::BOOKMARK_11].value <<
      shortcut[Prefs::BOOKMARK_12].value,
    _bookmark_callback,
    this
  );
  group.end();
  
  group.end();
  
  _p->menu->set(group);
}

//------------------------------------------------------------------------------

}
}

