/*------------------------------------------------------------------------------
 include/djv_image/application.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_APPLICATION_H
#define DJV_IMAGE_APPLICATION_H

#include <djv_image/application_def.h>

#include <djv_image/base.h>
#include <djv_base/application.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::application
//
//! Image application.
//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------
//!\class Application_Base
//
//! A base class for image applications.
//------------------------------------------------------------------------------

class _DJV_IMAGE Application_Base :
  public djv_base::application::Application_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Application_Base(const String & name, int & argc, char ** argv) throw (Error);
  
  virtual ~Application_Base();

  //@}
  //----------------------------------------------------------------------------
  //!\name Information
  //@{
  
  virtual String info() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Command Line
  //@{
  
  void command_line(String &) throw (Error);
  
  virtual String command_line_help() const;
  
  //@}

protected:

  bool _help_default;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE extern const String
  label_info,
  label_command_line_help,
  label_command_line_image,
  label_command_line_io_base,
  label_command_line_io_load,
  label_command_line_io_save,
  label_command_line_io_option;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to the global application instance.

_DJV_IMAGE Application_Base * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

