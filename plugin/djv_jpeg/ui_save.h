/*------------------------------------------------------------------------------
 plugin/djv_jpeg/ui_save.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_JPEG_UI_SAVE_H
#define DJV_JPEG_UI_SAVE_H

#include "save.h"
#include <djv_ui/image_io.h>

namespace djv_plugin {
namespace jpeg {
using namespace djv_ui;

//------------------------------------------------------------------------------
//!\class Save_Widget
//
//! Save user-interface plugin.
//------------------------------------------------------------------------------

class Save_Widget : public image_io::Save
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Save_Widget();

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

  DJV_CALLBACK(Save_Widget, quality_callback, int);
  
  void plugin_update();
  void widget_update();

  djv_image::io::Base * _plugin;
  Save_Option _option;
  
  struct _Save_Widget;
  std::auto_ptr<_Save_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  label_quality_group;

//------------------------------------------------------------------------------

}
}

#endif

