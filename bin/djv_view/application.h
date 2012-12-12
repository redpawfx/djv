/*------------------------------------------------------------------------------
 bin/djv_view/application.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_APP
#define DJV_VIEW_APP

#include "playback_base.h"
#include "view_base.h"
#include <djv_ui/application.h>
#include <djv_image/data.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::application
//
//! Application.
//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------
//!\class Application
//
//! The djv_view application.
//------------------------------------------------------------------------------

class Application : public djv_ui::application::Application_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Application(int & argc, char ** argv);
  
  ~Application();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Command Line
  //@{
  
  void command_line(String &) throw (Error);
  
  String command_line_help() const;

  //@}
  
private:
  
  window::Window * window() const;
  
  struct Command_Line
  {
    std::auto_ptr<bool> file_seq_auto;
    std::auto_ptr<int> file_layer;
    std::auto_ptr<djv_image::data::PROXY> file_proxy;
    std::auto_ptr<bool> file_cache;
    std::auto_ptr<bool> window_fullscreen;
    std::auto_ptr<playback::PLAYBACK> playback;
    std::auto_ptr<int> playback_frame;
    std::auto_ptr<djv_image::time::Speed> playback_speed;
  } _command_line;

  List<String> _input;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  label_command_line_help;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to the global application instance.

Application * global();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

