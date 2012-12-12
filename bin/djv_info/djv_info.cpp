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

//! \file djv_info.cpp

#include "djv_info.h"

#include <djv_directory.h>
#include <djv_image_io.h>
#include <djv_system.h>

namespace djv_info
{

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

namespace
{

const String
    label_info = "%%x%%:%% %% %%@%%",
    label_layer_time = "%%@%%",
    label_layer_name = "    %%. %%",
    label_layer_info = "%%x%%:%% %%",
    label_verbose_layer = "Layer = %%",
    label_verbose_width = "  Width = %%",
    label_verbose_height = "  Height = %%",
    label_verbose_aspect = "  Aspect = %%",
    label_verbose_pixel = "  Pixel = %%",
    label_verbose_start = "Start = %%",
    label_verbose_end = "End = %%",
    label_verbose_duration = "Duration = %%",
    label_verbose_speed = "Speed = %%",
    label_verbose_tag = "Tag %% = %%",
    label_directory = "%%:";

const String
    error_command_line_input = "Input",
    error_directory = "Cannot open directory: %%";

} // namespace

Application::Application(int argc, char ** argv) :
    Core_Application("djv_info", argc, argv),
    _info(true),
    _verbose(false),
    _file_path(false),
    _seq(Seq::COMPRESS_RANGE),
    _recurse(false),
    _columns(System::terminal_width())
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
        //DJV_DEBUG_PRINT("input = " << _input[i]);

        File file(_input[i]);

        // Match wildcards.

        if (_seq && file.is_seq_wildcard())
        {
            file = File_Util::seq_wildcard_match(
                file,
                Directory(file.path()).list());

            //DJV_DEBUG_PRINT("  wildcard match = " << file);
        }

        // Is this is a sequence?

        if (_seq && file.is_seq_valid())
        {
            file.type(File::SEQ);

            //DJV_DEBUG_PRINT("sequence = " << file);
        }

        // Expand the sequence.

        List<String> tmp = File_Util::seq_expand(file);

        for (size_t j = 0; j < tmp.size(); ++j)
        {
            file = File(tmp[j]);

            if (_seq && file.is_seq_valid())
            {
                file.type(File::SEQ);
            }

            list += file;
        }
    }

    //DJV_DEBUG_PRINT("list = " << list);

    // Compress inputs into sequences.

    File_Util::seq_compress(list, _seq);

    //DJV_DEBUG_PRINT("list = " << list);

    // Work.

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (File::DIRECTORY == list[i].type())
        {
            try
            {
                print_directory(
                    list[i],
                    ((list.size() > 1) || _recurse) && ! _file_path);
            }
            catch (Error in)
            {
                error(in);
                exit(EXIT_ERROR);
            }
        }
        else
        {
            try
            {
                print_file(list[i], _file_path, _info);
            }
            catch (Error in)
            {
                error(in);
                exit(EXIT_ERROR);
            }
        }
    }
}

