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

//! \file djv_radio_button_group.h

#ifndef DJV_RADIO_BUTTON_GROUP_H
#define DJV_RADIO_BUTTON_GROUP_H

#include <djv_widget.h>

namespace djv
{

class Label;
class Radio_Button;
class Vertical_Layout;

//------------------------------------------------------------------------------
//! \class Radio_Button_Group
//!
//! This class provides a group of radio buttons.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Radio_Button_Group : public Widget
{
public:

    //! Constructor.

    Radio_Button_Group(
        const List<String> & label = List<String>());

    //! Constructor.

    Radio_Button_Group(
        const String & title,
        const List<String> & label = List<String>());

    //! Destructor.

    virtual ~Radio_Button_Group();

    //! Set the value.

    void set(int);

    //! Get the value.

    int get() const;

    //! This signal is emitted when the value is changed.

    Signal<int> signal;

    //! Set the title.

    void title(const String &);

    //! Get the title.

    const String & title() const;

    //! Set the labels.

    void label(const List<String> &);

    //! Get the labels.

    const List<String> & label() const;

private:

    void init();

    DJV_CALLBACK(Radio_Button_Group, widget_callback, int);

    void value_update();
    void widget_update();

    int                  _value;
    String               _title;
    List<String>         _label;
    Label *              _title_widget;
    Vertical_Layout *    _widget_layout;
    Vertical_Layout *    _layout;
    List<Radio_Button *> _widgets;
};

} // djv

#endif // DJV_RADIO_BUTTON_GROUP_H

