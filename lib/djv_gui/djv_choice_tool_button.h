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

//! \file djv_choice_tool_button.h

#ifndef DJV_CHOICE_TOOL_BUTTON_H
#define DJV_CHOICE_TOOL_BUTTON_H

#include <djv_frame.h>

class Fl_Image;

namespace djv
{

//------------------------------------------------------------------------------
//! \class Choice_Tool_Button
//!
//! This class provides a tool button with multiple states.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Choice_Tool_Button : public Frame
{
public:

    //! Constructor.

    Choice_Tool_Button(
        const List<String> & icon,
        int margin = 3);

    //! Destructor.

    virtual ~Choice_Tool_Button();

    //! Set the state.

    void set(int);

    //! Get the state.

    int get() const;

    //! This signal is emitted when the state is changed.

    Signal<int> signal;

    //! Set the shortcut.

    void shortcut(int);

    //! Get the shortcut.

    int shortcut() const;

    //! Set the margin.

    void margin(int);

    //! Get the margin.

    int margin() const;

    virtual int handle(int);

    virtual void draw();

    virtual void dirty();

private:

    void action();

    int              _value;
    List<Fl_Image *> _icon;
    int              _shortcut;
    int              _margin;
    bool             _pressed;
    bool             _pressed_inside;
};

} // djv

#endif // DJV_CHOICE_TOOL_BUTTON_H

