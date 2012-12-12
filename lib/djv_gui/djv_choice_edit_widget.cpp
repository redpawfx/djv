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

//! \file djv_choice_edit_widget.cpp

#include <djv_choice_edit_widget.h>

#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_style.h>
#include <djv_text_edit.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Choice_Edit_Widget
//------------------------------------------------------------------------------

Choice_Edit_Widget::Choice_Edit_Widget(const List<String> & list) :
    signal (this),
    _list  (list),
    _widget(0),
    _menu  (0)
{
    debug_path("Choice_Edit_Widget");

    // Create widgets.

    _widget = new Text_Edit;
    _widget->style(STYLE_EMPTY);

    _menu = new Popup_Menu;

    // Layout.

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->spacing(2);
    layout->margin(0);
    layout->add(_widget);
    layout->stretch(_widget);
    layout->add(_menu);

    // Initialize.

    style(STYLE_BORDER);

    list_update();

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    _menu->signal.set(this, menu_callback);
}

Choice_Edit_Widget::~Choice_Edit_Widget()
{}

void Choice_Edit_Widget::set(const String & in)
{
    _widget->set(in);
}

String Choice_Edit_Widget::get() const
{
    return _widget->get();
}

void Choice_Edit_Widget::list(const List<String> & in)
{
    if (in == _list)
    {
        return;
    }

    _list = in;

    list_update();
}

const List<String> & Choice_Edit_Widget::list() const
{
    return _list;
}

void Choice_Edit_Widget::size_string(const String & in)
{
    _widget->size_string(in);
}

void Choice_Edit_Widget::dirty()
{
    Frame::dirty();
}

void Choice_Edit_Widget::widget_callback(const String & in)
{
    signal.emit(in);
}

void Choice_Edit_Widget::menu_callback(int in)
{
    set(_list[in]);
}

void Choice_Edit_Widget::list_update()
{
    _menu->set(_list);

    // Set to width of largest menu item.

    if (_list.size() && ! _widget->size_string().size())
    {
        fl_font(text_font(), Style::global()->font_size());

        size_t i = 0;

        for (size_t j = 1; j < _list.size(); ++j)
            if (fl_width(_list[j].c_str()) > fl_width(_list[i].c_str()))
            {
                i = j;
            }

        _widget->size_string(_list[i] + " ");

        update();
    }
}

} // djv

