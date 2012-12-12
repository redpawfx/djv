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

//! \file djv_view_window_prefs.cpp

#include <djv_view_window_prefs.h>

#include <djv_check_button.h>
#include <djv_check_button_group.h>
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
// c
//------------------------------------------------------------------------------

Window_Prefs::Window_Prefs() :
    toolbar_signal(this),
    _resize_fit(true),
    _resize_max(Window::RESIZE_MAX_75),
    _toolbar(Window::_TOOLBAR_SIZE)
{
    _toolbar[Window::TOOLBAR_BUTTONS] = true;
    _toolbar[Window::TOOLBAR_PLAYBACK] = true;
    _toolbar[Window::TOOLBAR_INFO] = true;

    Prefs prefs(Prefs::prefs(), "window");
    Prefs::get_(&prefs, "resize_fit", &_resize_fit);
    Prefs::get_(&prefs, "resize_max", &_resize_max);
    Prefs::get_(&prefs, "toolbar", &_toolbar);
}

Window_Prefs::~Window_Prefs()
{
    Prefs prefs(Prefs::prefs(), "window");
    Prefs::set_(&prefs, "resize_fit", _resize_fit);
    Prefs::set_(&prefs, "resize_max", _resize_max);
    Prefs::set_(&prefs, "toolbar", _toolbar);
}

void Window_Prefs::resize_fit(bool in)
{
    _resize_fit = in;
}

bool Window_Prefs::resize_fit() const
{
    return _resize_fit;
}

void Window_Prefs::resize_max(Window::RESIZE_MAX in)
{
    _resize_max = in;
}

Window::RESIZE_MAX Window_Prefs::resize_max() const
{
    return _resize_max;
}

void Window_Prefs::toolbar(const List<bool> & in)
{
    if (in == _toolbar)
    {
        return;
    }

    _toolbar = in;

    toolbar_signal.emit(_toolbar);
}

const List<bool> & Window_Prefs::toolbar() const
{
    return _toolbar;
}

//------------------------------------------------------------------------------
// Window_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_resize_group = "Resizing",
    label_resize_fit = "Fit window to image",
    label_resize_max = "Maximum size:",
    label_toolbar_group = "Toolbars";

} // namespace

Window_Prefs_Widget::Window_Prefs_Widget()
{
    // Create resize widgets.

    Group_Box * resize_group = new Group_Box(label_resize_group);

    Check_Button * resize_fit_widget = new Check_Button(label_resize_fit);

    Radio_Button_Group * resize_max_widget =
        new Radio_Button_Group(label_resize_max, Window::label_resize_max());

    // Create toolbar widgets.

    Group_Box * toolbar_group = new Group_Box(label_toolbar_group);

    Check_Button_Group * toolbar_widget =
        new Check_Button_Group(Window::label_toolbar());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(resize_group);
    resize_group->layout()->add(resize_fit_widget);
    resize_group->layout()->add(resize_max_widget);

    layout->add(toolbar_group);
    toolbar_group->layout()->add(toolbar_widget);

    layout->add_stretch();

    // Initialize.

    resize_fit_widget->set(Window_Prefs::global()->resize_fit());
    resize_max_widget->set(Window_Prefs::global()->resize_max());
    toolbar_widget->set(Window_Prefs::global()->toolbar());

    // Callbacks.

    resize_fit_widget->signal.set(this, resize_fit_callback);
    resize_max_widget->signal.set(this, resize_max_callback);
    toolbar_widget->signal.set(this, toolbar_callback);
}

void Window_Prefs_Widget::resize_fit_callback(bool in)
{
    Window_Prefs::global()->resize_fit(in);
}

void Window_Prefs_Widget::resize_max_callback(int in)
{
    Window_Prefs::global()->resize_max(static_cast<Window::RESIZE_MAX>(in));
}

void Window_Prefs_Widget::toolbar_callback(const List<bool> & in)
{
    Window_Prefs::global()->toolbar(in);
}

} // djv_view

