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

//! \file djv_prefs.h

#ifndef DJV_PREFS_H
#define DJV_PREFS_H

#include <djv_gui_export.h>

#include <djv_time.h>

class Fl_Preferences;

namespace djv
{

//------------------------------------------------------------------------------
//! \class Prefs
//!
//! This class provides preferences.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Prefs
{
public:

    //! Constructor.

    Prefs(const String & name);

    //! Constructor.

    Prefs(const Prefs &, const String & group);

    //! Constructor.

    Prefs(Prefs *, const String & group);

    //! Destructor.

    ~Prefs();

    //! Get the name.

    const String & name() const;

    //! Set a preference.

    void set(const String &, const String &);

    //! Set a preference.

    template<typename T>
    static inline void set_(Prefs *, const String &, const T &);

    //! Remove a preference.

    void del(const String &);

    //! Remove a preference group.

    void del_group(const String &);

    //! Get a preference.

    bool get(const String &, String *) const;

    //! Get a preference.

    template<typename T>
    static inline bool get_(Prefs *, const String &, T *);

    //! Get the list of preferences.

    List<String> list() const;

    //! Get the list of preference groups.

    List<String> group() const;

    //! Get the file name.

    String file_name() const;

    //! Get the file path.

    String file_path() const;

    //! Get the file base name.

    static String file_base();

    //! Get the global preferences.

    static Prefs * prefs();

    //! Get the global preferences.

    static Prefs * prefs_global();

private:

    String _name;

    std::auto_ptr<Fl_Preferences> _prefs;
};

} // djv

#include <djv_prefs_inline.h>

#endif // DJV_PREFS_H

