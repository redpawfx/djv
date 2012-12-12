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

//! \file djv_multi_choice_dialog.cpp

#include <djv_multi_choice_dialog.h>

#include <djv_check_button_group.h>
#include <djv_row_layout.h>
#include <djv_label.h>
#include <djv_push_button.h>
#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// Multi_Choice_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_multi_choice = "Multiple Choice Dialog",
    label_ok = "O&k",
    label_close = "Clos&e";

} // namespace

Multi_Choice_Dialog::Multi_Choice_Dialog(
    const String & label,
    const List<String> & list) :
    Abstract_Message_Dialog(label_multi_choice),
    signal      (this),
    index_signal(this),
    _widget     (0),
    _label      (0)
{
    // Create widgets.

    _widget = new Check_Button_Group(list);
    _widget->align(CENTER, CENTER);

    _label = new Label(label);

    Push_Button * ok_button = new Push_Button(label_ok);
    Push_Button * close_button = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    Vertical_Layout * layout_v = new Vertical_Layout(layout);
    layout->stretch(layout_v);
    layout_v->margin(Style::global()->margin_large());
    layout_v->add(_label);
    layout_v->add(_widget);
    layout_v->stretch(_widget);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(ok_button);
    layout_h->add(close_button);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    size(size_hint(), false);

    // Callbacks.

    ok_button->signal.set(this, ok_callback);
    close_button->signal.set(this, close_callback);
}

Multi_Choice_Dialog::~Multi_Choice_Dialog()
{}

void Multi_Choice_Dialog::list(const List<String> & in)
{
    _widget->label(in);

    size(size_hint(), false);
}

List<String> Multi_Choice_Dialog::list() const
{
    return _widget->label();
}

void Multi_Choice_Dialog::label(const String & in)
{
    _label->set(in);

    size(size_hint(), false);
}

const String & Multi_Choice_Dialog::label() const
{
    return _widget->title();
}

void Multi_Choice_Dialog::show()
{
    _widget->set(List<bool>());

    _widget->take_focus();

    Abstract_Message_Dialog::show();
}

namespace
{

Multi_Choice_Dialog * _dialog = 0;

} // namespace

void Multi_Choice_Dialog::dialog(
    const String &                             label,
    const List<String> &                       list,
    Callback *                                 in,
    Signal<const List<bool> &>::Callback_Fnc * callback)
{
    if (! _dialog)
    {
        _dialog = new Multi_Choice_Dialog;
    }

    _dialog->signal.del();
    _dialog->index_signal.del();

    _dialog->list(list);
    _dialog->label(label);

    _dialog->signal.set(in, callback);

    _dialog->show();
}

void Multi_Choice_Dialog::dialog(
    const String &                            label,
    const List<String> &                      list,
    Callback *                                in,
    Signal<const List<int> &>::Callback_Fnc * callback)
{
    if (! _dialog)
    {
        _dialog = new Multi_Choice_Dialog;
    }

    _dialog->signal.del();
    _dialog->index_signal.del();

    _dialog->list(list);
    _dialog->label(label);

    _dialog->index_signal.set(in, callback);

    _dialog->show();
}

void Multi_Choice_Dialog::ok_callback(bool)
{
    signal.emit(_widget->get());
    index_signal.emit(_widget->get_index());

    hide();
}

void Multi_Choice_Dialog::close_callback(bool)
{
    hide();
}

} // djv

