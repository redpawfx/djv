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

//! \file djv_view_playback_group.cpp

#include <djv_view_playback_group.h>

#include <djv_view_playback_prefs.h>
#include <djv_view_shortcut.h>
#include <djv_view_shortcut_prefs.h>
#include <djv_view_widget.h>

#include <djv_choice_tool_button.h>
#include <djv_frame_group.h>
#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_shuttle.h>
#include <djv_tool_button.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Playback_Group
//------------------------------------------------------------------------------

Playback_Group::Playback_Group(
    Menu *                menu,
    Abstract_Row_Layout * toolbar,
    Playback_Group *      copy) :
    image_signal          (this),
    overlay_signal        (this),
    update_signal         (this),
    _playback             (Playback::STOP),
    _loop                 (Playback_Prefs::global()->loop()),
    _speed_real           (0.0),
    _dropped_frames       (false),
    _dropped_frames_tmp   (false),
    _every_frame          (Playback_Prefs::global()->every_frame()),
    _frame                (0),
    _frame_tmp            (0),
    _in_out               (true),
    _in                   (0),
    _out                  (0),
    _shuttle              (false),
    _shuttle_speed        (0.0),
    _idle_pause           (false),
    _idle_init            (true),
    _idle_frame           (0),
    _speed_counter        (0),
    _layout               (Playback_Prefs::global()->layout()),
    _menu                 (0),
    _stop_widget          (0),
    _forward_widget       (0),
    _reverse_widget       (0),
    _playback_shuttle     (0),
    _loop_widget          (0),
    _speed_widget         (0),
    _speed_real_widget    (0),
    _every_frame_widget   (0),
    _frame_widget         (0),
    _frame_slider         (0),
    _frame_shuttle        (0),
    _start_widget         (0),
    _end_widget           (0),
    _start_button         (0),
    _end_button           (0),
    _prev_widget          (0),
    _next_widget          (0),
    _duration_widget      (0),
    _in_out_widget        (0),
    _in_widget            (0),
    _out_widget           (0),
    _in_mark_widget       (0),
    _out_mark_widget      (0),
    _out_reset_widget     (0),
    _layout_toolbar       (0),
    _layout_playback_group(0),
    _layout_frame_group   (0),
    _layout_in_group      (0),
    _layout_out_group     (0),
    _window_handle_spacer (0)
{
    if (copy)
    {
        _playback    = copy->_playback;
        _loop        = copy->_loop;
        _seq         = copy->_seq;
        _speed       = copy->_speed;
        _every_frame = copy->_every_frame;
        _frame       = copy->_frame;
        _in_out      = copy->_in_out;
        _in          = copy->_in;
        _out         = copy->_out;
        _layout      = copy->_layout;
    }

    // Create widgets.

    _menu = menu;

    // Create playback widgets.

    _stop_widget = new Tool_Button("playback_stop");
    _stop_widget->type(Tool_Button::RADIO);

    _forward_widget = new Tool_Button("playback_forward");
    _forward_widget->type(Tool_Button::RADIO);

    _reverse_widget = new Tool_Button("playback_reverse");
    _reverse_widget->type(Tool_Button::RADIO);

    Tool_Button::create_radio_group(List<Tool_Button *>() <<
        _reverse_widget <<
        _stop_widget <<
        _forward_widget);

    _playback_shuttle = new Shuttle("playback_");

    _loop_widget = new Choice_Tool_Button(List<String>() <<
        "playback_loop_once" <<
        "playback_loop_repeat" <<
        "playback_loop_ping_pong");

    // Create speed widgets.

    _speed_widget = new Speed_Widget;

    _speed_real_widget = new Speed_Display_Widget;

    _every_frame_widget = new Tool_Button("tool_lock");
    _every_frame_widget->type(Tool_Button::TOGGLE);

    // Create frame widgets.

    _frame_widget = new Frame_Widget;

    _frame_slider = new Frame_Slider;

    _frame_shuttle = new Shuttle("playback_frame_");

    _start_widget = new Frame_Display_Widget;

    _start_button = new Tool_Button("playback_start");

    _end_widget = new Frame_Display_Widget;

    _end_button = new Tool_Button("playback_end");

    _prev_widget = new Tool_Button("playback_prev");
    _prev_widget->type(Tool_Button::REPEAT);

    _next_widget = new Tool_Button("playback_next");
    _next_widget->type(Tool_Button::REPEAT);

    _duration_widget = new Frame_Display_Widget;

    // Create in/out point widgets.

    _in_out_widget = new Tool_Button("playback_in_out");
    _in_out_widget->type(Tool_Button::TOGGLE);

    _in_widget = new Frame_Widget;
    _in_widget->text_color(FL_SELECTION_COLOR);

    _in_mark_widget = new Tool_Button("tool_start");

    _in_reset_widget = new Tool_Button("tool_x");

    _out_widget = new Frame_Widget;
    _out_widget->text_color(FL_SELECTION_COLOR);

    _out_mark_widget = new Tool_Button("tool_end");

    _out_reset_widget = new Tool_Button("tool_x");

    // Layout.

    _layout_toolbar = new Horizontal_Layout;
    _layout_toolbar->margin(0);
    toolbar->insert(_layout_toolbar, 0);

    _layout_playback_group = new Horizontal_Frame_Group(List<Frame *>() <<
        _reverse_widget <<
        _stop_widget <<
        _forward_widget <<
        _playback_shuttle);

    _layout_frame_group = new Horizontal_Frame_Group(List<Frame *>() <<
        _start_button <<
        _prev_widget <<
        _next_widget <<
        _end_button <<
        _frame_shuttle);

    _layout_in_group = new Horizontal_Frame_Group(List<Frame *>() <<
        _in_mark_widget <<
        _in_reset_widget);
    _layout_out_group = new Horizontal_Frame_Group(List<Frame *>() <<
        _out_reset_widget <<
        _out_mark_widget);

    _window_handle_spacer = new Layout_Spacer;

    layout_update();

    // Initialize.

    playback_update();
    time_update();
    speed_update();
    frame_update();
    widget_update();

    // Playback callbacks.

    _stop_widget->signal.set(this, stop_callback);
    _forward_widget->signal.set(this, forward_callback);
    _reverse_widget->signal.set(this, reverse_callback);

    _playback_shuttle->signal.set(this, playback_shuttle_callback);
    _playback_shuttle->value_signal.set(this, playback_shuttle_value_callback);

    _loop_widget->signal.set(this, loop_callback);

    // Speed callbacks.

    _speed_widget->signal.set(this, speed_callback);
    _every_frame_widget->signal.set(this, every_frame_callback);

    // Frame callbacks.

    _frame_widget->signal.set(this, frame_stop_callback);
    _frame_slider->signal.set(this, frame_slider_callback);
    _frame_slider->push_signal.set(this, frame_push_callback);
    _frame_shuttle->signal.set(this, frame_shuttle_callback);
    _frame_shuttle->signal.set(this, frame_push_callback);
    _frame_shuttle->value_signal.set(this, frame_shuttle_value_callback);

    _start_button->signal.set(this, start_callback);
    _start_button->push_signal.set(this, frame_push_callback);
    _end_button->signal.set(this, end_callback);
    _end_button->push_signal.set(this, frame_push_callback);

    _prev_widget->signal.set(this, prev_callback);
    _prev_widget->push_signal.set(this, frame_push_callback);
    _next_widget->signal.set(this, next_callback);
    _next_widget->push_signal.set(this, frame_push_callback);

    // In/out callbacks.

    _in_out_widget->signal.set(this, in_out_callback);

    _in_widget->signal.set(this, in_callback);
    _in_mark_widget->signal.set(this, in_mark_callback);
    _in_reset_widget->signal.set(this, in_reset_callback);

    _out_widget->signal.set(this, out_callback);
    _out_mark_widget->signal.set(this, out_mark_callback);
    _out_reset_widget->signal.set(this, out_reset_callback);

    // Callbacks.

    Playback_Prefs::global()->every_frame_signal.set(
        this, every_frame_callback);
    Playback_Prefs::global()->layout_signal.set(this, layout_callback);
}

