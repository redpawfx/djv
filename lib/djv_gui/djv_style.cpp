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

//! \file djv_style.cpp

#include <djv_style.h>

#include <djv_application.h>

#include <FL/Fl.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Style::Color
//------------------------------------------------------------------------------

Style::Color::Color()
{}

Style::Color::Color(
    const djv::Color & foreground,
    const djv::Color & background,
    const djv::Color & background2,
    const djv::Color & button,
    const djv::Color & select,
    const djv::Color & highlight) :
    foreground (foreground),
    background (background),
    background2(background2),
    button     (button),
    select     (select),
    highlight  (highlight)
{}

bool Style::Color::operator == (const Color & other) const
{
    return
        foreground  == other.foreground  &&
        background  == other.background  &&
        background2 == other.background2 &&
        button      == other.button      &&
        select      == other.select      &&
        highlight   == other.highlight;
}

bool Style::Color::operator != (const Color & other) const
{
    return ! (*this == other);
}

//------------------------------------------------------------------------------
// Style
//------------------------------------------------------------------------------

namespace
{

const int _spacing       [] = { 4, 4, 4, 4 };
const int _spacing_large [] = { 16, 16, 16, 16 };

const int _margin        [] = { 4, 4, 4, 4 };
const int _margin_text   [] = { 2, 2, 2, 2 };
const int _margin_widget [] = { 4, 4, 4, 4 };
const int _margin_large  [] = { 16, 16, 16, 16 };

const int _size_icon      = 22;
const int _size_tool_icon = 11;

void box_draw(
    int      x,
    int      y,
    int      w,
    int      h,
    bool     west,
    bool     north,
    bool     east,
    bool     south,
    bool     down,
    Fl_Color fltk_color)
{
    bool select    = false;
    bool highlight = false;

    if (FL_SELECTION_COLOR == fltk_color)
    {
        select = true;
        
        fltk_color = FL_COLOR_BUTTON;
    }
    else if (FL_COLOR_HIGHLIGHT == fltk_color)
    {
        highlight = true;
        
        fltk_color = FL_COLOR_BUTTON;
    }

    djv::Color color = down ?
        djv::Color::scale(0.8, Style::fltk_to_color(fltk_color)) :
        Style::fltk_to_color(fltk_color);
    
    djv::Color tmp(Pixel::RGB_U8);

    // Draw border.

    int x1 = x;
    int y1 = y;
    int x2 = x + w - 1;
    int y2 = y + h - 1;

    if (select && Style_Prefs::global()->focus())
    {
        fl_color(FL_SELECTION_COLOR);
    }
    else if (highlight && Style_Prefs::global()->highlight())
    {
        fl_color(FL_COLOR_HIGHLIGHT);
    }
    else
    {
        fl_color(FL_COLOR_BORDER);
    }

    if (west)
    {
        fl_line(x1, y1 + north, x1, y2 - south);
    }

    if (north)
    {
        fl_line(x1 + west, y1, x2 - east, y1);
    }

    if (east)
    {
        fl_line(x2, y1 + north, x2, y2 - south);
    }

    if (south)
    {
        fl_line(x1 + west, y2, x2 - east, y2);
    }

    fl_rectf(
        x + west,
        y + north,
        w - (west + east),
        h - (north + south));

    // Draw background.

    djv::Color::convert(color, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    fl_rectf(
        x + west * 2,
        y + north * 2,
        w - (west + east) * 2,
        h - (north + south) * 2);

    // Draw west/north bevels.

    x1 += west;
    y1 += north;
    x2 -= east;
    y2 -= south;

    djv::Color::scale(down ? 0.8 : 1.2, color, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    if (west)
    {
        fl_line(x1, y1 + north, x1, y2 - south);
    }

    if (north)
    {
        fl_line(x1 + west, y1, x2 - east, y1);
    }

    // Draw east/south bevels.

    djv::Color::scale(down ? 1.2 : 0.8, color, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    if (east)
    {
        fl_line(x2, y1 + north, x2, y2 - south);
    }

    if (south)
    {
        fl_line(x1 + west, y2, x2 - east, y2);
    }
}

void box_up_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, true, true, true, false, fltk_color);
}

void box_up_h_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, true, false, true, false, fltk_color);
}

void box_up_h_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, false, true, false, true, false, fltk_color);
}

void box_up_h_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, false, true, true, true, false, fltk_color);
}

void box_up_v_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, true, true, false, false, fltk_color);
}

void box_up_v_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, false, true, false, false, fltk_color);
}

void box_up_v_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, false, true, true, false, fltk_color);
}

void box_down_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, true, true, true, true, fltk_color);
}

void box_down_h_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, true, false, true, true, fltk_color);
}

