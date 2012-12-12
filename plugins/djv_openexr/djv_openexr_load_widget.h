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

//! \file djv_openexr_load_widget.h

#ifndef DJV_OPENEXR_LOAD_WIDGET_H
#define DJV_OPENEXR_LOAD_WIDGET_H

#include <djv_openexr_load.h>

#include <djv_image_io_widget.h>

namespace djv
{

class Float_Edit_Slider;
class Radio_Button_Group;

} // djv

namespace djv_openexr
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Load_Widget
//!
//! This class provides a load widget plugin.
//------------------------------------------------------------------------------

class Load_Widget : public Image_Load_Widget
{
public:

    Load_Widget();

    virtual ~Load_Widget();

    Plugin * copy() const;

    String name() const;

    void set(Image_Io_Base *);

private:

    DJV_CALLBACK(Load_Widget, color_profile_callback, int);
    DJV_CALLBACK(Load_Widget, gamma_callback, double);
    DJV_CALLBACK(Load_Widget, exposure_callback, double);
    DJV_CALLBACK(Load_Widget, exposure_defog_callback, double);
    DJV_CALLBACK(Load_Widget, exposure_knee_low_callback, double);
    DJV_CALLBACK(Load_Widget, exposure_knee_high_callback, double);
    DJV_CALLBACK(Load_Widget, channels_callback, int);
    DJV_CALLBACK(Load_Widget, callback, bool);

    void plugin_update();
    void widget_update();

    Image_Io_Base *      _plugin;
    Load::Options        _options;
    Radio_Button_Group * _color_profile_widget;
    Float_Edit_Slider *  _gamma_widget;
    Float_Edit_Slider *  _exposure_widget;
    Float_Edit_Slider *  _exposure_defog_widget;
    Float_Edit_Slider *  _exposure_knee_low_widget;
    Float_Edit_Slider *  _exposure_knee_high_widget;
    Radio_Button_Group * _channels_widget;
};

} // djv_openexr

#endif // DJV_OPENEXR_LOAD_WIDGET_H

