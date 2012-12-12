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

//! \file djv_application_message_dialog.cpp

#include <djv_application_message_dialog.h>

#include <djv_application.h>
#include <djv_check_button.h>
#include <djv_multiline_text_display.h>
#include <djv_row_layout.h>
#include <djv_prefs.h>
#include <djv_push_button.h>
#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// Application_Message_Dialog
//------------------------------------------------------------------------------

Application_Message_Dialog::Application_Message_Dialog() :
    Dialog("Messages"),
    _show        (true),
    _widget      (0),
    _show_widget (0),
    _clear_widget(0),
    _close_widget(0)
{
    // Create widgets.

    _widget = new Multiline_Text_Display;

    _show_widget = new Check_Button("&Show");

    _clear_widget = new Push_Button("&Clear");

    _close_widget = new Push_Button("Clos&e");

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(_widget);
    layout->stretch(_widget);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(_show_widget);
    layout_h->add_stretch();
    layout_h->add(_clear_widget);
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Preferences.

    Prefs prefs(Prefs::prefs_global(), "application_message_dialog");
    Prefs::get_(&prefs, "show", &_show);

    // Initialize.

    widget_update();
    dialog_update();

    size(size_hint());

    // Callbacks.

    _show_widget->signal.set(this, show_callback);
    _clear_widget->signal.set(this, clear_callback);
    _close_widget->signal.set(this, close_callback);
}

Application_Message_Dialog::~Application_Message_Dialog()
{}

void Application_Message_Dialog::del()
{
    Prefs prefs(Prefs::prefs_global(), "application_message_dialog");
    Prefs::set_(&prefs, "show", _show);

    Dialog::del();
}

void Application_Message_Dialog::set(const String & in)
{
    _list.push_front(in);

    if (_list.size() > 100)
    {
        _list.pop_back();
    }

    widget_update();

    if (_show)
    {
        show();
    }
}

void Application_Message_Dialog::set(const Error & in)
{
    set(Error_Util::format(in));
}

void Application_Message_Dialog::show()
{
    _close_widget->take_focus();

    Dialog::show();
}

void Application_Message_Dialog::dirty()
{
    Dialog::dirty();

    _size_min = Vector_Util::max(Dialog::size_min(), V2i(400, 200));
}

const V2i & Application_Message_Dialog::size_min() const
{
    return _size_min;
}

Application_Message_Dialog * Application_Message_Dialog::global()
{
    static Application_Message_Dialog * dialog = 0;

    if (! dialog)
    {
        dialog = new Application_Message_Dialog;
    }

    return dialog;
}

void Application_Message_Dialog::message(const String & in)
{
    global()->set(in);
}

void Application_Message_Dialog::error(const Error & in)
{
    global()->set(in);
}

void Application_Message_Dialog::show_callback(bool in)
{
    _show = in;

    dialog_update();
}

void Application_Message_Dialog::clear_callback(bool)
{
    _list.clear();

    widget_update();
}

void Application_Message_Dialog::close_callback(bool)
{
    hide();
}

void Application_Message_Dialog::widget_update()
{
    callbacks(false);

    _widget->set(String_Util::join(_list, "\n"));

    callbacks(true);
}

void Application_Message_Dialog::dialog_update()
{
    callbacks(false);

    _show_widget->set(_show);

    callbacks(true);
}

} // djv

