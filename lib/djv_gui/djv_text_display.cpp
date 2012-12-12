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

//! \file djv_text_display.cpp

#include <djv_text_display.h>

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
// Text_Display::Widget
//------------------------------------------------------------------------------

class Text_Display::Widget : public Fl_Output
{
public:

    Widget() :
        Fl_Output(0, 0, 0, 0)
    {
        box(FL_FLAT_BOX);

        //! \todo See Fl_Text_Display.cxx
        //scrollbar.slider(Style::FL_BOX_THIN_UP);
        //hscrollbar.slider(Style::FL_BOX_THIN_UP);
    }
};

//------------------------------------------------------------------------------
// Text_Display
//------------------------------------------------------------------------------

Text_Display::Text_Display(const String & in) :
    _value(in),
    _widget(0)
{
    debug_path("Text_Display");

    add(_widget = new Widget);

    widget_update();
}

Text_Display::~Text_Display()
{}

void Text_Display::set(const String & in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Text_Display::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _value = in;

    widget_update();
}

const String & Text_Display::get() const
{
    return _value;
}

void Text_Display::text_font(Fl_Font in)
{
    Abstract_Text_Widget::text_font(in);

    _widget->textfont(in);
}

Fl_Font Text_Display::text_font() const
{
    return Abstract_Text_Widget::text_font();
}

void Text_Display::text_color(Fl_Color in)
{
    Abstract_Text_Widget::text_color(in);

    _widget->textcolor(in);
}

Fl_Color Text_Display::text_color() const
{
    return Abstract_Text_Widget::text_color();
}

void Text_Display::size_string(const String & in)
{
    _size_string = in;
}

const String & Text_Display::size_string() const
{
    return _size_string;
}

void Text_Display::dirty()
{
    Abstract_Text_Widget::dirty();

    _widget->textfont(text_font());
    _widget->textsize(Style::global()->font_size());
    _widget->textcolor(text_color());

    size_hint(
        V2i(
            //Math::max(
            //  Style::global()->size_button(),
            //  base::string_size(_size_string)
            //),
            Font::string_width(_size_string),
            fl_height()
        ) +
        Style::global()->margin_text() * 2 +
        frame_size() * 2);
}

void Text_Display::geom(const Box2i & in)
{
    Abstract_Text_Widget::geom(in);

    const Box2i & geom = frame_geom();

    _widget->resize(geom.x, geom.y, geom.w, geom.h);
}

const Box2i & Text_Display::geom() const
{
    return Abstract_Text_Widget::geom();
}

void Text_Display::widget_update()
{
    _widget->value(_value.c_str());
    _widget->position(0, 0);
}

} // djv
