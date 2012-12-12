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

//! \file djv_view_application.cpp

#include <djv_view_application.h>

#include <djv_view_file_prefs.h>
#include <djv_view_file_save.h>
#include <djv_view_image_prefs.h>
#include <djv_view_input_prefs.h>
#include <djv_view_playback_prefs.h>
#include <djv_view_shortcut_prefs.h>
#include <djv_view_view_prefs.h>
#include <djv_view_window.h>
#include <djv_view_window_prefs.h>

#include <djv_prefs_dialog.h>

namespace djv_view
{

namespace
{

File_Prefs *     file_prefs     = 0;
Image_Prefs *    image_prefs    = 0;
Input_Prefs *    input_prefs    = 0;
Playback_Prefs * playback_prefs = 0;
Shortcut_Prefs * shortcut_prefs = 0;
View_Prefs *     view_prefs     = 0;
Window_Prefs *   window_prefs   = 0;

} // namespace

File_Prefs * File_Prefs::global()
{
    return file_prefs;
}

Image_Prefs * Image_Prefs::global()
{
    return image_prefs;
}

Input_Prefs * Input_Prefs::global()
{
    return input_prefs;
}

Playback_Prefs * Playback_Prefs::global()
{
    return playback_prefs;
}

Shortcut_Prefs * Shortcut_Prefs::global()
{
    return shortcut_prefs;
}

View_Prefs * View_Prefs::global()
{
    return view_prefs;
}

Window_Prefs * Window_Prefs::global()
{
    return window_prefs;
}

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

Application::Application(int & argc, char ** argv) :
    djv::Application("djv_view", argc, argv)
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

    // Initialize user-interface.

    file_prefs     = new File_Prefs;
    image_prefs    = new Image_Prefs;
    input_prefs    = new Input_Prefs;
    playback_prefs = new Playback_Prefs;
    shortcut_prefs = new Shortcut_Prefs;
    view_prefs     = new View_Prefs;
    window_prefs   = new Window_Prefs;

    is_valid(true);

    // Initialize preferences dialog.

    Prefs_Dialog::global()->add_shortcut("djv_view", new Shortcut_Prefs_Widget);
    Prefs_Dialog::global()->add("Input", new Input_Prefs_Widget);
    Prefs_Dialog::global()->add("Playback", new Playback_Prefs_Widget);
    Prefs_Dialog::global()->add("Images", new Image_Prefs_Widget);
    Prefs_Dialog::global()->add("Views", new View_Prefs_Widget);
    Prefs_Dialog::global()->add("Windows", new Window_Prefs_Widget);
    Prefs_Dialog::global()->add("Files", new File_Prefs_Widget);

    // Show main window(s).

    if (_input.size())
    {
        // Combine command line arguments.

        if (File_Prefs::global()->command_line_combine())
        {
            File file(_input[0]);

            if (file.is_seq_valid())
            {
                file.type(File::SEQ);
            }

            for (size_t i = 1; i < _input.size(); ++i)
            {
                file.seq_add(_input[i]);
            }

            File_Util::seq_sort(file);

            _input.resize(1);
            _input[0] = file;
        }

        // Create and show a window for each input.

        for (size_t i = 0; i < _input.size(); ++i)
        {
            File file(_input[i]);

            if (file.is_seq_valid())
            {
                file.type(File::SEQ);
            }

            Window * window = this->window();

            window->file_open(file);

            //! \todo Is this still necessary?

            if (_command_line.file_layer.get())
            {
                window->file_layer(*_command_line.file_layer);
            }

            if (_command_line.playback.get())
            {
                window->playback(*_command_line.playback);
            }

            if (_command_line.playback_frame.get())
            {
                window->playback_frame(*_command_line.playback_frame);
            }

            if (_command_line.playback_speed.get())
            {
                window->playback_speed(*_command_line.playback_speed);
            }

            window->show();
        }
    }
    else
    {
        // Create and show an empty window.

        window()->show();
    }
}

Application::~Application()
{
    //DJV_DEBUG("Application::~Application");

    delete file_prefs;
    delete image_prefs;
    delete input_prefs;
    delete playback_prefs;
    delete shortcut_prefs;
    delete view_prefs;
    delete window_prefs;
}

