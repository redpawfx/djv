/*------------------------------------------------------------------------------
 bin/djv_convert/command_line.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include "etc.h"
#include <djv_image/io.h>

namespace djv_bin {
namespace convert {

//------------------------------------------------------------------------------
// Application::command_line()
//------------------------------------------------------------------------------

void Application::command_line(String & in) throw (Error)
{
  DJV_DEBUG("command_line");
  DJV_DEBUG_PRINT("in = " << in);
  
  djv_image::application::Application_Base::command_line(in);
  
  if (exit() != djv_base::application::EXIT_DEFAULT)
    return;

  if (in.empty())
  {
    exit(djv_base::application::EXIT_HELP);
    return;
  }
  
  List<String> arg_list;
  String arg;
  
  try
  {
    while (! in.empty())
    {
      in >> arg;
    
      // Options.
  
      if (string::compare_no_case("-mirror_h", arg))
      {
        _image.mirror.x = true;
      }
      else if (string::compare_no_case("-mirror_v", arg))
      {
        _image.mirror.y = true;
      }
      else if (string::compare_no_case("-scale", arg))
      {
        in >> _image.scale.x;
        _image.scale.y = _image.scale.x;
      }
      else if (string::compare_no_case("-scale_xy", arg))
      {
        in >> _image.scale;
      }
      else if (string::compare_no_case("-resize", arg))
      {
        in >> _image.size;
      }
      else if (string::compare_no_case("-width", arg))
      {
        in >> _image.size.x;
      }
      else if (string::compare_no_case("-height", arg))
      {
        in >> _image.size.y;
      }
      else if (string::compare_no_case("-channel", arg))
      {
        in >> _image.channel;
      }
      
      // Input options.
  
      else if (string::compare_no_case("-layer", arg))
      {
        in >> _input.layer;
      }
      else if (string::compare_no_case("-proxy", arg))
      {
        in >> _input.proxy;
      }
      else if (string::compare_no_case("-time", arg))
      {
        in >> _input.start;
        in >> _input.end;
      }
      else if (string::compare_no_case("-slate", arg))
      {
        String tmp;
        in >> tmp;
        _input.slate = tmp;
        in >> _input.slate_frames;
      }
      else if (string::compare_no_case("-timeout", arg))
      {
        in >> _input.timeout;
      }
      
      // Output options.
      
      else if (string::compare_no_case("-pixel", arg))
      {
        djv_image::PIXEL value = djv_image::PIXEL(0);
        in >> value;
        _output.pixel = std::auto_ptr<djv_image::PIXEL>(
          new djv_image::PIXEL(value));
      }
      else if (string::compare_no_case("-speed", arg))
      {
        djv_image::time::FPS value = djv_image::time::FPS(0);
        in >> value;
        _output.speed = std::auto_ptr<djv_image::time::FPS>(
          new djv_image::time::FPS(value));
      }
      else if (string::compare_no_case("-tag", arg))
      {
        String name, value;
        in >> name;
        in >> value;
        _output.tag[name] = value;
      }
      else if (string::compare_no_case("-tag_auto", arg))
      {
        in >> _output.tag_auto;
      }
  
      // Arguments.
      
      else
      {
        arg_list += arg;
      }
    }
  }
  catch (String)
  {
    throw Error(arg);
  }
  
  DJV_DEBUG_PRINT("arg list = " << arg_list);
  
  // Input file.
  
  if (! arg_list.size())
    throw Error(error_command_line_input);

  _input.file = arg_list.pop_front();

  if (_input.file.is_seq_valid())
    _input.file.type(file::SEQ);
  
  // Output file.
  
  if (! arg_list.size())
    throw Error(error_command_line_output);

  _output.file = arg_list.pop_front();

  if (_output.file.is_seq_valid())
    _output.file.type(file::SEQ);
  
  if (! arg_list.empty())
    throw Error(arg_list[0]);
}

//------------------------------------------------------------------------------
// Application::command_line_help()
//------------------------------------------------------------------------------

const String label_command_line_help =
"%%\n"
" djv_convert\n"
"\n"
" Utility for converting and processing images.\n"
"\n"
"%%\n"
" Usage\n"
"\n"
"   djv_convert (input) (output) [option]...\n"
"\n"
" Command line options are case-insensitive.\n"
"\n"
"%%\n"
" Image Options\n"
"\n"
" Mirror the image horizontally or vertically:\n"
"\n"
"   -mirror_h\n"
"   -mirror_v\n"
"\n"
" Scale the image:\n"
"\n"
"   -scale (value)\n"
"   -scale_xy (x) (y)\n"
"\n"
" Resize the image:\n"
"\n"
"   -resize (width) (height)\n"
"\n"
" Resize the width or height of the image, maintaining the aspect ratio:\n"
"\n"
"   -width (value)\n"
"   -height (value)\n"
"\n"
" Show only specific image channels:\n"
"\n"
"   -channel (value)\n"
"\n"
"   Values = %%\n"
"\n"
"%%\n"
" Input Options\n"
"\n"
" Specify input layer:\n"
"\n"
"   -layer (value)\n"
"\n"
" Proxy scale:\n"
"\n"
"   -proxy (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
" Specify the start and end time:\n"
"\n"
"   -time (start) (end)\n"
"\n"
" Specify a slate:\n"
"\n"
"   -slate (input) (frames)\n"
"\n"
" Maximum number of seconds to wait for each input frame:\n"
"\n"
"   -timeout (value)\n"
"\n"
"   Values = %%\n"
"\n"
"%%\n"
" Output Options\n"
"\n"
" Convert pixel type:\n"
"\n"
"   -pixel (value)\n"
"\n"
"   Values = %%\n"
"\n"
" Set speed:\n"
"\n"
"   -speed (value)\n"
"\n"
"   Values = %%\n"
"\n"
" Set an image tag:\n"
"\n"
"   -tag (name) (value)\n"
"\n"
" Automatically generate image tags (e.g., timecode):\n"
"\n"
"   -tag_auto (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
"%%"
"%%\n"
" Examples\n"
"\n"
" Convert an image:\n"
"\n"
"   > djv_convert input.sgi output.tga\n"
"\n"
" Convert an image sequence:\n"
"\n"
"   > djv_convert input.1-100.sgi output.1.tga\n"
"\n"
" Note that only the first frame of the output is specified.\n"
"\n"
" Create an RLE compressed image sequence:\n"
"\n"
"   > djv_convert input.1-100.sgi output.1.tga -save tga compression rle\n"
"\n"
" Convert an image sequence to a movie:\n"
"\n"
"   > djv_convert input.0001-0100.dpx output.mov\n"
"\n"
" Convert a movie to an image sequence:\n"
"\n"
"   > djv_convert input.mov output.1.tga\n"
"\n"
" Convert pixel type:\n"
"\n"
"   > djv_convert input.sgi output.sgi -pixel rgb u16\n"
"\n"
" Scale an image by half:\n"
"\n"
"   > djv_convert input.tga output.tga -scale 0.5 0.5\n"
"\n"
" Resize an image:\n"
"\n"
"   > djv_convert input.tga output.tga -resize 2048 1556\n"
"\n"
" Convert a Cineon file to a linear format using the default settings:\n"
"\n"
"   > djv_convert input.cin output.tga\n"
"\n"
" Convert a Cineon file to a linear format using custom print settings (black "
"point, white point, gamma, and soft clip):\n"
"\n"
"   > djv_convert input.cin output.tga -load cineon print 95 685 2.2 10\n"
"\n";

String Application::command_line_help() const
{
  return _help_default ?
    String(string::Format(label_command_line_help).
    arg(command_line_separator()).
    arg(command_line_separator()).
    arg(command_line_separator()).
    arg(string::lower(djv_image::gl_data::label_channel()), ", ").
    arg(command_line_separator()).
    arg(string::lower(djv_image::data::label_proxy()), ", ").
    arg(string::lower(string::label(_input.proxy))).
    arg(_input.timeout).
    arg(command_line_separator()).
    arg(string::lower(djv_image::pixel::label_pixel()), ", ").
    arg(string::lower(djv_image::time::label_fps()), ", ").
    arg(string::lower(string::label_bool()), ", ").
    arg(string::lower(string::label(_output.tag_auto))).
    arg(djv_image::application::Application_Base::command_line_help()).
    arg(command_line_separator())) :
  djv_image::application::Application_Base::command_line_help();
}

//------------------------------------------------------------------------------

}
}

