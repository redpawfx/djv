/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_lut.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_IO_LUT_H
#define DJV_UI_IMAGE_IO_LUT_H

#include <djv_image/io_lut.h>
#include <djv_ui/image_io.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::image_io_lut
//
//! LUT image I/O widgets.
//------------------------------------------------------------------------------

namespace image_io_lut {

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

  DJV_CALLBACK(Load, format_callback, int);
  DJV_CALLBACK(Load, type_callback, int);
  
  void plugin_update();
  void widget_update();

  djv_image::io::Base * _plugin;
  djv_image::io_lut::Load_Option _option;
  
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

  DJV_CALLBACK(Save, format_callback, int);
  
  void plugin_update();
  void widget_update();

  djv_image::io::Base * _plugin;
  djv_image::io_lut::Save_Option _option;
  
  struct _Save;
  std::auto_ptr<_Save> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  label_format_group,
  label_type_group;

//@}
//------------------------------------------------------------------------------

}
}

#endif

