/*------------------------------------------------------------------------------
 bin/djv_view/window.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_WINDOW_H
#define DJV_VIEW_WINDOW_H

#include "image_base.h"
#include "input_base.h"
#include "playback_base.h"
#include "view_base.h"
#include "window_base.h"
#include <djv_ui/menu_def.h>
#include <djv_ui/window.h>
#include <djv_image/image.h>

namespace djv_bin {
namespace view {
namespace window {

//------------------------------------------------------------------------------
//!\class Window
//
//! Main window.
//------------------------------------------------------------------------------

class Window : public djv_ui::window::Window
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Window(const Window * copy = 0);
  
  ~Window();
  
  void del();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name File Options
  //@{
  
  void file_open(const File &, bool init = true);
  void file_seq_auto(bool);
  void file_layer(int);
  void file_proxy(djv_image::data::PROXY);
  void file_cache(bool);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Window Options
  //@{
  
  void window_fullscreen(bool);
  void window_toolbar(const List<bool> &);
  
  const List<bool> & window_toolbar() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name View Options
  //@{

  void view_grid(view::GRID);
  void view_hud(bool);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Playback Options
  //@{

  void playback(playback::PLAYBACK);
  void playback_frame(int64_t);
  void playback_speed(const djv_image::time::Speed &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  int handle(int);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  void dirty();
  void geom(const Box2i &);
  
  const Box2i & geom() const;

  //@}
  
private:

  void window_fit();
  void menu_update(djv_ui::menu::Group *);
  void toolbar_update();

  djv_image::gl_data::Option image_option() const;
  
  V2i image_pick() const;

  // Callbacks.
  
  DJV_CALLBACK(Window, file_open_callback, const File &);
  DJV_CALLBACK(Window, file_reload_callback, bool);
  DJV_CALLBACK(Window, file_save_callback, const File &);
  DJV_CALLBACK(Window, file_save_frame_callback, const File &);
  
  DJV_CALLBACK(Window, window_new_callback, bool);
  DJV_CALLBACK(Window, window_copy_callback, bool);
  DJV_CALLBACK(Window, window_close_callback, bool);
  DJV_CALLBACK(Window, window_close2_callback, bool);
  DJV_CALLBACK(Window, window_fit_callback, bool);
  DJV_CALLBACK(Window, window_fullscreen_callback, bool);
  DJV_CALLBACK(Window, window_toolbar_callback, const List<bool> &);
  DJV_CALLBACK(Window, window_toolbar_buttons_callback, bool);
  DJV_CALLBACK(Window, window_toolbar_playback_callback, bool);
  DJV_CALLBACK(Window, window_toolbar_info_callback, bool);
  DJV_FL_WIDGET_CALLBACK(Window, _window_new_callback);
  DJV_FL_WIDGET_CALLBACK(Window, _window_copy_callback);
  DJV_FL_WIDGET_CALLBACK(Window, _window_close_callback);
  DJV_FL_WIDGET_CALLBACK(Window, _window_fit_callback);
  DJV_FL_WIDGET_CALLBACK(Window, _window_fullscreen_callback);
  DJV_FL_WIDGET_CALLBACK(Window, _window_toolbar_buttons_callback);
  DJV_FL_WIDGET_CALLBACK(Window, _window_toolbar_playback_callback);
  DJV_FL_WIDGET_CALLBACK(Window, _window_toolbar_info_callback);
  
  DJV_CALLBACK(Window, view_callback, bool);
  DJV_CALLBACK(Window, view_move_callback, const V2i &);
  DJV_CALLBACK(Window, view_center_callback, bool);
  DJV_CALLBACK(Window, view_zoom_callback, double);
  DJV_CALLBACK(Window, view_zoom_reset_callback, bool);
  DJV_CALLBACK(Window, view_reset_callback, bool);
  DJV_CALLBACK(Window, view_fit_callback, bool);
  DJV_CALLBACK(Window, view_grid_callback, view::GRID);
  DJV_CALLBACK(Window, view_hud_callback, bool);
  DJV_CALLBACK(Window, view_pick_callback, const V2i &);
  DJV_CALLBACK(Window, view_overlay_callback, bool);
  
  DJV_CALLBACK(Window, image_callback, bool);
  DJV_CALLBACK(Window, image_redraw_callback, bool);
  DJV_CALLBACK(Window, image_resize_callback, bool);
  DJV_CALLBACK(Window, image_frame_store_callback, bool);
  DJV_CALLBACK(Window, image_display_profile_callback, bool);
  DJV_CALLBACK(Window, image_display_profile_value_callback,
    const image::Display_Profile &);

  DJV_CALLBACK(Window, tool_magnify_callback, bool);
  DJV_CALLBACK(Window, tool_color_picker_callback, bool);
  DJV_CALLBACK(Window, tool_histogram_callback, bool);
  DJV_CALLBACK(Window, tool_info_callback, bool);
  
  DJV_CALLBACK(Window, mouse_wheel_callback, input::MOUSE_WHEEL);
  DJV_CALLBACK(Window, mouse_wheel_value_callback, int);
  
  DJV_CALLBACK(Window, cache_callback, const List<int64_t> &);
  
  DJV_CALLBACK(Window, group_callback, bool);
  
  // Updates.
  
  void file_update();
  void window_toolbar_update();
  void window_fullscreen_update();
  void window_resize_update();
  void view_update();
  void view_resize_update();
  void view_overlay_update();
  void image_update();
  void image_redraw_update();
  void image_dialog_update();
  void image_focus_update();
  void image_pick_update();
  void tool_dialog_update();
  void cache_update();
  void group_update();

  // Variables.
  
  input::MOUSE_WHEEL _mouse_wheel;
  std::auto_ptr<file::Group> _file;
  bool _window_fullscreen;
  List<bool> _window_toolbar;
  std::auto_ptr<view::Group> _view;
  std::auto_ptr<image::Group> _image;
  const djv_image::Image * _image_p;
  djv_image::Image _image_tmp;
  V2i _image_pick;
  djv_image::Color _image_sample;
  std::auto_ptr<playback::Group> _playback;
  int64_t _playback_frame_tmp;
  double _playback_speed_tmp;
  std::auto_ptr<tool::Group> _tool;
  std::auto_ptr<help::Group> _help;
  
  struct _Window;
  std::auto_ptr<_Window> _p;
  
  bool _is_valid;
};

//------------------------------------------------------------------------------
//!\class Factory
//
//! Main window factory.
//------------------------------------------------------------------------------

class Factory
{
public:

  ~Factory();

  Window * add(const Window * copy = 0);
  void del(Window *);
  
  const List<Window *> list() const;

private:
  
  List<Window *> _list;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  label_toolbar_info_pixel,
  label_toolbar_info_pixel_size,
  label_toolbar_info_image,
  label_toolbar_info_cache;

extern const String
  menu_title,
  menu_new,
  menu_copy,
  menu_close,
  menu_fit,
  menu_fullscreen,
  menu_toolbar_buttons,
  menu_toolbar_playback,
  menu_toolbar_info;

extern const String
  tooltip_new,
  tooltip_copy,
  tooltip_close,
  tooltip_fit,
  tooltip_fullscreen,
  tooltip_info_pixel,
  tooltip_info_image,
  tooltip_info_cache;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global factory.

Factory * factory();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

