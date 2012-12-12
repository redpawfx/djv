/*------------------------------------------------------------------------------
 bin/djv_view/playback_group.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_PLAYBACK_GROUP_H
#define DJV_VIEW_PLAYBACK_GROUP_H

#include "playback_base.h"
#include <djv_ui/layout_row_def.h>
#include <djv_ui/menu_def.h>

namespace djv_bin {
namespace view {
namespace playback {

//------------------------------------------------------------------------------
//!\class Group
//
//! Playback group.
//------------------------------------------------------------------------------

class Group : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Group(
    djv_ui::menu::Menu *,
    djv_ui::layout_row::Layout_Row_Base * toolbar,
    Group * copy = 0
  );
  
  ~Group();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Playback
  //@{
  
  void playback(PLAYBACK);

  void loop(LOOP);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Time
  //@{
  
  void time(const djv_image::Time &);
  
  const djv_image::Time & time() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Speed
  //@{
  
  void speed(const djv_image::time::Speed &);
  
  const djv_image::time::Speed & speed() const;
  double speed_real() const;
  bool dropped_frames() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Frame
  //@{
  
  void frame(int64_t, bool in_out = true);
  
  int64_t frame() const;
  int64_t in() const;
  int64_t out() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Cache
  //@{
  
  void cached_frames(const List<int64_t> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{
  
  djv_ui::callback::Signal<bool> image_signal;
  djv_ui::callback::Signal<bool> overlay_signal;
  djv_ui::callback::Signal<bool> update_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void menu_update(djv_ui::menu::Group *);
  
  void toolbar_update();
  void toolbar_info_update(bool);
  
  void idle();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void layout(LAYOUT);
  
  LAYOUT layout() const;

  //@}
  
private:

  int64_t frame_min() const;
  int64_t frame_max() const;
  
  // Callbacks.
  
  DJV_CALLBACK(Group, playback_callback, PLAYBACK);
  DJV_CALLBACK(Group, stop_callback, bool);
  DJV_CALLBACK(Group, forward_callback, bool);
  DJV_CALLBACK(Group, reverse_callback, bool);
  DJV_CALLBACK(Group, playback_shuttle_callback, bool);
  DJV_CALLBACK(Group, playback_shuttle_value_callback, int);
  DJV_CALLBACK(Group, loop_callback, int);
  DJV_CALLBACK(Group, speed_callback, const djv_image::time::Speed &);
  DJV_CALLBACK(Group, every_frame_callback, bool);
  DJV_CALLBACK(Group, frame_callback, int64_t);
  DJV_CALLBACK(Group, frame_stop_callback, int64_t);
  DJV_CALLBACK(Group, frame_slider_callback, int);
  DJV_CALLBACK(Group, frame_shuttle_callback, bool);
  DJV_CALLBACK(Group, frame_shuttle_value_callback, int);
  DJV_CALLBACK(Group, frame_push_callback, bool);
  DJV_CALLBACK(Group, start_callback, bool);
  DJV_CALLBACK(Group, start_abs_callback, bool);
  DJV_CALLBACK(Group, end_callback, bool);
  DJV_CALLBACK(Group, end_abs_callback, bool);
  DJV_CALLBACK(Group, prev_callback, bool);
  DJV_CALLBACK(Group, next_callback, bool);
  DJV_CALLBACK(Group, in_out_callback, bool);
  DJV_CALLBACK(Group, in_callback, int64_t);
  DJV_CALLBACK(Group, in_mark_callback, bool);
  DJV_CALLBACK(Group, in_reset_callback, bool);
  DJV_CALLBACK(Group, out_callback, int64_t);
  DJV_CALLBACK(Group, out_mark_callback, bool);
  DJV_CALLBACK(Group, out_reset_callback, bool);
  DJV_CALLBACK(Group, layout_callback, LAYOUT);
  
  DJV_FL_WIDGET_CALLBACK(Group, _stop_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _forward_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _reverse_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _loop_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _every_frame_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _start_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _start_abs_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _end_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _end_abs_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _prev_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _prev_x10_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _prev_x100_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _next_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _next_x10_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _next_x100_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _in_out_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _in_goto_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _in_mark_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _in_reset_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _out_goto_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _out_mark_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _out_reset_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _layout_callback);
  
  // Update.
  
  void playback_update();
  void time_update();
  void frame_update();
  void speed_update();
  void widget_update();
  void layout_update();
  
  // Variables.
  
  PLAYBACK _playback;
  
  LOOP _loop;
  
  djv_image::Time _time;

  djv_image::time::Speed _speed;
  double _speed_real;
  bool _dropped_frames;
  bool _dropped_frames_tmp;
  bool _every_frame;
  
  int64_t _frame;
  int64_t _frame_tmp;
  
  bool _in_out;
  int64_t _in, _out;
  
  bool _shuttle;
  double _shuttle_speed;
  
  bool _idle_pause;
  bool _idle_init;
  djv_base::time::Timer _idle_timer;
  djv_base::time::Timer _speed_timer;
  uint64_t _idle_frame;
  uint64_t _speed_counter;
  
  LAYOUT _layout;
  
  struct _Group;
  std::auto_ptr<_Group> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  menu_title,
  menu_stop,
  menu_forward,
  menu_reverse,
  menu_loop,
  menu_every_frame,
  menu_start,
  menu_start_abs,
  menu_end,
  menu_end_abs,
  menu_prev,
  menu_prev_x10,
  menu_prev_x100,
  menu_next,
  menu_next_x10,
  menu_next_x100,
  menu_in_out,
  menu_in_mark,
  menu_in_reset,
  menu_out_mark,
  menu_out_reset,
  menu_layout;

extern const String
  tooltip_title,
  tooltip_stop,
  tooltip_forward,
  tooltip_reverse,
  tooltip_playback_shuttle,
  tooltip_loop,
  tooltip_speed,
  tooltip_speed_real,
  tooltip_every_frame,
  tooltip_frame,
  tooltip_frame_slider,
  tooltip_frame_shuttle,
  tooltip_start,
  tooltip_start_button,
  tooltip_end,
  tooltip_end_button,
  tooltip_prev,
  tooltip_next,
  tooltip_duration,
  tooltip_in_out,
  tooltip_in_mark,
  tooltip_in_reset,
  tooltip_out_mark,
  tooltip_out_reset;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

