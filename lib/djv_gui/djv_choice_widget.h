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

//! \file djv_choice_widget.h

#ifndef DJV_CHOICE_WIDGET_H
#define DJV_CHOICE_WIDGET_H

#include <djv_frame.h>

namespace djv
{

class Vertical_Layout;

//------------------------------------------------------------------------------
//! \class Choice_Widget
//!
//! This class provides a widget for selecting an item from a list.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Choice_Widget : public Frame
{
public:

    //! Constructor.

    Choice_Widget();

    //! Constructor.

    Choice_Widget(const List<String> & list);

    //! Constructor.

    Choice_Widget(
        const List<String> & list,
        const List<String> & text);

    //! Destructor.

    virtual ~Choice_Widget();

    //! Set the current item.

    void set(int);

    //! Get the current item.

    int get() const;

    //! Set the items.

    void list(const List<String> &);

    //! Get the items.

    const List<String> & list() const;

    //! This signal is emitted when the item is changed.

    Signal<int> signal;

    //! Set the item labels.

    void text(const List<String> &);

    //! Get the item labels.

    const List<String> & text() const;

    virtual void draw();

    virtual const Fl_Widget * fltk_widget() const;

private:

    virtual void down(bool);
    virtual void select(bool);

    void init(
        const List<String> & list,
        const List<String> & text);

    DJV_CALLBACK(Choice_Widget, widget_callback, int);

    class _Menu_Button;

    List<String>      _list;
    List<String>      _text;
    bool              _down;
    bool              _select;
    _Menu_Button *    _widget;
    Vertical_Layout * _layout;

    friend class _Menu_Button;
};

} // djv

#endif // DJV_CHOICE_WIDGET_H