Playback_Group::~Playback_Group()
{
    Playback::global()->timer_del(this);
}

namespace
{

const String
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

} // namespace

void Playback_Group::menu_update(Menu_Item_Group * in)
{
    //DJV_DEBUG("Playback_Group::menu_update");

    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    // Menu:
    //
    // * Stop
    // * Forward
    // * Reverse
    // * Loop
    //   * ...
    // * Every Frame
    //   ---
    // * Start
    // * Start Absolute
    // * End
    // * End Absolute
    // * Previous
    // * Previous X10
    // * Previous X100
    // * Next
    // * Next X10
    // * Next X100
    //   ---
    // * In/Out Points
    // * In Mark
    // * In Reset
    // * Out Mark
    // * Out Reset
    //   ---
    // * Layout
    //   * ...

    in->add(menu_title, 0, 0, 0, Menu_Item::SUB_MENU);

    in->add(
        menu_stop,
        shortcuts[Shortcut::PLAYBACK_STOP].value,
        _stop_callback,
        this,
        Menu_Item::RADIO,
        Playback::STOP == _playback);

    in->add(
        menu_forward,
        shortcuts[Shortcut::PLAYBACK_FORWARD].value,
        _forward_callback,
        this,
        Menu_Item::RADIO,
        Playback::FORWARD == _playback);

    in->add(
        menu_reverse,
        shortcuts[Shortcut::PLAYBACK_REVERSE].value,
        _reverse_callback,
        this,
        Menu_Item::RADIO,
        Playback::REVERSE == _playback);

    in->add(menu_loop, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_loop = in->add(
        Playback::label_loop(),
        List<int>(),
        _loop_callback,
        this,
        Menu_Item::RADIO,
        _loop);
    in->end();

    in->add(
        menu_every_frame,
        0,
        _every_frame_callback,
        this,
        Menu_Item::TOGGLE | Menu_Item::DIVIDER,
        _every_frame);

    in->add(
        menu_start,
        shortcuts[Shortcut::PLAYBACK_START].value,
        _start_callback,
        this);

    in->add(
        menu_start_abs,
        shortcuts[Shortcut::PLAYBACK_START_ABS].value,
        _start_abs_callback,
        this);

    in->add(
        menu_end,
        shortcuts[Shortcut::PLAYBACK_END].value,
        _end_callback,
        this);

    in->add(
        menu_end_abs,
        shortcuts[Shortcut::PLAYBACK_END_ABS].value,
        _end_abs_callback,
        this);

    in->add(
        menu_prev,
        shortcuts[Shortcut::PLAYBACK_PREV].value,
        _prev_callback,
        this);

    in->add(
        menu_prev_x10,
        shortcuts[Shortcut::PLAYBACK_PREV_X10].value,
        _prev_x10_callback,
        this);

    in->add(
        menu_prev_x100,
        shortcuts[Shortcut::PLAYBACK_PREV_X100].value,
        _prev_x100_callback,
        this);

    in->add(
        menu_next,
        shortcuts[Shortcut::PLAYBACK_NEXT].value,
        _next_callback,
        this);

    in->add(
        menu_next_x10,
        shortcuts[Shortcut::PLAYBACK_NEXT_X10].value,
        _next_x10_callback,
        this);

    in->add(
        menu_next_x100,
        shortcuts[Shortcut::PLAYBACK_NEXT_X100].value,
        _next_x100_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(
        menu_in_out,
        shortcuts[Shortcut::PLAYBACK_IN_OUT].value,
        _in_out_callback,
        this,
        Menu_Item::TOGGLE,
        _in_out);

    in->add(
        menu_in_mark,
        shortcuts[Shortcut::PLAYBACK_IN_MARK].value,
        _in_mark_callback,
        this);

    in->add(
        menu_in_reset,
        shortcuts[Shortcut::PLAYBACK_IN_RESET].value,
        _in_reset_callback,
        this);

    in->add(
        menu_out_mark,
        shortcuts[Shortcut::PLAYBACK_OUT_MARK].value,
        _out_mark_callback,
        this);

    in->add(
        menu_out_reset,
        shortcuts[Shortcut::PLAYBACK_OUT_RESET].value,
        _out_reset_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(menu_layout, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_layout = in->add(
        Playback::label_layout(),
        List<int>(),
        _layout_callback,
        this,
        Menu_Item::RADIO,
        _layout);
    
    in->end();

    in->end();
}

namespace
{

const String
    tooltip_stop = "Stop playback\n\nShortcut: %%",
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

} // namespace

void Playback_Group::toolbar_update()
{
    //DJV_DEBUG("Playback_Group::toolbar_update");

    // Update shortcuts.

    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    _loop_widget->shortcut(shortcuts[Shortcut::PLAYBACK_LOOP].value);

    // Update tooltips.

    _stop_widget->tooltip(String_Format(tooltip_stop).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_STOP].value)));
    _forward_widget->tooltip(String_Format(tooltip_forward).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_FORWARD].value)));
    _reverse_widget->tooltip(String_Format(tooltip_reverse).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_REVERSE].value)));

    _playback_shuttle->tooltip(tooltip_playback_shuttle);

    _loop_widget->tooltip(String_Format(tooltip_loop).
        arg(String_Util::label(_loop)).
        arg(String_Util::join(Playback::label_loop(), ", ")).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_LOOP].value)));

    _speed_widget->tooltip(tooltip_speed);
    _speed_real_widget->tooltip(tooltip_speed_real);
    _every_frame_widget->tooltip(tooltip_every_frame);

    _frame_widget->tooltip(tooltip_frame);
    _frame_slider->tooltip(tooltip_frame_slider);
    _frame_shuttle->tooltip(tooltip_frame_shuttle);

    _start_widget->tooltip(tooltip_start);
    _start_button->tooltip(String_Format(tooltip_start_button).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_START].value)));

    _end_widget->tooltip(tooltip_end);
    _end_button->tooltip(String_Format(tooltip_end_button).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_END].value)));

    _prev_widget->tooltip(String_Format(tooltip_prev).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_PREV].value)));
    _next_widget->tooltip(String_Format(tooltip_next).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_NEXT].value)));

    _in_out_widget->tooltip(String_Format(tooltip_in_out).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_IN_OUT].value)));
    _in_widget->tooltip(tooltip_start);
    _in_mark_widget->tooltip(String_Format(tooltip_in_mark).
        arg(djv::Shortcut::label(shortcuts[Shortcut::PLAYBACK_IN_MARK].value)));
    _in_reset_widget->tooltip(String_Format(tooltip_in_reset).
        arg(djv::Shortcut::label(
            shortcuts[Shortcut::PLAYBACK_IN_RESET].value)));
    _out_widget->tooltip(tooltip_end);
    _out_mark_widget->tooltip(String_Format(tooltip_out_mark).
        arg(djv::Shortcut::label(
            shortcuts[Shortcut::PLAYBACK_OUT_MARK].value)));
    _out_reset_widget->tooltip(String_Format(tooltip_out_reset).
        arg(djv::Shortcut::label(
            shortcuts[Shortcut::PLAYBACK_OUT_RESET].value)));

    _duration_widget->tooltip(tooltip_duration);
}

