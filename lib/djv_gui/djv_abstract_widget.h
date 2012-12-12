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

//! \file djv_abstract_widget.h

#ifndef DJV_ABSTRACT_WIDGET_H
#define DJV_ABSTRACT_WIDGET_H

#include <djv_callback.h>
#include <djv_layout.h>

#include <FL/Fl.H>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Abstract_Widget
//!
//! This class provides the base functionality for widgets.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Abstract_Widget : public Layout_Item, public Callback
{
public:

    //! Constructor.

    Abstract_Widget();

    //! Destructor.

    virtual ~Abstract_Widget() = 0;

    //! Destroy the widget.

    virtual void del();

    //! Set the text font.

    virtual void text_font(Fl_Font);

    //! Set the text color.

    virtual void text_color(Fl_Color);

    //! Get the text font.

    inline virtual Fl_Font text_font() const;

    //! Get the text color.

    inline virtual Fl_Color text_color() const;

    //! Set the tooltip.

    virtual void tooltip(const String &);

    //! Get the tooltip.

    inline virtual const String & tooltip() const;

    //! Event handling.

    virtual int handle(int) = 0;

    //! Show the widget.

    virtual void show() = 0;

    //! Draw the widget.

    virtual void draw() = 0;

    //! Is the mouse inside the widget?

    inline bool inside() const;

    //! Set whether arrow keys are used for navigation.

    void arrow_navkeys(bool);

    //! Get whether arrow keys are used for navigation.

    bool arrow_navkeys() const;

    //! Get the associated FLTK widget.

    virtual const Fl_Widget * fltk_widget() const = 0;

    virtual void dirty();

    virtual void layout(Layout *);

    virtual void geom(const Box2i &) = 0;

    virtual void resize(int, int, int, int) = 0;

    inline virtual const Box2i & geom() const;

    inline virtual Layout * layout() const;

    virtual int visible() const = 0;

private:

    Fl_Font  _text_font;
    Fl_Color _text_color;
    String   _tooltip;
    bool     _inside;
    bool     _arrow_navkeys;

    std::auto_ptr<Layout> _layout;

    bool _del;
};

} // djv

#include <djv_abstract_widget_inline.h>

#endif // DJV_ABSTRACT_WIDGET_H