void Application::command_line(String & in) throw (Error)
{
    //DJV_DEBUG("Application::command_line");
    //DJV_DEBUG_PRINT("in = " << in);

    djv::Application::command_line(in);

    if (exit() != EXIT_DEFAULT)
    {
        return;
    }

    String arg;

    try
    {
        while (! in.empty())
        {
            in >> arg;

            // File options.

            if ("-file_seq_auto" == arg)
            {
                bool value = 0;
                in >> value;
                _command_line.file_seq_auto =
                    std::auto_ptr<bool>(new bool(value));
            }
            else if ("-file_layer" == arg)
            {
                int value = 0;
                in >> value;
                _command_line.file_layer = std::auto_ptr<int>(new int(value));
            }
            else if ("-file_proxy" == arg)
            {
                Pixel_Data_Info::PROXY value(Pixel_Data_Info::PROXY(0));
                in >> value;
                _command_line.file_proxy =
                    std::auto_ptr<Pixel_Data_Info::PROXY>(
                        new Pixel_Data_Info::PROXY(value));
            }
            else if ("-file_cache" == arg)
            {
                bool value = false;
                in >> value;
                _command_line.file_cache = std::auto_ptr<bool>(new bool(value));
            }

            // Window options.

            else if ("-window_fullscreen" == arg)
            {
                _command_line.window_fullscreen =
                    std::auto_ptr<bool>(new bool(true));
            }

            // Playback options.

            else if ("-playback" == arg)
            {
                Playback::PLAYBACK value(Playback::PLAYBACK(0));
                in >> value;
                _command_line.playback =
                    std::auto_ptr<Playback::PLAYBACK>(
                        new Playback::PLAYBACK(value));
            }
            else if ("-playback_frame" == arg)
            {
                int value = 0;
                in >> value;
                _command_line.playback_frame =
                    std::auto_ptr<int>(new int(value));
            }
            else if ("-playback_speed" == arg)
            {
                Speed::FPS value(Speed::FPS(0));
                in >> value;
                _command_line.playback_speed =
                    std::auto_ptr<Speed>(new Speed(value));
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
" djv_view\n"
"\n"
"     This program provides movie playback and image viewing.\n"
"\n"
" Usage\n"
"\n"
"     djv_view [image]... [option]...\n"
"\n"
" File Options\n"
"\n"
"     -file_seq_auto (value)\n"
"         Automatically detect sequences when opening files. Options = %%.\n"
"\n"
"     -file_layer (value)\n"
"         Set the input layer. Default = %%.\n"
"\n"
"     -file_proxy (value)\n"
"         Set the proxy scale. Options = %%.\n"
"\n"
"     -file_cache (value)\n"
"         Set whether the file cache is enabled. Options = %%.\n"
"\n"
" Window Options\n"
"\n"
"     -window_fullscreen\n"
"         Set the window fullscreen.\n"
"\n"
" Playback Options\n"
"\n"
"     -playback (value)\n"
"         Set the playback. Options = %%.\n"
"\n"
"     -playback_frame (value)\n"
"         Set the playback frame.\n"
"\n"
"     -playback_speed (value)\n"
"         Set the playback speed. Options = %%.\n"
"\n"
"%%";

} // namespace

String Application::command_line_help() const
{
    return String(String_Format(label_command_line_help).
        arg(String_Util::lower(String_Util::label_bool()), ", ").
        arg(0).
        arg(String_Util::lower(Pixel_Data_Info::label_proxy()), ", ").
        arg(String_Util::lower(String_Util::label_bool()), ", ").
        arg(String_Util::lower(Playback::label_playback()), ", ").
        arg(String_Util::lower(Speed::label_fps()), ", ").
        arg(djv::Application::command_line_help()));
}

Window * Application::window() const
{
    Window * out = Window_Factory::global()->add();

    // Apply command line file options.

    if (_command_line.file_seq_auto.get())
    {
        out->file_seq_auto(*_command_line.file_seq_auto);
    }

    if (_command_line.file_layer.get())
    {
        out->file_layer(*_command_line.file_layer);
    }

    if (_command_line.file_proxy.get())
    {
        out->file_proxy(*_command_line.file_proxy);
    }

    if (_command_line.file_cache.get())
    {
        out->file_cache(*_command_line.file_cache);
    }

    // Apply command line window options.

    if (_command_line.window_fullscreen.get())
    {
        out->window_fullscreen(*_command_line.window_fullscreen);
    }

    // Apply command line playback options.

    if (_command_line.playback.get())
    {
        out->playback(*_command_line.playback);
    }

    if (_command_line.playback_frame.get())
    {
        out->playback_frame(*_command_line.playback_frame);
    }

    if (_command_line.playback_speed.get())
    {
        out->playback_speed(*_command_line.playback_speed);
    }

    return out;
}

} // djv_view

