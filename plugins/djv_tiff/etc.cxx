/*------------------------------------------------------------------------------
 plugin/djv_tiff/etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "base.h"

#include "load.h"
#include "save.h"

namespace djv_plugin {
namespace tiff {

//------------------------------------------------------------------------------
// tiff_init()
//------------------------------------------------------------------------------

void tiff_init()
{
  TIFFSetErrorHandler(0);
  TIFFSetWarningHandler(0);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(COMPRESSION, label_compression())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_compression()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "RLE" <<
    "LZW";
  
  DJV_ASSERT(data.size() == _COMPRESSION_SIZE);
  
  return data;
}

const List<String> & label_compression_text()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "RLE (Run Length Encoded)" <<
    "LZW (Lempel-Ziv-Welch)";
  
  DJV_ASSERT(data.size() == _COMPRESSION_SIZE);
  
  return data;
}

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "Compression";
  
  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);
  
  return data;
}

extern const String
  label_compression_group = "Compression",
  label_channels_group = "Channels";

//------------------------------------------------------------------------------

}
}