void Playback_Group::toolbar_info_update(bool in)
{
    _window_handle_spacer->spacer_size(
        V2i(in ? 0 : Layout::window_handle_size(), 0));
}

void Playback_Group::playback(Playback::PLAYBACK in)
{
    if (in == _playback)
    {
        return;
    }

    //DJV_DEBUG("Playback_Group::playback");
    //DJV_DEBUG_PRINT("in = " << in);

    _playback = in;

    playback_update();

    update_signal.emit(true);
}

void Playback_Group::playback_callback(Playback::PLAYBACK in)
{
    _playback = (in == _playback) ? Playback::STOP : in;

    playback_update();

    update_signal.emit(true);
}

void Playback_Group::stop_callback(bool)
{
    playback_callback(Playback::STOP);
}

void Playback_Group::forward_callback(bool)
{
    playback_callback(Playback::FORWARD);
}

void Playback_Group::reverse_callback(bool)
{
    playback_callback(Playback::REVERSE);
}

void Playback_Group::_stop_callback()
{
    stop_callback(true);
}

void Playback_Group::_forward_callback()
{
    forward_callback(true);
}

void Playback_Group::_reverse_callback()
{
    reverse_callback(true);
}

void Playback_Group::playback_update()
{
    //DJV_DEBUG("Playback_Group::playback_update");

    callbacks(false);

    switch (_playback)
    {
        case Playback::STOP:
            _stop_widget->set();
            break;

        case Playback::FORWARD:
            _forward_widget->set();
            break;

        case Playback::REVERSE:
            _reverse_widget->set();
            break;

        default:
            break;
    }

    _loop_widget->set(_loop);

    if (Playback::LOOP_ONCE == _loop)
    {
        switch (_playback)
        {
            case Playback::REVERSE:
                if (_frame == frame_min())
                {
                    _frame = frame_max();
                }

                break;

            case Playback::FORWARD:
                if (_frame == frame_max())
                {
                    _frame = frame_min();
                }

                break;

            default:
                break;
        }
    }

    Playback::global()->timer_del(this);

    switch (_playback)
    {
        case Playback::REVERSE:
        case Playback::FORWARD:

            _idle_init = true;

            Playback::global()->timer_add(this);

            break;

        case Playback::STOP:
        default:
            break;
    }

    callbacks(true);
}

