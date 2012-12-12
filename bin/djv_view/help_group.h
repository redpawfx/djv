/*------------------------------------------------------------------------------
 bin/djv_view/help_group.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_HELP_GROUP_H
#define DJV_VIEW_HELP_GROUP_H

#include "base.h"
#include <djv_ui/callback.h>
#include <djv_ui/menu_def.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::help
//
//! Help.
//------------------------------------------------------------------------------

namespace help {

//------------------------------------------------------------------------------
//!\class Group
//
//! Help group.
//------------------------------------------------------------------------------

class Group : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Group(
    djv_ui::menu::Menu *,
    const Group * copy = 0
  );
  
  ~Group();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void menu_update(djv_ui::menu::Group *);

  //@}

private:

  // Callbacks.
  
  DJV_FL_WIDGET_CALLBACK(Group, _help_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _info_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _about_callback);

  // Variables.
  
  struct _Group;
  std::auto_ptr<_Group> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  menu_title;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

