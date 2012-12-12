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

//! \file djv_view_shortcut_prefs.h

#ifndef DJV_VIEW_SHORTCUT_PREFS_H
#define DJV_VIEW_SHORTCUT_PREFS_H

#include <djv_shortcut.h>
#include <djv_widget.h>

#if defined(DJV_WINDOWS)
#undef FILE_OPEN //! \todo Windows name-space pollution.
#endif

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Shortcut_Prefs
//!
//! This class provides keyboard shortcut preferences.
//------------------------------------------------------------------------------

class Shortcut_Prefs
{
public:

    //! Constructor.

    Shortcut_Prefs();

    //! Destructor.

    ~Shortcut_Prefs();

    //! Set the shortcuts.

    void shortcuts(const List<djv::Shortcut> &);

    //! Get the shortcuts.

    const List<djv::Shortcut> & shortcuts() const;

    //! Get the global preferences.

    static Shortcut_Prefs * global();

private:

    List<djv::Shortcut> _shortcuts;
};

//------------------------------------------------------------------------------
//! \class Shortcut_Prefs_Widget
//!
//! This class provides a keyboard shortcut preferences widget.
//------------------------------------------------------------------------------

class Shortcut_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    Shortcut_Prefs_Widget();

private:

    DJV_CALLBACK(Shortcut_Prefs_Widget, shortcut_callback,
                 const List<djv::Shortcut> &);
};

} // djv_view

#endif // DJV_VIEW_SHORTCUT_PREFS_H

