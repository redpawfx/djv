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

//! \file djv_float_edit.cpp

#include <djv_float_edit.h>

#include <djv_inc_button.h>
#include <djv_row_layout.h>
#include <djv_text_edit.h>

namespace djv
{

//------------------------------------------------------------------------------
// Float_Edit
//------------------------------------------------------------------------------

namespace
{

const String format_float = "000.00";

} // namespace

Float_Edit::Float_Edit() :
    signal (this),
    _widget(0)
{
    debug_path("Float_Edit");

    // Create widgets.

    _widget = new Text_Edit;
    _widget->size_string(format_float);

    Inc_Button * inc_widget = new Inc_Button;

    // Layout.

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    Horizontal_Layout * layout = new Horizontal_Layout(this);
    layout->spacing(0);
    layout->margin(0);
    layout->add(_widget);
    layout->stretch(_widget);
    layout->add(inc_widget);

    // Initialize.

    value_update();

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    inc_widget->inc_signal.set(this, inc_callback);
    inc_widget->dec_signal.set(this, dec_callback);
}

Float_Edit::~Float_Edit()
{}

int Float_Edit::handle(int in)
{
    if (Abstract_Float_Input::handle(in, visible_focus()))
    {
        return 1;
    }

    return Widget::handle(in);
}

Layout_Item * Float_Edit::group_align()
{
    return _widget->group_align();
}

void Float_Edit::value_callback()
{
    value_update();

    signal.emit(get());
}

void Float_Edit::widget_callback(const String & in)
{
    set(String_Util::string_to_float(in));

    value_callback();
}

void Float_Edit::inc_callback(bool)
{
    inc_small_action();
}

void Float_Edit::dec_callback(bool)
{
    dec_small_action();
}

void Float_Edit::value_update()
{
    callbacks(false);

    _widget->set(String_Util::float_to_string(get(), precision()));

    callbacks(true);
}

} // djv

