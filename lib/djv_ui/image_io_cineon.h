/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_cineon.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_IO_CINEON_H
#define DJV_UI_IMAGE_IO_CINEON_H

#include <djv_image/io_cineon.h>
#include <djv_ui/image_io.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::image_io_cineon
//
//! Cineon plugin - Image I/O widgets.
//------------------------------------------------------------------------------

namespace image_io_cineon {

//------------------------------------------------------------------------------
//!\class Load
//
//! Load user-interface plugin.
//------------------------------------------------------------------------------

class _DJV_UI Load : public image_io::Load
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Load();
  
  ~Load();

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

  DJV_CALLBACK(Load, color_profile_callback, int);
  DJV_CALLBACK(Load, black_callback, int);
  DJV_CALLBACK(Load, white_callback, int);
  DJV_CALLBACK(Load, gamma_callback, double);
  DJV_CALLBACK(Load, soft_clip_callback, int);
  DJV_CALLBACK(Load, convert_callback, int);
  DJV_CALLBACK(Load, callback, bool);
  
  void plugin_update();
  void widget_update();
  
  djv_image::io::Base * _plugin;
  djv_image::io_cineon::Load_Option _option;
  
  struct _Load;
  std::auto_ptr<_Load> _p;
};

//------------------------------------------------------------------------------
//!\class Save
//
//! Save user-interface plugin.
//------------------------------------------------------------------------------

class _DJV_UI Save : public image_io::Save
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Save();
  
  ~Save();

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

  DJV_CALLBACK(Save, color_profile_callback, int);
  DJV_CALLBACK(Save, black_callback, int);
  DJV_CALLBACK(Save, white_callback, int);
  DJV_CALLBACK(Save, gamma_callback, double);
  DJV_CALLBACK(Save, callback, bool);
  
  void plugin_update();
  void widget_update();

  djv_image::io::Base * _plugin;
  djv_image::io_cineon::Save_Option _option;
  
  struct _Save;
  std::auto_ptr<_Save> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  label_color_profile_group,
  label_film_print_group,
  label_film_print_black,
  label_film_print_white,
  label_film_print_gamma,
  label_film_print_soft_clip,
  label_convert_group;

//@}
//------------------------------------------------------------------------------

}
}

#endif

