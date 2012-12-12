/*------------------------------------------------------------------------------
 bin/djv_info/etc.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_INFO_ETC_H
#define DJV_INFO_ETC_H

#include <djv_image/time.h>

namespace djv_bin {
namespace info {
using namespace djv_base;

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  label_info,
  label_layer_time,
  label_layer_name,
  label_layer_info,
  label_verbose_layer,
  label_verbose_width,
  label_verbose_height,
  label_verbose_aspect,
  label_verbose_pixel,
  label_verbose_start,
  label_verbose_end,
  label_verbose_duration,
  label_verbose_speed,
  label_verbose_tag,
  label_directory,
  label_command_line_help;

extern const String
  error_command_line_input,
  error_directory;

//@}
//------------------------------------------------------------------------------

}
}

#endif

