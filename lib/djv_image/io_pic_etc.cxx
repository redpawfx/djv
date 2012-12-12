/*------------------------------------------------------------------------------
 lib/djv_image/io_pic_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_pic.h"

namespace djv_image {
namespace io_pic {

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

//------------------------------------------------------------------------------

}
}