void Playback_Group::loop(Playback::LOOP in)
{
    if (in == _loop)
    {
        return;
    }

    //DJV_DEBUG("Playback_Group::loop");
    //DJV_DEBUG_PRINT("in = " << in);

    _loop = in;

    playback_update();
    update_signal.emit(true);
}

void Playback_Group::loop_callback(int in)
{
    loop(static_cast<Playback::LOOP>(in));
}

void Playback_Group::_loop_callback()
{
    loop_callback(List_Util::find(_menu->item(), _menu_loop));
}

namespace
{

int64_t seq_max(const Seq & in)
{
    return in.list.size() ? (static_cast<int64_t>(in.list.size()) - 1) : 0;
}

}

void Playback_Group::seq(const Seq & seq)
{
    //DJV_DEBUG("Playback_Group::seq");
    //DJV_DEBUG_PRINT("seq = " << seq);

    _seq        = seq;
    _in         = 0;
    _out        = seq_max(_seq);
    _speed      = seq.speed;
    _speed_real = 0.0;

    time_update();
    speed_update();
}

const Seq & Playback_Group::seq() const
{
    return _seq;
}

void Playback_Group::time_update()
{
    //DJV_DEBUG("Playback_Group::time_update");

    callbacks(false);

    const int64_t max = seq_max(_seq);

    _frame_widget->list(_seq.list);
    _frame_slider->range(0, max);
    _frame_slider->in_out(_in_out);
    _frame_slider->in_out(_in, _out);

    _in_out_widget->set(_in_out);

    if (_in_out && _in != 0)
    {
        _in_widget->list(_seq.list);
        _in_widget->set(_in);
    }
    else
    {
        _start_widget->set(_seq.start());
    }

    if (_in_out && max != _out)
    {
        _out_widget->list(_seq.list);
        _out_widget->set(_out);
    }
    else
    {
        _end_widget->set(_seq.end());
    }

    _duration_widget->set(
        _in_out ?
        (_out - _in + 1) :
        (max + 1));
    _duration_widget->text_color(
        (_in_out && (_in != 0 || _out != max)) ?
        FL_SELECTION_COLOR :
        FL_FOREGROUND_COLOR);

    callbacks(true);

    _duration_widget->update(); //! \todo Is this still necessary?
}

