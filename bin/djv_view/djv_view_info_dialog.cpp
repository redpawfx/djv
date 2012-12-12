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

//! \file djv_view_info_dialog.cpp

#include <djv_view_info_dialog.h>

#include <djv_view_view_widget.h>

#include <djv_form_widget.h>
#include <djv_label.h>
#include <djv_multiline_text_display.h>
#include <djv_prefs.h>
#include <djv_push_button.h>
#include <djv_row_layout.h>
#include <djv_text_display.h>

namespace djv_view
{
using djv::Time;

//------------------------------------------------------------------------------
// Info_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_file_name = "File Name:",
    label_layer_name = "Layer:",
    label_size = "Size:",
    label_pixel = "Pixel:",
    label_time = "Time:",
    label_tag = "Tags:",
    label_close = "Clos&e";

} // namespace

Info_Dialog::Info_Dialog() :
    Dialog("Information"),
    _view             (0),
    _file_name_widget (0),
    _layer_name_widget(0),
    _size_widget      (0),
    _pixel_widget     (0),
    _time_widget      (0),
    _tag_widget       (0),
    _close_widget     (0)
{
    // Create widgets.

    _file_name_widget = new Text_Display;

    _layer_name_widget = new Text_Display;

    _size_widget = new Text_Display;

    _pixel_widget = new Text_Display;

    _time_widget = new Text_Display;

    _tag_widget = new Multiline_Text_Display;

    _close_widget = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    
    Form_Widget * form_widget = new Form_Widget;
    layout->add(form_widget);
    form_widget->add_row(label_file_name, _file_name_widget);
    form_widget->add_row(label_layer_name, _layer_name_widget);
    form_widget->add_row(label_size, _size_widget);
    form_widget->add_row(label_pixel, _pixel_widget);
    form_widget->add_row(label_time, _time_widget);
    form_widget->add_row(label_tag, _tag_widget);

    layout->add_spacer(-1, true);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    size(size_hint());

    // Callbacks.

    _close_widget->signal.set(this, close_callback);
}

Info_Dialog::~Info_Dialog()
{}

void Info_Dialog::set(const View_Widget * in, const Image_Io_Info & info)
{
    _view = in;
    _info = info;

    widget_update();
}

void Info_Dialog::pick(const View_Widget * in, const Image_Io_Info & info)
{
    if (in == _view || ! shown() || ! visible())
    {
        return;
    }

    set(in, info);
}

void Info_Dialog::update(const View_Widget * in, const Image_Io_Info & info)
{
    if (in != _view || ! shown() || ! visible())
    {
        return;
    }

    _info = info;

    widget_update();
}

void Info_Dialog::del(const View_Widget * in)
{
    if (in != _view)
    {
        return;
    }

    _view = 0;
}

void Info_Dialog::show()
{
    _close_widget->take_focus();

    Dialog::show();
}

Info_Dialog * Info_Dialog::global()
{
    static Info_Dialog * global = 0;

    if (! global)
    {
        global = new Info_Dialog;
    }

    return global;
}

void Info_Dialog::close_callback(bool)
{
    hide();
}

void Info_Dialog::widget_update()
{
    callbacks(false);

    _file_name_widget->set(_info.file_name);

    _layer_name_widget->set(_info.layer_name);

    _size_widget->set(String_Format("%%x%%:%%").
        arg(_info.size.x).
        arg(_info.size.y).
        arg(Vector_Util::aspect(_info.size)));

    _pixel_widget->set(String_Format("%%").
        arg(String_Util::label(_info.pixel)));

    _time_widget->set(String_Format("%%@%%").
        arg(Time::units_to_string(
            static_cast<int>(_info.seq.list.size()), _info.seq.speed)).
        arg(Speed::speed_to_float(_info.seq.speed), 0, -2));

    String tmp;
    const List<String> keys = _info.tag.keys();

    for (size_t i = 0; i < keys.size(); ++i)
        tmp += String_Format("%% = %%\n").
            arg(keys[i]).arg(_info.tag[keys[i]]);

    _tag_widget->set(tmp);

    callbacks(true);
}

} // djv_view

