/*------------------------------------------------------------------------------
 bin/djv_view/playback_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "playback_base.h"

#include "playback_group.h"
#include "playback_prefs.h"
#include <FL/Fl.H>
#include <algorithm>

namespace djv_bin {
namespace view {
namespace playback {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(PLAYBACK, label_playback())
_DJV_STRING_OPERATOR_LABEL(LOOP, label_loop())
_DJV_STRING_OPERATOR_LABEL(TIMER, label_timer())
_DJV_STRING_OPERATOR_LABEL(LAYOUT, label_layout())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_playback()
{
  static const List<String> data = List<String>() <<
    "Stop" <<
    "Forward" <<
    "Reverse";
  
  DJV_ASSERT(data.size() == _PLAYBACK_SIZE);
  
  return data;
}

const List<String> & label_loop()
{
  static const List<String> data = List<String>() <<
    "Once" <<
    "Repeat" <<
    "Ping-Pong";
  
  DJV_ASSERT(data.size() == _LOOP_SIZE);
  
  return data;
}

const List<String> & label_timer()
{
  static const List<String> data = List<String>() <<
    "Sleep" <<
    "Timeout";
  
  DJV_ASSERT(data.size() == _TIMER_SIZE);
  
  return data;
}

const List<String> & label_layout()
{
  static const List<String> data = List<String>() <<
    "Default" <<
    "Left" <<
    "Center" <<
    "Minimal";
  
  DJV_ASSERT(data.size() == _LAYOUT_SIZE);
  
  return data;
}

extern const String
  menu_title = "Playback",
  menu_stop = "&Stop",
  menu_forward = "&Forward",
  menu_reverse = "&Reverse",
  menu_loop = "&Loop",
  menu_start = "S&tart",
  menu_start_abs = "Start Absolute",
  menu_end = "&End",
  menu_end_abs = "End Absolute",
  menu_prev = "&Previous",
  menu_prev_x10 = "Previous X10",
  menu_prev_x100 = "Previous X100",
  menu_next = "&Next",
  menu_next_x10 = "Next X10",
  menu_next_x100 = "Next X100",
  menu_in_out = "Use &In/Out Points",
  menu_in_mark = "In &Mark",
  menu_in_reset = "In Reset",
  menu_out_mark = "Out Mar&k",
  menu_out_reset = "Out Reset",
  menu_every_frame = "Ever&y Frame",
  menu_layout = "Layout";

extern const String
  tooltip_stop ="Stop playback\n\nShortcut: %%",
  tooltip_forward = "Forward playback\n\nShortcut: %%",
  tooltip_reverse = "Reverse playback\n\nShortcut: %%",
  tooltip_playback_shuttle =
  "Playback shuttle\n\nClick and drag to start playback; the speed is "
  "determined by how far you drag.",
  tooltip_loop =
  "Loop mode: %%\n\nClick to cycle through values: %%\n\nShortcut: %%",
  tooltip_frame = "Current frame",
  tooltip_frame_slider = "Frame slider",
  tooltip_frame_shuttle =
  "Frame shuttle\n\nClick and drag to change the current frame.",
  tooltip_start = "Start frame / in point",
  tooltip_start_button = "Start frame\n\nShortcut: %%",
  tooltip_end = "End frame / out point",
  tooltip_end_button = "End frame\n\nShortcut: %%",
  tooltip_prev = "Previous frame\n\nShortcut: %%",
  tooltip_next = "Next frame\n\nShortcut: %%",
  tooltip_in_out = "Enable in/out points\n\nShortcut: %%",
  tooltip_in_mark = "Mark in point\n\nShortcut: %%",
  tooltip_in_reset = "Reset in point\n\nShortcut: %%",
  tooltip_out_mark = "Mark out point\n\nShortcut: %%",
  tooltip_out_reset = "Reset out point\n\nShortcut: %%",
  tooltip_duration = "Playback duration",
  tooltip_speed = "Playback speed",
  tooltip_speed_real = "Real playback speed",
  tooltip_every_frame = "Playback every frame";

extern const String
  prefs_title = "Playback",
  prefs_group = "Playback Options",
  prefs_loop = "Loop Mode",
  prefs_start = "Start playback when opening files",
  prefs_every_frame = "Play every frame",
  prefs_frame_controls_stop_playback = "Frame controls stop playback",
  prefs_ui_group = "Playback User-Interface",
  prefs_layout = "Layout";

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  _timer(TIMER_SLEEP),
  _timer_resolution(0.001)
{}

Global::~Global()
{
  Fl::remove_idle(timer_callback, this);
  Fl::remove_timeout(timer_callback, this);
}

void Global::timer(TIMER in)
{
  _timer = in;
}

TIMER Global::timer() const
{
  return _timer;
}

void Global::timer_resolution(double in)
{
  _timer_resolution = in;
}

double Global::timer_resolution() const
{
  return _timer_resolution;
}

void Global::timer_add(Group * in)
{
  DJV_DEBUG("Global::timer_add");
  
  _timer_list += in;
  
  if (1 == _timer_list.size())
    switch (_timer)
    {
      case TIMER_SLEEP:
        _timer_time.start();
        Fl::add_idle(timer_callback, this);
        break;
      
      case TIMER_TIMEOUT:
        Fl::add_timeout(_timer_resolution, timer_callback, this);
        break;
    }
}

void Global::timer_del(Group * in)
{
  DJV_DEBUG("Global::timer_del");

  List<playback::Group *>::iterator i = std::find(
    _timer_list.begin(),
    _timer_list.end(),
    in
  );
  if (i != _timer_list.end())
    _timer_list.erase(i);
  
  if (! _timer_list.size())
  {
    Fl::remove_idle(timer_callback, this);
    Fl::remove_timeout(timer_callback, this);
  }
}

void Global::timer_callback()
{
  DJV_DEBUG("Global::timer_callback");
  DJV_DEBUG_PRINT("size = " << static_cast<int>(_timer_list.size()));
  
  for (list::size_type i = 0; i < _timer_list.size(); ++i)
    _timer_list[i]->idle();
  
  switch (_timer)
  {
    case TIMER_SLEEP:
      while (_timer_time.seconds() < _timer_resolution)
      {
        djv_base::time::sleep(0.0);
        _timer_time.check();
      }
      _timer_time.start();
      break;
      
    case TIMER_TIMEOUT:
      Fl::repeat_timeout(_timer_resolution, timer_callback, this);
      break;
  }
}

//------------------------------------------------------------------------------

}
}
}

