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

//! \file djv_cineon_save_widget.h

#ifndef DJV_CINEON_SAVE_WIDGET_H
#define DJV_CINEON_SAVE_WIDGET_H

#include <djv_cineon_save.h>

#include <djv_image_io_widget.h>

namespace djv
{

class Float_Edit_Slider;
class Int_Edit_Slider;
class Radio_Button_Group;

} // djv

namespace djv_cineon
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Save_Widget
//!
//! This class provides a save widget plugin.
//------------------------------------------------------------------------------

class Save_Widget : public Image_Save_Widget
{
public:

    //! Constructor.

    Save_Widget();

    //! Destructor.

    virtual ~Save_Widget();

    virtual Plugin * copy() const;

    virtual String name() const;

    virtual void set(Image_Io_Base *);

private:

    DJV_CALLBACK(Save_Widget, color_profile_callback, int);
    DJV_CALLBACK(Save_Widget, black_callback, int);
    DJV_CALLBACK(Save_Widget, white_callback, int);
    DJV_CALLBACK(Save_Widget, gamma_callback, double);
    DJV_CALLBACK(Save_Widget, callback, bool);

    void plugin_update();
    void widget_update();

    Image_Io_Base *      _plugin;
    Save::Options        _options;
    Radio_Button_Group * _color_profile_widget;
    Int_Edit_Slider *    _black_widget;
    Int_Edit_Slider *    _white_widget;
    Float_Edit_Slider *  _gamma_widget;
};

} // djv_cineon

#endif // DJV_CINEON_SAVE_WIDGET_H

