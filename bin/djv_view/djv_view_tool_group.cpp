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

//! \file djv_view_tool_group.cpp

#include <djv_view_tool_group.h>

#include <djv_view_color_picker.h>
#include <djv_view_histogram_dialog.h>
#include <djv_view_info_dialog.h>
#include <djv_view_magnify_dialog.h>
#include <djv_view_shortcut.h>
#include <djv_view_shortcut_prefs.h>

#include <djv_frame_group.h>
#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_style.h>
#include <djv_tool_button.h>

#include <djv_image_io.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Tool_Group
//------------------------------------------------------------------------------

Tool_Group::Tool_Group(
    Menu *                 menu,
    Abstract_Row_Layout *  toolbar,
    const Tool_Group *     copy) :
    magnify_signal      (this),
    color_picker_signal (this),
    histogram_signal    (this),
    info_signal         (this),
    _menu               (0),
    _magnify_widget     (0),
    _color_picker_widget(0),
    _histogram_widget   (0),
    _info_widget        (0)
{
    //DJV_DEBUG("Tool_Group::Tool_Group");

    // Create widgets.

    _menu = menu;

    _magnify_widget = new Tool_Button("magnify");

    _color_picker_widget = new Tool_Button("color_picker");

    _histogram_widget = new Tool_Button("histogram");

    _info_widget = new Tool_Button("info");

    // Layout.

    toolbar->add(new Horizontal_Frame_Group(List<Frame *>() <<
        _magnify_widget <<
        _color_picker_widget <<
        _histogram_widget <<
        _info_widget));

    // Initialize.

    // Callbacks.

    _magnify_widget->signal.set(this, magnify_callback);

    _color_picker_widget->signal.set(this, color_picker_callback);

    _histogram_widget->signal.set(this, histogram_callback);

    _info_widget->signal.set(this, info_callback);
}

Tool_Group::~Tool_Group()
{}

namespace
{

const String
    menu_title = "Tool",
    menu_magnify = "&Magnify",
    menu_color_picker = "&Color Picker",
    menu_histogram = "Histo&gram",
    menu_info = "I&n&formation";

} // namespace

void Tool_Group::menu_update(Menu_Item_Group * in)
{
    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    // Menu:
    //
    // * Magnify
    // * Color Picker
    // * Histogram
    // * Information

    in->add(menu_title, 0, 0, 0, Menu_Item::SUB_MENU);

    in->add(
        menu_magnify,
        shortcuts[Shortcut::TOOL_MAGNIFY].value,
        _magnify_callback,
        this);

    in->add(
        menu_color_picker,
        shortcuts[Shortcut::TOOL_COLOR_PICKER].value,
        _color_picker_callback,
        this);

    in->add(
        menu_histogram,
        shortcuts[Shortcut::TOOL_HISTOGRAM].value,
        _histogram_callback,
        this);

    in->add(
        menu_info,
        shortcuts[Shortcut::TOOL_INFO].value,
        _info_callback,
        this);

    in->end();
}

namespace
{

const String
    tooltip_magnify = "Magnify\n\nShortcut: %%",
    tooltip_color_picker = "Color picker\n\nShortcut: %%",
    tooltip_histogram = "Histogram\n\nShortcut: %%",
    tooltip_info = "Information\n\nShortcut: %%";

} // namespace

void Tool_Group::toolbar_update()
{
    // Update tooltips.

    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    _magnify_widget->tooltip(String_Format(tooltip_magnify).
        arg(djv::Shortcut::label(shortcuts[Shortcut::TOOL_MAGNIFY].value)));

    _color_picker_widget->tooltip(String_Format(tooltip_color_picker).
        arg(djv::Shortcut::label(
            shortcuts[Shortcut::TOOL_COLOR_PICKER].value)));

    _histogram_widget->tooltip(String_Format(tooltip_histogram).
        arg(djv::Shortcut::label(shortcuts[Shortcut::TOOL_HISTOGRAM].value)));

    _info_widget->tooltip(String_Format(tooltip_info).
        arg(djv::Shortcut::label(shortcuts[Shortcut::TOOL_INFO].value)));
}

void Tool_Group::magnify_callback(bool)
{
    magnify_signal.emit(true);
}

void Tool_Group::_magnify_callback()
{
    magnify_callback(true);
}

void Tool_Group::color_picker_callback(bool)
{
    color_picker_signal.emit(true);
}

void Tool_Group::_color_picker_callback()
{
    color_picker_callback(true);
}

void Tool_Group::histogram_callback(bool)
{
    histogram_signal.emit(true);
}

void Tool_Group::_histogram_callback()
{
    histogram_callback(true);
}

void Tool_Group::info_callback(bool)
{
    info_signal.emit(true);
}

void Tool_Group::_info_callback()
{
    info_callback(true);
}

} // djv_view

