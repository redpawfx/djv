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

//! \file djv_tabs.h

#ifndef DJV_TABS_H
#define DJV_TABS_H

#include <djv_abstract_widget.h>

#include <FL/Fl_Tabs.H>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Tabs
//!
//! This class provides a tabs widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Tabs : public Fl_Tabs, public Abstract_Widget
{
public:

    //! Constructor.

    Tabs();

    //! Destructor.

    virtual ~Tabs();

    //! Set the current tab.

    void set(int);

    //! Get the current tab.

    int get() const;

    //! This signal is emitted when the current tab is changed.

    Signal<int> signal;

    //! Add a tab.

    void tab_add(const String & name, Abstract_Widget *);

    //! Insert a tab.

    void tab_insert(const String & name, Abstract_Widget *, int = 0);

    //! Set the tab name.

    void tab_name(const String & name, int tab);

    //! Remove a tab.

    void tab_del(int tab);

    //! Remove all tabs.

    void tab_del();

    //! Get the number of tabs.

    int tab_size() const;

    virtual int handle(int);

    virtual void show();

    virtual void draw();

    virtual const Fl_Widget * fltk_widget() const
    {
        return this;
    }

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual void resize(int, int, int, int);

    virtual const Box2i & geom() const;

    virtual int visible() const;

private:

    DJV_FL_WIDGET_CALLBACK(Tabs, widget_callback);

    List<Abstract_Widget *> _list; //! \todo These should be layout items.
    V2i                     _size_tab;
};

} // djv

#endif

