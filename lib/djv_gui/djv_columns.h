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

//! \file djv_columns.h

#ifndef DJV_COLUMNS_H
#define DJV_COLUMNS_H

#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Columns
//!
//! This class provides a widget for displaying columns of information.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Columns : public Widget
{
public:

    //! Constructor.

    Columns();

    //! Destructor.

    virtual ~Columns();

    //! Set the name list.

    void names(const List<String> &);

    //! Set the size list.

    void sizes(const List<int> &);

    //! Get the name list.

    List<String> names() const;

    //! Get the size list.

    List<int> sizes() const;

    //! This signal is emitted when the size is changed.

    Signal<const List<int> &> signal;

    //! This signal is emitted when a column header is clicked.

    Signal<int> action_signal;

    //! Set the sorting column.

    void sort(int);

    //! Get the sorting column.

    int sort() const;

    //! Set whether the sort is reversed.

    void sort_reverse(bool);

    //! Get whether the sort is reversed.

    bool sort_reverse() const;

    virtual int handle(int);

    virtual void draw();

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    struct Column
    {
        String name;
        int    column;
        int    size_hint;
    };

    static int handle_push(int, const List<Column> &);
    static int handle_drag(int, const List<Column> &);

    void columns_update();
    void widget_update();

    List<Column> _columns;
    int          _sort;
    bool         _sort_reverse;
    int          _push;
    int          _drag;
};

} // djv

#endif // DJV_COLUMNS_H

