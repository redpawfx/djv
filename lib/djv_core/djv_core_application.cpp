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

//! \file djv_core_application.cpp

#include <djv_core_application.h>

#include <djv_file.h>
#include <djv_gl_context.h>
#include <djv_gl_image.h>
#include <djv_image_io.h>
#include <djv_math.h>
#include <djv_memory.h>
#include <djv_system.h>

#include <FL/Fl.H>

namespace djv
{

//------------------------------------------------------------------------------
// application_path
//
// Utility function to find the application path.
//------------------------------------------------------------------------------

namespace
{

String application_path(const String & command_line_name)
{
    String out;

    // Try getting the path from the command line.

    if (out.empty())
    {
        out = File(command_line_name).path();

        if (! out.empty())
        {
            out = File_Util::path_absolute(out);
        }
    }

    // Try getting the path from:
    //
    // * The "PATH" environment variable.
    // * Current directory.

    if (out.empty())
    {
        static const List<String> path_env = List<String>() <<
            String_Util::split(System::env("PATH"), File_Util::list_separators);

        const List<String> list = List<String>() << path_env << ".";

        for (size_t i = 0; i < list.size(); ++i)
        {
            const String path = File_Util::path_absolute(list[i]);
            
            const String name =
                path + File_Util::path_separator + command_line_name;

            //! \todo Check for Windows specific file extensions.

            if (
                File(name).stat() ||
                File(name + ".exe").stat() ||
                File(name + ".com").stat())
            {
                out = path;
                
                break;
            }
        }
    }

    return File_Util::path_fix(out);
}

const String path_doc_ =
#if defined(DJV_WINDOWS)
    "../doc";
#elif defined(DJV_OSX)
    "../Resources/doc";
#else
    "../doc";
#endif

} // namespace

//------------------------------------------------------------------------------
// Core_Application
//------------------------------------------------------------------------------

namespace
{

// Global application pointer.

Core_Application * _application = 0;

} // namespace

String Core_Application::_name;

Core_Application::Core_Application(
    const String & name,
    int & argc,
    char ** argv) throw (Error):
    _exit     (EXIT_DEFAULT),
    _endline  (false),
    _separator(false),
    _context  (0)
{
    _application = this;

    _name = name;

    //DJV_DEBUG("Core_Application::Core_Application");
    //DJV_DEBUG_PRINT("name = " << _name);

    // Store the command line arguments.

    if (argc)
    {
        _command_line_name = argv[0];
    }

    for (int i = 1; i < argc; ++i)
    {
        _command_line_args << String(argv[i]);
    }

    //DJV_DEBUG_PRINT("command_line = " << _command_line_args);

    // Get the application path.

    _path = application_path(_command_line_name);

    _path_doc = File_Util::path_fix(_path + path_doc_);

    //DJV_DEBUG_PRINT("path = " << _path);
    //DJV_DEBUG_PRINT("path doc = " << _path_doc);

    // Create the default OpenGL context.

    _context = Gl_Context_Factory::create();

    //! Initialize the FLTK visual.

    Fl::visual(FL_DOUBLE | FL_RGB);

    //! Force image I/O plugin loading.

    Image_Io_Base_Factory::global();
    Image_Load_Factory::global();
    Image_Save_Factory::global();
}

Core_Application::~Core_Application()
{
    //DJV_DEBUG("Core_Application::~Core_Application");

    delete _context;
}

int Core_Application::exec()
{
    switch (_exit)
    {
        case EXIT_HELP:
            message("\n" + command_line_help());
            break;

        case EXIT_INFO:
            message("\n" + info());
            break;

        case EXIT_ABOUT:
            message("\n" + about());
            break;

        default:
            break;
    }

    return _exit;
}

void Core_Application::exit(EXIT in)
{
    _exit = in;
}

Core_Application::EXIT Core_Application::exit() const
{
    return _exit;
}

const String & Core_Application::name()
{
    return _name;
}

const String & Core_Application::path() const
{
    return _path;
}

const String & Core_Application::path_doc() const
{
    return _path_doc;
}

namespace
{

const String label_info =
" General\n"
"\n"
"     Version:     %%\n"
"     System:      %%\n"
"     Information: %%\n"
"     Endian:      %%\n"
"     Search Path: %%\n"
"\n"
" OpenGL\n"
"\n"
"     Vendor:   %%\n"
"     Renderer: %%\n"
"     Version:  %%\n"
"\n"
" Image I/O Plugins\n"
"\n"
"     Base: %%\n"
"     Load: %%\n"
"     Save: %%\n";

} // namespace

String Core_Application::info() const
{
    return String_Format(label_info).
        arg(DJV_VERSION_NAME).
        arg(DJV_SYSTEM_NAME).
        arg(System::info()).
        arg(String_Util::label(Memory::endian())).
        arg(System::search_path(), ", ").
        arg(_context->vendor()).
        arg(_context->renderer()).
        arg(_context->version()).
        arg(Image_Io_Base_Factory::global()->names(), ", ").
        arg(Image_Load_Factory::global()->names(), ", ").
        arg(Image_Save_Factory::global()->names(), ", ");
}

namespace
{

const String label_about =
" DJV Imaging %%\n"
"\n"
" http://djv.sourceforge.net\n"
"\n"
" Made in Berkeley, CA, USA\n"
"\n"
" Copyright (c) 2004-2012 Darby Johnston\n"
" All rights reserved.\n"
"\n"
" Redistribution and use in source and binary forms, with or without"
" modification, are permitted provided that the following conditions are"
" met:\n"
"\n"
" * Redistributions of source code must retain the above copyright notice,"
" this list of conditions, and the following disclaimer.\n"
"\n"
" * Redistributions in binary form must reproduce the above copyright"
" notice, this list of conditions, and the following disclaimer in the"
" documentation and/or other materials provided with the distribution.\n"
"\n"
" * Neither the names of the copyright holders nor the names of any"
" contributors may be used to endorse or promote products derived from this"
" software without specific prior written permission.\n"
"\n"
" THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS"
" \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT"
" LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A"
" PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER"
" OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,"
" EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,"
" PROCUREMENT OF SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR"
" PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF"
" LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING"
" NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"
" SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
"\n"
" Portions of this software are copyright (in alphabetical order):\n"
"\n"
"     Copyright (c) 2008 Alan Jones.\n"
"     All rights reserved.\n"
"\n"
"     Copyright (c) 2008-2009 Mikael Sundell\n"
"     All rights reserved.\n"
"\n"
" This software is based in part on the works of (in alphabetical order):\n"
"\n"
"     FLTK\n"
"     http://www.fltk.org\n"
"     Copyright 1998-2011 Bill Spitzak and others\n"
"\n"
"     GLEW\n"
"     http://glew.sourceforge.net\n"
"     Copyright (c) 2002-2007, Milan Ikits <milan ikits[]ieee org>\n"
"     Copyright (c) 2002-2007, Marcelo E. Magallon <mmagallo[]debian org>\n"
"     Copyright (c) 2002, Lev Povalahev\n"
"     All rights reserved.\n"
"\n"
"     libjpeg\n"
"     http://www.ijg.org\n"
"     Copyright (c) 1991-2012, Thomas G. Lane, Guido Vollbeding\n"
"\n"
"     libpng\n"
"     http://www.libpng.org\n"
"     Copyright (c) 2004, 2006-2011 Glenn Randers-Pehrson\n"
"\n"
"     libquicktime\n"
"     http://libquicktime.sourceforge.net\n"
"     Copyright (c) 2002 Heroine Virtual Ltd.\n"
"     Copyright (c) 2002-2007 Members of the libquicktime project\n"
"     LGPL License http://www.gnu.org/licenses/lgpl.html\n"
"\n"
"     libtiff\n"
"     http://www.libtiff.org\n"
"     Copyright (c) 1988-1997 Sam Leffler\n"
"     Copyright (c) 1991-1997 Silicon Graphics, Inc.\n"
"\n"
"     OpenEXR\n"
"     http://www.openexr.com\n"
"     Copyright (c) 2006, Industrial Light & Magic, a division of Lucasfilm"
" Entertainment Company Ltd.\n"
"\n"
"     PortAudio\n"
"     http://www.portaudio.com\n"
"     Copyright (c) 1999-2006 Ross Bencina and Phil Burk\n"
"\n"
"     Qt\n"
"     http://www.qtsoftware.com\n"
"     Copyright (c) 2008-2012 Nokia Corporation\n"
"\n"
"     zlib\n"
"     http://www.zlib.net/\n"
"     Copyright (c) 1995-2012 Jean-loup Gailly and Mark Adler\n";

} // namespace

String Core_Application::about() const
{
    return String_Format(label_about).
           arg(DJV_VERSION_NAME);
}

void Core_Application::message(const String & in) const
{
    print(in);
}

void Core_Application::error(const Error & in) const
{
    if (_endline)
    {
        System::print("");

        const_cast<Core_Application *>(this)->_endline = false;
    }

    Error_Util::print(in);
}

void Core_Application::print(const String & in, bool newline) const
{
    if (_separator)
    {
        System::print("");

        const_cast<Core_Application *>(this)->_separator = false;
    }

    System::print(in, newline);

    const_cast<Core_Application *>(this)->_endline = ! newline;
}

void Core_Application::print_separator() const
{
    const_cast<Core_Application *>(this)->_separator = true;
}

void Core_Application::command_line(String & in) throw (Error)
{
    //DJV_DEBUG("Core_Application::command_line");
    //DJV_DEBUG_PRINT("in = " << in);

    String tmp, arg;

    try
    {
        while (! in.empty())
        {
            in >> arg;

            // OpenGL options.

            if ("-render_filter" == arg)
            {
                Gl_Image_Filter value;
                in >> value;
                Gl_Image_Filter::default_filter = value;
            }

            // Image I/O options.

            else if ("-base" == arg)
            {
                String name;
                in >> name;

                const List<Plugin *> & plugins =
                    Image_Io_Base_Factory::global()->plugins();

                for (size_t i = 0; i < plugins.size(); ++i)
                {
                    if (Image_Io_Base * plugin =
                        dynamic_cast<Image_Io_Base *>(plugins[i]))
                    {
                        if (name == plugin->name())
                        {
                            String option;

                            if (! serialize(&in, &option))
                            {
                                throw in;
                            }

                            if (! plugin->option(option, &in))
                            {
                                throw in;
                            }

                            break;
                        }
                    }
                }
            }
            else if ("-load" == arg)
            {
                String name;
                in >> name;

                const List<Plugin *> & plugins =
                    Image_Load_Factory::global()->plugins();

                for (size_t i = 0; i < plugins.size(); ++i)
                {
                    if (Image_Load * plugin =
                        dynamic_cast<Image_Load *>(plugins[i]))
                    {
                        if (name == plugin->name())
                        {
                            String option;

                            if (! serialize(&in, &option))
                            {
                                throw in;
                            }

                            if (! plugin->option(option, &in))
                            {
                                throw in;
                            }

                            break;
                        }
                    }
                }
            }
            else if ("-save" == arg)
            {
                String name;
                in >> name;

                const List<Plugin *> & plugins =
                    Image_Save_Factory::global()->plugins();

                for (size_t i = 0; i < plugins.size(); ++i)
                {
                    if (Image_Save * plugin =
                        dynamic_cast<Image_Save *>(plugins[i]))
                    {
                        if (name == plugin->name())
                        {
                            String option;

                            if (! serialize(&in, &option))
                            {
                                throw in;
                            }

                            if (! plugin->option(option, &in))
                            {
                                throw in;
                            }

                            break;
                        }
                    }
                }
            }

            // General options.

            else if ("-time_units" == arg)
            {
                Time::UNITS value = Time::UNITS(0);
                in >> value;
                Time::default_units = value;
            }
            else if ("-default_speed" == arg)
            {
                Speed::FPS value = Speed::FPS(0);
                in >> value;
                Speed::default_fps = value;
            }

            else if ("-help" == arg || "-h" == arg)
            {
                exit(EXIT_HELP);
            }

            else if ("-info" == arg)
            {
                exit(EXIT_INFO);
            }
            else if ("-about" == arg)
            {
                exit(EXIT_ABOUT);
            }

#           if defined(DJV_OSX)

            else if ("-psn_" == String(arg, 0, 5))
            {
                //! \todo Ignore Mac OS process id.
            }

#           endif

            // Leftovers.

            else
            {
                tmp << arg;
            }
        }
    }
    catch (String)
    {
        throw Error(arg);
    }

    in = tmp;
}

const String & Core_Application::command_line_name() const
{
    return _command_line_name;
}

const String & Core_Application::command_line_args() const
{
    return _command_line_args;
}

namespace
{

const String label_command_line_help =
" OpenGL Options\n"
"\n"
"     -render_filter (minify) (magnify)\n"
"         Set the render filter. Options = %%. Default = %%, %%.\n"
"\n"
" Image I/O Options\n"
"\n"
"     -base (plugin) (option) (default)\n"
"         Set a base option.\n"
"\n"
"     -load (plugin) (option) (default)\n"
"         Set a load option.\n"
"\n"
"     -save (plugin) (option) (default)\n"
"         Set a save option.\n"
"\n"
"     See the online documentation for image I/O options.\n"
"\n"
" General Options\n"
"\n"
"     -time_units (value)\n"
"         Set the time units. Options = %%. Default = %%.\n"
"\n"
"     -default_speed (value)\n"
"         Set the default speed. Options = %%. Default = %%.\n"
"\n"
"     -help, -h\n"
"         Show the help message.\n"
"\n"
"     -info\n"
"         Show the information message.\n"
"\n"
"     -about\n"
"         Show the about message.\n"
"\n";

} // namespace

String Core_Application::command_line_help() const
{
    return String_Format(label_command_line_help).
        arg(String_Util::lower(Gl_Image_Filter::label_filter()), ", ").
        arg(String_Util::lower(
            String_Util::label(Gl_Image_Filter::default_filter.min))).
        arg(String_Util::lower(
            String_Util::label(Gl_Image_Filter::default_filter.mag))).
        arg(String_Util::lower(Time::label_units()), ", ").
        arg(String_Util::lower(String_Util::label(Time::default_units))).
        arg(String_Util::lower(Speed::label_fps()), ", ").
        arg(String_Util::lower(String_Util::label(Speed::default_fps)));
}

const String Core_Application::error_command_line =
    "Cannot parse the command line argument: %%";

Gl_Context * Core_Application::context()
{
    return _context;
}

Core_Application * Core_Application::global()
{
    return _application;
}

} // djv

