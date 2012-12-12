/*------------------------------------------------------------------------------
 bin/djv_view/file_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "file_group.h"

#include "file_prefs.h"
#include "file_save.h"

namespace djv_bin {
namespace view {
namespace file {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  menu_title = "File",
  menu_open = "&Open",
  menu_recent = "&Recent",
  menu_reload = "Re&load",
  menu_close = "Clos&e",
  menu_save = "&Save",
  menu_save_frame = "Save &Frame",
  menu_seq_auto = "&Auto Sequence",
  menu_layer = "La&yer",
  menu_layer_prev = "Layer Previous",
  menu_layer_next = "Layer Next",
  menu_proxy = "Pro&xy Scale",
  menu_cache = "&Memory Cache",
  menu_cache_clear = "Clear Memory Cache";

extern const String
  tooltip_open = "Open a file\n\nShortcut: %%",
  tooltip_reload = "Reload the current file\n\nShortcut: %%",
  tooltip_close = "Close the current file\n\nShortcut: %%";

extern const String
  prefs_title = "File",
  prefs_group = "File Options",
  prefs_seq_auto =
  "Automatically detect sequences when opening files",
  prefs_command_line_combine =
  "Combine command line arguments into a single input",
  prefs_proxy_group = "Proxy Scale",
  prefs_proxy_text =
  "Reduce image resolution to cache more frames in memory.",
  prefs_cache_group = "Memory Cache",
  prefs_cache = "Enable",
  prefs_cache_text =
  "Stores frames in memory. When the cache is disabled frames are streamed\n"
  "from disk.",
  prefs_cache_size = "Size (megabytes):",
  prefs_cache_type = "Type",
  prefs_cache_display = "Display cached frames in timeline";

extern const String
  dialog_save = "Saving \"%%\":",
  dialog_save_check = "Overwrite existing file \"%%\"?";

//------------------------------------------------------------------------------

}
}
}

