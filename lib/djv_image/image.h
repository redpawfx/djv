/*------------------------------------------------------------------------------
 include/djv_image/image.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_H
#define DJV_IMAGE_H

#include <djv_image/image_def.h>

#include <djv_image/color_profile.h>
#include <djv_image/data.h>
#include <djv_image/tag.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::image
//
//! Images.
//------------------------------------------------------------------------------

namespace image {

//------------------------------------------------------------------------------
//!\class Image
//
//! An image.
//------------------------------------------------------------------------------

class _DJV_IMAGE Image : public data::Data
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Image();
  Image(const data::Info & in, const uint8_t * p = 0);

  //@}
  
  Tag tag;
  Color_Profile color_profile;
};

//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Image &, const Image &);

_DJV_IMAGE bool operator != (const Image &, const Image &);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, const Image &);

//@}
//------------------------------------------------------------------------------

}
}

#endif

