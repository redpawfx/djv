/*------------------------------------------------------------------------------
 bin/djv_view/image_display_profile.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_IMAGE_DISPLAY_PROFILE_H
#define DJV_VIEW_IMAGE_DISPLAY_PROFILE_H

#include "image_base.h"
#include <djv_ui/dialog.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::image_display_profile
//
//! Image display profile.
//------------------------------------------------------------------------------

namespace image_display_profile {

//------------------------------------------------------------------------------
//!\class Dialog
//
//! Display profile dialog.
//------------------------------------------------------------------------------

class Dialog : public djv_ui::dialog::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Dialog();
  
  ~Dialog();
  
  void del() { Base::del(); }
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const void *, const image::Display_Profile &);
  void pick(const void *, const image::Display_Profile &);
  void update(const void *, const image::Display_Profile &);
  void del(const void *);
  
  djv_ui::callback::Signal<const image::Display_Profile &> signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void show();
  
  //@}
  
private:

  DJV_CALLBACK(Dialog, widget_callback, const image::Display_Profile &);
  DJV_CALLBACK(Dialog, lut_callback, const File &);
  DJV_CALLBACK(Dialog, brightness_callback, double);
  DJV_CALLBACK(Dialog, contrast_callback, double);
  DJV_CALLBACK(Dialog, saturation_callback, double);
  DJV_CALLBACK(Dialog, levels_in0_callback, double);
  DJV_CALLBACK(Dialog, levels_in1_callback, double);
  DJV_CALLBACK(Dialog, gamma_callback, double);
  DJV_CALLBACK(Dialog, levels_out0_callback, double);
  DJV_CALLBACK(Dialog, levels_out1_callback, double);
  DJV_CALLBACK(Dialog, soft_clip_callback, double);
  DJV_CALLBACK(Dialog, close_callback, bool);

  void widget_update();
  
  const void * _obj;
  image::Display_Profile _value;
  
  struct _Dialog;
  std::auto_ptr<_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  dialog_title,
  dialog_lut,
  dialog_color,
  dialog_brightness,
  dialog_contrast,
  dialog_saturation,
  dialog_levels,
  dialog_levels_in,
  dialog_gamma,
  dialog_levels_out,
  dialog_soft_clip;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global dialog.

Dialog * dialog();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

