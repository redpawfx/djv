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

//! \file djv_push_button.cpp

#include <djv_push_button.h>

#include <djv_font.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Push_Button::Push_Button
//------------------------------------------------------------------------------

Push_Button::Push_Button(const String & label) :
    signal(this),
    _shortcut(0),
    _pressed(false),
    _pressed_inside(false),
    _draw_index(-1)
{
    debug_path("Push_Button");

    Fl_Group::set_visible_focus();

    highlight(true);

    size(FIXED, FIXED);

    this->label(label);
}

Push_Button::~Push_Button()
{}

void Push_Button::label(const String & in)
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

const String & Push_Button::label() const
{
    return _label;
}

int Push_Button::handle(int in)
{
    const Box2i & geom = this->geom();
    const V2i mouse(Fl::event_x(), Fl::event_y());

    switch (in)
    {
        case FL_KEYBOARD:
            if (this == Fl::focus())
                switch (Fl::event_key())
                {
                    case ' ':

                        signal.emit(true);

                        redraw();

                        return 1;
                }

            break;

        case FL_PUSH:
        {
            take_focus();

            _pressed = true;
            _pressed_inside = true;

            redraw();
        }

        return 1;

        case FL_RELEASE:
        {
            if (Box_Util::intersect(geom, mouse))
            {
                signal.emit(true);
            }

            _pressed = false;
            _pressed_inside = false;

            redraw();
        }

        return 1;

        case FL_DRAG:

            if (Box_Util::intersect(geom, mouse))
            {
                if (! _pressed_inside)
                {
                    _pressed_inside = true;

                    redraw();
                }
            }
            else
            {
                if (_pressed_inside)
                {
                    _pressed_inside = false;

                    redraw();
                }
            }

            return 1;

        case FL_SHORTCUT:
            if (Fl::event_key() == _shortcut)
            {
                signal.emit(true);

                redraw();

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

void Push_Button::draw()
{
    const Box2i & geom = this->geom();

    const bool inside = Abstract_Widget::inside();
    const bool select = this == Fl::focus() && active();
    const bool highlight = inside && active();

    Color tmp(Pixel::RGB_U8);

    const bool down = _pressed && _pressed_inside;

    fl_draw_box(
        down ? FL_BOX_DOWN : FL_BOX_UP,
        geom.x, geom.y, geom.w, geom.h,
        select ?
        FL_SELECTION_COLOR :
        (highlight ? FL_COLOR_HIGHLIGHT : FL_COLOR_BUTTON));

    fl_font(text_font(), Style::global()->font_size());
    fl_color(text_color());

    int x = geom.x + (geom.w - Font::string_width(_draw_label)) / 2;
    int y = geom.y + (geom.h - fl_height()) / 2;

    fl_draw(_draw_label.c_str(), x, y + fl_height() - fl_descent());

    if (_draw_index != -1)
    {
        x += Font::string_width(String(_draw_label, 0, _draw_index));
        y += fl_height();
        fl_line(x, y, x + Font::char_width(_draw_label[_draw_index]) - 1, y);
    }
}

void Push_Button::dirty()
{
    //DJV_DEBUG("Push_Button::dirty");

    Widget::dirty();

    fl_font(text_font(), Style::global()->font_size());

    const V2i label_size = Font::string_size(_label);

    size_hint(
        V2i(
            Math::max(label_size.x, Style::global()->size_button()),
            label_size.y
        ) +
        Style::global()->margin_widget() * 2 +
        Fl::box_dx(FL_BOX_UP) * 2);
}

} // djv

