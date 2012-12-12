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

//! \file djv_abstract_slider.cpp

#include <djv_abstract_slider.h>

#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Abstract_Slider
//------------------------------------------------------------------------------

Abstract_Slider::Abstract_Slider() :
    _handle_size(10)
{
    // Layout.

    size(STRETCH, FIXED);

    // Initialize.

    Fl_Group::color(FL_BACKGROUND2_COLOR);
    Fl_Group::set_visible_focus();

    style(STYLE_BORDER);

    highlight(true);
}

Abstract_Slider::~Abstract_Slider()
{}

int Abstract_Slider::handle(int in)
{
    switch (in)
    {
        case FL_FOCUS:
        case FL_UNFOCUS:
            redraw();
            return 1;
    }

    return Frame::handle(in);
}

void Abstract_Slider::draw()
{
    const Box2i & geom = frame_geom();

    //const bool inside = Abstract_Widget::inside();

    fl_color(FL_BACKGROUND2_COLOR);
    fl_rectf(geom.x, geom.y, geom.w, geom.h);

    Frame::draw();
}

void Abstract_Slider::handle_size(int in)
{
    _handle_size = in;
}

int Abstract_Slider::handle_size() const
{
    return _handle_size;
}

Box2i Abstract_Slider::slider_geom() const
{
    return Box_Util::border(frame_geom(), V2i(-_handle_size / 2, 0));
}

void Abstract_Slider::handle_draw(
    double        in,
    const Box2i & contents,
    bool          inside)
{
    //DJV_DEBUG("Abstract_Slider::handle_draw");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("contents = " << contents);
    //DJV_DEBUG_PRINT("inside = " << inside);

    fl_color(fl_darker(FL_BACKGROUND2_COLOR));
    fl_rectf(
        contents.x + _handle_size / 2,
        contents.y + contents.h / 2 - 1,
        contents.w - _handle_size,
        2);

    const Box2i box(
        contents.x + static_cast<int>(in * (contents.w - _handle_size)),
        contents.y,
        _handle_size,
        contents.h);

    fl_draw_box(
        FL_BOX_THIN_UP,
        box.x, box.y, box.w, box.h,
        this == Fl::focus() && active() ?
        FL_SELECTION_COLOR :
        (inside && active() ? FL_COLOR_HIGHLIGHT : FL_COLOR_BUTTON));
}

void Abstract_Slider::dirty()
{
    Frame::dirty();

    fl_font(Style::global()->font(), Style::global()->font_size());

    const int frame_margin = 2;

    size_hint(
        V2i(Style::global()->size_button() * 2, fl_height()) +
        Style::global()->margin_text() * 2 + frame_margin * 2);
}

} // djv

