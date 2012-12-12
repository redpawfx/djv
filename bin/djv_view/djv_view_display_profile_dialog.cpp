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

//! \file djv_view_display_profile_dialog.h

#include <djv_view_display_profile_dialog.h>

#include <djv_view_image.h>

#include <djv_color_widget.h>
#include <djv_file_widget.h>
#include <djv_float_edit_slider.h>
#include <djv_form_widget.h>
#include <djv_group_box.h>
#include <djv_push_button.h>
#include <djv_row_layout.h>

#include <djv_image.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Display_Profile_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_lut = "Lookup Table",
    label_color = "Color",
    label_brightness = "Brightness:",
    label_contrast = "Contrast:",
    label_saturation = "Saturation:",
    label_levels = "Levels",
    label_levels_in = "Input:",
    label_gamma = "Gamma:",
    label_levels_out = "Output:",
    label_soft_clip = "Soft clip:",
    label_close = "Clos&e";

} // namespace

Display_Profile_Dialog::Display_Profile_Dialog() :
    Dialog("Display Profile"),
    signal            (this),
    _view             (0),
    _lut_widget       (0),
    _brightness_widget(0),
    _contrast_widget  (0),
    _saturation_widget(0),
    _gamma_widget     (0),
    _soft_clip_widget (0),
    _close_widget     (0)
{
    // Create LUT widgets.

    Group_Box * lut_group = new Group_Box(label_lut);

    _lut_widget = new File_Edit;

    // Create color widgets.

    Group_Box * color_group = new Group_Box(label_color);

    _brightness_widget = new Float_Edit_Slider;
    _brightness_widget->default_value(_value.color.brightness);
    _brightness_widget->range(0.0, 4.0);

    _contrast_widget = new Float_Edit_Slider;
    _contrast_widget->default_value(_value.color.contrast);
    _contrast_widget->range(0.0, 4.0);

    _saturation_widget = new Float_Edit_Slider;
    _saturation_widget->default_value(_value.color.saturation);
    _saturation_widget->range(0.0, 4.0);

    // Create levels widgets.

    Group_Box * levels_group = new Group_Box(label_levels);

    _levels_in_widget[0] = new Float_Edit_Slider;
    _levels_in_widget[0]->default_value(_value.levels.in_low);
    _levels_in_widget[0]->range(0.0, 1.0);

    _levels_in_widget[1] = new Float_Edit_Slider;
    _levels_in_widget[1]->default_value(_value.levels.in_high);
    _levels_in_widget[1]->range(0.0, 1.0);

    _gamma_widget = new Float_Edit_Slider;
    _gamma_widget->default_value(_value.levels.gamma);
    _gamma_widget->range(0.0, 4.0);

    _levels_out_widget[0] = new Float_Edit_Slider;
    _levels_out_widget[0]->default_value(_value.levels.out_low);
    _levels_out_widget[0]->range(0.0, 1.0);

    _levels_out_widget[1] = new Float_Edit_Slider;
    _levels_out_widget[1]->default_value(_value.levels.out_high);
    _levels_out_widget[1]->range(0.0, 1.0);

    _soft_clip_widget = new Float_Edit_Slider;
    _soft_clip_widget->default_value(_value.soft_clip);
    _soft_clip_widget->range(0.0, 1.0);

    // Create dialog widgets.

    _close_widget = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    layout->add(lut_group);
    lut_group->layout()->add(_lut_widget);

    layout->add(color_group);
    Form_Widget * form_widget = new Form_Widget;
    color_group->layout()->add(form_widget);
    form_widget->add_row(label_brightness, _brightness_widget);
    form_widget->add_row(label_contrast, _contrast_widget);
    form_widget->add_row(label_saturation, _saturation_widget);
    
    layout->add(levels_group);
    form_widget = new Form_Widget;
    levels_group->layout()->add(form_widget);
    form_widget->add_row(label_levels_in, _levels_in_widget[0]);
    form_widget->add_row(String(), _levels_in_widget[1]);
    form_widget->add_row(label_gamma, _gamma_widget);
    form_widget->add_row(label_levels_out, _levels_out_widget[0]);
    form_widget->add_row(String(), _levels_out_widget[1]);
    form_widget->add_row(label_soft_clip, _soft_clip_widget);
    
    layout->add_spacer(-1, true);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    widget_update();

    size(size_hint());

    // Callbacks.

    _lut_widget->signal.set(this, lut_callback);

    _brightness_widget->signal.set(this, brightness_callback);
    _contrast_widget->signal.set(this, contrast_callback);
    _saturation_widget->signal.set(this, saturation_callback);

    _levels_in_widget[0]->signal.set(this, levels_in0_callback);
    _levels_in_widget[1]->signal.set(this, levels_in1_callback);
    _gamma_widget->signal.set(this, gamma_callback);
    _levels_out_widget[0]->signal.set(this, levels_out0_callback);
    _levels_out_widget[1]->signal.set(this, levels_out1_callback);
    _soft_clip_widget->signal.set(this, soft_clip_callback);

    _close_widget->signal.set(this, close_callback);
}

