/*------------------------------------------------------------------------------
 plugin/djv_jpeg/etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "base.h"

#include "load.h"
#include "save.h"

namespace djv_plugin {
namespace jpeg {

//------------------------------------------------------------------------------
// djv_jpeg_error()
//------------------------------------------------------------------------------

extern "C" {

void djv_jpeg_error(libjpeg::j_common_ptr in)
{
  Jpeg_Error * error = (Jpeg_Error *)in->err;
  
  in->err->format_message(in, error->msg);
  
  ::longjmp(error->jump, 1);
}

}

//------------------------------------------------------------------------------
// djv_jpeg_djv_jpeg_warning()
//------------------------------------------------------------------------------

extern "C" {

void djv_jpeg_warning(libjpeg::j_common_ptr in, int level)
{
  if (level > 0)
    return;

  Jpeg_Error * error = (Jpeg_Error *)in->err;
  
  in->err->format_message(in, error->msg);
  
  ::longjmp(error->jump, 1);
}

}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "Quality";
  
  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);
  
  return data;
}

extern const String
  label_quality = "0 (lowest quality) - 100 (highest quality)";

extern const String
  label_quality_group = "Quality";

//------------------------------------------------------------------------------

}
}

