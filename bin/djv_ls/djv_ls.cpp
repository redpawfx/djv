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

//! \file djv_ls.cpp

#include "djv_ls.h"

#include <djv_directory.h>
#include <djv_math.h>
#include <djv_system.h>
#include <djv_time.h>
#include <djv_user.h>

namespace djv_ls
{

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

namespace
{

const String
#   if defined(DJV_WINDOWS)
    label_info_format = "%% %% %% %%",
#   else
    label_info_format = "%% %% %% %% %%",
#   endif
    label_directory = "%%:";

const String
    error_open = "Cannot open: %%";

} // namespace

Application::Application(int argc, char ** argv) :
    Core_Application("djv_ls", argc, argv),
    _info(true),
    _file_path(false),
    _seq(Seq::COMPRESS_RANGE),
    _recurse(false),
    _hidden(false),
    _columns(System::terminal_width()),
    _sort(File_Util::SORT_NAME),
    _reverse_sort(false),
    _x_sort_dirs(false)
{
    //DJV_DEBUG("Application::Application");

    // Command line.

    try
    {
        String tmp = command_line_args();
        command_line(tmp);
    }
    catch (Error in)
    {
        error(Error(String_Format(error_command_line).arg(in)));
        exit(EXIT_ERROR);
    }

    if (exit() != EXIT_DEFAULT)
    {
        return;
    }

    // Convert inputs:
    //
    // * Match wildcards
    // * Expand sequences

    List<File> list;

    for (size_t i = 0; i < _input.size(); ++i)
    {
        File file(_input[i], false);

        //DJV_DEBUG_PRINT("file = " << file);

        // Match wildcards.

        if (_seq && file.is_seq_wildcard())
        {
            file = File_Util::seq_wildcard_match(
                file,
                Directory(file.path()).list());

            //DJV_DEBUG_PRINT("  wildcard match = " << file);
        }

        // Is this a sequence?

        if (_seq && file.is_seq_valid())
        {
            file.type(File::SEQ);

            //DJV_DEBUG_PRINT("  sequence = " << file);
        }

        // Expand the sequence.

        List<String> tmp = File_Util::seq_expand(file);

        for (size_t j = 0; j < tmp.size(); ++j)
        {
            file = File(tmp[j], false);

            if (! file.stat())
            {
                error(Error(String_Format(error_open).arg(file)));
                exit(EXIT_ERROR);
                continue;
            }

            if (_seq && file.is_seq_valid())
            {
                file.type(File::SEQ);
            }

            list += file;
        }
    }

    //DJV_DEBUG_PRINT("list = " << list);

    // Process inputs.

    process(list);

    //DJV_DEBUG_PRINT("process = " << list);

    // If there are no inputs, list the current directory.

    if (! list.size() && exit() != EXIT_ERROR)
    {
        list += File(".");
    }

    // Work.

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (File::DIRECTORY == list[i].type())
        {
            if (! print_directory(
                    list[i],
                    ((list.size() > 1) || _recurse) && ! _file_path))
            {
                error(Error(String_Format(error_open).arg(list[i])));
                exit(EXIT_ERROR);
            }
        }
        else
        {
            print_file(list[i], true, _info);
        }
    }
}

Application::~Application()
{
    //DJV_DEBUG("Application::~Application");
}

void Application::command_line(String & in) throw (Error)
{
    //DJV_DEBUG("command_line");
    //DJV_DEBUG_PRINT("in = " << in);

    Core_Application::command_line(in);

    if (exit() != EXIT_DEFAULT)
    {
        return;
    }

    List<String> args;
    
    String arg;

    try
    {
        while (! in.empty())
        {
            in >> arg;

            // Options.

            if ("-x_info" == arg || "-xi" == arg)
            {
                _info = false;
            }
            else if ("-file_path" == arg || "-fp" == arg)
            {
                _file_path = true;
            }
            else if ("-seq" == arg || "-q" == arg)
            {
                in >> _seq;
            }
            else if ("-recurse" == arg || "-r" == arg)
            {
                _recurse = true;
            }
            else if ("-hidden" == arg)
            {
                _hidden = true;
            }
            else if ("-filter" == arg || "-f" == arg)
            {
                in >> _filter;
            }
            else if ("-columns" == arg || "-c" == arg)
            {
                in >> _columns;
            }

            // Sorting options.

            else if ("-sort" == arg || "-s" == arg)
            {
                in >> _sort;
            }
            else if ("-reverse_sort" == arg || "-rs" == arg)
            {
                _reverse_sort = true;
            }
            else if ("-x_sort_dirs" == arg || "-xsd" == arg)
            {
                _x_sort_dirs = false;
            }

            // Arguments.

            else
            {
                _input += arg;
            }
        }
    }
    catch (String)
    {
        throw Error(arg);
    }
}

