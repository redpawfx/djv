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

//! \file djv_view_playback_prefs.cpp

#include <djv_view_playback_prefs.h>

#include <djv_check_button.h>
#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_menu.h>
#include <djv_prefs.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Playback_Prefs
//------------------------------------------------------------------------------

Playback_Prefs::Playback_Prefs() :
    loop_signal(this),
    every_frame_signal(this),
    layout_signal(this),
    _start(true),
    _loop(Playback::LOOP_REPEAT),
    _every_frame(true),
    _layout(Playback::LAYOUT_DEFAULT)
{
    Prefs prefs(Prefs::prefs(), "playback");
    Prefs::get_(&prefs, "start", &_start);
    Prefs::get_(&prefs, "loop", &_loop);
    Prefs::get_(&prefs, "every_frame", &_every_frame);
    Prefs::get_(&prefs, "layout", &_layout);
}

Playback_Prefs::~Playback_Prefs()
{
    Prefs prefs(Prefs::prefs(), "playback");
    Prefs::set_(&prefs, "start", _start);
    Prefs::set_(&prefs, "loop", _loop);
    Prefs::set_(&prefs, "every_frame", _every_frame);
    Prefs::set_(&prefs, "layout", _layout);
}

void Playback_Prefs::start(bool in)
{
    _start = in;
}

bool Playback_Prefs::start() const
{
    return _start;
}

void Playback_Prefs::loop(Playback::LOOP in)
{
    if (in == _loop)
    {
        return;
    }

    _loop = in;

    loop_signal.emit(_loop);
}

Playback::LOOP Playback_Prefs::loop() const
{
    return _loop;
}

void Playback_Prefs::every_frame(bool in)
{
    if (in == _every_frame)
    {
        return;
    }

    _every_frame = in;

    every_frame_signal.emit(_every_frame);
}

bool Playback_Prefs::every_frame() const
{
    return _every_frame;
}

void Playback_Prefs::layout(Playback::LAYOUT in)
{
    if (in == _layout)
    {
        return;
    }

    _layout = in;

    layout_signal.emit(_layout);
}

Playback::LAYOUT Playback_Prefs::layout() const
{
    return _layout;
}

//------------------------------------------------------------------------------
// Playback_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_group = "General",
    label_loop = "Loop mode:",
    label_start = "Start playback when opening files",
    label_every_frame = "Play every frame",
    label_frame_controls_stop_playback = "Frame controls stop playback",
    label_layout_group = "Layout";

} // namespace

Playback_Prefs_Widget::Playback_Prefs_Widget()
{
    // Create widgets.

    Group_Box * general_group = new Group_Box(label_group);

    Check_Button * start_widget = new Check_Button(label_start);

    Radio_Button_Group * loop_widget =
        new Radio_Button_Group(label_loop, Playback::label_loop());

    Check_Button * every_frame_widget = new Check_Button(label_every_frame);

    Group_Box * ui_group = new Group_Box(label_layout_group);

    Radio_Button_Group * layout_widget =
        new Radio_Button_Group(Playback::label_layout());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(general_group);
    general_group->layout()->add(start_widget);
    general_group->layout()->add(loop_widget);
    general_group->layout()->add(every_frame_widget);

    layout->add(ui_group);
    ui_group->layout()->add(layout_widget);

    layout->add_stretch();

    // Initialize.

    start_widget->set(Playback_Prefs::global()->start());
    loop_widget->set(Playback_Prefs::global()->loop());
    every_frame_widget->set(Playback_Prefs::global()->every_frame());
    layout_widget->set(Playback_Prefs::global()->layout());

    // Callbacks.

    start_widget->signal.set(this, start_callback);
    loop_widget->signal.set(this, loop_callback);
    every_frame_widget->signal.set(this, every_frame_callback);
    layout_widget->signal.set(this, layout_callback);
}

void Playback_Prefs_Widget::start_callback(bool in)
{
    Playback_Prefs::global()->start(in);
}

void Playback_Prefs_Widget::loop_callback(int in)
{
    Playback_Prefs::global()->loop(static_cast<Playback::LOOP>(in));
}

void Playback_Prefs_Widget::every_frame_callback(bool in)
{
    Playback_Prefs::global()->every_frame(in);
}

void Playback_Prefs_Widget::layout_callback(int in)
{
    Playback_Prefs::global()->layout(static_cast<Playback::LAYOUT>(in));
}

} // djv_view

