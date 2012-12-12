/*------------------------------------------------------------------------------
 include/djv_image/color_profile.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_COLOR_PROFILE_H
#define DJV_IMAGE_COLOR_PROFILE_H

#include <djv_image/color_profile_def.h>

#include <djv_image/data.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::color_profile
//
//! Color profiles.
//------------------------------------------------------------------------------

namespace color_profile {

//------------------------------------------------------------------------------
//!\name Color Profile
//------------------------------------------------------------------------------
//@{

//! Exposure values.

struct _DJV_IMAGE Exposure
{
  Exposure() :
    value(0),
    defog(0),
    knee_low(0),
    knee_high(5)
  {}

  double value;
  double defog;
  double knee_low, knee_high;
};

//! Color profile.

enum PROFILE
{
  RAW,
  GAMMA,
  LUT,
  EXPOSURE,
  
  _PROFILE_SIZE
};

//! Color profile.

struct _DJV_IMAGE Color_Profile
{
  Color_Profile() :
    type(RAW),
    gamma(2.2)
  {}

  PROFILE type;
  
  double gamma;
  
  Data lut;
  
  Exposure exposure;
};

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Exposure &, const Exposure &);
_DJV_IMAGE bool operator == (const Color_Profile &, const Color_Profile &);

_DJV_IMAGE bool operator != (const Exposure &, const Exposure &);
_DJV_IMAGE bool operator != (const Color_Profile &, const Color_Profile &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, Exposure &) throw (String);
_DJV_IMAGE String & operator >> (String &, PROFILE &) throw (String);

_DJV_IMAGE String & operator << (String &, const Exposure &);
_DJV_IMAGE String & operator << (String &, PROFILE);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_profile();

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, PROFILE &);

//@}
//------------------------------------------------------------------------------

}
}

#endif

