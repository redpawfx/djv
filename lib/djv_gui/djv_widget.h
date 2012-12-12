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

//! \file djv_widget.h

#ifndef DJV_WIDGET_H
#define DJV_WIDGET_H

#include <djv_abstract_widget.h>

#include <djv_memory_buffer.h>

#include <FL/Fl_Group.H>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Widget
//!
//! This class provides a widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Widget :
    public Fl_Group,
    public Abstract_Widget
{
public:

    //! Constructor.

    Widget();

    //! Destructor.

    virtual ~Widget();

    //! Set the highlight.

    void highlight(bool);

    //! Get the highlight.

    bool highlight() const;

    virtual void tooltip(const String &);

    inline virtual const String & tooltip() const;

    virtual int handle(int);

    virtual void show();

    virtual void draw();

    virtual const Fl_Widget * fltk_widget() const
    {
        return this;
    }

    virtual void dirty();
    virtual void geom(const Box2i &);
    inline virtual void resize(int, int, int, int);

    inline virtual const Box2i & geom() const;
    inline virtual int visible() const;

    virtual void size(SIZE, SIZE);
    virtual void align(ALIGN, ALIGN);

    inline const SIZE * size() const;
    inline const ALIGN * align() const;

    //! Debug the widget children.

    static void debug_children(const Fl_Group * in, int indent = 0);

private:

    Memory_Buffer<char> _tooltip_cstr;
    bool                _highlight;
    bool                _visible;
};

} // djv

#include <djv_widget_inline.h>

#endif // DJV_WIDGET_H

