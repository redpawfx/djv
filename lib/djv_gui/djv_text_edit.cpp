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

//! \file djv_text_edit.cpp

#include <djv_text_edit.h>

#include <djv_style.h>
#include <djv_font.h>

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Text_Edit::Input
//------------------------------------------------------------------------------

struct Text_Edit::Input : public Fl_Input
{
    Input(Abstract_Text_Widget * parent) :
        Fl_Input(0, 0, 0, 0),
        _parent(parent)
    {
        when(FL_WHEN_CHANGED);

        box(FL_FLAT_BOX);
    }

    int handle(int in)
    {
        switch (in)
        {
            case FL_FOCUS:
                _parent->draw_select(true);
                break;

            case FL_UNFOCUS:
                _parent->draw_select(false);
                break;
        }

        return Fl_Input::handle(in);
    }

private:

    Abstract_Text_Widget * _parent;
};

//------------------------------------------------------------------------------
// Text_Edit
//------------------------------------------------------------------------------

Text_Edit::Text_Edit(const String & in) :
    signal(this),
    action_signal(this),
    change_signal(this),
    _value       (in),
    _widget      (0)
{
    debug_path("Text_Edit");

    // Create widgets.

    add(_widget = new Input(this));

    // Initialize.

    highlight(true);

    widget_update();

    // Callbacks.

    _widget->callback(widget_callback, this);
}

Text_Edit::~Text_Edit()
{}

void Text_Edit::set(const String & in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Text_Edit::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _value = in;

    widget_update();

    signal.emit(_value);
}

const String & Text_Edit::get() const
{
    return _value;
}

void Text_Edit::text_font(Fl_Font in)
{
    Abstract_Text_Widget::text_font(in);

    _widget->textfont(in);
}

void Text_Edit::color(Fl_Color in)
{
    Abstract_Text_Widget::color(in);

    _widget->color(in);
}

Fl_Color Text_Edit::color() const
{
    return Abstract_Text_Widget::color();
}

Fl_Font Text_Edit::text_font() const
{
    return Abstract_Text_Widget::text_font();
}

void Text_Edit::text_color(Fl_Color in)
{
    Abstract_Text_Widget::text_color(in);

    _widget->textcolor(in);
}

Fl_Color Text_Edit::text_color() const
{
    return Abstract_Text_Widget::text_color();
}

int Text_Edit::handle(int in)
{
    switch (in)
    {
        case FL_KEYBOARD:
        {
            if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
                switch (Fl::event_key())
                {
                    case FL_Enter:
                    case FL_KP_Enter:
                    {
                        signal.emit(_widget->value());
                        action_signal.emit(_widget->value());
                    }

                    return 1;

                    default:
                        break;
                }
        }
        break;

        case FL_UNFOCUS:

            signal.emit(_widget->value());

            break;
    }

    return Abstract_Text_Widget::handle(in);
}

const Fl_Widget * Text_Edit::fltk_widget() const
{
    return _widget;
}

void Text_Edit::size_string(const String & in)
{
    _size_string = in;
}

const String & Text_Edit::size_string() const
{
    return _size_string;
}

void Text_Edit::dirty()
{
    Abstract_Text_Widget::dirty();

    _widget->textfont(text_font());
    _widget->textsize(Style::global()->font_size());
    _widget->textcolor(text_color());

    fl_font(text_font(), Style::global()->font_size());

    size_hint(
        V2i(
            Font::string_width(_size_string),
            fl_height()
        ) +
        Style::global()->margin_text() * 2 +
        frame_size() * 2);
}

void Text_Edit::geom(const Box2i & in)
{
    Abstract_Text_Widget::geom(in);

    const Box2i & geom = frame_geom();

    _widget->resize(geom.x, geom.y, geom.w, geom.h);
}

const Box2i & Text_Edit::geom() const
{
    return Abstract_Text_Widget::geom();
}

void Text_Edit::widget_callback()
{
    _value = _widget->value();

    change_signal.emit(_value);
}

void Text_Edit::widget_update()
{
    _widget->value(_value.c_str());
    _widget->position(static_cast<int>(_value.size()));
}

} // djv

