/*------------------------------------------------------------------------------
 lib/djv_ui/icon.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "icon.h"

#include <FL/Fl_Pixmap.H>

#include "../../icon/file.xpm"
#include "../../icon/file_open.xpm"
#include "../../icon/file_reload.xpm"
#include "../../icon/file_close.xpm"
#include "../../icon/directory.xpm"
#include "../../icon/directory_up.xpm"
#include "../../icon/directory_prev.xpm"
#include "../../icon/directory_reload.xpm"
#include "../../icon/window_new.xpm"
#include "../../icon/window_copy.xpm"
#include "../../icon/window_close.xpm"
#include "../../icon/window_fit.xpm"
#include "../../icon/window_fullscreen.xpm"
#include "../../icon/view_zoom_in.xpm"
#include "../../icon/view_zoom_out.xpm"
#include "../../icon/view_zoom_reset.xpm"
#include "../../icon/view_fit.xpm"
#include "../../icon/shuttle_0.xpm"
#include "../../icon/shuttle_1.xpm"
#include "../../icon/shuttle_2.xpm"
#include "../../icon/shuttle_3.xpm"
#include "../../icon/shuttle_4.xpm"
#include "../../icon/shuttle_5.xpm"
#include "../../icon/shuttle_6.xpm"
#include "../../icon/shuttle_7.xpm"
#include "../../icon/playback_stop.xpm"
#include "../../icon/playback_forward.xpm"
#include "../../icon/playback_reverse.xpm"
#include "../../icon/playback_start.xpm"
#include "../../icon/playback_end.xpm"
#include "../../icon/playback_prev.xpm"
#include "../../icon/playback_next.xpm"
#include "../../icon/playback_shuttle_0.xpm"
#include "../../icon/playback_shuttle_1.xpm"
#include "../../icon/playback_shuttle_2.xpm"
#include "../../icon/playback_shuttle_3.xpm"
#include "../../icon/playback_shuttle_4.xpm"
#include "../../icon/playback_shuttle_5.xpm"
#include "../../icon/playback_shuttle_6.xpm"
#include "../../icon/playback_shuttle_7.xpm"
#include "../../icon/playback_frame_shuttle_0.xpm"
#include "../../icon/playback_frame_shuttle_1.xpm"
#include "../../icon/playback_frame_shuttle_2.xpm"
#include "../../icon/playback_frame_shuttle_3.xpm"
#include "../../icon/playback_frame_shuttle_4.xpm"
#include "../../icon/playback_frame_shuttle_5.xpm"
#include "../../icon/playback_frame_shuttle_6.xpm"
#include "../../icon/playback_frame_shuttle_7.xpm"
#include "../../icon/playback_loop_once.xpm"
#include "../../icon/playback_loop_repeat.xpm"
#include "../../icon/playback_loop_ping_pong.xpm"
#include "../../icon/playback_in_out.xpm"
#include "../../icon/tool_check.xpm"
#include "../../icon/tool_radio.xpm"
#include "../../icon/tool_start.xpm"
#include "../../icon/tool_end.xpm"
#include "../../icon/tool_x.xpm"
#include "../../icon/tool_lock.xpm"
#include "../../icon/tool_add.xpm"
#include "../../icon/tool_inc.xpm"
#include "../../icon/tool_dec.xpm"
#include "../../icon/color_wheel.xpm"
#include "../../icon/grey_ramp.xpm"
#include "../../icon/magnify.xpm"
#include "../../icon/color_picker.xpm"
#include "../../icon/histogram.xpm"
//#include "../../icon/info.xpm"

namespace djv_ui {
namespace icon {

//------------------------------------------------------------------------------
// Icon
//------------------------------------------------------------------------------

namespace {

struct Icon { String name; char ** data; } icon [] =
{
  { "file", file_xpm },
  { "file_open", file_open_xpm },
  { "file_reload", file_reload_xpm },
  { "file_close", file_close_xpm },
  { "directory", directory_xpm },
  { "directory_up", directory_up_xpm },
  { "directory_prev", directory_prev_xpm },
  { "directory_reload", directory_reload_xpm },
  { "window_new", window_new_xpm },
  { "window_copy", window_copy_xpm },
  { "window_close", window_close_xpm },
  { "window_fit", window_fit_xpm },
  { "window_fullscreen", window_fullscreen_xpm },
  { "view_zoom_in", view_zoom_in_xpm },
  { "view_zoom_out", view_zoom_out_xpm },
  { "view_zoom_reset", view_zoom_reset_xpm },
  { "view_fit", view_fit_xpm },
  { "shuttle_0", shuttle_0_xpm },
  { "shuttle_1", shuttle_1_xpm },
  { "shuttle_2", shuttle_2_xpm },
  { "shuttle_3", shuttle_3_xpm },
  { "shuttle_4", shuttle_4_xpm },
  { "shuttle_5", shuttle_5_xpm },
  { "shuttle_6", shuttle_6_xpm },
  { "shuttle_7", shuttle_7_xpm },
  { "playback_stop", playback_stop_xpm },
  { "playback_forward", playback_forward_xpm },
  { "playback_reverse", playback_reverse_xpm },
  { "playback_start", playback_start_xpm },
  { "playback_end", playback_end_xpm },
  { "playback_prev", playback_prev_xpm },
  { "playback_next", playback_next_xpm },
  { "playback_shuttle_0", playback_shuttle_0_xpm },
  { "playback_shuttle_1", playback_shuttle_1_xpm },
  { "playback_shuttle_2", playback_shuttle_2_xpm },
  { "playback_shuttle_3", playback_shuttle_3_xpm },
  { "playback_shuttle_4", playback_shuttle_4_xpm },
  { "playback_shuttle_5", playback_shuttle_5_xpm },
  { "playback_shuttle_6", playback_shuttle_6_xpm },
  { "playback_shuttle_7", playback_shuttle_7_xpm },
  { "playback_frame_shuttle_0", playback_frame_shuttle_0_xpm },
  { "playback_frame_shuttle_1", playback_frame_shuttle_1_xpm },
  { "playback_frame_shuttle_2", playback_frame_shuttle_2_xpm },
  { "playback_frame_shuttle_3", playback_frame_shuttle_3_xpm },
  { "playback_frame_shuttle_4", playback_frame_shuttle_4_xpm },
  { "playback_frame_shuttle_5", playback_frame_shuttle_5_xpm },
  { "playback_frame_shuttle_6", playback_frame_shuttle_6_xpm },
  { "playback_frame_shuttle_7", playback_frame_shuttle_7_xpm },
  { "playback_loop_once", playback_loop_once_xpm },
  { "playback_loop_repeat", playback_loop_repeat_xpm },
  { "playback_loop_ping_pong", playback_loop_ping_pong_xpm },
  { "playback_in_out", playback_in_out_xpm },
  { "tool_check", tool_check_xpm },
  { "tool_radio", tool_radio_xpm },
  { "tool_start", tool_start_xpm },
  { "tool_end", tool_end_xpm },
  { "tool_x", tool_x_xpm },
  { "tool_lock", tool_lock_xpm },
  { "tool_add", tool_add_xpm },
  { "tool_inc", tool_inc_xpm },
  { "tool_dec", tool_dec_xpm },
  { "color_wheel", color_wheel_xpm },
  { "grey_ramp", grey_ramp_xpm },
  { "magnify", magnify_xpm },
  { "color_picker", color_picker_xpm },
  { "histogram", histogram_xpm },
  { "info", file_xpm }
};

const uint icon_size = sizeof(icon) / sizeof(Icon);

}

//------------------------------------------------------------------------------
// Factory
//------------------------------------------------------------------------------

Factory::Factory() : _empty(0, 0, 0)
{
  for (uint i = 0; i < icon_size; ++i)
    add(icon[i].name, new Fl_Pixmap((const char **)icon[i].data));
}

Factory::~Factory()
{
  for (List::iterator i = _list.begin(); i != _list.end(); ++i)
    delete i->second;
}

void Factory::add(const String & name, Fl_Image * icon)
{
  _list[name] = icon;
}

Fl_Image * Factory::get(const String & name) const
{
  if (_list.find(name) == _list.end())
    return const_cast<Fl_Image *>(&_empty);
  
  return const_cast<Factory *>(this)->_list[name];
}

Fl_Image * get(const String & name)
{
  return global()->get(name);
}

//------------------------------------------------------------------------------

}
}
