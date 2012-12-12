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

//! \file djv_ls.h

#ifndef DJV_LS_H
#define DJV_LS_H

#include <djv_core_application.h>
#include <djv_file.h>

//! \namespace djv_ls
//!
//! This program provides a command line tool for listing directories with file
//! sequences.

namespace djv_ls
{

using namespace djv;

//------------------------------------------------------------------------------
//! \class Application
//!
//! This class provides the application.
//------------------------------------------------------------------------------

class Application : public Core_Application
{
public:

    //! Constructor.

    Application(int, char **);

    //! Destructor.

    virtual ~Application();
    
    //! Parse the command line.

    void command_line(String &) throw (Error);

    virtual String command_line_help() const;

private:

    void process(List<File> &);

    void print_file(const File &, bool path = false, bool info = true);

    bool print_directory(const File &, bool label);

    List<String>    _input;
    bool            _info;
    bool            _file_path;
    Seq::COMPRESS   _seq;
    bool            _recurse;
    bool            _hidden;
    List<String>    _filter;
    int             _columns;
    File_Util::SORT _sort;
    bool            _reverse_sort;
    bool            _x_sort_dirs;
};

} // djv_ls

#endif // DJV_LS_H

