/*------------------------------------------------------------------------------
 lib/djv_ui/image_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_PREFS_H
#define DJV_UI_IMAGE_PREFS_H

#include <djv_ui/image_prefs_def.h>

#include <djv_image/gl_data.h>
#include <djv_image/time.h>
#include <djv_ui/widget.h>

namespace djv_ui {
namespace image {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! Image preferences.
//------------------------------------------------------------------------------

class _DJV_UI Prefs
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Image Filter
  //@{
  
  void filter(const djv_image::gl_data::Filter &);
  
  const djv_image::gl_data::Filter & filter() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Speed
  //@{
  
  void speed(djv_image::time::FPS);
  
  djv_image::time::FPS speed() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Units
  //@{
  
  void units(djv_image::time::UNITS);
  
  djv_image::time::UNITS units() const;
  
  //@}
};

//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

_DJV_UI Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}

#endif

