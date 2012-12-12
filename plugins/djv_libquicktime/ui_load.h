/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/ui_load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_LIBQUICKTIME_UI_LOAD_H
#define DJV_LIBQUICKTIME_UI_LOAD_H

#include "load.h"
#include <djv_ui/image_io.h>

namespace djv_plugin {
namespace libquicktime {

using namespace djv_ui;

//------------------------------------------------------------------------------
//!\class Load_Widget
//
//! Load user-interface plugin.
//------------------------------------------------------------------------------

class Load_Widget : public djv_ui::image_io::Load
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Load_Widget();
  
  ~Load_Widget();

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{

  Plugin * copy() const;
  String name() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(djv_image::io::Base *);
  
  //@}
  
private:

  DJV_CALLBACK(Load_Widget, start_frame_callback, int);

  void plugin_update();
  void widget_update();
  
  djv_image::io::Base * _plugin;
  Load_Option _option;
  
  struct _Load_Widget;
  std::auto_ptr<_Load_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  label_start_frame_group;

//------------------------------------------------------------------------------

}
}

#endif

