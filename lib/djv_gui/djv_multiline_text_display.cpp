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

//! \file djv_multiline_text_display.cpp

#include <djv_multiline_text_display.h>

#include <djv_font.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Multiline_Text_Display::Widget
//------------------------------------------------------------------------------

class Multiline_Text_Display::Widget : public Fl_Text_Display
{
public:

    Widget() :
        Fl_Text_Display(0, 0, 0, 0)
    {
        buffer(&_value);

        box(FL_FLAT_BOX);
    }

    void set(const String & in)
    {
        _value.text(in.c_str());
    }

private:

    Fl_Text_Buffer _value;
};

//------------------------------------------------------------------------------
// Multiline_Text_Display
//------------------------------------------------------------------------------

Multiline_Text_Display::Multiline_Text_Display(const String & in) :
    _value(in),
    _widget(0)
{
    debug_path("Multiline_Text_Display");

    // Create widgets.

    add(_widget = new Widget);

    // Initialize.

    widget_update();
}

Multiline_Text_Display::~Multiline_Text_Display()
{}

void Multiline_Text_Display::set(const String & in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Multiline_Text_Display::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _value = in;

    widget_update();
}

const String & Multiline_Text_Display::get() const
{
    return _value;
}

void Multiline_Text_Display::text_font(Fl_Font in)
{
    Abstract_Text_Widget::text_font(in);

    _widget->textfont(in);
}

Fl_Font Multiline_Text_Display::text_font() const
{
    return Abstract_Text_Widget::text_font();
}

void Multiline_Text_Display::text_color(Fl_Color in)
{
    Abstract_Text_Widget::text_color(in);

    _widget->textcolor(in);
}

Fl_Color Multiline_Text_Display::text_color() const
{
    return Abstract_Text_Widget::text_color();
}

void Multiline_Text_Display::dirty()
{
    Abstract_Text_Widget::dirty();

    _widget->textfont(text_font());
    _widget->textsize(Style::global()->font_size());
    _widget->textcolor(text_color());

    fl_font(text_font(), Style::global()->font_size());

    const V2i size = Font::string_size(String(20, '0'));

    size_hint(
        V2i(size.x, size.y * 8) +
        Style::global()->margin_text() * 2 +
        frame_size() * 2);
}

void Multiline_Text_Display::geom(const Box2i & in)
{
    //DJV_DEBUG("Multiline_Text_Display::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    const Box2i geom = this->geom();

    Abstract_Text_Widget::geom(in);

    if (geom != this->geom())
    {
        const Box2i & frame_geom = this->frame_geom();

        _widget->resize(frame_geom.x, frame_geom.y, frame_geom.w, frame_geom.h);

        widget_update();
    }
}

const Box2i & Multiline_Text_Display::geom() const
{
    return Abstract_Text_Widget::geom();
}

void Multiline_Text_Display::widget_update()
{
    _widget->set(_value);
}

} // djv
