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

//! \file djv_text_display.h

#ifndef DJV_TEXT_DISPLAY_H
#define DJV_TEXT_DISPLAY_H

#include <djv_abstract_text_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Text_Display
//!
//! This class provides a widget for displaying text. Unlike a label, this
//! works with copy and paste.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Text_Display : public Abstract_Text_Widget
{
public:

    //! Constructor.

    Text_Display(const String & = String());

    //! Destructor.

    virtual ~Text_Display();

    //! Set the text.

    void set(const String &);

    //! Get the text.

    const String & get() const;

    virtual void text_font(Fl_Font);

    virtual void text_color(Fl_Color);

    virtual Fl_Font text_font() const;

    virtual Fl_Color text_color() const;

    virtual void size_string(const String &);

    virtual const String & size_string() const;

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    void widget_update();

    class Widget;

    String   _value;
    String   _size_string;
    Widget * _widget;
};

} // djv

#endif // DJV_TEXT_DISPLAY_H

