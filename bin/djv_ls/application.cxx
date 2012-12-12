/*------------------------------------------------------------------------------
 bin/djv_ls/application.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include "etc.h"
#include <djv_base/directory.h>
#include <djv_base/math.h>
#include <djv_base/system.h>
#include <djv_base/user.h>

namespace djv_bin {
namespace ls {

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

Application::Application(int argc, char ** argv) :
  djv_image::application::Application_Base("djv_ls", argc, argv),
  _info(true),
  _file_path(false),
  _seq(file_seq::COMPRESS_RANGE),
  _recurse(false),
  _hidden(false),
  _columns(djv_base::system::terminal_width()),
  _sort(file::SORT_NAME),
  _sort_reverse(false),
  _x_sort_directory(false)
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
    File file(_input[i], false);
    
    DJV_DEBUG_PRINT("file = " << file);
    
    // Match wildcards.
      
    if (_seq && file.is_seq_wildcard())
    {
      file = file::seq_wildcard_match(
        file,
        Directory(file.path()).list()
      );
      
      DJV_DEBUG_PRINT("  wildcard match = " << file);
    }
      
    // Is this a sequence?
      
    if (_seq && file.is_seq_valid())
    {
      file.type(file::SEQ);
      
      DJV_DEBUG_PRINT("  sequence = " << file);
    }

    // Expand the sequence.
    
    List<String> tmp = file::seq_expand(file);
    
    for (list::size_type j = 0; j < tmp.size(); ++j)
    {
      file = File(tmp[j], false);
      
      if (! file.stat())
      {
        error(Error(string::Format(error_open).arg(file)));
        exit(application::EXIT_ERROR);
        continue;
      }
      
      if (_seq && file.is_seq_valid())
        file.type(file::SEQ);
      
      list += file;
    }
  }
  
  DJV_DEBUG_PRINT("list = " << list);
  
  // Process inputs.
  
  process(list);

  DJV_DEBUG_PRINT("process = " << list);

  // If there are no inputs, list the current directory.
  
  if (! list.size() && exit() != application::EXIT_ERROR)
    list += File(".");

  // Work.

  for (list::size_type i = 0; i < list.size(); ++i)
    if (file::DIRECTORY == list[i].type())
    {
      if (
        ! print_directory(
          list[i],
          (list.size() > 1) || _recurse && ! _file_path
        )
      ) {
        error(Error(string::Format(error_open).arg(list[i])));
        exit(application::EXIT_ERROR);
      }
    }
    else
    {
      print_file(list[i], true, _info);
    }
}

Application::~Application()
{
  DJV_DEBUG("Application::~Application");
}

void Application::process(List<File> & in)
{
  DJV_DEBUG("Application::process");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Compress files into sequences.
  
  file::seq_compress(in, _seq);

  DJV_DEBUG_PRINT("compress = " << in);

  // Remove hidden files.
  
  if (! _hidden)
  {
    file::filter(in, file::FILTER_HIDDEN, _filter);

    DJV_DEBUG_PRINT("hidden = " << in);
  }
  
  // Sort.
  
  file::sort(in, _sort, _sort_reverse);
  
  DJV_DEBUG_PRINT("sort = " << in);

  if (! _x_sort_directory)
  {
    file::sort_directories_first(in);
    
    DJV_DEBUG_PRINT("sort directories = " << in);
  }
}

void Application::print_file(const File & in, bool path, bool info)
{
  DJV_DEBUG("Application::print_file");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("path = " << path);
  DJV_DEBUG_PRINT("info = " << info);
  DJV_DEBUG_PRINT("columns = " << _columns);
  
  String name = in.get(-1, path);
  
  if (info)
  {
    const String info_string = string::Format(label_info_format).
      arg(file::label_type()[in.type()], 4).
      arg(file::label_size(in.size())).
#if ! defined(DJV_WINDOWS)
      arg(user::uid_to_string(in.user())).
#endif // DJV_WINDOWS
      arg(file::label_perm(in.perm()), 3).
      arg(djv_base::time::time_to_string(in.time()));

    // Elide the name if there isn't enough space.
    
    if (_columns)
    {
      const String::size_type size =
        math::max(_columns - static_cast<int>(info_string.size()) - 2, 0);
    
      if (name.size() > size)
      {
        name = String(name, 0, size);
      
        size_t i = name.size();
        size_t j = 3;
        while (i-- > 0 && j-- > 0)
          name[i] = '.';
      }
    }
  
    print(
      string::Format("%% %%").
      arg(name).
      arg(info_string, _columns - static_cast<int>(name.size()) - 2)
    );
  }
  else
  {
    print(name);
  }
}

bool Application::print_directory(const File & in, bool label)
{
  DJV_DEBUG("Application::print_directory");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Read directory.

  Directory directory(in);
  
  if (! directory.is_valid())
    return false;
  
  List<File> list = directory.list();
  
  // Process files.
  
  process(list);
  
  // Print.

  if (label)
    print(string::Format(label_directory).arg(in));

  for (list::size_type i = 0; i < list.size(); ++i)
    print_file(list[i], _file_path, _info);

  if (label)
    print_separator();
  
  // Recurse.

  bool r = true;
  
  if (_recurse)
  {
    List<File> list = Directory(in).list();
    
    file::filter(
      list,
      file::FILTER_FILE | (! _hidden ? file::FILTER_HIDDEN : 0)
    );
    
    for (list::size_type i = 0; i < list.size(); ++i)
      r &= print_directory(list[i], label);
  }
  
  return r;
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
    r = djv_bin::ls::Application(argc, argv).exec();
  }
  catch (djv_base::Error in)
  {
    djv_base::error::print(in);
  }
  
  return r;
}

//------------------------------------------------------------------------------