void Playback_Group::speed_update()
{
    //DJV_DEBUG("Playback_Group::speed_update");

    callbacks(false);

    _speed_widget->set(_speed);
    _speed_widget->set_default(_seq.speed);
    _speed_real_widget->set(_speed_real);
    _speed_real_widget->dropped_frames(_dropped_frames);
    _every_frame_widget->set(_every_frame);

    _frame_widget->speed(_speed);
    _start_widget->speed(_speed);
    _end_widget->speed(_speed);
    _in_widget->speed(_speed);
    _out_widget->speed(_speed);
    _duration_widget->speed(_speed);

    callbacks(true);
}

void Playback_Group::playback_shuttle_callback(bool in)
{
    if (in)
    {
        playback(Playback::STOP);

        _shuttle       = true;
        _shuttle_speed = 0.0;
        _idle_init     = true;

        Playback::global()->timer_del(this);
        Playback::global()->timer_add(this);
    }
    else
    {
        _shuttle = false;
        _dropped_frames = false;

        Playback::global()->timer_del(this);

        _speed_widget->set(_speed);
    }
}

void Playback_Group::playback_shuttle_value_callback(int in)
{
    _shuttle_speed =
        Math::pow(static_cast<double>(Math::abs(in)), 1.5) *
        (in >= 0 ? 1.0 : -1.0);
    //_shuttle_speed = static_cast<double>(in);

    _idle_frame = uint64_t(_idle_timer.seconds() * _shuttle_speed);
}

void Playback_Group::speed(const Speed & in)
{
    if (in == _speed)
    {
        return;
    }

    //DJV_DEBUG("Playback_Group::playback_speed");
    //DJV_DEBUG_PRINT("in = " << in);

    _speed = in;
    _speed_real = 0.0;
    _dropped_frames = false;

    playback_update();
    speed_update();

    overlay_signal.emit(true);
    update_signal.emit(true);
}

const Speed & Playback_Group::speed() const
{
    return _speed;
}

double Playback_Group::speed_real() const
{
    return _speed_real;
}

bool Playback_Group::dropped_frames() const
{
    return _dropped_frames;
}

void Playback_Group::speed_callback(const Speed & in)
{
    speed(in);
}

void Playback_Group::every_frame_callback(bool in)
{
    //DJV_DEBUG("Playback_Group::playback_every_frame_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _every_frame = in;

    speed_update();

    overlay_signal.emit(true);
    update_signal.emit(true);
}

void Playback_Group::_every_frame_callback()
{
    every_frame_callback(_menu->value());
}

void Playback_Group::frame(int64_t in, bool in_out)
{
    const int64_t frame_in = in_out ? frame_min() : 0;
    const int64_t frame_out = in_out ? frame_max() : seq_max(_seq);

    switch (_loop)
    {
        case Playback::LOOP_ONCE:
            if (in <= frame_in || in >= frame_out)
            {
                playback(Playback::STOP);
            }

            in = Math::clamp(in, frame_in, frame_out);
            break;

        case Playback::LOOP_REPEAT:
            in = Math::wrap(in, frame_in, frame_out);
            break;

        case Playback::LOOP_PING_PONG:
            if (_playback != Playback::STOP)
            {
                if (in <= frame_in)
                {
                    playback(Playback::FORWARD);
                }
                else if (in >= frame_out)
                {
                    playback(Playback::REVERSE);
                }
            }

            in = Math::clamp(in, frame_in, frame_out);
            break;

        default:
            break;
    }

    if (in == _frame)
    {
        return;
    }

    //DJV_DEBUG("Playback_Group::frame");
    //DJV_DEBUG_PRINT("in = " << in);

    _frame = in;

    frame_update();

    image_signal.emit(true);
}

