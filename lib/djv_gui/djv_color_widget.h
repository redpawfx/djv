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

//! \file djv_color_widget.h

#ifndef DJV_COLOR_WIDGET_H
#define DJV_COLOR_WIDGET_H

#include <djv_frame.h>

#include <djv_color.h>

namespace djv
{

class Choice_Widget;
class Float_Edit_Slider;
class Horizontal_Layout;
class Int_Edit_Slider;

//------------------------------------------------------------------------------
//! \class Color_Widget
//!
//! This class provides color picker widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Color_Widget : public Widget
{
public:

    //! Constructor.

    Color_Widget();

    //! Destructor.

    virtual ~Color_Widget();

    //! Set the color.

    void set(const Color &);

    //! Get the color.

    const Color & get() const;

    //! This signal is emitted when the color is changed.

    Signal<const Color &> signal;

    //! Set the floating point range.

    void float_range(double min, double max);

    //! Get the minimum floating point value.

    double float_min() const;

    //! Get the maximum floating point value.

    double float_max() const;

    //! Get the bottom layout.

    Horizontal_Layout * bottom_layout();

private:

    DJV_CALLBACK(Color_Widget, int_callback, int);
    DJV_CALLBACK(Color_Widget, float_callback, double);
    DJV_CALLBACK(Color_Widget, format_callback, int);
    DJV_CALLBACK(Color_Widget, type_callback, int);

    void widget_update();
    void value_update();

    Color               _value;
    double              _float_min;
    double              _float_max;
    Int_Edit_Slider *   _int_widget [Pixel::channels_max];
    Float_Edit_Slider * _float_widget [Pixel::channels_max];
    Choice_Widget *     _format_widget;
    Choice_Widget *     _type_widget;
    Horizontal_Layout * _bottom_layout;
};

} // djv

#endif // DJV_COLOR_WIDGET_H

