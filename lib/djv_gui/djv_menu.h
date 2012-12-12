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

//! \file djv_menu.h

#ifndef DJV_MENU_H
#define DJV_MENU_H

#include <djv_frame.h>

#include <FL/Fl_Menu_.H>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Menu_Item
//!
//! This class provides a menu item.
//------------------------------------------------------------------------------

struct DJV_GUI_EXPORT Menu_Item
{
    static const int item_label_max_size;

    enum
    {
        INACTIVE = 2 << 0,
        TOGGLE   = 2 << 1,
        RADIO    = 2 << 2,
        VALUE    = 2 << 3,
        SUB_MENU = 2 << 4,
        DIVIDER  = 2 << 5
    };

    //! Constructor.

    Menu_Item();

    //! Constructor.

    Menu_Item(
        const String & label,
        int            shortcut,
        Fl_Callback *  callback,
        void *         data,
        int            flags);

    String              label;
    Memory_Buffer<char> cstr;
    int                 shortcut;
    Fl_Callback *       callback;
    void *              data;
    int                 flags;
};

//------------------------------------------------------------------------------
//! \class Menu_Item_Group
//!
//! This class provides a group of menu items.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Menu_Item_Group
{
public:

    //! Constructor.

    Menu_Item_Group();

    //! Constructor.

    Menu_Item_Group(const Menu_Item_Group &);

    //! Destructor.

    ~Menu_Item_Group();

    Menu_Item_Group & operator = (const Menu_Item_Group &);

    //! Copy from another item group.

    void set(const Menu_Item_Group &);

    //! Add an item.

    int add(
        const String & label,
        int shortcut = 0,
        Fl_Callback * = 0,
        void * = 0,
        int flags = 0,
        bool value = false);

    //! Add an item.

    List<int> add(
        const List<String> & label,
        const List<int> & shortcut = List<int>(),
        Fl_Callback * = 0,
        void * = 0,
        int flags = 0,
        int value = -1);

    //! End a group.

    void end();

    //! Remove all items.

    void del();

    //! Get the items.

    const List<Menu_Item *> & get() const;

    //! Create FLTK menu items.

    void fl(Memory_Buffer<Fl_Menu_Item> *) const;

private:

    List<Menu_Item *> _list;
};

//------------------------------------------------------------------------------
//! \class Menu
//!
//! This class provides a menu widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Menu : public Frame
{
public:

    //! Constructor.

    Menu();

    //! Set the item group.

    void set(const Menu_Item_Group &);

    //! Get the item group.

    const Menu_Item_Group & get() const;

    //! Get the current selection.

    int item() const;

    //! Get the current selection toggle value.

    bool value() const;

    //! Test a shortcut.

    int test_shortcut();

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    class Widget;

    Menu_Item_Group _group;
    Widget *        _widget;
    bool            _test_shortcut;
};

//------------------------------------------------------------------------------
//! \class Popup_Menu
//!
//! This class provides a popup-menu widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Popup_Menu : public Frame
{
public:

    //! Constructor.

    Popup_Menu(const List<String> & = List<String>());

    //! Set the items.

    void set(const List<String> &);

    //! Set the item group.

    void set(const Menu_Item_Group &);

    //! Get the item group.

    const Menu_Item_Group & get() const;

    //! Get the current item.

    int value() const;

    //! This signal is emitted when the user chooses an item.

    Signal<int> signal;

    virtual void draw();

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    void down(bool);
    void select(bool);

    DJV_FL_WIDGET_CALLBACK(Popup_Menu, widget_callback);

    Menu_Item_Group _group;

    class Widget;
    Widget * _widget;
    friend class Widget;

    Fl_Image * _icon;

    bool _down;
    bool _select;
};

} // djv

#endif // DJV_MENU_H
