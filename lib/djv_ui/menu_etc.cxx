/*------------------------------------------------------------------------------
 lib/djv_ui/menu_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "menu.h"

namespace djv_ui {
namespace menu {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

String label_list(const String & in, int index)
{
  return string::Format("%%%%. %%").
    arg(index < 9 ? "&" : "").
    arg(index + 1).arg(in);
}

extern const String
  menu_messages = "Messa&ges",
  menu_prefs = "&Preferences",
  menu_info = "Information",
  menu_help = "Help",
  menu_about = "About",
  menu_exit = "Exit";

//------------------------------------------------------------------------------

}
}

