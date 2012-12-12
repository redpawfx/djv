/*------------------------------------------------------------------------------
 bin/djv_view/cache_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "cache.h"

namespace djv_bin {
namespace view {
namespace cache {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(CACHE, label_cache())

//------------------------------------------------------------------------------
// Defaults
//------------------------------------------------------------------------------

const List<int> & default_size()
{
  static const List<int> data = List<int>() <<
    200 <<
    400 <<
    600 <<
    800 <<
    1000 <<
    2000 <<
    3000 <<
    4000 <<
    5000 <<
    6000 <<
    7000 <<
    8000;

  return data;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_size()
{
  static const List<String> data = List<String>() <<
    "200" <<
    "400" <<
    "600" <<
    "800" <<
    "1000" <<
    "2000" <<
    "3000" <<
    "4000" <<
    "5000" <<
    "6000" <<
    "7000" <<
    "8000";

  return data;
}

const List<String> & label_cache()
{
  static const List<String> data = List<String>() <<
    "LRU" <<
    "LRU + Playback";
  
  DJV_ASSERT(data.size() == _CACHE_SIZE);
  
  return data;
}

const List<String> & label_cache_text()
{
  static const List<String> data = List<String>() <<
    "Remove least recently used frames" <<
    "Remove frames previous to the current frame";
  
  DJV_ASSERT(data.size() == _CACHE_SIZE);
  
  return data;
}

//------------------------------------------------------------------------------

}
}
}

