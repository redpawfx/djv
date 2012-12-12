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

//! \file djv_view_shortcut_prefs.cpp

#include <djv_view_shortcut_prefs.h>

#include <djv_view_shortcut.h>

#include <djv_application.h>
#include <djv_row_layout.h>
#include <djv_prefs.h>
#include <djv_shortcut_widget.h>
#include <djv_style.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Shortcut_Prefs
//------------------------------------------------------------------------------

Shortcut_Prefs::Shortcut_Prefs()
{
    String tmp;

    const List<String> & label = Shortcut::label_shortcut();

    _shortcuts += djv::Shortcut(label[Shortcut::EXIT], FL_COMMAND + 'q');

    _shortcuts += djv::Shortcut(label[Shortcut::FILE_OPEN], FL_COMMAND + 'o');
    _shortcuts += djv::Shortcut(label[Shortcut::FILE_RELOAD], FL_COMMAND + 'r');
    _shortcuts += djv::Shortcut(label[Shortcut::FILE_SAVE], FL_COMMAND + 's');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_SAVE_FRAME], FL_SHIFT + FL_COMMAND + 's');
    _shortcuts += djv::Shortcut(label[Shortcut::FILE_CLOSE], FL_COMMAND + 'w');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_DEFAULT], FL_COMMAND + '`');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_1], FL_COMMAND + '1');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_2], FL_COMMAND + '2');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_3], FL_COMMAND + '3');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_4], FL_COMMAND + '4');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_5], FL_COMMAND + '5');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_6], FL_COMMAND + '6');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_7], FL_COMMAND + '7');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_8], FL_COMMAND + '8');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_9], FL_COMMAND + '9');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_10], FL_COMMAND + '0');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_PREV], FL_COMMAND + '-');
    _shortcuts += djv::Shortcut(
        label[Shortcut::FILE_LAYER_NEXT], FL_COMMAND + '=');
    _shortcuts += djv::Shortcut(label[Shortcut::FILE_PROXY_NONE], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::FILE_PROXY_1_2], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::FILE_PROXY_1_4], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::FILE_PROXY_1_8], 0);

    _shortcuts += djv::Shortcut(label[Shortcut::WINDOW_NEW], FL_COMMAND + 'n');
    _shortcuts += djv::Shortcut(label[Shortcut::WINDOW_COPY], FL_COMMAND + 'c');
    _shortcuts += djv::Shortcut(
        label[Shortcut::WINDOW_CLOSE], FL_COMMAND + 'e');
    _shortcuts += djv::Shortcut(label[Shortcut::WINDOW_FIT], 'f');
    _shortcuts += djv::Shortcut(label[Shortcut::WINDOW_FULLSCREEN], 'u');
    _shortcuts += djv::Shortcut(label[Shortcut::WINDOW_TOOLBAR_BUTTONS], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::WINDOW_TOOLBAR_PLAYBACK], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::WINDOW_TOOLBAR_INFO], 0);

    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_LEFT], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_RIGHT], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_UP], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_DOWN], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_CENTER], FL_Insert);
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_ZOOM_IN], '=');
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_ZOOM_OUT], '-');
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_ZOOM_RESET], '0');
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_RESET], FL_Delete);
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_FIT], FL_BackSpace);
    _shortcuts += djv::Shortcut(label[Shortcut::VIEW_HUD], 'h');

    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_FRAME_STORE], 'e');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_FRAME_STORE_LOAD], FL_SHIFT + 'e');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_MIRROR_HORIZONTAL], FL_F + 1);
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_MIRROR_VERTICAL], FL_F + 2);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_SCALE_NONE], FL_F + 5);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_SCALE_16_9], FL_F + 6);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_SCALE_1_85], FL_F + 7);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_SCALE_2_35], FL_F + 8);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_ROTATE_0], FL_F + 9);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_ROTATE_90], FL_F + 10);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_ROTATE_180], FL_F + 11);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_ROTATE_270], FL_F + 12);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_COLOR_PROFILE], 0);
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_CHANNEL_RED], 'r');
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_CHANNEL_GREEN], 'g');
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_CHANNEL_BLUE], 'b');
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_CHANNEL_ALPHA], 'a');
    _shortcuts += djv::Shortcut(label[Shortcut::IMAGE_DISPLAY_PROFILE], 0);
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_RESET], FL_SHIFT + '`');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_1], FL_SHIFT + '1');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_2], FL_SHIFT + '2');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_3], FL_SHIFT + '3');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_4], FL_SHIFT + '4');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_5], FL_SHIFT + '5');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_6], FL_SHIFT + '6');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_7], FL_SHIFT + '7');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_8], FL_SHIFT + '8');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_9], FL_SHIFT + '9');
    _shortcuts += djv::Shortcut(
        label[Shortcut::IMAGE_DISPLAY_PROFILE_10], FL_SHIFT + '0');

    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_STOP], 'k');
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_FORWARD], 'l');
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_REVERSE], 'j');
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_LOOP], ';');
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_START], FL_Up);
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_START_ABS], FL_SHIFT + FL_Up);
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_END], FL_Down);
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_END_ABS], FL_SHIFT + FL_Down);
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_PREV], FL_Left);
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_PREV_X10], FL_SHIFT + FL_Left);
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_PREV_X100], FL_ALT + FL_Left);
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_NEXT], FL_Right);
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_NEXT_X10], FL_SHIFT + FL_Right);
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_NEXT_X100], FL_ALT + FL_Right);
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_IN_OUT], 'p');
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_IN_MARK], 'i');
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_IN_RESET], FL_SHIFT + 'i');
    _shortcuts += djv::Shortcut(label[Shortcut::PLAYBACK_OUT_MARK], 'o');
    _shortcuts += djv::Shortcut(
        label[Shortcut::PLAYBACK_OUT_RESET], FL_SHIFT + 'o');

    _shortcuts += djv::Shortcut(label[Shortcut::TOOL_MAGNIFY], '1');
    _shortcuts += djv::Shortcut(label[Shortcut::TOOL_COLOR_PICKER], '2');
    _shortcuts += djv::Shortcut(label[Shortcut::TOOL_HISTOGRAM], '3');
    _shortcuts += djv::Shortcut(label[Shortcut::TOOL_INFO], '4');

    Prefs prefs(Prefs::prefs(), "shortcut");

    for (size_t i = 0; i < _shortcuts.size(); ++i)
    {
        if (prefs.get(_shortcuts[i].name, &tmp))
        {
            djv::Shortcut::serialize(&tmp, &_shortcuts[i].value);
        }
    }
}

