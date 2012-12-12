/*------------------------------------------------------------------------------
 bin/djv_view/tool_group.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_TOOL_GROUP_H
#define DJV_VIEW_TOOL_GROUP_H

#include "base.h"
#include <djv_ui/callback.h>
#include <djv_ui/layout_row_def.h>
#include <djv_ui/menu_def.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::tool
//
//! Tools.
//------------------------------------------------------------------------------

namespace tool {

//------------------------------------------------------------------------------
//!\class Group
//
//! Tool group.
//------------------------------------------------------------------------------

class Group : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Group(
    djv_ui::menu::Menu *,
    djv_ui::Layout_H * toolbar,
    const Group * copy = 0
  );
  
  ~Group();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{
  
  djv_ui::callback::Signal<bool> magnify_signal;
  djv_ui::callback::Signal<bool> color_picker_signal;
  djv_ui::callback::Signal<bool> histogram_signal;
  djv_ui::callback::Signal<bool> info_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void menu_update(djv_ui::menu::Group *);
  
  void toolbar_update();

  //@}
  
private:

  // Callbacks.
  
  DJV_CALLBACK(Group, magnify_callback, bool);
  DJV_CALLBACK(Group, color_picker_callback, bool);
  DJV_CALLBACK(Group, histogram_callback, bool);
  DJV_CALLBACK(Group, info_callback, bool);
  
  DJV_FL_WIDGET_CALLBACK(Group, _magnify_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _color_picker_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _histogram_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _info_callback);

  // Variables.
  
  struct _Group;
  std::auto_ptr<_Group> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  menu_title,
  menu_magnify,
  menu_color_picker,
  menu_histogram,
  menu_info;

extern const String
  tooltip_magnify,
  tooltip_color_picker,
  tooltip_histogram,
  tooltip_info;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

