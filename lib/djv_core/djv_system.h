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

//! \file djv_system.h

#ifndef DJV_SYSTEM_H
#define DJV_SYSTEM_H

#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct System
//!
//! This struct provides system utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT System
{
    //! Get system information.

    static String info();

    //! Get the search path. The default search path consists of:
    //!
    //! - The DJV_PATH environment variable
    //! - The LD_LIBRARY_PATH environment variable
    //! - The application path
    //! - The current directory

    static List<String> search_path();

    //! Get the width of the terminal.

    static int terminal_width();

    //! Print a message to the terminal.

    static void print(const String &, bool newline = true);

    //! Execute a command.

    static int exec(const String &);

    //! Open an URL in an external web-browser.

    static bool open_url(const String &);

    //! Set an environment variable.

    static bool env(const String &, const String &);

    //! Get an environment variable.

    static String env(const String &);

    //! Labels.

    static const String
    label_info;
};

} // djv

#endif // DJV_SYSTEM_H

