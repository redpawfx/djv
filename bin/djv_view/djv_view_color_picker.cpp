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

//! \file djv_view_color_picker.cpp

#include <djv_view_color_picker.h>

#include <djv_view_display_profile.h>
#include <djv_view_view_widget.h>

#include <djv_application.h>
#include <djv_check_button.h>
#include <djv_choice_widget.h>
#include <djv_color_swatch.h>
#include <djv_color_widget.h>
#include <djv_frame_group.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_prefs.h>
#include <djv_push_button.h>
#include <djv_tool_button.h>

#include <djv_gl_offscreen_buffer.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Color_Picker
//------------------------------------------------------------------------------

const List<String> & Color_Picker::label_size()
{
    static const List<String> data = List<String>() <<
        "1x1" <<
        "3x3" <<
        "5x5" <<
        "7x7" <<
        "9x9" <<
        "51x51" <<
        "101x101";

    DJV_ASSERT(data.size() == _SIZE_SIZE);

    return data;
}

int Color_Picker::size(SIZE in)
{
    static const int data [] =
    {
        1,
        3,
        5,
        7,
        9,
        51,
        101
    };

    DJV_ASSERT(sizeof(data) / sizeof(data[0]) == _SIZE_SIZE);

    return data[in];
}

namespace
{

const String
    label_close = "Clos&e",
    tooltip_size = "Sample size",
    tooltip_color_profile = "Use color profile",
    tooltip_display_profile = "Use display profile",
    tooltip_lock = "Lock pixel";

} // namespace

Color_Picker::Color_Picker() :
    Dialog("Color Picker"),
    _view                  (0),
    _value                 (Pixel::RGBA_F32),
    _size                  (SIZE_3),
    _color_profile         (true),
    _display_profile       (true),
    _lock                  (false),
    _buffer                (0),
    _widget                (0),
    _swatch                (0),
    _size_widget           (0),
    _color_profile_widget  (0),
    _display_profile_widget(0),
    _lock_widget           (0),
    _close_widget          (0)
{
    //DJV_DEBUG("Color_Picker::Color_Picker");

    // Create widgets.

    _widget = new Color_Widget;

    _swatch = new Color_Swatch;
    _swatch->size(STRETCH, STRETCH);

    _size_widget = new Choice_Widget(label_size());
    _size_widget->tooltip(tooltip_size);

    _color_profile_widget = new Tool_Button("grey_ramp");
    _color_profile_widget->type(Tool_Button::TOGGLE);
    _color_profile_widget->tooltip(tooltip_color_profile);

    _display_profile_widget = new Tool_Button("grey_ramp");
    _display_profile_widget->type(Tool_Button::TOGGLE);
    _display_profile_widget->tooltip(tooltip_display_profile);

    _lock_widget = new Tool_Button("tool_lock");
    _lock_widget->type(Tool_Button::TOGGLE);
    _lock_widget->tooltip(tooltip_lock);

    _close_widget = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(_swatch);
    layout_h->add(_widget);
    layout_h->stretch(_widget);

    layout->add_spacer(-1, true);

    layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(_size_widget);
    layout_h->add(new Horizontal_Frame_Group(List<Frame *>() <<
        _color_profile_widget <<
        _display_profile_widget));
    layout_h->add_spacer(-1, true);
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    _widget->bottom_layout()->insert(_lock_widget, 1);

    // Preferences.

    Prefs prefs(Prefs::prefs(), "color_picker");
    Prefs::get_(&prefs, "size", &_size);
    Pixel::PIXEL pixel = _value.pixel();
    Prefs::get_(&prefs, "pixel", &pixel);
    _value.pixel(pixel);
    Prefs::get_(&prefs, "color_profile", &_color_profile);
    Prefs::get_(&prefs, "display_profile", &_display_profile);
    Prefs::get_(&prefs, "lock", &_lock);

    // Initialize.

    widget_update();

    Dialog::size(size_hint());

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    _size_widget->signal.set(this, size_callback);
    _color_profile_widget->signal.set(this, color_profile_callback);
    _display_profile_widget->signal.set(this, display_profile_callback);
    _lock_widget->signal.set(this, lock_callback);
    _close_widget->signal.set(this, close_callback);
}

