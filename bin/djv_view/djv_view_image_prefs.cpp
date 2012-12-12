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

//! \file djv_view_image_prefs.cpp

#include <djv_view_image_prefs.h>

#include <djv_check_button.h>
#include <djv_check_button_group.h>
#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_prefs.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

#include <djv_image.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Image_Prefs
//------------------------------------------------------------------------------

Image_Prefs::Image_Prefs() :
    mirror_signal(this),
    scale_signal(this),
    rotate_signal(this),
    color_profile_signal(this),
    display_profile_signal(this),
    display_profile_value_signal(this),
    display_profiles_signal(this),
    channel_signal(this),
    _frame_store_file_reload(false),
    _scale(Image::SCALE(0)),
    _rotate(Image::ROTATE(0)),
    _color_profile(true),
    _display_profile(-1),
    _channel(Gl_Image_Options::CHANNEL(0))
{
    Prefs prefs(Prefs::prefs(), "image");

    Prefs::get_(&prefs, "frame_store_file_reload",
                &_frame_store_file_reload);
    Prefs::get_(&prefs, "mirror", &_mirror);
    Prefs::get_(&prefs, "scale", &_scale);
    Prefs::get_(&prefs, "rotate", &_rotate);
    Prefs::get_(&prefs, "color_profile", &_color_profile);

    Prefs display_profile_prefs(prefs, "display_profile");
    Prefs::get_(&display_profile_prefs, "value", &_display_profile);
    int display_profiles_size = 0;
    Prefs::get_(&display_profile_prefs, "size", &display_profiles_size);

    for (int i = 0; i < display_profiles_size; ++i)
    {
        Display_Profile value;
        Prefs::get_(&display_profile_prefs, String_Format("%%").arg(i),
            &value);
        Image::lut_load(value.lut_file, value.lut);
        _display_profiles += value;
    }

    Prefs::get_(&prefs, "channel", &_channel);
}

Image_Prefs::~Image_Prefs()
{
    Prefs prefs(Prefs::prefs(), "image");

    Prefs::set_(&prefs, "frame_store_file_reload", _frame_store_file_reload);
    Prefs::set_(&prefs, "mirror", _mirror);
    Prefs::set_(&prefs, "scale", _scale);
    Prefs::set_(&prefs, "rotate", _rotate);
    Prefs::set_(&prefs, "color_profile", _color_profile);

    Prefs display_profile_prefs(prefs, "display_profile");
    Prefs::set_(&display_profile_prefs, "value", _display_profile);
    Prefs::set_(&display_profile_prefs, "size",
        static_cast<int>(_display_profiles.size()));

    for (size_t i = 0; i < _display_profiles.size(); ++i)
        Prefs::set_(
            &display_profile_prefs,
            String_Format("%%").arg(static_cast<int>(i)),
            _display_profiles[i]);

    Prefs::set_(&prefs, "channel", _channel);
}

void Image_Prefs::frame_store_file_reload(bool in)
{
    _frame_store_file_reload = in;
}

bool Image_Prefs::frame_store_file_reload() const
{
    return _frame_store_file_reload;
}

void Image_Prefs::mirror(const V2b & in)
{
    if (in == _mirror)
    {
        return;
    }

    _mirror = in;

    mirror_signal.emit(_mirror);
}

const V2b & Image_Prefs::mirror() const
{
    return _mirror;
}

void Image_Prefs::scale(Image::SCALE in)
{
    if (in == _scale)
    {
        return;
    }

    _scale = in;

    scale_signal.emit(_scale);
}

Image::SCALE Image_Prefs::scale() const
{
    return _scale;
}

void Image_Prefs::rotate(Image::ROTATE in)
{
    if (in == _rotate)
    {
        return;
    }

    _rotate = in;

    rotate_signal.emit(_rotate);
}

Image::ROTATE Image_Prefs::rotate() const
{
    return _rotate;
}

void Image_Prefs::color_profile(bool in)
{
    if (in == _color_profile)
    {
        return;
    }

    _color_profile = in;

    color_profile_signal.emit(_color_profile);
}

bool Image_Prefs::color_profile() const
{
    return _color_profile;
}

void Image_Prefs::display_profile(int in)
{
    if (in == _display_profile)
    {
        return;
    }

    _display_profile = in;

    Display_Profile tmp;

    display_profile_signal.emit(_display_profile);
    display_profile_value_signal.emit(
        (_display_profile != -1) ?
        _display_profiles[_display_profile] :
        tmp);
}

int Image_Prefs::display_profile() const
{
    return _display_profile;
}

Display_Profile Image_Prefs::display_profile_value() const
{
    Display_Profile tmp;

    return
        _display_profile != -1 ?
        _display_profiles[_display_profile] :
        tmp;
}

void Image_Prefs::display_profiles(const List<Display_Profile> & in)
{
    _display_profiles = in;
    _display_profile = Math::min(
        _display_profile,
        static_cast<int>(_display_profiles.size()) - 1);

    display_profiles_signal.emit(true);
}

const List<Display_Profile> & Image_Prefs::display_profiles() const
{
    return _display_profiles;
}

List<String> Image_Prefs::display_profile_names() const
{
    List<String> out;

    for (size_t i = 0; i < _display_profiles.size(); ++i)
    {
        out += _display_profiles[i].name;
    }

    return out;
}

