/*------------------------------------------------------------------------------
 lib/djv_image/application_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

namespace djv_image {
namespace application {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String label_info =
"%%"
"\n"
" OpenGL:\n"
"\n"
" * Renderer: %%\n"
" * Offscreen buffer: %%\n"
" * Vendor string: %%\n"
" * Renderer string: %%\n"
" * Version string: %%\n"
"\n"
" Image I/O plugins:\n"
"\n"
" * Load: %%\n"
" * Save: %%\n";

extern const String label_command_line_help =
"%%\n"
" General Image Options\n"
"\n"
" Time units:\n"
"\n"
"   -time_units (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
" Default speed:\n"
"\n"
"   -speed_default (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
" Show additional options:\n"
"\n"
"   -help_image\n"
"\n"
"%%";

extern const String label_command_line_image =
"%%\n"
" OpenGL Options\n"
"\n"
" Renderer:\n"
"\n"
"   -render (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
" Filter:\n"
"\n"
"   -render_filter (minify) (magnify)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
" Offscreen buffer:\n"
"\n"
"   -render_offscreen (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
"%%";

extern const String label_command_line_io_base =
"%%\n"
" %% Options\n"
"\n";

extern const String label_command_line_io_load =
"%%\n"
" %% Load Options\n"
"\n";

extern const String label_command_line_io_save =
"%%\n"
" %% Save Options\n"
"\n";

extern const String label_command_line_io_option =
" %%:\n"
"\n"
"   %% %% %% (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n";

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Application_Base * global()
{
  return static_cast<Application_Base *>(djv_base::application::global());
}

//------------------------------------------------------------------------------

}
}

