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

//! \file djv_color_dialog.h

#ifndef DJV_COLOR_DIALOG_H
#define DJV_COLOR_DIALOG_H

#include <djv_dialog.h>

#include <djv_color.h>

namespace djv
{

class Color_Swatch;
class Color_Widget;
class Push_Button;

//------------------------------------------------------------------------------
//! \class Color_Dialog
//!
//! This class provides a color picker dialog.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Color_Dialog : public Dialog
{
public:

    //! Constructor.

    Color_Dialog(const Color & = Color());

    //! Destructor.

    virtual ~Color_Dialog();

    //! Set the color.

    void set(const Color &);

    //! Get the color.

    const Color & get() const;

    //! This signal is emitted when the dialog is accepted.

    Signal<const Color &> signal;

    //! Show the global dialog.

    static void dialog(
        const Color &,
        Callback *,
        Signal<const Color &>::Callback_Fnc *);

private:

    DJV_CALLBACK(Color_Dialog, widget_callback, const Color &);
    DJV_CALLBACK(Color_Dialog, close_callback, bool);

    void widget_update();

    Color          _value;
    Color_Widget * _widget;
    Color_Swatch * _swatch;
    Push_Button *  _close_widget;
};

} // djv

#endif // DJV_COLOR_DIALOG_H
