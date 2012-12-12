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

//! \file djv_row_layout.h

#ifndef DJV_ROW_LAYOUT_H
#define DJV_ROW_LAYOUT_H

#include <djv_layout.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Abstract_Row_Layout
//!
//! This class provides the base functionality for row layouts.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Abstract_Row_Layout : public Layout
{
public:

    //! Constructor.

    Abstract_Row_Layout(Layout_Item * parent = 0);

    //! Destructor.

    virtual ~Abstract_Row_Layout() = 0;

    //! Add a spacer.

    Layout_Item * add_spacer(
        const V2i & = V2i(-1),
        bool stretch = false);

    //! Add stretch.

    Layout_Item * add_stretch(bool stretch = true);

    //! Set the stretch item.

    void stretch(int);

    //! Set the stretch item.

    void stretch(const Layout_Item *);

    //! Get the stretch item.

    int stretch() const;

protected:

    enum DIRECTION
    {
        HORIZONTAL,
        VERTICAL
    };

    static void layout(
        const List<Layout_Item *> &,
        List<Box2i> *,
        const V2i & size,
        DIRECTION,
        int spacing,
        int stretch);

private:

    int _stretch;
};

//------------------------------------------------------------------------------
//! \class Horizontal_Layout
//!
//! This class provides a horizontal layout.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Horizontal_Layout : public Abstract_Row_Layout
{
public:

    //! Constructor.

    Horizontal_Layout(Layout_Item * parent = 0);

    //! Add a spacer.

    void add_spacer(int = -1, bool stretch = false);

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    Box2i       _geom_prev;
    List<Box2i> _list;
};

//------------------------------------------------------------------------------
//! \class Vertical_Layout
//!
//! This class provides a vertical layout.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Vertical_Layout : public Abstract_Row_Layout
{
public:

    //! Constructor.

    Vertical_Layout(Layout_Item * parent = 0);

    //! Add a spacer.

    void add_spacer(int = -1, bool stretch = false);

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    Box2i       _geom_prev;
    List<Box2i> _list;
};

} // djv

#endif // DJV_ROW_LAYOUT_H
