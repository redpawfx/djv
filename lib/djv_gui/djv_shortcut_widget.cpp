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

//! \file djv_shortcut_widget.cpp

#include <djv_shortcut_widget.h>

#include <djv_row_layout.h>
#include <djv_label.h>
#include <djv_scroll.h>
#include <djv_shortcut.h>
#include <djv_text_edit.h>
#include <djv_tool_button.h>

#include <FL/Fl.H>

namespace djv
{

//------------------------------------------------------------------------------
// Callback_Data
//------------------------------------------------------------------------------

namespace
{

struct Callback_Data
{
    Callback_Data(Widget * widget, int value) :
        widget(widget),
        value(value)
    {}

    Widget * widget;
    int value;
};

} // namespace

//------------------------------------------------------------------------------
// Shortcut_Widget::Edit
//------------------------------------------------------------------------------

class Shortcut_Widget::Edit : public Text_Edit
{
public:

    Edit(int = 0);

    void set(int);

    int get() const
    {
        return _value;
    }

    Signal<const void *> signal;

private:

    DJV_CALLBACK(Edit, widget_callback, const String &);

    void widget_update();

    int _value;
};

Shortcut_Widget::Edit::Edit(int value) :
    signal(this),
    _value(value)
{
    // Initialize.

    style(STYLE_EMPTY);

    widget_update();

    // Callbacks.

    Text_Edit::signal.set(this, widget_callback);
}

void Shortcut_Widget::Edit::set(int value)
{
    if (value == _value)
    {
        return;
    }

    //DJV_DEBUG("Shortcut_Widget::Edit::set");
    //DJV_DEBUG_PRINT("value = " << value);

    _value = value;

    widget_update();

    Callback_Data data(this, _value);
    signal.emit(&data);
}

bool alt(int in)
{
    return
        in == FL_Shift_L ||
        in == FL_Shift_R ||
        in == FL_Alt_L ||
        in == FL_Alt_R ||
        in == FL_Control_L ||
        in == FL_Control_R ||
        in == FL_Meta_L ||
        in == FL_Meta_R;
}

void Shortcut_Widget::Edit::widget_callback(const String & in)
{
    int value = 0;
    String tmp = in;
    Shortcut::serialize(&tmp, &value);
    set(value);

    widget_update();
}

void Shortcut_Widget::Edit::widget_update()
{
    callbacks(false);

    Text_Edit::set(Shortcut::serialize(_value));

    callbacks(true);
}

//------------------------------------------------------------------------------
// Shortcut_Widget::Button
//------------------------------------------------------------------------------

class Shortcut_Widget::Button : public Tool_Button
{
public:

    Button();

    Signal<const void *> signal;

    int handle(int);

private:

    DJV_CALLBACK(Button, widget_callback, bool);
};

const String
tooltip_button = "Click, then type shortcut";

Shortcut_Widget::Button::Button() :
    Tool_Button("tool_add"),
    signal(this)
{
    type(TOGGLE);
    tooltip(tooltip_button);

    Tool_Button::signal.set(this, widget_callback);
}

void Shortcut_Widget::Button::widget_callback(bool in)
{
    take_focus();
}

int Shortcut_Widget::Button::handle(int in)
{
    switch (in)
    {
        case FL_KEYBOARD:
        {
            int key = Fl::event_key();

            if (get() && ! alt(key))
            {
                if (Fl::event_shift())
                {
                    key |= FL_SHIFT;
                }

                if (Fl::event_alt())
                {
                    key |= FL_ALT;
                }

                if (Fl::event_ctrl())
                {
                    key |= FL_CTRL;
                }

                set(false);

                Callback_Data data(this, key);
                signal.emit(&data);

                return 1;
            }
        }
        break;

        case FL_FOCUS:
            if (get())
            {
                return 1;
            }

            break;

        case FL_UNFOCUS:
            set(false);
            return 1;
    }

    return Tool_Button::handle(in);
}

//------------------------------------------------------------------------------
// Shortcut_Widget
//------------------------------------------------------------------------------

Shortcut_Widget::Shortcut_Widget() :
    signal (this),
    _scroll(0),
    _layout(0)
{
    _scroll = new Scroll;

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(0);
    layout->add(_scroll);

    _layout = new Vertical_Layout(_scroll);
    _layout->spacing(1);

    widget_update();
    value_update();
}

Shortcut_Widget::~Shortcut_Widget()
{}

void Shortcut_Widget::set(const List<Shortcut> & in)
{
    if (in == _list)
    {
        return;
    }

    _list = in;

    widget_update();
    value_update();

    signal.emit(_list);
}

const List<Shortcut> & Shortcut_Widget::get() const
{
    return _list;
}

void Shortcut_Widget::widget_callback(const void * in)
{
    const Callback_Data * data = static_cast<const Callback_Data *>(in);

    for (size_t i = 0; i < _list.size(); ++i)
        if (data->widget == _edit[i] || data->widget == _button[i])
        {
            _list[i].value = data->value;

            _edit[i]->set(_list[i].value);

            break;
        }

    value_update();

    signal.emit(_list);
}

void Shortcut_Widget::widget_update()
{
    //DJV_DEBUG("Shortcut_Widget::widget_update");

    callbacks(false);

    _layout->del();
    _edit.clear();
    _button.clear();

    List<Layout_Item *> layout_group;

    for (size_t i = 0; i < _list.size(); ++i)
    {
        Label * label = new Label(_list[i].name);
        Edit * edit = new Edit(_list[i].value);
        Button * button = new Button;

        layout_group += label;
        Horizontal_Layout * layout = new Horizontal_Layout(_layout);
        layout->margin(0);
        layout->add(label);
        layout->add(edit);
        layout->add(button);

        edit->signal.set(this, widget_callback);
        button->signal.set(this, widget_callback);

        _label += label;
        _edit += edit;
        _button += button;
    }

    _layout->add_stretch();
    Layout_Item::create_group(layout_group);

    callbacks(true);
}

void Shortcut_Widget::value_update()
{
    callbacks(false);

    for (size_t i = 0; i < _list.size(); ++i)
    {
        _edit[i]->color(FL_BACKGROUND2_COLOR);
        _edit[i]->text_color(text_color());
    }

    for (size_t i = 0; i < _list.size(); ++i)
        for (size_t j = 0; j < _list.size(); ++j)
            if (
                i != j &&
                _list[i].value &&
                _list[i].value == _list[j].value)
            {
                _edit[i]->color(FL_SELECTION_COLOR);
                _edit[i]->text_color(FL_BLACK);
                break;
            }

    callbacks(true);
}

} // djv

