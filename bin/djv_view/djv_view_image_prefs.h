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

//! \file djv_view_image_prefs.h

#ifndef DJV_VIEW_IMAGE_PREFS_H
#define DJV_VIEW_IMAGE_PREFS_H

#include <djv_view_display_profile.h>
#include <djv_view_image.h>

#include <djv_widget.h>

namespace djv
{

class Radio_Button_Group;

} // djv

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Image_Prefs
//!
//! This class provides image preferences.
//------------------------------------------------------------------------------

class Image_Prefs : public Callback
{
public:

    //! Constructor.

    Image_Prefs();

    //! Destructor.

    virtual ~Image_Prefs();

    //! Set whether to store the frame when reloading files.

    void frame_store_file_reload(bool);

    //! Get whether to store the frame when reloading files.

    bool frame_store_file_reload() const;

    //! Set the mirror.

    void mirror(const V2b &);

    //! Get the mirror.

    const V2b & mirror() const;

    //! This signal is emitted when the mirror is changed.

    Signal<const V2b &> mirror_signal;

    //! Set the scale.

    void scale(Image::SCALE);

    //! Get the scale.

    Image::SCALE scale() const;

    //! This signal is emitted when the scale is changed.

    Signal<Image::SCALE> scale_signal;

    //! Set the rotation.

    void rotate(Image::ROTATE);

    //! Get the rotation.

    Image::ROTATE rotate() const;

    //! This signal is emitted when the rotation is changed.

    Signal<Image::ROTATE> rotate_signal;

    //! Set whether the color profile is enabled.

    void color_profile(bool);

    //! Get whether the color profile is enabled.

    bool color_profile() const;

    //! This signal is emitted when the color profile is changed.

    Signal<bool> color_profile_signal;

    //! Set whether the display profile is enabled.

    void display_profile(int);

    //! Set the display profiles.

    void display_profiles(const List<Display_Profile> &);

    //! Get the current display profile index.

    int display_profile() const;

    //! Get the current display profile values.

    Display_Profile display_profile_value() const;

    //! Get the display profiles.

    const List<Display_Profile> & display_profiles() const;

    //! Get the display profile names.

    List<String> display_profile_names() const;

    //! This signal is emitted when the current display profile is changed.

    Signal<int> display_profile_signal;

    //! This signal is emitted when the current display profile is changed.

    Signal<const Display_Profile &> display_profile_value_signal;

    //! This signal is emitted the the display profiles are changed.

    Signal<bool> display_profiles_signal;

    //! Set the image channel.

    void channel(Gl_Image_Options::CHANNEL);

    //! Get the image channel.

    Gl_Image_Options::CHANNEL channel() const;

    //! This signal is emitted when the image channel is changed.

    Signal<Gl_Image_Options::CHANNEL> channel_signal;

    //! Get global preferences.

    static Image_Prefs * global();

private:

    bool                      _frame_store_file_reload;
    V2b                       _mirror;
    Image::SCALE              _scale;
    Image::ROTATE             _rotate;
    bool                      _color_profile;
    int                       _display_profile;
    List<Display_Profile>     _display_profiles;
    Gl_Image_Options::CHANNEL _channel;
};

//------------------------------------------------------------------------------
//! \class Image_Prefs_Widget
//!
//! This class provides an image preferences widget.
//------------------------------------------------------------------------------

class Image_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    Image_Prefs_Widget();

    //! Destructor.

    virtual ~Image_Prefs_Widget();

private:

    DJV_CALLBACK(Image_Prefs_Widget, frame_store_file_reload_callback, bool);
    DJV_CALLBACK(Image_Prefs_Widget, mirror_callback, const List<bool> &);
    DJV_CALLBACK(Image_Prefs_Widget, scale_callback, int);
    DJV_CALLBACK(Image_Prefs_Widget, rotate_callback, int);
    DJV_CALLBACK(Image_Prefs_Widget, color_profile_callback, bool);
    DJV_CALLBACK(Image_Prefs_Widget, display_profile_callback, int);
    DJV_CALLBACK(Image_Prefs_Widget, display_profiles_callback, bool);
    DJV_CALLBACK(Image_Prefs_Widget, channel_callback, int);

    void widget_update();

    Radio_Button_Group * _display_profile_widget;
};

} // djv_view

#endif // DJV_VIEW_IMAGE_PREFS_H

