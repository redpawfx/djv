/*------------------------------------------------------------------------------
 plugin/djv_openexr/ui_load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_OPENEXR_UI_LOAD_H
#define DJV_OPENEXR_UI_LOAD_H

#include "load.h"
#include <djv_ui/image_io.h>

namespace djv_plugin {
namespace openexr {
using namespace djv_ui;

//------------------------------------------------------------------------------
//!\class Load_Widget
//
//! Load user-interface plugin.
//------------------------------------------------------------------------------

class Load_Widget : public image_io::Load
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
  
  plugin::Plugin * copy() const;
  
  String name() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(djv_image::io::Base *);
  
  //@}
  
private:

  DJV_CALLBACK(Load_Widget, color_profile_callback, int);
  DJV_CALLBACK(Load_Widget, gamma_callback, double);
  DJV_CALLBACK(Load_Widget, exposure_callback, double);
  DJV_CALLBACK(Load_Widget, exposure_defog_callback, double);
  DJV_CALLBACK(Load_Widget, exposure_knee_low_callback, double);
  DJV_CALLBACK(Load_Widget, exposure_knee_high_callback, double);
  DJV_CALLBACK(Load_Widget, channels_callback, int);
  DJV_CALLBACK(Load_Widget, callback, bool);
  
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
  label_color_profile_group,
  label_gamma_group,
  label_gamma,
  label_exposure_group,
  label_exposure,
  label_exposure_defog,
  label_exposure_knee_low,
  label_exposure_knee_high,
  label_channels_group;

//@}
//------------------------------------------------------------------------------

}
}

#endif