void box_down_h_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, false, true, false, true, true, fltk_color);
}

void box_down_h_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, false, true, true, true, true, fltk_color);
}

void box_down_v_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, true, true, false, true, fltk_color);
}

void box_down_v_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, false, true, false, true, fltk_color);
}

void box_down_v_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_draw(x, y, w, h, true, false, true, true, true, fltk_color);
}

void box_thin_draw(
    int      x,
    int      y,
    int      w,
    int      h,
    bool     down,
    Fl_Color fltk_color)
{
    bool select    = false;
    bool highlight = false;

    if (FL_SELECTION_COLOR == fltk_color)
    {
        select     = true;
        fltk_color = FL_COLOR_BUTTON;
    }
    else if (FL_COLOR_HIGHLIGHT == fltk_color)
    {
        highlight  = true;
        fltk_color = FL_COLOR_BUTTON;
    }

    const Style::Color & color_value = Style::global()->color_value();
    djv::Color color = Style::fltk_to_color(fltk_color);
    djv::Color tmp(Pixel::RGB_U8);

    // Draw background.

    djv::Color::convert(color, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    fl_rectf(x, y, w, h);

    if (select && Style_Prefs::global()->focus())
    {
        djv::Color::convert(color_value.select, tmp);
        fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

        fl_rect(x, y, w, h);
    }
    else if (highlight && Style_Prefs::global()->highlight())
    {
        djv::Color::convert(color_value.highlight, tmp);
        fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

        fl_rect(x, y, w, h);
    }
    else
    {
        // Draw west/north bevels.

        const int x1 = x;
        const int y1 = y;
        const int x2 = x + w - 1;
        const int y2 = y + h - 1;

        djv::Color::scale(down ? 0.8 : 1.2, color, tmp);
        fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

        fl_line(x1, y1, x1, y2); // west
        fl_line(x1, y1, x2, y1); // north

        // Draw east/south bevels.

        djv::Color::scale(down ? 1.2 : 0.8, color, tmp);
        fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

        fl_line(x2, y1 + 1, x2, y2); // east
        fl_line(x1 + 1, y2, x2, y2); // south
    }
}

void box_up_thin_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_thin_draw(x, y, w, h, false, fltk_color);
}

void box_down_thin_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_thin_draw(x, y, w, h, true, fltk_color);
}

void box_border_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    const bool select = FL_SELECTION_COLOR == fltk_color;
    const bool highlight = FL_COLOR_HIGHLIGHT == fltk_color;

    djv::Color color = Style::fltk_to_color(fltk_color);

    // Draw border.

    if (select && Style_Prefs::global()->focus())
    {
        fl_color(FL_SELECTION_COLOR);
    }
    else if (highlight && Style_Prefs::global()->highlight())
    {
        fl_color(FL_COLOR_HIGHLIGHT);
    }
    else
    {
        fl_color(FL_COLOR_BORDER);
    }

    fl_rect(x, y , w, h);
}

void box_menu_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
    box_thin_draw(x, y, w, h, false, FL_BACKGROUND_COLOR);
}

} // namespace

Style::Style() :
    _color(Style_Prefs::global()->color()),
    _size(Style_Prefs::global()->size())
{
    //DJV_DEBUG("Style::Style");
    //DJV_DEBUG_PRINT("color = " << _color);

    for (int i = 0; i < _COLOR_SIZE; ++i)
    {
        _color_value[i] =
            Style_Prefs::global()->color_value(static_cast<COLOR>(i));
    }

    for (int i = 0; i < _SIZE_SIZE; ++i)
    {
        _size_value[i] =
            Style_Prefs::global()->size_value_(static_cast<SIZE>(i));
    }

    Fl::set_boxtype(FL_BOX_UP, box_up_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_UP_H1, box_up_h_start_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_UP_H2, box_up_h_middle_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_UP_H3, box_up_h_end_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_UP_V1, box_up_v_start_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_UP_V2, box_up_v_middle_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_UP_V3, box_up_v_end_draw, 2, 2, 4, 4);

    Fl::set_boxtype(FL_BOX_DOWN, box_down_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_DOWN_H1, box_down_h_start_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_DOWN_H2, box_down_h_middle_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_DOWN_H3, box_down_h_end_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_DOWN_V1, box_down_v_start_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_DOWN_V2, box_down_v_middle_draw, 2, 2, 4, 4);
    Fl::set_boxtype(FL_BOX_DOWN_V3, box_down_v_end_draw, 2, 2, 4, 4);

    Fl::set_boxtype(FL_BOX_THIN_UP, box_up_thin_draw, 1, 1, 2, 2);
    Fl::set_boxtype(FL_BOX_THIN_DOWN, box_down_thin_draw, 1, 1, 2, 2);

    Fl::set_boxtype(FL_BOX_BORDER, box_border_draw, 1, 1, 2, 2);

    Fl::set_boxtype(FL_BOX_MENU, box_menu_draw, 1, 1, 2, 2);

    color_update(true);
    size_update();
}

