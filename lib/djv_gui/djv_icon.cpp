//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_icon.cpp

#include <djv_icon.h>

#include <djv_gl_image.h>

#include <FL/Fl_Pixmap.H>

#include "../../icons/file.xpm"
#include "../../icons/file_open.xpm"
#include "../../icons/file_reload.xpm"
#include "../../icons/file_close.xpm"
#include "../../icons/directory.xpm"
#include "../../icons/directory_up.xpm"
#include "../../icons/directory_prev.xpm"
#include "../../icons/directory_reload.xpm"
#include "../../icons/window_new.xpm"
#include "../../icons/window_copy.xpm"
#include "../../icons/window_close.xpm"
#include "../../icons/window_fit.xpm"
#include "../../icons/window_fullscreen.xpm"
#include "../../icons/view_zoom_in.xpm"
#include "../../icons/view_zoom_out.xpm"
#include "../../icons/view_zoom_reset.xpm"
#include "../../icons/view_fit.xpm"
#include "../../icons/shuttle_0.xpm"
#include "../../icons/shuttle_1.xpm"
#include "../../icons/shuttle_2.xpm"
#include "../../icons/shuttle_3.xpm"
#include "../../icons/shuttle_4.xpm"
#include "../../icons/shuttle_5.xpm"
#include "../../icons/shuttle_6.xpm"
#include "../../icons/shuttle_7.xpm"
#include "../../icons/playback_stop.xpm"
#include "../../icons/playback_forward.xpm"
#include "../../icons/playback_reverse.xpm"
#include "../../icons/playback_start.xpm"
#include "../../icons/playback_end.xpm"
#include "../../icons/playback_prev.xpm"
#include "../../icons/playback_next.xpm"
#include "../../icons/playback_shuttle_0.xpm"
#include "../../icons/playback_shuttle_1.xpm"
#include "../../icons/playback_shuttle_2.xpm"
#include "../../icons/playback_shuttle_3.xpm"
#include "../../icons/playback_shuttle_4.xpm"
#include "../../icons/playback_shuttle_5.xpm"
#include "../../icons/playback_shuttle_6.xpm"
#include "../../icons/playback_shuttle_7.xpm"
#include "../../icons/playback_frame_shuttle_0.xpm"
#include "../../icons/playback_frame_shuttle_1.xpm"
#include "../../icons/playback_frame_shuttle_2.xpm"
#include "../../icons/playback_frame_shuttle_3.xpm"
#include "../../icons/playback_frame_shuttle_4.xpm"
#include "../../icons/playback_frame_shuttle_5.xpm"
#include "../../icons/playback_frame_shuttle_6.xpm"
#include "../../icons/playback_frame_shuttle_7.xpm"
#include "../../icons/playback_loop_once.xpm"
#include "../../icons/playback_loop_repeat.xpm"
#include "../../icons/playback_loop_ping_pong.xpm"
#include "../../icons/playback_in_out.xpm"
#include "../../icons/tool_check.xpm"
#include "../../icons/tool_radio.xpm"
#include "../../icons/tool_start.xpm"
#include "../../icons/tool_end.xpm"
#include "../../icons/tool_x.xpm"
#include "../../icons/tool_lock.xpm"
#include "../../icons/tool_add.xpm"
#include "../../icons/tool_inc.xpm"
#include "../../icons/tool_dec.xpm"
#include "../../icons/color_wheel.xpm"
#include "../../icons/grey_ramp.xpm"
#include "../../icons/magnify.xpm"
#include "../../icons/color_picker.xpm"
#include "../../icons/histogram.xpm"
//#include "../../icons/info.xpm"

namespace djv
{

//------------------------------------------------------------------------------
// Icon_Factory
//------------------------------------------------------------------------------

namespace
{

struct Icon
{
    String name;
    char ** data;
} icon [] =
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

} // namespace

//------------------------------------------------------------------------------
// Icon_Factory
//------------------------------------------------------------------------------

Icon_Factory::Icon_Factory() : _empty(0, 0, 0)
{
    for (uint i = 0; i < icon_size; ++i)
    {
        add(icon[i].name, new Fl_Pixmap((const char **)icon[i].data));
    }
}

Icon_Factory::~Icon_Factory()
{
    for (List::iterator i = _list.begin(); i != _list.end(); ++i)
    {
        delete i->second;
    }
}

void Icon_Factory::add(const String & name, Fl_Image * icon)
{
    _list[name] = icon;
}

Fl_Image * Icon_Factory::get(const String & name) const
{
    if (_list.find(name) == _list.end())
    {
        return const_cast<Fl_Image *>(&_empty);
    }

    return const_cast<Icon_Factory *>(this)->_list[name];
}

Fl_Image * Icon_Factory::get_(const String & name)
{
    return global()->get(name);
}

Fl_Image * Icon_Factory::image_to_fltk(
    const Pixel_Data &    in,
    const V2i &           resize,
    const Color_Profile & color_profile)
{
    //DJV_DEBUG("image_to_fltk");
    //DJV_DEBUG_PRINT("in = " << in);

    // Temporary buffer.

    Pixel_Data tmp(
        Pixel_Data_Info(
            Vector_Util::is_size_valid(resize) ? resize : in.size(),
            Pixel::RGB_U8));

    //DJV_DEBUG_PRINT("tmp = " << tmp);

    // Transform.

    Gl_Image_Options options;

    options.xform.scale =
        V2f(tmp.size()) /
        (V2f(in.size() * Pixel_Data::proxy_scale(in.info().proxy)));

    options.xform.mirror.y = true;

    options.color_profile = color_profile;

    Gl_Image::copy(in, tmp, options);

    // Convert to FLTK.

    return Fl_RGB_Image(tmp.data(), tmp.w(), tmp.h()).copy();
}

Icon_Factory * Icon_Factory::global()
{
    static Icon_Factory * global = 0;

    if (! global)
    {
        global = new Icon_Factory;
    }

    return global;
}

} // djv
