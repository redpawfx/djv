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

//! \file djv_application_message_dialog.h

#ifndef DJV_APPLICATION_MESSAGE_DIALOG_H
#define DJV_APPLICATION_MESSAGE_DIALOG_H

#include <djv_dialog.h>

#include <djv_error.h>

namespace djv
{

class Check_Button;
class Multiline_Text_Display;
class Push_Button;

//------------------------------------------------------------------------------
//! \class Application_Message_Dialog
//!
//! This class provides a dialog for displaying application messages.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Application_Message_Dialog : public Dialog
{
public:

    //! Constructor.

    Application_Message_Dialog();

    //! Destructor.

    virtual ~Application_Message_Dialog();

    virtual void del();

    //! Add a message to the dialog.

    void set(const String &);

    //! Add an error to the dialog.

    void set(const Error &);

    //! Clear the dialog.

    void clear();

    virtual void show();

    virtual void dirty();

    virtual const V2i & size_min() const;

    //! Add a message to the global dialog.

    static void message(const String &);

    //! Add an error to the global dialog.

    static void error(const Error &);

    //! Get the global dialog.

    static Application_Message_Dialog * global();

private:

    DJV_CALLBACK(Application_Message_Dialog, show_callback, bool);
    DJV_CALLBACK(Application_Message_Dialog, clear_callback, bool);
    DJV_CALLBACK(Application_Message_Dialog, close_callback, bool);

    void widget_update();
    void dialog_update();

    List<String>             _list;
    bool                     _show;
    V2i                      _size_min;
    Multiline_Text_Display * _widget;
    Check_Button *           _show_widget;
    Push_Button *            _clear_widget;
    Push_Button *            _close_widget;
};

} // djv

#endif // DJV_APPLICATION_MESSAGE_DIALOG_H

