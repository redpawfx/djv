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

//! \file djv_radio_button.cpp

#include <djv_radio_button.h>

#include <djv_font.h>
#include <djv_icon.h>
#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Radio_Button
//------------------------------------------------------------------------------

Radio_Button::Radio_Button(const String & label) :
    signal(this),
    group_signal(this),
    _value(false),
    _shortcut(0),
    _icon(Icon_Factory::get_("tool_radio")),
    _draw_index(-1)
{
    debug_path("Radio_Button");

    Fl_Group::set_visible_focus();

    highlight(true);

    size(FIXED, FIXED);
    align(LEFT, LEFT);

    this->label(label);
}

Radio_Button::~Radio_Button()
{}

void Radio_Button::set(bool in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Radio_Button::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _value = in;

    widget_update();

    signal.emit(_value);

    if (_value)
    {
        const int group_value = group_get();

        for (size_t i = 0; i < _group.size(); ++i)
        {
            _group[i]->group_signal.emit(group_value);
        }
    }
}

bool Radio_Button::get() const
{
    return _value;
}

void Radio_Button::group_set(int in)
{
    //DJV_DEBUG("Radio_Button::group_set");
    //DJV_DEBUG_PRINT("in = " << in);

    if (in >= 0 && in < static_cast<int>(_group.size()))
    {
        _group[in]->set(true);
    }
}

void Radio_Button::group(const List<Radio_Button *> & in)
{
    _group = in;
}

int Radio_Button::group_get() const
{
    int out = -1;

    for (size_t i = 0; i < _group.size(); ++i)
        if (_group[i]->get())
        {
            out = static_cast<int>(i);
            break;
        }

    return out;
}

void Radio_Button::create_group(const List<Radio_Button *> & in)
{
    for (size_t i = 0; i < in.size(); ++i)
    {
        in[i]->group(in);
    }
}

const List<Radio_Button *> & Radio_Button::group() const
{
    return _group;
}

void Radio_Button::label(const String & in)
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

const String & Radio_Button::label() const
{
    return _label;
}

int Radio_Button::handle(int in)
{
    const V2i mouse(Fl::event_x(), Fl::event_y());

    switch (in)
    {
        case FL_KEYBOARD:
            if (this == Fl::focus())
                switch (Fl::event_key())
                {
                    case ' ':

                        set(true);

                        return 1;
                }

            break;

        case FL_PUSH:
        {
            take_focus();

            set(true);
        }

        return 1;

        case FL_SHORTCUT:
            if (Fl::event_key() == _shortcut)
            {
                set(true);

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

void Radio_Button::draw()
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
    {
        _icon->draw(
            x + (fl_height() - _icon->w()) / 2,
            y + (fl_height() - _icon->h()) / 2);
    }

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

void Radio_Button::dirty()
{
    Widget::dirty();

    fl_font(text_font(), Style::global()->font_size());

    const int margin = Style::global()->margin();

    size_hint(V2i(
        fl_height() + margin + Font::string_width(_draw_label),
        fl_height()));
}

void Radio_Button::widget_update()
{
    callbacks(false);

    if (_value)
        for (size_t i = 0; i < _group.size(); ++i)
            if (_group[i] != this)
            {
                _group[i]->set(false);
            }

    redraw();

    callbacks(true);
}

} // djv

