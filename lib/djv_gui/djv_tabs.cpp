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

//! \file djv_tabs.cpp

#include <djv_tabs.h>

#include <djv_font.h>
#include <djv_row_layout.h>
#include <djv_stack_layout.h>
#include <djv_style.h>
#include <djv_scroll.h>
#include <djv_widget.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// _Tab
//------------------------------------------------------------------------------

namespace
{

class _Tab : public Widget
{
public:

    _Tab(Abstract_Widget *);

    void name(const String &);

    const String & name() const
    {
        return _name;
    }

private:

    String _name;
    Memory_Buffer<char> _name_cstr;
};

_Tab::_Tab(Abstract_Widget * item)
{
    debug_path("tabs::_Tab");

    text_font(Style::global()->font_bold());

    Scroll * scroll_widget = new Scroll;
    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(scroll_widget);
    scroll_widget->layout()->margin(0);

    if (item)
    {
        scroll_widget->layout()->add(item);
    }
}

void _Tab::name(const String & in)
{
    if (in == _name)
    {
        return;
    }

    _name = in;

    const String::size_type size = _name.size() + 1;
    _name_cstr.size(size);
    String_Util::cstring(_name, _name_cstr(), size);
    Fl_Group::label(_name_cstr());
}

}

//------------------------------------------------------------------------------
// Tabs
//------------------------------------------------------------------------------

Tabs::Tabs() :
    Fl_Tabs(0, 0, 0, 0),
    signal(this)
{
    debug_path("Tabs");

    Fl_Tabs::end();

    box(FL_BOX_THIN_UP);

    Stack_Layout * layout = new Stack_Layout(this);
    layout->margin(0);

    callback(widget_callback, this);
}

Tabs::~Tabs()
{}

void Tabs::set(int in)
{
    DJV_ASSERT(in < static_cast<int>(_list.size()));

    value(dynamic_cast<Fl_Widget *>(_list[in]));
}

int Tabs::get() const
{
    //! \todo FLTK forgot their const.

    Tabs * widget = const_cast<Tabs *>(this);

    for (size_t i = 0; i < _list.size(); ++i)
        if (widget->value() == dynamic_cast<Fl_Widget *>(_list[i]))
        {
            return static_cast<int>(i);
        }

    return -1;
}

void Tabs::tab_add(const String & name, Abstract_Widget * in)
{
    tab_insert(name, in, static_cast<int>(_list.size()));
}

void Tabs::tab_insert(
    const String &    name,
    Abstract_Widget * in,
    int               index)
{
    //DJV_DEBUG("Tabs::tab_insert");
    //DJV_DEBUG_PRINT("name = " << name);
    //DJV_DEBUG_PRINT("index = " << index);

    _Tab * widget = new _Tab(in);
    widget->name(name);

    List<Abstract_Widget *>::iterator i = _list.begin();
    
    int j = 0;

    for (; j < index && j < static_cast<int>(_list.size()); ++j, ++i)
        ;

    _list.insert(i, widget);

    layout()->insert(widget, j);

    //dirty();
    update();
    redraw();

    //tab(index);
}

void Tabs::tab_name(const String & name, int tab)
{
    DJV_ASSERT(tab < static_cast<int>(_list.size()));

    //DJV_DEBUG("Tabs::tab_name");
    //DJV_DEBUG_PRINT("name = " << name);
    //DJV_DEBUG_PRINT("tab = " << tab);

    static_cast<_Tab *>(_list[tab])->name(name);

    //dirty();
    update();
    redraw();
}

void Tabs::tab_del(int in)
{
    DJV_ASSERT(in < static_cast<int>(_list.size()));

    //DJV_DEBUG("Tabs::tab_del");
    //DJV_DEBUG_PRINT("in = " << in);

    List<Abstract_Widget *>::iterator i = _list.begin();

    for (int j = 0; j < in; ++j, ++i) ;

    _list.erase(i);

    layout()->del(in);

    //dirty();
    update();
    redraw();
}

void Tabs::tab_del()
{
    while (_list.size())
    {
        tab_del(0);
    }
}

int Tabs::tab_size() const
{
    return static_cast<int>(_list.size());
}

int Tabs::handle(int in)
{
    switch (Abstract_Widget::handle(in))
    {
        case -1:
            return 0;

        case 1:
            return 1;
    }

    return Fl_Tabs::handle(in);
}

void Tabs::show()
{
    Fl_Tabs::show();

    Abstract_Widget::show();
}

void Tabs::draw()
{
    Fl_Tabs::draw();

    const Box2i & geom = this->geom();

    //! \todo Erase extra tab lines.

    fl_color(FL_BACKGROUND_COLOR);
    fl_line(geom.x, geom.y + _size_tab.y, geom.x, geom.y + geom.h - 1);
    fl_line(geom.x + geom.w - 1, geom.y + _size_tab.y, geom.x + geom.w - 1,
            geom.y + geom.h - 1);
    fl_line(geom.x, geom.y + geom.h - 1, geom.x + geom.w - 1,
            geom.y + geom.h - 1);

    Abstract_Widget::draw();
}

void Tabs::dirty()
{
    //DJV_DEBUG("Tabs::dirty");

    Abstract_Widget::dirty();

    fl_font(text_font(), Style::global()->font_size());

    int label_hint = 0;

    for (size_t i = 0; i < _list.size(); ++i)
    {
        label_hint = Math::max(
            Font::string_width(static_cast<_Tab *>(_list[i])->name()),
            label_hint);
    }
    
    _size_tab =
        V2i(0, fl_height()) +
        Style::global()->margin_widget() * 2 +
        V2i(1 * 2, 1);

    //DJV_DEBUG_PRINT("tab = " << _size_tab);

    size_hint(_size_tab + layout()->size_hint() + 1 * 2);
}

void Tabs::geom(const Box2i & in)
{
    //DJV_DEBUG("Tabs::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    Fl_Widget::resize(in.x, in.y, in.w, in.h);

    Layout_Item::geom(in);

    layout()->geom(
        Box2i(
            in.x + 1,
            in.y + _size_tab.y + 1,
            in.w - 1 * 2,
            in.h - _size_tab.y - 1 * 2));
}

void Tabs::resize(int x, int y, int w, int h)
{
    geom(Box2i(x, y, w, h));
}

const Box2i & Tabs::geom() const
{
    return Abstract_Widget::geom();
}

int Tabs::visible() const
{
    return Fl_Tabs::visible();
}

void Tabs::widget_callback()
{
    signal.emit(get());
}

} // djv
