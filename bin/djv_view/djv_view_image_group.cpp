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

//! \file djv_view_image_group.cpp

#include <djv_view_image_group.h>

#include <djv_view_display_profile.h>
#include <djv_view_image_prefs.h>
#include <djv_view_shortcut.h>
#include <djv_view_shortcut_prefs.h>

#include <djv_choice_widget.h>
#include <djv_input_dialog.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_multi_choice_dialog.h>
#include <djv_question_dialog.h>
#include <djv_style.h>
#include <djv_tool_button.h>

#include <djv_image_io.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Image_Group
//------------------------------------------------------------------------------

Image_Group::Image_Group(
    Menu *                menu,
    Abstract_Row_Layout * toolbar,
    const Image_Group *   copy) :
    frame_store_signal          (this),
    display_profile_signal      (this),
    display_profile_value_signal(this),
    redraw_signal               (this),
    resize_signal               (this),
    update_signal               (this),
    _frame_store                (false),
    _mirror                     (Image_Prefs::global()->mirror()),
    _scale                      (Image_Prefs::global()->scale()),
    _rotate                     (Image_Prefs::global()->rotate()),
    _color_profile              (Image_Prefs::global()->color_profile()),
    _display_profile            (Image_Prefs::global()->
                                    display_profile_value()),
    _channel                    (Image_Prefs::global()->channel())
{
    //DJV_DEBUG("Image_Group::Image_Group");

    if (copy)
    {
        _frame_store     = copy->_frame_store;
        _mirror          = copy->_mirror;
        _scale           = copy->_scale;
        _rotate          = copy->_rotate;
        _color_profile   = copy->_color_profile;
        _display_profile = copy->_display_profile;
        _channel         = copy->_channel;
    }

    // Create widgets.

    _menu = menu;

    _display_profile_widget = new Tool_Button("color_wheel");

    // Layout.

    toolbar->add(_display_profile_widget);

    // Callbacks.

    _display_profile_widget->signal.set(this, display_profile_callback);

    Image_Prefs::global()->mirror_signal.set(this, mirror_callback);
    Image_Prefs::global()->scale_signal.set(this, scale_callback);
    Image_Prefs::global()->rotate_signal.set(this, rotate_callback);
    Image_Prefs::global()->color_profile_signal.set(
        this, color_profile_callback);
    Image_Prefs::global()->display_profile_value_signal.set(
        this, display_profile_callback);
    Image_Prefs::global()->display_profiles_signal.set(
        this, display_profiles_callback);
    Image_Prefs::global()->channel_signal.set(this, channel_callback);
}

Image_Group::~Image_Group()
{}

void Image_Group::display_profile(const Display_Profile & in)
{
    _display_profile = in;
}

namespace
{

const String
    menu_title = "Image",
    menu_frame_store = "&Frame Store",
    menu_frame_store_load = "&Load Frame Store",
    menu_mirror_h = "Mirror &Horizontal",
    menu_mirror_v = "Mirror &Vertical",
    menu_scale = "&Scale",
    menu_rotate = "&Rotate",
    menu_color_profile = "&Color Profile",
    menu_display_profile = "&Display Profile",
    menu_display_profile_show = "Show",
    menu_display_profile_reset = "Reset",
    menu_display_profile_add = "Add",
    menu_display_profile_del = "Delete",
    menu_display_profile_del_all = "Delete All",
    menu_channel = "C&hannel";

} // namespace