Display_Profile_Dialog::~Display_Profile_Dialog()
{}

void Display_Profile_Dialog::set(
    const View_Widget * in,
    const Display_Profile & value)
{
    _view = in;
    _value = value;

    widget_update();
}

void Display_Profile_Dialog::pick(
    const View_Widget * in,
    const Display_Profile & value)
{
    if (in == _view || ! shown() || ! visible())
    {
        return;
    }

    set(in, value);
}

void Display_Profile_Dialog::update(
    const View_Widget * in,
    const Display_Profile & value)
{
    if (in != _view)
    {
        return;
    }

    _value = value;

    widget_update();
}

void Display_Profile_Dialog::del(const View_Widget * in)
{
    if (in != _view)
    {
        return;
    }

    _view = 0;
}

void Display_Profile_Dialog::show()
{
    _close_widget->take_focus();

    Dialog::show();
}

Display_Profile_Dialog * Display_Profile_Dialog::global()
{
    static Display_Profile_Dialog * global = 0;

    if (! global)
    {
        global = new Display_Profile_Dialog;
    }

    return global;
}

void Display_Profile_Dialog::widget_callback(const Display_Profile & in)
{
    _value = in;

    widget_update();
}

void Display_Profile_Dialog::lut_callback(const File & in)
{
    _value.lut_file = in;
    
    Image::lut_load(_value.lut_file, _value.lut);

    signal.emit(_value);
}

void Display_Profile_Dialog::brightness_callback(double in)
{
    _value.color.brightness = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::contrast_callback(double in)
{
    _value.color.contrast = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::saturation_callback(double in)
{
    _value.color.saturation = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::levels_in0_callback(double in)
{
    _value.levels.in_low = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::levels_in1_callback(double in)
{
    _value.levels.in_high = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::gamma_callback(double in)
{
    _value.levels.gamma = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::levels_out0_callback(double in)
{
    _value.levels.out_low = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::levels_out1_callback(double in)
{
    _value.levels.out_high = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::soft_clip_callback(double in)
{
    _value.soft_clip = in;

    signal.emit(_value);
}

void Display_Profile_Dialog::close_callback(bool)
{
    hide();
}

void Display_Profile_Dialog::widget_update()
{
    callbacks(false);

    _lut_widget->set(_value.lut_file);

    _brightness_widget->set(_value.color.brightness);
    _contrast_widget->set(_value.color.contrast);
    _saturation_widget->set(_value.color.saturation);

    _levels_in_widget[0]->set(_value.levels.in_low);
    _levels_in_widget[1]->set(_value.levels.in_high);
    _gamma_widget->set(_value.levels.gamma);
    _levels_out_widget[0]->set(_value.levels.out_low);
    _levels_out_widget[1]->set(_value.levels.out_high);
    _soft_clip_widget->set(_value.soft_clip);

    callbacks(true);
}

} // djv_view

