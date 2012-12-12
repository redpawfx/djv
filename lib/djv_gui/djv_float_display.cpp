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

//! \file djv_float_display.cpp

#include <djv_float_display.h>

#include <djv_row_layout.h>
#include <djv_text_display.h>

namespace djv
{

//------------------------------------------------------------------------------
// Float_Display::Float_Display
//------------------------------------------------------------------------------

namespace
{

const String format_float = "000.00";

} // namespace

Float_Display::Float_Display() :
    _value    (0.0),
    _precision(0),
    _widget   (0)
{
    debug_path("Float_Display");

    // Create widgets.

    _widget = new Text_Display;
    _widget->size_string(format_float);

    // Layout.

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->spacing(0);
    layout->margin(0);
    layout->add(_widget);

    // Initialize.

    widget_update();
}

Float_Display::~Float_Display()
{}

void Float_Display::set(double in)
{
    _value = in;

    widget_update();
}

double Float_Display::get() const
{
    return _value;
}

void Float_Display::precision(int in)
{
    _precision = in;
}

int Float_Display::precision() const
{
    return _precision;
}

void Float_Display::text_color(Fl_Color in)
{
    Widget::text_color(in);

    _widget->text_color(in);
}

Fl_Color Float_Display::text_color() const
{
    return Widget::text_color();
}

void Float_Display::draw()
{
    Widget::draw();
}

void Float_Display::dirty()
{
    Widget::dirty();

    _widget->text_color(text_color());
}

void Float_Display::widget_update()
{
    _widget->set(String_Util::float_to_string(_value, _precision));
}

} // djv

