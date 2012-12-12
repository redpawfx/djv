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

//! \file djv_view_input_prefs.cpp

#include <djv_view_input_prefs.h>

#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_prefs.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Input_Prefs
//------------------------------------------------------------------------------

Input_Prefs::Input_Prefs() :
    _mouse_wheel      (Input::MOUSE_WHEEL_VIEW_ZOOM),
    _mouse_wheel_shift(Input::MOUSE_WHEEL_PLAYBACK_SHUTTLE),
    _mouse_wheel_ctrl (Input::MOUSE_WHEEL_PLAYBACK_SPEED)
{
    //DJV_DEBUG("Input_Prefs::Input_Prefs");

    Prefs prefs(Prefs::prefs(), "input");
    Prefs::get_(&prefs, "mouse_wheel", &_mouse_wheel);
    Prefs::get_(&prefs, "mouse_wheel_shift", &_mouse_wheel_shift);
    Prefs::get_(&prefs, "mouse_wheel_ctrl", &_mouse_wheel_ctrl);
}

Input_Prefs::~Input_Prefs()
{
    //DJV_DEBUG("Prefs::~Prefs");

    Prefs prefs(Prefs::prefs(), "input");
    Prefs::set_(&prefs, "mouse_wheel", _mouse_wheel);
    Prefs::set_(&prefs, "mouse_wheel_shift", _mouse_wheel_shift);
    Prefs::set_(&prefs, "mouse_wheel_ctrl", _mouse_wheel_ctrl);
}

void Input_Prefs::mouse_wheel(Input::MOUSE_WHEEL in)
{
    if (in == _mouse_wheel)
    {
        return;
    }

    _mouse_wheel = in;
}

Input::MOUSE_WHEEL Input_Prefs::mouse_wheel() const
{
    return _mouse_wheel;
}

void Input_Prefs::mouse_wheel_shift(Input::MOUSE_WHEEL in)
{
    if (in == _mouse_wheel_shift)
    {
        return;
    }

    _mouse_wheel_shift = in;
}

Input::MOUSE_WHEEL Input_Prefs::mouse_wheel_shift() const
{
    return _mouse_wheel_shift;
}

void Input_Prefs::mouse_wheel_ctrl(Input::MOUSE_WHEEL in)
{
    if (in == _mouse_wheel_ctrl)
    {
        return;
    }

    _mouse_wheel_ctrl = in;
}

Input::MOUSE_WHEEL Input_Prefs::mouse_wheel_ctrl() const
{
    return _mouse_wheel_ctrl;
}

//------------------------------------------------------------------------------
// Input_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_mouse_wheel_group = "Mouse Wheel",
    label_mouse_wheel = "Wheel:",
    label_mouse_wheel_shift = "Shift + wheel:",
    label_mouse_wheel_ctrl = "Ctrl + wheel:";

} // namespace

Input_Prefs_Widget::Input_Prefs_Widget()
{
    // Create widgets.

    Group_Box * mouse_wheel_group =
        new Group_Box(label_mouse_wheel_group);

    Radio_Button_Group * mouse_wheel_widget =
        new Radio_Button_Group(label_mouse_wheel,
            Input::label_mouse_wheel());

    Radio_Button_Group * mouse_wheel_shift_widget =
        new Radio_Button_Group(label_mouse_wheel_shift,
            Input::label_mouse_wheel());

    Radio_Button_Group * mouse_wheel_ctrl_widget =
        new Radio_Button_Group(label_mouse_wheel_ctrl,
            Input::label_mouse_wheel());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(mouse_wheel_group);
    Horizontal_Layout * layout_h =
        new Horizontal_Layout(mouse_wheel_group->layout());
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(mouse_wheel_widget);
    layout_h->add(mouse_wheel_shift_widget);
    layout_h->add(mouse_wheel_ctrl_widget);
    layout_h->add_stretch();

    layout->add_stretch();

    // Initialize.

    mouse_wheel_widget->set(Input_Prefs::global()->mouse_wheel());
    mouse_wheel_shift_widget->set(Input_Prefs::global()->mouse_wheel_shift());
    mouse_wheel_ctrl_widget->set(Input_Prefs::global()->mouse_wheel_ctrl());

    // Callbacks.

    mouse_wheel_widget->signal.set(this, mouse_wheel_callback);
    mouse_wheel_shift_widget->signal.set(this, mouse_wheel_shift_callback);
    mouse_wheel_ctrl_widget->signal.set(this, mouse_wheel_ctrl_callback);
}

Input_Prefs_Widget::~Input_Prefs_Widget()
{}

void Input_Prefs_Widget::mouse_wheel_callback(int in)
{
    Input_Prefs::global()->mouse_wheel(static_cast<Input::MOUSE_WHEEL>(in));
}

void Input_Prefs_Widget::mouse_wheel_shift_callback(int in)
{
    Input_Prefs::global()->mouse_wheel_shift(
        static_cast<Input::MOUSE_WHEEL>(in));
}

void Input_Prefs_Widget::mouse_wheel_ctrl_callback(int in)
{
    Input_Prefs::global()->mouse_wheel_ctrl(
        static_cast<Input::MOUSE_WHEEL>(in));
}

} // djv_view

