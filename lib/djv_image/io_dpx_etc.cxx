/*------------------------------------------------------------------------------
 lib/djv_image/io_dpx_etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_dpx.h"

namespace djv_image {
namespace io_dpx {

//------------------------------------------------------------------------------
// Load_Option
//------------------------------------------------------------------------------

Load_Option::Load_Option() :
  color_profile(io_cineon::COLOR_PROFILE_AUTO),
  convert(io_cineon::CONVERT_NONE)
{}

//------------------------------------------------------------------------------
// Save_Option
//------------------------------------------------------------------------------

Save_Option::Save_Option() :
  color_profile(io_cineon::COLOR_PROFILE_FILM_PRINT),
  version(VERSION_2_0),
  type(TYPE_U10),
  endian(ENDIAN_MSB)
{}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(VERSION, label_version())
_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())
_DJV_STRING_OPERATOR_LABEL(ENDIAN, label_endian())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_tag()
{
  static const List<String> data = List<String>() <<
    "Source offset" <<
    "Source center" <<
    "Source size" <<
    "Source file" <<
    "Source time" <<
    "Source input device" <<
    "Source input serial" <<
    "Source border" <<
    "Source pixel aspect" <<
    "Source scan size" <<
    "Film format" <<
    "Film frame" <<
    "Film sequence" <<
    "Film hold" <<
    "Film frame rate" <<
    "Film shutter" <<
    "Film frame ID" <<
    "Film slate" <<
    "TV interlace" <<
    "TV field" <<
    "TV video signal" <<
    "TV sample rate" <<
    "TV frame rate" <<
    "TV time offset" <<
    "TV gamma" <<
    "TV black level" <<
    "TV black gain" <<
    "TV break point" <<
    "TV white level" <<
    "TV integration times";
  
  DJV_ASSERT(data.size() == _TAG_SIZE);

  return data;
}

const List<String> & label_version()
{
  static const List<String> data = List<String>() <<
    "1.0" <<
    "2.0";
  
  DJV_ASSERT(data.size() == _VERSION_SIZE);

  return data;
}

const List<String> & label_type()
{
  static const List<String> data = List<String>() <<
    "Auto" <<
    "U10";
  
  DJV_ASSERT(data.size() == _TYPE_SIZE);

  return data;
}

const List<String> & label_type_text()
{
  static const List<String> data = List<String>() <<
    "Automatic" <<
    "10-bits";
  
  DJV_ASSERT(data.size() == _TYPE_SIZE);

  return data;
}

const List<String> & label_endian()
{
  static const List<String> data = List<String>() <<
    "Auto" <<
    memory::label_endian();
  
  DJV_ASSERT(data.size() == _ENDIAN_SIZE);

  return data;
}

const List<String> & label_endian_text()
{
  static const List<String> data = List<String>() <<
    "Use the current machine's endian" <<
    memory::label_endian_text();
  
  DJV_ASSERT(data.size() == _ENDIAN_SIZE);

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
    "Film print" <<
    "File version" <<
    "File type" <<
    "File endian";
  
  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);

  return data;
}

//------------------------------------------------------------------------------

}
}

