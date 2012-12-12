/*------------------------------------------------------------------------------
 include/djv_base/application.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_APPLICATION_H
#define DJV_APPLICATION_H

#include <djv_base/application_def.h>

#include <djv_base/base.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::application
//
//! Base application.
//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Exit value.

enum EXIT
{
  EXIT_DEFAULT,
  EXIT_ERROR,
  EXIT_HELP,
  EXIT_INFO,
  EXIT_ABOUT
};

//@}
//------------------------------------------------------------------------------
//!\class Application_Base
//
//! A base class for applications.
//------------------------------------------------------------------------------

class _DJV_BASE Application_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Application_Base(const String & name, int & argc, char ** argv);

  virtual ~Application_Base();

  //@}
  //----------------------------------------------------------------------------
  //!\name Execution
  //@{
  
  //! Start the application.
  
  virtual int exec();
  
  //! Exit value.

  virtual void exit(EXIT);
  
  EXIT exit() const;

  //@}
  
  //----------------------------------------------------------------------------
  //!\name Information
  //@{
  
  static const String & name();
  const String & path() const;
  const String & path_doc() const;
  
  virtual String info() const;
  virtual String about() const;
  
  //@}
  
  //----------------------------------------------------------------------------
  //!\name Messages
  //@{
  
  virtual void message(const String &) const;
  virtual void error(const Error &) const;
  virtual void print(const String &, bool newline = true) const;
  virtual void print_separator() const;

  //@}
  
  //----------------------------------------------------------------------------
  //!\name Command line
  //@{
  
  void command_line(String &) throw (Error);
  
  const String & command_line_name() const;
  const String & command_line_args() const;
  virtual String command_line_help() const;

  //@}
    
protected:

  static String command_line_separator();

private:

  EXIT _exit;
  static String _name;
  String _path;
  String _path_doc;
  String _command_line_name;
  String _command_line_args;
  bool _endline;
  bool _separator;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE extern const String
  label_info,
  label_about,
  label_command_line_help;

_DJV_BASE extern const String
  error_command_line;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to the global application instance.

_DJV_BASE Application_Base * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

