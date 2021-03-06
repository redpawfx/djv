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

//! \file djv_color_swatch.h

#ifndef DJV_COLOR_SWATCH_H
#define DJV_COLOR_SWATCH_H

#include <djv_frame.h>

#include <djv_color.h>

namespace djv
{

class Label;
class Vertical_Layout;

//------------------------------------------------------------------------------
//! \class Color_Swatch
//!
//! This class provides a basic color swatch widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Color_Swatch : public Frame
{
public:

    //! Constructor.

    Color_Swatch();

    //! Destructor.

    virtual ~Color_Swatch();

    //! Set the color.

    void set(const Color &);

    //! Get the color.

    const Color & get() const;

    //! This signal is emitted when the color is changed.

    Signal<const Color &> signal;

    //! This signal is emitted when the swatch is clicked.

    Signal<bool> action_signal;

    virtual int handle(int);

    virtual void draw();

    virtual void dirty();

private:

    Color _value;
};

//------------------------------------------------------------------------------
//! \class Color_Swatch_Widget
//!
//! This class provides a color swatch widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Color_Swatch_Widget : public Widget
{
public:

    //! Constructor.

    Color_Swatch_Widget(const String & label = String());

    //! Destructor.

    virtual ~Color_Swatch_Widget();

    //! Set the color.

    void set(const Color &);

    //! Get the color.

    const Color & get() const;

    //! This signal is emitted when the color is changed.

    Signal<const Color &> signal;

    //! Set the label.

    void label(const String &);

    //! Get the label.

    const String & label() const;

private:

    DJV_CALLBACK(Color_Swatch_Widget, widget_callback, const Color &);
    DJV_CALLBACK(Color_Swatch_Widget, dialog_callback, bool);

    Color_Swatch *    _widget;
    Label *           _label;
    Vertical_Layout * _layout;
};

} // djv

#endif // DJV_COLOR_SWATCH_H

