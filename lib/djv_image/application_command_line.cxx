/*------------------------------------------------------------------------------
 lib/djv_image/application_command_line.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include <djv_image/gl.h>
#include <djv_image/gl_data.h>
#include <djv_image/gl_offscreen.h>
#include <djv_image/io.h>

namespace djv_image {
namespace application {

//------------------------------------------------------------------------------
// help_image_io()
//------------------------------------------------------------------------------

namespace {

String help_image_io_option(
  const io::Base * in,
  const String & flag
) {
  String out;
  
  const String & name = in->name();
  const List<String> & option_list = in->option_list();
  
  for (list::size_type i = 0; i < option_list.size(); ++i)
    out += string::Format(label_command_line_io_option).
      arg(option_list[i]).
      arg(flag).
      arg(string::lower(name)).
      arg(string::lower(option_list[i])).
      arg(string::lower(in->option_value(option_list[i])), ", ").
      arg(string::lower(string::label(in->option_default(option_list[i]))));
  
  return out;
}

String help_image_io(const String & separator)
{
  String out;
  
  const List<io::Global::Group> & list = io::global()->group_list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    const io::Global::Group & group = list[i];
    
    if (group.base && group.base->option_list().size())
    {
      out += string::Format(label_command_line_io_base).
        arg(separator).
        arg(group.name);
      out += help_image_io_option(group.base, "-io");
    }
  
    if (group.load && group.load->option_list().size())
    {
      out += string::Format(label_command_line_io_load).
        arg(separator).
        arg(group.name);
      out += help_image_io_option(group.load, "-load");
    }
  
    if (group.save && group.save->option_list().size())
    {
      out += string::Format(label_command_line_io_save).
        arg(separator).
        arg(group.name);
      out += help_image_io_option(group.save, "-save");
    }
  }
  
  return out;
}

}

//------------------------------------------------------------------------------
// Application_Base::command_line()
//------------------------------------------------------------------------------

void Application_Base::command_line(String & in) throw (Error)
{
  DJV_DEBUG("Application_Base::command_line");
  DJV_DEBUG_PRINT("in = " << in);
  
  djv_base::application::Application_Base::command_line(in);
  
  const List<io::Load *> & load = io::global()->load()->list();
  const List<io::Save *> & save = io::global()->save()->list();

  String tmp, arg;
  
  try
  {
    while (! in.empty())
    {
      in >> arg;
      
      // Time options.
      
      if (string::compare_no_case("-time_units", arg))
      {
        time::UNITS value = time::UNITS(0);
        in >> value;
        time::global()->units(value);
      }
      else if (string::compare_no_case("-speed_default", arg))
      {
        time::FPS value = time::FPS(0);
        in >> value;
        time::global()->speed(value);
      }
      
      // Render options.
      
      else if (string::compare_no_case("-render", arg))
      {
        gl::PIPELINE value = gl::PIPELINE(0);
        in >> value;
        gl::global()->pipeline(value);
      }
      else if (string::compare_no_case("-render_filter", arg))
      {
        gl_data::Filter value;
        in >> value;
        gl_data::global()->filter(value);
      }
      else if (string::compare_no_case("-render_offscreen", arg))
      {
        gl_offscreen::BUFFER value = gl_offscreen::BUFFER(0);
        in >> value;
        gl_offscreen::global()->buffer(value);
      }
            
      // Image I/O options.
      
      else if (string::compare_no_case("-load", arg))
      {
        String plugin;
        in >> plugin;
        for (list::size_type i = 0; i < load.size(); ++i)
          if (string::compare_no_case(plugin, load[i]->name()))
          {
            String option;
            if (! string::serialize(&in, &option, load[i]->option_list()))
              throw in;
            if (! load[i]->option(option, &in))
              throw in;
            break;
          }
      }
      else if (string::compare_no_case("-save", arg))
      {
        String plugin;
        in >> plugin;
        for (list::size_type i = 0; i < save.size(); ++i)
          if (string::compare_no_case(plugin, save[i]->name()))
          {
            String option;
            if (! string::serialize(&in, &option, save[i]->option_list()))
              throw in;
            if (! save[i]->option(option, &in))
              throw in;
            break;
          }
      }
      
      // Help options.
      
      else if (string::compare_no_case("-help_image", arg))
      {
        _help_default = false;

        exit(djv_base::application::EXIT_HELP);
      }
      
      // Leftovers.
      
      else
      {
        tmp << arg;
      }
    }
  }
  catch (String) { throw Error(arg); }
  
  in = tmp;
}

//------------------------------------------------------------------------------
// Application_Base::command_line_help()
//------------------------------------------------------------------------------

String Application_Base::command_line_help() const
{
  return _help_default ?
    string::Format(label_command_line_help).
      arg(command_line_separator()).
      arg(string::lower(time::label_units()), ", ").
      arg(string::lower(string::label(time::global()->units()))).
      arg(string::lower(time::label_fps()), ", ").
      arg(string::lower(string::label(time::global()->speed()))).
      arg(djv_base::application::Application_Base::command_line_help()) :
    string::Format(label_command_line_image).
      arg(command_line_separator()).
      arg(string::lower(gl::label_pipeline()), ", ").
      arg(string::lower(string::label(gl::global()->pipeline()))).
      arg(string::lower(gl_data::label_filter()), ", ").
      arg(string::lower(string::label(gl_data::global()->filter()))).
      arg(string::lower(gl_offscreen::label_buffer()), ", ").
      arg(string::lower(string::label(gl_offscreen::global()->buffer()))).
      arg(help_image_io(command_line_separator()));
}

//------------------------------------------------------------------------------

}
}