Color_Picker::~Color_Picker()
{
    //DJV_DEBUG("Color_Picker::~Color_Picker");
}

void Color_Picker::del()
{
    //DJV_DEBUG("Color_Picker::del");

    Dialog::del();

    Prefs prefs(Prefs::prefs(), "color_picker");
    Prefs::set_(&prefs, "size", _size);
    Prefs::set_(&prefs, "pixel", _value.pixel());
    Prefs::set_(&prefs, "color_profile", _color_profile);
    Prefs::set_(&prefs, "display_profile", _display_profile);
    Prefs::set_(&prefs, "lock", _lock);

    delete _buffer;
}

void Color_Picker::set(const View_Widget * in, const V2i & pick)
{
    _view = in;
    _pick = pick;

    widget_update();
}

void Color_Picker::pick(const View_Widget * in, const V2i & pick)
{
    if (! shown() || ! visible())
    {
        return;
    }

    set(in, pick);
}

void Color_Picker::update(const View_Widget * in)
{
    if (in != _view || ! shown() || ! visible())
    {
        return;
    }

    widget_update();
}

void Color_Picker::del(const View_Widget * in)
{
    if (in != _view)
    {
        return;
    }

    _view = 0;
}

void Color_Picker::show()
{
    //DJV_DEBUG("Color_Picker::show");

    _close_widget->take_focus();

    Dialog::show();
}

Color_Picker * Color_Picker::global()
{
    static Color_Picker * global = 0;

    if (! global)
    {
        global = new Color_Picker;
    }

    return global;
}

void Color_Picker::widget_callback(const Color & in)
{
    _value = in;

    _swatch->set(in);
}

void Color_Picker::size_callback(int in)
{
    _size = static_cast<SIZE>(in);

    widget_update();
}

void Color_Picker::color_profile_callback(bool in)
{
    _color_profile = in;

    widget_update();
}

void Color_Picker::display_profile_callback(bool in)
{
    _display_profile = in;

    widget_update();
}

void Color_Picker::lock_callback(bool in)
{
    _lock = in;

    widget_update();
}

void Color_Picker::close_callback(bool)
{
    hide();
}

void Color_Picker::widget_update()
{
    if (! _view)
    {
        return;
    }

    //DJV_DEBUG("Color_Picker::widget_update");
    //DJV_DEBUG_PRINT("color profile = " << _color_profile);
    //DJV_DEBUG_PRINT("display profile = " << _display_profile);
    //DJV_DEBUG_PRINT("lock = " << _lock);

    callbacks(false);

    const Pixel_Data * data = _view->get();

    if (! _lock && data)
    {
        _value.pixel(data->pixel());
    }
    else
    {
        _value.pixel(_widget->get().pixel());
    }

    // Pick.

    const V2i pick =
        Vector_Util::floor<double, int>(V2f(_pick) / _view->zoom());

    //DJV_DEBUG_PRINT("pick = " << pick);

    // Render color sample.

    const int size = Color_Picker::size(_size);

    //DJV_DEBUG_PRINT("size = " << size);

    Pixel_Data tmp(Pixel_Data_Info(V2i(size), _value.pixel()));

    try
    {
        if (! _buffer)
        {
            _buffer = new Gl_Offscreen_Buffer(tmp.info());
        }

        Gl_Image_Options options = _view->options();
        options.xform.position -= pick - V2i((size - 1) / 2);

        if (! _color_profile)
        {
            options.color_profile = Color_Profile();
        }

        if (! _display_profile)
        {
            Display_Profile tmp;

            options.display_profile = tmp;
        }

        Pixel_Data empty;

        if (! data)
        {
            data = &empty;
        }

        Gl_Image::copy(
            *data,
            tmp,
            options,
            &_state,
            _buffer);

        Gl_Image::average(tmp, &_value);
    }
    catch (Error in)
    {
        DJV_APP->error(in);
    }

    //DJV_DEBUG_PRINT("value = " << _value);

    _widget->set(_value);
    _swatch->set(_value);
    _size_widget->set(_size);
    _color_profile_widget->set(_color_profile);
    _display_profile_widget->set(_display_profile);
    _lock_widget->set(_lock);

    callbacks(true);
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Color_Picker::SIZE, Color_Picker::label_size())

} // djv_view

