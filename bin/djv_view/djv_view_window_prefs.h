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

//! \file djv_view_window_prefs.h

#ifndef DJV_VIEW_WINDOW_PREFS_H
#define DJV_VIEW_WINDOW_PREFS_H

#include <djv_view_window.h>

#include <djv_widget.h>

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Window_Prefs
//!
//! This class provides main window preferences.
//------------------------------------------------------------------------------

class Window_Prefs : public Callback
{
public:

    //! Constructor.

    Window_Prefs();

    //! Destructor.

    virtual ~Window_Prefs();

    //! Set whether resize fit is enabled.

    void resize_fit(bool);

    //! Get whether resize fit is enabled.

    bool resize_fit() const;

    //! Set the maximum resize amount.

    void resize_max(Window::RESIZE_MAX);

    //! Get the maximum resize amount.

    Window::RESIZE_MAX resize_max() const;

    //! Set the toolbar visibility.

    void toolbar(const List<bool> &);

    //! Get the toolbar visibility.

    const List<bool> & toolbar() const;

    //! This signal is emitted when the toolbar visibility is changed.

    Signal<const List<bool> &> toolbar_signal;

    //! Get global preferences.

    static Window_Prefs * global();

private:

    Box2i              _geom;
    bool               _resize_fit;
    Window::RESIZE_MAX _resize_max;
    List<bool>         _toolbar;
};

//------------------------------------------------------------------------------
//! \class Window_Prefs_Widget
//!
//! This class provides a main window preferences widget.
//------------------------------------------------------------------------------

class Window_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    Window_Prefs_Widget();

private:

    DJV_CALLBACK(Window_Prefs_Widget, resize_fit_callback, bool);
    DJV_CALLBACK(Window_Prefs_Widget, resize_max_callback, int);
    DJV_CALLBACK(Window_Prefs_Widget, toolbar_callback, const List<bool> &);
};

} // djv_view

#endif // DJV_VIEW_WINDOW_PREFS_H

