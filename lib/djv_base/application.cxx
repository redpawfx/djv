/*------------------------------------------------------------------------------
 lib/djv_base/application.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include <djv_base/file.h>
#include <djv_base/math.h>
#include <djv_base/system.h>

namespace djv_base {
namespace application {

namespace {

//------------------------------------------------------------------------------
// application_path()
//
// Utility function to find the application path.
//------------------------------------------------------------------------------

String application_path(const String & command_line_name)
{
  String out;
  
  // Try getting the path from the command line.
  
  if (out.empty())
  {
    out = File(command_line_name).path();
    if (! out.empty())
      out = file::path_absolute(out);
  }
  
  // Try getting the path from:
  //
  // * The "PATH" environment variable.
  // * Current directory.
  
  if (out.empty())
  {
    static const List<String> path_env = List<String>() <<
      string::split(system::env("PATH"), file::list_separator_list);
    
    const List<String> list = List<String>() << path_env << ".";
    for (list::size_type i = 0; i < list.size(); ++i)
    {
      const String path = file::path_absolute(list[i]);
      const String name = path + file::path_separator + command_line_name;

      // XXX Check for Windows specific file extensions.
      
      if (
        File(name).stat() ||
        File(name + ".exe").stat() ||
        File(name + ".com").stat()
      ) {
        out = path;
        break;
      }
    }
  }
  
  return file::path_fix(out);
}

const String path_doc_ =
#if defined(DJV_WINDOWS)
    "../doc";
#elif defined(DJV_MACOSX)
    "../Resources/doc";
#else
    "../doc";
#endif

}

//------------------------------------------------------------------------------
// Application_Base
//------------------------------------------------------------------------------

namespace {

// Global application pointer.

Application_Base * _application = 0;

}

String Application_Base::_name;

Application_Base::Application_Base(
  const String & name,
  int & argc,
  char ** argv
) :
  _exit(EXIT_DEFAULT),
  _endline(false),
  _separator(false)
{
  _application = this;
  _name = name;
  
  DJV_DEBUG("Application_Base::Application_Base");
  DJV_DEBUG_PRINT("name = " << _name);

  // Command line.
  
  if (argc)
    _command_line_name = argv[0];
  
  for (int i = 1; i < argc; ++i)
    _command_line_args << String(argv[i]);
  
  DJV_DEBUG_PRINT("command_line = " << _command_line_args);
  
  // Application path.
  
  _path = application_path(_command_line_name);
  _path_doc = file::path_fix(_path + path_doc_);

  DJV_DEBUG_PRINT("path = " << _path);
  DJV_DEBUG_PRINT("path doc = " << _path_doc);
}

Application_Base::~Application_Base()
{
  DJV_DEBUG("Application_Base::~Application_Base");
}

int Application_Base::exec()
{
  switch (_exit)
  {
    case EXIT_HELP: message(command_line_help()); break;
    case EXIT_INFO: message("\n" + info() + "\n"); break;
    case EXIT_ABOUT: message("\n" + about() + "\n"); break;
    default: break;
  }
  
  return _exit;
}

void Application_Base::exit(EXIT in)
{
  _exit = in;
}

EXIT Application_Base::exit() const
{
  return _exit;
}

const String & Application_Base::name()
{
  return _name;
}

const String & Application_Base::path() const
{
  return _path;
}

const String & Application_Base::path_doc() const
{
  return _path_doc;
}

String Application_Base::info() const
{
  return string::Format(label_info).
    arg(DJV_VERSION_NAME).
    arg(DJV_SYSTEM_NAME).
    arg(DJV_SYSTEM_PROCESSOR).
    arg(system::info()).
    arg(string::label(memory::endian())).
    arg(system::search_path(), ", ");
}

String Application_Base::about() const
{
  return string::Format(label_about).
    arg(DJV_VERSION_NAME);
}

void Application_Base::message(const String & in) const
{
  const List<String> tmp = string::lines(in, system::terminal_width() - 2);
  
  for (list::size_type i = 0; i < tmp.size(); ++i)
    print(tmp[i]);
}

void Application_Base::error(const Error & in) const
{
  if (_endline)
  {
    system::print("");
    
    const_cast<Application_Base *>(this)->_endline = false;
  }
  
  error::print(in);
}

void Application_Base::print(const String & in, bool newline) const
{
  if (_separator)
  {
    system::print("");

    const_cast<Application_Base *>(this)->_separator = false;
  }
  
  system::print(in, newline);

  const_cast<Application_Base *>(this)->_endline = ! newline;
}

void Application_Base::print_separator() const
{
  const_cast<Application_Base *>(this)->_separator = true;
}

void Application_Base::command_line(String & in) throw (Error)
{
  DJV_DEBUG("Application_Base::command_line");
  DJV_DEBUG_PRINT("in = " << in);
  
  List<String> tmp;
  
  while (! in.empty())
  {
    String arg;
    
    // Parse options.
    
    if (! string::serialize(&in, &arg))
      throw Error(arg);
    
    if (
      string::compare_no_case("-help", arg) ||
      string::compare_no_case("-h", arg)
    ) {
      exit(EXIT_HELP);
    }
    if (string::compare_no_case("-info", arg))
    {
      exit(EXIT_INFO);
    }
    else if (string::compare_no_case("-about", arg))
    {
      exit(EXIT_ABOUT);
    }

#if defined(DJV_MACOSX)
    else if (string::compare_no_case("-psn_", String(arg, 0, 5)))
    {
      // XXX Ignore Mac OS process id.
    }
#endif

    else
    {
      tmp += string::serialize(arg);
    }
  }
  
  // Put back any options we didn't parse.
    
  in = string::join(tmp, ' ');
}

const String & Application_Base::command_line_name() const
{
  return _command_line_name;
}

const String & Application_Base::command_line_args() const
{
  return _command_line_args;
}

String Application_Base::command_line_help() const
{
  return string::Format(label_command_line_help).
    arg(command_line_separator());
}

String Application_Base::command_line_separator()
{
  return String('-', system::terminal_width());
}

//------------------------------------------------------------------------------
// global()
//------------------------------------------------------------------------------

Application_Base * global()
{
  return _application;
}

//------------------------------------------------------------------------------

}
}

