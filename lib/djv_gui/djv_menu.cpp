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

//! \file djv_menu.cpp

#include <djv_menu.h>

#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Menu_Item
//------------------------------------------------------------------------------

const int Menu_Item::item_label_max_size = 100;

Menu_Item::Menu_Item() :
    shortcut(0),
    callback(0),
    data    (0),
    flags   (0)
{}

Menu_Item::Menu_Item(
    const String & label,
    int            shortcut,
    Fl_Callback *  callback,
    void *         data,
    int            flags) :
    label   (label),
    shortcut(shortcut),
    callback(callback),
    data    (data),
    flags   (flags)
{
    // Convert label to a c-string, chopping it if necessary.

    const int label_size = static_cast<int>(label.size());

    const int size = Math::min(label_size, item_label_max_size);
    cstr.size(size + 1);
    Memory::copy(label.c_str(), cstr(), size);
    cstr()[size] = 0;

    if (label_size > item_label_max_size)
    {
        DJV_ASSERT(item_label_max_size >= 3);

        cstr()[size - 1] = '.';
        cstr()[size - 2] = '.';
        cstr()[size - 3] = '.';
    }
}

//------------------------------------------------------------------------------
// Menu::Widget
//------------------------------------------------------------------------------

class Menu::Widget : public Fl_Menu_Bar
{
public:

    Widget() :
        Fl_Menu_Bar(0, 0, 0, 0)
    {
        box(FL_BOX_MENU);
    }

    void draw()
    {
        //! \todo Is this still necessary?

        const Fl_Boxtype tmp = box();
        box(FL_NO_BOX);

        Fl_Menu_Bar::draw();

        box(tmp);
    }
};

//------------------------------------------------------------------------------
// Menu_Item_Group
//------------------------------------------------------------------------------

Menu_Item_Group::Menu_Item_Group()
{}

Menu_Item_Group::Menu_Item_Group(const Menu_Item_Group & in)
{
    set(in);
}

Menu_Item_Group::~Menu_Item_Group()
{
    del();
}

Menu_Item_Group & Menu_Item_Group::operator = (const Menu_Item_Group & in)
{
    if (&in != this)
    {
        set(in);
    }

    return *this;
}

void Menu_Item_Group::set(const Menu_Item_Group & in)
{
    del();

    _list.resize(in._list.size());

    for (size_t i = 0; i < in._list.size(); ++i)
    {
        _list[i] = new Menu_Item(*in._list[i]);
    }
}

int Menu_Item_Group::add(
    const String & label,
    int            shortcut,
    Fl_Callback *  callback,
    void *         data,
    int            flags,
    bool           value)
{
    _list += new Menu_Item(
        label,
        shortcut,
        callback,
        data,
        flags | (value ? Menu_Item::VALUE : 0));

    return static_cast<int>(_list.size()) - 1;
}

namespace
{

String label_list(const String & in, int index)
{
    return String_Format("%%%%. %%").
        arg(index < 9 ? "&" : "").
        arg(index + 1).arg(in);
}

} // namespace

List<int> Menu_Item_Group::add(
    const List<String> & label,
    const List<int> &    shortcut,
    Fl_Callback *        callback,
    void *               data,
    int                  flags,
    int                  value)
{
    List<int> out;

    for (size_t i = 0; i < label.size(); ++i)
    {
        out += add(
            label_list(label[i], static_cast<int>(i)),
            i < shortcut.size() ? shortcut[i] : 0,
            callback,
            data,
            flags,
            value != -1 ? (static_cast<int>(i) == value) : false);
    }

    return out;
}

const List<Menu_Item *> & Menu_Item_Group::get() const
{
    return _list;
}

void Menu_Item_Group::end()
{
    _list += new Menu_Item;
}

void Menu_Item_Group::del()
{
    for (size_t i = 0; i < _list.size(); ++i)
    {
        delete _list[i];
    }

    _list.clear();
}

void Menu_Item_Group::fl(Memory_Buffer<Fl_Menu_Item> * out) const
{
    const size_t size = _list.size();
    
    out->size(size + 1);
    out->zero();

    Fl_Menu_Item * p = (*out)();

    for (size_t i = 0; i < size; ++i)
    {
        p[i].text = _list[i]->cstr();
        p[i].shortcut_ = _list[i]->shortcut;
        p[i].callback_ = _list[i]->callback;
        p[i].user_data_ = _list[i]->data;
        p[i].flags =
            (_list[i]->flags & Menu_Item::INACTIVE ? FL_MENU_INACTIVE : 0) |
            (_list[i]->flags & Menu_Item::TOGGLE   ? FL_MENU_TOGGLE   : 0) |
            (_list[i]->flags & Menu_Item::RADIO    ? FL_MENU_RADIO    : 0) |
            (_list[i]->flags & Menu_Item::VALUE    ? FL_MENU_VALUE    : 0) |
            (_list[i]->flags & Menu_Item::SUB_MENU ? FL_SUBMENU       : 0) |
            (_list[i]->flags & Menu_Item::DIVIDER  ? FL_MENU_DIVIDER  : 0);
        //p[i].labelfont_ = (_list[i]->flags & Menu_Item::SUB_MENU) ?
        //  style::global()->font_bold() : 0;
    }
}

//------------------------------------------------------------------------------
// Menu
//------------------------------------------------------------------------------

Menu::Menu() :
    _test_shortcut(false)
{
    Frame::add(_widget = new Widget);

    style(STYLE_THIN);

    size(STRETCH, FIXED);
}

void Menu::set(const Menu_Item_Group & in)
{
    _group = in;

    Memory_Buffer<Fl_Menu_Item> tmp;
    _group.fl(&tmp);
    _widget->clear();
    _widget->copy(tmp());
}

const Menu_Item_Group & Menu::get() const
{
    return _group;
}

int Menu::item() const
{
    return _widget->value();
}

bool Menu::value() const
{
    //DJV_DEBUG("Menu::value");

    //return _widget->mvalue()->value();

    const int item = _widget->value();
    bool value = (_widget->menu()[item].value() & FL_MENU_VALUE) != 0;

    if (_test_shortcut)
    {
        value = ! value;
    }

    //DJV_DEBUG_PRINT("item = " << item);
    //DJV_DEBUG_PRINT("value = " << value);

    return value;
}

int Menu::test_shortcut()
{
    //DJV_DEBUG("Menu::test_shortcut");

    const List<Menu_Item *> & list = _group.get();

    for (size_t i = 0; i < list.size(); ++i)
        if (Fl::test_shortcut(list[i]->shortcut))
        {
            //DJV_DEBUG_PRINT("item = " << i);

            _widget->value(static_cast<int>(i));

            const Fl_Menu_Item & item = _widget->menu()[i];

            if (item.callback())
            {
                _test_shortcut = true;
                item.do_callback(_widget);
                _test_shortcut = false;
            }

            return 1;
        }

    return 0;
}

void Menu::dirty()
{
    Frame::dirty();

    _widget->textfont(text_font());
    _widget->textsize(Style::global()->font_size());
    _widget->textcolor(text_color());

    fl_font(text_font(), Style::global()->font_size());

    size_hint(
        V2i(0, fl_height()) +
        Style::global()->margin_widget() * 2);
}

void Menu::geom(const Box2i & in)
{
    Frame::geom(in);

    _widget->resize(in.x, in.y, in.w, in.h);
}

const Box2i & Menu::geom() const
{
    return Frame::geom();
}

} // djv
