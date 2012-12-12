/*------------------------------------------------------------------------------
 lib/djv_image/io_sgi_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_sgi.h"

namespace djv_image {
namespace io_sgi {

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
    "RLE";
  
  DJV_ASSERT(data.size() == _COMPRESSION_SIZE);

  return data;
}

const List<String> & label_compression_text()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "RLE (Run Length Encoded)";
  
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

//------------------------------------------------------------------------------

}
}

