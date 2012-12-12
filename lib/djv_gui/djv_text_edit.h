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

//! \file djv_text_edit.h

#ifndef DJV_TEXT_EDIT_H
#define DJV_TEXT_EDIT_H

#include <djv_abstract_text_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Text_Edit
//!
//! This class provides a widget for editing text.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Text_Edit : public Abstract_Text_Widget
{
public:

    //! Constructor.

    Text_Edit(const String & = String());

    //! Destructor.

    virtual ~Text_Edit();

    //! Set the text.

    void set(const String &);

    //! Get the text.

    const String & get() const;

    //! This signal is emitted when the text is changed.

    Signal<const String &> signal;

    //! This signal is emitted when the text is changed.

    Signal<const String &> action_signal;

    //! This signal is emitted when the text is changed.

    Signal<const String &> change_signal;

    virtual void color(Fl_Color);

    virtual Fl_Color color() const;

    virtual void text_font(Fl_Font);

    virtual void text_color(Fl_Color);

    virtual Fl_Font text_font() const;

    virtual Fl_Color text_color() const;

    virtual int handle(int);

    virtual const Fl_Widget * fltk_widget() const;

    virtual void size_string(const String &);

    virtual const String & size_string() const;

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    DJV_FL_WIDGET_CALLBACK(Text_Edit, widget_callback);

    void widget_update();

    struct Input;

    String  _value;
    String  _size_string;
    Input * _widget;
};

} // djv

#endif // DJV_TEXT_EDIT_H

