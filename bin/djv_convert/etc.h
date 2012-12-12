/*------------------------------------------------------------------------------
 bin/djv_convert/etc.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_CONVERT_ETC_H
#define DJV_CONVERT_ETC_H

#include <djv_image/image.h>
#include <djv_image/time.h>

namespace djv_bin {
namespace convert {
using namespace djv_base;

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

String label_image(const djv_image::Info &, const djv_image::Time &);

extern const String
  label_slate,
  label_timeout,
  label_estimate,
  label_complete,
  label_elapsed,
  label_command_line_help;

extern const String
  error_command_line_input,
  error_command_line_output;

//@}
//------------------------------------------------------------------------------

}
}

#endif