void Image_Prefs::channel(Gl_Image_Options::CHANNEL in)
{
    if (in == _channel)
    {
        return;
    }

    _channel = in;

    channel_signal.emit(_channel);
}

Gl_Image_Options::CHANNEL Image_Prefs::channel() const
{
    return _channel;
}

//------------------------------------------------------------------------------
// Image_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_group = "General",
    label_frame_store_file_reload =
        "Automatically store the current frame when files are reloaded",
    label_xform_group = "Transforms",
    label_xform_mirror = "Mirror:",
    label_xform_mirror_h = "Horizontal",
    label_xform_mirror_v = "Vertical",
    label_xform_scale = "Scale:",
    label_xform_rotate = "Rotate:",
    label_color_profile_group = "Color Profile",
    label_color_profile = "Enable color profile",
    label_display_profile_group = "Display Profile",
    label_display_profile_none = "None",
    label_channel_group = "Channels";

} // namespace

Image_Prefs_Widget::Image_Prefs_Widget() :
    _display_profile_widget(0)
{
    // Create widgets.

    Group_Box * general_group = new Group_Box(label_group);

    Check_Button * frame_store_file_reload =
        new Check_Button(label_frame_store_file_reload);

    // Create transform widgets.

    Group_Box * transform_group = new Group_Box(label_xform_group);

    Check_Button_Group * mirror = new Check_Button_Group(
        label_xform_mirror,
        List<String>() << label_xform_mirror_h << label_xform_mirror_v);

    Radio_Button_Group * scale =
        new Radio_Button_Group(label_xform_scale, Image::label_scale());

    Radio_Button_Group * rotate =
        new Radio_Button_Group(label_xform_rotate, Image::label_rotate());

    // Create color profile widgets.

    Group_Box * color_profile_group =
        new Group_Box(label_color_profile_group);

    Check_Button * color_profile = new Check_Button(label_color_profile);

    // Create display profile widgets.

    Group_Box * display_profile_group =
        new Group_Box(label_display_profile_group);

    _display_profile_widget = new Radio_Button_Group(List<String>());

    // Create image channel widgets.

    Group_Box * channel_group = new Group_Box(label_channel_group);

    Radio_Button_Group * channel =
        new Radio_Button_Group(Gl_Image_Options::label_channel());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(general_group);
    general_group->layout()->add(frame_store_file_reload);

    layout->add(transform_group);
    Horizontal_Layout * layout_h =
        new Horizontal_Layout(transform_group->layout());
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(mirror);
    layout_h->add(scale);
    layout_h->add(rotate);
    layout_h->add_stretch();

    layout->add(color_profile_group);
    color_profile_group->layout()->add(color_profile);

    layout->add(display_profile_group);
    display_profile_group->layout()->add(_display_profile_widget);

    layout->add(channel_group);
    channel_group->layout()->add(channel);

    layout->add_stretch();

    // Initialize.

    frame_store_file_reload->set(
        Image_Prefs::global()->frame_store_file_reload());
    mirror->set(List<bool>() << Image_Prefs::global()->mirror().x <<
        Image_Prefs::global()->mirror().y);
    scale->set(Image_Prefs::global()->scale());
    rotate->set(Image_Prefs::global()->rotate());
    color_profile->set(Image_Prefs::global()->color_profile());
    channel->set(Image_Prefs::global()->channel());

    widget_update();

    // Callbacks.

    frame_store_file_reload->signal.set(this, frame_store_file_reload_callback);

    mirror->signal.set(this, mirror_callback);

    scale->signal.set(this, scale_callback);

    rotate->signal.set(this, rotate_callback);

    color_profile->signal.set(this, color_profile_callback);

    _display_profile_widget->signal.set(this, display_profile_callback);

    Image_Prefs::global()->display_profiles_signal.set(
        this, display_profiles_callback);

    channel->signal.set(this, channel_callback);
}

Image_Prefs_Widget::~Image_Prefs_Widget()
{}

void Image_Prefs_Widget::frame_store_file_reload_callback(bool in)
{
    Image_Prefs::global()->frame_store_file_reload(in);
}

void Image_Prefs_Widget::mirror_callback(const List<bool> & in)
{
    Image_Prefs::global()->mirror(V2b(in[0], in[1]));
}

void Image_Prefs_Widget::scale_callback(int in)
{
    Image_Prefs::global()->scale(static_cast<Image::SCALE>(in));
}

void Image_Prefs_Widget::rotate_callback(int in)
{
    Image_Prefs::global()->rotate(static_cast<Image::ROTATE>(in));
}

void Image_Prefs_Widget::color_profile_callback(bool in)
{
    Image_Prefs::global()->color_profile(in);
}

void Image_Prefs_Widget::display_profile_callback(int in)
{
    Image_Prefs::global()->display_profile(in - 1);
}

void Image_Prefs_Widget::display_profiles_callback(bool)
{
    widget_update();
}

void Image_Prefs_Widget::channel_callback(int in)
{
    Image_Prefs::global()->channel(static_cast<Gl_Image_Options::CHANNEL>(in));
}

void Image_Prefs_Widget::widget_update()
{
    callbacks(false);

    _display_profile_widget->label(List<String>() <<
        label_display_profile_none <<
        Image_Prefs::global()->display_profile_names());

    _display_profile_widget->set(Image_Prefs::global()->display_profile() + 1);

    callbacks(true);
}

} // djv_view

