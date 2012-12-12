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

//! \file djv_style_prefs.cpp

#include <djv_style.h>

#include <djv_application.h>
#include <djv_prefs.h>

namespace djv
{

//------------------------------------------------------------------------------
// Style_Prefs
//------------------------------------------------------------------------------

Style_Prefs::Style_Prefs() :
    _color    (Style::COLOR_DARK),
    _focus    (true),
    _highlight(true),
    _size     (Style::SIZE_MEDIUM)
{
    //DJV_DEBUG("Style_Prefs::Style_Prefs");

    _color_value[Style::COLOR_DARK] =
        Style::Color(
            djv::Color(0.8f),
            djv::Color(0.3f),
            djv::Color(0.2f),
            djv::Color(0.4f),
            djv::Color(1.0f, 0.9f, 0.5f),
            djv::Color(0.6f, 0.6f, 0.8f));
    _color_value[Style::COLOR_NEUTRAL] =
        Style::Color(
            djv::Color(0.9f),
            djv::Color(0.5f),
            djv::Color(0.4f),
            djv::Color(0.6f),
            djv::Color(1.0f, 0.9f, 0.5f),
            djv::Color(0.5f, 0.5f, 0.9f));
    _color_value[Style::COLOR_LIGHT] =
        Style::Color(
            djv::Color(0.0f),
            djv::Color(0.8f),
            djv::Color(0.7f),
            djv::Color(0.8f),
            djv::Color(1.0f, 0.9f, 0.5f),
            djv::Color(0.4f, 0.4f, 1.0f));
    _color_value[Style::COLOR_CUSTOM] = _color_value[_color];

    _size_value[Style::SIZE_SMALL] = 10;
    _size_value[Style::SIZE_MEDIUM] = 12;
    _size_value[Style::SIZE_LARGE] = 14;
    _size_value[Style::SIZE_CUSTOM] = _size_value[_size];

    Prefs prefs(Prefs::prefs_global(), "style");
    Prefs::get_(&prefs, "color", &_color);

    for (int i = 0; i < Style::_COLOR_SIZE; ++i)
        Prefs::get_(
            &prefs,
            String_Format("color_value%%").arg(i),
            &_color_value[i]);

    Prefs::get_(&prefs, "focus", &_focus);
    Prefs::get_(&prefs, "highlight", &_highlight);
    Prefs::get_(&prefs, "size", &_size);

    for (int i = 0; i < Style::_SIZE_SIZE; ++i)
        Prefs::get_(
            &prefs,
            String_Format("size_value%%").arg(i),
            &_size_value[i]);
}

Style_Prefs::~Style_Prefs()
{
    //DJV_DEBUG("Style_Prefs::~Style_Prefs");

    Prefs prefs(Prefs::prefs_global(), "style");
    Prefs::set_(&prefs, "color", _color);

    for (int i = 0; i < Style::_COLOR_SIZE; ++i)
        Prefs::set_(
            &prefs,
            String_Format("color_value%%").arg(i),
            _color_value[i]);

    Prefs::set_(&prefs, "focus", _focus);
    Prefs::set_(&prefs, "highlight", _highlight);
    Prefs::set_(&prefs, "size", _size);

    for (int i = 0; i < Style::_SIZE_SIZE; ++i)
        Prefs::set_(
            &prefs,
            String_Format("size_value%%").arg(i),
            _size_value[i]);
}

void Style_Prefs::color(Style::COLOR in)
{
    if (in == _color)
    {
        return;
    }

    _color = in;

    Style::global()->color(_color);
}

Style::COLOR Style_Prefs::color() const
{
    return _color;
}

void Style_Prefs::color_value(Style::COLOR color, const Style::Color & in)
{
    if (in == _color_value[color])
    {
        return;
    }

    _color_value[color] = in;

    Style::global()->color_value(_color, _color_value[color]);
}

void Style_Prefs::color_value(const Style::Color & in)
{
    color_value(_color, in);
}

const Style::Color & Style_Prefs::color_value(Style::COLOR color) const
{
    return _color_value[color];
}

const Style::Color & Style_Prefs::color_value() const
{
    return color_value(_color);
}

void Style_Prefs::focus(bool in)
{
    if (in == _focus)
    {
        return;
    }

    _focus = in;

    DJV_APP->window_redraw();
}

bool Style_Prefs::focus() const
{
    return _focus;
}

void Style_Prefs::highlight(bool in)
{
    if (in == _highlight)
    {
        return;
    }

    _highlight = in;

    DJV_APP->window_redraw();
}

bool Style_Prefs::highlight() const
{
    return _highlight;
}

void Style_Prefs::size(Style::SIZE in)
{
    if (in == _size)
    {
        return;
    }

    _size = in;

    Style::global()->size(_size);
}

Style::SIZE Style_Prefs::size() const
{
    return _size;
}

void Style_Prefs::size_value_(Style::SIZE size, int in)
{
    if (in == _size_value[size])
    {
        return;
    }

    _size_value[size] = in;

    Style::global()->size_value_(size, _size_value[size]);
}

void Style_Prefs::size_value(int in)
{
    size_value_(_size, in);
}

int Style_Prefs::size_value_(Style::SIZE size) const
{
    return _size_value[size];
}

int Style_Prefs::size_value() const
{
    return size_value_(_size);
}

} // djv

