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

//! \file djv_check_button.cpp

#include <djv_check_button.h>

#include <djv_font.h>
#include <djv_icon.h>
#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Check_Button
//------------------------------------------------------------------------------

Check_Button::Check_Button(const String & label) :
    signal(this),
    _value(false),
    _shortcut(0),
    _icon(Icon_Factory::get_("tool_check")),
    _draw_index(-1)
{
    debug_path("Check_Button");

    Fl_Group::set_visible_focus();

    highlight(true);

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    this->label(label);
}

Check_Button::~Check_Button()
{}

void Check_Button::set(bool in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    signal.emit(_value);

    redraw();
}

void Check_Button::toggle()
{
    _value = ! _value;

    signal.emit(_value);

    redraw();
}

bool Check_Button::get() const
{
    return _value;
}

void Check_Button::label(const String & in)
{
    if (in == _label)
    {
        return;
    }

    _label = in;

    _draw_label = _label;
    String::size_type i = _draw_label.find('&');

    if (i != String::npos)
    {
        _shortcut = String_Util::lower(_draw_label[i + 1]);

        _draw_index = static_cast<int>(i);
        _draw_label.erase(i, 1);
    }

    redraw();
}

const String & Check_Button::label() const
{
    return _label;
}

int Check_Button::handle(int in)
{
    const V2i mouse(Fl::event_x(), Fl::event_y());

    switch (in)
    {
        case FL_KEYBOARD:
            if (this == Fl::focus())
                switch (Fl::event_key())
                {
                    case ' ':

                        toggle();

                        return 1;
                }

            break;

        case FL_PUSH:
        {
            take_focus();

            toggle();
        }

        return 1;

        case FL_SHORTCUT:
            if (Fl::event_key() == _shortcut)
            {
                toggle();

                return 1;
            }

            break;

        case FL_FOCUS:
        case FL_UNFOCUS:
            redraw();
            return 1;
    }

    return Widget::handle(in);
}

void Check_Button::draw()
{
    const Box2i & geom = this->geom();

    const bool inside = Abstract_Widget::inside();

    Color tmp(Pixel::RGB_U8);

    fl_font(text_font(), Style::global()->font_size());

    int x = geom.x;
    int y = geom.y;

    fl_rectf(x, y, geom.w, geom.h, FL_BACKGROUND_COLOR);

    fl_rectf(x, y, fl_height(), fl_height(), FL_BACKGROUND2_COLOR);
    fl_draw_box(
        FL_BOX_BORDER,
        x, y, fl_height(), fl_height(),
        this == Fl::focus() && active() ?
        FL_SELECTION_COLOR :
        (inside && active() ? FL_COLOR_HIGHLIGHT : FL_BACKGROUND_COLOR));

    if (_value)
        _icon->draw(
            x + (fl_height() - _icon->w()) / 2,
            y + (fl_height() - _icon->h()) / 2);

    x += fl_height() + Style::global()->margin();

    fl_color(text_color());

    fl_draw(_draw_label.c_str(), x, y + fl_height() - fl_descent());

    if (_draw_index != -1)
    {
        x += Font::string_width(String(_draw_label, 0, _draw_index));
        y += fl_height();
        fl_line(x, y, x + Font::char_width(_draw_label[_draw_index]) - 1, y);
    }
}

void Check_Button::dirty()
{
    Widget::dirty();

    fl_font(text_font(), Style::global()->font_size());

    const int margin = Style::global()->margin();

    size_hint(V2i(
        fl_height() + margin + Font::string_width(_draw_label),
        fl_height()));
}

} // djv

