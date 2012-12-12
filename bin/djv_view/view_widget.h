/*------------------------------------------------------------------------------
 bin/djv_view/view_widget.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_VIEW_WIDGET_H
#define DJV_VIEW_VIEW_WIDGET_H

#include "input_base.h"
#include "view_base.h"
#include <djv_ui/image_view.h>

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
//!\class Widget
//
//! View widget.
//------------------------------------------------------------------------------

class Widget : public djv_ui::image::View
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Widget();
  
  void del();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Zoom
  //@{
  
  void zoom(double);
  void zoom(double, const V2i & focus);
  void zoom_focus(double);
  
  double zoom() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Grid
  //@{
  
  void grid(GRID);
  void grid_color(const djv_image::Color &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name HUD
  //@{
  
  void hud(bool);
  void hud_info(const Hud_Info &);
  void hud_show(const List<bool> &);
  void hud_color(const djv_image::Color &);
  void hud_background(HUD_BACKGROUND);
  void hud_background_color(const djv_image::Color &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{
  
  djv_ui::callback::Signal<const V2i &> pick_signal;
  djv_ui::callback::Signal<input::MOUSE_WHEEL> mouse_wheel_signal;
  djv_ui::callback::Signal<int> mouse_wheel_value_signal;
  djv_ui::callback::Signal<const File &> file_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  int handle(int);
  void draw();
  const V2i & mouse() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  const V2i & size_hint() const;
  const V2i & size_min() const;

  //@}
  
private:

  void draw_grid();
  
  void draw_hud();
  
  static void draw_hud(
    const String &,
    const V2i & position,
    const djv_image::Color & text_color,
    HUD_BACKGROUND,
    const djv_image::Color & background_color
  );
  
  DJV_FL_CALLBACK(Widget, mouse_wheel_timeout_callback);
  
  V2i _view_tmp;
  double _zoom_tmp;
  bool _mouse_wheel;
  int _mouse_wheel_tmp;
  GRID _grid;
  djv_image::Color _grid_color;
  bool _hud;
  Hud_Info _hud_info;
  List<bool> _hud_show;
  djv_image::Color _hud_color;
  HUD_BACKGROUND _hud_background;
  djv_image::Color _hud_background_color;
  bool _inside;
  V2i _mouse, _mouse_td;
  V2i _size_min;
};

//------------------------------------------------------------------------------

}
}
}

#endif

