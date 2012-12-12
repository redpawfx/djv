/*------------------------------------------------------------------------------
 bin/djv_view/image_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_IMAGE_PREFS_H
#define DJV_VIEW_IMAGE_PREFS_H

#include "image_base.h"
#include <djv_ui/widget.h>

namespace djv_bin {
namespace view {
namespace image {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! Image preferences.
//------------------------------------------------------------------------------

class Prefs : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Frame Store
  //@{
  
  void frame_store_file_reload(bool);
  
  bool frame_store_file_reload() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Transforms
  //@{
  
  void mirror(const V2b &);
  void scale(image::SCALE);
  void rotate(image::ROTATE);
  
  const V2b & mirror() const;
  image::SCALE scale() const;
  image::ROTATE rotate() const;
  
  djv_ui::callback::Signal<const V2b &> mirror_signal;
  djv_ui::callback::Signal<image::SCALE> scale_signal;
  djv_ui::callback::Signal<image::ROTATE> rotate_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Color Profile
  //@{
  
  void color_profile(bool);
  
  bool color_profile() const;
  
  djv_ui::callback::Signal<bool> color_profile_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Display Profile
  //@{
  
  void display_profile(int);
  void display_profile_list(const List<Display_Profile> &);
  
  int display_profile() const;
  Display_Profile display_profile_value() const;
  const List<Display_Profile> & display_profile_list() const;
  List<String> display_profile_name_list() const;
  
  djv_ui::callback::Signal<int> display_profile_signal;
  djv_ui::callback::Signal<const Display_Profile &>
    display_profile_value_signal;
  djv_ui::callback::Signal<bool> display_profile_list_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Channel
  //@{
  
  void channel(image::CHANNEL);
  
  image::CHANNEL channel() const;
  
  djv_ui::callback::Signal<image::CHANNEL> channel_signal;

  //@}
  
private:

  bool _frame_store_file_reload;
  V2b _mirror;
  image::SCALE _scale;
  image::ROTATE _rotate;
  bool _color_profile;
  int _display_profile;
  List<Display_Profile> _display_profile_list;
  image::CHANNEL _channel;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! Image preferences widget.
//------------------------------------------------------------------------------

class Prefs_Widget : public djv_ui::Widget
{
public:

  Prefs_Widget();
  
  ~Prefs_Widget();
  
private:

  DJV_CALLBACK(Prefs_Widget, frame_store_file_reload_callback, bool);
  DJV_CALLBACK(Prefs_Widget, mirror_h_callback, bool);
  DJV_CALLBACK(Prefs_Widget, mirror_v_callback, bool);
  DJV_CALLBACK(Prefs_Widget, scale_callback, int);
  DJV_CALLBACK(Prefs_Widget, rotate_callback, int);
  DJV_CALLBACK(Prefs_Widget, color_profile_callback, bool);
  DJV_CALLBACK(Prefs_Widget, display_profile_callback, int);
  DJV_CALLBACK(Prefs_Widget, display_profile_list_callback, bool);
  DJV_CALLBACK(Prefs_Widget, channel_callback, int);
  
  void widget_update();
  
  struct _Prefs_Widget;
  std::auto_ptr<_Prefs_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  prefs_title,
  prefs_group,
  prefs_frame_store_file_reload,
  prefs_xform_group,
  prefs_xform_mirror,
  prefs_xform_mirror_h,
  prefs_xform_mirror_v,
  prefs_xform_scale,
  prefs_xform_rotate,
  prefs_color_profile_group,
  prefs_color_profile,
  prefs_display_profile_group,
  prefs_display_profile_none,
  prefs_channel_group;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

