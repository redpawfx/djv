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

//! \file djv_image_label.cpp

#include <djv_image_label.h>

#include <djv_style.h>

#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Image_Label
//------------------------------------------------------------------------------

Image_Label::Image_Label(Fl_Image * in) :
    action_signal(this),
    _value(in)
{
    debug_path("Image_Label");
}

Image_Label::~Image_Label()
{}

void Image_Label::set(Fl_Image * in)
{
    _value = in;

    update();
    redraw();
}

Fl_Image * Image_Label::get() const
{
    return _value;
}

int Image_Label::handle(int in)
{
    switch (in)
    {
        case FL_PUSH:
            action_signal.emit(true);
            return 1;
    }

    return Widget::handle(in);
}

void Image_Label::draw()
{
    Widget::draw();

    const Box2i & geom = this->geom();

    if (_value)
    {
        _value->draw(
            geom.x + (geom.w - _value->w()) / 2,
            geom.y + (geom.h - _value->h()) / 2);
    }
}

void Image_Label::dirty()
{
    Widget::dirty();

    size_hint(_value ? V2i(_value->w(), _value->h()) : V2i());
}

Layout_Item * Image_Label::group_align()
{
    return 0;
}

Layout_Item * Image_Label::label_align()
{
    return this;
}

} // djv

