/*------------------------------------------------------------------------------
 bin/djv_view/help_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "help_group.h"

#include "application.h"
#include <djv_ui/application_dialog.h>
#include <djv_ui/menu.h>
#include <FL/Fl.H>

namespace djv_bin {
namespace view {
namespace help {

//------------------------------------------------------------------------------
// Group::_Group
//------------------------------------------------------------------------------

struct Group::_Group
{
  djv_ui::menu::Menu * menu;
};

//------------------------------------------------------------------------------
// Group
//------------------------------------------------------------------------------

Group::Group(
  djv_ui::menu::Menu * menu,
  const Group * copy
) :
  _p(new _Group)
{
  using namespace djv_ui;
  
  // Create widgets.
  
  _p->menu = menu;
}

Group::~Group()
{}

void Group::menu_update(djv_ui::menu::Group * in)
{
  using namespace djv_ui::menu;
    
  // Menu:
  //
  // * Documentation
  // * Information
  // * About
  
  in->add(menu_title, 0, 0, 0, SUB_MENU);

  in->add(djv_ui::menu::menu_help, 0, _help_callback, this);
  in->add(djv_ui::menu::menu_info, 0, _info_callback, this);
  in->add(djv_ui::menu::menu_about, 0, _about_callback, this);
  
  in->end();
}

void Group::_help_callback()
{
  application::global()->help();
}

void Group::_info_callback()
{
  djv_ui::application_dialog::global()->info()->show();
}

void Group::_about_callback()
{
  djv_ui::application_dialog::global()->about()->show();
}

//------------------------------------------------------------------------------

}
}
}

