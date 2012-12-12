/*------------------------------------------------------------------------------
 include/djv_image/image_util.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_UTIL_H
#define DJV_IMAGE_UTIL_H

#include <djv_image/image_util_def.h>

#include <djv_image/image.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::util
//
//! Image utilities.
//------------------------------------------------------------------------------

namespace util {

//------------------------------------------------------------------------------
//!\class Double_Buffer
//
//! Image double buffer utility class.
//------------------------------------------------------------------------------

class _DJV_IMAGE Double_Buffer
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Double_Buffer();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{

  const Image & in() const;

  Image * out();

  void swap();

  void tag(const Tag &);
  void color_profile(const Color_Profile &);

  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{

  Image * operator () ();
  const Image * operator () () const;

  //@}
  
private:

  image::Image _in, _out;
  image::Image * _p0, * _p1;
};

//------------------------------------------------------------------------------

}
}

#endif

