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

//! \file djv_shortcut.h

#ifndef DJV_SHORTCUT_H
#define DJV_SHORTCUT_H

#include <djv_gui_export.h>

#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Shortcut
//!
//! This class provides a keyboard shortcut.
//------------------------------------------------------------------------------

struct DJV_GUI_EXPORT Shortcut
{
    //! Constructor.

    Shortcut(const String & name = String(), int value = 0) :
        name(name),
        value(value)
    {}

    String name;
    int    value;

    bool operator == (const Shortcut &) const;
    bool operator != (const Shortcut &) const;

    static bool   serialize(String *, int *);
    static String serialize(int);

    static String               label(int);
    static const List<String> & label_key();
    static const List<String> & label_mod();
    static const List<String> & label_shortcut();
};

} // djv

#endif // DJV_SHORTCUT_H