void Image_Group::menu_update(Menu_Item_Group * in)
{
    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    // Menu:
    //
    // * Frame Store
    // * Load Frame Store
    //   ---
    // * Mirror Horizontal
    // * Mirror Vertical
    // * Scale
    //   * ...
    // * Rotate
    //   * ...
    //   ---
    // * Color Profile
    // * Display Profile
    //   * Show
    //   * Reset
    //     ---
    //   * Add
    //   * Delete
    //   * Delete All
    //     ---
    //   * ...
    //   ---
    // * Channel
    //   * ...

    in->add(menu_title, 0, 0, 0, Menu_Item::SUB_MENU);

    in->add(
        menu_frame_store,
        shortcuts[Shortcut::IMAGE_FRAME_STORE].value,
        _frame_store_callback,
        this,
        Menu_Item::TOGGLE,
        _frame_store);

    in->add(
        menu_frame_store_load,
        shortcuts[Shortcut::IMAGE_FRAME_STORE_LOAD].value,
        _frame_store_load_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(
        menu_mirror_h,
        shortcuts[Shortcut::IMAGE_MIRROR_HORIZONTAL].value,
        _mirror_h_callback,
        this,
        Menu_Item::TOGGLE,
        _mirror.x);

    in->add(
        menu_mirror_v,
        shortcuts[Shortcut::IMAGE_MIRROR_VERTICAL].value,
        _mirror_v_callback,
        this,
        Menu_Item::TOGGLE,
        _mirror.y);

    in->add(menu_scale, 0, 0, 0, Menu_Item::SUB_MENU);

    _menu_scale = in->add(
        Image::label_scale(),
        List<int>() <<
        shortcuts[Shortcut::IMAGE_SCALE_NONE].value <<
        shortcuts[Shortcut::IMAGE_SCALE_16_9].value <<
        shortcuts[Shortcut::IMAGE_SCALE_1_85].value <<
        shortcuts[Shortcut::IMAGE_SCALE_2_35].value,
        _scale_callback,
        this,
        Menu_Item::RADIO,
        _scale);
    
    in->end();

    in->add(menu_rotate, 0, 0, 0, Menu_Item::SUB_MENU | Menu_Item::DIVIDER);
    
    _menu_rotate = in->add(
        Image::label_rotate(),
        List<int>() <<
        shortcuts[Shortcut::IMAGE_ROTATE_0].value <<
        shortcuts[Shortcut::IMAGE_ROTATE_90].value <<
        shortcuts[Shortcut::IMAGE_ROTATE_180].value <<
        shortcuts[Shortcut::IMAGE_ROTATE_270].value,
        _rotate_callback,
        this,
        Menu_Item::RADIO,
        _rotate);
    
    in->end();

    in->add(
        menu_color_profile,
        shortcuts[Shortcut::IMAGE_COLOR_PROFILE].value,
        _color_profile_callback,
        this,
        Menu_Item::TOGGLE,
        _color_profile);

    in->add(
        menu_display_profile,
        0,
        0,
        0,
        Menu_Item::SUB_MENU | Menu_Item::DIVIDER);

    in->add(
        menu_display_profile_show,
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE].value,
        _display_profile_callback,
        this);

    in->add(
        menu_display_profile_reset,
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_RESET].value,
        _display_profile_reset_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(menu_display_profile_add, 0, _display_profile_add_callback, this);
    in->add(menu_display_profile_del, 0, _display_profile_del_callback, this);

    in->add(
        menu_display_profile_del_all,
        0,
        _display_profile_del_all_callback,
        this,
        Menu_Item::DIVIDER);

    _menu_display_profile = in->add(
        Image_Prefs::global()->display_profile_names(),
        List<int>() <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_1].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_2].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_3].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_4].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_5].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_6].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_7].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_8].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_9].value <<
        shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE_10].value,
        _display_profiles_callback,
        this);
    
    in->end();

    in->add(menu_channel, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_channel = in->add(
        Gl_Image_Options::label_channel(),
        List<int>() <<
        0 <<
        shortcuts[Shortcut::IMAGE_CHANNEL_RED].value <<
        shortcuts[Shortcut::IMAGE_CHANNEL_GREEN].value <<
        shortcuts[Shortcut::IMAGE_CHANNEL_BLUE].value <<
        shortcuts[Shortcut::IMAGE_CHANNEL_ALPHA].value,
        _channel_callback,
        this,
        Menu_Item::RADIO,
        _channel);
    
    in->end();

    in->end();
}

namespace
{

const String
    tooltip_display_profile = "Display profile\n\nShortcut: %%";

} // namespace

void Image_Group::toolbar_update()
{
    // Update tooltips.

    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    _display_profile_widget->tooltip(String_Format(tooltip_display_profile).
        arg(djv::Shortcut::label(
            shortcuts[Shortcut::IMAGE_DISPLAY_PROFILE].value)));
}

bool Image_Group::frame_store() const
{
    return _frame_store;
}

void Image_Group::frame_store_callback(bool in)
{
    //DJV_DEBUG("Image_Group::frame_store_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _frame_store = in;

    redraw_signal.emit(true);
    update_signal.emit(true);
}

void Image_Group::frame_store_load_callback(bool)
{
    //DJV_DEBUG("Image_Group::frame_store_load_callback");

    frame_store_signal.emit(true);
}

void Image_Group::_frame_store_callback()
{
    frame_store_callback(_menu->value());
}

void Image_Group::_frame_store_load_callback()
{
    frame_store_load_callback(true);
}

const V2b & Image_Group::mirror() const
{
    return _mirror;
}

void Image_Group::mirror_callback(const V2b & in)
{
    //DJV_DEBUG("Image_Group::mirror_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _mirror = in;

    redraw_signal.emit(true);
    update_signal.emit(true);
}

void Image_Group::_mirror_h_callback()
{
    mirror_callback(V2b(_menu->value(), _mirror.y));
}

void Image_Group::_mirror_v_callback()
{
    mirror_callback(V2b(_mirror.x, _menu->value()));
}

Image::SCALE Image_Group::scale() const
{
    return _scale;
}

void Image_Group::scale_callback(Image::SCALE in)
{
    //DJV_DEBUG("Image_Group::scale_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _scale = in;

    redraw_signal.emit(true);
    resize_signal.emit(true);
    update_signal.emit(true);
}

