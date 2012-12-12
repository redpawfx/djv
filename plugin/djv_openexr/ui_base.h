/*------------------------------------------------------------------------------
 plugin/djv_openexr/ui_base.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_OPENEXR_UI_BASE_H
#define DJV_OPENEXR_UI_BASE_H

#include "base.h"
#include <djv_ui/image_io.h>

namespace djv_plugin {
namespace openexr {
using namespace djv_ui;

//------------------------------------------------------------------------------
//!\class Base_Widget
//
//! Base user-interface plugin.
//------------------------------------------------------------------------------

class Base_Widget : public image_io::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Base_Widget();
  
  ~Base_Widget();

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{
  
  plugin::Plugin * copy() const;
  
  String name() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(djv_image::io::Base *);
  
  //@}
  
private:

  DJV_CALLBACK(Base_Widget, thread_count_callback, int);
  DJV_CALLBACK(Base_Widget, callback, bool);
  
  void plugin_update();
  void widget_update();
  
  djv_image::io::Base * _plugin;
  Base_Option _option;
  
  struct _Base_Widget;
  std::auto_ptr<_Base_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  label_thread_count_group,
  label_thread_count_text;

//@}
//------------------------------------------------------------------------------

}
}

#endif

