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

//! \file djv_core_application.h

#ifndef DJV_CORE_APPLICATION_H
#define DJV_CORE_APPLICATION_H

#include <djv_debug.h>
#include <djv_error.h>
#include <djv_string.h>

namespace djv
{

class Gl_Context;

//------------------------------------------------------------------------------
//! \class Core_Application
//!
//! This class provides the core functionality for applications.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Core_Application
{
public:

    //! Constructor.

    Core_Application(const String & name, int & argc, char ** argv)
        throw (Error);

    //! Destructor.

    virtual ~Core_Application();

    //! Exit value.

    enum EXIT
    {
        EXIT_DEFAULT,
        EXIT_ERROR,
        EXIT_HELP,
        EXIT_INFO,
        EXIT_ABOUT
    };

    //! Start the application.

    virtual int exec();

    //! Set the exit value.

    virtual void exit(EXIT);

    //! Get the exit value.

    EXIT exit() const;

    //! Get the application name.

    static const String & name();

    //! Get the application path.

    const String & path() const;

    //! Get the documentation path.

    const String & path_doc() const;

    //! Get application information.

    virtual String info() const;

    //! Get information about the application.

    virtual String about() const;

    //! Print a message.

    virtual void message(const String &) const;

    //! Print an error.

    virtual void error(const Error &) const;

    //! Print.

    virtual void print(const String &, bool newline = true) const;

    //! Print a separator.

    virtual void print_separator() const;

    //! Parse the comamand line.

    void command_line(String &) throw (Error);

    //! Get the command line name.

    const String & command_line_name() const;

    //! Get the command line arguments.

    const String & command_line_args() const;

    //! Get the command line help.

    virtual String command_line_help() const;

    //! Get the default OpenGL context.

    Gl_Context * context();

    //! Get the global application instance.

    static Core_Application * global();

protected:

    static const String error_command_line;

private:

    EXIT          _exit;
    static String _name;
    String        _path;
    String        _path_doc;
    String        _command_line_name;
    String        _command_line_args;
    bool          _endline;
    bool          _separator;
    Gl_Context *  _context;
};

//! Get the global application instance.

#define DJV_CORE_APP djv::Core_Application::global()

} // djv

#endif // DJV_CORE_APPLICATION_H

