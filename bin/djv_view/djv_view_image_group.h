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

//! \file djv_view_image_group.h

#ifndef DJV_VIEW_IMAGE_GROUP_H
#define DJV_VIEW_IMAGE_GROUP_H

#include <djv_view_display_profile.h>
#include <djv_view_image.h>

#include <djv_callback.h>
#include <djv_row_layout.h>
#include <djv_menu.h>


namespace djv
{

class Menu;
class Tool_Button;

} // djv

namespace djv_view
{

//------------------------------------------------------------------------------
//! \class Image_Group
//!
//! This class provides the image group.
//------------------------------------------------------------------------------

class Image_Group : public Callback
{
public:

    //! Constructor.

    Image_Group(
        Menu *                menu,
        Abstract_Row_Layout * toolbar,
        const Image_Group *   copy    = 0);

    //! Destructor.

    virtual ~Image_Group();

    //! Get whether the frame store is enabled.

    bool frame_store() const;

    //! This signal is emitted when the frame store is changed.

    Signal<bool> frame_store_signal;

    //! Get the mirror.

    const V2b & mirror() const;

    //! Get the scale.

    Image::SCALE scale() const;

    //! Get the rotation.

    Image::ROTATE rotate() const;

    //! Get whether the color profile is enabled.

    bool color_profile() const;

    //! Set the display profile.

    void display_profile(const Display_Profile &);

    //! Get the display profile.

    const Display_Profile & display_profile() const;

    //! This signal is emitted when the display profile is changed.

    Signal<bool> display_profile_signal;

    //! This signal is emitted when the display profile is changed.

    Signal<const Display_Profile &> display_profile_value_signal;

    //! Get the image channel.

    Gl_Image_Options::CHANNEL channel() const;

    //! This signal is emitted when a redraw is needed.

    Signal<bool> redraw_signal;

    //! This signal is emitted when a resize is needed.

    Signal<bool> resize_signal;

    //! This signal is emitted when an update is needed.

    Signal<bool> update_signal;

    //! Update menu items.

    void menu_update(Menu_Item_Group *);

    //! Update the toolbar.

    void toolbar_update();

private:

    // Callbacks.

    DJV_CALLBACK(Image_Group, frame_store_callback, bool);
    DJV_CALLBACK(Image_Group, frame_store_load_callback, bool);
    DJV_CALLBACK(Image_Group, mirror_callback, const V2b &);
    DJV_CALLBACK(Image_Group, scale_callback, Image::SCALE);
    DJV_CALLBACK(Image_Group, rotate_callback, Image::ROTATE);
    DJV_CALLBACK(Image_Group, color_profile_callback, bool);
    DJV_CALLBACK(Image_Group, display_profile_callback, bool);
    DJV_CALLBACK(Image_Group, display_profile_callback,
        const Display_Profile &);
    DJV_CALLBACK(Image_Group, display_profile_reset_callback, bool);
    DJV_CALLBACK(Image_Group, display_profile_add_callback, const String &);
    DJV_CALLBACK(Image_Group, display_profile_del_callback, const List<int> &);
    DJV_CALLBACK(Image_Group, display_profile_del_all_callback, bool);
    DJV_CALLBACK(Image_Group, display_profiles_callback, bool);
    DJV_CALLBACK(Image_Group, channel_callback, Gl_Image_Options::CHANNEL);

    DJV_FL_WIDGET_CALLBACK(Image_Group, _frame_store_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _frame_store_load_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _mirror_h_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _mirror_v_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _scale_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _rotate_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _color_profile_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _display_profile_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _display_profile_reset_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _display_profile_add_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _display_profile_del_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _display_profile_del_all_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _display_profiles_callback);
    DJV_FL_WIDGET_CALLBACK(Image_Group, _channel_callback);

    // Variables.

    bool                      _frame_store;
    V2b                       _mirror;
    Image::SCALE              _scale;
    Image::ROTATE             _rotate;
    bool                      _color_profile;
    Display_Profile           _display_profile;
    Gl_Image_Options::CHANNEL _channel;
    Menu *                    _menu;
    List<int>                 _menu_scale;
    List<int>                 _menu_rotate;
    List<int>                 _menu_display_profile;
    List<int>                 _menu_channel;
    Tool_Button *             _display_profile_widget;
};

} // djv_view

#endif // DJV_VIEW_IMAGE_GROUP_H

