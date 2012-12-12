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

//! \file djv_view_playback_group.h

#ifndef DJV_VIEW_PLAYBACK_GROUP_H
#define DJV_VIEW_PLAYBACK_GROUP_H

#include <djv_view_playback.h>

#include <djv_menu.h>
#include <djv_row_layout.h>

#include <djv_seq.h>

namespace djv
{

class Choice_Tool_Button;
class Horizontal_Frame_Group;
class Layout_Spacer;
class Menu;
class Tool_Button;
class Shuttle;

} // djv

namespace djv_view
{
using namespace djv;

class Frame_Display_Widget;
class Frame_Slider;
class Frame_Widget;
class Speed_Display_Widget;
class Speed_Widget;

//------------------------------------------------------------------------------
//! \class Playback_Group
//!
//! This class provides the playback group.
//------------------------------------------------------------------------------

class Playback_Group : public Callback
{
public:

    //! Constructor.

    Playback_Group(
        Menu *                menu,
        Abstract_Row_Layout * toolbar,
        Playback_Group *      copy    = 0);

    //! Destructor.

    virtual ~Playback_Group();

    //! Set the playback.

    void playback(Playback::PLAYBACK);

    //! Set the playback loop.

    void loop(Playback::LOOP);

    //! Set the sequence.

    void seq(const Seq &);

    //! Get the sequence.

    const Seq & seq() const;

    //! Set the speed.

    void speed(const Speed &);

    //! Get the speed.

    const Speed & speed() const;

    //! Get the real playback speed.

    double speed_real() const;

    //! Get whether frames were dropped.

    bool dropped_frames() const;

    //! Set the frame.

    void frame(int64_t, bool in_out = true);

    //! Get the frame.

    int64_t frame() const;

    //! Get the in point.

    int64_t in() const;

    //! Get the out point.

    int64_t out() const;

    //! Set the cached frames.

    void cached_frames(const List<int64_t> &);

    //! This signal is emitted when an image update is needed.

    Signal<bool> image_signal;

    //! This signal is emitted when an overlay update is needed.

    Signal<bool> overlay_signal;

    //! This signal is emitted when an update is needed.

    Signal<bool> update_signal;

    //! Update menu items.

    void menu_update(Menu_Item_Group *);

    //! Update toolbar.

    void toolbar_update();

    //! Update information toolbar.

    void toolbar_info_update(bool);

    //! Idle.

    void idle();

    //! Set the layout.

    void layout(Playback::LAYOUT);

    //! Get the layout.

    Playback::LAYOUT layout() const;

private:

    int64_t frame_min() const;
    int64_t frame_max() const;

    // Callbacks.

    DJV_CALLBACK(Playback_Group, playback_callback, Playback::PLAYBACK);
    DJV_CALLBACK(Playback_Group, stop_callback, bool);
    DJV_CALLBACK(Playback_Group, forward_callback, bool);
    DJV_CALLBACK(Playback_Group, reverse_callback, bool);
    DJV_CALLBACK(Playback_Group, playback_shuttle_callback, bool);
    DJV_CALLBACK(Playback_Group, playback_shuttle_value_callback, int);
    DJV_CALLBACK(Playback_Group, loop_callback, int);
    DJV_CALLBACK(Playback_Group, speed_callback, const Speed &);
    DJV_CALLBACK(Playback_Group, every_frame_callback, bool);
    DJV_CALLBACK(Playback_Group, frame_callback, int64_t);
    DJV_CALLBACK(Playback_Group, frame_stop_callback, int64_t);
    DJV_CALLBACK(Playback_Group, frame_slider_callback, int);
    DJV_CALLBACK(Playback_Group, frame_shuttle_callback, bool);
    DJV_CALLBACK(Playback_Group, frame_shuttle_value_callback, int);
    DJV_CALLBACK(Playback_Group, frame_push_callback, bool);
    DJV_CALLBACK(Playback_Group, start_callback, bool);
    DJV_CALLBACK(Playback_Group, start_abs_callback, bool);
    DJV_CALLBACK(Playback_Group, end_callback, bool);
    DJV_CALLBACK(Playback_Group, end_abs_callback, bool);
    DJV_CALLBACK(Playback_Group, prev_callback, bool);
    DJV_CALLBACK(Playback_Group, next_callback, bool);
    DJV_CALLBACK(Playback_Group, in_out_callback, bool);
    DJV_CALLBACK(Playback_Group, in_callback, int64_t);
    DJV_CALLBACK(Playback_Group, in_mark_callback, bool);
    DJV_CALLBACK(Playback_Group, in_reset_callback, bool);
    DJV_CALLBACK(Playback_Group, out_callback, int64_t);
    DJV_CALLBACK(Playback_Group, out_mark_callback, bool);
    DJV_CALLBACK(Playback_Group, out_reset_callback, bool);
    DJV_CALLBACK(Playback_Group, layout_callback, Playback::LAYOUT);

