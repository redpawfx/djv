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

//! \file djv_choice_edit_widget.h

#ifndef DJV_CHOICE_EDIT_WIDGET_H
#define DJV_CHOICE_EDIT_WIDGET_H

#include <djv_frame.h>

namespace djv
{

class Popup_Menu;
class Text_Edit;

//------------------------------------------------------------------------------
//! \class Choice_Edit_Widget
//!
//! This class provides a widget for editing text with a list of preset values.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Choice_Edit_Widget : public Frame
{
public:

    //! Constructor.

    Choice_Edit_Widget(const List<String> & list = List<String>());

    //! Destructor.

    virtual ~Choice_Edit_Widget();

    //! Set the text.

    void set(const String &);

    //! Get the text.

    String get() const;

    //! This signal is emitted when the text is changed.

    Signal<const String &> signal;

    //! Set the item labels.

    void list(const List<String> &);

    //! Get the item labels.

    const List<String> & list() const;

    virtual void size_string(const String &);

    virtual void dirty();

private:

    DJV_CALLBACK(Choice_Edit_Widget, widget_callback, const String &);
    DJV_CALLBACK(Choice_Edit_Widget, menu_callback, int);

    void list_update();

    String       _value;
    List<String> _list;
    Text_Edit *  _widget;
    Popup_Menu * _menu;
};

} // djv

#endif // DJV_CHOICE_EDIT_WIDGET_H
