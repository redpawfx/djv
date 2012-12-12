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

//! \file djv_directory.h

#ifndef DJV_DIRECTORY_H
#define DJV_DIRECTORY_H

#include <djv_file.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Directory
//!
//! This class provides access to file-system directories.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Directory
{
public:

    //! Constructor.

    Directory();

    //! Constructor.

    Directory(const String &);

    //! Set the directory.

    void set(const String &);

    //! Get the directory.

    const String & get() const;

    //! Get a list of items in the directory.

    List<File> list() const;

    //! Change directory.

    void cd(const String &);

    //! Go up a directory.

    void up();

    //! Is the directory valid?

    bool is_valid() const;

    //! Get the user's home directory.

    static String home();

    //! Get the user's desktop directory.

    static String desktop();

    //! Get a list of drives (Windows specific).

    static List<String> drives();

    //! Get the root directory.

    static String root();

private:

    void update();

    String _path;

    bool _is_valid;
};

} // djv

#endif // DJV_DIRECTORY_H

