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

//! \file djv_layout.h

#ifndef DJV_LAYOUT_H
#define DJV_LAYOUT_H

#include <djv_layout_item.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Layout
//!
//! This class provides a layout.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Layout : public Layout_Item
{
public:

    //! Constructor.

    Layout(Layout_Item * parent = 0);

    //! Destructor.

    virtual ~Layout() = 0;

    //! Add an item.

    virtual void add(Layout_Item *);

    //! Insert an item.

    virtual void insert(Layout_Item *, int);

    //! Remove an item.

    virtual Layout_Item * remove(int);

    //! Remove an item.

    virtual Layout_Item * remove(Layout_Item *);

    //! Remove an item.

    virtual Layout_Item * remove_child(Layout_Item *);

    //! Destroy an item.

    virtual void del(int);

    //! Destroy an item.

    virtual void del(Layout_Item *);

    //! Destroy all items.

    virtual void del();

    //! Get the list of items.

    inline const List<Layout_Item *> & list() const;

    //! Set the spacing.

    void spacing(int);

    //! Get the spacing.

    int spacing() const;

    //! Set the margin.

    void margin(int);

    //! Get the margin.

    int margin() const;

    virtual void dirty();

    virtual int visible() const;

    //! Get the window handle size.

    static int window_handle_size();

    //! Debug the layout.

    void debug(int indent = 0);

private:

    List<Layout_Item *> _list;

    int _spacing;
    int _margin;
};

} // djv

#include <djv_layout_inline.h>

#endif // DJV_LAYOUT_H