void Application::command_line(String & in) throw (Error)
{
    Core_Application::command_line(in);

    if (exit() != EXIT_DEFAULT)
    {
        return;
    }

    if (in.empty())
    {
        exit(EXIT_HELP);
        return;
    }

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
            else if ("-verbose" == arg || "-v" == arg)
            {
                _verbose = true;
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
            else if ("-columns" == arg || "-c" == arg)
            {
                in >> _columns;
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

    if (! _input.size())
    {
        throw Error(error_command_line_input);
    }
}

namespace
{

const String label_command_line_help =
" djv_info\n"
"\n"
"     This program provides a command line tool for displaying information\n"
"     about movies and images.\n"
"\n"
"     Example output:\n"
"\n"
"     yesterdayview.mov                    640x424:1.51 RGB U8 00:02:00:01@12\n"
"     dlad.dpx                          2048x1556:1.32 RGB U10 00:00:00:01@24\n"
"     render0001-1000.exr                                      00:00:41:16@24\n"
"         0: A,B,G,R                                     720x480:1.5 RGBA F16\n"
"         1: Ambient                                      720x480:1.5 RGB F16\n"
"         2: Diffuse                                      720x480:1.5 RGB F16\n"
"         3: Specular                                     720x480:1.5 RGB F16\n"
"         4: Z                                              720x480:1.5 L F32\n"
"\n"
"     Key:\n"
"\n"
"     (name) (width)x(height):(aspect) (format) (type) (duration)@(speed)\n"
"\n"
"     Layer key:\n"
"\n"
"     (name) (duration)@(speed)\n"
"         (layer): (name) (width)x(height):(aspect) (format) (type)\n"
"\n"
" Usage\n"
"\n"
"     djv_info (image|directory)... [option]...\n"
"\n"
" Options\n"
"\n"
"     -x_info, -xi\n"
"         Don't show image information, only file names.\n"
"\n"
"     -verbose, -v\n"
"         Show verbose image information.\n"
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
"     -columns, -c (value)\n"
"         Set the number of columns for formatting output. A value of zero"
" disables formatting.\n"
"\n"
"%%"
" Examples\n"
"\n"
"     > djv_info image.sgi image2.sgi\n"
"     Display image information.\n"
"\n"
"     > djv_info image.1-100.sgi\n"
"     Display image sequence information.\n"
"\n"
"     > djv_info ~/pics\n"
"     Display information about all images within a directory.\n";

} // namespace

String Application::command_line_help() const
{
    return String(String_Format(label_command_line_help).
        arg(String_Util::lower(Seq::label_compress()), ", ").
        arg(String_Util::lower(String_Util::label(_seq))).
        arg(Core_Application::command_line_help()));
}

void Application::print_file(const File & in, bool path, bool info)
    throw (Error)
{
    //DJV_DEBUG("Application::print_file");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open.

    Image_Io_Info _info;

    std::auto_ptr<Image_Load>(Image_Load_Factory::global()->get(in, &_info));

    // Print.

    const String name = in.get(-1, path);

    if (info && ! _verbose)
    {
        if (1 == _info.layer_size())
        {
            // Print single layer information.

            const String str = String_Format(label_info).
                arg(_info[0].size.x).
                arg(_info[0].size.y).
                arg(Vector_Util::aspect(_info[0].size), 0, -2).
                arg(String_Util::label(_info[0].pixel)).
                arg(Time::units_to_string(
                    static_cast<int>(_info.seq.list.size()), _info.seq.speed)).
                arg(Speed::speed_to_float(_info.seq.speed), 0, -2);

            print(String_Format("%% %%").
                arg(name).
                arg(str, _columns - static_cast<int>(name.size()) - 2));
        }
        else
        {
            // Print time information.

            const String str = String_Format(label_layer_time).
                arg(Time::units_to_string(
                    static_cast<int>(_info.seq.list.size()), _info.seq.speed)).
                arg(Speed::speed_to_float(_info.seq.speed), 0, -2);

            print(String_Format("%% %%").
                arg(name).
                arg(str, _columns - static_cast<int>(name.size()) - 2));

            // Print each layer's information.

            for (size_t i = 0; i < _info.layer_size(); ++i)
            {
                const String name_string = String_Format(label_layer_name).
                    arg(static_cast<int>(i)).
                    arg(_info[i].layer_name);

                const String info_string = String_Format(label_layer_info).
                    arg(_info[i].size.x).
                    arg(_info[i].size.y).
                    arg(Vector_Util::aspect(_info[i].size), 0, -2).
                    arg(String_Util::label(_info[i].pixel));

                print(String_Format("%% %%").
                    arg(name_string).
                    arg(info_string, _columns -
                        static_cast<int>(name_string.size()) - 2));
            }
        }
    }
    else if (info && _verbose)
    {
        // Print verbose informaton.

        print(name);

        for (size_t i = 0; i < _info.layer_size(); ++i)
        {
            print(String_Format(label_verbose_layer).arg(_info[i].layer_name));
            print(String_Format(label_verbose_width).arg(_info[i].size.x));
            print(String_Format(label_verbose_height).arg(_info[i].size.y));
            print(String_Format(label_verbose_aspect).
                arg(Vector_Util::aspect(_info[i].size), 0, -2));
            print(String_Format(label_verbose_pixel).
                arg(String_Util::label(_info[i].pixel)));
        }

        print(String_Format(label_verbose_start).
            arg(Time::units_to_string(_info.seq.start(), _info.seq.speed)));

        print(String_Format(label_verbose_end).
            arg(Time::units_to_string(_info.seq.end(), _info.seq.speed)));

        print(String_Format(label_verbose_duration).
            arg(Time::units_to_string(
                static_cast<int>(_info.seq.list.size()), _info.seq.speed)));

        print(String_Format(label_verbose_speed).
            arg(Speed::speed_to_float(_info.seq.speed)));

        const List<String> keys = _info.tag.keys();

        for (size_t i = 0; i < keys.size(); ++i)
            print(String_Format(label_verbose_tag).
                arg(keys[i]).
                arg(_info.tag[keys[i]]));

        print_separator();
    }
    else
    {
        print(name);
    }
}

void Application::print_directory(const File & in, bool label) throw (Error)
{
    //DJV_DEBUG("Application::print_directory");
    //DJV_DEBUG_PRINT("in = " << in);

    // Read directory.

    Directory directory(in);

    if (! directory.is_valid() && ! _recurse)
    {
        throw Error(String_Format(error_directory).arg(in));
    }

    List<File> list = directory.list();

    // Process files.

    File_Util::filter(list, File_Util::FILTER_DIRECTORY);
    File_Util::seq_compress(list, _seq);

    // Print.

    if (label)
    {
        print(String_Format(label_directory).arg(in));

        if (_verbose)
        {
            print_separator();
        }
    }

    for (size_t i = 0; i < list.size(); ++i)
        try
        {
            print_file(list[i], _file_path, _info);
        }
        catch (Error)
        {}

    if (label)
    {
        print_separator();
    }

    // Recurse.

    if (_recurse)
    {
        List<File> list = Directory(in).list();

        File_Util::filter(
            list,
            File_Util::FILTER_FILE | File_Util::FILTER_HIDDEN);

        for (size_t i = 0; i < list.size(); ++i)
        {
            print_directory(list[i], label);
        }
    }
}

} // djv_info

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    int r = 1;

    try
    {
        r = djv_info::Application(argc, argv).exec();
    }
    catch (djv::Error in)
    {
        djv::Error_Util::print(in);
    }

    return r;
}
