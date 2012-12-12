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

//! \file djv_multiline_label.cpp

#include <djv_multiline_label.h>

#include <djv_font.h>
#include <djv_style.h>

#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Multiline_Label
//------------------------------------------------------------------------------

Multiline_Label::Multiline_Label(const String & in) :
    action_signal(this),
    _word_size_max(0),
    _space_size(0),
    _text_height(0),
    _font_size(0)
{
    debug_path("Multiline_Label");

    align(LEFT, CENTER);

    set(in);
}

Multiline_Label::~Multiline_Label()
{}

void Multiline_Label::set(const String & in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Multiline_Label::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _value = in;

    _words.clear();

    List<String> tmp;

    if (_value.size())
    {
        tmp = String_Util::split(_value, '\n', true);
    }

    for (size_t i = 0; i < tmp.size(); ++i)
    {
        if (tmp[i].size())
        {
            _words += String_Util::split(tmp[i], ' ', true);
        }

        if (i < tmp.size() - 1)
        {
            _words += "\n";
        }
    }

    _word_sizes.resize(_words.size() + 1);

    text_measure();
    update();
    redraw();
}

const String & Multiline_Label::get() const
{
    return _value;
}

int Multiline_Label::handle(int in)
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

void Multiline_Label::draw()
{
    Widget::draw();

    const Box2i & geom = this->geom();

    fl_font(text_font(), Style::global()->font_size());
    fl_color(FL_FOREGROUND_COLOR);

    int x = 0, y = 0;

    for (size_t i = 0; i < _words.size(); ++i)
    {
        if (_words[i] != "\n")
        {
            fl_draw(
                _words[i].c_str(),
                geom.x + offset(align()[0], geom.w, _text_hint.x) + x,
                geom.y + offset(align()[1], geom.h, _text_hint.y) + y +
                fl_height() - fl_descent());
        }

        x += _word_sizes[i];

        if ("\n" == _words[i] ||
            ((x + _word_sizes[i + 1]) > (geom.w - 1)))
        {
            x = 0;
            y += _text_height;
        }
    }
}

void Multiline_Label::dirty()
{
    //DJV_DEBUG("Multiline_Label::dirty");

    Widget::dirty();

    const int font_size = Style::global()->font_size();
    fl_font(text_font(), font_size);

    if (font_size != _font_size)
    {
        _font_size = font_size;
        text_measure();
    }

    _text_hint = text_hint(geom().size);

    _size_min = V2i(
        Math::max(_word_size_max, Style::global()->size_text()),
        _text_hint.y);

    //DJV_DEBUG_PRINT("text = " << _text_hint);
    //DJV_DEBUG_PRINT("min = " << _size_min);

    //! \todo Is this still necessary?

    //if (_text_hint != size_hint())
    const int slop = static_cast<int>(
        Vector_Util::length(Math::abs(_text_hint - size_hint())));

    if (slop > 20)
    {
        //DJV_DEBUG_PRINT("update");

        size_hint(_text_hint);

        update();
    }
}

void Multiline_Label::geom(const Box2i & in)
{
    //DJV_DEBUG("Multiline_Label::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    const Box2i geom = this->geom();

    Widget::geom(in);

    if (geom != this->geom())
    {
        const V2i size = text_hint(in.size);
        const V2i hint = size_hint();

        //DJV_DEBUG_PRINT("size = " << size);
        //DJV_DEBUG_PRINT("hint = " << hint);

        if (size != hint)
        {
            dirty();
        }
    }
}

const Box2i & Multiline_Label::geom() const
{
    return Widget::geom();
}

const V2i & Multiline_Label::size_min() const
{
    return _size_min;
}

Layout_Item * Multiline_Label::group_align()
{
    return 0;
}

Layout_Item * Multiline_Label::label_align()
{
    return this;
}

void Multiline_Label::text_measure()
{
    _word_size_max = 0;

    if (_words.size())
    {
        size_t i = 0;

        for (; i < _words.size(); ++i)
        {
            _word_sizes[i] =
                _words[i] != "\n" ?
                Font::string_width(_words[i] + " ") :
                0;

            _word_size_max = Math::max(_word_size_max, _word_sizes[i]);
        }

        _word_sizes[i] = 0;
    }

    _space_size = Font::string_width(" ");

    _text_height = fl_height();
}

V2i Multiline_Label::text_hint(const V2i & in) const
{
    V2i out;

    int _x = 0;
    int _y = _text_height;

    for (size_t i = 0; i < _words.size(); ++i)
    {
        const bool newline = "\n" == _words[i];

        _x += _word_sizes[i];

        if (newline ||
            ((_x + _word_sizes[i + 1]) > (in.x - 1)))
        {
            out.x = Math::max(out.x, _x);

            _x = 0;
            _y += _text_height;
        }
    }

    out.y = _y;

    return out;
}

} // djv
