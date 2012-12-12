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

//! \file djv_choice_widget.cpp

#include <djv_choice_widget.h>

#include <djv_font.h>
#include <djv_icon.h>
#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Choice_Widget::_Menu_Button
//------------------------------------------------------------------------------

class Choice_Widget::_Menu_Button :
    public Fl_Menu_Button,
    public Abstract_Widget
{
public:

    _Menu_Button(Choice_Widget * parent);

    void set(int);

    void list(const List<String> &);

    int get() const
    {
        return _value;
    }

    Signal<int> signal;

    void text(const List<String> &);

    int handle(int);

    void show();

    void draw();

    const Fl_Widget * fltk_widget() const
    {
        return this;
    }

    void dirty();

    void geom(const Box2i &);

    const Box2i & geom() const
    {
        return Abstract_Widget::geom();
    }

    void resize(int x, int y, int w, int h)
    {
        geom(Box2i(x, y, w, h));
    }

    int visible() const
    {
        return Fl_Menu_Button::visible();
    }

private:

    DJV_FL_WIDGET_CALLBACK(_Menu_Button, menu_callback)
    {
        set(value());
    }

    void widget_update();

    int _value;

    List<Memory_Buffer<char> > _list;

    List<Memory_Buffer<char> > _text;

    Fl_Image * _icon;

    Choice_Widget * _parent;
};

Choice_Widget::_Menu_Button::_Menu_Button(Choice_Widget * parent) :
    Fl_Menu_Button(0, 0, 0, 0),
    signal(this),
    _value(0),
    _icon(Icon_Factory::get_("tool_dec")),
    _parent(parent)
{
    debug_path("choice::_Menu_Button");

    box(FL_BOX_MENU);

    widget_update();

    callback(menu_callback, this);
}

void Choice_Widget::_Menu_Button::set(int in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    redraw();

    signal.emit(_value);
}

namespace
{

String pack(const String & in)
{
    String out = in;

    String::size_type pos = 0;

    while ((pos = out.find('&', pos)) != String::npos)
    {
        out = out.replace(pos, 1, 0, ' ');
    }

    return out;
}

List<Memory_Buffer<char> > cstr_list(const List<String> & in)
{
    List<Memory_Buffer<char> > out(in.size());

    for (size_t i = 0; i < in.size(); ++i)
    {
        const String tmp = pack(in[i]);

        out[i].size(tmp.size() + 1);

        char * p = out[i]();
        Memory::copy(tmp.c_str(), p, tmp.size());
        p[tmp.size()] = 0;
    }

    return out;
}

} // namespace

void Choice_Widget::_Menu_Button::list(const List<String> & in)
{
    _list = cstr_list(in);

    _value = 0;

    widget_update();

    signal.emit(_value);
}

void Choice_Widget::_Menu_Button::text(const List<String> & in)
{
    _text = cstr_list(in);

    _value = 0;

    widget_update();

    signal.emit(_value);
}

