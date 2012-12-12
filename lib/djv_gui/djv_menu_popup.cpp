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

//! \file djv_menu_popup.cpp

#include <djv_menu.h>

#include <djv_icon.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Popup_Menu::Widget
//------------------------------------------------------------------------------

class Popup_Menu::Widget : public Fl_Menu_Button
{
public:

    Widget(Popup_Menu * parent) :
        Fl_Menu_Button(0, 0, 0, 0),
        _parent(parent)
    {
        box(FL_BOX_MENU);
    }

    int handle(int in)
    {
        switch (in)
        {
            case FL_PUSH:
                _parent->down(true);
                _parent->select(true);
                break;

            case FL_RELEASE:
                _parent->down(false);
                break;

            case FL_FOCUS:
                _parent->select(true);
                break;

            case FL_UNFOCUS:
                _parent->down(false);
                _parent->select(false);
                break;
        }

        return Fl_Menu_Button::handle(in);
    }

    void draw()
    {}

private:

    Popup_Menu * _parent;
};

//------------------------------------------------------------------------------
// Popup_Menu
//------------------------------------------------------------------------------

Popup_Menu::Popup_Menu(const List<String> & list) :
    signal(this),
    _icon(Icon_Factory::get_("tool_dec")),
    _down(false),
    _select(false)
{
    // Create widgets.

    Frame::add(_widget = new Widget(this));

    // Layout.

    size(FIXED, FIXED);

    // Initialize.

    highlight(true);

    set(list);

    // Callbacks.

    _widget->callback(widget_callback, this);
}

void Popup_Menu::set(const List<String> & in)
{
    _group.del();

    for (size_t i = 0; i < in.size(); ++i)
    {
        _group.add(in[i]);
    }

    Memory_Buffer<Fl_Menu_Item> tmp;
    _group.fl(&tmp);
    _widget->clear();
    _widget->copy(tmp());
}

void Popup_Menu::set(const Menu_Item_Group & in)
{
    _group = in;

    Memory_Buffer<Fl_Menu_Item> tmp;
    _group.fl(&tmp);
    _widget->clear();
    _widget->copy(tmp());
}

const Menu_Item_Group & Popup_Menu::get() const
{
    return _group;
}

int Popup_Menu::value() const
{
    return _widget->value();
}

void Popup_Menu::draw()
{
    const Box2i & geom = this->geom();

    const bool inside = Abstract_Widget::inside();
    const bool select = _select && active();
    const bool highlight = inside && active();

    fl_draw_box(
        box_type(style(), _down),
        geom.x, geom.y, geom.w, geom.h,
        select ?
        FL_SELECTION_COLOR :
        (highlight ? FL_COLOR_HIGHLIGHT : FL_COLOR_BUTTON));

    if (_icon)
    {
        _icon->draw(
            x() + w() / 2 - _icon->w() / 2,
            y() + h() / 2 - _icon->h() / 2);
    }
}

void Popup_Menu::down(bool in)
{
    if (in == _down)
    {
        return;
    }

    _down = in;

    redraw();
}

void Popup_Menu::select(bool in)
{
    if (in == _select)
    {
        return;
    }

    _select = in;

    redraw();
}

void Popup_Menu::dirty()
{
    Frame::dirty();

    _widget->textfont(text_font());
    _widget->textsize(Style::global()->font_size());
    _widget->textcolor(text_color());

    size_hint(
        Style::global()->size_tool_icon() + Style::global()->margin() * 2);
}

void Popup_Menu::geom(const Box2i & in)
{
    Frame::geom(in);

    _widget->resize(in.x, in.y, in.w, in.h);
}

const Box2i & Popup_Menu::geom() const
{
    return Frame::geom();
}

void Popup_Menu::widget_callback()
{
    signal.emit(_widget->value());
}

} // djv