void Image_Group::_scale_callback()
{
    scale_callback(
        Image::SCALE(List_Util::find(_menu->item(), _menu_scale)));
}

Image::ROTATE Image_Group::rotate() const
{
    return _rotate;
}

void Image_Group::rotate_callback(Image::ROTATE in)
{
    //DJV_DEBUG("Image_Group::rotate_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _rotate = in;

    redraw_signal.emit(true);
    resize_signal.emit(true);
    update_signal.emit(true);
}

void Image_Group::_rotate_callback()
{
    rotate_callback(Image::ROTATE(
        List_Util::find(_menu->item(), _menu_rotate)));
}

bool Image_Group::color_profile() const
{
    return _color_profile;
}

void Image_Group::color_profile_callback(bool in)
{
    //DJV_DEBUG("Image_Group::color_profile_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _color_profile = in;

    redraw_signal.emit(true);
    update_signal.emit(true);
}

void Image_Group::_color_profile_callback()
{
    color_profile_callback(_menu->value());
}

const Display_Profile & Image_Group::display_profile() const
{
    return _display_profile;
}

void Image_Group::display_profile_callback(bool)
{
    display_profile_signal.emit(true);
}

void Image_Group::display_profile_callback(const Display_Profile & in)
{
    display_profile_value_signal.emit(in);
}

void Image_Group::_display_profile_callback()
{
    display_profile_callback(true);
}

void Image_Group::display_profile_reset_callback(bool)
{
    Display_Profile tmp;

    display_profile_callback(tmp);
}

void Image_Group::_display_profile_reset_callback()
{
    display_profile_reset_callback(true);
}

void Image_Group::display_profile_add_callback(const String & in)
{
    //DJV_DEBUG("Image_Group::display_profile_add_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    List<Display_Profile> list = Image_Prefs::global()->display_profiles();
    Display_Profile tmp = _display_profile;
    tmp.name = in;
    list += tmp;
    Image_Prefs::global()->display_profiles(list);
}

namespace
{

const String
    label_display_profile_add = "Add image display profile:",
    label_display_profile_del = "Delete image display profiles:",
    label_display_profile_del_all = "Delete all image display profiles?";

} // namespace

void Image_Group::_display_profile_add_callback()
{
    Input_Dialog::dialog(
        label_display_profile_add,
        "",
        this,
        display_profile_add_callback);
}

void Image_Group::display_profile_del_callback(const List<int> & in)
{
    //DJV_DEBUG("Image_Group::display_profile_del_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    const List<Display_Profile> & list =
        Image_Prefs::global()->display_profiles();

    List<Display_Profile> tmp;

    for (size_t i = 0; i < list.size(); ++i)
    {
        size_t j = 0;

        for (; j < in.size(); ++j)
        {
            if (in[j] == static_cast<int>(i))
            {
                break;
            }
        }

        if (j < in.size())
        {
            continue;
        }

        tmp += list[i];
    }

    Image_Prefs::global()->display_profiles(tmp);
}

void Image_Group::_display_profile_del_callback()
{
    const List<String> list = Image_Prefs::global()->display_profile_names();

    if (! list.size())
    {
        return;
    }

    Multi_Choice_Dialog::dialog(
        label_display_profile_del,
        list,
        this,
        display_profile_del_callback);
}

void Image_Group::display_profile_del_all_callback(bool)
{
    Image_Prefs::global()->display_profiles(List<Display_Profile>());
}

void Image_Group::_display_profile_del_all_callback()
{
    const List<String> list = Image_Prefs::global()->display_profile_names();

    if (! list.size())
    {
        return;
    }

    Question_Dialog::dialog(
        label_display_profile_del_all,
        this,
        display_profile_del_all_callback);
}

void Image_Group::display_profiles_callback(bool)
{
    //DJV_DEBUG("Image_Group::display_profiles_callback");

    update_signal.emit(true);
}

void Image_Group::_display_profiles_callback()
{
    display_profile_callback(
        Image_Prefs::global()->display_profiles()[
            List_Util::find(_menu->item(), _menu_display_profile)]);
}

Gl_Image_Options::CHANNEL Image_Group::channel() const
{
    return _channel;
}

void Image_Group::channel_callback(Gl_Image_Options::CHANNEL in)
{
    //DJV_DEBUG("Image_Group::channel_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    if (in == _channel)
    {
        _channel = Gl_Image_Options::CHANNEL_DEFAULT;
    }
    else
    {
        _channel = in;
    }

    redraw_signal.emit(true);
    update_signal.emit(true);
}

void Image_Group::_channel_callback()
{
    channel_callback(
        Gl_Image_Options::CHANNEL(List_Util::find(_menu->item(),
            _menu_channel)));
}

} // djv_view