int64_t Playback_Group::frame() const
{
    return _frame;
}

void Playback_Group::frame_callback(int64_t in)
{
    frame(in, _in_out);
}

void Playback_Group::frame_stop_callback(int64_t in)
{
    playback(Playback::STOP);

    frame_callback(in);
}

void Playback_Group::frame_slider_callback(int in)
{
    frame(in, false);
}

void Playback_Group::frame_shuttle_callback(bool in)
{
    _frame_tmp = _frame;
}

void Playback_Group::frame_shuttle_value_callback(int in)
{
    frame(_frame_tmp + in, _in_out);
}

void Playback_Group::start_callback(bool)
{
    frame_callback(_in);
}

void Playback_Group::_start_callback()
{
    start_callback(true);
}

void Playback_Group::start_abs_callback(bool)
{
    frame_slider_callback(0);
}

void Playback_Group::_start_abs_callback()
{
    start_abs_callback(true);
}

void Playback_Group::end_callback(bool)
{
    frame_callback(_out);
}

void Playback_Group::_end_callback()
{
    end_callback(true);
}

void Playback_Group::end_abs_callback(bool)
{
    frame_slider_callback(seq_max(_seq));
}

void Playback_Group::_end_abs_callback()
{
    end_abs_callback(true);
}

void Playback_Group::prev_callback(bool)
{
    frame_callback(_frame - 1);
}

void Playback_Group::_prev_callback()
{
    prev_callback(true);
}

void Playback_Group::_prev_x10_callback()
{
    frame_callback(_frame - 10);
}

void Playback_Group::_prev_x100_callback()
{
    frame_callback(_frame - 100);
}

void Playback_Group::next_callback(bool)
{
    frame_callback(_frame + 1);
}

void Playback_Group::_next_callback()
{
    next_callback(true);
}

void Playback_Group::_next_x10_callback()
{
    frame_callback(_frame + 10);
}

void Playback_Group::_next_x100_callback()
{
    frame_callback(_frame + 100);
}

void Playback_Group::frame_push_callback(bool in)
{
    //DJV_DEBUG("Playback_Group::frame_push_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _idle_pause = in;
    _idle_init = ! in;
}

void Playback_Group::frame_update()
{
    //DJV_DEBUG("Playback_Group::frame_update");

    callbacks(false);

    _frame_widget->set(_frame);
    _frame_slider->set(_frame);

    callbacks(true);
}

int64_t Playback_Group::in() const
{
    return _in;
}

void Playback_Group::in_out_callback(bool in)
{
    _in_out = in;

    //frame(_frame, _in_out);

    time_update();
    widget_update();

    update_signal.emit(true);
}

void Playback_Group::_in_out_callback()
{
    in_out_callback(! _in_out);
}

void Playback_Group::in_callback(int64_t in)
{
    _in = in;

    frame(_frame, _in_out);

    time_update();
    widget_update();

    update_signal.emit(true);
}

void Playback_Group::_in_goto_callback()
{
    frame(_in, _in_out);
}

void Playback_Group::in_mark_callback(bool)
{
    in_callback(_frame);
}

void Playback_Group::_in_mark_callback()
{
    in_mark_callback(true);
}

void Playback_Group::in_reset_callback(bool)
{
    in_callback(0);
}

void Playback_Group::_in_reset_callback()
{
    in_reset_callback(true);
}

int64_t Playback_Group::out() const
{
    return _out;
}

void Playback_Group::out_callback(int64_t in)
{
    _out = in;

    frame(_frame, _in_out);

    time_update();
    widget_update();

    update_signal.emit(true);
}

void Playback_Group::_out_goto_callback()
{
    frame(_out, _in_out);
}

void Playback_Group::out_mark_callback(bool)
{
    out_callback(_frame);
}

void Playback_Group::_out_mark_callback()
{
    out_mark_callback(true);
}

void Playback_Group::out_reset_callback(bool)
{
    out_callback(seq_max(_seq));
}

void Playback_Group::_out_reset_callback()
{
    out_reset_callback(true);
}

int64_t Playback_Group::frame_min() const
{
    return Math::max(static_cast<int64_t>(0), _in);
}

int64_t Playback_Group::frame_max() const
{
    return Math::min(seq_max(_seq), _out);
}

