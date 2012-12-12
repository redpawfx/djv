/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_prefs_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_browser_prefs.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/shortcut.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget()
{
  // Create widgets.

  shortcut::Shortcut_Widget * shortcut_widget =
    new shortcut::Shortcut_Widget;
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());
  
  layout->add(shortcut_widget);
  layout->stretch(shortcut_widget);
  
  // Initialize.
  
  shortcut_widget->set(prefs()->shortcut());
  
  // Callbacks.

  shortcut_widget->signal.set(this, shortcut_callback);
}

void Prefs_Widget::shortcut_callback(const List<shortcut::Shortcut> & in)
{
  prefs()->shortcut(in);
}

//------------------------------------------------------------------------------

}
}

