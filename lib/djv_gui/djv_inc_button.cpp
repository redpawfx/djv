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

//! \file djv_inc_button.cpp

#include <djv_inc_button.h>

#include <djv_frame_group.h>
#include <djv_tool_button.h>
#include <djv_row_layout.h>

#include <FL/Fl.H>

namespace djv
{

//------------------------------------------------------------------------------
// Inc_Button
//------------------------------------------------------------------------------

Inc_Button::Inc_Button() :
    inc_signal (this),
    dec_signal (this),
    push_signal(this),
    _inc_widget(0),
    _dec_widget(0)
{
    debug_path("Inc_Button");

    // Create widgets.

    _inc_widget = new Tool_Button("tool_inc");
    _inc_widget->type(Tool_Button::REPEAT);
    _inc_widget->debug_path("inc");

    _dec_widget = new Tool_Button("tool_dec");
    _dec_widget->type(Tool_Button::REPEAT);
    _dec_widget->debug_path("dec");

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->spacing(0);
    layout->margin(0);
    layout->add(new Vertical_Frame_Group(
        List<Frame *>() << _inc_widget << _dec_widget));

    // Callbacks.

    _inc_widget->signal.set(this, inc_callback);
    _inc_widget->push_signal.set(this, push_callback);
    _dec_widget->signal.set(this, dec_callback);
    _dec_widget->push_signal.set(this, push_callback);
}

Inc_Button::~Inc_Button()
{}

void Inc_Button::inc_callback(bool)
{
    inc_signal.emit(true);
}

void Inc_Button::dec_callback(bool)
{
    dec_signal.emit(true);
}

void Inc_Button::push_callback(bool in)
{
    push_signal.emit(in);
}

} // djv

