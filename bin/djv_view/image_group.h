/*------------------------------------------------------------------------------
 bin/djv_view/image_group.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_IMAGE_GROUP_H
#define DJV_VIEW_IMAGE_GROUP_H

#include "image_base.h"
#include <djv_ui/callback.h>
#include <djv_ui/layout_row_def.h>
#include <djv_ui/menu_def.h>

namespace djv_bin {
namespace view {
namespace image {

//------------------------------------------------------------------------------
//!\class Group
//
//! Image group.
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
  //!\name Options
  //@{
  
  bool frame_store() const;

  const V2b & mirror() const;

  SCALE scale() const;

  ROTATE rotate() const;

  bool color_profile() const;
  
  void display_profile(const Display_Profile &);

  const Display_Profile & display_profile() const;

  CHANNEL channel() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{
  
  djv_ui::callback::Signal<bool> redraw_signal;
  djv_ui::callback::Signal<bool> resize_signal;
  djv_ui::callback::Signal<bool> frame_store_signal;
  djv_ui::callback::Signal<bool> display_profile_signal;
  djv_ui::callback::Signal<const Display_Profile &>
    display_profile_value_signal;
  djv_ui::callback::Signal<bool> update_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void menu_update(djv_ui::menu::Group *);
  
  void toolbar_update();

  //@}
  
private:

  // Callbacks.
  
  DJV_CALLBACK(Group, frame_store_callback, bool);
  DJV_CALLBACK(Group, frame_store_load_callback, bool);
  DJV_CALLBACK(Group, mirror_callback, const V2b &);
  DJV_CALLBACK(Group, scale_callback, SCALE);
  DJV_CALLBACK(Group, rotate_callback, ROTATE);
  DJV_CALLBACK(Group, color_profile_callback, bool);
  DJV_CALLBACK(Group, display_profile_callback, bool);
  DJV_CALLBACK(Group, display_profile_callback, const Display_Profile &);
  DJV_CALLBACK(Group, display_profile_reset_callback, bool);
  DJV_CALLBACK(Group, display_profile_add_callback, const String &);
  DJV_CALLBACK(Group, display_profile_del_callback, const List<int> &);
  DJV_CALLBACK(Group, display_profile_del_all_callback, bool);
  DJV_CALLBACK(Group, display_profile_list_callback, bool);
  DJV_CALLBACK(Group, channel_callback, CHANNEL);
  
  DJV_FL_WIDGET_CALLBACK(Group, _frame_store_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _frame_store_load_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _mirror_h_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _mirror_v_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _scale_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _rotate_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _color_profile_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _display_profile_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _display_profile_reset_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _display_profile_add_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _display_profile_del_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _display_profile_del_all_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _display_profile_list_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _channel_callback);

  // Variables.
  
  bool _frame_store;
  V2b _mirror;
  SCALE _scale;
  ROTATE _rotate;
  bool _color_profile;
  Display_Profile _display_profile;
  CHANNEL _channel;
  
  struct _Group;
  std::auto_ptr<_Group> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  menu_title,
  menu_frame_store,
  menu_frame_store_load,
  menu_mirror_h,
  menu_mirror_v,
  menu_scale,
  menu_rotate,
  menu_color_profile,
  menu_display_profile,
  menu_display_profile_show,
  menu_display_profile_reset,
  menu_display_profile_add,
  menu_display_profile_del,
  menu_display_profile_del_all,
  menu_channel;

extern const String
  tooltip_display_profile;

extern const String
  dialog_display_profile_add,
  dialog_display_profile_del,
  dialog_display_profile_del_all;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

