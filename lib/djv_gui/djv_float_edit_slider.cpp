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

//! \file djv_float_edit_slider.cpp

#include <djv_float_edit_slider.h>

#include <djv_float_slider.h>
#include <djv_inc_button.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_text_edit.h>
#include <djv_tool_button.h>

#include <FL/Fl.H>

namespace djv
{

//------------------------------------------------------------------------------
// Float_Edit_Slider
//------------------------------------------------------------------------------

namespace
{

const String format_float = "000.00";
const String tooltip_reset = "Reset value";

} // namespace

Float_Edit_Slider::Float_Edit_Slider(double min, double max,
                                     const String & label) :
    Abstract_Float_Input(min, max),
    signal         (this),
    _widget        (0),
    _slider        (0),
    _label         (0),
    _default_widget(0),
    _layout        (0)
{
    debug_path("Float_Edit_Slider");

    clamp(false);

    // Create widgets.

    _widget = new Text_Edit;
    _widget->size_string(format_float);

    Inc_Button * inc_widget = new Inc_Button;

    _slider = new Float_Slider;

    if (! label.empty())
    {
        _label = new Label(label);
    }

    _default_widget = new Tool_Button("tool_x");
    _default_widget->tooltip(tooltip_reset);
    _default_widget->deactivate();
    _default_widget->hide();

    // Layout.

    size(STRETCH, FIXED);

    _layout = new Horizontal_Layout(this);
    _layout->margin(0);

    if (_label)
    {
        _layout->add(_label);
    }

    Horizontal_Layout * layout_h = new Horizontal_Layout(_layout);
    layout_h->spacing(0);
    layout_h->margin(0);
    layout_h->add(_widget);
    layout_h->add(inc_widget);
    _layout->add(_slider);
    _layout->stretch(_slider);
    _layout->add(_default_widget);

    // Initialize.

    range_update();
    value_update();

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    inc_widget->inc_signal.set(this, inc_callback);
    inc_widget->dec_signal.set(this, dec_callback);
    _slider->signal.set(this, slider_callback);
    _default_widget->signal.set(this, default_callback);
}

Float_Edit_Slider::~Float_Edit_Slider()
{}

void Float_Edit_Slider::is_default_valid(bool in)
{
    Abstract_Float_Input::is_default_valid(in);

    if (in)
    {
        _default_widget->show();
    }
    else
    {
        _default_widget->hide();
    }

    update();
}

bool Float_Edit_Slider::is_default_valid() const
{
    return Abstract_Float_Input::is_default_valid();
}

void Float_Edit_Slider::range(double min, double max)
{
    Abstract_Float_Input::range(min, max);

    slider_range(min, max);
}

void Float_Edit_Slider::slider_range(double min, double max)
{
    _slider->range(min, max);
}

void Float_Edit_Slider::label(Widget * in)
{
    if (_label)
    {
        _layout->del(_label);
    }

    _label = in;

    if (_label)
    {
        _layout->insert(_label, 0);
    }

    _layout->stretch(_slider);

    update();
}

Widget * Float_Edit_Slider::label() const
{
    return _label;
}

Layout_Item * Float_Edit_Slider::label_align()
{
    return _label;
}

void Float_Edit_Slider::text_font(Fl_Font in)
{
    Widget::text_font(in);

    if (_label)
    {
        _label->text_font(in);
    }
}

Fl_Font Float_Edit_Slider::text_font() const
{
    return Widget::text_font();
}

void Float_Edit_Slider::text_color(Fl_Color in)
{
    Widget::text_color(in);

    if (_label)
    {
        _label->text_color(in);
    }
}

Fl_Color Float_Edit_Slider::text_color() const
{
    return Widget::text_color();
}

int Float_Edit_Slider::handle(int in)
{
    if (Abstract_Float_Input::handle(in, visible_focus()))
    {
        return 1;
    }

    return Widget::handle(in);
}

Layout_Item * Float_Edit_Slider::group_align()
{
    return _widget->group_align();
}

void Float_Edit_Slider::value_callback()
{
    value_update();

    signal.emit(get());
}

void Float_Edit_Slider::range_callback()
{
    range_update();
}

void Float_Edit_Slider::widget_callback(const String & in)
{
    Abstract_Float_Input::set(String_Util::string_to_float(in));

    value_callback();
}

void Float_Edit_Slider::inc_callback(bool)
{
    inc_small_action();
}

void Float_Edit_Slider::dec_callback(bool)
{
    dec_small_action();
}

void Float_Edit_Slider::slider_callback(double in)
{
    Abstract_Float_Input::set(in);
}

void Float_Edit_Slider::default_callback(bool)
{
    Abstract_Float_Input::set(default_value());
}

void Float_Edit_Slider::value_update()
{
    callbacks(false);

    const double value = get();

    _widget->set(String_Util::float_to_string(value));
    _slider->set(value);

    if (is_default_valid() && value != default_value())
    {
        _default_widget->activate();
    }
    else
    {
        _default_widget->deactivate();
    }

    callbacks(true);
}

void Float_Edit_Slider::range_update()
{
    callbacks(false);

    _slider->range(min(), max());
    _slider->inc(inc_small(), inc_large());

    callbacks(true);
}

} // djv

