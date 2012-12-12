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

//! \file djv_layout_item.h

#ifndef DJV_LAYOUT_ITEM_H
#define DJV_LAYOUT_ITEM_H

#include <djv_gui_export.h>

#include <djv_box.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Layout_Item
//!
//! This class provides a layout item.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Layout_Item
{
public:

    //! Item type.

    enum ITEM
    {
        WIDGET,
        LAYOUT,
        SPACER
    };

    //! Constructor.

    Layout_Item(ITEM);

    //! Destructor.

    virtual ~Layout_Item() = 0;

    //! Get the item type.

    inline ITEM item() const;

    //! Dirty the item.

    virtual void dirty();

    //! Update the item

    virtual void update();

    //! Set the geometry.

    virtual void geom(const Box2i &);

    //! Get the geometry.

    inline virtual const Box2i & geom() const;

    //! Get the size hint.

    inline virtual const V2i & size_hint() const;

    //! Get the minimum size.

    inline virtual const V2i & size_min() const;

    //! Get the item visiblity.

    inline virtual int visible() const;

    //! Item size.

    enum SIZE
    {
        FIXED,
        STRETCH
    };

    //! Set the item size.

    virtual void size(SIZE, SIZE);

    //! Get the item size.

    inline const SIZE * size() const;

    //! Item alignment.

    enum ALIGN
    {
        LEFT,
        CENTER,
        RIGHT
    };

    //! Set the item alignment.

    virtual void align(ALIGN, ALIGN);

    //! Get the item alignment.

    inline const ALIGN * align() const;

    //! Set the item group.
    //!
    //! \todo This is a cheesy way to align items.

    void group(const List<Layout_Item *> &);

    //! Get the item group.

    inline const List<Layout_Item *> & group() const;

    //! Get the item group alignment.

    inline virtual Layout_Item * group_align();

    //! Get the item group label alignment.

    inline virtual Layout_Item * label_align();

    //! Create an item group.

    static void create_group(const List<Layout_Item *> &);

    //! Set the parent.

    virtual void parent(Layout_Item *);

    //! Get the parent.

    inline Layout_Item * parent() const;

    //! Set the debugging path.

    void debug_path(const String &);

    //! Get the debugging path.

    String debug_path() const;

    //! Create a debugging path.

    static String debug_path(const Layout_Item *);

protected:

    void size_hint(const V2i &);

private:

    ITEM                _item;
    Box2i               _geom;
    V2i                 _size_hint;
    V2i                 _size_min;
    SIZE                _size [2];
    ALIGN               _align [2];
    List<Layout_Item *> _group;
    Layout_Item *       _parent;
    String              _debug_path;
};

//------------------------------------------------------------------------------
//! \class Layout_Spacer
//!
//! This class provides a layout spacer.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Layout_Spacer : public Layout_Item
{
public:

    //! Constructor.

    Layout_Spacer(const V2i & size = V2i());

    //! Set the spacer size.

    void spacer_size(const V2i &);

    //! Get the spacer size.

    const V2i & spacer_size() const;

    virtual void dirty();

private:

    V2i _spacer_size;
};

} // djv

#include <djv_layout_item_inline.h>

#endif // DJV_LAYOUT_ITEM_H