namespace
{

const String label_command_line_help =
" djv_ls\n"
"\n"
"     This program provides a command line tool for listing directories with\n"
"     file sequences.File sequences are groups of files that represent frames\n"
"     from a movie or animation. Since there may be a large number of files\n"
"     within a sequence it can be useful to view them as a group instead of\n"
"     individually.\n"
"\n"
"     Example output:\n"
"\n"
"     el_cerrito_bart.1k.tiff   File 2.23MB darby rw Mon Jun 12 21:21:55 2006\n"
"     richmond_train.2k.tiff    File 8.86MB darby rw Mon Jun 12 21:21:58 2006\n"
"     fishpond.1-749.png       Seq 293.17MB darby rw Thu Aug 17 16:47:43 2006\n"
"\n"
"     Key:\n"
"\n"
"     (name) (size) (user) (permissions) (time)\n"
"\n"
"     File types:                   Permissions:\n"
"\n"
"     * File - Regular file         * r - Readable\n"
"     * Seq  - File sequence        * w - Writable\n"
"     * Dir  - Directory            * x - Executable\n"
"\n"
" Usage\n"
"\n"
"     djv_ls [file|directory]... [option]...\n"
"\n"
" Options\n"
"\n"
"     -x_info, -xi\n"
"         Don't show information, only file names.\n"
"\n"
"     -file_path, -fp\n"
"         Show file path names.\n"
"\n"
"     -seq, -q (value)\n"
"         Set file sequencing. Options = %%. Default = %%.\n"
"\n"
"     -recurse, -r\n"
"         Descend into sub-directories.\n"
"\n"
"     -hidden\n"
"         Show hidden files.\n"
"\n"
"     -filter, -f (value)\n"
"         Set filters (e.g., *, ?).\n"
"\n"
"     -columns, -c (value)\n"
"         Set the number of columns for formatting output. A value of zero"
" disables formatting.\n"
"\n"
" Sorting Options\n"
"\n"
"     -sort, -s (value)\n"
"         Set the sorting. Options = %%. Default = %%.\n"
"\n"
"     -reverse_sort, -rs\n"
"         Reverse the sorting order.\n"
"\n"
"     -x_sort_dirs, -xsd\n"
"         Don't sort directories first.\n"
"\n"
"%%"
" Examples\n"
"\n"
"     > djv_ls\n"
"     List the current directory.\n"
"\n"
"     > djv_ls ~/movies ~/pictures\n"
"     List specific directories.\n"
"\n"
"     > djv_ls -sort time -reverse_sort\n"
"     Sort by time with the most recent first.\n"
"\n"
"     > djv_ls -filter \"*.tiff\" -filter \"*.tif\"\n"
"     Show only files ending with .tiff or .tif (note the use of"
" double-quotes).\n";

} // namespace

String Application::command_line_help() const
{
    return String_Format(label_command_line_help).
        arg(String_Util::lower(Seq::label_compress()), ", ").
        arg(String_Util::lower(String_Util::label(_seq))).
        arg(String_Util::lower(File_Util::label_sort()), ", ").
        arg(String_Util::lower(String_Util::label(_sort))).
        arg(Core_Application::command_line_help());
}

void Application::process(List<File> & in)
{
    //DJV_DEBUG("Application::process");
    //DJV_DEBUG_PRINT("in = " << in);

    // Compress files into sequences.

    File_Util::seq_compress(in, _seq);

    //DJV_DEBUG_PRINT("compress = " << in);

    // Remove hidden files.

    if (! _hidden)
    {
        File_Util::filter(in, File_Util::FILTER_HIDDEN, _filter);

        //DJV_DEBUG_PRINT("hidden = " << in);
    }

    // Sort.

    File_Util::sort(in, _sort, _reverse_sort);

    //DJV_DEBUG_PRINT("sort = " << in);

    if (! _x_sort_dirs)
    {
        File_Util::sort_directories_first(in);

        //DJV_DEBUG_PRINT("sort directories = " << in);
    }
}

void Application::print_file(const File & in, bool path, bool info)
{
    //DJV_DEBUG("Application::print_file");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("path = " << path);
    //DJV_DEBUG_PRINT("info = " << info);
    //DJV_DEBUG_PRINT("columns = " << _columns);

    String name = in.get(-1, path);

    if (info)
    {
        const String info_string = String_Format(label_info_format).
            arg(File_Util::label_type()[in.type()], 4).
            arg(File_Util::label_size(in.size())).
#           if ! defined(DJV_WINDOWS)
            arg(User::uid_to_string(in.user())).
#           endif // DJV_WINDOWS
            arg(File_Util::label_perm(in.perm()), 3).
            arg(Time::time_to_string(in.time()));

        // Elide the name if there isn't enough space.

        if (_columns)
        {
            const String::size_type size = Math::max(
                _columns - static_cast<int>(info_string.size()) - 2,
                0);

            if (name.size() > size)
            {
                name = String(name, 0, size);

                size_t i = name.size();
                size_t j = 3;

                while (i-- > 0 && j-- > 0)
                {
                    name[i] = '.';
                }
            }
        }

        print(
            String_Format("%% %%").
            arg(name).
            arg(info_string, _columns - static_cast<int>(name.size()) - 2));
    }
    else
    {
        print(name);
    }
}

bool Application::print_directory(const File & in, bool label)
{
    //DJV_DEBUG("Application::print_directory");
    //DJV_DEBUG_PRINT("in = " << in);

    // Read directory.

    Directory directory(in);

    if (! directory.is_valid())
    {
        return false;
    }

    List<File> list = directory.list();

    // Process files.

    process(list);

    // Print.

    if (label)
    {
        print(String_Format(label_directory).arg(in));
    }

    for (size_t i = 0; i < list.size(); ++i)
    {
        print_file(list[i], _file_path, _info);
    }

    if (label)
    {
        print_separator();
    }

    // Recurse.

    bool r = true;

    if (_recurse)
    {
        List<File> list = Directory(in).list();

        File_Util::filter(
            list,
            File_Util::FILTER_FILE |
                (! _hidden ? File_Util::FILTER_HIDDEN : 0));

        for (size_t i = 0; i < list.size(); ++i)
        {
            r &= print_directory(list[i], label);
        }
    }

    return r;
}

} // djv_ls

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    int r = 1;

    try
    {
        r = djv_ls::Application(argc, argv).exec();
    }
    catch (djv::Error in)
    {
        djv::Error_Util::print(in);
    }

    return r;
}
