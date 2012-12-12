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

//! \file djv_question_dialog.cpp

#include <djv_question_dialog.h>

#include <djv_row_layout.h>
#include <djv_label.h>
#include <djv_push_button.h>
#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// Question_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_question = "Question Dialog",
    label_yes = "&Yes",
    label_no = "&No";

} // namespace

Question_Dialog::Question_Dialog(const String & label) :
    Abstract_Message_Dialog(label_question),
    signal(this),
    _label       (0),
    _ok_widget   (0),
    _close_widget(0)
{
    // Create widgets.

    _label = new Label(label);
    _label->align(CENTER, CENTER);

    _ok_widget = new Push_Button(label_yes);

    _close_widget = new Push_Button(label_no);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout->stretch(layout_h);
    layout_h->margin(Style::global()->margin_large());
    layout_h->add(_label);
    layout_h->stretch(_label);

    layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(_ok_widget);
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    size(size_hint(), false);

    // Callbacks.

    _ok_widget->signal.set(this, ok_callback);
    _close_widget->signal.set(this, close_callback);
}

Question_Dialog::~Question_Dialog()
{}

void Question_Dialog::label(const String & in)
{
    _label->set(in);

    size(size_hint(), false);
}

const String & Question_Dialog::label() const
{
    return _label->get();
}

void Question_Dialog::show()
{
    _close_widget->take_focus();

    Abstract_Message_Dialog::show();
}

void Question_Dialog::dialog(
    const String &               label,
    Callback *                   in,
    Signal<bool>::Callback_Fnc * callback)
{
    static Question_Dialog * dialog = 0;

    if (! dialog)
    {
        dialog = new Question_Dialog;
    }

    dialog->signal.del();

    dialog->label(label);

    dialog->signal.set(in, callback);

    dialog->show();
}

void Question_Dialog::ok_callback(bool)
{
    signal.emit(true);

    hide();
}

void Question_Dialog::close_callback(bool)
{
    hide();
}

} // djv
