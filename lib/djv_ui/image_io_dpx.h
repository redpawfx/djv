/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_dpx.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_IO_DPX_H
#define DJV_UI_IMAGE_IO_DPX_H

#include <djv_image/io_dpx.h>
#include <djv_ui/image_io.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::image_io_dpx
//
//! DPX plugin - Image I/O widgets.
//------------------------------------------------------------------------------

namespace image_io_dpx {

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
  djv_image::io_dpx::Load_Option _option;

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
  DJV_CALLBACK(Save, version_callback, int);
  DJV_CALLBACK(Save, type_callback, int);
  DJV_CALLBACK(Save, endian_callback, int);
  DJV_CALLBACK(Save, callback, bool);
  
  void plugin_update();
  void widget_update();

  djv_image::io::Base * _plugin;
  djv_image::io_dpx::Save_Option _option;
  
  struct _Save;
  std::auto_ptr<_Save> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  label_version_group,
  label_type_group,
  label_endian_group;

//------------------------------------------------------------------------------

}
}

#endif