Shortcut_Prefs::~Shortcut_Prefs()
{
    Prefs prefs(Prefs::prefs(), "shortcut");

    for (size_t i = 0; i < _shortcuts.size(); ++i)
    {
        prefs.set(
            _shortcuts[i].name,
            djv::Shortcut::serialize(_shortcuts[i].value));
    }
}

void Shortcut_Prefs::shortcuts(const List<djv::Shortcut> & in)
{
    if (in == _shortcuts)
    {
        return;
    }

    _shortcuts = in;

    DJV_APP->window_update();
}

const List<djv::Shortcut> & Shortcut_Prefs::shortcuts() const
{
    return _shortcuts;
}

//------------------------------------------------------------------------------
// Shortcut_Prefs_Widget
//------------------------------------------------------------------------------

Shortcut_Prefs_Widget::Shortcut_Prefs_Widget()
{
    // Create widgets.

    Shortcut_Widget * shortcut_widget = new Shortcut_Widget;

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    layout->add(shortcut_widget);
    layout->stretch(shortcut_widget);

    // Initialize.

    shortcut_widget->set(Shortcut_Prefs::global()->shortcuts());

    // Callbacks.

    shortcut_widget->signal.set(this, shortcut_callback);
}

void Shortcut_Prefs_Widget::shortcut_callback(const List<djv::Shortcut> & in)
{
    Shortcut_Prefs::global()->shortcuts(in);
}

} // djv_view

