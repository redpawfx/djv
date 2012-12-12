/*------------------------------------------------------------------------------
 bin/djv_view/view_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_VIEW_PREFS_H
#define DJV_VIEW_VIEW_PREFS_H

#include "view_base.h"
#include <djv_ui/widget.h>

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! View preferences.
//------------------------------------------------------------------------------

class Prefs : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Background
  //@{
  
  void background(const djv_image::Color &);
  
  const djv_image::Color & background() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Resize
  //@{
  
  void resize(RESIZE);
  
  RESIZE resize() const;
  
  djv_ui::callback::Signal<RESIZE> resize_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Grid
  //@{
  
  void grid(GRID);
  void grid_color(const djv_image::Color &);
  
  GRID grid() const;
  const djv_image::Color & grid_color() const;
  
  djv_ui::callback::Signal<GRID> grid_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name HUD
  //@{
  
  void hud(bool);
  void hud_show(HUD_SHOW, bool);
  void hud_color(const djv_image::Color &);
  void hud_background(HUD_BACKGROUND);
  void hud_background_color(const djv_image::Color &);
  
  bool hud() const;
  List<bool> hud_show() const;
  bool hud_show(HUD_SHOW in) const;
  const djv_image::Color & hud_color() const;
  HUD_BACKGROUND hud_background() const;
  const djv_image::Color & hud_background_color() const;
  
  djv_ui::callback::Signal<bool> hud_signal;
  
  //@}
  
private:

  djv_image::Color _background;
  RESIZE _resize;
  GRID _grid;
  djv_image::Color _grid_color;
  bool _hud;
  List<bool> _hud_show;
  djv_image::Color _hud_color;
  HUD_BACKGROUND _hud_background;
  djv_image::Color _hud_background_color;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! View preferences widget.
//------------------------------------------------------------------------------

class Prefs_Widget : public djv_ui::Widget
{
public:

  Prefs_Widget();
  
  ~Prefs_Widget();

private:

  DJV_CALLBACK(Prefs_Widget, background_callback, const djv_image::Color &);
  DJV_CALLBACK(Prefs_Widget, resize_callback, int);
  DJV_CALLBACK(Prefs_Widget, grid_callback, int);
  DJV_CALLBACK(Prefs_Widget, grid_callback, const djv_image::Color &);
  DJV_CALLBACK(Prefs_Widget, hud_callback, bool);
  DJV_CALLBACK(Prefs_Widget, hud_show_callback, const List<bool> &);
  DJV_CALLBACK(Prefs_Widget, hud_color_callback, const djv_image::Color &);
  DJV_CALLBACK(Prefs_Widget, hud_background_callback, int);
  DJV_CALLBACK(Prefs_Widget, hud_background_color_callback,
    const djv_image::Color &);
  
  struct _Prefs_Widget;
  std::auto_ptr<_Prefs_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  prefs_title,
  prefs_group,
  prefs_background,
  prefs_resize_group,
  prefs_grid_group,
  prefs_grid_color,
  prefs_hud_group,
  prefs_hud,
  prefs_hud_show,
  prefs_hud_color,
  prefs_hud_background,
  prefs_hud_background_color;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

