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

//! \file djv_group_box.cpp

#include <djv_group_box.h>

#include <djv_font.h>
#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl_Group.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Group_Box
//------------------------------------------------------------------------------

Group_Box::Group_Box(const String & label) :
    _label(label)
{
    debug_path("Group_Box::Group_Box");

    text_font(Style::global()->font_bold());

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());
}

Group_Box::~Group_Box()
{}

void Group_Box::del()
{
    Widget::del();
}

void Group_Box::label(const String & in)
{
    if (in == _label)
    {
        return;
    }

    _label = in;

    redraw();
}

const String & Group_Box::label() const
{
    return _label;
}

void Group_Box::draw()
{
    const Box2i & geom = this->geom();
    const int margin = Style::global()->margin_widget();

    const Style::Color & color_value = Style::global()->color_value();
    Color tmp(Pixel::RGB_U8);

    Color::scale(0.6, color_value.background, tmp);
    const Fl_Color color =
        fl_rgb_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    if (_label.empty())
    {
        // Draw box.

        fl_draw_box(
            FL_BORDER_FRAME,
            geom.x,
            geom.y,
            geom.w,
            geom.h,
            color);
    }
    else
    {
        // Draw box.

        fl_draw_box(
            FL_BORDER_FRAME,
            geom.x,
            geom.y + _size_label.y / 2,
            geom.w,
            geom.h - _size_label.y / 2,
            color);

        // Draw label.

        fl_font(text_font(), Style::global()->font_size());

        fl_color(FL_BACKGROUND_COLOR);
        //fl_color(FL_BLUE);
        fl_rectf(
            geom.x + 2 + margin,
            geom.y,
            _size_label.x + margin * 2,
            _size_label.y);

        fl_color(FL_FOREGROUND_COLOR);
        fl_draw(
            _label.c_str(),
            geom.x + 2 + margin * 2,
            geom.y + fl_height() - fl_descent());
    }

    Widget::draw();
}

void Group_Box::dirty()
{
    //DJV_DEBUG("Group_Box::dirty");

    Widget::dirty();

    fl_font(text_font(), Style::global()->font_size());

    const V2i size = layout() ? layout()->size_hint() : V2i();

    //DJV_DEBUG_PRINT("size = " << size);

    _size_label =
        _label.empty() ?
        V2i(0, 2) :
        V2i(Font::string_width(_label), fl_height());

    //DJV_DEBUG_PRINT("label size = " << _size_label);

    const int margin = Style::global()->margin_widget();
    const int spacing = Style::global()->spacing();

    size_hint(V2i(
        Math::max(_size_label.x + margin * 4, size.x) + 2 * 2,
        _size_label.y + spacing + size.y + 2));
}

void Group_Box::geom(const Box2i & in)
{
    //DJV_DEBUG("Group_Box::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    //Fl_Group::resize(in.x, in.y, in.w, in.h);
    Fl_Widget::resize(in.x, in.y, in.w, in.h);
    Layout_Item::geom(in);

    const int spacing = Style::global()->spacing();

    if (layout())
        layout()->geom(Box2i(
            in.x + 2,
            in.y + _size_label.y + spacing,
            in.w - 2 * 2,
            in.h - _size_label.y - spacing - 2));
}

const Box2i & Group_Box::geom() const
{
    return Widget::geom();
}

} // djv
