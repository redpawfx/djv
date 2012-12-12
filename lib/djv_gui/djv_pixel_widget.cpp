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

//! \file djv_pixel_widget.cpp

#include <djv_pixel_widget.h>

#include <djv_choice_widget.h>
#include <djv_row_layout.h>

namespace djv
{

//------------------------------------------------------------------------------
// Pixel_Widget
//------------------------------------------------------------------------------

Pixel_Widget::Pixel_Widget() :
    signal(this),
    _value (Pixel::PIXEL(0)),
    _widget(0)
{
    debug_path("Pixel_Widget::Pixel_Widget");

    add(_widget = new Choice_Widget(Pixel::label_pixel()));

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    widget_update();

    _widget->signal.set(this, widget_callback);
}

Pixel_Widget::~Pixel_Widget()
{}

void Pixel_Widget::del()
{
    Widget::del();

    _widget->del();
}

void Pixel_Widget::set(Pixel::PIXEL in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();

    signal.emit(_value);
}

Pixel::PIXEL Pixel_Widget::get() const
{
    return _value;
}

void Pixel_Widget::draw()
{
    Widget::draw();
}

void Pixel_Widget::dirty()
{
    Widget::dirty();

    _widget->dirty();

    size_hint(_widget->size_hint());
}

void Pixel_Widget::geom(const Box2i & in)
{
    Widget::geom(in);

    _widget->geom(in);
}

const Box2i & Pixel_Widget::geom() const
{
    return Widget::geom();
}

void Pixel_Widget::widget_callback(int in)
{
    set(static_cast<Pixel::PIXEL>(in));
}

void Pixel_Widget::widget_update()
{
    callbacks(false);

    _widget->set(_value);

    callbacks(true);
}

} // djv

