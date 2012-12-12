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

//! \file djv_radio_button.h

#ifndef DJV_RADIO_BUTTON_H
#define DJV_RADIO_BUTTON_H

#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Radio_Button
//!
//! This class provides a radio button widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Radio_Button : public Widget
{
public:

    //! Constructor.

    Radio_Button(const String &);

    //! Destructor.

    virtual ~Radio_Button();

    //! Set the value.

    void set(bool = true);

    //! Get the value.

    bool get() const;

    //! This signal is emitted when the value is changed.

    Signal<bool> signal;

    //! Set the radio button group.

    void group(const List<Radio_Button *> &);

    //! Get the radio button group.

    const List<Radio_Button *> & group() const;

    //! Set the radio button in a group.

    void group_set(int);

    //! Get the radio button in a group.

    int group_get() const;

    //! This signal is emitted when the radio button in a group is changed.

    Signal<int> group_signal;

    //! Create a radio button group.

    static void create_group(const List<Radio_Button *> &);

    //! Set the label.

    void label(const String &);

    //! Get the label.

    const String & label() const;

    virtual int handle(int);

    virtual void draw();

    virtual void dirty();

private:

    void widget_update();

    bool                 _value;
    List<Radio_Button *> _group;
    String               _label;
    int                  _shortcut;
    Fl_Image *           _icon;
    String               _draw_label;
    int                  _draw_index;
};

} // djv

#endif // DJV_RADIO_BUTTON_H

