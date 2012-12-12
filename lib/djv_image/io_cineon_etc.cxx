/*------------------------------------------------------------------------------
 lib/djv_image/io_cineon_etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_cineon.h"

namespace djv_image {
namespace io_cineon {

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Film_Print & a, const Film_Print & b)
{
  return
    a.black == b.black &&
    a.white == b.white &&
    a.gamma == b.gamma;
}

bool operator == (const Film_Unprint & a, const Film_Unprint & b)
{
  return
    a.black == b.black &&
    a.white == b.white &&
    a.gamma == b.gamma &&
    a.soft_clip == b.soft_clip;
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

String & operator >> (String & in, Film_Print & out) throw (String)
{
  return in >>
    out.black >>
    out.white >>
    out.gamma;
}

String & operator << (String & out, const Film_Print & in)
{
  return out <<
    in.black <<
    in.white <<
    in.gamma;
}

String & operator >> (String & in, Film_Unprint & out) throw (String)
{
  return in >>
    out.black >>
    out.white >>
    out.gamma >>
    out.soft_clip;
}

String & operator << (String & out, const Film_Unprint & in)
{
  return out <<
    in.black <<
    in.white <<
    in.gamma <<
    in.soft_clip;
}

_DJV_STRING_OPERATOR_LABEL(COLOR_PROFILE, label_color_profile())
_DJV_STRING_OPERATOR_LABEL(CONVERT, label_convert())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_tag()
{
  static const List<String> data = List<String>() <<
    "Source offset" <<
    "Source file" <<
    "Source time" <<
    "Source input device" <<
    "Source input model" <<
    "Source input serial" <<
    "Source input pitch" <<
    "Source gamma" <<
    "Film format" <<
    "Film frame" <<
    "Film frame rate" <<
    "Film frame ID" <<
    "Film slate";
  
  DJV_ASSERT(data.size() == _TAG_SIZE);
  
  return data;
}

const List<String> & label_color_profile()
{
  static const List<String> data = List<String>() <<
    "Auto" <<
    "None" <<
    "Film print";
  
  DJV_ASSERT(data.size() == _COLOR_PROFILE_SIZE);

  return data;
}

const List<String> & label_convert()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "U8" <<
    "U16";
  
  DJV_ASSERT(data.size() == _CONVERT_SIZE);

  return data;
}

const List<String> & label_load_option()
{
  static const List<String> data = List<String>() <<
    "Color profile" <<
    "Film print" <<
    "Convert";
  
  DJV_ASSERT(data.size() == _LOAD_OPTION_SIZE);

  return data;
}

const List<String> & label_save_option() 
{
  static const List<String> data = List<String>() <<
    "Color profile" <<
    "Film print";
  
  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);

  return data;
}

extern const String
  label_film_print_load = "Black, white, gamma, soft clip",
  label_film_print_save = "Black, white, gamma";

//------------------------------------------------------------------------------

}
}

