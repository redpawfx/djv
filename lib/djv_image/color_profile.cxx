/*------------------------------------------------------------------------------
 lib/djv_image/color_profile.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "color_profile.h"

namespace djv_image {
namespace color_profile {

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Exposure & a, const Exposure & b)
{
  return
    a.value == b.value &&
    a.defog == b.defog &&
    a.knee_low == b.knee_low &&
    a.knee_high == b.knee_high;
}

bool operator != (const Exposure & a, Exposure & b)
{
  return ! (a == b);
}

bool operator == (const Color_Profile & a, const Color_Profile & b)
{
  return
    a.type == b.type &&
    a.gamma == b.gamma &&
    a.lut == b.lut &&
    a.exposure == b.exposure;
}

bool operator != (const Color_Profile & a, const Color_Profile & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

String & operator >> (String & in, Exposure & out) throw (String)
{
  return in >> out.value >> out.defog >> out.knee_low >> out.knee_high;
}

String & operator << (String & out, const Exposure & in)
{
  return out << in.value << in.defog << in.knee_low << in.knee_high;
}

_DJV_STRING_OPERATOR_LABEL(PROFILE, label_profile())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_profile()
{
  static const List<String> data = List<String>() <<
    "Raw" <<
    "Gamma" <<
    "LUT" <<
    "Exposure";

  DJV_ASSERT(data.size() == _PROFILE_SIZE);

  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, PROFILE & in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------

}
}