const List<String> & Style::label_color()
{
    static const List<String> data = List<String>() <<
        "Dark" <<
        "Neutral" <<
        "Light" <<
        "Custom";

    DJV_ASSERT(data.size() == _COLOR_SIZE);

    return data;
}

void Style::color(COLOR in)
{
    if (in == _color)
    {
        return;
    }

    _color = in;

    color_update(true);
}

Style::COLOR Style::color() const
{
    return _color;
}

void Style::color_value(COLOR color, const Color & in)
{
    if (in == _color_value[color])
    {
        return;
    }

    _color_value[color] = in;

    color_update(true);
}

void Style::color_value(const Color & in)
{
    color_value(_color, in);
}

const Style::Color & Style::color_value(COLOR color) const
{
    return _color_value[color];
}

const Style::Color & Style::color_value() const
{
    return color_value(_color);
}

void Style::color_update()
{
    color_update(false);
}

void Style::color_update(bool redraw)
{
    //DJV_DEBUG("Style::color_update");
    //DJV_DEBUG_PRINT("redraw = " << redraw);

    //DJV_DEBUG_PRINT("foreground = " << _color_value[_color].foreground);
    //DJV_DEBUG_PRINT("background = " << _color_value[_color].background);
    //DJV_DEBUG_PRINT("background 2 = " << _color_value[_color].background2);
    //DJV_DEBUG_PRINT("button = " << _color_value[_color].button);
    //DJV_DEBUG_PRINT("select = " << _color_value[_color].select);
    //DJV_DEBUG_PRINT("highlight = " << _color_value[_color].highlight);

    djv::Color tmp(Pixel::RGB_U8);
    djv::Color::convert(_color_value[_color].background, tmp);
    Fl::background(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    djv::Color::convert(_color_value[_color].background2, tmp);
    Fl::background2(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    djv::Color::convert(_color_value[_color].foreground, tmp);
    Fl::foreground(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    djv::Color::convert(_color_value[_color].button, tmp);
    Fl::set_color(FL_COLOR_BUTTON, tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    djv::Color::convert(_color_value[_color].select, tmp);
    Fl::set_color(FL_SELECTION_COLOR, tmp.get_u8(0), tmp.get_u8(1),
                  tmp.get_u8(2));

    djv::Color::convert(_color_value[_color].highlight, tmp);
    Fl::set_color(FL_COLOR_HIGHLIGHT, tmp.get_u8(0), tmp.get_u8(1),
                  tmp.get_u8(2));

    djv::Color::scale(0.6, _color_value[_color].background, tmp);
    Fl::set_color(FL_COLOR_BORDER, tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

    if (redraw)
    {
        DJV_APP->window_redraw();
    }

    /*uchar r, g, b;
    Fl::get_color(FL_SELECTION_COLOR, r, g, b);

    //DJV_DEBUG_PRINT("r = " << r);
    //DJV_DEBUG_PRINT("g = " << g);
    //DJV_DEBUG_PRINT("b = " << b);*/
}

const List<String> & Style::label_size()
{
    static const List<String> data = List<String>() <<
        "Small" <<
        "Medium" <<
        "Large" <<
        "Custom";

    DJV_ASSERT(data.size() == _SIZE_SIZE);

    return data;
}

void Style::size(SIZE in)
{
    if (in == _size)
    {
        return;
    }

    _size = in;

    size_update();
}

Style::SIZE Style::size() const
{
    return _size;
}

void Style::size_value_(SIZE size, int in)
{
    if (in == _size_value[size])
    {
        return;
    }

    _size_value[size] = in;

    size_update();
}

int Style::size_value_(SIZE size) const
{
    return _size_value[size];
}

void Style::size_value(int in)
{
    size_value_(_size, in);
}

int Style::size_value() const
{
    return size_value_(_size);
}

int Style::size_pick() const
{
    return Math::round(_size_value[_size] * 0.8);
}

int Style::size_handle() const
{
    return Math::round(_size_value[_size] * 3.0);
}

void Style::size_update()
{
    //DJV_DEBUG("Style::size_update");

    Fl_Tooltip::size(_size_value[_size]);

    DJV_APP->window_update();
}

Fl_Font Style::font() const
{
    return FL_HELVETICA;
}

Fl_Font Style::font_bold() const
{
    return FL_HELVETICA_BOLD;
}

Fl_Font Style::font_fixed() const
{
    return FL_COURIER;
}

int Style::font_size() const
{
    return _size_value[_size];
}

int Style::spacing() const
{
    return _spacing[_size];
}

int Style::spacing_large() const
{
    return _spacing_large[_size];
}

int Style::margin() const
{
    return _margin[_size];
}

int Style::margin_text() const
{
    return _margin_text[_size];
}

int Style::margin_widget() const
{
    return _margin_widget[_size];
}

int Style::margin_large() const
{
    return _margin_large[_size];
}

int Style::size_icon() const
{
    return _size_icon;
}

int Style::size_tool_icon() const
{
    return _size_tool_icon;
}

int Style::size_button() const
{
    return Math::round(_size_value[_size] * 4.2);
}

int Style::size_text() const
{
    return Math::round(_size_value[_size] * 10.0);
}

int Style::size_swatch() const
{
    return Math::round(_size_value[_size] * 4.2);
}

int Style::size_thumbnail() const
{
    return Math::round(_size_value[_size] * 6.5);
}

djv::Color Style::fltk_to_color(Fl_Color in)
{
    djv::Color out(Pixel::RGB_U8);

    const Color & color_value = global()->color_value();
    djv::Color tmp;

    if (FL_FOREGROUND_COLOR == in)
    {
        tmp = color_value.foreground;
    }
    else if (FL_BACKGROUND_COLOR == in)
    {
        tmp = color_value.background;
    }
    else if (FL_BACKGROUND2_COLOR == in)
    {
        tmp = color_value.background2;
    }
    else if (FL_SELECTION_COLOR == in)
    {
        tmp = color_value.select;
    }
    else if (FL_COLOR_BUTTON == in)
    {
        tmp = color_value.button;
    }
    else if (FL_COLOR_HIGHLIGHT)
    {
        tmp = color_value.highlight;
    }

    djv::Color::convert(tmp, out);

    return out;
}

void Style::color_dark(Fl_Color in)
{
    djv::Color tmp(Pixel::RGB_U8);
    djv::Color::scale(0.8, fltk_to_color(in), tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
}

void Style::color_light(Fl_Color in)
{
    djv::Color tmp(Pixel::RGB_U8);
    djv::Color::scale(1.2, fltk_to_color(in), tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
}

Style * Style::global()
{
    static Style * global = 0;

    if (! global)
    {
        global = new Style;
    }

    return global;
}

//------------------------------------------------------------------------------

extern const Fl_Color FL_COLOR_BUTTON =
    static_cast<Fl_Color>(FL_FREE_COLOR + 0);
extern const Fl_Color FL_COLOR_HIGHLIGHT =
    static_cast<Fl_Color>(FL_FREE_COLOR + 1);
extern const Fl_Color FL_COLOR_BORDER =
    static_cast<Fl_Color>(FL_FREE_COLOR + 2);

extern const Fl_Boxtype FL_BOX_UP =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 0);
extern const Fl_Boxtype FL_BOX_UP_H1 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 1);
extern const Fl_Boxtype FL_BOX_UP_H2 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 2);
extern const Fl_Boxtype FL_BOX_UP_H3 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 3);
extern const Fl_Boxtype FL_BOX_UP_V1 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 4);
extern const Fl_Boxtype FL_BOX_UP_V2 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 5);
extern const Fl_Boxtype FL_BOX_UP_V3 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 6);

