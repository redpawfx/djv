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

//! \file djv_input_dialog.h

#ifndef DJV_INPUT_DIALOG_H
#define DJV_INPUT_DIALOG_H

#include <djv_message_dialog.h>

namespace djv
{

class Label;
class Text_Edit;

//------------------------------------------------------------------------------
//! \class Input_Dialog
//!
//! This class provides an input dialog.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Input_Dialog : public Abstract_Message_Dialog
{
public:

    //! Constructor.

    Input_Dialog(
        const String & label = String(),
        const String & value = String());

    //! Destructor.

    virtual ~Input_Dialog();

    //! Set the text.

    void set(const String &);

    //! Get the text.

    String get() const;

    //! This signal is emitted when the dialog is accepted.

    Signal<const String &> signal;

    //! Set the label.

    void label(const String &);

    //! Get the label.

    const String & label() const;

    virtual void show();

    //! Show the global dialog.

    static void dialog(
        const String & label,
        const String & value,
        Callback *,
        Signal<const String &>::Callback_Fnc *);

private:

    DJV_CALLBACK(Input_Dialog, widget_callback, const String &);
    DJV_CALLBACK(Input_Dialog, ok_callback, bool);
    DJV_CALLBACK(Input_Dialog, close_callback, bool);

    Text_Edit * _widget;
    Label *     _label;
};

} // djv

#endif // DJV_INPUT_DIALOG_H
