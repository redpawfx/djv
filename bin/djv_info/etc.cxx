/*------------------------------------------------------------------------------
 bin/djv_info/etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "etc.h"

namespace djv_bin {
namespace info {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const String
  label_info = "%%x%%:%% %% %%@%%",
  label_layer_time = "%%@%%",
  label_layer_name = "  %%. %%",
  label_layer_info = "%%x%%:%% %%",
  label_verbose_layer = "Layer = %%",
  label_verbose_width = "  Width = %%",
  label_verbose_height = "  Height = %%",
  label_verbose_aspect = "  Aspect = %%",
  label_verbose_pixel = "  Pixel = %%",
  label_verbose_start = "Start = %%",
  label_verbose_end = "End = %%",
  label_verbose_duration = "Duration = %%",
  label_verbose_speed = "Speed = %%",
  label_verbose_tag = "Tag %% = %%",
  label_directory = "%%:";

const String
  error_command_line_input = "Input",
  error_directory = "Cannot open directory: %%";

//------------------------------------------------------------------------------

}
}
