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

//! \file djv_view_input_prefs.h

#ifndef DJV_VIEW_INPUT_PREFS_H
#define DJV_VIEW_INPUT_PREFS_H

#include <djv_view_input.h>

#include <djv_widget.h>

namespace djv_view
{

//------------------------------------------------------------------------------
//! \class Input_Prefs
//!
//! This class provides device input preferences.
//------------------------------------------------------------------------------

class Input_Prefs : public Callback
{
public:

    //! Constructor.

    Input_Prefs();

    //! Destructor.

    virtual ~Input_Prefs();

    //! Set the mouse wheel action.

    void mouse_wheel(Input::MOUSE_WHEEL);

    //! Set the shift + mouse wheel action.

    void mouse_wheel_shift(Input::MOUSE_WHEEL);

    //! Set the control + mouse wheel action.

    void mouse_wheel_ctrl(Input::MOUSE_WHEEL);

    //! Get the mouse wheel action.

    Input::MOUSE_WHEEL mouse_wheel() const;

    //! Get the shift + mouse wheel action.

    Input::MOUSE_WHEEL mouse_wheel_shift() const;

    //! Get the control + mouse wheel action.

    Input::MOUSE_WHEEL mouse_wheel_ctrl() const;

    //! Get the global preferences.

    static Input_Prefs * global();

private:

    Input::MOUSE_WHEEL _mouse_wheel;
    Input::MOUSE_WHEEL _mouse_wheel_shift;
    Input::MOUSE_WHEEL _mouse_wheel_ctrl;
};

//------------------------------------------------------------------------------
//! \class Input_Prefs_Widget
//!
//! This class provides a device input preferences widget.
//------------------------------------------------------------------------------

class Input_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    Input_Prefs_Widget();

    //! Destructor.

    virtual ~Input_Prefs_Widget();

private:

    DJV_CALLBACK(Input_Prefs_Widget, mouse_wheel_callback, int);
    DJV_CALLBACK(Input_Prefs_Widget, mouse_wheel_shift_callback, int);
    DJV_CALLBACK(Input_Prefs_Widget, mouse_wheel_ctrl_callback, int);
};

} // djv_view

#endif // DJV_VIEW_INPUT_PREFS_H