extern const Fl_Boxtype FL_BOX_DOWN =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 7);
extern const Fl_Boxtype FL_BOX_DOWN_H1 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 8);
extern const Fl_Boxtype FL_BOX_DOWN_H2 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 9);
extern const Fl_Boxtype FL_BOX_DOWN_H3 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 10);
extern const Fl_Boxtype FL_BOX_DOWN_V1 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 11);
extern const Fl_Boxtype FL_BOX_DOWN_V2 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 12);
extern const Fl_Boxtype FL_BOX_DOWN_V3 =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 13);

extern const Fl_Boxtype FL_BOX_THIN_UP =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 14);
extern const Fl_Boxtype FL_BOX_THIN_DOWN =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 15);

extern const Fl_Boxtype FL_BOX_BORDER =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 16);

extern const Fl_Boxtype FL_BOX_MENU =
    static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 17);

_DJV_STRING_OPERATOR_LABEL(Style::COLOR, Style::label_color())

String & operator >> (String & in, Style::Color & out) throw (String)
{
    in >> out.foreground;
    in >> out.background;
    in >> out.background2;
    in >> out.button;
    in >> out.select;
    in >> out.highlight;
    return in;
}

String & operator << (String & out, const Style::Color & in)
{
    out << in.foreground;
    out << in.background;
    out << in.background2;
    out << in.button;
    out << in.select;
    out << in.highlight;
    return out;
}

_DJV_STRING_OPERATOR_LABEL(Style::SIZE, Style::label_size())

} // djv

