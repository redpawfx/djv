/*------------------------------------------------------------------------------
 lib/djv_ui/application.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_APPLICATION_H
#define DJV_UI_APPLICATION_H

#include <djv_ui/application_def.h>

#include <djv_ui/window.h>
#include <djv_image/application.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace application
//
//! User-interface application.
//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------
//!\class Application_Base
//
//! A base class for user-interface applications.
//------------------------------------------------------------------------------

class _DJV_UI Application_Base :
  public djv_image::application::Application_Base,
  public Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Application_Base(const String & name, int & argc, char ** argv) throw (Error);
  
  virtual ~Application_Base();

  //@}
  //----------------------------------------------------------------------------
  //!\name Execution
  //@{

  virtual int exec();
  
  virtual void exit(
    djv_base::application::EXIT = djv_base::application::EXIT_DEFAULT
  );

  //! Has the user-interface started yet?
  
  void is_valid(bool);
    
  bool is_valid() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Information
  //@{
  
  virtual String info() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Messages
  //@{

  virtual void message(const String &) const;
  
  virtual void error(const Error &) const;

  virtual void help() const;

  //@}
  
private:

  bool _is_valid;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  label_info;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to the global application instance.

_DJV_UI Application_Base * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

