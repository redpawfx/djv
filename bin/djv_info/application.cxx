/*------------------------------------------------------------------------------
 bin/djv_info/application.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include "etc.h"
#include <djv_image/application.h>
#include <djv_image/io.h>
#include <djv_base/directory.h>
#include <djv_base/system.h>

namespace djv_bin {
namespace info {

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

Application::Application(int argc, char ** argv) :
  djv_image::application::Application_Base("djv_info", argc, argv),
  _info(true),
  _verbose(false),
  _file_path(false),
  _seq(file_seq::COMPRESS_RANGE),
  _recurse(false),
  _columns(djv_base::system::terminal_width())
{
  DJV_DEBUG("Application::Application");
    
  // Command line.

  try
  {
    String tmp = command_line_args();
    command_line(tmp);
  }
  catch (Error in)
  {
    error(Error(string::Format(application::error_command_line).arg(in)));
    exit(application::EXIT_ERROR);
  }
  
  if (exit() != application::EXIT_DEFAULT)
    return;

  // Convert inputs:
  //
  // * Match wildcards
  // * Expand sequences
  
  List<File> list;
  
  for (list::size_type i = 0; i < _input.size(); ++i)
  {
    DJV_DEBUG_PRINT("input = " << _input[i]);
    
    File file(_input[i]);
    
    // Match wildcards.
    
    if (_seq && file.is_seq_wildcard())
    {
      file = file::seq_wildcard_match(
        file,
        Directory(file.path()).list()
      );
      
      DJV_DEBUG_PRINT("  wildcard match = " << file);
    }
    
    // Is this is a sequence?
    
    if (_seq && file.is_seq_valid())
    {
      file.type(file::SEQ);
    
      DJV_DEBUG_PRINT("sequence = " << file);
    }
    
    // Expand the sequence.
    
    List<String> tmp = file::seq_expand(file);
    
    for (list::size_type j = 0; j < tmp.size(); ++j)
    {
      file = File(tmp[j]);
      
      if (_seq && file.is_seq_valid())
        file.type(file::SEQ);
      
      list += file;
    }
  }
  
  DJV_DEBUG_PRINT("list = " << list);

  // Compress inputs into sequences.

  file::seq_compress(list, _seq);
  
  DJV_DEBUG_PRINT("list = " << list);
  
  // Work.
  
  for (list::size_type i = 0; i < list.size(); ++i)
    if (file::DIRECTORY == list[i].type())
    {
      try
      {
        print_directory(
          list[i],
          (list.size() > 1) || _recurse && ! _file_path
        );
      }
      catch (Error in)
      {
        error(in);
        exit(application::EXIT_ERROR);
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
        exit(application::EXIT_ERROR);
      }
    }
}

void Application::print_file(const File & in, bool path, bool info)
  throw (Error)
{
  DJV_DEBUG("Application::print_file");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Open.

  djv_image::io::Info _info;
  
  std::auto_ptr<djv_image::io::Load>(djv_image::io::global()->load(in, &_info));
  
  // Print.

  const String name = in.get(-1, path);
  
  if (info && ! _verbose)
  {
    if (1 == _info.layer_size())
    {
      // Print single layer information.
      
      const String str = string::Format(label_info).
        arg(_info[0].size.x).
        arg(_info[0].size.y).
        arg(aspect(_info[0].size), 0, -2).
        arg(string::label(_info[0].pixel)).
        arg(djv_image::time::units_to_string(
          static_cast<int>(_info.time.list.size()), _info.time.speed)).
        arg(djv_image::time::speed_to_float(_info.time.speed), 0, -2);
      
      print(string::Format("%% %%").
        arg(name).
        arg(str, _columns - static_cast<int>(name.size()) - 2));
    }
    else
    {
      // Print time information.
      
      const String str = string::Format(label_layer_time).
        arg(djv_image::time::units_to_string(
          static_cast<int>(_info.time.list.size()), _info.time.speed)).
        arg(djv_image::time::speed_to_float(_info.time.speed), 0, -2);
      
      print(string::Format("%% %%").
        arg(name).
        arg(str, _columns - static_cast<int>(name.size()) - 2));
      
      // Print each layer's information.
      
      for (list::size_type i = 0; i < _info.layer_size(); ++i)
      {
        const String name_string = string::Format(label_layer_name).
          arg(static_cast<int>(i)).
          arg(_info[i].layer_name);
        
        const String info_string = string::Format(label_layer_info).
          arg(_info[i].size.x).
          arg(_info[i].size.y).
          arg(aspect(_info[i].size), 0, -2).
          arg(string::label(_info[i].pixel));
        
        print(string::Format("%% %%").
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
    
    for (list::size_type i = 0; i < _info.layer_size(); ++i)
    {
      print(string::Format(label_verbose_layer).arg(_info[i].layer_name));
      print(string::Format(label_verbose_width).arg(_info[i].size.x));
      print(string::Format(label_verbose_height).arg(_info[i].size.y));
      print(string::Format(label_verbose_aspect).
        arg(aspect(_info[i].size), 0, -2));
      print(string::Format(label_verbose_pixel).
        arg(string::label(_info[i].pixel)));
    }

    print(string::Format(label_verbose_start).
      arg(djv_image::time::units_to_string(
        _info.time.start(), _info.time.speed)));
    
    print(string::Format(label_verbose_end).
      arg(djv_image::time::units_to_string(
        _info.time.end(), _info.time.speed)));
    
    print(string::Format(label_verbose_duration).
      arg(djv_image::time::units_to_string(
        static_cast<int>(_info.time.list.size()), _info.time.speed)));
    
    print(string::Format(label_verbose_speed).
      arg(djv_image::time::speed_to_float(_info.time.speed)));

    const List<String> key_list = _info.tag.key_list();
    
    for (list::size_type i = 0; i < key_list.size(); ++i)
      print(string::Format(label_verbose_tag).
        arg(key_list[i]).
        arg(_info.tag[key_list[i]]));

    print_separator();
  }
  else
  {
    print(name);
  }
}

void Application::print_directory(const File & in, bool label) throw (Error)
{
  DJV_DEBUG("Application::print_directory");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Read directory.

  Directory directory(in);
  
  if (! directory.is_valid() && ! _recurse)
    throw Error(string::Format(error_directory).arg(in));
  
  List<File> list = directory.list();
  
  // Process files.
  
  file::filter(list, file::FILTER_DIRECTORY);
  file::seq_compress(list, _seq);

  // Print.

  if (label)
  {
    print(string::Format(label_directory).arg(in));
    
    if (_verbose)
      print_separator();
  }
  
  for (list::size_type i = 0; i < list.size(); ++i)
    try
    {
      print_file(list[i], _file_path, _info);
    }
    catch (Error)
    {}
  
  if (label)
    print_separator();
  
  // Recurse.
  
  if (_recurse)
  {
    List<File> list = Directory(in).list();
    
    file::filter(list, file::FILTER_FILE | file::FILTER_HIDDEN);
    
    for (list::size_type i = 0; i < list.size(); ++i)
      print_directory(list[i], label);
  }
}

}
}

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
  int r = 1;
  
  try
  {
    r = djv_bin::info::Application(argc, argv).exec();
  }
  catch (djv_base::Error in)
  {
    djv_base::error::print(in);
  }
  
  return r;
}

//------------------------------------------------------------------------------
