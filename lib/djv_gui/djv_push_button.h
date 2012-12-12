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

//! \file djv_push_button.h

#ifndef DJV_PUSH_BUTTON_H
#define DJV_PUSH_BUTTON_H

#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Push_Button
//!
//! This class provides a push-button widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Push_Button : public Widget
{
public:

    //! Constructor.

    Push_Button(const String & label);

    //! Destructor.

    virtual ~Push_Button();

    //! This signal is emitted when the button is clicked.

    Signal<bool> signal;

    //! Set the label.

    void label(const String &);

    //! Get the label.

    const String & label() const;

    virtual int handle(int);

    virtual void draw();

    virtual void dirty();

private:

    String _label;
    int    _shortcut;
    bool   _pressed;
    bool   _pressed_inside;
    String _draw_label;
    int    _draw_index;
};

} // djv

#endif // DJV_PUSH_BUTTON_H

