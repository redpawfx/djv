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

//! \file djv_inc_button.h

#ifndef DJV_INC_BUTTON_H
#define DJV_INC_BUTTON_H

#include <djv_widget.h>

namespace djv
{

class Tool_Button;

//------------------------------------------------------------------------------
//! \class Inc_Button
//!
//! This class provides a widget with increment/decrement buttons.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Inc_Button : public Widget
{
public:

    //! Constructor.

    Inc_Button();

    //! !Denstructor.

    virtual ~Inc_Button();

    //! This signal is emitted to increment the value.

    Signal<bool> inc_signal;

    //! This signal is emitted to decrement the value.

    Signal<bool> dec_signal;

    //! This signal is emitted when the widget is clicked.

    Signal<bool> push_signal;

private:

    DJV_CALLBACK(Inc_Button, inc_callback, bool);
    DJV_CALLBACK(Inc_Button, dec_callback, bool);
    DJV_CALLBACK(Inc_Button, push_callback, bool);

    Tool_Button * _inc_widget;
    Tool_Button * _dec_widget;
};

} // djv

#endif // DJV_INC_BUTTON_H

