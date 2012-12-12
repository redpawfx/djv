/*------------------------------------------------------------------------------
 bin/djv_view/command_line.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "application.h"

namespace djv_bin {
namespace view {
namespace application {

//------------------------------------------------------------------------------
// Application::command_line()
//------------------------------------------------------------------------------

void Application::command_line(String & in) throw (Error)
{
  DJV_DEBUG("Application::command_line");
  DJV_DEBUG_PRINT("in = " << in);
  
  djv_ui::application::Application_Base::command_line(in);
  
  if (
    djv_image::application::Application_Base::exit() !=
    djv_base::application::EXIT_DEFAULT
  )
    return;

  String arg;
  
  try
  {
    while (! in.empty())
    {
      in >> arg;

      // File options.

      if (string::compare_no_case("-file_seq_auto", arg))
      {
        bool value = 0;
        in >> value;
        _command_line.file_seq_auto = std::auto_ptr<bool>(new bool(value));
      }
      else if (string::compare_no_case("-file_layer", arg))
      {
        int value = 0;
        in >> value;
        _command_line.file_layer = std::auto_ptr<int>(new int(value));
      }
      else if (string::compare_no_case("-file_proxy", arg))
      {
        djv_image::data::PROXY value(djv_image::data::PROXY(0));
        in >> value;
        _command_line.file_proxy = std::auto_ptr<djv_image::data::PROXY>(
          new djv_image::data::PROXY(value));
      }
      else if (string::compare_no_case("-file_cache", arg))
      {
        bool value = false;
        in >> value;
        _command_line.file_cache = std::auto_ptr<bool>(new bool(value));
      }

      // Window options.

      else if (string::compare_no_case("-window_fullscreen", arg))
      {
        _command_line.window_fullscreen = std::auto_ptr<bool>(new bool(true));
      }

      // Playback options.

      else if (string::compare_no_case("-playback", arg))
      {
        playback::PLAYBACK value(playback::PLAYBACK(0));
        in >> value;
        _command_line.playback =
          std::auto_ptr<playback::PLAYBACK>(new playback::PLAYBACK(value));
      }
      else if (string::compare_no_case("-playback_frame", arg))
      {
        int value = 0;
        in >> value;
        _command_line.playback_frame = std::auto_ptr<int>(new int(value));
      }
      else if (string::compare_no_case("-playback_speed", arg))
      {
        djv_image::time::FPS value(djv_image::time::FPS(0));
        in >> value;
        _command_line.playback_speed =
          std::auto_ptr<djv_image::time::Speed>(
            new djv_image::time::Speed(value));
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

//------------------------------------------------------------------------------
// Application::command_line_help()
//------------------------------------------------------------------------------

extern const String label_command_line_help =
"%%\n"
" djv_view\n"
"\n"
" Image and movie viewer.\n"
"\n"
"%%\n"
" Usage\n"
"\n"
"   djv_view [image]... [option]...\n"
"\n"
" Command line options are case-insensitive.\n"
"\n"
"%%\n"
" File Options\n"
"\n"
" Automatically detect sequences when opening files:\n"
"\n"
"   -file_seq_auto (value)\n"
"\n"
"   Values = %%\n"
"\n"
" Specify input layer:\n"
"\n"
"   -file_layer (value)\n"
"\n"
"   Values = 0 (default) - maximum\n"
"\n"
"   default = %%\n"
"\n"
" Proxy scale:\n"
"\n"
"   -file_proxy (value)\n"
"\n"
"   Values = %%\n"
"\n"
" Cache:\n"
"\n"
"   -file_cache (value)\n"
"\n"
"   Values = %%\n"
"\n"
"%%\n"
" Window Options\n"
"\n"
" Fullscreen:\n"
"\n"
"   -window_fullscreen\n"
"\n"
"%%\n"
" Playback Options\n"
"\n"
" Playback:\n"
"\n"
"   -playback (value)\n"
"\n"
"   Values = %%\n"
"\n"
" Frame:\n"
"\n"
"   -playback_frame (value)\n"
"\n"
" Speed:\n"
"\n"
"   -playback_speed (value)\n"
"\n"
"   Values = %%\n"
"\n"
"%%";

String Application::command_line_help() const
{
  return _help_default ?
    String(string::Format(label_command_line_help).
    arg(command_line_separator()).
    arg(command_line_separator()).
    arg(command_line_separator()).
    arg(string::lower(string::label_bool()), ", ").
    arg(0).
    arg(string::lower(djv_image::data::label_proxy()), ", ").
    arg(string::lower(string::label_bool()), ", ").
    arg(command_line_separator()).
    arg(command_line_separator()).
    arg(string::lower(playback::label_playback()), ", ").
    arg(string::lower(djv_image::time::label_fps()), ", ").
    arg(djv_ui::application::Application_Base::command_line_help())) :
  djv_ui::application::Application_Base::command_line_help();
}

//------------------------------------------------------------------------------

}
}
}