void Playback_Group::idle()
{
    if (_idle_pause)
    {
        return;
    }

    //DJV_DEBUG("Playback_Group::idle");
    //DJV_DEBUG_PRINT("playback = " << _playback);
    //DJV_DEBUG_PRINT("loop = " << _loop);
    //DJV_DEBUG_PRINT("every frame = " << _every_frame);
    //DJV_DEBUG_PRINT("frame = " << _frame);

    if (_idle_init)
    {
        //DJV_DEBUG_PRINT("init");

        _idle_timer.start();
        _idle_frame         = 0;
        _speed_timer        = _idle_timer;
        _speed_counter      = 0;
        _dropped_frames     = false;
        _dropped_frames_tmp = false;
        _idle_init          = false;
    }
    else
    {
        _idle_timer.check();
    }

    const double speed =
        _shuttle ? _shuttle_speed : Speed::speed_to_float(_speed);

    //DJV_DEBUG_PRINT("speed = " << speed);

    //! \todo Overflow?

    const uint64_t absolute_frame = uint64_t(_idle_timer.seconds() * speed);
    int inc = static_cast<int>(absolute_frame - _idle_frame);

    //DJV_DEBUG_PRINT("absolute frame = " << static_cast<int>(absolute_frame));
    //DJV_DEBUG_PRINT("inc = " << inc);

    if (! inc)
    {
        return;
    }

    // Calculate current frame.

    _idle_frame = absolute_frame;

    if (Playback::REVERSE == _playback)
    {
        inc = -inc;
    }

    const bool every_frame = _every_frame && ! _shuttle;

    if (every_frame)
    {
        inc = inc > 0 ? 1 : (inc < 0 ? -1 : 0);
    }
    else
    {
        _dropped_frames_tmp |= inc > 1 || inc < -1;
    }

    _speed_counter += Math::abs(inc);

    // Calculate real playback speed.

    _speed_timer.check();

    if (_speed_timer.seconds() >= 1.0)
    {
        _speed_real = _speed_counter / _speed_timer.seconds();
        _speed_timer.start();

        _dropped_frames = _dropped_frames_tmp;
        _dropped_frames_tmp = false;
        _speed_counter = 0;

        _speed_real_widget->set(_speed_real);
        _speed_real_widget->dropped_frames(
            every_frame ? false : _dropped_frames);
    }

    frame(_frame + inc, _in_out);
}

void Playback_Group::cached_frames(const List<int64_t> & in)
{
    _frame_slider->cached_frames(in);
}

void Playback_Group::layout(Playback::LAYOUT in)
{
    if (in == _layout)
    {
        return;
    }

    //DJV_DEBUG("Playback_Group::layout");
    //DJV_DEBUG_PRINT("in = " << in);

    _layout = in;

    layout_update();
    update_signal.emit(true);
}

void Playback_Group::layout_callback(Playback::LAYOUT in)
{
    layout(in);
}

void Playback_Group::_layout_callback()
{
    layout_callback(static_cast<Playback::LAYOUT>(
        List_Util::find(_menu->item(), _menu_layout)));
}

void Playback_Group::widget_update()
{
    //DJV_DEBUG("Playback_Group::widget_update");

    callbacks(false);

    const int64_t max = seq_max(_seq);

    switch (_layout)
    {
        case Playback::LAYOUT_DEFAULT:
        case Playback::LAYOUT_LEFT:
        case Playback::LAYOUT_CENTER:

            _layout_playback_group->show();
            _layout_frame_group->show();
            _speed_widget->show();
            _speed_real_widget->show();
            _every_frame_widget->show();
            _loop_widget->show();
            _in_out_widget->show();

            _frame_slider->show();
            _frame_widget->show();
            _layout_in_group->show();

            if (_in_out && _in != 0)
            {
                _in_widget->show();
                _start_widget->hide();
            }
            else
            {
                _start_widget->show();
                _in_widget->hide();
            }

            if (_in_out && max != _out)
            {
                _out_widget->show();
                _end_widget->hide();
            }
            else
            {
                _end_widget->show();
                _out_widget->hide();
            }

            _layout_out_group->show();
            _duration_widget->show();

            break;

        case Playback::LAYOUT_MINIMAL:

            _layout_playback_group->show();
            _layout_frame_group->hide();
            _speed_widget->hide();
            _speed_real_widget->hide();
            _every_frame_widget->hide();
            _loop_widget->hide();
            _in_out_widget->hide();

            _frame_slider->show();
            _frame_widget->hide();
            _layout_in_group->hide();
            _in_widget->hide();
            _start_widget->hide();
            _end_widget->hide();
            _out_widget->hide();
            _layout_out_group->hide();
            _duration_widget->hide();

            break;

        default:
            break;
    }

    callbacks(true);
}