int Choice_Widget::_Menu_Button::handle(int in)
{
    switch (Abstract_Widget::handle(in))
    {
        case -1:
            return 0;

        case 1:
            return 1;
    }

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

void Choice_Widget::_Menu_Button::show()
{
    Fl_Menu_Button::show();

    Abstract_Widget::show();
}

void Choice_Widget::_Menu_Button::draw()
{
    //DJV_DEBUG("_Menu_Button::draw");

    const Box2i & geom = this->geom();

    //DJV_DEBUG_PRINT("geom = " << geom);

    // Label.

    const int margin = Fl::box_dx(box()) + 3;

    //DJV_DEBUG_PRINT("margin = " << margin);

    Box2i tmp = Box_Util::border(geom, -V2i(margin));

    if (_value >= 0 && _value < static_cast<int>(_list.size()))
    {
        //DJV_DEBUG_PRINT("value = " << _list[_value]());

        fl_font(textfont(), textsize());
        fl_color(text_color());
        fl_draw(
            _list[_value](),
            tmp.x,
            tmp.y + fl_height() - fl_descent());
    }

    // Icon.

    if (_icon)
        _icon->draw(
            tmp.x + tmp.w - Style::global()->size_tool_icon(),
            tmp.y + tmp.h / 2 - _icon->h() / 2);
}

void Choice_Widget::_Menu_Button::dirty()
{
    //DJV_DEBUG("Choice_Widget::_Menu_Button::dirty");

    Abstract_Widget::dirty();

    textfont(text_font());
    textsize(Style::global()->font_size());
    textcolor(text_color());

    fl_font(text_font(), Style::global()->font_size());
    V2i size;

    for (size_t i = 0; i < _list.size(); ++i)
    {
        size = Vector_Util::max(Font::string_size(_list[i]()), size);
    }

    size_hint(
        V2i(
            size.x +
            Style::global()->size_tool_icon() +
            Style::global()->margin_widget(),
            size.y
        ) +
        Style::global()->margin_widget() * 2);
}

void Choice_Widget::_Menu_Button::geom(const Box2i & in)
{
    Fl_Menu_Button::resize(in.x, in.y, in.w, in.h);

    Abstract_Widget::geom(in);
}

void Choice_Widget::_Menu_Button::widget_update()
{
    callbacks(false);

    clear();

    const List<Memory_Buffer<char> > & list = _text.size() ? _text : _list;

    const size_t size = list.size();
    Memory_Buffer<Fl_Menu_Item> tmp(size + 1);
    Memory::set<uint8_t>(0, tmp(), (size + 1) * sizeof(Fl_Menu_Item));

    Fl_Menu_Item * p = tmp();

    for (size_t i = 0; i < size; ++i)
    {
        p[i].text = list[i]();
        p[i].callback_ = menu_callback;
        p[i].user_data_ = this;
    }

    copy(tmp());

    update();

    callbacks(true);
}

//------------------------------------------------------------------------------
// Choice_Widget
//------------------------------------------------------------------------------

void Choice_Widget::init(
    const List<String> & list,
    const List<String> & text)
{
    //DJV_DEBUG("Choice_Widget::init");

    debug_path("Choice_Widget");

    _down = false;
    _select = false;

    highlight(true);

    _widget = new _Menu_Button(this);

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    _layout = new Vertical_Layout(this);
    _layout->margin(0);
    _layout->add(_widget);

    _widget->list(list);
    _widget->text(text);

    _widget->signal.set(this, widget_callback);
}

Choice_Widget::Choice_Widget() :
    signal (this),
    _widget(0),
    _layout(0)
{
    init(List<String>(), List<String>());
}

Choice_Widget::Choice_Widget(const List<String> & list) :
    signal (this),
    _widget(0),
    _layout(0)
{
    init(list, List<String>());
}

Choice_Widget::Choice_Widget(
    const List<String> & list,
    const List<String> & text) :
    signal (this),
    _widget(0),
    _layout(0)
{
    init(list, text);
}

Choice_Widget::~Choice_Widget()
{}

void Choice_Widget::set(int in)
{
    _widget->set(in);
}

int Choice_Widget::get() const
{
    return _widget->get();
}

void Choice_Widget::list(const List<String> & in)
{
    if (in == _list)
    {
        return;
    }

    _list = in;

    _widget->list(_list);
}

const List<String> & Choice_Widget::list() const
{
    return _list;
}

void Choice_Widget::text(const List<String> & in)
{
    if (in == _text)
    {
        return;
    }

    _text = in;

    _widget->text(_text);
}

const List<String> & Choice_Widget::text() const
{
    return _text;
}

void Choice_Widget::draw()
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

    Widget::draw();
}

void Choice_Widget::down(bool in)
{
    if (in == _down)
    {
        return;
    }

    _down = in;

    redraw();
}

void Choice_Widget::select(bool in)
{
    if (in == _select)
    {
        return;
    }

    _select = in;

    redraw();

    if (in)
    {
        select_signal.emit(this);
    }
    else
    {
        select_off_signal.emit(this);
    }
}

const Fl_Widget * Choice_Widget::fltk_widget() const
{
    return _widget;
}

void Choice_Widget::widget_callback(int in)
{
    signal.emit(in);
}

} // djv

