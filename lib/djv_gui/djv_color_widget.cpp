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

//! \file djv_color_widget.cpp

#include <djv_color_widget.h>

#include <djv_choice_widget.h>
#include <djv_float_edit_slider.h>
#include <djv_frame_group.h>
#include <djv_int_edit_slider.h>
#include <djv_row_layout.h>

namespace djv
{

//------------------------------------------------------------------------------
// Color_Widget
//------------------------------------------------------------------------------

namespace
{

const String
tooltip_red = "Red channel",
tooltip_green = "Green channel",
tooltip_blue = "Blue channel",
tooltip_alpha = "Alpha channel",
tooltip_luminance = "Luminance channel",
tooltip_pixel_format = "Pixel format",
tooltip_pixel_type = "Pixel type";

} // namespace

Color_Widget::Color_Widget() :
    signal        (this),
    _value        (Pixel::RGB_U8),
    _float_min    (0.0),
    _float_max    (1.0),
    _format_widget(0),
    _type_widget  (0),
    _bottom_layout(0)
{
    debug_path("Color_Widget");

    //DJV_DEBUG("Color_Widget::Color_Widget");

    // Create widgets.

    for (int i = 0; i < Pixel::channels_max; ++i)
    {
        _int_widget[i] = new Int_Edit_Slider;
        _float_widget[i] = new Float_Edit_Slider;
    }

    _format_widget = new Choice_Widget(Pixel::label_format());
    _format_widget->tooltip(tooltip_pixel_format);

    _type_widget = new Choice_Widget(Pixel::label_type());
    _type_widget->tooltip(tooltip_pixel_type);

    // Layout.

    size(STRETCH, FIXED);

    List<Layout_Item *> group;

    for (int i = 0; i < Pixel::channels_max; ++i)
    {
        group += _int_widget[i];
        group += _float_widget[i];
    }

    create_group(group);

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(0);

    for (int i = 0; i < Pixel::channels_max; ++i)
    {
        layout->add(_int_widget[i]);
        layout->add(_float_widget[i]);
    }

    _bottom_layout = new Horizontal_Layout(layout);
    _bottom_layout->margin(0);
    _bottom_layout->add_stretch();
    _bottom_layout->add(new Horizontal_Frame_Group(List<Frame *>() <<
        _format_widget <<
        _type_widget));

    // Initialize.

    widget_update();
    value_update();

    // Callbacks.

    for (int i = 0; i < Pixel::channels_max; ++i)
    {
        _int_widget[i]->signal.set(this, int_callback);
        _float_widget[i]->signal.set(this, float_callback);
    }

    _format_widget->signal.set(this, format_callback);
    _type_widget->signal.set(this, type_callback);
}

Color_Widget::~Color_Widget()
{}

void Color_Widget::set(const Color & in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Color_Widget::set");
    //DJV_DEBUG_PRINT("in = " << in);

    const Color prev = _value;
    _value = in;

    if (_value.pixel() != prev.pixel())
    {
        widget_update();
    }

    value_update();

    signal.emit(_value);
}

const Color & Color_Widget::get() const
{
    return _value;
}

void Color_Widget::float_range(double min, double max)
{
    if (min == _float_min && max == _float_max)
    {
        return;
    }

    _float_min = min;
    _float_max = max;

    set(_value);

    widget_update();
}

double Color_Widget::float_min() const
{
    return _float_min;
}

double Color_Widget::float_max() const
{
    return _float_max;
}

Horizontal_Layout * Color_Widget::bottom_layout()
{
    return _bottom_layout;
}

void Color_Widget::int_callback(int)
{
    for (int i = 0; i < Pixel::channels(_value.pixel()); ++i)
        switch (Pixel::type(_value.pixel()))
        {
            case Pixel::U8:
                _value.set_u8 (_int_widget[i]->get(), i);
                break;

            case Pixel::U10:
                _value.set_u10(_int_widget[i]->get(), i);
                break;

            case Pixel::U16:
                _value.set_u16(_int_widget[i]->get(), i);
                break;

            case Pixel::F16:
                _value.set_f16(
                    static_cast<Pixel::F16_T>(_float_widget[i]->get()),
                    i);
                break;

            case Pixel::F32:
                _value.set_f32(
                    static_cast<Pixel::F32_T>(_float_widget[i]->get()),
                    i);
                break;

            default:
                break;
        }

    value_update();

    signal.emit(_value);
}

void Color_Widget::float_callback(double)
{
    int_callback(0);
}

void Color_Widget::format_callback(int in)
{
    Color tmp(Pixel::pixel(
                  static_cast<Pixel::FORMAT>(in),
                  Pixel::type(_value.pixel())));

    Color::convert(_value, tmp);

    set(tmp);

    widget_update();
}

void Color_Widget::type_callback(int in)
{
    Color tmp(Pixel::pixel(
                  Pixel::format(_value.pixel()),
                  static_cast<Pixel::TYPE>(in)));

    Color::convert(_value, tmp);

    set(tmp);

    widget_update();
}

void Color_Widget::widget_update()
{
    //DJV_DEBUG("Color_Widget::widget_update");
    //DJV_DEBUG_PRINT("value = " << _value);

    callbacks(false);

    const int channels = Pixel::channels(_value.pixel());

    const String tooltip0 [] =
    {
        tooltip_luminance,
        tooltip_alpha
    };

    const String tooltip1 [] =
    {
        tooltip_red,
        tooltip_green,
        tooltip_blue,
        tooltip_alpha
    };

    const String * tooltip = 0;

    switch (Pixel::format(_value.pixel()))
    {
        case Pixel::L:
        case Pixel::LA:
            tooltip = tooltip0;
            break;

        case Pixel::RGB:
        case Pixel::RGBA:
            tooltip = tooltip1;
            break;

        default:
            break;
    }

    for (int i = 0; i < channels; ++i)
    {
        _int_widget[i]->range(0, Pixel::max(_value.pixel()));
        _int_widget[i]->tooltip(tooltip[i]);
        _float_widget[i]->range(_float_min, _float_max);
        _float_widget[i]->tooltip(tooltip[i]);
    }

    for (int i = 0; i < Pixel::channels_max; ++i)
        switch (Pixel::type(_value.pixel()))
        {
            case Pixel::U8:
            case Pixel::U10:
            case Pixel::U16:
                if (i < channels)
                {
                    _int_widget[i]->show();
                }
                else
                {
                    _int_widget[i]->hide();
                }

                _float_widget[i]->hide();
                break;

            case Pixel::F16:
            case Pixel::F32:
                _int_widget[i]->hide();

                if (i < channels)
                {
                    _float_widget[i]->show();
                }
                else
                {
                    _float_widget[i]->hide();
                }

                break;

            default:
                break;
        }

    _format_widget->set(Pixel::format(_value.pixel()));
    _type_widget->set(Pixel::type(_value.pixel()));

    update();

    callbacks(true);
}

void Color_Widget::value_update()
{
    //DJV_DEBUG("Color_Widget::value_update");

    callbacks(false);

    for (int i = 0; i < Pixel::channels(_value.pixel()); ++i)
        switch (Pixel::type(_value.pixel()))
        {
            case Pixel::U8:
                _int_widget  [i]->set(_value.get_u8 (i));
                break;

            case Pixel::U10:
                _int_widget  [i]->set(_value.get_u10(i));
                break;

            case Pixel::U16:
                _int_widget  [i]->set(_value.get_u16(i));
                break;

            case Pixel::F16:
                _float_widget[i]->set(_value.get_f16(i));
                break;

            case Pixel::F32:
                _float_widget[i]->set(_value.get_f32(i));
                break;

            default:
                break;
        }

    callbacks(true);
}

} // djv
