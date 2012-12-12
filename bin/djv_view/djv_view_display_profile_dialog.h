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

#ifndef DJV_VIEW_DISPLAY_PROFILE_DIALOG_H
#define DJV_VIEW_DISPLAY_PROFILE_DIALOG_H

#include <djv_view_display_profile.h>

#include <djv_dialog.h>

namespace djv
{

class File_Edit;
class Float_Edit_Slider;
class Push_Button;

} // djv

namespace djv_view
{

class View_Widget;

//------------------------------------------------------------------------------
//! \class Display_Profile_Dialog
//!
//! This class provides a display profile dialog.
//------------------------------------------------------------------------------

class Display_Profile_Dialog : public Dialog
{
public:

    //! Constructor.

    Display_Profile_Dialog();

    //! Destructor.

    virtual ~Display_Profile_Dialog();

    virtual void del()
    {
        Dialog::del();
    }

    //! Set the view.

    void set(const View_Widget *, const Display_Profile &);

    //! Update the view pick position.

    void pick(const View_Widget *, const Display_Profile &);

    //! Update the dialog.

    void update(const View_Widget *, const Display_Profile &);

    //! Remove the view.

    void del(const View_Widget *);

    //! This signal is emitted when the display profile is changed.

    Signal<const Display_Profile &> signal;

    virtual void show();

    virtual void update()
    {
        Dialog::update();
    }

    //! Get the global dialog.

    static Display_Profile_Dialog * global();

private:

    DJV_CALLBACK(Display_Profile_Dialog, widget_callback,
                 const Display_Profile &);
    DJV_CALLBACK(Display_Profile_Dialog, lut_callback, const File &);
    DJV_CALLBACK(Display_Profile_Dialog, brightness_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, contrast_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, saturation_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, levels_in0_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, levels_in1_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, gamma_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, levels_out0_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, levels_out1_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, soft_clip_callback, double);
    DJV_CALLBACK(Display_Profile_Dialog, close_callback, bool);

    void widget_update();

    const View_Widget * _view;
    Display_Profile     _value;
    File_Edit *         _lut_widget;
    Float_Edit_Slider * _brightness_widget;
    Float_Edit_Slider * _contrast_widget;
    Float_Edit_Slider * _saturation_widget;
    Float_Edit_Slider * _levels_in_widget [2];
    Float_Edit_Slider * _gamma_widget;
    Float_Edit_Slider * _levels_out_widget [2];
    Float_Edit_Slider * _soft_clip_widget;
    Push_Button *       _close_widget;
};

} // djv_view

#endif // DJV_VIEW_DISPLAY_PROFILE_DIALOG_H

