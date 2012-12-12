/*------------------------------------------------------------------------------
 lib/djv_ui/prefs_dialog.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "prefs_dialog.h"

#include <djv_ui/file_browser_prefs.h>
#include <djv_ui/image.h>
#include <djv_ui/image_io.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/push_button.h>
#include <djv_ui/shortcut.h>
#include <djv_ui/style.h>
#include <djv_ui/tabs.h>

namespace djv_ui {
namespace prefs_dialog {

//------------------------------------------------------------------------------
// Dialog::_Dialog
//------------------------------------------------------------------------------

struct Dialog::_Dialog
{
  tabs::Tabs * tabs;
  tabs::Tabs * shortcut_tabs;
  
  Push_Button * close;
};

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

Dialog::Dialog() :
  dialog::Base(dialog_title),
  _p(new _Dialog)
{
  // Create widgets.

  _p->tabs =
    new tabs::Tabs;
  _p->shortcut_tabs =
    new tabs::Tabs;
  
  _p->close =
    new Push_Button(dialog::label_close);

  // Layout.

  Layout_V * layout = new Layout_V(this);
  layout->add(_p->tabs);
  layout->stretch(_p->tabs);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  add(prefs_general_title, new prefs_dialog::General);
  add(prefs_style_title, new prefs_dialog::Style);

  add(image_io::prefs_title, new image_io::Prefs_Widget);
  
  add(shortcut::prefs_title, _p->shortcut_tabs);
  add_shortcut(file_browser::prefs_title, new file_browser::Prefs_Widget);
  
  size(V2i(600, 600));

  // Callbacks.

  _p->close->signal.set(this, close_callback);
}

Dialog::~Dialog()
{}

void Dialog::add(const String & name, Widget_Base * in)
{
  DJV_DEBUG("Dialog::add");
  DJV_DEBUG_PRINT("name = " << name);
  
  _p->tabs->tab_insert(name, in);
}

void Dialog::add_shortcut(const String & name, Widget_Base * in)
{
  DJV_DEBUG("Dialog::add_shortcut");
  DJV_DEBUG_PRINT("name = " << name);
  
  _p->shortcut_tabs->tab_insert(name, in);
}

void Dialog::show()
{
  _p->close->take_focus();

  dialog::Base::show();
}

void Dialog::dirty()
{
  Base::dirty();
}

void Dialog::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------

}
}

