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

//! \file djv_color_dialog.cpp

#include <djv_color_dialog.h>

#include <djv_color_swatch.h>
#include <djv_color_widget.h>
#include <djv_row_layout.h>
#include <djv_push_button.h>
#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// Color_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_color = "Color Dialog",
    label_close = "Clos&e";

} // namespace

Color_Dialog::Color_Dialog(const Color & value) :
    Dialog(label_color),
    signal       (this),
    _widget      (0),
    _swatch      (0),
    _close_widget(0)
{
    // Create widgets.

    _widget = new Color_Widget;

    _swatch = new Color_Swatch;
    _swatch->size(STRETCH, STRETCH);

    _close_widget = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout->stretch(layout_h);
    layout_h->add(_swatch);
    layout_h->add(_widget);
    layout_h->stretch(_widget);

    layout->add_spacer(-1, true);

    layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    size(size_hint());

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    _close_widget->signal.set(this, close_callback);
}

Color_Dialog::~Color_Dialog()
{}

void Color_Dialog::set(const Color & in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();

    signal.emit(_value);
}

const Color & Color_Dialog::get() const
{
    return _value;
}

void Color_Dialog::dialog(
    const Color &                         value,
    Callback *                            in,
    Signal<const Color &>::Callback_Fnc * callback)
{
    static Color_Dialog * dialog = 0;

    if (! dialog)
    {
        dialog = new Color_Dialog;
    }

    dialog->signal.del();

    dialog->set(value);

    if (callback)
    {
        dialog->signal.set(in, callback);
    }

    dialog->show();
}

void Color_Dialog::widget_callback(const Color & in)
{
    set(in);
}

void Color_Dialog::close_callback(bool)
{
    hide();
}

void Color_Dialog::widget_update()
{
    callbacks(false);

    _widget->set(_value);
    _swatch->set(_value);

    callbacks(true);
}

} // djv

