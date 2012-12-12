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

//! \file djv_label.cpp

#include <djv_label.h>

#include <djv_font.h>
#include <djv_math.h>
#include <djv_style.h>

#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Label
//------------------------------------------------------------------------------

Label::Label(const String & in) :
    action_signal(this),
    _lines_max_width(0)
{
    debug_path("Label");

    box(FL_FLAT_BOX);

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    set(in);
}

Label::~Label()
{}

void Label::set(const String & in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Label::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _value = in;

    // Split the value into lines.

    _lines = String_Util::split(_value, '\n', true);

    // Update.

    const V2i size = size_hint();
    dirty();

    if (size != size_hint())
    {
        update();
    }

    redraw();
}

const String & Label::get() const
{
    return _value;
}

int Label::handle(int in)
{
    switch (in)
    {
        case FL_PUSH:
            action_signal.emit(true);
            return 1;
    }

    return Widget::handle(in);
}

namespace
{

int offset(Layout_Item::ALIGN in, int size, int size_hint)
{
    int out = 0;

    switch (in)
    {
        case Layout_Item::LEFT:
            break;

        case Layout_Item::CENTER:
            out += size / 2 - size_hint / 2;
            break;

        case Layout_Item::RIGHT:
            out += size - size_hint;
            break;
    }

    return out;
}

} // namespace

void Label::draw()
{
    Widget::draw();

    const Box2i & geom = this->geom();

    fl_font(text_font(), Style::global()->font_size());
    fl_color(text_color());

    // Draw each line of text.

    int x = geom.x + offset(align()[0], geom.w, _lines_max_width);
    int y = geom.y +
        offset(align()[1], geom.h, fl_height() *
        static_cast<int>(_lines.size()));

    for (size_t i = 0; i < _lines.size(); ++i, y += fl_height())
    {
        fl_draw(_lines[i].c_str(), x, y + fl_height() - fl_descent());
    }
}

void Label::size_string(const String & in)
{
    _size_string = in;
}

const String & Label::size_string() const
{
    return _size_string;
}

void Label::dirty()
{
    Widget::dirty();

    fl_font(text_font(), Style::global()->font_size());

    // Find the maximum width of the lines.

    _lines_max_width = 0;

    for (size_t i = 0; i < _lines.size(); ++i)
        _lines_max_width =
            Math::max(Font::string_width(_lines[i]), _lines_max_width);

    // Size hint is either the maximum line width or user-specified
    // "size_string".

    size_hint(V2i(
        Math::max(_lines_max_width, Font::string_width(_size_string)),
        static_cast<int>(_lines.size()) * fl_height()));
}

Layout_Item * Label::group_align()
{
    return 0;
}

Layout_Item * Label::label_align()
{
    return this;
}

} // djv

