/*------------------------------------------------------------------------------
 bin/djv_convert/etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "etc.h"

namespace djv_bin {
namespace convert {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

String label_image(const djv_image::Info & in, const djv_image::Time & time)
{
  return string::Format("%%x%%:%% %% %%@%%").
    arg(in.size.x).
    arg(in.size.y).
    arg(aspect(in.size), 0, -2).
    arg(string::label(in.pixel)).
    arg(djv_image::time::units_to_string(
      static_cast<int>(time.list.size()), time.speed)).
    arg(djv_image::time::speed_to_float(time.speed));
}

const String
  label_slate = "Slating...",
  label_timeout = "Timeout...",
  label_estimate = "[%%%] Estimated = %% (%% Frames/Second)",
  label_complete = "[100%] ",
  label_elapsed = "Elapsed = %%";

const String
  error_command_line_input = "Input",
  error_command_line_output = "Output";

//------------------------------------------------------------------------------

}
}

