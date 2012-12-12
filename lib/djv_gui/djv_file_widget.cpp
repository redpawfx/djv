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

//! \file djv_file_widget.cpp

#include <djv_file_widget.h>

#include <djv_file_browser.h>
#include <djv_row_layout.h>
#include <djv_push_button.h>
#include <djv_text_edit.h>

namespace djv
{

//------------------------------------------------------------------------------
// File_Edit
//------------------------------------------------------------------------------

namespace
{

const String
    label_browse = "&Browse";

} // namespace

File_Edit::File_Edit() :
    signal (this),
    _widget(0)
{
    // Create widgets.

    _widget = new Text_Edit;

    Push_Button * dialog_widget = new Push_Button(label_browse);

    // Layout.

    Horizontal_Layout * layout = new Horizontal_Layout(this);
    layout->margin(0);

    layout->add(_widget);
    layout->stretch(_widget);

    layout->add(dialog_widget);

    // Initialize.

    widget_update();

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    dialog_widget->signal.set(this, dialog_callback);
}

File_Edit::~File_Edit()
{}

void File_Edit::del()
{
    File_Browser::del(this);

    Widget::del();
}

void File_Edit::set(const File & in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();

    signal.emit(_value);
}

const File & File_Edit::get() const
{
    return _value;
}

void File_Edit::widget_callback(const String & in)
{
    set(in);
}

void File_Edit::dialog_callback(bool)
{
    File_Browser::set(this, dialog_callback);
}

void File_Edit::dialog_callback(const File & in)
{
    set(in);
}

void File_Edit::widget_update()
{
    _widget->set(_value);
}

} // djv

