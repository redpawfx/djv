/*------------------------------------------------------------------------------
 bin/djv_view/view_group.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_VIEW_GROUP_H
#define DJV_VIEW_VIEW_GROUP_H

#include "image_base.h"
#include "view_base.h"
#include <djv_ui/callback.h>
#include <djv_ui/layout_row_def.h>
#include <djv_ui/menu_def.h>

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
//!\class Group
//
//! View group.
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
  
  void zoom(double);

  void grid(GRID);
  
  GRID grid() const;

  void hud(bool);

  bool hud() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void menu_update(djv_ui::menu::Group *);
  
  void toolbar_update();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{
  
  djv_ui::callback::Signal<const V2i &> move_signal;
  djv_ui::callback::Signal<bool> center_signal;
  djv_ui::callback::Signal<double> zoom_signal;
  djv_ui::callback::Signal<bool> zoom_reset_signal;
  djv_ui::callback::Signal<bool> reset_signal;
  djv_ui::callback::Signal<bool> fit_signal;
  djv_ui::callback::Signal<bool> overlay_signal;
  djv_ui::callback::Signal<bool> update_signal;
  
  //@}
  
private:

  // Callbacks.

  DJV_CALLBACK(Group, zoom_in_callback, bool);
  DJV_CALLBACK(Group, zoom_out_callback, bool);
  DJV_CALLBACK(Group, zoom_inc_callback, bool);
  DJV_CALLBACK(Group, zoom_reset_callback, bool);
  DJV_CALLBACK(Group, zoom_dec_callback, bool);
  DJV_CALLBACK(Group, fit_callback, bool);
  DJV_CALLBACK(Group, grid_callback, GRID);
  DJV_CALLBACK(Group, hud_callback, bool);
  
  DJV_FL_WIDGET_CALLBACK(Group, _left_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _right_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _up_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _down_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _center_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _zoom_in_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _zoom_out_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _zoom_reset_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _reset_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _fit_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _grid_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _hud_callback);

  // Variables.
  
  GRID _grid;
  bool _hud;
  
  struct _Group;
  std::auto_ptr<_Group> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  menu_title,
  menu_left,
  menu_right,
  menu_up,
  menu_down,
  menu_center,
  menu_zoom_in,
  menu_zoom_out,
  menu_zoom_reset,
  menu_reset,
  menu_fit,
  menu_grid,
  menu_hud;

extern const String
  toolbar_zoom,
  toolbar_zoom_format;

extern const String
  tooltip_zoom,
  tooltip_zoom_in,
  tooltip_zoom_out,
  tooltip_zoom_reset,
  tooltip_fit;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

