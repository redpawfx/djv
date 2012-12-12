/*------------------------------------------------------------------------------
 lib/djv_image/io_ppm_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_ppm.h"

namespace djv_image {
namespace io_ppm {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())
_DJV_STRING_OPERATOR_LABEL(DATA, label_data())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_type()
{
  static const List<String> data = List<String>() <<
    "Auto" <<
    "U1";

  DJV_ASSERT(data.size() == _TYPE_SIZE);
  
  return data;
}

const List<String> & label_data()
{
  static const List<String> data = List<String>() <<
    "ASCII" <<
    "Binary";

  DJV_ASSERT(data.size() == _DATA_SIZE);

  return data;
}

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "File type" <<
    "File data";

  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);

  return data;
}

//------------------------------------------------------------------------------

}
}
