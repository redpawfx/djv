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

//! \file djv_application.h

#ifndef DJV_APPLICATION_H
#define DJV_APPLICATION_H

#include <djv_callback.h>

#include <djv_core_application.h>

class Fl_Widget;

namespace djv
{

class Abstract_Widget;
class Window;

//------------------------------------------------------------------------------
//! \class Application
//!
//! This class provides the base functionality for applications.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Application : public Core_Application, public Callback
{
public:

    //! Constructor.

    Application(const String & name, int & argc, char ** argv) throw (Error);

    //! Destructor.

    virtual ~Application();

    //! Set whether the user-interface has started.

    void is_valid(bool);

    //! Get whether the user-interface has started.

    bool is_valid() const;

    //! Add a widget.

    void widget_add(Abstract_Widget *);

    //! Remove a widget.

    void widget_del(Abstract_Widget *);

    //! Add a window.

    void window_add(Window *);

    //! Remove a window.

    void window_del(Window *);

    //! Focus a window.

    void window_focus(Window *);

    //! Update all windows.

    void window_update();

    //! Redraw all windows.

    void window_redraw();

    //! Hide all windows.

    void window_hide();

    //! Get a widget from a FLTK widget.

    const Abstract_Widget * widget_get(const Fl_Widget *);

    //! This signal is emitted when a window is added.

    Signal<Window *> window_add_signal;

    //! This signal is emitted when a window is removed.

    Signal<Window *> window_del_signal;

    //! This signal is emitted when a window is focused.

    Signal<Window *> window_focus_signal;

    virtual int exec();

    virtual void exit(EXIT);

    virtual EXIT exit() const;

    virtual String info() const;

    virtual void message(const String &) const;

    virtual void error(const Error &) const;

    virtual void help() const;

private:

    bool _is_valid;

    List<Abstract_Widget *> _widgets;
    List<Window *>          _windows;
};

//! Get the global application instance.

#define DJV_APP \
  static_cast<Application*>(djv::Core_Application::global())

} // djv

#endif // DJV_APPLICATION_H
