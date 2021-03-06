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

//! \file djv_input_dialog.cpp

#include <djv_input_dialog.h>

#include <djv_row_layout.h>
#include <djv_label.h>
#include <djv_push_button.h>
#include <djv_style.h>
#include <djv_text_edit.h>

namespace djv
{

//------------------------------------------------------------------------------
// Input_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_input = "Input Dialog",
    label_ok = "O&k",
    label_close = "Clos&e";

} // namespace

Input_Dialog::Input_Dialog(const String & label, const String & value) :
    Abstract_Message_Dialog(label_input),
    signal (this),
    _widget(0),
    _label (0)
{
    // Create widgets.

    _widget = new Text_Edit(value);

    _label = new Label(label);

    Push_Button * ok_button = new Push_Button(label_ok);
    Push_Button * close_button = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout->stretch(layout_h);
    layout_h->margin(Style::global()->margin_large());
    layout_h->add(_label);
    layout_h->add(_widget);
    layout_h->stretch(_widget);

    layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(ok_button);
    layout_h->add(close_button);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    size(size_hint());

    // Callbacks.

    _widget->action_signal.set(this, widget_callback);
    ok_button->signal.set(this, ok_callback);
    close_button->signal.set(this, close_callback);
}

Input_Dialog::~Input_Dialog()
{}

void Input_Dialog::set(const String & in)
{
    _widget->set(in);

    size(size_hint());
}

String Input_Dialog::get() const
{
    return _widget->get();
}

void Input_Dialog::label(const String & in)
{
    _label->set(in);

    size(size_hint());
}

const String & Input_Dialog::label() const
{
    return _label->get();
}

void Input_Dialog::show()
{
    _widget->take_focus();

    Abstract_Message_Dialog::show();
}

void Input_Dialog::dialog(
    const String &                         label,
    const String &                         value,
    Callback *                             in,
    Signal<const String &>::Callback_Fnc * callback)
{
    static Input_Dialog * dialog = 0;

    if (! dialog)
    {
        dialog = new Input_Dialog;
    }

    dialog->signal.del();

    dialog->set(value);
    dialog->label(label);

    dialog->signal.set(in, callback);

    dialog->show();
}

void Input_Dialog::widget_callback(const String &)
{
    ok_callback(true);
}

void Input_Dialog::ok_callback(bool)
{
    signal.emit(_widget->get());

    hide();
}

void Input_Dialog::close_callback(bool)
{
    hide();
}

} // djv

