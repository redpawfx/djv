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

//! \file djv_progress_dialog.h

#ifndef DJV_PROGRESS_DIALOG_H
#define DJV_PROGRESS_DIALOG_H

#include <djv_message_dialog.h>

namespace djv
{

class Label;

//------------------------------------------------------------------------------
//! \class Progress_Dialog
//!
//! This class provides a progress bar dialog.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Progress_Dialog : public Abstract_Message_Dialog
{
public:

    //! Constructor.

    Progress_Dialog();

    //! Destructor.

    virtual ~Progress_Dialog();

    virtual void del();

    //! Set the progress.

    void set(int);

    //! This signal is emitted when the progress is changed.

    Signal<int> signal;

    //! This signal is emitted when the progress is finished or cancelled.

    Signal<bool> del_signal;

    //! Set the label.

    void label(const String &);

    //! Get the label.

    const String & label() const;

    //! Show the global dialog.

    static void dialog(
        const String & label,
        int value,
        Callback *,
        Signal<int>::Callback_Fnc * callback,
        Signal<bool>::Callback_Fnc * del_callback);

    //! Remove the global dialog.

    static void dialog_del(Callback *);

private:

    DJV_CALLBACK(Progress_Dialog, cancel_callback, bool);
    DJV_FL_CALLBACK(Progress_Dialog, idle_callback);

    class Widget;

    int      _value;
    int      _current;
    Timer    _time;
    double   _time_accum;
    Timer    _elapsed;
    Widget * _widget;
    Label *  _label;
    Label *  _estimate_label;
    Label *  _elapsed_label;
};


} // djv

#endif // DJV_PROGRESS_DIALOG_H