void Playback_Group::layout_update()
{
    //DJV_DEBUG("Playback_Group::layout_update");

    widget_update();

    callbacks(false);

    _layout_toolbar->remove_child(_loop_widget);

    //DJV_DEBUG_PRINT("remove loop = " << long(item));

    _layout_toolbar->remove_child(_speed_widget);
    _layout_toolbar->remove_child(_speed_real_widget);
    _layout_toolbar->remove_child(_every_frame_widget);

    _layout_toolbar->remove_child(_frame_widget);
    _layout_toolbar->remove_child(_frame_slider);
    _layout_toolbar->remove_child(_start_widget);
    _layout_toolbar->remove_child(_end_widget);
    _layout_toolbar->remove_child(_duration_widget);

    _layout_toolbar->remove_child(_in_out_widget);
    _layout_toolbar->remove_child(_in_widget);
    _layout_toolbar->remove_child(_out_widget);

    _layout_toolbar->remove_child(_layout_playback_group);
    _layout_toolbar->remove_child(_layout_frame_group);
    _layout_toolbar->remove_child(_layout_in_group);
    _layout_toolbar->remove_child(_layout_out_group);
    _layout_toolbar->remove_child(_window_handle_spacer);

    //DJV_DEBUG_PRINT("window handle spacer = " << long(item));

    _layout_toolbar->del();

    switch (_layout)
    {
        case Playback::LAYOUT_DEFAULT:
        case Playback::LAYOUT_LEFT:
        {
            Vertical_Layout * left_layout =
                new Vertical_Layout(_layout_toolbar);
            left_layout->margin(0);
            Horizontal_Layout * layout_h = new Horizontal_Layout(left_layout);
            layout_h->margin(0);
            layout_h->add(_layout_playback_group);
            layout_h->add(_layout_frame_group);
            layout_h = new Horizontal_Layout(left_layout);
            layout_h->margin(0);
            layout_h->add(_speed_widget);
            layout_h->add(_speed_real_widget);
            layout_h->add(_every_frame_widget);
            layout_h->add_spacer(-1, true);
            layout_h->add(_loop_widget);
            layout_h->add(_in_out_widget);

            Vertical_Layout * right_layout =
                new Vertical_Layout(_layout_toolbar);
            right_layout->margin(0);
            _layout_toolbar->stretch(right_layout);
            layout_h = new Horizontal_Layout(right_layout);
            layout_h->margin(0);
            layout_h->add(_frame_slider);
            layout_h->stretch(_frame_slider);
            right_layout->add_stretch();
            layout_h = new Horizontal_Layout(right_layout);
            layout_h->margin(0);
            layout_h->add(_frame_widget);
            Horizontal_Layout * layout_h2 = new Horizontal_Layout(layout_h);
            layout_h2->spacing(0);
            layout_h2->margin(0);
            layout_h2->add(_layout_in_group);
            layout_h2->add(_in_widget);
            layout_h2->add(_start_widget);

            if (Playback::LAYOUT_DEFAULT == _layout)
            {
                layout_h->add_spacer(-1, true);
            }

            layout_h2 = new Horizontal_Layout(layout_h);
            layout_h2->spacing(0);
            layout_h2->margin(0);
            layout_h2->add(_end_widget);
            layout_h2->add(_out_widget);
            layout_h2->add(_layout_out_group);
            layout_h->add(_duration_widget);

            if (Playback::LAYOUT_LEFT == _layout)
            {
                layout_h->add_stretch();
            }

            layout_h->add(_window_handle_spacer);
        }
        break;

        case Playback::LAYOUT_CENTER:
        {
            Vertical_Layout * layout = new Vertical_Layout(_layout_toolbar);
            layout->margin(0);
            _layout_toolbar->stretch(layout);

            layout->add(_frame_slider);

            Horizontal_Layout * layout1 = new Horizontal_Layout(layout);
            layout1->size(Layout_Item::FIXED, Layout_Item::FIXED);
            layout1->align(Layout_Item::CENTER, Layout_Item::CENTER);
            layout1->margin(0);
            layout1->add(_frame_widget);
            Horizontal_Layout * layout_h = new Horizontal_Layout(layout1);
            layout_h->spacing(0);
            layout_h->margin(0);
            layout_h->add(_layout_in_group);
            layout_h->add(_in_widget);
            layout_h->add(_start_widget);
            layout1->add(_layout_playback_group);
            layout1->add(_layout_frame_group);
            layout_h = new Horizontal_Layout(layout1);
            layout_h->spacing(0);
            layout_h->margin(0);
            layout_h->add(_end_widget);
            layout_h->add(_out_widget);
            layout_h->add(_layout_out_group);
            layout1->add(_duration_widget);
            layout1->add(_window_handle_spacer);
        }
        break;

        case Playback::LAYOUT_MINIMAL:
        {
            _layout_toolbar->add(_layout_playback_group);
            _layout_toolbar->add(_frame_slider);
            _layout_toolbar->stretch(_frame_slider);
            _layout_toolbar->add(_window_handle_spacer);
        }
        break;

        default:
            break;
    }

    _layout_toolbar->dirty();
    _layout_toolbar->update();

    callbacks(true);
}

} // djv_view

