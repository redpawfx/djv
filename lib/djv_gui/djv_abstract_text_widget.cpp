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

//! \file djv_abstract_text_widget.cpp

#include <djv_abstract_text_widget.h>

#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Abstract_Text_Widget
//------------------------------------------------------------------------------

Abstract_Text_Widget::Abstract_Text_Widget() :
    _draw_select(false)
{
    style(STYLE_BORDER);

    size(STRETCH, FIXED);
}

Abstract_Text_Widget::~Abstract_Text_Widget()
{}

int Abstract_Text_Widget::handle(int in)
{
    switch (in)
    {
        case FL_KEYBOARD:
        {
            if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
                switch (Fl::event_key())
                {
                    case FL_Escape:
                    {
                        Fl::focus(0);
                    }

                    return 1;

                    default:
                        break;
                }
        }
        break;
    }

    return Frame::handle(in);
}

void Abstract_Text_Widget::draw()
{
    const bool inside = Abstract_Widget::inside();

    color(
        _draw_select && active() ?
        FL_SELECTION_COLOR :
        (inside && active() ? FL_COLOR_HIGHLIGHT : FL_BACKGROUND_COLOR));

    Frame::draw();
}

void Abstract_Text_Widget::draw_select(bool in)
{
    if (in == _draw_select)
    {
        return;
    }

    _draw_select = in;

    redraw();
}

void Abstract_Text_Widget::dirty()
{
    Frame::dirty();
}

} // djv