    DJV_FL_WIDGET_CALLBACK(Playback_Group, _stop_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _forward_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _reverse_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _loop_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _every_frame_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _start_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _start_abs_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _end_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _end_abs_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _prev_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _prev_x10_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _prev_x100_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _next_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _next_x10_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _next_x100_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _in_out_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _in_goto_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _in_mark_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _in_reset_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _out_goto_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _out_mark_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _out_reset_callback);
    DJV_FL_WIDGET_CALLBACK(Playback_Group, _layout_callback);

    // Update.

    void playback_update();
    void time_update();
    void frame_update();
    void speed_update();
    void widget_update();
    void layout_update();

    // Variables.

    Playback::PLAYBACK       _playback;
    Playback::LOOP           _loop;
    Seq                      _seq;
    Speed                    _speed;
    double                   _speed_real;
    bool                     _dropped_frames;
    bool                     _dropped_frames_tmp;
    bool                     _every_frame;
    int64_t                  _frame;
    int64_t                  _frame_tmp;
    bool                     _in_out;
    int64_t                  _in;
    int64_t                  _out;
    bool                     _shuttle;
    double                   _shuttle_speed;
    bool                     _idle_pause;
    bool                     _idle_init;
    Timer                    _idle_timer;
    Timer                    _speed_timer;
    uint64_t                 _idle_frame;
    uint64_t                 _speed_counter;
    Playback::LAYOUT         _layout;
    Menu *                   _menu;
    List<int>                _menu_loop;
    List<int>                _menu_layout;
    Tool_Button *            _stop_widget;
    Tool_Button *            _forward_widget;
    Tool_Button *            _reverse_widget;
    Shuttle *                _playback_shuttle;
    Choice_Tool_Button *     _loop_widget;
    Speed_Widget *           _speed_widget;
    Speed_Display_Widget *   _speed_real_widget;
    Tool_Button *            _every_frame_widget;
    Frame_Widget *           _frame_widget;
    Frame_Slider *           _frame_slider;
    Shuttle *                _frame_shuttle;
    Frame_Display_Widget *   _start_widget;
    Frame_Display_Widget *   _end_widget;
    Tool_Button *            _start_button;
    Tool_Button *            _end_button;
    Tool_Button *            _prev_widget;
    Tool_Button *            _next_widget;
    Frame_Display_Widget *   _duration_widget;
    Tool_Button *            _in_out_widget;
    Frame_Widget *           _in_widget;
    Frame_Widget *           _out_widget;
    Tool_Button *            _in_mark_widget;
    Tool_Button *            _in_reset_widget;
    Tool_Button *            _out_mark_widget;
    Tool_Button *            _out_reset_widget;
    Horizontal_Layout *      _layout_toolbar;
    Horizontal_Frame_Group * _layout_playback_group;
    Horizontal_Frame_Group * _layout_frame_group;
    Horizontal_Frame_Group * _layout_in_group;
    Horizontal_Frame_Group * _layout_out_group;
    Layout_Spacer *          _window_handle_spacer;
};

} // djv_view

#endif // DJV_VIEW_PLAYBACK_GROUP_H

